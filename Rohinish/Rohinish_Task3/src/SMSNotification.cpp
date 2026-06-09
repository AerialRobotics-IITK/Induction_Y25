#include "SMSNotification.h"

SMSNotification::SMSNotification(const std::string& phoneNumber, const std::string& message)
    : Notification(message), phoneNumber_(phoneNumber), deliveryStatus_("Pending") {}

void SMSNotification::sendNotification() {
    std::cout << "[SMS -> " << phoneNumber_ << "] " << message_ << std::endl;
    deliveryStatus_ = "Sent";
}
