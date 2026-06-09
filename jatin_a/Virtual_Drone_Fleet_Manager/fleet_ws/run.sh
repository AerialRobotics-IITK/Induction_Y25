#!/bin/bash

set -e

IMAGE_NAME="drone_fleet"

echo "Building ROS 2 image..."
docker build -t $IMAGE_NAME .

echo "Running container"

docker run -it --rm \
    --net=host \
    -e ROS_DOMAIN_ID=42 \
    $IMAGE_NAME