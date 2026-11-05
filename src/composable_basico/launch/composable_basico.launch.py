"""Inicia um container e carrega nele o componente do exemplo."""

from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode


def generate_launch_description():
    container = ComposableNodeContainer(
        name='meu_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='composable_basico',
                plugin='composable_basico::MeuComponente',
                name='meu_componente',
            ),
        ],
        output='screen',
    )

    return LaunchDescription([container])
