#pragma once
#include "Account.h"
#include "SavingsAccount.h"
#include "CurrentAccount.h"
#include "FixedDepositAccount.h"
#include <string>
#include <stdexcept>

class AccountFactory {
public:
    static Account* createAccount(const std::string& type, double initialBalance,
                                  const std::string& dateOpened, Branch* branch,
                                  Customer* customer) {
        if (type == "Savings")
            return new SavingsAccount(initialBalance, dateOpened, branch, customer);
        if (type == "Current")
            return new CurrentAccount(initialBalance, dateOpened, branch, customer);
        if (type == "FixedDeposit")
            return new FixedDepositAccount(initialBalance, dateOpened, branch, customer);
        throw std::invalid_argument("Unknown account type: " + type);
    }
};
