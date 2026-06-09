#include "Loan.h"

int Loan::idGenerator = 0;

Loan::Loan(std::string LoanType,
    double LoanAmount,
    double InterestRate,
    int TenureYears,
    double eMIAmount,
    Customer *Customer,
    std::string LoanStatus)
{
    loanId = ++idGenerator;
    loanType = LoanType;
    loanAmount = LoanAmount;
    interestRate = InterestRate;
    tenureYears = TenureYears;
    EMIAmount = eMIAmount;
    customer = Customer;
    loanStatus = LoanStatus;
}

int Loan::getId(){
    return loanId;
}

std::string Loan::getLoanType(){
    return loanType;
}
double Loan::getLoanAmount(){
    return loanAmount;
}
double Loan::getInterestRate(){
    return interestRate;
}
int Loan::getTenureYears(){
    return tenureYears;
}
double Loan::getEMIAmount(){
    return EMIAmount;
}
Customer *Loan::getCustomer(){
    return customer;
}
std::string Loan::getLoanStatus(){
    return loanStatus;
}