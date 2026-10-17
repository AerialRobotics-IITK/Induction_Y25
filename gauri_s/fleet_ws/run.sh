#!/bin/bash
# run.sh
# Builds the Docker image (if not already built) and starts the fleet.
# Usage:
#   ./run.sh            -- launch all five nodes
#   ./run.sh bash       -- open a shell inside the container instead

set -e
# set -e: exit this script immediately if any command returns a non-zero exit code.
# Prevents silent failures (e.g. if docker build fails, we stop here, not at run).

IMAGE_NAME="drone_fleet"
# IMAGE_NAME is the local Docker image tag we will build and run.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# BASH_SOURCE[0] is the path to this script file.
# dirname strips the filename, leaving the directory.
# cd + pwd resolves symlinks and gives an absolute path.
# We need the absolute path so the volume mount below works
# regardless of which directory the user runs ./run.sh from.

# Build the image only if it does not already exist.
# "docker images -q" returns the image ID if found, empty string if not.
if [[ "$(docker images -q "$IMAGE_NAME" 2>/dev/null)" == "" ]]; then
    echo "[run.sh] Image '$IMAGE_NAME' not found. Building..."
    docker build -t "$IMAGE_NAME" "$SCRIPT_DIR"
    # -t "$IMAGE_NAME" tags the image with our chosen name.
    # "$SCRIPT_DIR" is the build context (where Docker looks for the Dockerfile
    # and any files referenced by COPY instructions).
else
    echo "[run.sh] Image '$IMAGE_NAME' found. Skipping build."
    echo "[run.sh] To force a rebuild: docker rmi $IMAGE_NAME && ./run.sh"
fi

echo "[run.sh] Starting fleet..."

# Determine the command to run inside the container.
# If the user passed arguments to run.sh (e.g. ./run.sh bash),
# use those. Otherwise use the default launch command.
if [[ $# -gt 0 ]]; then
    # $# is the number of arguments passed to run.sh.
    # $@ expands to all of them.
    CMD=("$@")
else
    CMD=("ros2" "launch" "drone_fleet" "fleet.launch.py")
fi

docker run \
    --rm \
    -it \
    --net=host \
    -e ROS_DOMAIN_ID=42 \
    -v "$SCRIPT_DIR/src":/ros2_ws/src:ro \
    "$IMAGE_NAME" \
    "${CMD[@]}"

# Flag explanations:
#
# --rm
#   Remove the container automatically when it exits.
#   Keeps your system clean; no leftover stopped containers.
#
# -it
#   -i (interactive): keeps stdin open so Ctrl+C works.
#   -t (tty): allocates a pseudo-terminal so output is formatted
#   correctly and tools like less work inside the container.
#
# --net=host
#   Shares the host's network stack instead of creating a
#   separate virtual network. Required for ROS 2 DDS discovery:
#   nodes in the container and nodes on the host (or other
#   containers with --net=host) can find each other automatically.
#   Without this, ros2 topic list / ros2 node list from outside
#   the container would see nothing.
#
# -e ROS_DOMAIN_ID=42
#   Sets a ROS 2 environment variable inside the container.
#   ROS_DOMAIN_ID isolates your DDS traffic from other ROS 2
#   users on the same network. All nodes must share the same
#   domain ID to communicate. 42 is arbitrary; just be consistent.
#
# -v "$SCRIPT_DIR/src":/ros2_ws/src:ro
#   Mounts your local src/ folder into the container at /ros2_ws/src.
#   :ro means read-only inside the container (the container cannot
#   accidentally modify your source files).
#   Effect: you can edit source files in VS Code on your laptop
#   and the container sees the changes immediately. You still need
#   to rebuild (colcon build) for C++ changes to take effect, but
#   Python launch files and scripts update instantly without a rebuild.
