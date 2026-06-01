#include "MissionDrone.hpp"
#include "DroneExceptions.hpp"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

MissionDrone::MissionDrone(const std::string& name,
                           const std::string& mission_name,
                           const std::vector<std::tuple<float, float, float>>& waypoints,
                           float battery, float max_altitude, float speed)
    : Drone(name, battery, max_altitude, speed),
      mission_name(mission_name),
      waypoints_(waypoints),
      current_waypoint_index_(0) {}

std::string MissionDrone::get_timestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) % 1000;
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S")
        << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

std::tuple<float, float, float> MissionDrone::next_waypoint() {
    if (mission_complete()) {
        throw DroneException("Mission '" + mission_name +
                             "' already complete for " + name);
    }
    auto wp = waypoints_[static_cast<size_t>(current_waypoint_index_)];
    visited_waypoints_.emplace_back(wp, get_timestamp());
    drain_battery(1.5f);
    add_log_entry("Visited waypoint " +
                  std::to_string(current_waypoint_index_) + "/" +
                  std::to_string(waypoints_.size()) + " (" +
                  std::to_string(std::get<0>(wp)) + ", " +
                  std::to_string(std::get<1>(wp)) + ", " +
                  std::to_string(std::get<2>(wp)) + ")");
    current_waypoint_index_++;
    return wp;
}

void MissionDrone::skip_waypoint(const std::string& reason) {
    if (mission_complete()) return;
    add_log_entry("Skipped waypoint " +
                  std::to_string(current_waypoint_index_) + ": " + reason);
    current_waypoint_index_++;
}

bool MissionDrone::mission_complete() const {
    return current_waypoint_index_ >=
           static_cast<int>(waypoints_.size());
}

std::string MissionDrone::mission_summary() const {
    std::ostringstream oss;
    oss << "=== Mission Summary: " << mission_name << " ===\n"
        << "Drone: " << name << "\n"
        << "Total waypoints: " << waypoints_.size() << "\n"
        << "Visited: " << visited_waypoints_.size() << "\n"
        << "Skipped: "
        << (static_cast<int>(waypoints_.size()) -
            static_cast<int>(visited_waypoints_.size()))
        << "\n"
        << "Mission complete: " << (mission_complete() ? "Yes" : "No")
        << "\n"
        << "Battery remaining: " << std::fixed << std::setprecision(1)
        << get_battery_level() << "%\n";
    oss << "Visited waypoints:\n";
    for (size_t i = 0; i < visited_waypoints_.size(); ++i) {
        auto& [wp, ts] = visited_waypoints_[i];
        oss << "  [" << i << "] ("
            << std::get<0>(wp) << ", "
            << std::get<1>(wp) << ", "
            << std::get<2>(wp) << ") at " << ts << "\n";
    }
    return oss.str();
}

std::string MissionDrone::get_info() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "[MissionDrone] " << name
        << " | Mission: " << mission_name
        << " | Battery: " << get_battery_level() << "%"
        << " | Status: " << get_status()
        << " | Altitude: " << altitude_ << "m"
        << " | Waypoint: " << current_waypoint_index_ << "/"
        << waypoints_.size();
    return oss.str();
}

int MissionDrone::get_current_waypoint_index() const {
    return current_waypoint_index_;
}

int MissionDrone::get_total_waypoints() const {
    return static_cast<int>(waypoints_.size());
}

const std::vector<std::tuple<float, float, float>>&
MissionDrone::get_waypoints() const {
    return waypoints_;
}

void MissionDrone::reset_mission() {
    current_waypoint_index_ = 0;
    visited_waypoints_.clear();
    add_log_entry("Mission '" + mission_name + "' reset");
}
