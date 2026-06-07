#include "vehicle.hpp"
#include "drone_exceptions.hpp"
#include <chrono>
#include <sstream>
#include <iomanip>

Vehicle::Vehicle(const std::string& name, float battery)
    : name(name), battery_level(battery), status("idle") {}

std::string Vehicle::current_timestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%H:%M:%S");
    return ss.str();
}

void Vehicle::log_event(const std::string& event) {
    flight_log.push_back("[" + current_timestamp() + "] " + event);
}

void Vehicle::drain_battery(float amount) {

    //  not enough battery for action
    if (battery_level < amount) {
        throw BatteryDepletedError();
    }

    battery_level -= amount;

    log_event("Battery drained");
}

void Vehicle::charge_battery(float amount, int duration_seconds) {
    if (status != "charging")
        throw InvalidStateError("Not in charging state");

    battery_level += amount;
    if (battery_level > 100) battery_level = 100;

    log_event("Battery charged for " + std::to_string(duration_seconds) + " seconds");
}

void Vehicle::set_status(const std::string& new_status) {
    if (new_status != "idle" && new_status != "flying" && new_status != "charging")
        throw InvalidStateError("Unknown status");

    status = new_status;
    log_event("Status changed to " + new_status);
}

bool Vehicle::is_critical() const {
    return battery_level < 20;
}

std::string Vehicle::get_flight_log() const {
    std::string result;
    for (const auto& log : flight_log)
        result += log + "\n";
    return result;
}

float Vehicle::get_battery() const { return battery_level; }
std::string Vehicle::get_status() const { return status; }
std::string Vehicle::get_name() const { return name; }