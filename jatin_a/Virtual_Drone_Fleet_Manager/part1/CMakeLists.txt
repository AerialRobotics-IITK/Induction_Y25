cmake_minimum_required(VERSION 3.10)
project(part1)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra)

include_directories(include)

add_executable(main
    src/main.cpp
    src/vehicle.cpp
    src/drone.cpp
    src/mission_drone.cpp
    src/autonomous_drone.cpp
)


