#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "account.h"
#include "loan.h"
#include "atm_card.h"
#include "notification.h"

class Branch;

// ─── Customer ─────────────────────────────────────────────────────────────────
class Customer {
private:
    static int idCounter;
public:
    int customerId;
    std::string fullName;
    std::string dob;
    std::string gender;
    std::string mobileNumber;
    std::string email;
    std::string address;
    std::string aadhaarNumber;
    std::string PANNumber;
    std::vector<Account*>  accounts;
    std::vector<Loan*>     loans;
    std::vector<ATMCard*>  cards;

    Customer(const std::string& name,
             const std::string& dob_,
             const std::string& gender_,
             const std::string& mobile,
             const std::string& email_,
             const std::string& addr,
             const std::string& aadhaar,
             const std::string& pan)
        : customerId(++idCounter),
          fullName(name), dob(dob_), gender(gender_),
          mobileNumber(mobile), email(email_),
          address(addr), aadhaarNumber(aadhaar), PANNumber(pan) {}

    ~Customer() {
        for (auto a : accounts) delete a;
        for (auto l : loans)    delete l;
        for (auto c : cards)    delete c;
    }

    void addAccount(Account* acc) { accounts.push_back(acc); }
    void addLoan(Loan* loan)      { loans.push_back(loan);   }
    void addCard(ATMCard* card)   { cards.push_back(card);   }

    void notify(Notification* n) { n->sendNotification(); }

    void display() const {
        std::cout << "  Customer #" << customerId
                  << " | " << fullName
                  << " | Mobile: " << mobileNumber
                  << " | Email: " << email << "\n";
    }

    void showAccounts() const {
        std::cout << "  Accounts for " << fullName << ":\n";
        for (auto* a : accounts) a->displayInfo();
    }

    void showLoans() const {
        std::cout << "  Loans for " << fullName << ":\n";
        for (auto* l : loans) l->display();
    }
};

int Customer::idCounter = 0;


// ─── Employee ─────────────────────────────────────────────────────────────────
class Employee {
private:
    static int idCounter;
public:
    int employeeId;
    std::string employeeName;
    std::string designation;   // "Manager" or "Cashier"
    double salary;
    Branch* branch;

    Employee(const std::string& name,
             const std::string& desig,
             double sal,
             Branch* br)
        : employeeId(++idCounter),
          employeeName(name),
          designation(desig),
          salary(sal),
          branch(br) {}

    void display() const {
        std::cout << std::fixed << std::setprecision(2)
                  << "  Employee #" << employeeId
                  << " | " << employeeName
                  << " | " << designation
                  << " | Salary: Rs." << salary << "\n";
    }
};

int Employee::idCounter = 0;