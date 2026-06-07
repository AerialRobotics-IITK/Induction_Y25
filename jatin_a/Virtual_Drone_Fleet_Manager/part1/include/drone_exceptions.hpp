#ifndef DRONE_EXCEPTIONS_HPP
#define DRONE_EXCEPTIONS_HPP

#include <exception>
#include <string>

class DroneException : public std::exception {
protected:
    std::string message;

public:
    explicit DroneException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class BatteryDepletedError : public DroneException {
public:
    BatteryDepletedError() : DroneException("Battery is already depleted!") {}
};

class InvalidStateError : public DroneException {
public:
    InvalidStateError(const std::string& msg)
        : DroneException("Invalid State: " + msg) {}
};

class AltitudeError : public DroneException {
public:
    AltitudeError(const std::string& msg)
        : DroneException("Altitude Error: " + msg) {}
};

#endif