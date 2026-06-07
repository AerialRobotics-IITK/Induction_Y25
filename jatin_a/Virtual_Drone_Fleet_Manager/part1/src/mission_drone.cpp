#include "mission_drone.hpp"
#include "drone_exceptions.hpp"
#include <sstream>

MissionDrone::MissionDrone(const std::string& name, float battery, float max_alt, float speed,
                           const std::string& mission,
                           const std::vector<std::tuple<float,float,float>>& wps)
    : Drone(name, battery, max_alt, speed),
      mission_name(mission), waypoints(wps), current_waypoint_index(0) {}

std::tuple<float,float,float> MissionDrone::next_waypoint() {

    // ❌ OLD: silently returned {0,0,0}
    // ✅ FIX: throw proper exception
    if (mission_complete()) {
        throw InvalidStateError("Mission already complete");
    }

    // 🔋 REQUIRED by assignment
    drain_battery(1.5f);

    // (Optional but GOOD) safety check
    if (get_battery() <= 0) {
        throw BatteryDepletedError();
    }

    auto wp = waypoints[current_waypoint_index];

    // ✅ log visit with timestamp
    visited_waypoints.push_back({wp, current_timestamp()});

    current_waypoint_index++;

    return wp;
}

void MissionDrone::skip_waypoint(const std::string& reason) {

    // ✅ prevent skipping beyond bounds
    if (mission_complete()) {
        throw InvalidStateError("Cannot skip, mission complete");
    }

    // (Optional) you can log reason if you want
    current_waypoint_index++;
}

bool MissionDrone::mission_complete() const {
    return current_waypoint_index >= static_cast<int>(waypoints.size());
}

std::string MissionDrone::mission_summary() const {
    std::stringstream ss;

    ss << "Mission: " << mission_name << "\nVisited:\n";

    for (const auto& v : visited_waypoints) {
        ss << "("
           << std::get<0>(v.first) << ","
           << std::get<1>(v.first) << ","
           << std::get<2>(v.first) << ") at "
           << v.second << "\n";
    }

    return ss.str();
}

std::string MissionDrone::get_info() const {
    return "MissionDrone: " + mission_name;
}