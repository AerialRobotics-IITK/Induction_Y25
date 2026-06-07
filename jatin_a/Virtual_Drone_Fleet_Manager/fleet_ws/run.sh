#!/bin/bash

set -e

IMAGE_NAME="drone_fleet_jazzy"

echo "🐳 Building ROS 2 Jazzy image..."
docker build -t $IMAGE_NAME .

echo "🚁 Running fleet simulation..."

docker run -it --rm \
    --net=host \
    -e ROS_DOMAIN_ID=42 \
    $IMAGE_NAME