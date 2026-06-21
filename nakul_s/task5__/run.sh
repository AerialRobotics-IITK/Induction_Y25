#!/bin/bash

docker build -t drone_fleet .

docker run \
  --rm \
  -it \
  --net=host \
  -e ROS_DOMAIN_ID=42 \
  -v $(pwd)/src:/fleet_ws/src \
  drone_fleet \
  ros2 launch drone_fleet fleet.launch.py
