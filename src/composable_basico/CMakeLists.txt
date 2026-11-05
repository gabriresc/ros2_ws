cmake_minimum_required(VERSION 3.8)
project(composable_basico)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)

# Um componente é compilado como biblioteca compartilhada, carregada pelo container.
add_library(meu_componente SHARED src/meu_componente.cpp)
target_compile_features(meu_componente PUBLIC cxx_std_17)
ament_target_dependencies(meu_componente rclcpp rclcpp_components std_msgs)

# Registra a classe como plugin e gera também um executável para rodar sozinho.
rclcpp_components_register_node(meu_componente
  PLUGIN "composable_basico::MeuComponente"
  EXECUTABLE meu_componente_node
)

install(TARGETS meu_componente
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

ament_package()
