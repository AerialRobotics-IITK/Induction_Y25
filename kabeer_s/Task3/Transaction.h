#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include "Date.h"

class Account;
class Bank;

class Transaction{
    friend class Bank;
    private:
    int transactionId;
    std::string transactionType;
    double amount;
    Date transactionDate;
    Account *senderAccount;
    Account *receiverAccount;
    std::string status;
    
    static int idGenerator;

    Transaction(std::string transactionType, double amount, Date transactionDate, Account *senderAccount, Account *receiverAccount, std::string status);
    ~Transaction(){;}
    void editStatus(std::string);


    public:
    std::string getTransactionType();
    double getAmount();
    Date getTransactionDate();
    Account *getSenderAccount();
    Account *getReceiverAccount();
    std::string getStatus();
};

#endif