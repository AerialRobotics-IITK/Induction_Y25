#pragma once
#include "Notification.h"

class SMSNotification : public Notification {
    std::string phoneNumber_;
    std::string deliveryStatus_;
public:
    SMSNotification(const std::string& phoneNumber, const std::string& message);

    void sendNotification() override;

    std::string getPhoneNumber() const { return phoneNumber_; }
    std::string getDeliveryStatus() const { return deliveryStatus_; }
};
