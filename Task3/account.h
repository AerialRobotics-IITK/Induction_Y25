#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "transaction.h"

class Account
{
    long m_accountNumber{};
    std::string m_accountType{};
    double m_balance{};
    std::vector<Transaction> transactions{};

public:
    void deposit(double amount)
    {
        m_balance += amount;
        transactions.push_back(Transaction{"deposit", amount});
        std::cout << "Your money has been deposited successfully!\n";
    }

    void withdraw(double amount)
    {
        if(m_balance >= amount)
        {
            m_balance -= amount;
            transactions.push_back(Transaction{"withdraw", amount});
            std::cout << "Amount withdrawn successfully!\n";
        }
        else
        {
            std::cout << "You don't have enough money in your account to perform that transaction!\n";
        }
    }

    long getAccountNumber() const
    {
        return m_accountNumber;
    }

    void printBalance() const
    {
        std::cout << "Your current balance is: " << m_balance << std::endl;
    }

    void printTHistory() const
    {
        for(auto& txn : transactions)
        {
            std::cout << txn.getTType() << " \t" << txn.getAmount() << '\n';
        }

    }

    Account(long accnumber, std::string type, double balance)
        : m_accountNumber {accnumber}
        , m_accountType {type}
        , m_balance {balance}
    {
        std::cout << "Account created! Your account number is: " << accnumber <<std::endl; 
    }

};

//function to look for an account in a given accounts vector and a given account number.
Account* findAccount(long, std::vector<Account>&);
