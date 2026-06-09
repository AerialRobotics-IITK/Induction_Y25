#pragma once
#include "Notification.h"
#include <memory>

class NotificationService {
    std::unique_ptr<Notification> strategy_;
public:
    NotificationService() = default;

    void setStrategy(std::unique_ptr<Notification> strategy);
    void notify(const std::string& message);
    Notification* getStrategy() const { return strategy_.get(); }
};
