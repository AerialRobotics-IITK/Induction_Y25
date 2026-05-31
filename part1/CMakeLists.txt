cmake_minimum_required(VERSION 3.10)
project(DroneSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(drone_sim src/main.cpp)
target_include_directories(drone_sim PRIVATE include)
