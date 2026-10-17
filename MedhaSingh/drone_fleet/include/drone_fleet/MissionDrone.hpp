#ifndef MISSIONDRONE_HPP
#define MISSIONDRONE_HPP

#include "Drone.hpp"

#include <vector>
#include <tuple>
#include <string>
#include <utility>

class MissionDrone : public Drone
{
    private:
    std::string mission_name;
    std::vector<std::tuple<float,float,float>> waypoints;
    int current_waypoint_index;
    std::vector<std::pair<std::tuple<float,float,float>, std::string>>visited_waypoints;

    protected:
    const std::vector<std::tuple<float,float,float>>&
    get_waypoints() const;

    public:
    MissionDrone(
    const std::string& name,
    float battery_level,
    float max_altitude,
    float speed,
    const std::string& mission_name,
    const std::vector<std::tuple<float,float,float>>& waypoints
    );

    std::tuple<float,float,float> next_waypoint();

    void skip_waypoint(const std::string& reason);

    bool mission_complete() const;

    std::string mission_summary() const;

    std::string get_info() const override;

    int get_current_waypoint_index() const;

    int get_total_waypoints() const;

    std::string get_mission_name() const;

};

#endif