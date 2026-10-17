# ==========================
# Builder
# ==========================
FROM ros:jazzy AS builder

SHELL ["/bin/bash", "-c"]

WORKDIR /fleet_ws

RUN apt-get update && apt-get install -y \
    python3-colcon-common-extensions \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

COPY . .

RUN source /opt/ros/jazzy/setup.bash && \
    colcon build

# ==========================
# Runtime
# ==========================
FROM ros:jazzy

SHELL ["/bin/bash", "-c"]

WORKDIR /fleet_ws

COPY --from=builder /fleet_ws/install ./install

RUN echo "source /opt/ros/jazzy/setup.bash" >> /root/.bashrc && \
    echo "source /fleet_ws/install/setup.bash" >> /root/.bashrc

ENTRYPOINT ["/bin/bash","-c","source /opt/ros/jazzy/setup.bash && source /fleet_ws/install/setup.bash && exec \"$@\"","--"]

CMD ["bash"]