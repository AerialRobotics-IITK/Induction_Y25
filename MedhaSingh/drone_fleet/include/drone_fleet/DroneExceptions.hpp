#ifndef DRONE_EXCEPTIONS_HPP
#define DRONE_EXCEPTIONS_HPP

#include <exception>
#include <string>

class DroneException : public std::exception{
    protected:
    std::string message;

    public:
    DroneException(const std::string& msg)
        : message(msg)
    {
    }

    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

class BatteryDepletedError : public DroneException
{
public:
    BatteryDepletedError()
        : DroneException("Battery is depleted")
    {
    }
};

class InvalidStateError : public DroneException
{
public:
    InvalidStateError()
        : DroneException("Invalid state")
    {
    }
};

class AltitudeError : public DroneException
{
public:
    AltitudeError()
        : DroneException("Altitude limit exceeded")
    {
    }
};

#endif 