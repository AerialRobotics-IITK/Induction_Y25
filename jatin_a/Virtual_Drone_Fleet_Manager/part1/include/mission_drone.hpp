#ifndef MISSION_DRONE_HPP
#define MISSION_DRONE_HPP

#include "drone.hpp"
#include <tuple>
#include <vector>

class MissionDrone : public Drone {
protected:
    std::string mission_name;
    std::vector<std::tuple<float, float, float>> waypoints;
    int current_waypoint_index;

private:
    std::vector<std::pair<std::tuple<float,float,float>, std::string>> visited_waypoints;

public:
    MissionDrone(const std::string& name, float battery, float max_alt, float speed,
                 const std::string& mission,
                 const std::vector<std::tuple<float,float,float>>& wps);

    std::tuple<float,float,float> next_waypoint();
    void skip_waypoint(const std::string& reason);

    bool mission_complete() const;
    std::string mission_summary() const;

    std::string get_info() const override;
};

#endif