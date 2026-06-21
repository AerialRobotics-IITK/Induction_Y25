#pragma once
#include <exception>
#include <string>

class InsufficientBalanceException : public std::exception {
    std::string msg_;
public:
    explicit InsufficientBalanceException(const std::string& message = "Insufficient balance for this operation")
        : msg_(message) {}
    const char* what() const noexcept override { return msg_.c_str(); }
};

class InvalidPINException : public std::exception {
    std::string msg_;
public:
    explicit InvalidPINException(const std::string& message = "Invalid PIN entered")
        : msg_(message) {}
    const char* what() const noexcept override { return msg_.c_str(); }
};

class AccountBlockedException : public std::exception {
    std::string msg_;
public:
    explicit AccountBlockedException(const std::string& message = "Account is blocked")
        : msg_(message) {}
    const char* what() const noexcept override { return msg_.c_str(); }
};

class LoanRejectedException : public std::exception {
    std::string msg_;
public:
    explicit LoanRejectedException(const std::string& message = "Loan application rejected")
        : msg_(message) {}
    const char* what() const noexcept override { return msg_.c_str(); }
};
