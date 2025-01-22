cmake_minimum_required(VERSION 3.5)
project(cmd_vel_publisher_cpp)

# C++ のバージョンを指定
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# 必要なパッケージをfind_packageで探す
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(cmd_vel_publisher_cpp src/cmd_vel_publisher.cpp)

ament_target_dependencies(cmd_vel_publisher_cpp
  rclcpp
  geometry_msgs
)

install(TARGETS
  cmd_vel_publisher_cpp
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()

