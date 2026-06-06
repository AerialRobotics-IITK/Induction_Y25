#ifndef SAVINGSACCOUNT_H
#define SAVINGSACCOUNT_H

#include "Account.h"
#include "Branch.h"

class Branch;

class SavingsAccount: public Account{
    friend class Branch;
    private:
    double interest;
    double minimumBalance;
    SavingsAccount(std::string Type, double Balance, Date DateOpened, std::string Status, Branch *Branche, Customer * Cus, double bal, double rate) : Account(Type, Balance, DateOpened, Status, Branche, Cus), minimumBalance(bal), interest(rate){
        ;
    }

    ~SavingsAccount(){
        ;
    }
};

#endif