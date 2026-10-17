#ifndef DRONE_EXCEPTIONS_HPP
#define DRONE_EXCEPTIONS_HPP

#include <exception>
#include <string>

class DroneException : public std::exception
{
protected:
    std::string message;

public:
    DroneException(const std::string& msg)
        : message(msg) {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

class BatteryDepletedError : public DroneException
{
public:
    BatteryDepletedError(const std::string& msg)
        : DroneException(msg) {}
};

class InvalidStateError : public DroneException
{
public:
    InvalidStateError(const std::string& msg)
        : DroneException(msg) {}
};

class AltitudeError : public DroneException
{
public:
    AltitudeError(const std::string& msg)
        : DroneException(msg) {}
};

#endif