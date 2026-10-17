"""Launch both nodes for the cat-and-mouse subtask.

Run the leader's evasion script and the follower's vision/tracking node
together. Override topic names or intrinsics from the CLI, e.g.:

    ros2 launch swarm_tracker tracker.launch.py \\
        image_topic:=/iris_2/camera/image_raw show_window:=true
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    image_topic_arg = DeclareLaunchArgument(
        'image_topic',
        default_value='/camera/image_raw',
        description='Gimbal camera image topic for iris_2.',
    )
    cmd_vel_arg = DeclareLaunchArgument(
        'cmd_vel_topic',
        default_value='/iris_2/mavros/setpoint_velocity/cmd_vel_unstamped',
        description='MAVROS velocity setpoint topic for iris_2.',
    )
    show_window_arg = DeclareLaunchArgument(
        'show_window',
        default_value='true',
        description='Show the annotated OpenCV preview window.',
    )
    run_leader_arg = DeclareLaunchArgument(
        'run_leader',
        default_value='true',
        description='Start the leader_evasion node alongside the follower.',
    )

    follower = Node(
        package='swarm_tracker',
        executable='follower_node',
        name='follower_node',
        output='screen',
        parameters=[{
            'image_topic': LaunchConfiguration('image_topic'),
            'cmd_vel_topic': LaunchConfiguration('cmd_vel_topic'),
            'show_window': LaunchConfiguration('show_window'),
        }],
    )

    leader = Node(
        package='swarm_tracker',
        executable='leader_evasion',
        name='leader_evasion_node',
        output='screen',
        condition=IfCondition(LaunchConfiguration('run_leader')),
    )

    return LaunchDescription([
        image_topic_arg,
        cmd_vel_arg,
        show_window_arg,
        run_leader_arg,
        follower,
        leader,
    ])
