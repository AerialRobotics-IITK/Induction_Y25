#pragma once
#include <string>
#include <iostream>

class Notification {
protected:
    std::string message_;
public:
    explicit Notification(const std::string& message = "") : message_(message) {}
    virtual ~Notification() = default;

    virtual void sendNotification() = 0;

    void setMessage(const std::string& msg) { message_ = msg; }
    std::string getMessage() const { return message_; }
};
