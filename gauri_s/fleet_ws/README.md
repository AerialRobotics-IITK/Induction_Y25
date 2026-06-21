# fleet_ws

ROS 2 Humble drone fleet simulation — Parts 1, 2, and 3 of the OOP + ROS 2 assignment.

## Structure

```
fleet_ws/
├── Dockerfile                        # multi-stage build (builder + runtime)
├── run.sh                            # build image and launch fleet
├── .dockerignore
└── src/
    └── drone_fleet/
        ├── package.xml
        ├── CMakeLists.txt
        ├── include/drone_fleet/      # Part 1 OOP headers (header-only C++)
        │   ├── drone_exceptions.hpp
        │   ├── Vehicle.hpp
        │   ├── Drone.hpp
        │   ├── MissionDrone.hpp
        │   └── AutonomousDrone.hpp
        ├── src/                      # Part 2 ROS 2 nodes
        │   ├── drone_node.cpp
        │   ├── fleet_manager.cpp
        │   └── health_monitor.cpp
        └── launch/
            └── fleet.launch.py
```

## Quick start

```bash
git clone <your_repo_url>
cd fleet_ws
chmod +x run.sh
./run.sh
```

The first run builds the Docker image (takes a few minutes). Subsequent runs start instantly.

## What you will see

- Three drone nodes: Alpha (100%), Beta (60%), Gamma (35%)
- Fleet status report printed every 5 seconds
- Health diagnostics printed every 10 seconds
- Gamma hits critical battery (~20%) within about 30 seconds and lands automatically
- High drain rate warnings published to `/fleet/health_warning`

## Useful commands (open a second terminal)

```bash
# Attach a shell to the running container
docker exec -it $(docker ps -qf ancestor=drone_fleet) bash
source /opt/ros/humble/setup.bash
source /ros2_ws/install/setup.bash

# Watch a topic live
ros2 topic echo /drone/Alpha/status
ros2 topic echo /fleet/health_warning
ros2 topic echo /fleet/health_summary

# Trigger an immediate fleet report via the service
ros2 service call /fleet/status_report std_srvs/srv/Trigger {}
```

## Rebuild after source changes

```bash
docker rmi drone_fleet
./run.sh
```

Or rebuild without removing the image:

```bash
docker build -t drone_fleet .
./run.sh
```

## Requirements

- Docker (any recent version)
- No ROS 2 installation needed on the host machine
