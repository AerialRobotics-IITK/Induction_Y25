#pragma once
#include "Account.h"

class SavingsAccount : public Account {
    double interestRate_;
    double minimumBalance_;
public:
    SavingsAccount(double initialBalance, const std::string& dateOpened,
                   Branch* branch, Customer* customer,
                   double interestRate = 4.0, double minimumBalance = 1000.0);

    void deposit(double amount) override;
    void withdraw(double amount) override;
    double calculateInterest() const override;
    void displayAccountInfo() const override;

    double getInterestRate() const { return interestRate_; }
    double getMinimumBalance() const { return minimumBalance_; }
    void setInterestRate(double rate) { interestRate_ = rate; }
    void setMinimumBalance(double min) { minimumBalance_ = min; }
};
