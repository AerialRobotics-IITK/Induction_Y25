#include "drone.hpp"
#include "drone_exceptions.hpp"

Drone::Drone(const std::string& name, float battery, float max_alt, float speed)
    : Vehicle(name, battery), altitude(0), max_altitude(max_alt), speed(speed) {}

void Drone::take_off(float target_altitude) {
    if (target_altitude > max_altitude)
        throw AltitudeError("Target exceeds max altitude");

    altitude = target_altitude;
    set_status("flying");
}

void Drone::land() {
    altitude = 0;
    set_status("idle");
}

void Drone::emergency_stop() {
    drain_battery(30);
    land();
}

float Drone::get_altitude() const { return altitude; }
float Drone::get_speed() const { return speed; }

std::string Drone::get_info() const {
    return "Drone: " + name +
           " Battery: " + std::to_string(get_battery()) +
           " Altitude: " + std::to_string(altitude);
}