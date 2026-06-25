#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from geometry_msgs.msg import Twist
import cv2
from cv_bridge import CvBridge
import cv2.aruco as aruco
import numpy as np
import time

class FollowerNode(Node):
    def __init__(self):
        super().__init__('follower_node')
        
        # Subscribe to the gimbal camera image topic from iris_2
        # TODO: Update with the correct topic name for your setup
        self.image_sub = self.create_subscription(
            Image,
            '/camera/image_raw',
            self.image_callback,
            10
        )
        
        # Publisher for velocity commands via MAVROS
        # TODO: Ensure the drone is in GUIDED/OFFBOARD mode and armed before publishing
        self.vel_pub = self.create_publisher(
            Twist,
            '/iris_2/mavros/setpoint_velocity/cmd_vel_unstamped',
            10
        )
        
        self.bridge = CvBridge()
        
        # ArUco dictionary and parameters
        self.aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_4X4_50)
        self.aruco_params = aruco.DetectorParameters()
        
        # Camera intrinsic matrix (Assume standard parameters or calibrate)
        # TODO: Update with actual camera intrinsics from Gazebo
        self.camera_matrix = np.array([[530.0, 0.0, 320.0],
                                       [0.0, 530.0, 240.0],
                                       [0.0, 0.0, 1.0]])
        self.dist_coeffs = np.zeros((4,1))
        
        self.target_distance = 2.0 # meters

        self.kp_dist = 0.8
        self.ki_dist = 0.0
        self.kd_dist = 0.15
        self.kp_yaw = 1.2
        self.ki_yaw = 0.0
        self.kd_yaw = 0.2
        self.prev_dist_error = 0.0
        self.prev_yaw_error = 0.0
        self.dist_integral = 0.0
        self.yaw_integral = 0.0
        self.last_time = time.time()
        self.last_seen = time.time()
        self.get_logger().info("Follower Node initialized. Waiting for images...")

    def image_callback(self, msg):
        try:
            cv_image = self.bridge.imgmsg_to_cv2(msg, "bgr8")
        except Exception as e:
            self.get_logger().error(f"Failed to convert image: {e}")
            return

        gray = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)
        
        # Detect ArUco markers
        corners, ids, rejected = aruco.detectMarkers(gray, self.aruco_dict, parameters=self.aruco_params)
        
        if ids is not None:
            aruco.drawDetectedMarkers(cv_image, corners, ids)
            
            # Estimate pose of each marker
            # Marker size is assumed to be 0.15m x 0.15m (based on SDF)
            rvecs, tvecs, _ = aruco.estimatePoseSingleMarkers(corners, 0.15, self.camera_matrix, self.dist_coeffs)
            
            for i in range(len(ids)):
                cv2.drawFrameAxes(cv_image, self.camera_matrix, self.dist_coeffs, rvecs[i], tvecs[i], 0.1)
                distance = tvecs[i][0][2]
                x_offset = tvecs[i][0][0]
                cv2.putText(
                     cv_image,
                     f"Dist: {distance:.2f} m",
                     (20, 30),
                     cv2.FONT_HERSHEY_SIMPLEX,
                     0.8,
                     (0, 255, 0),
                     2
                    )

                cv2.putText(
                    cv_image,
                    f"X Offset: {x_offset:.2f} m",
                    (20, 60),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.8,
                    (0, 255, 0),
                    2
                    )
                # tvecs[i][0] contains [x, y, z] distance from camera to marker
                
                
                # TODO: Implement your control logic here!
                # 1. Calculate error based on target_distance and x_offset
                # 2. Compute PID outputs for linear.x and angular.z
                # 3. Publish to self.vel_pub
                current_time = time.time()

                dt = current_time - self.last_time

                if dt <= 0:
                   dt = 0.01

                self.last_seen = current_time

                distance_error = distance - self.target_distance
                yaw_error = x_offset

                self.dist_integral += distance_error * dt
                self.yaw_integral += yaw_error * dt

                dist_derivative = (distance_error - self.prev_dist_error) / dt

                yaw_derivative = (yaw_error - self.prev_yaw_error) / dt

                linear_x = (self.kp_dist * distance_error+ self.ki_dist * self.dist_integral+ self.kd_dist * dist_derivative)

                angular_z = -(self.kp_yaw * yaw_error+ self.ki_yaw * self.yaw_integral+ self.kd_yaw * yaw_derivative)

                cmd = Twist()

                cmd.linear.x = max(min(linear_x, 2.0), -2.0)
                cmd.angular.z = max(min(angular_z, 1.0), -1.0)
                self.vel_pub.publish(cmd)
                self.prev_dist_error = distance_error
                self.prev_yaw_error = yaw_error
                self.last_time = current_time

                self.get_logger().info(
                    f"Dist={distance:.2f} "
                    f"Offset={x_offset:.2f} "
                    f"CmdX={cmd.linear.x:.2f} "
                    f"CmdYaw={cmd.angular.z:.2f}"
                    )
                
                
        else:
            # TODO: Handle marker loss. Spin to search or hover?
            current_time = time.time()

            if current_time - self.last_seen > 5.0:
                self.get_logger().error("Marker lost for more than 5 seconds!")
                cmd = Twist()
                self.vel_pub.publish(cmd)

            else:
                cmd = Twist()
                cmd.angular.z = 0.3
                self.vel_pub.publish(cmd)

        # Display the feed (ensure you have an X-server running if doing this in Docker)
        cv2.imshow("Follower Camera", cv_image)
        cv2.waitKey(1)

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
