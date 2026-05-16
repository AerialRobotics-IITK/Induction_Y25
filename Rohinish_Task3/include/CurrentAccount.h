#pragma once
#include "Account.h"

class CurrentAccount : public Account {
    double overdraftLimit_;
    std::string businessName_;
public:
    CurrentAccount(double initialBalance, const std::string& dateOpened,
                   Branch* branch, Customer* customer,
                   double overdraftLimit = 50000.0,
                   const std::string& businessName = "");

    void deposit(double amount) override;
    void withdraw(double amount) override;
    double calculateInterest() const override;
    void displayAccountInfo() const override;

    double getOverdraftLimit() const { return overdraftLimit_; }
    std::string getBusinessName() const { return businessName_; }
    void setOverdraftLimit(double limit) { overdraftLimit_ = limit; }
    void setBusinessName(const std::string& name) { businessName_ = name; }
};
