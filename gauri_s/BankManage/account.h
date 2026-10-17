#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include "exceptions.h"
#include "transaction.h"

class Branch;
class Customer;

// -- Base class Account ------------------
class Account {
private:
    static long numCounter;
public:
    long accountNumber;
    std::string accountType;
    double balance;
    std::time_t dateOpened;
    std::string status;      // "Active" or "Blocked"
    Branch* branch;
    Customer* customer;
    std::vector<Transaction*> transactions;

    Account(const std::string& type,
            double             initialBalance,
            Branch*            br,
            Customer*          cust)
        : accountNumber(1000000000L + (++numCounter)),
          accountType(type),
          balance(initialBalance),
          dateOpened(std::time(nullptr)),
          status("Active"),
          branch(br),
          customer(cust) {}

    virtual ~Account() {
        for (auto t : transactions) delete t;
    }

    // -- Exception ----------------------------------
    void checkActive() const {
        if (status == "Blocked")
            throw AccountBlockedException("Account " + std::to_string(accountNumber) + " is blocked.");
    }

    // -- Core operations ---------------------------
    virtual void deposit(double amount) {
        checkActive();
        if (amount <= 0) throw std::invalid_argument("Deposit amount must be positive.");
        balance += amount;
        auto* t = new Transaction("Deposit", amount, nullptr, this);
        transactions.push_back(t);
        std::cout << std::fixed << std::setprecision(2)
                  << "  Deposited Rs." << amount
                  << "  Balance: Rs." << balance << "\n";
    }

    virtual void withdraw(double amount) {
        checkActive();
        if (amount <= 0) throw std::invalid_argument("Withdrawal amount must be positive.");
        if (amount > balance) throw InsufficientBalanceException();
        balance -= amount;
        auto* t = new Transaction("Withdrawal", amount, this, nullptr);
        transactions.push_back(t);
        std::cout << std::fixed << std::setprecision(2)
                  << "  Withdrew Rs." << amount
                  << "  Balance: Rs." << balance << "\n";
    }

    void transfer(Account* target, double amount) {
        checkActive();
        this->withdraw(amount);
        target->deposit(amount);
        auto* t = new Transaction("Transfer", amount, this, target);
        transactions.push_back(t);
    }

    void blockAccount()  { status = "Blocked"; }
    void activateAccount() { status = "Active"; }

    virtual void displayInfo() const {
        std::cout << std::fixed << std::setprecision(2)
                  << "  Account #" << accountNumber
                  << " | Type: "   << accountType
                  << " | Balance: Rs." << balance
                  << " | Status: " << status << "\n";
    }

    void printStatement() const {
        std::cout << "  -- Statement for Account #" << accountNumber << " --\n";
        if (transactions.empty()) {
            std::cout << "  No transactions yet.\n";
        } else {
            for (auto* t : transactions) t->display();
        }
    }

    // Pure virtual: subclasses must implement interest application
    virtual void applyInterest() = 0;
};

long Account::numCounter = 0;


// -- SavingsAccount ---------------------------------------------
class SavingsAccount : public Account {
public:
    double interestRate;      
    double minimumBalance;
    double dailyWithdrawalLimit;

    SavingsAccount(double initialBalance,
                   Branch* br, Customer* cust,
                   double rate = 4.0,
                   double minBal = 1000.0,
                   double dailyLimit = 25000.0)
        : Account("Savings", initialBalance, br, cust),
          interestRate(rate),
          minimumBalance(minBal),
          dailyWithdrawalLimit(dailyLimit) {}

    void withdraw(double amount) override {
        checkActive();
        if (amount > dailyWithdrawalLimit)
            throw std::runtime_error("Exceeds daily withdrawal limit of Rs."
                + std::to_string(static_cast<long long>(dailyWithdrawalLimit)) + ".");
        if ((balance - amount) < minimumBalance)
            throw InsufficientBalanceException(
                "Withdrawal would breach minimum balance of Rs."
                + std::to_string((int)minimumBalance) + ".");
        Account::withdraw(amount);
    }

    void applyInterest() override {
        double interest = balance * (interestRate / 100.0) / 12.0;
        balance += interest;
        std::cout << std::fixed << std::setprecision(2)
                  << "  [Savings] Monthly interest Rs." << interest << " applied. New balance: Rs." << balance << "\n";
    }

