#ifndef BANK_H
#define BANK_H

#include <string>
#include <list>
#include "Date.h"

class Branch;
class Customer;
class Employee;
class Account;
class Loan;
class Transaction;

class Bank {
    private:
    int bankId;
    std::string bankName;
    std::list<Branch *> branches;
    std::list<Customer * > customers;
    std::list<Employee *> employees;
    std::list<Loan *> loans;
    std::list<Transaction *> transactionHistory;
    

    static int idGenerator;

    public:
    Bank(std::string name);
    ~Bank();
    int addEmployee(std::string fullName, std::string designation, double salary); // returns unique id assigned
    int removeEmployee(int id); // returns success code
    int addCustomer(std::string fullName, 
        Date dob, 
        std::string gender, 
        std::string mobileNumber,
        std::string email, 
        std::string address, 
        std::string aadhaarNumber, 
        std::string PANNumber);
    int removeCustomer(int id);
    int addBranch(std::string name, std::string code, std::string address);
    int removeBranch(int id);


    int getId();

    int addLoan(std::string loanType,
    double loanAmount,
    double interestRate,
    int tenureYears,
    double EMIAmount,
    Customer *customer,
    std::string loanStatus);
    int removeLoan(int id);

    int addTransaction(std::string transactionType, double amount, Date transactionDate, Account *senderAccount, Account *receiverAccount, std::string status);
    
    std::string getName();
};

#endif