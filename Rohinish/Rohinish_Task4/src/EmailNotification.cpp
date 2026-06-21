#include "EmailNotification.h"

EmailNotification::EmailNotification(const std::string& emailAddress, const std::string& subject,
                                     const std::string& message)
    : Notification(message), emailAddress_(emailAddress), subject_(subject),
      deliveryStatus_("Pending") {}

void EmailNotification::sendNotification() {
    std::cout << "[Email -> " << emailAddress_ << "] Subject: " << subject_
              << " | Body: " << message_ << std::endl;
    deliveryStatus_ = "Sent";
}
