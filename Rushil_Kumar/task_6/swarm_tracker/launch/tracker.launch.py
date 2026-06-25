from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    return LaunchDescription([
        Node(
            package='swarm_tracker',
            executable='follower_node',   
            output='screen'
        ),
        Node(
            package='swarm_tracker',
            executable='leader_evasion',  
            output='screen'
        ),
    ])