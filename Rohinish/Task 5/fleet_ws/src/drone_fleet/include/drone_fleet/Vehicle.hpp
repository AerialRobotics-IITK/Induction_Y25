#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <string>
#include <vector>

class Vehicle {
public:
    std::string name;

    explicit Vehicle(const std::string& name, float battery = 100.0f,
                     const std::string& status = "idle");
    virtual ~Vehicle() = default;

    virtual std::string get_info() const = 0;

    void drain_battery(float amount);
    void charge_battery(float amount, int duration_seconds);
    bool is_critical() const;
    std::string get_flight_log() const;

    float get_battery_level() const;
    std::string get_status() const;

protected:
    void set_status(const std::string& new_status);
    void add_log_entry(const std::string& entry);

private:
    float battery_level_;
    std::string status_;
    std::vector<std::string> flight_log_;
};

#endif
