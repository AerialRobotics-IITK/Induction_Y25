# ============================================================
# Stage 1: Builder — install dependencies and build workspace
# ============================================================
FROM osrf/ros:humble-desktop AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    python3-colcon-common-extensions \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /ros2_ws

COPY src/ src/

RUN . /opt/ros/humble/setup.sh && \
    colcon build --cmake-args -DCMAKE_BUILD_TYPE=Release && \
    rm -rf build log src

# ============================================================
# Stage 2: Runtime — copy only the install directory
# ============================================================
FROM osrf/ros:humble-desktop AS runtime

ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /ros2_ws

COPY --from=builder /ros2_ws/install /ros2_ws/install

COPY src/ src/

ENV ROS_DOMAIN_ID=42

ENTRYPOINT ["/bin/bash", "-c", \
    "source /opt/ros/humble/setup.bash && \
     source /ros2_ws/install/setup.bash && \
     exec \"$@\"", "--"]

CMD ["ros2", "launch", "drone_fleet", "fleet.launch.py"]
