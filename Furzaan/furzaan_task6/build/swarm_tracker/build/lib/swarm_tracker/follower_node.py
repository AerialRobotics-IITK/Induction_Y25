#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from geometry_msgs.msg import Twist
from geometry_msgs.msg import QuaternionStamped
import cv2
from cv_bridge import CvBridge
import cv2.aruco as aruco
import numpy as np
import time
from mavros_msgs.msg import MountControl

class PIDController:
    def __init__(self, kp, ki, kd, max_output=1.5, min_output=-1.5):
        # Gain constants
        self.kp = kp
        self.ki = ki
        self.kd = kd
        
        # Output bounds to protect the drone from extreme commands
        self.max_output = max_output
        self.min_output = min_output
        
        # State variables kept across time steps
        self.integral = 0.0
        self.last_error = 0.0
        self.last_time = None

    def compute(self, current_value, target_value):
        # Calculate current time and time difference (dt)
        now = time.time()
        if self.last_time is None:
            self.last_time = now
            return 0.0
        
        dt = now - self.last_time
        if dt <= 0.0:
            return 0.0  # Prevent division by zero if called too rapidly
        
        # Calculate the error
        error = target_value - current_value
        
        p_term = self.kp * error
        self.integral += error * dt
        
        # Anti-windup clamping: prevent the integral term from growing infinitely
        # if the hardware cannot keep up with the target
        integral_max = self.max_output / max(self.ki, 0.001) if self.ki > 0 else 0
        self.integral = max(min(self.integral, integral_max), -integral_max)
        i_term = self.ki * self.integral
        
        derivative = (error - self.last_error) / dt
        d_term = self.kd * derivative
        
        # Save state metrics for the next execution step
        self.last_error = error
        self.last_time = now
        
        # Total control output
        output = p_term + i_term + d_term
        
        # Saturate output within safe drone operating limits
        return max(min(output, self.max_output), self.min_output)

    def reset(self):
        """Call this when tracking is lost to clear historical errors."""
        self.integral = 0.0
        self.last_error = 0.0
        self.last_time = None

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

        self.gimbal_pub = self.create_publisher(
            MountControl,
            '/iris_2/mavros/mount_control/command',
            10
        )

        self.gimbal_orientation_sub = self.create_subscription(QuaternionStamped, '/iris_2/mavros/mount_control/orientation', self.gimbal_orientation_callback, 10)
        self.current_gimbal_yaw = 0.0
        
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
        # Initialize velocity controller
        self.linear_pid = PIDController(kp = 0.5, ki = 0.01, kd = 0.1, max_output = 1.0, min_output = -1.0)
        self.linear_y_pid = PIDController(kp = 0.1, ki = 0.005, kd = 0.08, max_output = 1.0, min_output = -1.0)
        # Initialize gimbal controllers
        self.gimbal_yaw_pid = PIDController(kp = 0.1, ki = 0.005, kd = 0.01, max_output = 0.1, min_output = -0.1)
        self.gimbal_pitch_pid = PIDController(kp = 0.01, ki = 0.005, kd = 0.07, max_output = 0.1, min_output = -0.1)

        self.marker_lost_timestamp = None

        self.get_logger().info("Follower Node initialized. Waiting for images...")

    def gimbal_orientation_callback(self, msg):
        x = msg.quaternion.x
        y = msg.quaternion.y
        z = msg.quaternion.z
        w = msg.quaternion.w

        # Convert Quaternion to Euler Yaw (rotation around Z-axis)
        siny_cosp = 2.0*(w*z + x*y)
        cosy_cosp = 1.0 - 2.0*(y*y + z*z)
        yaw_rad = np.arctan2(siny_cosp, cosy_cosp)

        # Convert radians to degrees if your mount commands use degrees
        self.current_gimbal_yaw = np.degrees(yaw_rad)

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
            self.marker_lost_timestamp = None

            aruco.drawDetectedMarkers(cv_image, corners, ids)
            
            # Estimate pose of each marker
            # Marker size is assumed to be 0.15m x 0.15m (based on SDF)
            rvecs, tvecs, _ = aruco.estimatePoseSingleMarkers(corners, 0.15, self.camera_matrix, self.dist_coeffs)
            
            for i in range(len(ids)):
                cv2.drawFrameAxes(cv_image, self.camera_matrix, self.dist_coeffs, rvecs[i], tvecs[i], 0.1)
                
                # tvecs[i][0] contains [x, y, z] distance from camera to marker
                distance = tvecs[i][0][2]
                x_offset = tvecs[i][0][0]
                
                # TODO: Implement your control logic here!
                # 1. Calculate error based on target_distance and x_offset
                # 2. Compute PID outputs for linear.x and angular.z
                # 3. Publish to self.vel_pub

                linear_x_cmd = self.linear_pid.compute(current_value = self.target_distance, target_value = distance)
                linear_y_cmd = self.linear_y_pid.compute(current_value = self.current_gimbal_yaw, target_value = 0.0)
                twist_msg  = Twist()
                twist_msg.linear_x = linear_x_cmd
                twist_msg.linear_y = linear_y_cmd

                image_height, image_width = cv_image.shape[:2]
                screen_center_x = image_width / 2.0
                screen_center_y = image_height / 2.0

                marker_corners = corners[i][0]
                marker_center_x = np.mean(marker_corners[:, 0])
                marker_center_y = np.mean(marker_corners[:, 1])
                
                pitch_output = self.gimbal_pitch_pid.compute(current_value = marker_center_y, target_value = screen_center_y)
                yaw_output = self.gimbal_yaw_pid.compute(current_value =  marker_center_x, target_value = screen_center_x)

                self.vel_pub.publish(twist_msg)
                
                gimbal_msg = MountControl()
                gimbal_msg.mode = 2

                gimbal_msg.pitch = pitch_output
                gimbal_msg.roll = 0.0
                gimbal_msg.yaw = yaw_output

                self.gimbal_pub.publish(gimbal_msg)
                self.get_logger().info(f"Marker {ids[i][0]} detected at Z: {distance:.2f}m, X: {x_offset:.2f}m")
        else:
            # TODO: Handle marker loss. Spin to search or hover?
            self.linear_pid.reset()
            self.linear_y_pid.reset()
            self.gimbal_pitch_pid.reset()
            self.gimbal_yaw_pid.reset()

            if self.marker_lost_timestamp is None:
                self.marker_lost_timestamp = time.time()
                self.get_logger().warn("ArUco marker lost! Starting 5-second failure countdown...")

            elapsed_lost_time = time.time() - self.marker_lost_timestamp
            if elapsed_lost_time >= 5.0:
                self.get_logger().error(f"MISSION FAILED: Marker out of sight for {elapsed_lost_time:.2f} seconds")
                stop_msg = Twist()
                self.vel_pub.publish(stop_msg)
            else:
                self.get_logger().warn(f"Marker missing. Time remaining: {5.0 - elapsed_lost_time: .1f}s")
                twist_msg = Twist()
                twist_msg.angular_z = 0.1
                self.vel_pub.publish(twist_msg)

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
