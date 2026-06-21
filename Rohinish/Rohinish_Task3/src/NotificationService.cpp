#include "NotificationService.h"

void NotificationService::setStrategy(std::unique_ptr<Notification> strategy) {
    strategy_ = std::move(strategy);
}

void NotificationService::notify(const std::string& message) {
    if (strategy_) {
        strategy_->setMessage(message);
        strategy_->sendNotification();
    }
}
