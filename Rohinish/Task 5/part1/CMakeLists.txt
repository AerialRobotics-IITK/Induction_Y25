cmake_minimum_required(VERSION 3.14)
project(DroneFleetOOP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(drone_fleet_demo
    src/Vehicle.cpp
    src/Drone.cpp
    src/MissionDrone.cpp
    src/AutonomousDrone.cpp
    src/main.cpp
)
