# Task 5: OOPs + ROS 2 + Docker - Build a Virtual Drone Fleet Manager

You are a Junior Team Member at ARIITK. The team has three drones — **Alpha**, **Beta**, and **Gamma**. Each drone has a battery level, a name, a status (`idle`/`flying`/`charging`), and a current altitude. Your job is to build a ROS 2 system that simulates all three drones publishing their state, a central fleet manager node that monitors all of them, and package the entire thing inside Docker so any teammate can run it with one command.

---

## Part 1 — OOPs in C++

Before writing any ROS code, design the drone class structure in pure C++. You need to properly modularize your code using header files (`.hpp`) and source files (`.cpp`). Do not write everything in a single file! Your `main.cpp` should only contain `int main()` and the necessary setup code.

**Class Hierarchy:**
```text
Vehicle (abstract base class)
    └── Drone (inherits Vehicle)
            └── MissionDrone (inherits Drone)
                    └── AutonomousDrone (inherits MissionDrone)
```

**Custom Exceptions:**
Define a custom exception hierarchy using C++ standard exception mechanisms. All drone-specific exceptions must share a common base so they can be caught collectively or individually. You will need:
- A base drone exception
- Battery depleted error
- Invalid state error
- Altitude error

**Class Requirements:**

1. **`Vehicle` (Abstract Base Class)**
    - Attributes: `name` (`std::string`), `battery_level` (`float`, 0.0 to 100.0, private), `status` (`std::string`, private, only settable through a method that validates allowed states and logs with timestamp), `flight_log` (`std::vector<std::string>`, private).
    - Methods:
        - `get_info()` (pure virtual)
        - `drain_battery(float amount)`: reduces battery, never below 0; throws `BatteryDepletedError` if already at 0.
        - `charge_battery(float amount, int duration_seconds)`: increases battery; throws `InvalidStateError` if not in charging state.
        - `is_critical()`: returns `bool`.
        - `get_flight_log()`: returns log as formatted `std::string`.
        - Appropriate getters for all private members (no public setters for battery or status directly).

2. **`Drone`**
    - Attributes: `altitude` (`float`, protected), `max_altitude` (`float`, protected), `speed` (`float`, private).
    - Methods:
        - `take_off(float target_altitude)`: throws `AltitudeError` if limit exceeded.
        - `land()`
        - `emergency_stop()`: drains battery by 30 as a penalty.
        - Override `get_info()`.

3. **`MissionDrone`**
    - Attributes: `mission_name` (`std::string`), `waypoints` (`std::vector<std::tuple<float, float, float>>`), `current_waypoint_index` (`int`), `visited_waypoints` (`std::vector<std::pair<std::tuple<float,float,float>, std::string>>`, private, stores waypoint + timestamp).
    - Methods:
        - `next_waypoint()`: returns current position as tuple; drains battery by 1.5.
        - `skip_waypoint(const std::string& reason)`
        - `mission_complete()`: returns `bool`.
        - `mission_summary()`: returns `std::string`.
        - Override `get_info()`.

4. **`AutonomousDrone`**
    - Attributes: `ai_mode` (`std::string`: `"manual"`, `"auto"`, `"return_home"`), `home_position` (`std::tuple<float, float, float>`), `obstacle_log` (`std::vector<std::string>`, private).
    - Methods:
        - `set_ai_mode(const std::string& mode)`: `"return_home"` inserts home as next waypoint.
        - `detect_obstacle(std::tuple<float,float,float> position, const std::string& severity)`: logs with timestamp; calls `emergency_stop()` if severity is `"high"`.
        - `auto_replan(const std::vector<std::tuple<float,float,float>>& obstacles)`: returns a new waypoint list avoiding obstacles within 5 units.
        - Override `get_info()`.

