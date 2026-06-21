#include "SavingsAccount.h"
#include "Exceptions.h"
#include <sstream>
#include <iomanip>

static std::string fmtRs(double val) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << val;
    return oss.str();
}

SavingsAccount::SavingsAccount(double initialBalance, const std::string& dateOpened,
                               Branch* branch, Customer* customer,
                               double interestRate, double minimumBalance)
    : Account("Savings", initialBalance, dateOpened, branch, customer),
      interestRate_(interestRate),
      minimumBalance_(minimumBalance) {}

void SavingsAccount::deposit(double amount) {
    if (amount <= 0) throw std::invalid_argument("Deposit amount must be positive");
    balance_ += amount;
    std::cout << "  Deposited Rs." << amount << " into Savings A/C " << accountNumber_
              << ". New balance: Rs." << balance_ << std::endl;
}

void SavingsAccount::withdraw(double amount) {
    if (status_ != "Active")
        throw AccountBlockedException("Savings A/C " + std::to_string(accountNumber_) + " is blocked");
    if (amount <= 0) throw std::invalid_argument("Withdrawal amount must be positive");
    if (balance_ - amount < minimumBalance_)
        throw InsufficientBalanceException(
            "Withdrawal of Rs." + fmtRs(amount) +
            " denied: balance would fall below minimum (Rs." +
            fmtRs(minimumBalance_) + ")");
    balance_ -= amount;
    std::cout << "  Withdrew Rs." << amount << " from Savings A/C " << accountNumber_
              << ". New balance: Rs." << balance_ << std::endl;
}

double SavingsAccount::calculateInterest() const {
    return balance_ * interestRate_ / 100.0;
}

void SavingsAccount::displayAccountInfo() const {
    std::cout << "--- Savings Account ---\n"
              << "  Account Number : " << accountNumber_ << "\n"
              << "  Balance        : Rs." << balance_ << "\n"
              << "  Interest Rate  : " << interestRate_ << "%\n"
              << "  Min Balance    : Rs." << minimumBalance_ << "\n"
              << "  Status         : " << status_ << "\n"
              << "  Date Opened    : " << dateOpened_ << "\n";
}
