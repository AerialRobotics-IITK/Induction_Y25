cmake_minimum_required(VERSION 3.8)
project(drone_fleet)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)

include_directories(include)

# --------------------
# Core Drone Library
# --------------------

add_library(drone_core
  src/Vehicle.cpp
  src/Drone.cpp
  src/MissionDrone.cpp
  src/AutonomousDrone.cpp
  src/Utils.cpp
)

target_include_directories(drone_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(
  drone_core
  rclcpp
  std_msgs
  std_srvs
)

# --------------------
# Drone Node Executable
# --------------------

add_executable(
  drone_node
  src/drone_node.cpp
)

target_link_libraries(
  drone_node
  drone_core
)

ament_target_dependencies(
  drone_node
  rclcpp
  std_msgs
)

# --------------------
# Install
# --------------------

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS
    drone_core
    drone_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

ament_package()