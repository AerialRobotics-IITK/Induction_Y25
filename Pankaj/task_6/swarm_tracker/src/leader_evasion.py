#!/usr/bin/env python3
"""
leader_evasion.py  —  ARIITK Task 6
─────────────────────────────────────
Drives iris_1 (leader) on an aggressive lemniscate (figure-eight)
trajectory with altitude oscillation. iris_2 must track it.

Subtask 1: iris_1 runs this evasion; iris_2 must keep ArUco in frame.
Subtask 3: iris_1 flies toward the landing zone (20 0 5); node detects
           arrival and idles in a hover — follower then switches to land.

MAVROS namespace: /iris_1
OFFBOARD velocity topic: /iris_1/mavros/setpoint_velocity/cmd_vel_unstamped
"""

import math
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy, HistoryPolicy

from geometry_msgs.msg import Twist, PoseStamped
from mavros_msgs.msg import State
from mavros_msgs.srv import CommandBool, SetMode


class LeaderEvasion(Node):

    # ── Evasion trajectory (Subtasks 1 & 2) ─────────────────────────────────
    SPEED     = 1.5    # m/s peak
    PERIOD    = 20.0   # seconds per loop
    ALTITUDE  = 5.0    # base hover altitude (m AGL)
    ALT_AMP   = 1.0    # ±altitude oscillation amplitude
    RADIUS    = 6.0    # figure-eight radius

    # ── Landing approach (Subtask 3) ─────────────────────────────────────────
    LAND_ZONE = (20.0, 0.0, 5.0)   # x, y, z target
    LAND_SPEED = 1.0               # m/s approach speed
    LAND_TOL   = 0.5               # metres — consider "arrived"

    def __init__(self):
        super().__init__('leader_evasion')

        self.declare_parameter('mode', 'chase')
        self.mode = self.get_parameter('mode').get_parameter_value().string_value

        qos = QoSProfile(
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.VOLATILE,
            history=HistoryPolicy.KEEP_LAST, depth=10)

        self.armed   = False
        self.offboard = False
        self.t0      = None
        self.pos     = [0.0, 0.0, 0.0]   # rough position tracker

        self.pub_vel = self.create_publisher(
            Twist, '/iris_1/mavros/setpoint_velocity/cmd_vel_unstamped', qos)
        self.pub_pos = self.create_publisher(
            PoseStamped, '/iris_1/mavros/setpoint_position/local', qos)

        self.create_subscription(State, '/iris_1/mavros/state', self.state_cb, qos)

        self.cli_arm  = self.create_client(CommandBool, '/iris_1/mavros/cmd/arming')
        self.cli_mode = self.create_client(SetMode,     '/iris_1/mavros/set_mode')

        self.create_timer(0.05, self._heartbeat)   # 20 Hz
        self.create_timer(1.0,  self._arm_loop)    # 1 Hz

        self.get_logger().info(f'leader_evasion ready  mode={self.mode}')

    # ─────────────────────────────────────────────────────────────────────────

    def state_cb(self, msg: State):
        self.armed    = msg.armed
        self.offboard = (msg.mode == 'OFFBOARD')

    def _arm_loop(self):
        if not self.offboard:
            self._set_mode('OFFBOARD')
        if not self.armed:
            self._arm()
        elif self.t0 is None and self.armed and self.offboard:
            self.t0 = self.get_clock().now().nanoseconds * 1e-9
            self.get_logger().info('Mission start — evasion trajectory active.')

    def _heartbeat(self):
        if self.t0 is None:
            # Pre-arm: publish hover setpoint so OFFBOARD can engage
            pose = PoseStamped()
            pose.header.stamp = self.get_clock().now().to_msg()
            pose.pose.position.z = self.ALTITUDE
            self.pub_pos.publish(pose)
            return

        t = self.get_clock().now().nanoseconds * 1e-9 - self.t0

        if self.mode == 'landing':
            self._landing_approach(t)
        else:
            self._evasion(t)

    def _evasion(self, t: float):
        """Lemniscate of Bernoulli with altitude oscillation."""
        omega = 2 * math.pi / self.PERIOD
        s = math.sin(omega * t)
        c = math.cos(omega * t)
        denom = 1 + s * s

        # Parametric velocity derivatives of Bernoulli lemniscate
        dx = self.RADIUS * omega * (
            (-s * denom - c * 2 * s * c) / (denom * denom))
        dy = self.RADIUS * omega * (
            ((c * c - s * s) * denom - s * c * 2 * s * c) / (denom * denom))
        dz = self.ALT_AMP * omega * c   # altitude oscillation

        speed = math.sqrt(dx*dx + dy*dy + dz*dz)
        if speed > 0.01:
            sc = self.SPEED / speed
            dx *= sc; dy *= sc; dz *= sc

        twist = Twist()
        twist.linear.x = float(dx)
        twist.linear.y = float(dy)
        twist.linear.z = float(dz)
        self.pub_vel.publish(twist)

    def _landing_approach(self, t: float):
        """Fly iris_1 toward the landing zone (Subtask 3)."""
        gx, gy, gz = self.LAND_ZONE

        # Rough position integration (good enough for sim demo)
        dt = 0.05
        dist = math.sqrt(
            (self.pos[0] - gx)**2 +
            (self.pos[1] - gy)**2 +
            (self.pos[2] - gz)**2)

        if dist < self.LAND_TOL:
            # Arrived — hover in place so iris_2 can land
            self.get_logger().info(
                'iris_1 reached landing zone — hovering.', throttle_duration_sec=5.0)
            self.pub_vel.publish(Twist())
            return

        dx = (gx - self.pos[0]) / dist * self.LAND_SPEED
        dy = (gy - self.pos[1]) / dist * self.LAND_SPEED
        dz = (gz - self.pos[2]) / dist * self.LAND_SPEED

        self.pos[0] += dx * dt
        self.pos[1] += dy * dt
        self.pos[2] += dz * dt

        twist = Twist()
        twist.linear.x = float(np.clip(dx, -2.0, 2.0)) if False else float(dx)
        twist.linear.y = float(dy)
        twist.linear.z = float(dz)
        self.pub_vel.publish(twist)

    # ─────────────────────────────────────────────────────────────────────────

    def _set_mode(self, mode: str):
        if not self.cli_mode.service_is_ready():
            return
        req = SetMode.Request()
        req.custom_mode = mode
        self.cli_mode.call_async(req)

    def _arm(self):
        if not self.cli_arm.service_is_ready():
            return
        req = CommandBool.Request()
        req.value = True
        self.cli_arm.call_async(req)


# ─────────────────────────────────────────────────────────────────────────────
def main(args=None):
    rclpy.init(args=args)
    node = LeaderEvasion()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
