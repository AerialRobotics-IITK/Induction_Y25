#ifndef DRONE_HPP
#define DRONE_HPP

#include "drone_fleet/vehicle.hpp"

class Drone : public Vehicle {
public:
    Drone(const std::string& name, float battery = 100.0f,
          float max_altitude = 120.0f, float speed = 5.0f);
    ~Drone() override = default;

    void take_off(float target_altitude);
    void land();
    void emergency_stop();
    std::string get_info() const override;

    float get_altitude() const;
    float get_max_altitude() const;
    float get_speed() const;

protected:
    float altitude_;
    float max_altitude_;

private:
    float speed_;
};

#endif
