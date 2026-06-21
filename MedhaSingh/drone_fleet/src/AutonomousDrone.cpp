#include "drone_fleet/AutonomousDrone.hpp"
#include "drone_fleet/DroneExceptions.hpp"
#include "drone_fleet/Utils.hpp"

AutonomousDrone::AutonomousDrone(
    const std::string& name,
    float battery_level,
    float max_altitude,
    float speed,
    const std::string& mission_name,
    const std::vector<std::tuple<float,float,float>>& waypoints,
    const std::tuple<float,float,float>& home_position
)
: MissionDrone(
    name,
    battery_level,
    max_altitude,
    speed,
    mission_name,
    waypoints
)
{
    this->home_position = home_position;
    ai_mode = "manual";
}

std::string AutonomousDrone::get_info() const
{
    std::string info = MissionDrone::get_info();

    info += "\nAI Mode: " + ai_mode;

    info += "\nObstacle Count: " +
            std::to_string(obstacle_log.size());

    return info;
}

void AutonomousDrone::set_ai_mode(const std::string& mode)
{
    if(mode != "manual" && mode != "auto" && mode != "return_home")
    {
        throw InvalidStateError();
    }

    ai_mode = mode;
}

void AutonomousDrone::detect_obstacle(
    std::tuple<float,float,float> position,
    const std::string& severity
)
{
    (void)position;
    obstacle_log.push_back(
        "[" + get_current_timestamp() + "] "
        + "Obstacle detected. Severity: "
        + severity
    );
    if(severity == "high")
    {
        emergency_stop();
    }
}

//revisit
std::vector<std::tuple<float,float,float>>
AutonomousDrone::auto_replan(
    const std::vector<std::tuple<float,float,float>>& obstacles
)
{
    std::vector<std::tuple<float,float,float>> new_path;

    for(const auto& obstacle : obstacles)
    {
        new_path.push_back(obstacle);
    }

    return new_path;
}