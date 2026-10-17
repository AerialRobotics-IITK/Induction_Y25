#include "drone_fleet/MissionDrone.hpp"

#include <sstream>
#include <ctime>

MissionDrone::MissionDrone(
    const std::string& name,
    float max_altitude,
    const std::string& mission_name)
    : Drone(name,max_altitude),
      mission_name(mission_name),
      current_waypoint_index(0)
{
}

std::tuple<float,float,float>
MissionDrone::next_waypoint()
{
    drain_battery(1.5);

    auto wp =
        waypoints[current_waypoint_index++];

    time_t now = time(nullptr);

    visited_waypoints.push_back(
        {wp, ctime(&now)}
    );

    return wp;
}

void MissionDrone::skip_waypoint(
    const std::string& reason)
{
    current_waypoint_index++;
}

bool MissionDrone::mission_complete() const
{
    return current_waypoint_index >=
           waypoints.size();
}

std::string MissionDrone::mission_summary() const
{
    std::stringstream ss;

    ss << "Mission: "
       << mission_name
       << "\nVisited: "
       << current_waypoint_index;

    return ss.str();
}

std::string MissionDrone::get_info() const
{
    return Drone::get_info()
           + "\nMission: "
           + mission_name;
}

void MissionDrone::add_waypoint(float x,
                                float y,
                                float z)
{
    waypoints.push_back(
        std::make_tuple(x, y, z)
    );

    add_log(
        "Waypoint added: ("
        + std::to_string(x) + ", "
        + std::to_string(y) + ", "
        + std::to_string(z) + ")"
    );
}

int MissionDrone::get_current_waypoint_index() const
{
    return current_waypoint_index;
}

int MissionDrone::get_total_waypoints() const
{
    return static_cast<int>(waypoints.size());
}

void MissionDrone::reset_mission()
{
    current_waypoint_index = 0;
}