"""
tracker.launch.py  —  ARIITK Task 6 (final)
═════════════════════════════════════════════
Launches:
  1. gz_ros2_bridge  — bridges Gazebo ↔ ROS 2
       • iris_2 gimbal camera image
       • iris_2 gimbal commands (/iris_2/gimbal/cmd_*)
  2. leader_evasion  — drives iris_1 on evasion trajectory
  3. follower_node   — ArUco detection + PID control for iris_2

Usage:
  ros2 launch swarm_tracker tracker.launch.py mode:=chase
  ros2 launch swarm_tracker tracker.launch.py mode:=gps_denied
  ros2 launch swarm_tracker tracker.launch.py mode:=landing
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, LogInfo
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

# ── Gazebo camera topic ────────────────────────────────────────────────────────
# Matches world name "iris_runway" and iris_2 model hierarchy in task_6.sdf
GZ_CAM_TOPIC = (
    "/world/iris_runway/model/iris_2/model/gimbal"
    "/link/pitch_link/sensor/camera/image"
)


def generate_launch_description():

    mode_arg = DeclareLaunchArgument(
        'mode',
        default_value='chase',
        description='Operating mode: chase | gps_denied | landing')

    # ── gz_ros2_bridge ────────────────────────────────────────────────────────
    # Bridges Gazebo topics to/from ROS 2.
    # Format for each entry:
    #   gz_topic@ros_type[gz_type   →  Gazebo to ROS 2  (sensor data)
    #   gz_topic@ros_type]gz_type   →  ROS 2 to Gazebo  (commands)
    gz_bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        name='gz_ros2_bridge',
        output='screen',
        arguments=[
            # Camera image: Gazebo → ROS 2
            GZ_CAM_TOPIC
            + '@sensor_msgs/msg/Image'
            + '[gz.msgs.Image',

            # Gimbal commands: ROS 2 → Gazebo (namespaced to iris_2)
            '/iris_2/gimbal/cmd_pitch@std_msgs/msg/Float64]gz.msgs.Double',
            '/iris_2/gimbal/cmd_roll@std_msgs/msg/Float64]gz.msgs.Double',
            '/iris_2/gimbal/cmd_yaw@std_msgs/msg/Float64]gz.msgs.Double',
        ],
    )

    # ── leader_evasion ────────────────────────────────────────────────────────
    leader_node = Node(
        package='swarm_tracker',
        executable='leader_evasion',
        name='leader_evasion',
        output='screen',
        parameters=[{'mode': LaunchConfiguration('mode')}],
    )

    # ── follower_node ─────────────────────────────────────────────────────────
    follower_node = Node(
        package='swarm_tracker',
        executable='follower_node',
        name='follower_node',
        output='screen',
        parameters=[{'mode': LaunchConfiguration('mode')}],
    )

    return LaunchDescription([
        mode_arg,
        LogInfo(msg=['[swarm_tracker] Launching in mode: ', LaunchConfiguration('mode')]),
        gz_bridge,
        leader_node,
        follower_node,
    ])