    void displayInfo() const override {
        Account::displayInfo();
        std::cout << std::fixed << std::setprecision(2)
                  << "    Interest Rate: " << interestRate
                  << "% | Min Balance: Rs." << minimumBalance
                  << " | Daily Limit: Rs." << dailyWithdrawalLimit << "\n";
    }
};


// -- CurrentAccount ----------------------------------------
class CurrentAccount : public Account {
public:
    double overdraftLimit;
    std::string businessName;

    CurrentAccount(double initialBalance,
                   Branch* br, Customer* cust,
                   const std::string& business = "N/A",
                   double odLimit = 50000.0)
        : Account("Current", initialBalance, br, cust),
          overdraftLimit(odLimit),
          businessName(business) {}

    void withdraw(double amount) override {
        checkActive();
        if (amount <= 0) throw std::invalid_argument("Amount must be positive.");
        if ((balance - amount) < -overdraftLimit)
            throw InsufficientBalanceException(
                "Exceeds overdraft limit of Rs."
                + std::to_string((int)overdraftLimit) + ".");
        balance -= amount;
        auto* t = new Transaction("Withdrawal", amount, this, nullptr);
        transactions.push_back(t);
        std::cout << std::fixed << std::setprecision(2)
                  << "  Withdrew Rs." << amount
                  << " → Balance: Rs." << balance << "\n";
    }

    void applyInterest() override {
        // Current accounts typically don't earn interest; charge maintenance fee
        double fee = 500.0;
        balance -= fee;
        std::cout << std::fixed << std::setprecision(2)
                  << "  [Current] Monthly maintenance fee Rs." << fee
                  << " deducted. Balance: Rs." << balance << "\n";
    }

    void displayInfo() const override {
        Account::displayInfo();
        std::cout << std::fixed << std::setprecision(2)
                  << "    Business: " << businessName
                  << " | Overdraft Limit: Rs." << overdraftLimit << "\n";
    }
};


// -- FixedDepositAccount -----------------------------------------------
class FixedDepositAccount : public Account {
public:
    double FDAmount;
    std::time_t maturityDate;
    double FDInterestRate;   // annual %
    int tenureMonths;

    FixedDepositAccount(double fdAmount,
                        Branch* br, Customer* cust,
                        int tenure = 12,
                        double rate = 7.0)
        : Account("Fixed Deposit", fdAmount, br, cust),
          FDAmount(fdAmount),
          FDInterestRate(rate),
          tenureMonths(tenure) {
        maturityDate = std::time(nullptr) + (long long)tenure * 30 * 24 * 3600;
    }

    // FD cannot be withdrawn mid-tenure
    void withdraw(double /*amount*/) override {
        throw std::runtime_error("Premature withdrawal not allowed for Fixed Deposit.");
    }

    double maturityAmount() const {
        return FDAmount * std::pow(1.0 + FDInterestRate / 100.0,
                                   tenureMonths / 12.0);
    }

    void applyInterest() override {
        double monthly = FDAmount * (FDInterestRate / 100.0) / 12.0;
        balance += monthly;
        std::cout << std::fixed << std::setprecision(2)
                  << "  [FD] Monthly interest Rs." << monthly
                  << " accrued. Balance: Rs." << balance << "\n";
    }

    void displayInfo() const override {
        Account::displayInfo();
        std::cout << std::fixed << std::setprecision(2)
                  << "    FD Amount: Rs." << FDAmount
                  << " | Rate: " << FDInterestRate
                  << "% | Tenure: " << tenureMonths << " months"
                  << " | Maturity Value: Rs." << maturityAmount() << "\n";
    }
};


// -- Account Factory ---------------------------------------------
class AccountFactory {
public:
    static Account* createAccount(const std::string& type,
                                  double initialBalance,
                                  Branch* br,
                                  Customer* cust,
                                  const std::string& extra = "") {
        if (type == "Savings")
            return new SavingsAccount(initialBalance, br, cust);
        if (type == "Current")
            return new CurrentAccount(initialBalance, br, cust, extra);
        if (type == "FixedDeposit")
            return new FixedDepositAccount(initialBalance, br, cust);
        throw std::invalid_argument("Unknown account type: " + type);
    }
};
