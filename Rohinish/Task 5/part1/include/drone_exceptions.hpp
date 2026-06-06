#ifndef DRONE_EXCEPTIONS_HPP
#define DRONE_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

class DroneException : public std::runtime_error {
public:
    explicit DroneException(const std::string& msg)
        : std::runtime_error(msg) {}
};

class BatteryDepletedError : public DroneException {
public:
    explicit BatteryDepletedError(const std::string& msg = "Battery is depleted")
        : DroneException(msg) {}
};

class InvalidStateError : public DroneException {
public:
    explicit InvalidStateError(const std::string& msg = "Invalid state transition")
        : DroneException(msg) {}
};

class AltitudeError : public DroneException {
public:
    explicit AltitudeError(const std::string& msg = "Altitude limit exceeded")
        : DroneException(msg) {}
};

#endif
