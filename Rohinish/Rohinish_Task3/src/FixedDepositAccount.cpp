#include "FixedDepositAccount.h"
#include "Exceptions.h"
#include <cmath>
#include <sstream>

static std::string computeMaturityDate(const std::string& openDate, int tenureMonths) {
    int year, month, day;
    char dash;
    std::istringstream iss(openDate);
    iss >> year >> dash >> month >> dash >> day;
    month += tenureMonths;
    year += (month - 1) / 12;
    month = (month - 1) % 12 + 1;
    std::ostringstream oss;
    oss << year << "-"
        << (month < 10 ? "0" : "") << month << "-"
        << (day < 10 ? "0" : "") << day;
    return oss.str();
}

FixedDepositAccount::FixedDepositAccount(double fdAmount, const std::string& dateOpened,
                                         Branch* branch, Customer* customer,
                                         double fdInterestRate, int tenureMonths)
    : Account("FixedDeposit", fdAmount, dateOpened, branch, customer),
      fdAmount_(fdAmount),
      fdInterestRate_(fdInterestRate),
      tenureMonths_(tenureMonths) {
    maturityDate_ = computeMaturityDate(dateOpened, tenureMonths);
}

void FixedDepositAccount::deposit(double /*amount*/) {
    std::cout << "  Fixed Deposit A/C " << accountNumber_
              << ": additional deposits are not allowed.\n";
}

void FixedDepositAccount::withdraw(double amount) {
    if (status_ != "Active")
        throw AccountBlockedException("FD A/C " + std::to_string(accountNumber_) + " is blocked");
    if (amount > balance_)
        throw InsufficientBalanceException("Cannot withdraw more than FD balance");
    double penalty = amount * 0.01;
    balance_ -= (amount + penalty);
    std::cout << "  Premature withdrawal of Rs." << amount << " from FD A/C " << accountNumber_
              << " (penalty: Rs." << penalty << "). Remaining: Rs." << balance_ << std::endl;
}

double FixedDepositAccount::calculateInterest() const {
    return fdAmount_ * fdInterestRate_ * tenureMonths_ / (12.0 * 100.0);
}

double FixedDepositAccount::getMaturityAmount() const {
    return fdAmount_ + calculateInterest();
}

void FixedDepositAccount::displayAccountInfo() const {
    std::cout << "--- Fixed Deposit Account ---\n"
              << "  Account Number  : " << accountNumber_ << "\n"
              << "  FD Amount       : Rs." << fdAmount_ << "\n"
              << "  Balance         : Rs." << balance_ << "\n"
              << "  Interest Rate   : " << fdInterestRate_ << "%\n"
              << "  Tenure          : " << tenureMonths_ << " months\n"
              << "  Maturity Date   : " << maturityDate_ << "\n"
              << "  Maturity Amount : Rs." << getMaturityAmount() << "\n"
              << "  Status          : " << status_ << "\n"
              << "  Date Opened     : " << dateOpened_ << "\n";
}
