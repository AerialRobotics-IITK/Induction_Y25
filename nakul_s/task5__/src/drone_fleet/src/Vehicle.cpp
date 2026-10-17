#include "drone_fleet/Vehicle.hpp"
#include "drone_fleet/DroneExceptions.hpp"

#include <sstream>
#include <ctime>

static std::string timestamp()
{
    time_t now = time(nullptr);
    return ctime(&now);
}

Vehicle::Vehicle(const std::string& name)
    : name(name),
      battery_level(100),
      status("idle")
{
}

void Vehicle::add_log(const std::string& entry)
{
    flight_log.push_back(timestamp() + entry);
}

void Vehicle::set_status(const std::string& new_status)
{
    if(new_status != "idle" &&
       new_status != "flying" &&
       new_status != "charging" &&
       new_status != "landed")
    {
        throw InvalidStateError("Invalid state");
    }

    status = new_status;
    add_log("Status -> " + new_status);
}

void Vehicle::drain_battery(float amount)
{
    if(battery_level <= 0)
    {
        throw BatteryDepletedError(
            "Battery already depleted");
    }

    battery_level -= amount;

    if(battery_level < 0)
        battery_level = 0;
}

void Vehicle::charge_battery(float amount,
                             int duration_seconds)
{
    if(status != "charging")
    {
        throw InvalidStateError(
            "Drone not charging");
    }

    battery_level += amount;

    if(battery_level > 100)
        battery_level = 100;

    add_log("Charged for "
            + std::to_string(duration_seconds)
            + " seconds");
}

bool Vehicle::is_critical() const
{
    return battery_level < 20;
}

std::string Vehicle::get_flight_log() const
{
    std::stringstream ss;

    for(const auto& item : flight_log)
    {
        ss << item << "\n";
    }

    return ss.str();
}

float Vehicle::get_battery_level() const
{
    return battery_level;
}

std::string Vehicle::get_status() const
{
    return status;
}

std::string Vehicle::get_name() const
{
    return name;
}