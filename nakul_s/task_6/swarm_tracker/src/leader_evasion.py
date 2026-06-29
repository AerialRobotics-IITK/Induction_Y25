#!/usr/bin/env python3
"""Aggressive leader trajectory for the cat-and-mouse subtask."""

import math
import time

try:
    import rclpy
    from geometry_msgs.msg import Twist
    from rclpy.node import Node
except ImportError:
    rclpy = None
    Twist = None
    Node = object


def ensure_runtime_dependencies() -> None:
    """Raise a clear error if ROS 2 dependencies are unavailable."""
    if rclpy is None or Twist is None:
        raise RuntimeError(
            "leader_evasion requires ROS 2 Python packages at runtime."
        )


class LeaderEvasionNode(Node):
    """Publish a figure-eight velocity pattern for iris_1."""

    def __init__(self) -> None:
        ensure_runtime_dependencies()
        super().__init__("leader_evasion_node")
        self.vel_pub = self.create_publisher(
            Twist,
            "/iris_1/mavros/setpoint_velocity/cmd_vel_unstamped",
            10,
        )
        self.start_time = time.time()
        self.timer = self.create_timer(0.1, self.timer_callback)
        self.get_logger().info("Leader evasion node started.")

    def timer_callback(self) -> None:
        t = time.time() - self.start_time
        msg = Twist()
        msg.linear.x = 2.0 * math.sin(0.5 * t)
        msg.linear.y = 1.5 * math.cos(0.2 * t)
        msg.linear.z = 0.5 * math.sin(0.3 * t)
        msg.angular.z = 0.5 * math.cos(0.4 * t)
        self.vel_pub.publish(msg)


def main(args=None) -> None:
    ensure_runtime_dependencies()
    rclpy.init(args=args)
    node = LeaderEvasionNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
