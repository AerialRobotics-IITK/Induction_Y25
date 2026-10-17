#include "drone_fleet/MissionDrone.hpp"
#include "drone_fleet/DroneExceptions.hpp"
#include "drone_fleet/Utils.hpp"

MissionDrone::MissionDrone(
    const std::string& name,
    float battery_level,
    float max_altitude,
    float speed,
    const std::string& mission_name,
    const std::vector<std::tuple<float,float,float>>& waypoints
)
: Drone(name, battery_level, max_altitude, speed)
{
    this->mission_name = mission_name;
    this->waypoints = waypoints;
    current_waypoint_index = 0;
}

bool MissionDrone::mission_complete() const
{
    return current_waypoint_index >=
           static_cast<int>(waypoints.size());
}

std::tuple<float,float,float> MissionDrone::next_waypoint()
{
    if (mission_complete())
    {
        throw InvalidStateError();
    }

    auto waypoint = waypoints[current_waypoint_index];

    drain_battery(1.5f);

    visited_waypoints.push_back(
        {
            waypoint,
            get_current_timestamp()
        }
    );

    current_waypoint_index++;

    return waypoint;
}

void MissionDrone::skip_waypoint(const std::string& reason)
{
    if (mission_complete())
    {
        return;
    }

    auto waypoint = waypoints[current_waypoint_index];

    visited_waypoints.push_back(
        {
            waypoint,
            reason
        }
    );

    current_waypoint_index++;
}

std::string MissionDrone::mission_summary() const
{
    std::string result;

    result += "Mission Name: " + mission_name + "\n";

    result += "Total Waypoints: " +
              std::to_string(waypoints.size()) +
              "\n";

    result += "Visited Waypoints: " +
              std::to_string(visited_waypoints.size()) +
              "\n";

    if (mission_complete())
    {
        result += "Mission Complete: Yes\n";
    }
    else
    {
        result += "Mission Complete: No\n";
    }

    return result;
}

std::string MissionDrone::get_info() const
{
    std::string info = Drone::get_info();

    info += "\nMission Name: " + mission_name;

    info += "\nRemaining Waypoints: " +
            std::to_string(
                waypoints.size() - current_waypoint_index
            );

    return info;
}

const std::vector<std::tuple<float,float,float>>&
MissionDrone::get_waypoints() const
{
    return waypoints;
}

int MissionDrone::get_current_waypoint_index() const
{
    return current_waypoint_index;
}

int MissionDrone::get_total_waypoints() const
{
    return static_cast<int>(waypoints.size());
}

std::string MissionDrone::get_mission_name() const
{
    return mission_name;
}