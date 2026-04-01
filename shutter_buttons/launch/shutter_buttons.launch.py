from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

from launch_ros.actions import Node


def generate_launch_description():
    # Declare arguments
    port_arg = DeclareLaunchArgument(
        'port',
        default_value='/dev/ttyACM0',
        description='Serial port for micro-ROS agent'
    )

    baud_arg = DeclareLaunchArgument(
        'baud',
        default_value='115200',
        description='Baud rate for serial communication'
    )

    # micro-ROS agent process
    micro_ros_agent = Node(
        package="micro_ros_agent",
        executable="micro_ros_agent",
        arguments=["serial", "--dev", LaunchConfiguration("port"), "-b", LaunchConfiguration("baud")]
    )

    return LaunchDescription([
        port_arg,
        baud_arg,
        micro_ros_agent,
    ])
