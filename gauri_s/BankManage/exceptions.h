#pragma once
#include <stdexcept>
#include <string>

class InsufficientBalanceException : public std::runtime_error {
public:
    explicit InsufficientBalanceException(const std::string& msg = "Insufficient balance.")
        : std::runtime_error(msg) {}
};

class InvalidPINException : public std::runtime_error {
public:
    explicit InvalidPINException(const std::string& msg = "Invalid PIN entered.")
        : std::runtime_error(msg) {}
};

class AccountBlockedException : public std::runtime_error {
public:
    explicit AccountBlockedException(const std::string& msg = "Account is blocked.")
        : std::runtime_error(msg) {}
};

class LoanRejectedException : public std::runtime_error {
public:
    explicit LoanRejectedException(const std::string& msg = "Loan application rejected.")
        : std::runtime_error(msg) {}
};
