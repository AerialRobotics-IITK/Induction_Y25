#include "drone_fleet/Drone.hpp"
#include "drone_fleet/DroneExceptions.hpp"

#include <sstream>

Drone::Drone(const std::string& name,
             float max_altitude)
    : Vehicle(name),
      altitude(0),
      max_altitude(max_altitude),
      speed(0)
{
}

void Drone::take_off(float target_altitude)
{
    if(target_altitude > max_altitude)
    {
        throw AltitudeError(
            "Altitude exceeds limit");
    }

    altitude = target_altitude;
    set_status("flying");
}

void Drone::land()
{
    altitude = 0;
    speed = 0;

    set_status("landed");
}

void Drone::emergency_stop()
{
    speed = 0;

    drain_battery(30);
}

std::string Drone::get_info() const
{
    std::stringstream ss;

    ss << "Drone: "
       << name
       << "\nAltitude: "
       << altitude
       << "\nBattery: "
       << get_battery_level();

    return ss.str();
}

float Drone::get_altitude() const
{
    return altitude;
}

float Drone::get_speed() const
{
    return speed;
}