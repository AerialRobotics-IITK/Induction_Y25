#include "drone_fleet/AutonomousDrone.hpp"

#include <sstream>
#include <cmath>

AutonomousDrone::AutonomousDrone(
    const std::string& name,
    float max_altitude,
    const std::string& mission_name)
    : MissionDrone(
        name,
        max_altitude,
        mission_name),
      ai_mode("manual"),
      home_position({0,0,0})
{
}

void AutonomousDrone::set_ai_mode(
    const std::string& mode)
{
    ai_mode = mode;

    if(mode == "return_home")
    {
        waypoints.insert(
            waypoints.begin()
            + current_waypoint_index,
            home_position);
    }
}

void AutonomousDrone::detect_obstacle(
    std::tuple<float,float,float> position,
    const std::string& severity)
{
    obstacle_log.push_back(severity);

    if(severity == "high")
    {
        emergency_stop();
    }
}

std::vector<
    std::tuple<float,float,float>
>
AutonomousDrone::auto_replan(
    const std::vector<
        std::tuple<float,float,float>
    >& obstacles)
{
    std::vector<
        std::tuple<float,float,float>
    > new_path;

    for(auto wp : waypoints)
    {
        bool safe = true;

        for(auto ob : obstacles)
        {
            float dx =
                std::get<0>(wp)
                - std::get<0>(ob);

            float dy =
                std::get<1>(wp)
                - std::get<1>(ob);

            float dz =
                std::get<2>(wp)
                - std::get<2>(ob);

            float dist =
                sqrt(dx*dx +
                     dy*dy +
                     dz*dz);

            if(dist < 5)
            {
                safe = false;
                break;
            }
        }

        if(safe)
            new_path.push_back(wp);
    }

    return new_path;
}

std::string AutonomousDrone::get_info() const
{
    return MissionDrone::get_info()
           + "\nAI Mode: "
           + ai_mode;
}