from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(package='warehouse_dispatch', executable='dispatcher',
             output='screen'),
        Node(package='warehouse_dispatch', executable='simulator',
             output='screen'),
    ])
