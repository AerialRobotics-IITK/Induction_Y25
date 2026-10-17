cmake_minimum_required(VERSION 3.10)
project(DroneOOP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Strict warnings this must compile with zero warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Headers live in the include/ folder
include_directories(include)

add_executable(drone_sim main.cpp)