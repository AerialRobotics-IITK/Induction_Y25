# # =========================
# # Stage 1: Builder
# # =========================
# FROM ros:jazzy-ros-base AS builder

# SHELL ["/bin/bash", "-c"]

# RUN apt-get update && apt-get install -y \
#     python3-colcon-common-extensions \
#     build-essential \
#     && rm -rf /var/lib/apt/lists/*

# WORKDIR /ws

# COPY src ./src

# RUN source /opt/ros/jazzy/setup.bash && \
#     colcon build --symlink-install


# # =========================
# # Stage 2: Runtime
# # =========================
# FROM ros:jazzy-ros-base

# SHELL ["/bin/bash", "-c"]

# WORKDIR /ws

# COPY --from=builder /ws/install ./install
# COPY --from=builder /ws/build ./build

# RUN echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc && \
#     echo "source /ws/install/setup.bash" >> ~/.bashrc

# ENV ROS_DOMAIN_ID=42
# ENV RMW_IMPLEMENTATION=rmw_fastrtps_cpp

# CMD ["bash", "-c", "source /ws/install/setup.bash && ros2 launch drone_fleet fleet.launch.py"]

# =========================
# Base Image (ROS 2 Jazzy)
# =========================
FROM ros:jazzy

SHELL ["/bin/bash", "-c"]

# =========================
# Install dependencies
# =========================
RUN apt-get update && apt-get install -y \
    python3-colcon-common-extensions \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# =========================
# Set workspace
# =========================
WORKDIR /ws

# Copy entire ROS workspace
COPY . /ws
COPY ../part1 /ws/part1

# =========================
# Build workspace
# =========================
RUN source /opt/ros/jazzy/setup.bash && \
    colcon build --symlink-install

# =========================
# Auto-source environment
# =========================
RUN echo "source /opt/ros/jazzy/setup.bash" >> /root/.bashrc && \
    echo "source /ws/install/setup.bash" >> /root/.bashrc

# =========================
# Default command
# =========================
CMD ["bash", "-c", "source /ws/install/setup.bash && ros2 launch drone_fleet fleet.launch.py"]