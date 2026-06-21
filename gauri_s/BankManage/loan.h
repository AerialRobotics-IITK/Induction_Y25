#pragma once
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "exceptions.h"

class Customer;

class Loan {
private:
    static int idCounter;
public:
    int loanId;
    std::string loanType;        // "Home", "Car", "Personal"
    double  loanAmount;
    double interestRate;    // annual %
    int tenureYears;
    double EMIAmount;
    std::string loanStatus;      // "Pending", "Approved", "Rejected", "Closed"
    Customer* customer;

    Loan(const std::string& type,
         double amount,
         double rate,
         int tenure,
         Customer* cust)
        : loanId(++idCounter),
          loanType(type),
          loanAmount(amount),
          interestRate(rate),
          tenureYears(tenure),
          loanStatus("Pending"),
          customer(cust) {
        EMIAmount = calculateEMI();
    }

    // Standard EMI formula: EMI = P*r*(1+r)^n / ((1+r)^n - 1)
    double calculateEMI() const {
        double r = (interestRate / 100.0) / 12.0;
        int    n = tenureYears * 12;
        if (r == 0) return loanAmount / n;
        double factor = std::pow(1.0 + r, n);
        return loanAmount * r * factor / (factor - 1.0);
    }

    void approve() {
        if (loanAmount > 10000000.0)  // ₹1 Crore cap (simple rule)
            throw LoanRejectedException("Loan amount exceeds sanctioned limit.");
        loanStatus = "Approved";
    }

    void reject() { loanStatus = "Rejected"; }
    void close()  { loanStatus = "Closed";   }

    void display() const {
        std::cout << std::fixed << std::setprecision(2)
                  << "  [Loan #" << loanId << "] "
                  << loanType
                  << " | Principal: ₹" << loanAmount
                  << " | Rate: " << interestRate << "%"
                  << " | Tenure: " << tenureYears << " yrs"
                  << " | EMI: ₹" << EMIAmount
                  << " | Status: " << loanStatus << "\n";
    }
};

int Loan::idCounter = 0;
