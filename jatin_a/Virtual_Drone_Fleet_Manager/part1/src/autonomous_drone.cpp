#include "autonomous_drone.hpp"

AutonomousDrone::AutonomousDrone(const std::string& name, float battery, float max_alt, float speed,
                                 const std::string& mission,
                                 const std::vector<std::tuple<float,float,float>>& wps,
                                 std::tuple<float,float,float> home)
    : MissionDrone(name, battery, max_alt, speed, mission, wps),
      ai_mode("manual"), home_position(home) {}

void AutonomousDrone::set_ai_mode(const std::string& mode) {
    ai_mode = mode;
    if (mode == "return_home") {
        waypoints.insert(waypoints.begin() + current_waypoint_index, home_position);
    }
}

void AutonomousDrone::detect_obstacle(std::tuple<float,float,float> position,
                                      const std::string& severity) {
    obstacle_log.push_back("Obstacle detected");

    if (severity == "high") {
        emergency_stop();
    }
}

std::vector<std::tuple<float,float,float>> AutonomousDrone::auto_replan(
    const std::vector<std::tuple<float,float,float>>& obstacles) {

    return waypoints; // simplified
}

std::string AutonomousDrone::get_info() const {
    return "AutonomousDrone mode: " + ai_mode;
}