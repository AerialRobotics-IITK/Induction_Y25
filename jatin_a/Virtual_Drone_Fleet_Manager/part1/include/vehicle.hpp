#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <string>
#include <vector>

class Vehicle {
protected:
    std::string name;

private:
    float battery_level;
    std::string status;
    std::vector<std::string> flight_log;

protected:
    void log_event(const std::string& event);
    std::string current_timestamp() const;

public:
    Vehicle(const std::string& name, float battery);

    virtual ~Vehicle() = default;

    virtual std::string get_info() const = 0;

    void drain_battery(float amount);
    void charge_battery(float amount, int duration_seconds);

    void set_status(const std::string& new_status);

    bool is_critical() const;

    std::string get_flight_log() const;

    // getters
    float get_battery() const;
    std::string get_status() const;
    std::string get_name() const;
};

#endif