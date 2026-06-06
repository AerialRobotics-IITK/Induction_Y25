#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

class Notification{
    protected:
    std::string message;
    Notification(std::string msg);
    public:
    virtual ~Notification(){
        ;
    };
    virtual void send() = 0;
};

#endif