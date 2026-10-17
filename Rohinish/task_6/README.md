# Task 6 — Vision-Based Swarm Navigation (Rohinish)

This folder contains my solution to **Task 6: Vision-Based Swarm Navigation** from the
Aerial Robotics IITK induction round.

A follower drone (`iris_2`) carrying a 2-D gimbal camera autonomously tracks an ArUco
marker (Dictionary `DICT_4X4_50`) mounted on a leader drone (`iris_1`) using a
PID-controlled vision pipeline implemented in ROS 2 (Jazzy) + OpenCV.

## Layout
```
task_6/
├── README.md                 # This file
├── iris_with_aruco/          # Custom Gazebo model for the leader (ArUco texture on top)
│   ├── model.config
│   ├── model.sdf
│   └── materials/textures/   # ArUco marker .png/.svg used by the SDF
├── task_6.sdf                # Gazebo Harmonic simulation world (iris_1 + iris_2 + runway)
└── swarm_tracker/            # ROS 2 Python (ament_python) package
    ├── package.xml
    ├── setup.py
    ├── setup.cfg
    ├── resource/swarm_tracker
    ├── launch/tracker.launch.py
    ├── src/
    │   ├── __init__.py
    │   ├── follower_node.py  # Vision + PID + state machine
    │   └── leader_evasion.py # Provided evasion script (unchanged from upstream spec)
    └── test/
        └── test_aruco_pipeline.py  # Offline test of the ArUco/PID pipeline
```

> The visual mesh files referenced by `iris_with_aruco/model.sdf`
> (`model://iris_with_standoffs/meshes/...`) come from the ArduPilot Gazebo plugin's
> built-in `iris_with_standoffs` model, so they are **not** duplicated here.

## What the follower does
| Subtask | Behaviour |
|---|---|
| 1 — Cat & Mouse | PID on `(distance error, x_offset, y_offset, yaw_offset)` → smooth chase of a marker that is dodging via `leader_evasion.py`. 5 s marker-loss timeout. |
| 2 — GPS-Denied | Identical control law, only using the camera (`tvecs`) — no MAVROS global position / odom topics subscribed. |
| 3 — Cooperative Landing | After a parameterised dwell on the leader, the follower latches onto the landing-pad marker (`landing_pad_id`) and executes a descending PID landing. |

## How to run (high-level)

```bash
# 1. Build
cd Rohinish/task_6/swarm_tracker
colcon build --packages-select swarm_tracker

# 2. Source
source install/setup.bash

# 3. Bring up SITL + Gazebo Harmonic with the task_6.sdf world
#    (using ArduPilot SITL + ardupilot_gz, two iris instances)
gz sim -r ../task_6.sdf &
sim_vehicle.py -v ArduCopter -f gazebo-iris --instance 0 -I0 &
sim_vehicle.py -v ArduCopter -f gazebo-iris --instance 1 -I1 &

# 4. Launch the tracker
ros2 launch swarm_tracker tracker.launch.py mode:=chase
#   mode := chase | gps_denied | landing
```

Parameters of interest (overridable from the launch file):

| Param | Default | Purpose |
|---|---|---|
| `mode` | `chase` | One of `chase`, `gps_denied`, `landing` |
| `target_distance` | `2.0` m | Desired follow distance |
| `marker_id` | `0` | ID of the marker on `iris_1` |
| `landing_pad_id` | `5` | ID of the marker on the moving pad |
| `marker_size` | `0.15` m | Side length of the ArUco marker |
| `image_topic` | `/iris_2/camera/image` | Gimbal camera topic |
| `cmd_vel_topic` | `/iris_2/mavros/setpoint_velocity/cmd_vel_unstamped` | OFFBOARD velocity setpoint |
| `gimbal_topic` | `/iris_2/mavros/mount_control/command` | Mount control (pitch / yaw) |
| `pid_*` | see `follower_node.py` | Per-axis PID gains |

## Offline unit test

A self-contained pytest verifies that the vision + PID logic runs against a
synthetically-rendered ArUco marker (no ROS, no Gazebo needed):

```bash
pip install opencv-contrib-python numpy pytest
pytest Rohinish/task_6/swarm_tracker/test
```
