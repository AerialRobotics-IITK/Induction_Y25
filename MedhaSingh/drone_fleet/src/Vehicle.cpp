#include "drone_fleet/Vehicle.hpp"
#include "drone_fleet/DroneExceptions.hpp"
#include "drone_fleet/Utils.hpp"

Vehicle::Vehicle(
    const std::string& name,
    float battery_level
)
{
    this->name = name;
    this->battery_level = battery_level;
    status = "idle";
}

std::string Vehicle::get_name() const
{
    return name;
}

std::string Vehicle::get_status() const
{
    return status;
}

float Vehicle::get_battery_level() const
{
    return battery_level;
}

void Vehicle::add_log(const std::string& entry)
{
    flight_log.push_back(
        "[" + get_current_timestamp() + "] " + entry
    );
}

std::string Vehicle::get_flight_log() const
{
    std::string result = "";

    for(const auto& entry : flight_log)
    {
        result += entry + "\n";
    }

    return result;
}

void Vehicle::set_status(const std::string& new_status)
{
    if (new_status != "idle" &&
        new_status != "flying" &&
        new_status != "landing" &&
        new_status != "charging" &&
        new_status != "emergency")
    {
        throw InvalidStateError();
    }
    
    status = new_status;
    add_log("Status changed to " + new_status);
    
}

bool Vehicle::is_critical() const
{
    return battery_level < 20.0f;
}

void Vehicle::drain_battery(float amount)
{
    if (battery_level<=0)
    {
        throw BatteryDepletedError();
    }

    battery_level-=amount;

    if (battery_level<0)
    {
        battery_level=0;
    }

    add_log("Battery drained by " + std::to_string(amount));

}

void Vehicle::charge_battery(float amount, int duration_seconds)
{
    if (status!="charging")
    {
        throw InvalidStateError();
    }

    battery_level+=amount;

    if (battery_level>100)
    {
        battery_level=100;
    }

    add_log("Battery charged by " + std::to_string(amount) + " in " + std::to_string(duration_seconds) + " seconds");

}