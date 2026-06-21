#ifndef LOAN_H
#define LOAN_H

#include <string>
#include "Date.h"

class Customer;
class Bank;

class Loan{
    friend class Bank;

    private:
    int loanId;
    std::string loanType;
    double loanAmount;
    double interestRate;
    int tenureYears;
    double EMIAmount;
    Customer *customer;
    std::string loanStatus;
    
    static int idGenerator;

    Loan(std::string loanType,
    double loanAmount,
    double interestRate,
    int tenureYears,
    double EMIAmount,
    Customer *customer,
    std::string loanStatus);
    ~Loan(){
        ;
    }

    public:
    int getId();
    std::string getLoanType();
    double getLoanAmount();
    double getInterestRate();
    int getTenureYears();
    double getEMIAmount();
    Customer *getCustomer();
    std::string getLoanStatus();
};

#endif