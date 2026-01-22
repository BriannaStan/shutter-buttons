from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration


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
    micro_ros_agent = ExecuteProcess(
        cmd=[
            'ros2', 'run', 'micro_ros_agent', 'micro_ros_agent',
            'serial',
            '--dev', LaunchConfiguration('port'),
            '-b', LaunchConfiguration('baud')
        ],
        output='screen'
    )

    return LaunchDescription([
        port_arg,
        baud_arg,
        micro_ros_agent,
    ])
