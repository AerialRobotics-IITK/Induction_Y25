#!/usr/bin/env python3
"""Vision + control node for iris_2.

Part 1 — OpenCV pipeline:
  * Subscribe to iris_2's gimbal camera, detect DICT_4X4_50 ArUco markers
    (leader id=0, landing pad id=1), estimate pose with the correct
    physical marker size per id, and annotate the live frame.

Part 2 — Gimbal + flight control:
  * Run a 20 Hz control loop on a timer (decoupled from the camera
    callback) that reads the latest cached pose and emits
    geometry_msgs/Twist on the MAVROS velocity setpoint topic.
  * Two cascades of PIDs:
       - body: distance->linear.x, lateral->angular.z (yaw onto target),
         altitude->linear.z.
       - gimbal: yaw + pitch incrementally integrated from the marker's
         angular offset, published as a mavros_msgs/MountControl message
         (lazy import — node still runs without mavros_msgs installed).
  * Subtask 1 — 5s loss timer fires "search" behaviour (hover + slow
    yaw); >5s is reported as mission failure.
  * Subtask 2 — control inputs come purely from tvecs; no
    /mavros/global_position or odometry subscriptions exist in this node.
  * Subtask 3 — std_msgs/Bool on /swarm_tracker/start_landing flips the
    state machine into LANDING, where the gimbal + body track the pad
    marker (id=1) and descend at a controlled rate until touchdown.
"""

from dataclasses import dataclass
from enum import Enum
from typing import Dict, Optional

import cv2
import cv2.aruco as aruco
import numpy as np
import rclpy
from cv_bridge import CvBridge
from geometry_msgs.msg import Twist
from rclpy.node import Node
from sensor_msgs.msg import Image
from std_msgs.msg import Bool


LEADER_MARKER_ID = 0
LANDING_PAD_ID = 1

# Physical marker side length in metres (from iris_with_aruco/model.sdf
# visual = 0.3m, and the landing-pad visual in task_6.sdf = 1.0m).
MARKER_SIZES_M: Dict[int, float] = {
    LEADER_MARKER_ID: 0.30,
    LANDING_PAD_ID: 1.00,
}

# Lose-of-sight grace period before we declare the mission compromised.
MARKER_LOST_WARN_S = 5.0
# Below this loss duration the controller still expects the marker to
# reappear and just holds position; above it, the search behaviour kicks
# in (slow yaw + hover) so we have a chance of re-acquiring before 5s.
MARKER_LOST_SEARCH_S = 1.0


class FollowerState(Enum):
    FOLLOW = 'follow'           # subtask 1/2: track the leader at target distance.
    SEARCH = 'search'           # leader missing — hover + yaw to re-acquire.
    LANDING = 'landing'         # subtask 3: descend onto the pad.
    LANDED = 'landed'           # touchdown reached; output zero velocity.


@dataclass
class MarkerObservation:
    """Latest pose of a tracked marker, expressed in the camera frame."""

    marker_id: int
    tvec: np.ndarray   # shape (3,) -> [x, y, z] in metres
    rvec: np.ndarray   # shape (3,) Rodrigues rotation
    pixel_center: tuple  # (u, v) in image coords
    stamp_ns: int

    @property
    def distance(self) -> float:
        return float(self.tvec[2])

    @property
    def x_offset(self) -> float:
        return float(self.tvec[0])

    @property
    def y_offset(self) -> float:
        return float(self.tvec[1])


