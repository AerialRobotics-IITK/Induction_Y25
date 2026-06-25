#!/usr/bin/env python3

import os
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    follower_node = Node(
        package='swarm_tracker',
        executable='follower_node.py',
        name='follower_node',
        output='screen',
        emulate_tty=True,
        parameters=[

        ]
    )

    leader_node = Node(
        package='swarm_tracker',
        executable='leader_evasion.py',
        name='leader_evasion_node',
        output='screen',
        emulate_tty=True
    )

    return LaunchDescription([
        follower_node,
        leader_node
    ])