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
from mavros_msgs.msg import MountControl

class PIDController:
    def __init__(self, kp, ki, kd, max_output=1.5, min_output=-1.5):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.max_output = max_output
        self.min_output = min_output
        self.integral = 0.0
        self.last_error = 0.0
        self.last_time = None

    def compute(self, current_value, target_value):
        now = time.time()
        if self.last_time is None:
            self.last_time = now
            return 0.0
        
        dt = now - self.last_time
        if dt <= 0.0:
            return 0.0
        
        error = target_value - current_value
        p_term = self.kp * error
        self.integral += error*dt
        
        # Anti-windup clamping: prevent the integral term from growing infinitely
        # if the hardware cannot keep up with the target
        integral_max = self.max_output / max(self.ki, 0.001) if self.ki > 0 else 0
        self.integral = max(min(self.integral, integral_max), -integral_max)
        i_term = self.ki * self.integral
        
        derivative = (error - self.last_error) / dt
        d_term = self.kd * derivative
        
        self.last_error = error
        self.last_time = now
        
        output = p_term + i_term + d_term
        return max(min(output, self.max_output), self.min_output)

    def reset(self):
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

        self.state = "FORMATION_FLIGHT"
        self.LEADER_ID = 0
        self.LANDING_PAD_ID = 1
        
        self.target_distance = 2.0 

        # Initialize velocity controller
        self.linear_pid = PIDController(kp = 0.5, ki = 0.01, kd = 0.1, max_output = 1.0, min_output = -1.0)
        self.linear_y_pid = PIDController(kp = 0.1, ki = 0.005, kd = 0.08, max_output = 1.0, min_output = -1.0)
        self.linear_z_pid = PIDController(kp = 0.6, ki = 0.02, kd = 0.12, max_output = 1.0, min_output = -1.0)

        # Initialize gimbal controllers
        self.gimbal_yaw_pid = PIDController(kp = 0.1, ki = 0.005, kd = 0.01, max_output = 0.1, min_output = -0.1)
        self.gimbal_pitch_pid = PIDController(kp = 0.01, ki = 0.005, kd = 0.07, max_output = 0.1, min_output = -0.1)

        self.marker_lost_timestamp = None

        self.get_logger().info("Follower Node initialized. Mode : FORMATION_FLIGHT")

    def image_callback(self, msg):
        try:
            cv_image = self.bridge.imgmsg_to_cv2(msg, "bgr8")
        except Exception as e:
            self.get_logger().error(f"Failed to convert image: {e}")
            return

        gray = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)
        
        # Detect ArUco markers
        corners, ids, rejected = aruco.detectMarkers(gray, self.aruco_dict, parameters=self.aruco_params)
        
        leader_idx = None
        landing_idx = None

        if ids is not None:
            self.marker_lost_timestamp = None

            aruco.drawDetectedMarkers(cv_image, corners, ids)
            
            # Estimate pose of each marker
            # Marker size is assumed to be 0.15m x 0.15m (based on SDF)
            rvecs, tvecs, _ = aruco.estimatePoseSingleMarkers(corners, 0.15, self.camera_matrix, self.dist_coeffs)
            
            for i in range(len(ids)):
                cv2.drawFrameAxes(cv_image, self.camera_matrix, self.dist_coeffs, rvecs[i], tvecs[i], 0.1)
                actual_id = int(ids[i][0])
                if actual_id == self.LEADER_ID:
                    leader_idx = i
                elif actual_id == self.LANDING_PAD_ID:
                    landing_idx = i
                
                # STATE 1: FORMATION FLIGHT (for subtasks 1 and 2)
                if self.state == "FORMATION_FLIGHT":
                    
                    if landing_idx is not None:
                        self.get_logger().info("Landing pad detected! Switching to PRECISION_LANDING.")
                        self.state = "PRECISION_LANDING"
                        # Reset the PID controllers to avoid previous errors being used for another marker
                        self.linear_pid.reset()
                        self.linear_y_pid.reset()
                        self.linear_z_pid.reset()
                        return 
                    
                    elif leader_idx is not None:
                        self.marker_lost_timestamp = None

                        # Extract leader vectors
                        x_offset = tvecs[i][0][0]
                        y_offset = tvecs[i][0][1]
                        distance = tvecs[i][0][2]

                        linear_x_cmd = self.linear_pid.compute(current_value = self.target_distance, target_value = distance)
                        linear_y_cmd = self.linear_y_pid.compute(current_value = x_offset, target_value = 0.0)
                        linear_z_cmd = self.linear_z_pid.compute(current_value = y_offset, target_value = 0.0)

                        twist_msg  = Twist()
                        twist_msg.linear.x = float(linear_x_cmd)
                        twist_msg.linear.y = float(linear_y_cmd)
                        twist_msg.linear.z = float(linear_z_cmd)
                        self.vel_pub.publish(twist_msg)

                        image_height, image_width = cv_image.shape[:2]

                        marker_corners = corners[i][0]
                        marker_center_x = np.mean(marker_corners[:, 0])
                        marker_center_y = np.mean(marker_corners[:, 1])

                        normalised_x = marker_center_x / image_width
                        normalised_y = marker_center_y / image_height
                
                        pitch_output = self.gimbal_pitch_pid.compute(current_value = normalised_y, target_value = 0.5)
                        yaw_output = self.gimbal_yaw_pid.compute(current_value =  normalised_x, target_value = 0.5)
                
                        gimbal_msg = MountControl()
                        gimbal_msg.mode = 2
                        gimbal_msg.pitch = float(pitch_output)
                        gimbal_msg.roll = 0.0
                        gimbal_msg.yaw = float(yaw_output)

                        self.gimbal_pub.publish(gimbal_msg)
                        self.get_logger().info(f"Marker {ids[i][0]} detected at Z: {distance:.2f}m, X: {x_offset:.2f}m")
                    
                    else:
                        self.linear_pid.reset()
                        self.linear_y_pid.reset()
                        self.linear_z_pid.reset()
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
                            twist_msg.angular.z = 0.1
                            self.vel_pub.publish(twist_msg)

                # STATE 2: PRECISION LANDING (for subtask-3)
                elif self.state == "PRECISION_LANDING":
                    if landing_idx is not None:

                        landing_pad_distance = tvecs[landing_idx][0][2]
                        
                        if landing_pad_distance < 5.0:
                            self.get_logger().info("Landing Pad in range! Switching to PRECISION_LANDING.")
                            self.state = "PRECISION_LANDING"
                            self.linear_pid.reset()
                            self.linear_y_pid.reset()
                            self.linear_z_pid.reset()
                            return
                        
                        x_offset = tvecs[landing_idx][0][2]
                        y_offset = tvecs[landing_idx][0][1]
                        distance = tvecs[landing_idx][0][2]
                        
                        linear_x_cmd = self.linear_pid.compute(current_value = y_offset, target_value = 0.0)
                        linear_y_cmd = self.linear_y_pid.compute(current_value = x_offset, target_value = 0.0)

                        twist_msg = Twist()
                        twist_msg.linear.x = float(linear_x_cmd)
                        twist_msg.linear.y = float(linear_y_cmd)
                        twist_msg.linear.z = -0.2
                        self.vel_pub.publish(twist_msg)

                        marker_center_x = np.mean(corners[landing_idx][0][:, 0])
                        marker_center_y = np.mean(corners[landing_idx][0][:, 1])
                        normalised_x = marker_center_x / image_width
                        normalised_y = marker_center_y / image_height

                        pitch_output = self.gimbal_pitch_pid.compute(current_value = normalised_y, target_value = 0.5)
                        yaw_output = self.gimbal_yaw_pid.compute(current_value = normalised_x, target_value = 0.5)

                        gimbal_msg = MountControl()
                        gimbal_msg.mode = 2
                        gimbal_msg.pitch = float(pitch_output)
                        gimbal_msg.yaw = float(yaw_output)
                        gimbal_msg.roll = 0.0

                    else:
                        self.get_logger().warn("Landing Pad Lost! Hovering...")
                        twist_msg = Twist()
                        self.vel_pub.publish(twist_msg)

                        gimbal_msg = MountControl()
                        gimbal_msg.mode = 2
                        gimbal_msg.pitch = -45.0
                        gimbal_msg.roll = 0.0
                        gimbal_msg.yaw = 0.0
                        self.gimbal_pub.publish(gimbal_msg)

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
