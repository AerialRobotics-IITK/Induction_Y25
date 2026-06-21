from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():

    alpha_drone = Node(
        package='drone_fleet',
        executable='drone_node',
        name='alpha_drone',
        output='screen',
        parameters=[{'initial_battery': 100.0}]
    )

    beta_drone = Node(
        package='drone_fleet',
        executable='drone_node',
        name='beta_drone',
        output='screen',
        parameters=[{'initial_battery': 60.0}]
    )

    gamma_drone = Node(
        package='drone_fleet',
        executable='drone_node',
        name='gamma_drone',
        output='screen',
        parameters=[{'initial_battery': 35.0}]
    )

    fleet_manager = Node(
        package='drone_fleet',
        executable='fleet_manager',
        name='fleet_manager',
        output='screen'
    )

    health_monitor = Node(
        package='drone_fleet',
        executable='health_monitor',
        name='health_monitor',
        output='screen'
    )

    return LaunchDescription([
        alpha_drone,
        beta_drone,
        gamma_drone,
        fleet_manager,
        health_monitor
    ])