"""Framework-free pieces of the vision-based swarm follower.

Everything in this module is deliberately decoupled from ``rclpy`` so that
it can be exercised with nothing more than NumPy + ``opencv-contrib-python``
(see ``test/test_aruco_pipeline.py``).  ``follower_node.py`` is then a thin
ROS 2 glue layer on top of these primitives.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from enum import Enum, auto
from typing import Optional, Tuple

import time

import cv2
import cv2.aruco as aruco
import numpy as np


# ---------------------------------------------------------------------------
# PID controller
# ---------------------------------------------------------------------------


@dataclass
class PIDGains:
    kp: float = 0.0
    ki: float = 0.0
    kd: float = 0.0
    i_clamp: float = 1.0     # Anti-windup on the integral term.
    out_clamp: float = 5.0   # Saturate the final output (m/s or rad/s).


@dataclass
class PIDController:
    """A textbook PID with anti-windup and derivative-on-measurement.

    Derivative-on-measurement avoids the classic "derivative kick" when the
    setpoint changes (e.g. when we hand the follower a new target distance).
    """

    gains: PIDGains
    _integral: float = 0.0
    _prev_measurement: Optional[float] = None
    _last_time: Optional[float] = None

    def reset(self) -> None:
        self._integral = 0.0
        self._prev_measurement = None
        self._last_time = None

    def step(self, setpoint: float, measurement: float,
             now: Optional[float] = None) -> float:
        now = time.monotonic() if now is None else now
        error = setpoint - measurement

        if self._last_time is None:
            dt = 0.0
        else:
            dt = max(now - self._last_time, 1e-3)

        if dt > 0.0:
            self._integral += error * dt
            self._integral = max(min(self._integral, self.gains.i_clamp),
                                 -self.gains.i_clamp)

        if self._prev_measurement is None or dt == 0.0:
            derivative = 0.0
        else:
            derivative = -(measurement - self._prev_measurement) / dt

        out = (self.gains.kp * error
               + self.gains.ki * self._integral
               + self.gains.kd * derivative)
        out = max(min(out, self.gains.out_clamp), -self.gains.out_clamp)

        self._prev_measurement = measurement
        self._last_time = now
        return out


# ---------------------------------------------------------------------------
# Vision helpers
# ---------------------------------------------------------------------------


@dataclass
class MarkerObservation:
    """Result of running ArUco detection on a single frame."""

    marker_id: int
    tvec: np.ndarray                   # shape (3,), metres in camera frame.
    rvec: np.ndarray                   # shape (3,), Rodrigues rotation vec.
    pixel_offset: Tuple[float, float]  # (dx, dy) marker-center vs img-center.


def _solve_marker_pose(corners: np.ndarray, marker_size: float,
                       camera_matrix: np.ndarray,
                       dist_coeffs: np.ndarray
                       ) -> Tuple[np.ndarray, np.ndarray]:
    """Pose estimation that is portable across OpenCV 4.6 -> 4.10.

    ``cv2.aruco.estimatePoseSingleMarkers`` was deprecated in 4.7 and
    eventually removed.  We fall back to a manual ``solvePnP`` so that the
    same code works on the grader (OpenCV 4.10 on Jazzy) and on developer
    machines still shipping 4.6.
    """
    half = marker_size / 2.0
    object_points = np.array([
        [-half,  half, 0.0],
        [ half,  half, 0.0],
        [ half, -half, 0.0],
        [-half, -half, 0.0],
    ], dtype=np.float32)

    image_points = corners.reshape(4, 2).astype(np.float32)
    ok, rvec, tvec = cv2.solvePnP(
        object_points, image_points, camera_matrix, dist_coeffs,
        flags=cv2.SOLVEPNP_IPPE_SQUARE,
    )
    if not ok:
        raise RuntimeError('solvePnP failed for marker corners')
    return rvec.reshape(3), tvec.reshape(3)


class ArucoTracker:
    """Stateless wrapper around the OpenCV ArUco pipeline."""

    def __init__(self, dictionary_id: int = aruco.DICT_4X4_50,
                 marker_size: float = 0.15,
                 camera_matrix: Optional[np.ndarray] = None,
                 dist_coeffs: Optional[np.ndarray] = None) -> None:
        self.marker_size = float(marker_size)
        self.dictionary = aruco.getPredefinedDictionary(dictionary_id)
        try:
            params = aruco.DetectorParameters()
            self.detector = aruco.ArucoDetector(self.dictionary, params)
            self._use_detector_class = True
        except AttributeError:  # pragma: no cover - very old OpenCV
            self._params = aruco.DetectorParameters_create()
            self._use_detector_class = False

        self.camera_matrix = (camera_matrix
                              if camera_matrix is not None
                              else np.array([[530.0, 0.0, 320.0],
                                             [0.0, 530.0, 240.0],
                                             [0.0, 0.0, 1.0]],
                                            dtype=np.float32))
        self.dist_coeffs = (dist_coeffs
                            if dist_coeffs is not None
                            else np.zeros((4, 1), dtype=np.float32))

    def detect(self, image_bgr: np.ndarray):
        gray = cv2.cvtColor(image_bgr, cv2.COLOR_BGR2GRAY)
        if self._use_detector_class:
            corners, ids, _ = self.detector.detectMarkers(gray)
        else:  # pragma: no cover
            corners, ids, _ = aruco.detectMarkers(
                gray, self.dictionary, parameters=self._params)
        return corners, ids

    def observe(self, image_bgr: np.ndarray,
                wanted_id: Optional[int] = None
                ) -> Optional[MarkerObservation]:
        corners, ids = self.detect(image_bgr)
        if ids is None or len(ids) == 0:
            return None

        ids = ids.flatten().tolist()
        if wanted_id is not None and wanted_id in ids:
            idx = ids.index(wanted_id)
        else:
            idx = 0

        rvec, tvec = _solve_marker_pose(
            corners[idx], self.marker_size,
            self.camera_matrix, self.dist_coeffs)

        h, w = image_bgr.shape[:2]
        marker_center = corners[idx].reshape(4, 2).mean(axis=0)
        dx = marker_center[0] - w * 0.5
        dy = marker_center[1] - h * 0.5

        return MarkerObservation(
            marker_id=int(ids[idx]),
            tvec=tvec,
            rvec=rvec,
            pixel_offset=(float(dx), float(dy)),
        )

    def annotate(self, image_bgr: np.ndarray,
                 obs: Optional[MarkerObservation]) -> np.ndarray:
        corners, ids = self.detect(image_bgr)
        if ids is not None and len(ids) > 0:
            aruco.drawDetectedMarkers(image_bgr, corners, ids)
        if obs is not None:
            cv2.drawFrameAxes(image_bgr, self.camera_matrix, self.dist_coeffs,
                              obs.rvec, obs.tvec, 0.1)
            label = (f'id={obs.marker_id} '
                     f'd={float(obs.tvec[2]):.2f}m '
                     f'x={float(obs.tvec[0]):+.2f}m '
                     f'y={float(obs.tvec[1]):+.2f}m')
            cv2.putText(image_bgr, label, (10, 25),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
        return image_bgr


# ---------------------------------------------------------------------------
# Tracking state machine
# ---------------------------------------------------------------------------


class TrackerState(Enum):
    SEARCHING = auto()       # No marker in sight; hold position and yaw-spin.
    TRACKING = auto()        # Marker visible; chase to ``target_distance``.
    FORMATION = auto()       # Subtask 3 dwell before transitioning.
    LANDING = auto()         # Subtask 3 final descent on the landing pad.
    MISSION_FAILED = auto()  # Lost the marker for more than the timeout.


@dataclass
class FollowerCommand:
    """Output of one control step -- a Twist + a desired gimbal pose."""

    linear_x: float = 0.0
    linear_y: float = 0.0
    linear_z: float = 0.0
    angular_z: float = 0.0
    gimbal_pitch_deg: float = 0.0
    gimbal_yaw_deg: float = 0.0
    state: TrackerState = TrackerState.SEARCHING


@dataclass
class FollowerConfig:
    mode: str = 'chase'              # chase | gps_denied | landing
    target_distance: float = 2.0
    marker_id: int = 0
    landing_pad_id: int = 5
    marker_size: float = 0.15
    formation_dwell_s: float = 8.0
    landing_descent_rate: float = 0.4
    marker_lost_timeout_s: float = 5.0
    image_width: int = 640
    image_height: int = 480
    gimbal_max_deg: float = 45.0


@dataclass
class FollowerCore:
    """Mode-aware controller, decoupled from rclpy for ease of testing."""

    config: FollowerConfig
    pid_distance: PIDController = field(default_factory=lambda: PIDController(
        PIDGains(kp=0.9, ki=0.05, kd=0.35, i_clamp=1.0, out_clamp=2.0)))
    pid_lateral: PIDController = field(default_factory=lambda: PIDController(
        PIDGains(kp=1.1, ki=0.03, kd=0.25, i_clamp=0.8, out_clamp=2.0)))
    pid_vertical: PIDController = field(default_factory=lambda: PIDController(
        PIDGains(kp=0.8, ki=0.02, kd=0.2,  i_clamp=0.5, out_clamp=1.5)))
    pid_yaw: PIDController = field(default_factory=lambda: PIDController(
        PIDGains(kp=2.5, ki=0.0,  kd=0.1,  i_clamp=0.3, out_clamp=1.5)))

    state: TrackerState = TrackerState.SEARCHING
    last_seen: float = 0.0
    first_seen: Optional[float] = None
    _formation_t0: Optional[float] = None

    def expected_marker_id(self) -> int:
        if self.state == TrackerState.LANDING:
            return self.config.landing_pad_id
        return self.config.marker_id

    def update(self, obs: Optional[MarkerObservation],
               now: Optional[float] = None) -> FollowerCommand:
        now = time.monotonic() if now is None else now
        cmd = FollowerCommand()

        if obs is None or obs.marker_id != self.expected_marker_id():
            return self._handle_missing_marker(now, cmd)

        if self.first_seen is None:
            self.first_seen = now
        self.last_seen = now

        if self.state in (TrackerState.SEARCHING, TrackerState.MISSION_FAILED):
            if self.config.mode == 'landing':
                self.state = TrackerState.FORMATION
                self._formation_t0 = now
            else:
                self.state = TrackerState.TRACKING

        # Subtask 3: after the formation dwell, switch to landing on the pad.
        if (self.state == TrackerState.FORMATION
                and self._formation_t0 is not None
                and now - self._formation_t0 >= self.config.formation_dwell_s):
            self.state = TrackerState.LANDING
            for pid in (self.pid_distance, self.pid_lateral,
                        self.pid_vertical, self.pid_yaw):
                pid.reset()

        tx, ty, tz = (float(obs.tvec[0]), float(obs.tvec[1]),
                      float(obs.tvec[2]))

        # ------------------------------------------------------------------
        # Frame conventions used below:
        #
        # * OpenCV camera frame (where ``tvec`` lives):  x = right,
        #   y = down, z = forward.
        # * MAVROS body ENU (what ``setpoint_velocity/cmd_vel_unstamped``
        #   expects on iris_2): x = forward, y = LEFT, z = UP.
        #
        # Translating between the two:
        #
        #   linear.x (forward) <-  +tz (further => move forward)
        #   linear.y (left)    <-  -tx (marker right => move right => -y)
        #   linear.z (up)      <-  -ty (marker below => descend  => -z)
        #
        # The PID returns ``kp * (setpoint - measurement)``; the sign
        # bookkeeping below is what turns that into the correct body
        # velocity for each axis.
        # ------------------------------------------------------------------

        # Lateral / yaw alignment is identical across modes.
        cmd.linear_y = self.pid_lateral.step(0.0, tx, now)
        cmd.angular_z = self.pid_yaw.step(0.0, tx / max(tz, 0.1), now)

        if self.state == TrackerState.LANDING:
            # During landing we use the marker's vertical (image-y) offset to
            # close the longitudinal gap to the moving pad while descending.
            cmd.linear_x = self.pid_distance.step(0.0, ty, now)
            cmd.linear_z = -self.config.landing_descent_rate
        else:
            # Move forward when ``tz`` exceeds the target distance.
            cmd.linear_x = -self.pid_distance.step(
                self.config.target_distance, tz, now)
            cmd.linear_z = self.pid_vertical.step(0.0, ty, now)

        # Gimbal: keep the marker centred.
        dx_px, dy_px = obs.pixel_offset
        norm_x = dx_px / max(self.config.image_width * 0.5, 1.0)
        norm_y = dy_px / max(self.config.image_height * 0.5, 1.0)
        cmd.gimbal_yaw_deg = float(np.clip(
            norm_x * self.config.gimbal_max_deg,
            -self.config.gimbal_max_deg, self.config.gimbal_max_deg))
        cmd.gimbal_pitch_deg = float(np.clip(
            -norm_y * self.config.gimbal_max_deg,
            -self.config.gimbal_max_deg, self.config.gimbal_max_deg))

        cmd.state = self.state
        return cmd

    def _handle_missing_marker(self, now: float,
                               cmd: FollowerCommand) -> FollowerCommand:
        if self.first_seen is None:
            self.state = TrackerState.SEARCHING
            cmd.angular_z = 0.3
            cmd.state = self.state
            return cmd

        elapsed = now - self.last_seen
        if elapsed >= self.config.marker_lost_timeout_s:
            self.state = TrackerState.MISSION_FAILED
        else:
            self.state = TrackerState.SEARCHING
            cmd.angular_z = 0.3
        cmd.state = self.state
        return cmd
