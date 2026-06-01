#ifndef AUTONOMOUS_DRONE_HPP
#define AUTONOMOUS_DRONE_HPP

#include "MissionDrone.hpp"
#include <tuple>
#include <vector>
#include <string>

class AutonomousDrone : public MissionDrone {
public:
    AutonomousDrone(const std::string& name, const std::string& mission_name,
                    const std::vector<std::tuple<float, float, float>>& waypoints,
                    std::tuple<float, float, float> home_position,
                    float battery = 100.0f, float max_altitude = 120.0f,
                    float speed = 5.0f);
    ~AutonomousDrone() override = default;

    void set_ai_mode(const std::string& mode);
    void detect_obstacle(std::tuple<float, float, float> position,
                         const std::string& severity);
    std::vector<std::tuple<float, float, float>> auto_replan(
        const std::vector<std::tuple<float, float, float>>& obstacles);
    std::string get_info() const override;

    std::string get_ai_mode() const;

private:
    std::string ai_mode_;
    std::tuple<float, float, float> home_position_;
    std::vector<std::string> obstacle_log_;

    std::string get_timestamp() const;
};

#endif
