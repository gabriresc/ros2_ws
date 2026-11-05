from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='tf2_teste',
            executable='static_tf',
            name='tf_base_seyond',
            output='screen',
            parameters=[{
                # Posição em metros.
                'x': 2.0,
                'y': 0.0,
                'z': 3.0,

                # Rotação em radianos.
                'roll': 0.0,
                'pitch': 0.0,
                'yaw': 1.57079632679,
            }],
        ),
        Node(
            package='tf2_teste',
            executable='dynamic_tf',
            name='tf2_dynamic',
            output='screen',
            parameters=[{'velocidade': 1.0}],
        ),
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            output='screen',
            arguments=['-f', 'odom'],
        ),
    ])