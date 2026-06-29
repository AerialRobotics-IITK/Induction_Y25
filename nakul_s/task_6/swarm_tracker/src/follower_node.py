#!/usr/bin/env python3
"""Vision-based follower for the Task 6 swarm navigation assignment.

The node intentionally avoids GPS, global-position, and odometry inputs for
iris_2. All follower velocity commands are derived from ArUco pose estimates
in the gimbal camera frame.
"""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
from typing import Dict, List, Optional

try:
    import cv2
    import cv2.aruco as aruco
    import numpy as np
except ImportError:
    cv2 = None
    aruco = None
    np = None

try:
    import rclpy
    from cv_bridge import CvBridge
    from geometry_msgs.msg import Twist
    from rclpy.node import Node
    from sensor_msgs.msg import Image
    from std_msgs.msg import Bool, Float64
except ImportError:
    rclpy = None
    CvBridge = None
    Twist = None
    Image = object
    Bool = object
    Float64 = None
    Node = object


def ensure_runtime_dependencies() -> None:
    """Raise a clear error if ROS/OpenCV dependencies are unavailable."""
    missing = []
    if cv2 is None or aruco is None or np is None:
        missing.append("opencv-python with aruco and numpy")
    if rclpy is None or CvBridge is None or Twist is None or Float64 is None:
        missing.append("ROS 2 Python packages and cv_bridge")
    if missing:
        raise RuntimeError(
            "swarm_tracker runtime dependencies are missing: "
            + ", ".join(missing)
        )


class TrackerState(Enum):
    """Mission phases for the follower."""

    FOLLOW = "follow"
    SEARCH = "search"
    LANDING = "landing"
    LANDED = "landed"


@dataclass
class MarkerPose:
    """Pose and image metadata for one detected marker."""

    marker_id: int
    rvec: np.ndarray
    tvec: np.ndarray
    corners: np.ndarray
    stamp_ns: int

    @property
    def x(self) -> float:
        return float(self.tvec[0])

    @property
    def y(self) -> float:
        return float(self.tvec[1])

    @property
    def z(self) -> float:
        return float(self.tvec[2])

    @property
    def area_px(self) -> float:
        points = self.corners.reshape(-1, 2).astype(np.float32)
        return float(abs(cv2.contourArea(points)))

    @property
    def center_px(self) -> tuple:
        center = np.mean(self.corners.reshape(-1, 2), axis=0)
        return int(center[0]), int(center[1])


class PID:
    """Small PID controller with integral windup and output limits."""

    def __init__(
        self,
        kp: float,
        ki: float = 0.0,
        kd: float = 0.0,
        integral_limit: Optional[float] = None,
        output_limit: Optional[float] = None,
    ) -> None:
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.integral_limit = integral_limit
        self.output_limit = output_limit
        self.integral = 0.0
        self.prev_error: Optional[float] = None
        self.prev_time: Optional[float] = None

    def reset(self) -> None:
        self.integral = 0.0
        self.prev_error = None
        self.prev_time = None

    def step(self, error: float, now_s: float) -> float:
        dt = 0.0 if self.prev_time is None else max(1e-3, now_s - self.prev_time)
        derivative = 0.0
        if self.prev_error is not None and dt > 0.0:
            derivative = (error - self.prev_error) / dt

        self.integral += error * dt
        if self.integral_limit is not None:
            self.integral = float(np.clip(
                self.integral,
                -self.integral_limit,
                self.integral_limit,
            ))

        output = self.kp * error + self.ki * self.integral + self.kd * derivative
        if self.output_limit is not None:
            output = float(np.clip(output, -self.output_limit, self.output_limit))

        self.prev_error = error
        self.prev_time = now_s
        return output


