#include "Drone.hpp"
#include "DroneExceptions.hpp"
#include <sstream>
#include <iomanip>

Drone::Drone(const std::string& name, float battery,
             float max_altitude, float speed)
    : Vehicle(name, battery, "idle"),
      altitude_(0.0f), max_altitude_(max_altitude), speed_(speed) {}

void Drone::take_off(float target_altitude) {
    if (target_altitude > max_altitude_) {
        throw AltitudeError("Target altitude " +
                            std::to_string(target_altitude) +
                            " exceeds max " +
                            std::to_string(max_altitude_) +
                            " for drone '" + name + "'");
    }
    if (target_altitude < 0.0f) {
        throw AltitudeError("Target altitude cannot be negative");
    }
    set_status("flying");
    altitude_ = target_altitude;
    add_log_entry("Took off to altitude " + std::to_string(altitude_));
}

void Drone::land() {
    altitude_ = 0.0f;
    set_status("landed");
    add_log_entry("Landed safely");
}

void Drone::emergency_stop() {
    add_log_entry("EMERGENCY STOP initiated!");
    altitude_ = 0.0f;
    set_status("emergency");
    drain_battery(30.0f);
    add_log_entry("Emergency stop complete. Battery penalty applied.");
}

std::string Drone::get_info() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "[Drone] " << name
        << " | Battery: " << get_battery_level() << "%"
        << " | Status: " << get_status()
        << " | Altitude: " << altitude_ << "m"
        << " | Speed: " << speed_ << "m/s"
        << " | Max Alt: " << max_altitude_ << "m";
    return oss.str();
}

float Drone::get_altitude() const { return altitude_; }
float Drone::get_max_altitude() const { return max_altitude_; }
float Drone::get_speed() const { return speed_; }
