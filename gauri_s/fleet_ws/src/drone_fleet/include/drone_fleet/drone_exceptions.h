#pragma once
#include <stdexcept> //std::runtime_errpr is peresent here
#include<string> 
class DroneException:public std::runtime_error{
    public:
    explicit DroneException(const std::string &msg): std::runtime_error(msg){}
};
class BatteryDepletedError: public DroneException{
    
    public:
    explicit BatteryDepletedError(const std::string &msg): DroneException(msg) {}
};
class InvalidStateError: public DroneException{
    public:
    explicit InvalidStateError(const std::string &msg): DroneException(msg){}

};
class AltitudeError:public DroneException{
    public :
    explicit AltitudeError(const std::string &msg): DroneException(msg){}
};