#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <string>
#include <vector>

class Vehicle{
    protected:
    std::string name;

    private:
    float battery_level;
    std::string status;
    std::vector<std::string> flight_log;

    protected:
    void add_log(const std::string& entry);

    public:
    Vehicle(
        const std::string& name,
        float battery_level = 100.0f
    );

    virtual ~Vehicle() = default;

    virtual std::string get_info() const = 0;

    void drain_battery(float amount);

    void charge_battery(float amount, int duration_seconds);

    bool is_critical() const;

    std::string get_flight_log() const;

    void set_status(const std::string& new_status);

    float get_battery_level() const;

    std::string get_status() const;

    std::string get_name() const;
};

#endif