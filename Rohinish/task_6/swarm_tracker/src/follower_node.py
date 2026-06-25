#!/usr/bin/env python3
"""Vision-based ArUco follower node for Task 6.

Thin ROS 2 glue layer over the framework-free primitives in
:mod:`src.tracker_core`.  See that module for the PID controller, ArUco
pipeline and tracking state-machine implementations.

Modes (selected via the ``mode`` ROS 2 parameter):

* **chase**       -- Subtask 1: follow ``iris_1`` at a fixed distance.
* **gps_denied**  -- Subtask 2: identical control law, but the node refuses
                     to subscribe to any GPS / odometry topic.
* **landing**     -- Subtask 3: dwell in formation, then transition to the
                     landing-pad marker and descend.
"""

from __future__ import annotations

from typing import Optional

import cv2
import cv2.aruco as aruco
import numpy as np

import rclpy
from cv_bridge import CvBridge
from geometry_msgs.msg import Twist, Vector3Stamped
from rclpy.node import Node
from sensor_msgs.msg import Image
from std_msgs.msg import String

try:  # Allow ``python3 follower_node.py`` for ad-hoc debugging.
    from .tracker_core import (
        ArucoTracker,
        FollowerConfig,
        FollowerCore,
        MarkerObservation,
        TrackerState,
    )
except ImportError:  # pragma: no cover - executed only when run as a script
    from src.tracker_core import (  # type: ignore[no-redef]
        ArucoTracker,
        FollowerConfig,
        FollowerCore,
        MarkerObservation,
        TrackerState,
    )


