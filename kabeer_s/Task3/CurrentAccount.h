#ifndef CURRENTACCOUNT_H
#define CURRENTACCOUNT_H

#include "Account.h"

class Branch;

class CurrentAccount: public Account{
    friend class Branch;
    private:
    double overdraftLimit;
    std::string businessName;
    CurrentAccount(std::string Type, double Balance, Date DateOpened, std::string Status, Branch *Branche, Customer * Cus, double limit, std::string name) : Account(Type, Balance, DateOpened, Status, Branche, Cus), overdraftLimit(limit), businessName(name){
        ;
    }

    ~CurrentAccount(){
        ;
    }
};

#endif