#ifndef ACCOUNT_H
#define ACCOUNT_H

/*
accountNumber	long	Unique account number
accountType	String	Savings/current
balance	double	Current balance
dateOpened	Date	Opening date
status	String	Active/blocked
branch	Branch	Linked branch
customer	Customer	Owner
transactions	List	Transaction history
*/

#include <string>
#include <list>
#include "Date.h"
#include "Bank.h"

class Customer;
class Transaction;
class ATMCard;
class Branch;

class Account{
    friend class Branch;
    friend class Bank;
    private:
    long accountNumber;
    std::string accountType;
    double balance;
    Date dateOpened;
    std::string status;
    Branch* branch;
    Customer* customer;
    std::list<Transaction *> transactionHistory;
    std::list<ATMCard *> ATMcards;

    static long numberGenerator;

    // operated by branch and child classes
    protected:
    Account(std::string Type, double Balance, Date DateOpened, std::string Status, Branch *Branche, Customer * Cus);
    virtual ~Account();
    void setStatus(std::string status);
    void setBalance(double balance);
    void addCard(ATMCard * card);
    int removeCard(long cardNumber);

    // operated by bank
    void addTransaction(Transaction *);
    
    public:
    long getAccountNumber();
    std::string getAccountType();
    double getBalance();
    Date getDateOpened();
    std::string getStatus();
    Branch* getBranch();
    Customer* getCustomer();
    std::list<Transaction *> getTransactionHistory();
};

#endif