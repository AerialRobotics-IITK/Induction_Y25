#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <string>
#include <iostream>
#include <chrono>

class Vehicle {
private:
    std::string name;
    double battery;

public:
    Vehicle(const std::string& n, double b) : name(n), battery(b) {}
    virtual ~Vehicle() = default;
    
    virtual std::string get_info() const {
        return "Vehicle: " + name;
    }
    
    double get_battery() const { return battery; }
    void set_battery(double b) { battery = b; }
};

#endif
