"""Offline tests for the framework-free pieces of the follower.

These tests deliberately avoid ``rclpy`` so they can run on any machine that
has ``opencv-contrib-python`` and ``numpy`` installed (no ROS 2 required).
"""

from __future__ import annotations

import os
import sys

import cv2
import cv2.aruco as aruco
import numpy as np
import pytest

# Make the ``src`` package importable without ament_python being available.
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.normpath(os.path.join(HERE, '..')))

from src.tracker_core import (  # noqa: E402
    ArucoTracker,
    FollowerConfig,
    FollowerCore,
    PIDController,
    PIDGains,
    TrackerState,
)


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

# The synthetic renderer paints the marker as it would appear through a
# pinhole camera, so we use a slightly bigger marker (0.40 m) to ensure
# OpenCV's contour-finder has enough pixels to lock onto it at the
# rendering distances used by these tests.
CAMERA_MATRIX = np.array([[530.0, 0.0, 320.0],
                          [0.0, 530.0, 240.0],
                          [0.0, 0.0, 1.0]], dtype=np.float32)
DIST = np.zeros((4, 1), dtype=np.float32)
MARKER_SIZE = 0.40


def _render_marker_image(marker_id: int,
                         tvec_world: np.ndarray,
                         image_size=(640, 480)) -> np.ndarray:
    """Render a 640x480 image of a single ArUco marker at ``tvec_world``.

    The synthetic camera is the same one we configured the tracker with so
    that we have a closed-loop sanity check: detect → estimate pose → assert
    we recover the original translation vector.
    """
    w, h = image_size
    image = np.full((h, w, 3), 200, dtype=np.uint8)

    dictionary = aruco.getPredefinedDictionary(aruco.DICT_4X4_50)
    marker_inner = aruco.generateImageMarker(dictionary, marker_id, 300)
    # ArUco detectors need a white quiet zone around the marker --
    # otherwise the contour-finder cannot lock onto the outer square.
    marker_with_quiet = np.full((360, 360), 255, dtype=np.uint8)
    marker_with_quiet[30:330, 30:330] = marker_inner
    marker_img = cv2.cvtColor(marker_with_quiet, cv2.COLOR_GRAY2BGR)

    # The physical marker spans MARKER_SIZE, but the white quiet zone is
    # part of the printed pattern -- model it explicitly.  In camera-frame
    # convention y points *down*, so the corner order below corresponds to
    # image-space (top-left, top-right, bottom-right, bottom-left).
    quiet_size = MARKER_SIZE * (360.0 / 300.0)
    half = quiet_size / 2.0
    object_points = np.array([
        [-half, -half, 0.0],   # top-left
        [ half, -half, 0.0],   # top-right
        [ half,  half, 0.0],   # bottom-right
        [-half,  half, 0.0],   # bottom-left
    ], dtype=np.float32)
    object_points = object_points + tvec_world.astype(np.float32)

    rvec = np.zeros((3, 1), dtype=np.float32)
    tvec = np.zeros((3, 1), dtype=np.float32)
    image_points, _ = cv2.projectPoints(
        object_points, rvec, tvec, CAMERA_MATRIX, DIST)
    image_points = image_points.reshape(4, 2).astype(np.float32)

    src = np.array([[0, 0], [360, 0], [360, 360], [0, 360]],
                   dtype=np.float32)
    H = cv2.getPerspectiveTransform(src, image_points)
    warped = cv2.warpPerspective(marker_img, H, (w, h),
                                 borderValue=(0, 0, 0))

    mask = np.zeros((h, w), dtype=np.uint8)
    cv2.fillConvexPoly(mask, image_points.astype(np.int32), 255)
    image[mask > 0] = warped[mask > 0]
    return image


# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------


def test_pid_zero_error_zero_output():
    pid = PIDController(PIDGains(kp=1.0, ki=0.1, kd=0.1))
    assert pid.step(1.0, 1.0, now=0.0) == pytest.approx(0.0)


def test_pid_drives_setpoint():
    pid = PIDController(PIDGains(kp=0.5, ki=0.0, kd=0.0, out_clamp=10.0))
    # Positive error -> positive output.
    out = pid.step(setpoint=2.0, measurement=0.0, now=0.0)
    assert out == pytest.approx(1.0)


def test_pid_anti_windup():
    pid = PIDController(PIDGains(kp=0.0, ki=1.0, kd=0.0, i_clamp=0.25,
                                 out_clamp=10.0))
    t = 0.0
    for _ in range(50):
        pid.step(setpoint=1.0, measurement=0.0, now=t)
        t += 0.1
    # Integral capped at i_clamp -> output capped at ki * i_clamp.
    assert abs(pid._integral) <= 0.25 + 1e-6


def test_aruco_detects_marker_and_recovers_pose():
    tracker = ArucoTracker(marker_size=MARKER_SIZE,
                           camera_matrix=CAMERA_MATRIX,
                           dist_coeffs=DIST)
    image = _render_marker_image(marker_id=0,
                                 tvec_world=np.array([0.0, 0.0, 2.5]))
    obs = tracker.observe(image, wanted_id=0)
    assert obs is not None
    assert obs.marker_id == 0
    # Pose recovery should be within ~5 cm of the rendered ground truth.
    assert obs.tvec[2] == pytest.approx(2.5, abs=0.1)
    assert obs.tvec[0] == pytest.approx(0.0, abs=0.1)
    assert obs.tvec[1] == pytest.approx(0.0, abs=0.1)


