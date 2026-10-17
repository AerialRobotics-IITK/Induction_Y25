# Task 6 — Vision-Based Swarm Navigation (Updated)

## Key Changes from Original Setup

1. **iris_1 and iris_2 wrapped as models in world file** — ArduPilot plugins defined at world level, not in model SDFs
2. **iris_2 uses stock iris_with_gimbal** — no separate iris_with_gimbal_follower needed
3. **Landing platform is dynamic** — follows a square trajectory (5,5)→(25,5)→(25,25)→(5,25)→(5,5)
4. **New spawn positions** — iris_1 at (0,0,0.3), iris_2 at (2,0,0.5)
5. **Updated camera topic** — reflects new model hierarchy with iris_2 as a wrapper model

---

## One-time setup

### 1. Add task_6 models to Gazebo's search path

Add to `~/.bashrc`:

```bash
export GZ_SIM_RESOURCE_PATH=$GZ_SIM_RESOURCE_PATH:\
/path/to/Induction_Y25/pankajphogat/task_6:\
/home/gz_ws/src/ardupilot_gazebo/models

source ~/.bashrc
```

### 2. Build the ROS 2 package

```bash
mkdir -p ~/ros2_ws/src
cp -r /path/to/task_6/swarm_tracker ~/ros2_ws/src/
cd ~/ros2_ws
colcon build --symlink-install --packages-select swarm_tracker
echo "source ~/ros2_ws/install/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

### 3. Install/upgrade Python dependencies

```bash
# Upgrade OpenCV to 4.7.0+ (required for ArUco API)
pip install --upgrade opencv-contrib-python

# Verify
python3 -c "import cv2.aruco; print('OpenCV version:', cv2.__version__)"
```

---

## Running the simulation — 6 terminals

### Source in every terminal:
```bash
source /opt/ros/jazzy/setup.bash && source ~/ros2_ws/install/setup.bash
```

### Terminal 1 — Gazebo
```bash
cd /path/to/task_6
gz sim task_6.sdf
```
Wait for both drones to appear.

### Terminal 2 — SITL iris_1 (instance 0, port 9002)
```bash
cd ~/ardupilot/ArduCopter
../Tools/autotest/sim_vehicle.py \
  -v ArduCopter -f gazebo-iris \
  --instance 0 \
  --out=udp:127.0.0.1:14550 \
  --no-rebuild
```
Wait for "EKF3 IMU0 origin set".

### Terminal 3 — SITL iris_2 (instance 1, port 9012)
```bash
cd ~/ardupilot/ArduCopter
../Tools/autotest/sim_vehicle.py \
  -v ArduCopter -f gazebo-iris \
  --instance 1 \
  --out=udp:127.0.0.1:14560 \
  --no-rebuild
```

### Terminal 4 — MAVROS iris_1
```bash
ros2 launch mavros apm.launch \
  fcu_url:="udp://:14550@127.0.0.1:14555" \
  tgt_system:=1 \
  namespace:=iris_1
```
Wait for "FCU: ArduCopter…".

### Terminal 5 — MAVROS iris_2
```bash
ros2 launch mavros apm.launch \
  fcu_url:="udp://:14560@127.0.0.1:14565" \
  tgt_system:=2 \
  namespace:=iris_2
```

### Terminal 6 — swarm_tracker nodes
```bash
# Subtask 1 — Cat & Mouse Chase
ros2 launch swarm_tracker tracker.launch.py mode:=chase

# Subtask 2 — GPS-Denied
ros2 launch swarm_tracker tracker.launch.py mode:=gps_denied

# Subtask 3 — Formation + Landing
ros2 launch swarm_tracker tracker.launch.py mode:=landing
```

An OpenCV window **"iris_2 | ArUco Tracker"** should appear showing the live gimbal camera feed with the marker bounding box.

---

## Camera and gimbal topics

| Topic | Direction | Purpose |
|-------|-----------|---------|
| `/world/iris_runway/model/iris_2/model/iris_with_gimbal/gimbal/link/pitch_link/sensor/camera/image` | Gz→ROS2 | Gimbal camera live feed |
| `/iris_2/gimbal/cmd_pitch` | ROS2→Gz | Tilt camera down/up |
| `/iris_2/gimbal/cmd_yaw` | ROS2→Gz | Pan camera left/right |
| `/iris_2/gimbal/cmd_roll` | ROS2→Gz | Roll (always 0) |

---

## ArUco markers

| Marker | ID | Size | Location | Used in |
|--------|-----|------|----------|---------|
| Leader | 0 | 15 cm | Top of iris_1 | All subtasks |
| Landing pad | 1 | 30 cm (1m visual) | landing_platform | Subtask 3 |

Dictionary: `DICT_4X4_50`

---

## Troubleshooting

**OpenCV window doesn't appear:**
```bash
# Check camera topic is flowing
ros2 topic hz /world/iris_runway/model/iris_2/model/iris_with_gimbal/gimbal/link/pitch_link/sensor/camera/image

# Check gz_ros2_bridge is bridging it
ros2 topic list | grep camera
```

**Drone doesn't move:**
```bash
# Check MAVROS state
ros2 topic echo /iris_2/mavros/state --once

# Check velocity commands are being published
ros2 topic echo /iris_2/mavros/setpoint_velocity/cmd_vel_unstamped
```

**"No module named 'cv2.aruco'":**
```bash
pip install --upgrade opencv-contrib-python
python3 -c "import cv2.aruco; print(cv2.__version__)"
```

---

## Submitting the PR

```bash
git checkout -b task6-pankajphogat
# Place this task_6/ folder at: Induction_Y25/pankajphogat/task_6/
git add Induction_Y25/pankajphogat/task_6/
git commit -m "Task 6: vision-based swarm navigation"
git push origin task6-pankajphogat
# Open PR on GitHub
```

Do NOT commit `build/`, `install/`, `log/`, or `__pycache__/`.
