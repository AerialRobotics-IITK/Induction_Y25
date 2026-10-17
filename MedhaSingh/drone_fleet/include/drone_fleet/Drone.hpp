#ifndef DRONE_HPP
#define DRONE_HPP

#include "Vehicle.hpp"

class Drone : public Vehicle
{
    protected:
    float altitude;
    float max_altitude;

    private:
    float speed;

    public:
    Drone(
        const std::string& name,
        float battery_level,
        float max_altitude,
        float speed
    );

    void take_off(float target_altitude);

    void land();

    void emergency_stop();

    std::string get_info() const override;

    float get_altitude() const;

    float get_max_altitude() const;

    float get_speed() const;
};

#endif