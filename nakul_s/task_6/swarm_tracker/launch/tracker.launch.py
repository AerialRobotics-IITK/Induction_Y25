"""Launch the Task 6 vision follower and optional leader evasion node."""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    image_topic = DeclareLaunchArgument(
        "image_topic",
        default_value="/camera/image_raw",
        description="iris_2 gimbal camera image topic.",
    )
    cmd_vel_topic = DeclareLaunchArgument(
        "cmd_vel_topic",
        default_value="/iris_2/mavros/setpoint_velocity/cmd_vel_unstamped",
        description="iris_2 MAVROS velocity setpoint topic.",
    )
    show_window = DeclareLaunchArgument(
        "show_window",
        default_value="true",
        description="Show the annotated OpenCV image feed.",
    )
    run_leader = DeclareLaunchArgument(
        "run_leader",
        default_value="true",
        description="Run the iris_1 leader evasion node.",
    )

    follower = Node(
        package="swarm_tracker",
        executable="follower_node",
        name="follower_node",
        output="screen",
        parameters=[
            {
                "image_topic": LaunchConfiguration("image_topic"),
                "cmd_vel_topic": LaunchConfiguration("cmd_vel_topic"),
                "show_window": LaunchConfiguration("show_window"),
            }
        ],
    )

    leader = Node(
        package="swarm_tracker",
        executable="leader_evasion",
        name="leader_evasion_node",
        output="screen",
        condition=IfCondition(LaunchConfiguration("run_leader")),
    )

    return LaunchDescription([
        image_topic,
        cmd_vel_topic,
        show_window,
        run_leader,
        follower,
        leader,
    ])