class FollowerNode(Node):
    def __init__(self) -> None:
        super().__init__('follower_node')

        # ----------------------------------------------------------- params
        self.declare_parameter('mode', 'chase')
        self.declare_parameter('image_topic', '/iris_2/camera/image')
        self.declare_parameter(
            'cmd_vel_topic',
            '/iris_2/mavros/setpoint_velocity/cmd_vel_unstamped')
        self.declare_parameter(
            'gimbal_topic', '/iris_2/mavros/mount_control/command')
        self.declare_parameter('status_topic', '/iris_2/swarm_tracker/status')
        self.declare_parameter('marker_id', 0)
        self.declare_parameter('landing_pad_id', 5)
        self.declare_parameter('marker_size', 0.15)
        self.declare_parameter('target_distance', 2.0)
        self.declare_parameter('formation_dwell_s', 8.0)
        self.declare_parameter('marker_lost_timeout_s', 5.0)
        self.declare_parameter('landing_descent_rate', 0.4)
        self.declare_parameter('camera_fx', 530.0)
        self.declare_parameter('camera_fy', 530.0)
        self.declare_parameter('camera_cx', 320.0)
        self.declare_parameter('camera_cy', 240.0)
        self.declare_parameter('image_width', 640)
        self.declare_parameter('image_height', 480)
        self.declare_parameter('show_window', True)
        self.declare_parameter('control_rate_hz', 20.0)

        mode = str(self.get_parameter('mode').value).lower()
        if mode not in ('chase', 'gps_denied', 'landing'):
            raise ValueError(f"unknown mode '{mode}'")

        camera_matrix = np.array([
            [float(self.get_parameter('camera_fx').value), 0.0,
             float(self.get_parameter('camera_cx').value)],
            [0.0, float(self.get_parameter('camera_fy').value),
             float(self.get_parameter('camera_cy').value)],
            [0.0, 0.0, 1.0],
        ], dtype=np.float32)

        config = FollowerConfig(
            mode=mode,
            target_distance=float(self.get_parameter('target_distance').value),
            marker_id=int(self.get_parameter('marker_id').value),
            landing_pad_id=int(self.get_parameter('landing_pad_id').value),
            marker_size=float(self.get_parameter('marker_size').value),
            formation_dwell_s=float(
                self.get_parameter('formation_dwell_s').value),
            marker_lost_timeout_s=float(
                self.get_parameter('marker_lost_timeout_s').value),
            landing_descent_rate=float(
                self.get_parameter('landing_descent_rate').value),
            image_width=int(self.get_parameter('image_width').value),
            image_height=int(self.get_parameter('image_height').value),
        )

        self.tracker = ArucoTracker(
            dictionary_id=aruco.DICT_4X4_50,
            marker_size=config.marker_size,
            camera_matrix=camera_matrix,
        )
        self.core = FollowerCore(config=config)
        self.bridge = CvBridge()
        self.show_window = bool(self.get_parameter('show_window').value)
        self._latest_obs: Optional[MarkerObservation] = None

        # ----------------------------------------------------------- ROS I/O
        self.image_sub = self.create_subscription(
            Image,
            str(self.get_parameter('image_topic').value),
            self._image_callback,
            10,
        )
        self.vel_pub = self.create_publisher(
            Twist,
            str(self.get_parameter('cmd_vel_topic').value),
            10,
        )
        self.gimbal_pub = self.create_publisher(
            Vector3Stamped,
            str(self.get_parameter('gimbal_topic').value),
            10,
        )
        self.status_pub = self.create_publisher(
            String,
            str(self.get_parameter('status_topic').value),
            10,
        )

        rate = float(self.get_parameter('control_rate_hz').value)
        self.control_timer = self.create_timer(1.0 / rate, self._tick)

        self.get_logger().info(
            f'FollowerNode up (mode={mode}, target_distance='
            f'{config.target_distance:.2f}m, marker_id={config.marker_id}).')

        if mode == 'gps_denied':
            self.get_logger().warn(
                'Operating in GPS-DENIED mode: no /mavros/global_position '
                'or odometry subscriptions are created.')

    # ---------------------------------------------------------------- ROS cb
    def _image_callback(self, msg: Image) -> None:
        try:
            cv_image = self.bridge.imgmsg_to_cv2(msg, 'bgr8')
        except Exception as e:  # noqa: BLE001
            self.get_logger().error(f'cv_bridge failed: {e}')
            return

        wanted = self.core.expected_marker_id()
        obs = self.tracker.observe(cv_image, wanted_id=wanted)
        self._latest_obs = obs

        if self.show_window:
            try:
                annotated = self.tracker.annotate(cv_image.copy(), obs)
                cv2.imshow('Follower Camera', annotated)
                cv2.waitKey(1)
            except cv2.error:  # No display available (headless CI / Docker).
                self.show_window = False

    def _tick(self) -> None:
        cmd = self.core.update(self._latest_obs)

        twist = Twist()
        twist.linear.x = float(cmd.linear_x)
        twist.linear.y = float(cmd.linear_y)
        twist.linear.z = float(cmd.linear_z)
        twist.angular.z = float(cmd.angular_z)
        self.vel_pub.publish(twist)

        gimbal = Vector3Stamped()
        gimbal.header.stamp = self.get_clock().now().to_msg()
        gimbal.header.frame_id = 'iris_2/gimbal'
        # MAVLink mount_control convention: x = pitch, y = roll, z = yaw.
        gimbal.vector.x = float(cmd.gimbal_pitch_deg)
        gimbal.vector.y = 0.0
        gimbal.vector.z = float(cmd.gimbal_yaw_deg)
        self.gimbal_pub.publish(gimbal)

        status = String()
        status.data = (f'state={cmd.state.name} '
                       f'vx={cmd.linear_x:+.2f} '
                       f'vy={cmd.linear_y:+.2f} '
                       f'vz={cmd.linear_z:+.2f} '
                       f'yaw_rate={cmd.angular_z:+.2f} '
                       f'gimbal=({cmd.gimbal_pitch_deg:+.1f},'
                       f'{cmd.gimbal_yaw_deg:+.1f})')
        self.status_pub.publish(status)

        if cmd.state == TrackerState.MISSION_FAILED:
            self.get_logger().error(
                'Marker lost for more than '
                f'{self.core.config.marker_lost_timeout_s:.1f}s -- mission '
                'failed. Holding position.')


def main(args=None) -> None:
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
