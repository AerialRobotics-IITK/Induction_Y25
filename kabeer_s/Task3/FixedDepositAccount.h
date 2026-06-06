#ifndef FIXEDDEPOSITACCOUNT_H
#define FIXEDDEPOSITACCOUNT_H

#include "Account.h"
#include "Date.h"

class Branch;

class FixedDepositAccount: public Account{
    friend class Branch;
    private:
    double FDAmount;
    Date maturityDate;
    double FDInterestRate;
    int tenureMonths;

    FixedDepositAccount(std::string Type, double Balance, Date DateOpened, std::string Status, Branch *Branche, Customer * Cus, double fdamount, Date maturity, double interest, int tenure) : 
    Account(Type, Balance, DateOpened, Status, Branche, Cus), FDAmount(fdamount), maturityDate(maturity), FDInterestRate(interest), tenureMonths(tenure){
        ;
    };

    ~FixedDepositAccount(){
        ;
    }
};

#endif