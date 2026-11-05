cmake_minimum_required(VERSION 3.8)
project(custom_action_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(custom_action_interfaces REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)

add_executable(action_server src/fibonacci_action_server.cpp)
add_executable(action_client src/fibonacci_action_client.cpp)

ament_target_dependencies(action_server
  "custom_action_interfaces"
  "rclcpp"
  "rclcpp_action"
)

ament_target_dependencies(action_client
  "custom_action_interfaces"
  "rclcpp"
  "rclcpp_action"
)

install(TARGETS
  action_server
  action_client
  DESTINATION lib/${PROJECT_NAME}
)
  
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()