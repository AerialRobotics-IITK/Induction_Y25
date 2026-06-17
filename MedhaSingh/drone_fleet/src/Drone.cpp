#include "drone_fleet/Drone.hpp"
#include "drone_fleet/DroneExceptions.hpp"

Drone::Drone::Drone(
    const std::string& name,
    float battery_level,
    float max_altitude,
    float speed
)
: Vehicle(name, battery_level)
{
    this->max_altitude = max_altitude;
    this->speed = speed;
    altitude = 0.0f;
}

void Drone::take_off(float target_altitude)
{
    if(target_altitude > max_altitude)
    {
        throw AltitudeError();
    }

    altitude = target_altitude;

    set_status("flying");
}

void Drone::land()
{
    altitude = 0;
    set_status("idle");
}

void Drone::emergency_stop()
{
    set_status("emergency");
    drain_battery(30);
}

std::string Drone::get_info() const
{
    return "Name: " + get_name() +
           "\nBattery: " + std::to_string(get_battery_level()) +
           "\nAltitude: " + std::to_string(altitude);
}

float Drone::get_altitude() const
{
    return altitude;
}

float Drone::get_max_altitude() const
{
    return max_altitude;
}

float Drone::get_speed() const
{
    return speed;
}