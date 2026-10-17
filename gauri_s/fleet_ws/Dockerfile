# =============================================================
# Stage 1: builder
# Installs all build dependencies, copies source, runs colcon.
# This stage is large (compilers, headers, etc.) but is thrown
# away before the final image is produced.
# =============================================================
FROM osrf/ros:humble-desktop AS builder

# Install colcon and any extra build tools.
# --no-install-recommends keeps the layer smaller by skipping
# optional packages that are not actually needed at build time.
RUN apt-get update && apt-get install -y --no-install-recommends \
    python3-colcon-common-extensions \
    python3-rosdep \
    && rm -rf /var/lib/apt/lists/*
# rm -rf /var/lib/apt/lists/* deletes the apt package index cache
# so it does not bloat the image layer.

# Set the working directory inside the container.
# All subsequent COPY / RUN commands that use relative paths
# will resolve relative to /ros2_ws.
WORKDIR /ros2_ws

# Copy the entire src/ tree into the image.
# Doing this before colcon build means Docker can cache the
# build layer: if src/ has not changed, the build step is skipped.
COPY src/ src/

# Source ROS 2, then run rosdep to install any package.xml
# dependencies that are not already present in the base image.
# rosdep init is skipped if /etc/ros/rosdep/sources.list.d/20-default.list
# already exists (the base image runs it during setup).
RUN /bin/bash -c "\
    source /opt/ros/humble/setup.bash && \
    rosdep update && \
    rosdep install --from-paths src --ignore-src -r -y && \
    colcon build --cmake-args -DCMAKE_BUILD_TYPE=Release"
# --cmake-args -DCMAKE_BUILD_TYPE=Release compiles with -O2 optimisations
# and strips debug symbols, which meaningfully shrinks the install directory.

# =============================================================
# Stage 2: runtime
# Starts fresh from the same ROS 2 base, copies ONLY the
# compiled install/ directory from the builder stage.
# No compilers, no build headers, no intermediate object files.
# This is what keeps the final image under 3 GB.
# =============================================================
FROM osrf/ros:humble-desktop AS runtime

# Copy the compiled install tree from the builder stage.
# COPY --from=builder <src-in-builder> <dest-in-runtime>
COPY --from=builder /ros2_ws/install /ros2_ws/install

# Copy the source tree too so the volume mount in run.sh has
# something to overlay (and so the launch file path is valid
# even when no volume is mounted, e.g. in CI).
COPY --from=builder /ros2_ws/src /ros2_ws/src

WORKDIR /ros2_ws

# Write the entrypoint script directly into the image.
# The entrypoint runs every time the container starts, before
# the CMD. Using a shell script (not exec form) so that
# source commands work — source is a bash built-in and does
# not exist in /bin/sh on all systems.
RUN echo '#!/bin/bash\n\
set -e\n\
source /opt/ros/humble/setup.bash\n\
source /ros2_ws/install/setup.bash\n\
exec "$@"' > /entrypoint.sh && chmod +x /entrypoint.sh
# set -e    exit immediately if any command fails
# exec "$@" replace the shell with whatever command was passed
#           (preserves signals like Ctrl+C correctly)

ENTRYPOINT ["/entrypoint.sh"]

# Default command: launch the entire fleet.
# This runs when you do: docker run <image>
# It can be overridden by appending a different command:
#   docker run <image> bash
CMD ["ros2", "launch", "drone_fleet", "fleet.launch.py"]
