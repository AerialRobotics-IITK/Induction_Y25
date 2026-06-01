#!/bin/bash
set -e

IMAGE_NAME="drone_fleet:latest"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=== Building Docker image ==="
docker build -t "$IMAGE_NAME" "$SCRIPT_DIR"

echo ""
echo "=== Launching Drone Fleet ==="
echo "  - Alpha  (battery: 100%)"
echo "  - Beta   (battery:  60%)"
echo "  - Gamma  (battery:  35%)"
echo ""

docker run -it --rm \
    --net=host \
    -e ROS_DOMAIN_ID=42 \
    -v "$SCRIPT_DIR/src:/ros2_ws/src:ro" \
    "$IMAGE_NAME"
