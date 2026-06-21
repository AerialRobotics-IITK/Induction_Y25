#pragma once //It tells the compiler: "Only include this file once during compilation, no matter how many times it gets called."
#include <string>
#include <iostream>

// ─── Notification Interface ───────────────────────────────────────────────────
class Notification {
public:
    std::string message;
    virtual void sendNotification() = 0;
    virtual ~Notification() = default;
};

// ─── SMS Notification ─────────────────────────────────────────────────────────
class SMSNotification : public Notification {
public:
    std::string phoneNumber;
    std::string deliveryStatus;

    SMSNotification(const std::string& phone, const std::string& msg) {
        phoneNumber    = phone;
        message        = msg;
        deliveryStatus = "Pending";
    }

    void sendNotification() override {
        std::cout << "[SMS] To: " << phoneNumber
                  << " | Message: " << message << "\n";
        deliveryStatus = "Sent";
    }
};

// ─── Email Notification ───────────────────────────────────────────────────────
class EmailNotification : public Notification {
public:
    std::string emailAddress;
    std::string subject;
    std::string deliveryStatus;

    EmailNotification(const std::string& email,
                      const std::string& subj,
                      const std::string& msg) {
        emailAddress   = email;
        subject        = subj;
        message        = msg;
        deliveryStatus = "Pending";
    }

    void sendNotification() override {
        std::cout << "[EMAIL] To: " << emailAddress
                  << " | Subject: " << subject
                  << " | Message: " << message << "\n";
        deliveryStatus = "Sent";
    }
};
