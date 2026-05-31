#!/bin/bash

# Docker run script for drone_fleet application
# Flags:
#   -it          : Interactive terminal
#   --rm         : Remove container after exit
#   --net=host   : Use host network
#   -e           : Set ROS_DOMAIN_ID environment variable

docker run -it --rm \
    --net=host \
    -e ROS_DOMAIN_ID=0 \
    -v "$(pwd)":/workspace \
    drone_fleet_ci_test:latest
