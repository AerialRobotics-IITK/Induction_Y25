#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "Branch.h"
#include "Customer.h"
#include "Employee.h"
#include "Account.h"
#include "Transaction.h"
#include "Loan.h"
#include "ATMCard.h"
#include "AccountFactory.h"
#include "NotificationService.h"

class Bank {
    int bankId_;
    std::string bankName_;
    std::vector<Branch*> branches_;
    std::vector<Customer*> customers_;
    std::vector<Employee*> employees_;
    std::vector<Account*> accounts_;
    std::vector<Transaction*> transactions_;
    std::vector<Loan*> loans_;
    std::vector<ATMCard*> atmCards_;
    NotificationService notificationService_;

    static int nextBankId_;
public:
    explicit Bank(const std::string& name);
    ~Bank();

    int getBankId() const { return bankId_; }
    std::string getBankName() const { return bankName_; }
    NotificationService& getNotificationService() { return notificationService_; }

    Branch* addBranch(const std::string& name, const std::string& ifsc,
                      const std::string& address);
    Customer* addCustomer(const std::string& fullName, const std::string& dob,
                          const std::string& gender, const std::string& mobile,
                          const std::string& email, const std::string& address,
                          const std::string& aadhaar, const std::string& pan);
    Employee* addEmployee(const std::string& name, const std::string& designation,
                          double salary, Branch* branch = nullptr);

    Account* openAccount(const std::string& type, double initialBalance,
                         const std::string& dateOpened, Branch* branch, Customer* customer);

    Transaction* performDeposit(Account* account, double amount, const std::string& date);
    Transaction* performWithdrawal(Account* account, double amount, const std::string& date);
    Transaction* performTransfer(Account* sender, Account* receiver,
                                 double amount, const std::string& date);

    Loan* applyForLoan(const std::string& type, double amount, double interestRate,
                       int tenureYears, Customer* customer);
    ATMCard* issueATMCard(const std::string& expiryDate, int pin,
                          const std::string& cardType, Account* linkedAccount);

    void displayBankInfo() const;
};
