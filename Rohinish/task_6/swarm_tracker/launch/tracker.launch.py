"""Launch the swarm tracker.

Examples
--------
ros2 launch swarm_tracker tracker.launch.py
ros2 launch swarm_tracker tracker.launch.py mode:=gps_denied
ros2 launch swarm_tracker tracker.launch.py mode:=landing landing_pad_id:=5
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description() -> LaunchDescription:
    mode = LaunchConfiguration('mode')
    target_distance = LaunchConfiguration('target_distance')
    marker_id = LaunchConfiguration('marker_id')
    landing_pad_id = LaunchConfiguration('landing_pad_id')
    marker_size = LaunchConfiguration('marker_size')
    image_topic = LaunchConfiguration('image_topic')
    cmd_vel_topic = LaunchConfiguration('cmd_vel_topic')
    gimbal_topic = LaunchConfiguration('gimbal_topic')
    show_window = LaunchConfiguration('show_window')
    run_leader = LaunchConfiguration('run_leader_evasion')

    declared_args = [
        DeclareLaunchArgument('mode', default_value='chase',
                              description='chase | gps_denied | landing'),
        DeclareLaunchArgument('target_distance', default_value='2.0'),
        DeclareLaunchArgument('marker_id', default_value='0'),
        DeclareLaunchArgument('landing_pad_id', default_value='5'),
        DeclareLaunchArgument('marker_size', default_value='0.15'),
        DeclareLaunchArgument('image_topic',
                              default_value='/iris_2/camera/image'),
        DeclareLaunchArgument(
            'cmd_vel_topic',
            default_value='/iris_2/mavros/setpoint_velocity/'
                          'cmd_vel_unstamped'),
        DeclareLaunchArgument(
            'gimbal_topic',
            default_value='/iris_2/mavros/mount_control/command'),
        DeclareLaunchArgument('show_window', default_value='true'),
        DeclareLaunchArgument(
            'run_leader_evasion', default_value='true',
            description='Whether to also start the provided leader '
                        'evasion node (Subtask 1).'),
    ]

    follower = Node(
        package='swarm_tracker',
        executable='follower_node',
        name='follower_node',
        output='screen',
        parameters=[{
            'mode': mode,
            'target_distance': target_distance,
            'marker_id': marker_id,
            'landing_pad_id': landing_pad_id,
            'marker_size': marker_size,
            'image_topic': image_topic,
            'cmd_vel_topic': cmd_vel_topic,
            'gimbal_topic': gimbal_topic,
            'show_window': show_window,
        }],
    )

    leader = Node(
        package='swarm_tracker',
        executable='leader_evasion',
        name='leader_evasion_node',
        output='screen',
        condition=IfCondition(run_leader),
    )

    return LaunchDescription(declared_args + [follower, leader])
