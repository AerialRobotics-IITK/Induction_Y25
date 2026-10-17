# Task 6: Vision-Based Swarm Navigation (Extended)

You are a Senior Computer Vision Engineer at ARIITK. The team is developing a vision-based tracking system where drones can follow each other without relying on GPS. We have a Leader drone (**iris_1**) carrying an ArUco marker on its top, and a Follower drone (**iris_2**) equipped with a 2D gimbal camera. Your job is to implement the vision processing pipeline and the control logic to make **iris_2** autonomously track and follow **iris_1**.

---

## Directory Structure

You must organize your code as a ROS 2 package named `swarm_tracker`. Your final submission inside this folder should look like this:
```text
task_6/
├── README.md               # This file
├── iris_with_aruco/        # Custom Gazebo model for the leader drone
├── task_6.sdf              # Gazebo Harmonic simulation world
└── swarm_tracker/          # Your ROS 2 workspace/package
    ├── launch/
    │   └── tracker.launch.py
    ├── src/
    │   ├── follower_node.py    # Your vision & tracking node
    │   └── leader_evasion.py   # Provided evasion script
    ├── package.xml
    └── setup.py
```

---

## Performance Metrics

Your solution will be graded based on your completion of all the subtasks, focusing on:
- Robustness of the PID controller in following the leader.
- Accuracy of the pose estimation in GPS-denied environments.
- Precision of the landing maneuver during cooperative delivery.

---

## Subtasks

You must complete all three of the following subtasks in order to successfully finish the assignment. These subtasks build upon each other.

### Subtask 1: The "Cat and Mouse" Chase
- **The Setup:** `iris_1` executes a pre-programmed, aggressive evasive trajectory using the provided `leader_evasion.py` script.
- **The Challenge:** `iris_2` must autonomously track the ArUco marker. If the marker leaves `iris_2`'s camera frame for more than 5 seconds, the mission fails.
- **Requirements:** Implement a robust PID controller for `iris_2`'s gimbal (to keep the marker centered in the image) and its velocity (to maintain a fixed distance of **2.0 meters** from `iris_1`).

### Subtask 2: GPS-Denied Navigation
- **The Setup:** The GPS sensor on `iris_2` is disabled.
- **The Challenge:** `iris_2` must navigate purely by keeping `iris_1` in its sights and using the ArUco pose estimation vectors (translation vectors) to understand its relative position.
- **Requirements:** You are forbidden from subscribing to any `/mavros/global_position/` or Odometry topics for `iris_2`. Your velocity commands must be generated purely from the visual offset (`tvecs`).

### Subtask 3: Cooperative Payload Delivery
- **The Setup:** `iris_1` acts as a steady escort moving towards a dynamic landing zone.
- **The Challenge:** `iris_2` must fly in tight formation with `iris_1`. Once `iris_1` reaches a specified area, `iris_2` must simultaneously execute a precision landing on a moving platform.
- **Requirements:** The gimbal must track the ArUco marker for formation flight, and seamlessly transition to tracking a landing pad marker for the final descent.

---

## Part 1 — Vision Processing (OpenCV)

Inside `follower_node.py`, you must implement the computer vision pipeline using `cv_bridge` and `opencv-python`:
1. Subscribe to `iris_2`'s gimbal camera image topic.
2. Detect the ArUco marker on `iris_1` (Dictionary: `DICT_4X4_50`).
3. Use `cv2.aruco.estimatePoseSingleMarkers` to calculate the distance and angle relative to the camera.
4. Draw the detected bounding box and the coordinate axes on the image, and display it using `cv2.imshow()`.

-   Sorry for giving the task on CV before teaching !! But u have your LLMs lol !! Use them wisely.
---

## Part 2 — Gimbal & Flight Control (ROS 2)

Using the translation vectors (`tvecs`) and error values from OpenCV:
1. **Gimbal:** Publish commands to the gimbal to keep the ArUco marker dead-center in the camera frame (via MAVROS mount control or direct joint commands).
2. **Velocity:** Publish `geometry_msgs/msg/Twist` messages using **MAVROS** (e.g., to the `/iris_2/mavros/setpoint_velocity/cmd_vel_unstamped` topic in OFFBOARD mode) to maintain the required following distance. You must implement a **PID controller** (Proportional-Integral-Derivative) to ensure smooth acceleration and deceleration. A simple P-controller will likely cause oscillations and fail.

---

## Submission Guidelines

### What to Submit
1. Your complete `swarm_tracker` ROS 2 package containing all scripts and launch files.
2. A **2-minute screen recording** demonstrating your successful track. The video must show both the 3D Gazebo simulation of your completed subtasks and an inset window of the live OpenCV camera feed with annotations.
3. **Screenshots:** Include at least two screenshots in your PR description. One showing the live **QGroundControl (QGC)** interface tracking the drones, and another showing your ROS 2 terminal output or MAVROS telemetry.

> [!CAUTION]
> **Do NOT commit your build files!** 
> Make sure you have a proper `.gitignore` in your workspace. You must **only** submit your source code (`src/`, `launch/`, `package.xml`, `setup.py`, etc.). Do **not** commit the `build/`, `install/`, or `log/` directories, nor any `__pycache__` folders. Committing binary artifacts will cause the autograder pipeline to fail and your PR will be rejected.

### How to Submit (Pull Request)
To integrate with our autograding CI/CD pipeline, you **must** submit your work via a GitHub Pull Request (PR).

1. **Fork & Clone:** Fork the main `Induction_Y25` repository to your personal GitHub account and clone it locally.
2. **Create a Branch:** Create a new branch for your task (e.g., `git checkout -b task6-yourname`).
3. **Add Your Files:** Place your `swarm_tracker` package strictly inside your personal folder following this structure: `Induction_Y25/<your_name>/task_6/swarm_tracker/`.
4. **Commit & Push:** Commit your changes and push the branch to your fork.
5. **Open a PR:** Go to the main repository and open a Pull Request. 

Once your PR is opened, our **GitHub Actions** bot (running Ubuntu 24.04 and ROS 2 Jazzy) will automatically verify that your package compiles successfully.We will review your code and video demo directly in the PR!

**Deadline: Jun 25th,2026 EOD**
---
Good luck!