class FollowerNode(Node):
    """Detect ArUco markers and command iris_2 from visual relative pose."""

    def __init__(self) -> None:
        ensure_runtime_dependencies()
        super().__init__("follower_node")

        self.declare_parameter("image_topic", "/camera/image_raw")
        self.declare_parameter(
            "cmd_vel_topic",
            "/iris_2/mavros/setpoint_velocity/cmd_vel_unstamped",
        )
        self.declare_parameter("landing_trigger_topic", "/swarm_tracker/start_landing")
        self.declare_parameter(
            "mount_control_topic",
            "/iris_2/mavros/mount_control/command",
        )
        self.declare_parameter("gimbal_yaw_joint_topic", "")
        self.declare_parameter("gimbal_pitch_joint_topic", "")
        self.declare_parameter("show_window", True)

        self.declare_parameter("leader_marker_id", 0)
        self.declare_parameter("landing_marker_id", 0)
        self.declare_parameter("leader_marker_size_m", 0.30)
        self.declare_parameter("landing_marker_size_m", 1.00)
        self.declare_parameter("target_distance_m", 2.0)
        self.declare_parameter("lost_search_after_s", 1.0)
        self.declare_parameter("lost_fail_after_s", 5.0)
        self.declare_parameter("landing_touchdown_distance_m", 0.35)

        # Gazebo gimbal camera defaults: 640x480 pinhole approximation.
        # Override from CameraInfo/calibration if your local model differs.
        self.declare_parameter("fx", 530.0)
        self.declare_parameter("fy", 530.0)
        self.declare_parameter("cx", 320.0)
        self.declare_parameter("cy", 240.0)

        self.image_topic = str(self.get_parameter("image_topic").value)
        self.cmd_vel_topic = str(self.get_parameter("cmd_vel_topic").value)
        self.show_window = bool(self.get_parameter("show_window").value)
        self.leader_marker_id = int(self.get_parameter("leader_marker_id").value)
        self.landing_marker_id = int(self.get_parameter("landing_marker_id").value)
        self.leader_marker_size_m = float(
            self.get_parameter("leader_marker_size_m").value
        )
        self.landing_marker_size_m = float(
            self.get_parameter("landing_marker_size_m").value
        )
        self.target_distance_m = float(self.get_parameter("target_distance_m").value)
        self.lost_search_after_s = float(
            self.get_parameter("lost_search_after_s").value
        )
        self.lost_fail_after_s = float(self.get_parameter("lost_fail_after_s").value)
        self.touchdown_distance_m = float(
            self.get_parameter("landing_touchdown_distance_m").value
        )

        fx = float(self.get_parameter("fx").value)
        fy = float(self.get_parameter("fy").value)
        cx = float(self.get_parameter("cx").value)
        cy = float(self.get_parameter("cy").value)
        self.camera_matrix = np.array(
            [[fx, 0.0, cx], [0.0, fy, cy], [0.0, 0.0, 1.0]],
            dtype=np.float64,
        )
        self.dist_coeffs = np.zeros((5, 1), dtype=np.float64)

        self.bridge = CvBridge()
        self.aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_4X4_50)
        self.aruco_params = aruco.DetectorParameters()
        self.detector = (
            aruco.ArucoDetector(self.aruco_dict, self.aruco_params)
            if hasattr(aruco, "ArucoDetector")
            else None
        )

        self.image_sub = self.create_subscription(
            Image,
            self.image_topic,
            self.image_callback,
            10,
        )
        self.landing_sub = self.create_subscription(
            Bool,
            str(self.get_parameter("landing_trigger_topic").value),
            self.landing_callback,
            10,
        )
        self.vel_pub = self.create_publisher(Twist, self.cmd_vel_topic, 10)

        self.mount_pub = None
        self.MountControl = None
        try:
            from mavros_msgs.msg import MountControl  # type: ignore

            self.MountControl = MountControl
            self.mount_pub = self.create_publisher(
                MountControl,
                str(self.get_parameter("mount_control_topic").value),
                10,
            )
        except Exception as exc:  # noqa: BLE001
            self.get_logger().warning(
                f"MAVROS MountControl unavailable ({exc}); using joint topics only."
            )

        yaw_joint_topic = str(self.get_parameter("gimbal_yaw_joint_topic").value)
        pitch_joint_topic = str(self.get_parameter("gimbal_pitch_joint_topic").value)
        self.yaw_joint_pub = (
            self.create_publisher(Float64, yaw_joint_topic, 10)
            if yaw_joint_topic
            else None
        )
        self.pitch_joint_pub = (
            self.create_publisher(Float64, pitch_joint_topic, 10)
            if pitch_joint_topic
            else None
        )

        self.distance_pid = PID(0.85, 0.04, 0.22, integral_limit=1.0, output_limit=2.0)
        self.yaw_pid = PID(1.10, 0.00, 0.12, output_limit=1.2)
        self.altitude_pid = PID(0.65, 0.02, 0.12, integral_limit=0.8, output_limit=0.9)
        self.gimbal_yaw_pid = PID(0.80, 0.00, 0.04, output_limit=18.0)
        self.gimbal_pitch_pid = PID(0.80, 0.00, 0.04, output_limit=18.0)
        self.pad_x_pid = PID(0.70, 0.02, 0.10, integral_limit=0.5, output_limit=0.8)
        self.pad_y_pid = PID(0.70, 0.02, 0.10, integral_limit=0.5, output_limit=0.8)

        self.state = TrackerState.FOLLOW
        self.landing_requested = False
        self.latest_markers: List[MarkerPose] = []
        self.last_leader: Optional[MarkerPose] = None
        self.last_pad: Optional[MarkerPose] = None
        self.last_seen_ns: Dict[str, int] = {}
        self.mission_failure_reported = False
        self.gimbal_yaw_deg = 0.0
        self.gimbal_pitch_deg = -25.0

        self.control_timer = self.create_timer(0.05, self.control_step)
        self.get_logger().info(
            "Follower ready: "
            f"image={self.image_topic}, cmd_vel={self.cmd_vel_topic}, "
            f"target={self.target_distance_m:.1f}m"
        )

    def image_callback(self, msg: Image) -> None:
        """Run the OpenCV ArUco pipeline and cache the latest marker poses."""
        try:
            frame = self.bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
        except Exception as exc:  # noqa: BLE001
            self.get_logger().error(f"cv_bridge conversion failed: {exc}")
            return

        stamp_ns = self.get_clock().now().nanoseconds
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        corners, ids, _ = self.detect_markers(gray)

        markers: List[MarkerPose] = []
        if ids is not None and len(ids) > 0:
            aruco.drawDetectedMarkers(frame, corners, ids)
            markers = self.estimate_poses_for_current_state(corners, ids, stamp_ns)
            for marker in markers:
                cv2.drawFrameAxes(
                    frame,
                    self.camera_matrix,
                    self.dist_coeffs,
                    marker.rvec,
                    marker.tvec,
                    self.current_marker_size_m() * 0.45,
                )

        self.latest_markers = markers
        self.refresh_targets(markers, stamp_ns)
        self.draw_overlay(frame, markers, stamp_ns)

        if self.show_window:
            cv2.imshow("iris_2 follower camera", frame)
            cv2.waitKey(1)

    def detect_markers(self, gray: np.ndarray):
        if self.detector is not None:
            return self.detector.detectMarkers(gray)
        return aruco.detectMarkers(gray, self.aruco_dict, parameters=self.aruco_params)

    def estimate_poses_for_current_state(
        self,
        corners,
        ids: np.ndarray,
        stamp_ns: int,
    ) -> List[MarkerPose]:
        marker_size = self.current_marker_size_m()
        results: List[MarkerPose] = []
        for index, marker_id in enumerate(ids.flatten().tolist()):
            if not self.is_relevant_marker(marker_id):
                continue

            if hasattr(aruco, "estimatePoseSingleMarkers"):
                rvecs, tvecs, _ = aruco.estimatePoseSingleMarkers(
                    [corners[index]],
                    marker_size,
                    self.camera_matrix,
                    self.dist_coeffs,
                )
                rvec = rvecs[0][0]
                tvec = tvecs[0][0]
            else:
                rvec, tvec = self.solve_pnp_square(corners[index], marker_size)

            results.append(
                MarkerPose(
                    marker_id=int(marker_id),
                    rvec=np.asarray(rvec, dtype=np.float64).reshape(3),
                    tvec=np.asarray(tvec, dtype=np.float64).reshape(3),
                    corners=np.asarray(corners[index], dtype=np.float32),
                    stamp_ns=stamp_ns,
                )
            )
        return results

    def current_marker_size_m(self) -> float:
        if self.state == TrackerState.LANDING:
            return self.landing_marker_size_m
        return self.leader_marker_size_m

    def is_relevant_marker(self, marker_id: int) -> bool:
        if self.state == TrackerState.LANDING:
            return marker_id == self.landing_marker_id
        return marker_id == self.leader_marker_id

    def solve_pnp_square(self, corner: np.ndarray, size_m: float) -> tuple:
        half = size_m / 2.0
        object_points = np.array(
            [
                [-half, half, 0.0],
                [half, half, 0.0],
                [half, -half, 0.0],
                [-half, -half, 0.0],
            ],
            dtype=np.float64,
        )
        image_points = corner.reshape(-1, 2).astype(np.float64)
        ok, rvec, tvec = cv2.solvePnP(
            object_points,
            image_points,
            self.camera_matrix,
            self.dist_coeffs,
            flags=cv2.SOLVEPNP_IPPE_SQUARE,
        )
        if not ok:
            return np.zeros(3), np.zeros(3)
        return rvec.reshape(3), tvec.reshape(3)

    def refresh_targets(self, markers: List[MarkerPose], stamp_ns: int) -> None:
        if not markers:
            return

        target = min(markers, key=lambda marker: abs(marker.x) + abs(marker.y))
        if self.state == TrackerState.LANDING:
            self.last_pad = target
            self.last_seen_ns["pad"] = stamp_ns
        else:
            self.last_leader = target
            self.last_seen_ns["leader"] = stamp_ns
            self.mission_failure_reported = False

    def landing_callback(self, msg: Bool) -> None:
        self.landing_requested = bool(msg.data)
        if self.landing_requested:
            self.enter_state(TrackerState.LANDING)
            self.get_logger().info("Landing requested; tracking landing pad marker.")
        elif self.state in (TrackerState.LANDING, TrackerState.LANDED):
            self.enter_state(TrackerState.FOLLOW)

    def control_step(self) -> None:
        now_ns = self.get_clock().now().nanoseconds
        now_s = now_ns / 1e9
        self.update_state(now_ns)

        cmd = Twist()
        if self.state == TrackerState.FOLLOW:
            self.follow_leader(cmd, now_s)
        elif self.state == TrackerState.SEARCH:
            self.search_for_leader(cmd)
        elif self.state == TrackerState.LANDING:
            self.land_on_pad(cmd, now_s)

        self.vel_pub.publish(cmd)
        self.publish_gimbal()

    def update_state(self, now_ns: int) -> None:
        if self.state in (TrackerState.LANDING, TrackerState.LANDED):
            return

        leader_stamp = self.last_seen_ns.get("leader")
        if leader_stamp is None:
            self.enter_state(TrackerState.SEARCH)
            return

        lost_s = (now_ns - leader_stamp) / 1e9

        if lost_s > self.lost_fail_after_s:
            self.enter_state(TrackerState.SEARCH)
            if not self.mission_failure_reported:
                self.get_logger().warning(
                    f"Leader marker lost for {lost_s:.1f}s; mission threshold exceeded."
                )
                self.mission_failure_reported = True
        elif lost_s > self.lost_search_after_s:
            self.enter_state(TrackerState.SEARCH)
        elif self.last_leader is not None:
            self.enter_state(TrackerState.FOLLOW)

    def enter_state(self, new_state: TrackerState) -> None:
        if new_state == self.state:
            return
        self.get_logger().info(f"State {self.state.value} -> {new_state.value}")
        self.state = new_state
        for pid in (
            self.distance_pid,
            self.yaw_pid,
            self.altitude_pid,
            self.gimbal_yaw_pid,
            self.gimbal_pitch_pid,
            self.pad_x_pid,
            self.pad_y_pid,
        ):
            pid.reset()

    def follow_leader(self, cmd: Twist, now_s: float) -> None:
        target = self.last_leader
        if target is None:
            return

        distance_error = target.z - self.target_distance_m
        cmd.linear.x = self.distance_pid.step(distance_error, now_s)
        cmd.angular.z = -self.yaw_pid.step(target.x, now_s)
        cmd.linear.z = -self.altitude_pid.step(target.y, now_s)
        self.track_gimbal(target, now_s)

    def search_for_leader(self, cmd: Twist) -> None:
        last_x = self.last_leader.x if self.last_leader is not None else 0.0
        cmd.angular.z = -0.35 if last_x < 0.0 else 0.35

    def land_on_pad(self, cmd: Twist, now_s: float) -> None:
        target = self.last_pad
        if target is None:
            self.search_for_leader(cmd)
            return

        cmd.linear.x = -self.pad_y_pid.step(target.y, now_s)
        cmd.linear.y = -self.pad_x_pid.step(target.x, now_s)
        if target.z <= self.touchdown_distance_m:
            self.enter_state(TrackerState.LANDED)
            return

        descent_rate = min(0.6, max(0.18, 0.20 + 0.12 * target.z))
        cmd.linear.z = -descent_rate
        self.track_gimbal(target, now_s)

    def track_gimbal(self, target: MarkerPose, now_s: float) -> None:
        yaw_error_deg = float(np.degrees(np.arctan2(target.x, target.z)))
        pitch_error_deg = float(np.degrees(np.arctan2(target.y, target.z)))
        self.gimbal_yaw_deg += self.gimbal_yaw_pid.step(yaw_error_deg, now_s) * 0.05
        self.gimbal_pitch_deg += (
            self.gimbal_pitch_pid.step(pitch_error_deg, now_s) * 0.05
        )
        self.gimbal_yaw_deg = float(np.clip(self.gimbal_yaw_deg, -180.0, 180.0))
        self.gimbal_pitch_deg = float(np.clip(self.gimbal_pitch_deg, -90.0, 45.0))

    def publish_gimbal(self) -> None:
        if self.mount_pub is not None and self.MountControl is not None:
            msg = self.MountControl()
            msg.mode = 2
            msg.pitch = float(self.gimbal_pitch_deg)
            msg.roll = 0.0
            msg.yaw = float(self.gimbal_yaw_deg)
            self.mount_pub.publish(msg)

        if self.yaw_joint_pub is not None:
            self.yaw_joint_pub.publish(Float64(data=np.radians(self.gimbal_yaw_deg)))
        if self.pitch_joint_pub is not None:
            self.pitch_joint_pub.publish(
                Float64(data=np.radians(self.gimbal_pitch_deg))
            )

    def draw_overlay(
        self,
        frame: np.ndarray,
        markers: List[MarkerPose],
        now_ns: int,
    ) -> None:
        height, width = frame.shape[:2]
        cv2.drawMarker(
            frame,
            (width // 2, height // 2),
            (0, 255, 255),
            markerType=cv2.MARKER_CROSS,
            markerSize=20,
            thickness=2,
        )
        cv2.putText(
            frame,
            f"STATE: {self.state.value.upper()}",
            (10, 24),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.65,
            (255, 255, 0),
            2,
        )

        y = 52
        for marker in markers:
            u, v = marker.center_px
            label = (
                "PAD"
                if self.state == TrackerState.LANDING
                else f"LEADER id={marker.marker_id}"
            )
            text = f"{label} z={marker.z:.2f}m x={marker.x:+.2f} y={marker.y:+.2f}"
            cv2.circle(frame, (u, v), 4, (0, 255, 255), -1)
            cv2.putText(
                frame,
                text,
                (10, y),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.55,
                (0, 255, 0),
                2,
            )
            y += 24

        cv2.putText(
            frame,
            f"gimbal yaw={self.gimbal_yaw_deg:+.1f} pitch={self.gimbal_pitch_deg:+.1f}",
            (10, height - 34),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.5,
            (220, 220, 220),
            1,
        )

        leader_stamp = self.last_seen_ns.get("leader")
        if self.state != TrackerState.LANDING and leader_stamp is not None:
            lost_s = (now_ns - leader_stamp) / 1e9
            if lost_s > self.lost_search_after_s:
                color = (0, 0, 255) if lost_s > self.lost_fail_after_s else (0, 165, 255)
                cv2.putText(
                    frame,
                    f"LEADER LOST {lost_s:.1f}s",
                    (10, height - 12),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.6,
                    color,
                    2,
                )


def main(args=None) -> None:
    ensure_runtime_dependencies()
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


if __name__ == "__main__":
    main()
