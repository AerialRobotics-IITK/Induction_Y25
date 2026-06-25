#!/usr/bin/env python3
"""Offline simulator for the swarm follower.

Useful when you have no Gazebo / SITL but still want to demonstrate that
``FollowerCore`` actually drives a (simulated) drone onto its leader.

The follower's body frame is assumed aligned with the world, no yaw
integration, so this is *not* a high-fidelity flight simulator -- only a
sanity check of the PID + vision pipeline.  Velocities are interpreted in
MAVROS body ENU (x = forward, y = left, z = up).
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

import cv2
import numpy as np

try:
    from .tracker_core import (ArucoTracker, FollowerConfig,
                               FollowerCore, TrackerState)
except ImportError:  # pragma: no cover - allows ``python3 sim_demo.py``
    HERE = Path(__file__).resolve().parent
    sys.path.insert(0, str(HERE.parent))
    from src.tracker_core import (  # type: ignore[no-redef]
        ArucoTracker, FollowerConfig, FollowerCore, TrackerState)


# ---------------------------------------------------------------------------
# Renderer (kept here so it ships alongside the simulator)
# ---------------------------------------------------------------------------

CAMERA_MATRIX = np.array([[530.0, 0.0, 320.0],
                          [0.0, 530.0, 240.0],
                          [0.0, 0.0, 1.0]], dtype=np.float32)
DIST = np.zeros((4, 1), dtype=np.float32)
DEFAULT_MARKER_SIZE = 0.40


def _render_marker_image(marker_id: int, tvec: np.ndarray,
                         marker_size: float = DEFAULT_MARKER_SIZE,
                         image_size=(640, 480)) -> np.ndarray:
    import cv2.aruco as aruco
    w, h = image_size
    image = np.full((h, w, 3), 200, dtype=np.uint8)

    dictionary = aruco.getPredefinedDictionary(aruco.DICT_4X4_50)
    inner = aruco.generateImageMarker(dictionary, marker_id, 300)
    quiet = np.full((360, 360), 255, dtype=np.uint8)
    quiet[30:330, 30:330] = inner
    marker_img = cv2.cvtColor(quiet, cv2.COLOR_GRAY2BGR)

    side = marker_size * (360.0 / 300.0)
    half = side / 2.0
    object_points = np.array([
        [-half, -half, 0.0],
        [ half, -half, 0.0],
        [ half,  half, 0.0],
        [-half,  half, 0.0],
    ], dtype=np.float32)
    object_points = object_points + tvec.astype(np.float32)

    rvec = np.zeros((3, 1), dtype=np.float32)
    zero = np.zeros((3, 1), dtype=np.float32)
    image_points, _ = cv2.projectPoints(
        object_points, rvec, zero, CAMERA_MATRIX, DIST)
    image_points = image_points.reshape(4, 2).astype(np.float32)

    src = np.array([[0, 0], [360, 0], [360, 360], [0, 360]], dtype=np.float32)
    H = cv2.getPerspectiveTransform(src, image_points)
    warped = cv2.warpPerspective(marker_img, H, (w, h),
                                 borderValue=(0, 0, 0))
    mask = np.zeros((h, w), dtype=np.uint8)
    cv2.fillConvexPoly(mask, image_points.astype(np.int32), 255)
    image[mask > 0] = warped[mask > 0]
    return image


# ---------------------------------------------------------------------------
# Simulator
# ---------------------------------------------------------------------------


def run_simulation(out_path: str,
                   duration_s: float = 18.0,
                   fps: int = 15,
                   leader_amp: float = 0.6,
                   marker_size: float = DEFAULT_MARKER_SIZE,
                   show: bool = False) -> dict:
    cfg = FollowerConfig(mode='chase', target_distance=2.0,
                         marker_size=marker_size, marker_id=0,
                         marker_lost_timeout_s=5.0)
    core = FollowerCore(config=cfg)
    tracker = ArucoTracker(marker_size=marker_size,
                           camera_matrix=CAMERA_MATRIX,
                           dist_coeffs=DIST)

    # Both positions live in *camera frame* coordinates for simplicity
    # (x = right, y = down, z = forward).  Follower yaw is held at zero,
    # so body ENU translates trivially:
    #   linear.x (forward, ENU) -> +z (camera)
    #   linear.y (left,    ENU) -> -x (camera)
    #   linear.z (up,      ENU) -> -y (camera)
    leader = np.array([0.0, 0.0, 5.0])
    follower = np.array([0.0, 0.0, 0.0])

    dt = 1.0 / fps
    w, h = 640, 480
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    writer = cv2.VideoWriter(out_path, fourcc, fps, (w, h))

    last_rel = leader - follower
    last_state = TrackerState.SEARCHING
    for step in range(int(duration_s * fps)):
        t = step * dt
        leader[0] = leader_amp * np.sin(0.5 * t)
        leader[1] = 0.25 * np.sin(0.25 * t)

        rel = leader - follower
        img = _render_marker_image(0, rel, marker_size=marker_size)
        obs = tracker.observe(img, wanted_id=0)
        cmd = core.update(obs, now=t)

        follower[2] += cmd.linear_x * dt
        follower[0] -= cmd.linear_y * dt
        follower[1] -= cmd.linear_z * dt

        annotated = tracker.annotate(img.copy(), obs)
        cv2.rectangle(annotated, (0, 440), (w, h), (40, 40, 40), -1)
        banner = (f"t={t:5.1f}s  state={cmd.state.name:>10}  "
                  f"dist={rel[2]:+.2f}m  lat={rel[0]:+.2f}m  "
                  f"vx={cmd.linear_x:+.2f}  vy={cmd.linear_y:+.2f}  "
                  f"vz={cmd.linear_z:+.2f}")
        cv2.putText(annotated, banner, (8, 465), cv2.FONT_HERSHEY_SIMPLEX,
                    0.42, (200, 255, 200), 1, cv2.LINE_AA)
        writer.write(annotated)
        if show:
            cv2.imshow('sim_demo', annotated)
            cv2.waitKey(1)
        last_rel = rel
        last_state = cmd.state

    writer.release()
    if show:
        cv2.destroyAllWindows()
    return {
        'final_distance': float(last_rel[2]),
        'final_lateral': float(last_rel[0]),
        'final_state': last_state.name,
    }


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', default='/tmp/follower_simulation.mp4',
                        help='Path to write the demonstration mp4.')
    parser.add_argument('--duration', type=float, default=18.0)
    parser.add_argument('--fps', type=int, default=15)
    parser.add_argument('--leader-amp', type=float, default=0.6,
                        help='Amplitude (m) of the leader weave.')
    parser.add_argument('--marker-size', type=float,
                        default=DEFAULT_MARKER_SIZE)
    parser.add_argument('--show', action='store_true',
                        help='Also display the simulation in an OpenCV '
                             'window while it runs.')
    args = parser.parse_args(argv)
    stats = run_simulation(out_path=args.out, duration_s=args.duration,
                           fps=args.fps, leader_amp=args.leader_amp,
                           marker_size=args.marker_size, show=args.show)
    print(f'final_state={stats["final_state"]}  '
          f'final_distance={stats["final_distance"]:+.2f}m  '
          f'final_lateral={stats["final_lateral"]:+.2f}m  '
          f'-> {args.out}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