**`main.cpp` Requirements:**
- Create one object of each class; store them all in a `std::vector<Vehicle*>` and call `get_info()` on each to demonstrate **polymorphism**.
- Show that private members cannot be accessed directly (attempt and explain with a comment).
- Call `drain_battery()`, `take_off()`, `detect_obstacle()` and catch all exceptions appropriately using the custom exception hierarchy.
- Run a full mission on an `AutonomousDrone`: take off, iterate all waypoints, simulate a high-severity obstacle, print mission summary.
- Use `std::chrono` for all timestamps in logs.
- Code must compile cleanly with `cmake .. && make` (no warnings with `-Wall -Wextra`).

---

## Part 2 — Wrap it in ROS 2 (C++ / rclcpp)

Make each drone a ROS 2 node written entirely in C++. Build a ROS 2 package `drone_fleet` containing:

1. **Drone Node (`drone_node`)**
    - Creates a `MissionDrone` object internally with **5 pre-defined waypoints**.
    - Publishes a `std_msgs/msg/String` to `/drone/<name>/status` every **1 second**.
    - Message format: `"name:Alpha|battery:87.3|altitude:15.2|status:flying|waypoint:2/5|speed:3.2"`
    - Every publish: drains battery by 0.5; advances waypoint every 3 publishes.
    - When battery hits critical: publishes to `/drone/<name>/alert` and calls `land()`.
    - When mission complete: publishes to `/drone/<name>/mission_complete` and restarts mission.
    - Publishes a JSON-formatted `std_msgs/msg/String` to `/drone/<name>/telemetry` every 2 seconds with full drone state.
    - Accepts ROS 2 parameters: `drone_name` (string), `initial_battery` (double, default 100.0), `mission_name` (string).

2. **Fleet Manager Node (`fleet_manager`)**
    - Subscribes to status (`/drone/<name>/status`), alert, `mission_complete`, and telemetry topics for Alpha, Beta, and Gamma.
    - Parses the telemetry JSON manually (no external JSON library).
    - Every 5 seconds, prints a formatted fleet report table in the console (showing Drone, Battery, Altitude, Waypoint, Status).
    - Prints a timestamped warning when any alert arrives.
    - Exposes a ROS 2 service `/fleet/status_report` using `std_srvs/srv/Trigger` that triggers an immediate report.

3. **Health Monitor Node (`health_monitor`)**
    - Subscribes to telemetry topics.
    - Tracks battery drain rate per drone from the last 10 samples using a circular buffer (`std::deque`).
    - Publishes a warning to `/fleet/health_warning` if drain rate exceeds 1.5 per second for any drone.
    - Every 10 seconds: prints a diagnostics table with drain rate, estimated time to critical, and estimated time to depletion, and publishes a JSON string to `/fleet/health_summary`.

4. **Launch File (`fleet.launch.py`)**
    - Must start:
        - Alpha drone node (`initial_battery:=100.0`)
        - Beta drone node (`initial_battery:=60.0`)
        - Gamma drone node (`initial_battery:=35.0` - starts nearly critical)
        - Fleet manager node
        - Health monitor node

---

## Part 3 — Docker

Write a `Dockerfile` and a `run.sh` script to containerize your workspace.

**Dockerfile requirements:**
- Base: `osrf/ros:humble-desktop`
- Copies your entire ROS 2 workspace into the container.
- Builds the workspace with `colcon build` during image build.
- Entrypoint sources ROS 2 and the workspace automatically.
- Uses a **multi-stage build**: stage 1 (`builder`) installs deps and builds; stage 2 (`runtime`) copies only the install directory. The final image must be under 3 GB.

**`run.sh` script requirements:**
- Mount the source code directory as a volume so changes reflect without rebuilding.
- Include necessary flags (like `-it`, `--rm`, `--net=host`, or `-e ROS_DOMAIN_ID=42` if required).
- The container should automatically run the fleet launch file.

**Expected final result:**
```bash
git clone <your_repo>
cd fleet_ws
chmod +x run.sh
./run.sh

# All five nodes running
# Fleet report every 5 seconds
# Health diagnostics every 10 seconds
# Gamma hits critical within ~30 seconds
```

## Submission
Open your **Pull Requests (PRs)** by **June 5 EOD**.
