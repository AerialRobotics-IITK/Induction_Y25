#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
import math
import time

class LeaderEvasionNode(Node):
    def __init__(self):
        super().__init__('leader_evasion_node')
        
        # Publisher for velocity commands via MAVROS
        self.vel_pub = self.create_publisher(
            Twist,
            '/iris_1/mavros/setpoint_velocity/cmd_vel_unstamped',
            10
        )
        
        self.timer = self.create_timer(0.1, self.timer_callback)
        self.start_time = time.time()
        self.get_logger().info("Leader Evasion Node started. Executing maneuvers...")

    def timer_callback(self):
        t = time.time() - self.start_time
        
        msg = Twist()
        # Complex figure-eight and altitude changing maneuver
        msg.linear.x = 2.0 * math.sin(0.5 * t)
        msg.linear.y = 1.5 * math.cos(0.2 * t)
        msg.linear.z = 0.5 * math.sin(0.3 * t)
        msg.angular.z = 0.5 * math.cos(0.4 * t)
        
        self.vel_pub.publish(msg)

def main(args=None):
    rclpy.init(args=args)
    node = LeaderEvasionNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
