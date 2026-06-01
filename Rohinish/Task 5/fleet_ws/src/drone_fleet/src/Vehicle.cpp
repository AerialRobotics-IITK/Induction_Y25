#include "drone_fleet/Vehicle.hpp"
#include "drone_fleet/DroneExceptions.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>

static std::string current_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) % 1000;
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S")
        << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

static const std::vector<std::string> ALLOWED_STATES = {
    "idle", "flying", "charging", "landed", "emergency"
};

Vehicle::Vehicle(const std::string& name, float battery,
                 const std::string& status)
    : name(name), battery_level_(battery), status_(status) {
    add_log_entry("Vehicle '" + name + "' created with battery=" +
                  std::to_string(battery_level_) + " status=" + status_);
}

void Vehicle::drain_battery(float amount) {
    if (battery_level_ <= 0.0f) {
        throw BatteryDepletedError("Battery of '" + name +
                                   "' is already depleted");
    }
    battery_level_ = std::max(0.0f, battery_level_ - amount);
    add_log_entry("Battery drained by " + std::to_string(amount) +
                  ", now at " + std::to_string(battery_level_));
}

void Vehicle::charge_battery(float amount, int duration_seconds) {
    if (status_ != "charging") {
        throw InvalidStateError(
            "Cannot charge '" + name + "': not in charging state (current: " +
            status_ + ")");
    }
    battery_level_ = std::min(100.0f, battery_level_ + amount);
    add_log_entry("Charged by " + std::to_string(amount) + " over " +
                  std::to_string(duration_seconds) + "s, now at " +
                  std::to_string(battery_level_));
}

bool Vehicle::is_critical() const {
    return battery_level_ < 20.0f;
}

std::string Vehicle::get_flight_log() const {
    std::ostringstream oss;
    oss << "=== Flight Log for " << name << " ===\n";
    for (const auto& entry : flight_log_) {
        oss << "  " << entry << "\n";
    }
    return oss.str();
}

float Vehicle::get_battery_level() const { return battery_level_; }
std::string Vehicle::get_status() const { return status_; }

void Vehicle::set_status(const std::string& new_status) {
    auto it = std::find(ALLOWED_STATES.begin(), ALLOWED_STATES.end(),
                        new_status);
    if (it == ALLOWED_STATES.end()) {
        throw InvalidStateError("Invalid status '" + new_status +
                                "' for vehicle '" + name + "'");
    }
    std::string old = status_;
    status_ = new_status;
    add_log_entry("Status changed: " + old + " -> " + new_status);
}

void Vehicle::add_log_entry(const std::string& entry) {
    flight_log_.push_back("[" + current_timestamp() + "] " + entry);
}
