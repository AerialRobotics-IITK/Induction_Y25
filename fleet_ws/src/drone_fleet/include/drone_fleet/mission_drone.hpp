#ifndef MISSION_DRONE_HPP
#define MISSION_DRONE_HPP

#include "drone_fleet/drone.hpp"
#include <tuple>
#include <vector>
#include <utility>

class MissionDrone : public Drone {
public:
    std::string mission_name;

    MissionDrone(const std::string& name, const std::string& mission_name,
                 const std::vector<std::tuple<float, float, float>>& waypoints,
                 float battery = 100.0f, float max_altitude = 120.0f,
                 float speed = 5.0f);
    ~MissionDrone() override = default;

    std::tuple<float, float, float> next_waypoint();
    void skip_waypoint(const std::string& reason);
    bool mission_complete() const;
    std::string mission_summary() const;
    std::string get_info() const override;

    int get_current_waypoint_index() const;
    int get_total_waypoints() const;
    const std::vector<std::tuple<float, float, float>>& get_waypoints() const;

    void reset_mission();

protected:
    std::vector<std::tuple<float, float, float>> waypoints_;
    int current_waypoint_index_;

private:
    std::vector<std::pair<std::tuple<float, float, float>, std::string>> visited_waypoints_;

    std::string get_timestamp() const;
};

#endif
