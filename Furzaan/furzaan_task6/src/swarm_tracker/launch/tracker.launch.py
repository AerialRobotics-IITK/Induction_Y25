#!/usr/bin/env python3

import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

def generate_launch_description():
    # Path to the standard MAVROS launch implementation
    mavros_launch_dir = os.path.join(get_package_share_directory('mavros'), 'launch')
    apm_launch_file = os.path.join(mavros_launch_dir, 'apm.launch')

    # 1. MAVROS for iris_1 (Leader)
    mavros_iris_1 = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(apm_launch_file),
        launch_arguments={
            'fcu_url': 'udp://127.0.0.1:14550@14555',
            'tgt_system': '1',
            'namespace': 'iris_1'
        }.items()
    )

    # 2. MAVROS for iris_2 (Follower)
    mavros_iris_2 = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(apm_launch_file),
        launch_arguments={
            'fcu_url': 'udp://127.0.0.1:14560@14565',
            'tgt_system': '2',
            'namespace': 'iris_2'
        }.items()
    )

    # 3. Your Custom Tracking Node
    follower_node = Node(
        package='swarm_tracker',
        executable='follower_node',
        name='follower_node',
        output='screen'
    )

    # 4. Provided Evasion Node (Delayed slightly to allow tracking loops to arm)
    leader_evasion_node = Node(
        package='swarm_tracker',
        executable='leader_evasion',
        name='leader_evasion_node',
        output='screen'
    )
    
    delayed_leader_evasion = TimerAction(
        period=5.0,
        actions=[leader_evasion_node]
    )

    return LaunchDescription([
        mavros_iris_1,
        mavros_iris_2,
        follower_node,
        delayed_leader_evasion
    ])
