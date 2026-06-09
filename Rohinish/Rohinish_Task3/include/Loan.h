#pragma once
#include <string>
#include <iostream>
#include <cmath>

class Customer;

class Loan {
    int loanId_;
    std::string loanType_;
    double loanAmount_;
    double interestRate_;
    int tenureYears_;
    double emiAmount_;
    std::string loanStatus_;
    Customer* customer_;

    static int nextLoanId_;

    double computeEMI() const;
public:
    Loan(const std::string& loanType, double loanAmount, double interestRate,
         int tenureYears, Customer* customer);

    int getLoanId() const { return loanId_; }
    std::string getLoanType() const { return loanType_; }
    double getLoanAmount() const { return loanAmount_; }
    double getInterestRate() const { return interestRate_; }
    int getTenureYears() const { return tenureYears_; }
    double getEMIAmount() const { return emiAmount_; }
    std::string getLoanStatus() const { return loanStatus_; }
    Customer* getCustomer() const { return customer_; }

    void approve();
    void reject(const std::string& reason = "");
    void displayLoanInfo() const;
};