def test_aruco_returns_none_without_marker():
    tracker = ArucoTracker(marker_size=MARKER_SIZE,
                           camera_matrix=CAMERA_MATRIX,
                           dist_coeffs=DIST)
    blank = np.full((480, 640, 3), 127, dtype=np.uint8)
    assert tracker.observe(blank, wanted_id=0) is None


def test_follower_chases_to_target_distance():
    config = FollowerConfig(mode='chase', target_distance=2.0,
                            marker_lost_timeout_s=5.0)
    core = FollowerCore(config=config)
    tracker = ArucoTracker(marker_size=MARKER_SIZE,
                           camera_matrix=CAMERA_MATRIX,
                           dist_coeffs=DIST)

    distance = 5.0
    t = 0.0
    for _ in range(200):
        image = _render_marker_image(
            marker_id=0, tvec_world=np.array([0.0, 0.0, distance]))
        obs = tracker.observe(image, wanted_id=0)
        cmd = core.update(obs, now=t)
        # Integrate the commanded forward velocity into the simulated
        # closing distance (positive vx = move forward = distance shrinks).
        distance = max(0.5, distance - cmd.linear_x * 0.1)
        t += 0.1

    assert distance == pytest.approx(2.0, abs=0.2)
    assert core.state == TrackerState.TRACKING


def test_follower_marks_mission_failed_after_timeout():
    config = FollowerConfig(mode='chase', marker_lost_timeout_s=2.0)
    core = FollowerCore(config=config)

    # Acquire the marker once so the timeout clock starts ticking.
    img = _render_marker_image(0, np.array([0.0, 0.0, 2.0]))
    tracker = ArucoTracker(marker_size=MARKER_SIZE,
                           camera_matrix=CAMERA_MATRIX,
                           dist_coeffs=DIST)
    obs = tracker.observe(img, wanted_id=0)
    core.update(obs, now=0.0)

    # Now lose it for longer than the timeout.
    cmd_before = core.update(None, now=1.0)
    cmd_after = core.update(None, now=3.5)

    assert cmd_before.state == TrackerState.SEARCHING
    assert cmd_after.state == TrackerState.MISSION_FAILED


def test_follower_landing_mode_transitions_to_landing():
    config = FollowerConfig(
        mode='landing', target_distance=2.0,
        marker_id=0, landing_pad_id=5,
        formation_dwell_s=1.0, landing_descent_rate=0.6,
        marker_lost_timeout_s=10.0,
    )
    core = FollowerCore(config=config)
    tracker = ArucoTracker(marker_size=MARKER_SIZE,
                           camera_matrix=CAMERA_MATRIX,
                           dist_coeffs=DIST)

    # Formation phase: see the leader marker.
    leader_img = _render_marker_image(0, np.array([0.0, 0.0, 2.0]))
    obs = tracker.observe(leader_img, wanted_id=0)
    core.update(obs, now=0.0)
    assert core.state == TrackerState.FORMATION

    # Dwell elapses while we still see the leader.
    obs = tracker.observe(leader_img, wanted_id=0)
    core.update(obs, now=1.5)
    assert core.state == TrackerState.LANDING

    # Now show the landing-pad marker; the controller should command a
    # negative vertical velocity (i.e. descend) in MAVROS body ENU.
    pad_img = _render_marker_image(5, np.array([0.0, 0.2, 1.5]))
    obs = tracker.observe(pad_img, wanted_id=5)
    assert obs is not None, 'landing-pad marker should be detected'
    cmd = core.update(obs, now=2.0)
    assert cmd.state == TrackerState.LANDING
    assert cmd.linear_z < 0.0  # descending (ENU body: -z = down)


def test_follower_chase_sign_conventions():
    """If the marker is to the right (tx > 0) and below (ty > 0) the
    controller should command -y (move right) and -z (descend) in MAVROS
    body ENU, and a clockwise yaw (angular.z < 0)."""
    cfg = FollowerConfig(mode='chase', target_distance=2.0,
                         marker_lost_timeout_s=5.0)
    core = FollowerCore(config=cfg)
    tracker = ArucoTracker(marker_size=MARKER_SIZE,
                           camera_matrix=CAMERA_MATRIX,
                           dist_coeffs=DIST)

    # Marker placed slightly right + below + close.
    img = _render_marker_image(0, np.array([0.3, 0.2, 1.8]))
    obs = tracker.observe(img, wanted_id=0)
    assert obs is not None
    cmd = core.update(obs, now=0.0)
    assert cmd.linear_y < 0.0       # right of frame -> move right (-y)
    assert cmd.linear_z < 0.0       # below frame    -> descend (-z)
    assert cmd.angular_z < 0.0      # yaw clockwise to face the marker

    # Marker pulled further away -> command forward velocity.
    img2 = _render_marker_image(0, np.array([0.0, 0.0, 3.5]))
    obs2 = tracker.observe(img2, wanted_id=0)
    cmd2 = core.update(obs2, now=1.0)
    assert cmd2.linear_x > 0.0      # too far -> move forward
