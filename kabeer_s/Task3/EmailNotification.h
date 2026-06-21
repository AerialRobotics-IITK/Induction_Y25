#ifndef EMAILNOTIFICATION_H
#define EMAILNOTIFICATION_H

#include "Notification.h"

class EmailNotification: public Notification{
    private:
    std::string emailAddress;
    std::string subject;
    std::string deliveryStatus;

    public:
    EmailNotification(std::string msg, std::string email) 
        : Notification(msg), emailAddress(email), deliveryStatus("Pending"){
            
        }

    void send() {
        deliveryStatus = "Sent";
    }
};

#endif