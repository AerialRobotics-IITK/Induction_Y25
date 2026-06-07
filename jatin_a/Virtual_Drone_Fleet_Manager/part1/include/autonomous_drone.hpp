#ifndef AUTONOMOUS_DRONE_HPP
#define AUTONOMOUS_DRONE_HPP

#include "mission_drone.hpp"

class AutonomousDrone : public MissionDrone {
private:
    std::string ai_mode;
    std::tuple<float,float,float> home_position;
    std::vector<std::string> obstacle_log;

public:
    AutonomousDrone(const std::string& name, float battery, float max_alt, float speed,
                    const std::string& mission,
                    const std::vector<std::tuple<float,float,float>>& wps,
                    std::tuple<float,float,float> home);

    void set_ai_mode(const std::string& mode);

    void detect_obstacle(std::tuple<float,float,float> position,
                         const std::string& severity);

    std::vector<std::tuple<float,float,float>> auto_replan(
        const std::vector<std::tuple<float,float,float>>& obstacles);

    std::string get_info() const override;
};

#endif