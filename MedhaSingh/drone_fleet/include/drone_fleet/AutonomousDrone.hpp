#ifndef AUTONOMOUSDRONE_HPP
#define AUTONOMOUSDRONE_HPP

#include "MissionDrone.hpp"

#include <string>
#include <tuple>
#include <vector>

class AutonomousDrone : public MissionDrone
{
    private:
    std::string ai_mode;
    std::tuple<float,float,float> home_position;
    std::vector<std::string> obstacle_log;

    public:
    AutonomousDrone(
        const std::string& name,
        float battery_level,
        float max_altitude,
        float speed,
        const std::string& mission_name,
        const std::vector<std::tuple<float,float,float>>& waypoints,
        const std::tuple<float,float,float>& home_position
    );

    void set_ai_mode(const std::string& mode);
    void detect_obstacle(
        std::tuple<float,float,float> position,
        const std::string& severity
    );
    std::vector<std::tuple<float,float,float>> auto_replan(
        const std::vector<std::tuple<float,float,float>>& obstacles
    );

    std::string get_info() const override;

};

#endif