class PID:
    """Minimal PID with integral clamp and output saturation."""

    def __init__(self, kp: float, ki: float, kd: float,
                 i_clamp: Optional[float] = None,
                 out_clamp: Optional[float] = None):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.i_clamp = i_clamp
        self.out_clamp = out_clamp
        self._i = 0.0
        self._prev_err: Optional[float] = None
        self._prev_t: Optional[float] = None

    def reset(self) -> None:
        self._i = 0.0
        self._prev_err = None
        self._prev_t = None

    def step(self, err: float, t_s: float) -> float:
        dt = 0.0 if self._prev_t is None else max(1e-3, t_s - self._prev_t)
        d_err = 0.0
        if self._prev_err is not None and dt > 0:
            d_err = (err - self._prev_err) / dt

        self._i += err * dt
        if self.i_clamp is not None:
            self._i = max(-self.i_clamp, min(self.i_clamp, self._i))

        out = self.kp * err + self.ki * self._i + self.kd * d_err
        if self.out_clamp is not None:
            out = max(-self.out_clamp, min(self.out_clamp, out))

        self._prev_err = err
        self._prev_t = t_s
        return out


class FollowerNode(Node):
    """ROS 2 node that runs the ArUco pipeline + PID control for iris_2."""

    CONTROL_HZ = 20.0

    def __init__(self):
        super().__init__('follower_node')

        # ------------------------------------------------------------------
        # Parameters
        # ------------------------------------------------------------------
        self.declare_parameter('image_topic', '/camera/image_raw')
        self.declare_parameter('cmd_vel_topic',
                               '/iris_2/mavros/setpoint_velocity/cmd_vel_unstamped')
        self.declare_parameter('mount_control_topic',
                               '/iris_2/mavros/mount_control/command')
        self.declare_parameter('landing_trigger_topic',
                               '/swarm_tracker/start_landing')
        self.declare_parameter('show_window', True)

        # Default intrinsics: 640x480 pinhole, ~60deg HFOV.
        # Replace with the actual camera_info values from your gimbal sensor.
        self.declare_parameter('fx', 530.0)
        self.declare_parameter('fy', 530.0)
        self.declare_parameter('cx', 320.0)
        self.declare_parameter('cy', 240.0)
        self.declare_parameter('target_distance_m', 2.0)
        self.declare_parameter('landing_touchdown_m', 0.40)

        image_topic = self.get_parameter('image_topic').value
        cmd_vel_topic = self.get_parameter('cmd_vel_topic').value
        mount_topic = self.get_parameter('mount_control_topic').value
        landing_topic = self.get_parameter('landing_trigger_topic').value
        self.show_window = self.get_parameter('show_window').value
        self.target_distance = float(self.get_parameter('target_distance_m').value)
        self.touchdown_m = float(self.get_parameter('landing_touchdown_m').value)

        fx = float(self.get_parameter('fx').value)
        fy = float(self.get_parameter('fy').value)
        cx = float(self.get_parameter('cx').value)
        cy = float(self.get_parameter('cy').value)
        self.camera_matrix = np.array(
            [[fx, 0.0, cx],
             [0.0, fy, cy],
             [0.0, 0.0, 1.0]],
            dtype=np.float64,
        )
        self.dist_coeffs = np.zeros((5, 1), dtype=np.float64)

        # ------------------------------------------------------------------
        # PIDs (body-frame velocity + gimbal incremental angles)
        # ------------------------------------------------------------------
        # Forward velocity from distance error: positive err => marker too far,
        # so drive forward. Tight i_clamp avoids windup during marker dropout.
        self.pid_distance = PID(kp=0.8, ki=0.05, kd=0.25,
                                i_clamp=0.5, out_clamp=2.0)
        # Yaw onto the marker: positive x_offset => marker to the right,
        # need positive yaw rate (CCW in ENU is +z, so this sign may need
        # flipping per your frame; tune in sim).
        self.pid_yaw = PID(kp=1.2, ki=0.0, kd=0.10,
                           i_clamp=None, out_clamp=1.0)
        # Altitude: positive y_offset (image-down) => marker is below the
        # camera centre line, so descend. Sign flipped to push linear.z up
        # when marker is above centre.
        self.pid_altitude = PID(kp=0.6, ki=0.02, kd=0.15,
                                i_clamp=0.5, out_clamp=1.0)
        # Gimbal PIDs are P-only so the commanded angle stays smooth.
        self.pid_gimbal_yaw = PID(kp=0.7, ki=0.0, kd=0.05,
                                  i_clamp=None, out_clamp=15.0)
        self.pid_gimbal_pitch = PID(kp=0.7, ki=0.0, kd=0.05,
                                    i_clamp=None, out_clamp=15.0)
        # Landing-descent rate scales with altitude; the PID handles the
        # lateral centring on the pad while linear.z is computed directly.
        self.pid_pad_lateral_x = PID(kp=0.6, ki=0.02, kd=0.10,
                                     i_clamp=0.3, out_clamp=0.8)
        self.pid_pad_lateral_y = PID(kp=0.6, ki=0.02, kd=0.10,
                                     i_clamp=0.3, out_clamp=0.8)

        # Integrated gimbal angles (degrees). Reset on state changes.
        self.gimbal_yaw_deg = 0.0
        self.gimbal_pitch_deg = -30.0   # nominal forward-down attitude

        # ------------------------------------------------------------------
        # ROS plumbing
        # ------------------------------------------------------------------
        self.bridge = CvBridge()

        self.image_sub = self.create_subscription(
            Image, image_topic, self.image_callback, 10)
        self.landing_sub = self.create_subscription(
            Bool, landing_topic, self._on_landing_trigger, 10)

        self.vel_pub = self.create_publisher(Twist, cmd_vel_topic, 10)

        # Gimbal publisher: try the MAVROS MountControl message. If
        # mavros_msgs isn't on the path we fall back to disabling the
        # gimbal publisher rather than crashing the node.
        self._MountControl = None
        self.mount_pub = None
        try:
            from mavros_msgs.msg import MountControl  # noqa: WPS433
            self._MountControl = MountControl
            self.mount_pub = self.create_publisher(MountControl, mount_topic, 10)
            self.get_logger().info(f"Gimbal mount control -> {mount_topic}")
        except Exception as exc:  # noqa: BLE001
            self.get_logger().warning(
                f"mavros_msgs/MountControl unavailable ({exc}); "
                f"gimbal commands will not be published.")

        # ------------------------------------------------------------------
        # ArUco detector
        # ------------------------------------------------------------------
        self.aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_4X4_50)
        self.aruco_params = aruco.DetectorParameters()
        if hasattr(aruco, 'ArucoDetector'):
            self.detector = aruco.ArucoDetector(self.aruco_dict, self.aruco_params)
        else:
            self.detector = None

        # ------------------------------------------------------------------
        # State
        # ------------------------------------------------------------------
        self.state = FollowerState.FOLLOW
        self.observations: Dict[int, MarkerObservation] = {}
        self.last_seen_ns: Dict[int, int] = {}
        self.frame_count = 0
        self.landing_requested = False

        # Fixed-rate control loop, independent of camera framerate.
        self.timer = self.create_timer(1.0 / self.CONTROL_HZ, self._control_step)

        self.get_logger().info(
            f"Follower node up. image={image_topic}  cmd_vel={cmd_vel_topic}  "
            f"target_distance={self.target_distance}m  control_hz={self.CONTROL_HZ}")

    # ----------------------------------------------------------------------
    # Image callback — Part 1 pipeline, plus cache for the control loop
    # ----------------------------------------------------------------------
    def image_callback(self, msg: Image) -> None:
        try:
            frame = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
        except Exception as exc:  # noqa: BLE001 - cv_bridge raises broad type
            self.get_logger().error(f"cv_bridge conversion failed: {exc}")
            return

        self.frame_count += 1
        stamp_ns = self.get_clock().now().nanoseconds

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        corners, ids, _rejected = self._detect_markers(gray)

        observations: Dict[int, MarkerObservation] = {}
        if ids is not None and len(ids) > 0:
            aruco.drawDetectedMarkers(frame, corners, ids)
            observations = self._estimate_marker_poses(frame, corners, ids, stamp_ns)

        self.observations = observations
        self._draw_hud(frame, observations, stamp_ns)

        if self.show_window:
            cv2.imshow('iris_2 / follower', frame)
            cv2.waitKey(1)

    # ----------------------------------------------------------------------
    # Control loop — Part 2
    # ----------------------------------------------------------------------
    def _control_step(self) -> None:
        now_ns = self.get_clock().now().nanoseconds
        now_s = now_ns / 1e9

        self._update_state(now_ns)

        cmd = Twist()
        if self.state == FollowerState.FOLLOW:
            self._control_follow(cmd, now_s)
        elif self.state == FollowerState.SEARCH:
            self._control_search(cmd)
        elif self.state == FollowerState.LANDING:
            self._control_landing(cmd, now_s)
        else:  # LANDED
            pass  # all-zero Twist

        self.vel_pub.publish(cmd)
        self._publish_gimbal()

    def _update_state(self, now_ns: int) -> None:
        """Drive transitions between FOLLOW / SEARCH / LANDING / LANDED."""
        leader_last = self.last_seen_ns.get(LEADER_MARKER_ID)
        pad_visible = LANDING_PAD_ID in self.observations
        leader_loss_s = ((now_ns - leader_last) / 1e9
                         if leader_last is not None else float('inf'))

        # Landing trigger takes precedence over follow/search.
        if self.landing_requested and pad_visible and self.state != FollowerState.LANDED:
            if self.state != FollowerState.LANDING:
                self._enter_state(FollowerState.LANDING)
            # Touchdown check inside landing controller.
            return

        if self.state in (FollowerState.FOLLOW, FollowerState.SEARCH):
            if leader_loss_s > MARKER_LOST_WARN_S:
                # README: > 5s means mission fails. Still hover, don't crash.
                self.get_logger().warning(
                    f"Leader marker lost for {leader_loss_s:.1f}s — "
                    f"mission threshold breached, holding.")
                self._enter_state(FollowerState.SEARCH)
            elif leader_loss_s > MARKER_LOST_SEARCH_S:
                self._enter_state(FollowerState.SEARCH)
            elif LEADER_MARKER_ID in self.observations:
                self._enter_state(FollowerState.FOLLOW)

    def _enter_state(self, new_state: FollowerState) -> None:
        if new_state == self.state:
            return
        self.get_logger().info(f"state: {self.state.value} -> {new_state.value}")
        self.state = new_state
        # Reset every PID on transition so accumulated integrals from the
        # previous mode don't kick in.
        for pid in (self.pid_distance, self.pid_yaw, self.pid_altitude,
                    self.pid_gimbal_yaw, self.pid_gimbal_pitch,
                    self.pid_pad_lateral_x, self.pid_pad_lateral_y):
            pid.reset()

    # ------------------------------------------------------------------
    # State handlers — all derive velocity from tvecs (GPS-denied OK).
    # ------------------------------------------------------------------
    def _control_follow(self, cmd: Twist, now_s: float) -> None:
        obs = self.observations.get(LEADER_MARKER_ID)
        if obs is None:
            return  # safety: state machine should have moved us to SEARCH

        distance_err = obs.distance - self.target_distance
        cmd.linear.x = self.pid_distance.step(distance_err, now_s)
        cmd.angular.z = -self.pid_yaw.step(obs.x_offset, now_s)
        cmd.linear.z = -self.pid_altitude.step(obs.y_offset, now_s)

        self._track_gimbal(obs, now_s)

    def _control_search(self, cmd: Twist) -> None:
        # Hover and yaw slowly in the direction we last saw the marker
        # drift. Falls back to a steady CCW sweep if no history.
        last_x = 0.0
        last = self.observations.get(LEADER_MARKER_ID)
        if last is not None:
            last_x = last.x_offset
        cmd.angular.z = 0.4 if last_x >= 0 else -0.4

    def _control_landing(self, cmd: Twist, now_s: float) -> None:
        pad = self.observations.get(LANDING_PAD_ID)
        if pad is None:
            # Pad not visible right now — hold position and let the gimbal
            # try to recover; the state machine will return to FOLLOW if
            # both leader and pad disappear, which is the safe behaviour.
            return

        # Lateral centring: drive body sideways so the pad sits under us.
        cmd.linear.x = -self.pid_pad_lateral_y.step(pad.y_offset, now_s)
        cmd.linear.y = -self.pid_pad_lateral_x.step(pad.x_offset, now_s)

        # Vertical descent: scale rate with remaining altitude. Once close
        # enough, declare touchdown.
        altitude_est = max(0.0, pad.distance - self.touchdown_m)
        if altitude_est < 0.05:
            self._enter_state(FollowerState.LANDED)
            return
        descent = min(0.6, 0.25 + 0.15 * altitude_est)
        # Negative linear.z = descend in ENU.
        cmd.linear.z = -descent

        # Gimbal continues to track the pad marker so the descent stays
        # visually verified.
        self._track_gimbal(pad, now_s)

    # ------------------------------------------------------------------
    # Gimbal command (incremental angle integration from marker offsets)
    # ------------------------------------------------------------------
    def _track_gimbal(self, obs: MarkerObservation, now_s: float) -> None:
        # Angular offset of the marker from the optical axis.
        yaw_err_deg = float(np.degrees(np.arctan2(obs.x_offset, obs.distance)))
        pitch_err_deg = float(np.degrees(np.arctan2(obs.y_offset, obs.distance)))

        # Integrate small steps so the commanded angle stays smooth even
        # when the marker briefly drops out.
        self.gimbal_yaw_deg += self.pid_gimbal_yaw.step(yaw_err_deg, now_s) * 0.05
        self.gimbal_pitch_deg += self.pid_gimbal_pitch.step(pitch_err_deg, now_s) * 0.05

        # Clip to reasonable gimbal travel.
        self.gimbal_yaw_deg = max(-180.0, min(180.0, self.gimbal_yaw_deg))
        self.gimbal_pitch_deg = max(-90.0, min(30.0, self.gimbal_pitch_deg))

    def _publish_gimbal(self) -> None:
        if self.mount_pub is None or self._MountControl is None:
            return
        msg = self._MountControl()
        # MAVROS MountControl: pitch/roll/yaw in degrees, mode selects
        # MAV_MOUNT_MODE. 2 = MAV_MOUNT_MODE_MAVLINK_TARGETING (absolute).
        msg.mode = 2
        msg.pitch = float(self.gimbal_pitch_deg)
        msg.roll = 0.0
        msg.yaw = float(self.gimbal_yaw_deg)
        self.mount_pub.publish(msg)

    # ------------------------------------------------------------------
    # Landing trigger
    # ------------------------------------------------------------------
    def _on_landing_trigger(self, msg: Bool) -> None:
        if msg.data and not self.landing_requested:
            self.get_logger().info("Landing trigger received — switching to LANDING when pad visible.")
        if not msg.data and self.landing_requested:
            self.get_logger().info("Landing trigger cleared — resuming FOLLOW.")
            if self.state == FollowerState.LANDING:
                self._enter_state(FollowerState.FOLLOW)
        self.landing_requested = msg.data

    # ----------------------------------------------------------------------
    # Detection + pose estimation
    # ----------------------------------------------------------------------
    def _detect_markers(self, gray: np.ndarray):
        if self.detector is not None:
            return self.detector.detectMarkers(gray)
        return aruco.detectMarkers(gray, self.aruco_dict, parameters=self.aruco_params)

    def _estimate_marker_poses(
        self,
        frame: np.ndarray,
        corners,
        ids: np.ndarray,
        stamp_ns: int,
    ) -> Dict[int, MarkerObservation]:
        observations: Dict[int, MarkerObservation] = {}
        flat_ids = ids.flatten().tolist()

        for i, marker_id in enumerate(flat_ids):
            size_m = MARKER_SIZES_M.get(marker_id)
            if size_m is None:
                continue

            if hasattr(aruco, 'estimatePoseSingleMarkers'):
                rvecs, tvecs, _ = aruco.estimatePoseSingleMarkers(
                    [corners[i]], size_m, self.camera_matrix, self.dist_coeffs)
                rvec = rvecs[0][0]
                tvec = tvecs[0][0]
            else:
                rvec, tvec = self._solve_pnp_square(corners[i], size_m)

            cv2.drawFrameAxes(
                frame, self.camera_matrix, self.dist_coeffs,
                rvec, tvec, size_m * 0.5)

            pixel_center = tuple(np.mean(corners[i].reshape(-1, 2), axis=0).astype(int))
            obs = MarkerObservation(
                marker_id=int(marker_id),
                tvec=np.asarray(tvec, dtype=np.float64).reshape(3),
                rvec=np.asarray(rvec, dtype=np.float64).reshape(3),
                pixel_center=(int(pixel_center[0]), int(pixel_center[1])),
                stamp_ns=stamp_ns,
            )
            observations[obs.marker_id] = obs
            self.last_seen_ns[obs.marker_id] = stamp_ns

        return observations

    def _solve_pnp_square(self, corner: np.ndarray, size_m: float):
        """Fallback pose estimate for OpenCV builds that dropped
        estimatePoseSingleMarkers. Object frame matches the legacy helper:
        origin at marker centre, +X right, +Y down, +Z out of the marker."""
        half = size_m / 2.0
        object_points = np.array(
            [[-half, half, 0.0],
             [half, half, 0.0],
             [half, -half, 0.0],
             [-half, -half, 0.0]],
            dtype=np.float64,
        )
        image_points = corner.reshape(-1, 2).astype(np.float64)
        ok, rvec, tvec = cv2.solvePnP(
            object_points, image_points,
            self.camera_matrix, self.dist_coeffs,
            flags=cv2.SOLVEPNP_IPPE_SQUARE)
        if not ok:
            return np.zeros(3), np.zeros(3)
        return rvec.reshape(3), tvec.reshape(3)

    # ----------------------------------------------------------------------
    # HUD
    # ----------------------------------------------------------------------
    def _draw_hud(
        self,
        frame: np.ndarray,
        observations: Dict[int, MarkerObservation],
        now_ns: int,
    ) -> None:
        h, w = frame.shape[:2]
        cv2.drawMarker(
            frame, (w // 2, h // 2), (0, 255, 255),
            markerType=cv2.MARKER_CROSS, markerSize=18, thickness=1)

        # State badge top-left
        badge = f"STATE {self.state.value.upper()}"
        cv2.putText(frame, badge, (10, 22),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)

        y = 46
        for marker_id, obs in observations.items():
            tag = 'LEADER' if marker_id == LEADER_MARKER_ID else (
                'PAD' if marker_id == LANDING_PAD_ID else f'ID{marker_id}')
            text = (f"{tag}  d={obs.distance:5.2f}m  "
                    f"x={obs.x_offset:+.2f}  y={obs.y_offset:+.2f}")
            cv2.putText(frame, text, (10, y),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.55, (0, 255, 0), 2)
            y += 22

        # Gimbal angle line (helps debug commanded vs actual when wiring up)
        cv2.putText(frame,
                    f"GIMBAL  yaw={self.gimbal_yaw_deg:+.1f}  "
                    f"pitch={self.gimbal_pitch_deg:+.1f}",
                    (10, h - 38),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (200, 200, 200), 1)

        last = self.last_seen_ns.get(LEADER_MARKER_ID)
        if last is not None and LEADER_MARKER_ID not in observations:
            dt_s = (now_ns - last) / 1e9
            colour = (0, 0, 255) if dt_s > MARKER_LOST_WARN_S else (0, 165, 255)
            cv2.putText(frame, f"LEADER LOST  {dt_s:4.1f}s",
                        (10, h - 14), cv2.FONT_HERSHEY_SIMPLEX, 0.6, colour, 2)


def main(args=None):
    rclpy.init(args=args)
    node = FollowerNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        cv2.destroyAllWindows()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
