#ifndef MISSION_DRONE_HPP
#define MISSION_DRONE_HPP

#include "Drone.hpp"

#include <tuple>
#include <vector>

class MissionDrone : public Drone
{
protected:
    std::string mission_name;

    std::vector<
        std::tuple<float,float,float>
    > waypoints;

    int current_waypoint_index;

private:
    std::vector<
        std::pair<
            std::tuple<float,float,float>,
            std::string
        >
    > visited_waypoints;

public:
    MissionDrone(
        const std::string& name,
        float max_altitude,
        const std::string& mission_name);

    std::tuple<float,float,float>
    next_waypoint();

    void skip_waypoint(
        const std::string& reason);

    bool mission_complete() const;

    std::string mission_summary() const;

    std::string get_info() const override;

    void add_waypoint(float x, float y, float z);

    int get_current_waypoint_index() const;

    int get_total_waypoints() const;

    void reset_mission();
};

#endif