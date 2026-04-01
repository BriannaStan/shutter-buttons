from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='shutter_buttons',
            executable='button_board',
            name='button_board',
            output='screen',
        ),
    ])
