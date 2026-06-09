#pragma once
#include "Notification.h"

class EmailNotification : public Notification {
    std::string emailAddress_;
    std::string subject_;
    std::string deliveryStatus_;
public:
    EmailNotification(const std::string& emailAddress, const std::string& subject,
                      const std::string& message);

    void sendNotification() override;

    std::string getEmailAddress() const { return emailAddress_; }
    std::string getSubject() const { return subject_; }
    std::string getDeliveryStatus() const { return deliveryStatus_; }
};
