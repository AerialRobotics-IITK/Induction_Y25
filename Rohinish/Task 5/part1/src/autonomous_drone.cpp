#include "autonomous_drone.hpp"
#include "drone_exceptions.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <ctime>
#include <algorithm>

AutonomousDrone::AutonomousDrone(
    const std::string& name, const std::string& mission_name,
    const std::vector<std::tuple<float, float, float>>& waypoints,
    std::tuple<float, float, float> home_position,
    float battery, float max_altitude, float speed)
    : MissionDrone(name, mission_name, waypoints, battery, max_altitude, speed),
      ai_mode_("manual"),
      home_position_(home_position) {}

std::string AutonomousDrone::get_timestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) % 1000;
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S")
        << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

void AutonomousDrone::set_ai_mode(const std::string& mode) {
    if (mode != "manual" && mode != "auto" && mode != "return_home") {
        throw InvalidStateError("Invalid AI mode: " + mode);
    }
    ai_mode_ = mode;
    add_log_entry("AI mode set to '" + mode + "'");

    if (mode == "return_home") {
        auto it = waypoints_.begin() + current_waypoint_index_;
        waypoints_.insert(it, home_position_);
        add_log_entry("Home position inserted as next waypoint for return_home");
    }
}

void AutonomousDrone::detect_obstacle(std::tuple<float, float, float> position,
                                       const std::string& severity) {
    std::ostringstream oss;
    oss << "[" << get_timestamp() << "] Obstacle detected at ("
        << std::get<0>(position) << ", "
        << std::get<1>(position) << ", "
        << std::get<2>(position) << ") severity=" << severity;
    obstacle_log_.push_back(oss.str());
    add_log_entry("Obstacle: " + severity + " at (" +
                  std::to_string(std::get<0>(position)) + ", " +
                  std::to_string(std::get<1>(position)) + ", " +
                  std::to_string(std::get<2>(position)) + ")");

    if (severity == "high") {
        add_log_entry("High severity obstacle — triggering emergency stop");
        emergency_stop();
    }
}

std::vector<std::tuple<float, float, float>> AutonomousDrone::auto_replan(
    const std::vector<std::tuple<float, float, float>>& obstacles) {
    std::vector<std::tuple<float, float, float>> new_waypoints;
    for (const auto& wp : waypoints_) {
        bool too_close = false;
        for (const auto& obs : obstacles) {
            float dx = std::get<0>(wp) - std::get<0>(obs);
            float dy = std::get<1>(wp) - std::get<1>(obs);
            float dz = std::get<2>(wp) - std::get<2>(obs);
            float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
            if (dist < 5.0f) {
                too_close = true;
                float nx = std::get<0>(wp) + 5.0f;
                float ny = std::get<1>(wp) + 5.0f;
                float nz = std::get<2>(wp);
                new_waypoints.emplace_back(nx, ny, nz);
                add_log_entry("Replanned waypoint away from obstacle");
                break;
            }
        }
        if (!too_close) {
            new_waypoints.push_back(wp);
        }
    }
    return new_waypoints;
}

std::string AutonomousDrone::get_info() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "[AutonomousDrone] " << name
        << " | Mission: " << mission_name
        << " | AI: " << ai_mode_
        << " | Battery: " << get_battery_level() << "%"
        << " | Status: " << get_status()
        << " | Altitude: " << altitude_ << "m"
        << " | Waypoint: " << current_waypoint_index_ << "/"
        << waypoints_.size()
        << " | Obstacles logged: " << obstacle_log_.size();
    return oss.str();
}

std::string AutonomousDrone::get_ai_mode() const { return ai_mode_; }
