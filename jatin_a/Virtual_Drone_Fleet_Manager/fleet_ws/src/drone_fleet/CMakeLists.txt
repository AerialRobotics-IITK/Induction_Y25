cmake_minimum_required(VERSION 3.8)
project(drone_fleet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)


# Build Part1 as a library
add_library(drone_core
  # /home/clutchballer_08/Induction_Y25/jatin_a/Virtual_Drone_Fleet_Manager/part1/src/drone.cpp
  # /home/clutchballer_08/Induction_Y25/jatin_a/Virtual_Drone_Fleet_Manager/part1/src/vehicle.cpp
  # /home/clutchballer_08/Induction_Y25/jatin_a/Virtual_Drone_Fleet_Manager/part1/src/mission_drone.cpp
  # /home/clutchballer_08/Induction_Y25/jatin_a/Virtual_Drone_Fleet_Manager/part1/src/autonomous_drone.cpp
  /ws/part1/src/drone.cpp
  /ws/part1/src/vehicle.cpp
  /ws/part1/src/mission_drone.cpp
  /ws/part1/src/autonomous_drone.cpp

)
target_include_directories(drone_core PUBLIC
  /ws/part1/include
)


# ROS2 executables
add_executable(drone_node src/drone_node.cpp)
add_executable(fleet_manager src/fleet_manager.cpp)
add_executable(health_monitor src/health_monitor.cpp)

# Link libraries
target_link_libraries(drone_node drone_core)
target_link_libraries(fleet_manager drone_core)
target_link_libraries(health_monitor drone_core)

# ROS dependencies
ament_target_dependencies(drone_core rclcpp)
ament_target_dependencies(drone_node rclcpp std_msgs)

ament_target_dependencies(fleet_manager
  rclcpp
  std_msgs
  std_srvs
)

ament_target_dependencies(health_monitor
  rclcpp
  std_msgs
)

# Install
install(TARGETS
  drone_node
  fleet_manager
  health_monitor
  DESTINATION lib/${PROJECT_NAME}
)
install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)
ament_package() 

