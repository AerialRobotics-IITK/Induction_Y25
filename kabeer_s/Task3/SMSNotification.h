#ifndef SMSNOTIFICATION_H
#define SMSNOTIFICATION_H

#include "Notification.h"

class SMSNotification: public Notification{
    private:
    std::string phoneNumber;
    std::string deliveryStatus;

    public:
    SMSNotification(std::string msg, std::string phone) 
        : Notification(msg), phoneNumber(phone), deliveryStatus("Pending"){
            
        }

    void send() {
        deliveryStatus = "Sent";
    }
};

#endif