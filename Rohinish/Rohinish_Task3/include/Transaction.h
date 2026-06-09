#pragma once
#include <string>
#include <iostream>

class Account;

class Transaction {
    int transactionId_;
    std::string transactionType_;
    double amount_;
    std::string transactionDate_;
    Account* senderAccount_;
    Account* receiverAccount_;
    std::string status_;

    static int nextTransactionId_;
public:
    Transaction(const std::string& type, double amount, const std::string& date,
                Account* sender, Account* receiver);

    int getTransactionId() const { return transactionId_; }
    std::string getTransactionType() const { return transactionType_; }
    double getAmount() const { return amount_; }
    std::string getTransactionDate() const { return transactionDate_; }
    Account* getSenderAccount() const { return senderAccount_; }
    Account* getReceiverAccount() const { return receiverAccount_; }
    std::string getStatus() const { return status_; }

    void setStatus(const std::string& status) { status_ = status; }
    void displayTransaction() const;
};
