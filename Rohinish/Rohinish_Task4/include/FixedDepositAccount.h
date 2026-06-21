#pragma once
#include "Account.h"

class FixedDepositAccount : public Account {
    double fdAmount_;
    std::string maturityDate_;
    double fdInterestRate_;
    int tenureMonths_;
public:
    FixedDepositAccount(double fdAmount, const std::string& dateOpened,
                        Branch* branch, Customer* customer,
                        double fdInterestRate = 7.0, int tenureMonths = 12);

    void deposit(double amount) override;
    void withdraw(double amount) override;
    double calculateInterest() const override;
    void displayAccountInfo() const override;

    double getFDAmount() const { return fdAmount_; }
    std::string getMaturityDate() const { return maturityDate_; }
    double getFDInterestRate() const { return fdInterestRate_; }
    int getTenureMonths() const { return tenureMonths_; }

    double getMaturityAmount() const;
};
