#pragma once
#include <string>
#include <vector>
#include <iostream>

class Branch;
class Customer;
class Transaction;

class Account {
protected:
    long accountNumber_;
    std::string accountType_;
    double balance_;
    std::string dateOpened_;
    std::string status_;
    Branch* branch_;
    Customer* customer_;
    std::vector<Transaction*> transactions_;

    static long nextAccountNumber_;

public:
    Account(const std::string& type, double initialBalance,
            const std::string& dateOpened, Branch* branch, Customer* customer);
    virtual ~Account() = default;

    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
    virtual double calculateInterest() const = 0;
    virtual void displayAccountInfo() const = 0;

    long getAccountNumber() const { return accountNumber_; }
    std::string getAccountType() const { return accountType_; }
    double getBalance() const { return balance_; }
    std::string getDateOpened() const { return dateOpened_; }
    std::string getStatus() const { return status_; }
    Branch* getBranch() const { return branch_; }
    Customer* getCustomer() const { return customer_; }
    const std::vector<Transaction*>& getTransactions() const { return transactions_; }

    void setStatus(const std::string& status) { status_ = status; }
    void addTransaction(Transaction* txn) { transactions_.push_back(txn); }
};
