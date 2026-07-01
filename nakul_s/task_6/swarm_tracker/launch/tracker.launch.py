"""Launch the Task 6 vision follower and optional leader evasion node."""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    image_topic = DeclareLaunchArgument(
        "image_topic",
        default_value="/iris_2/gimbal_camera",
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

    landing_trigger = DeclareLaunchArgument(
        "landing_trigger_topic",
        default_value="/swarm_tracker/start_landing",
    )
    mission_fail = DeclareLaunchArgument(
        "mission_fail_topic",
        default_value="/swarm_tracker/mission_failed",
    )

    leader_marker_id = DeclareLaunchArgument(
        "leader_marker_id",
        default_value="0",
    )
    landing_marker_id = DeclareLaunchArgument(
        "landing_marker_id",
        default_value="1",
    )

    leader_marker_size = DeclareLaunchArgument(
        "leader_marker_size_m",
        default_value="0.30",
    )
    landing_marker_size = DeclareLaunchArgument(
        "landing_marker_size_m",
        default_value="1.0",
    )

    target_distance = DeclareLaunchArgument(
        "target_distance_m",
        default_value="2.0",
    )

    lost_search = DeclareLaunchArgument(
        "lost_search_after_s",
        default_value="1.0",
    )
    lost_fail = DeclareLaunchArgument(
        "lost_fail_after_s",
        default_value="5.0",
    )

    touchdown_distance = DeclareLaunchArgument(
        "landing_touchdown_distance_m",
        default_value="0.35",
    )

    fx = DeclareLaunchArgument("fx", default_value="530.0")
    fy = DeclareLaunchArgument("fy", default_value="530.0")
    cx = DeclareLaunchArgument("cx", default_value="320.0")
    cy = DeclareLaunchArgument("cy", default_value="240.0")

    mount_control = DeclareLaunchArgument(
        "mount_control_topic",
        default_value="/iris_2/mavros/mount_control/command",
    )

    gimbal_roll = DeclareLaunchArgument(
        "gimbal_roll_topic",
        default_value="/gimbal/cmd_roll",
    )
    gimbal_pitch = DeclareLaunchArgument(
        "gimbal_pitch_topic",
        default_value="/gimbal/cmd_pitch",
    )
    gimbal_yaw = DeclareLaunchArgument(
        "gimbal_yaw_topic",
        default_value="/gimbal/cmd_yaw",
    )

    follower = Node(
        package="swarm_tracker",
        executable="follower_node",
        name="follower_node",
        output="screen",
        parameters=[{
            "image_topic": LaunchConfiguration("image_topic"),
            "cmd_vel_topic": LaunchConfiguration("cmd_vel_topic"),
            "show_window": LaunchConfiguration("show_window"),

            "landing_trigger_topic": LaunchConfiguration("landing_trigger_topic"),
            "mission_fail_topic": LaunchConfiguration("mission_fail_topic"),

            "leader_marker_id": LaunchConfiguration("leader_marker_id"),
            "landing_marker_id": LaunchConfiguration("landing_marker_id"),

            "leader_marker_size_m": LaunchConfiguration("leader_marker_size_m"),
            "landing_marker_size_m": LaunchConfiguration("landing_marker_size_m"),

            "target_distance_m": LaunchConfiguration("target_distance_m"),

            "lost_search_after_s": LaunchConfiguration("lost_search_after_s"),
            "lost_fail_after_s": LaunchConfiguration("lost_fail_after_s"),

            "landing_touchdown_distance_m":
                LaunchConfiguration("landing_touchdown_distance_m"),

            "fx": LaunchConfiguration("fx"),
            "fy": LaunchConfiguration("fy"),
            "cx": LaunchConfiguration("cx"),
            "cy": LaunchConfiguration("cy"),

            "mount_control_topic":
                LaunchConfiguration("mount_control_topic"),

            "gimbal_roll_topic":
                LaunchConfiguration("gimbal_roll_topic"),

            "gimbal_pitch_topic":
                LaunchConfiguration("gimbal_pitch_topic"),

            "gimbal_yaw_topic":
                LaunchConfiguration("gimbal_yaw_topic"),
        }],
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

        landing_trigger,
        mission_fail,

        leader_marker_id,
        landing_marker_id,

        leader_marker_size,
        landing_marker_size,

        target_distance,

        lost_search,
        lost_fail,

        touchdown_distance,

        fx,
        fy,
        cx,
        cy,

        mount_control,

        gimbal_roll,
        gimbal_pitch,
        gimbal_yaw,

        follower,
        leader,
    ])