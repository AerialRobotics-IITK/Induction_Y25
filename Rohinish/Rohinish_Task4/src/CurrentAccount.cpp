#include "CurrentAccount.h"
#include "Exceptions.h"
#include <sstream>
#include <iomanip>

static std::string fmtRs(double val) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << val;
    return oss.str();
}

CurrentAccount::CurrentAccount(double initialBalance, const std::string& dateOpened,
                               Branch* branch, Customer* customer,
                               double overdraftLimit, const std::string& businessName)
    : Account("Current", initialBalance, dateOpened, branch, customer),
      overdraftLimit_(overdraftLimit),
      businessName_(businessName) {}

void CurrentAccount::deposit(double amount) {
    if (amount <= 0) throw std::invalid_argument("Deposit amount must be positive");
    balance_ += amount;
    std::cout << "  Deposited Rs." << amount << " into Current A/C " << accountNumber_
              << ". New balance: Rs." << balance_ << std::endl;
}

void CurrentAccount::withdraw(double amount) {
    if (status_ != "Active")
        throw AccountBlockedException("Current A/C " + std::to_string(accountNumber_) + " is blocked");
    if (amount <= 0) throw std::invalid_argument("Withdrawal amount must be positive");
    if (balance_ + overdraftLimit_ < amount)
        throw InsufficientBalanceException(
            "Withdrawal of Rs." + fmtRs(amount) +
            " denied: exceeds balance + overdraft limit (Rs." +
            fmtRs(balance_ + overdraftLimit_) + ")");
    balance_ -= amount;
    std::cout << "  Withdrew Rs." << amount << " from Current A/C " << accountNumber_
              << ". New balance: Rs." << balance_ << std::endl;
}

double CurrentAccount::calculateInterest() const {
    return 0.0;
}

void CurrentAccount::displayAccountInfo() const {
    std::cout << "--- Current Account ---\n"
              << "  Account Number  : " << accountNumber_ << "\n"
              << "  Balance         : Rs." << balance_ << "\n"
              << "  Overdraft Limit : Rs." << overdraftLimit_ << "\n"
              << "  Business Name   : " << (businessName_.empty() ? "N/A" : businessName_) << "\n"
              << "  Status          : " << status_ << "\n"
              << "  Date Opened     : " << dateOpened_ << "\n";
}
