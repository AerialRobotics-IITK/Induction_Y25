#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "people.h"

// ─── Branch ───────────────────────────────────────────────────────────────────
class Branch {
private:
    static int idCounter;
public:
    int branchId;
    std::string branchName;
    std::string IFSCCode;
    std::string address;
    std::vector<Account*>   accounts;
    std::vector<Employee*>  employees;

    Branch(const std::string& name,
           const std::string& ifsc,
           const std::string& addr)
        : branchId(++idCounter),
          branchName(name),
          IFSCCode(ifsc),
          address(addr) {}

    ~Branch() {
        // Accounts are owned by Customers; employees owned by bank
    }

    void addAccount(Account* acc)    { accounts.push_back(acc); }
    void addEmployee(Employee* emp)  { employees.push_back(emp); }

    void display() const {
        std::cout << "  Branch #" << branchId
                  << " | " << branchName
                  << " | IFSC: " << IFSCCode
                  << " | " << address << "\n";
    }

    void showEmployees() const {
        std::cout << "  Staff at " << branchName << ":\n";
        for (auto* e : employees) e->display();
    }

    void showAccounts() const {
        std::cout << "  Accounts at " << branchName << ":\n";
        for (auto* a : accounts) a->displayInfo();
    }
};

int Branch::idCounter = 0;


// ─── Bank ─────────────────────────────────────────────────────────────────────
class Bank {
private:
    static int idCounter;
public:
    int bankId;
    std::string bankName;
    std::vector<Branch*>   branches;
    std::vector<Customer*> customers;
    std::vector<Employee*> employees;

    explicit Bank(const std::string& name)
        : bankId(++idCounter), bankName(name) {}

    ~Bank() {
        for (auto b : branches)  delete b;
        for (auto c : customers) delete c;
        for (auto e : employees) delete e;
    }

    Branch* addBranch(const std::string& name,
                      const std::string& ifsc,
                      const std::string& addr) {
        auto* b = new Branch(name, ifsc, addr);
        branches.push_back(b);
        return b;
    }

    Customer* addCustomer(const std::string& fullName,
                          const std::string& dob,
                          const std::string& gender,
                          const std::string& mobile,
                          const std::string& email,
                          const std::string& address,
                          const std::string& aadhaar,
                          const std::string& pan) {
        auto* c = new Customer(fullName, dob, gender, mobile,
                               email, address, aadhaar, pan);
        customers.push_back(c);
        return c;
    }

    Employee* addEmployee(const std::string& name,
                          const std::string& desig,
                          double salary,
                          Branch* branch) {
        auto* e = new Employee(name, desig, salary, branch);
        employees.push_back(e);
        branch->addEmployee(e);
        return e;
    }

    // Open account via factory and link to customer + branch
    Account* openAccount(const std::string& type,
                         double initialDeposit,
                         Branch* branch,
                         Customer* customer,
                         const std::string& extra = "") {
        Account* acc = AccountFactory::createAccount(type, initialDeposit,
                                                     branch, customer, extra);
        customer->addAccount(acc);
        branch->addAccount(acc);
        return acc;
    }

    // Apply for a loan
    Loan* applyLoan(Customer* customer,
                    const std::string& type,
                    double amount,
                    double rate,
                    int tenureYears) {
        auto* loan = new Loan(type, amount, rate, tenureYears, customer);
        try {
            loan->approve();
            std::cout << "  Loan approved for " << customer->fullName << ".\n";
        } catch (LoanRejectedException& e) {
            std::cerr << "  Loan rejected: " << e.what() << "\n";
            loan->reject();
        }
        customer->addLoan(loan);
        return loan;
    }

    // Issue ATM card
    ATMCard* issueCard(Account* account, int pin, int cvv,
                       const std::string& type = "Debit") {
        auto* card = new ATMCard(pin, cvv, account, type);
        account->customer->addCard(card);
        return card;
    }

    void display() const {
        std::cout << "═══ Bank: " << bankName << " (ID: " << bankId << ") ═══\n";
        std::cout << "  Branches: " << branches.size()
                  << " | Customers: " << customers.size()
                  << " | Employees: " << employees.size() << "\n";
    }

    void showAllBranches() const {
        std::cout << "\n── Branches ──\n";
        for (auto* b : branches) b->display();
    }

    void showAllCustomers() const {
        std::cout << "\n── Customers ──\n";
        for (auto* c : customers) c->display();
    }
};

int Bank::idCounter = 0;
