#ifndef DRONE_HPP
#define DRONE_HPP

#include "vehicle.hpp"

class Drone : public Vehicle {
protected:
    float altitude;
    float max_altitude;

private:
    float speed;

public:
    Drone(const std::string& name, float battery, float max_alt, float speed);

    virtual void take_off(float target_altitude);
    void land();
    void emergency_stop();

    float get_altitude() const;
    float get_speed() const;

    std::string get_info() const override;
};

#endif