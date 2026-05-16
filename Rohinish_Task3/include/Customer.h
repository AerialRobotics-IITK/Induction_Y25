#pragma once
#include <string>
#include <vector>
#include <iostream>

class Account;
class Loan;

class Customer {
    int customerId_;
    std::string fullName_;
    std::string dob_;
    std::string gender_;
    std::string mobileNumber_;
    std::string email_;
    std::string address_;
    std::string aadhaarNumber_;
    std::string panNumber_;
    std::vector<Account*> accounts_;
    std::vector<Loan*> loans_;

    static int nextCustomerId_;
public:
    Customer(const std::string& fullName, const std::string& dob,
             const std::string& gender, const std::string& mobile,
             const std::string& email, const std::string& address,
             const std::string& aadhaar, const std::string& pan);

    int getCustomerId() const { return customerId_; }
    std::string getFullName() const { return fullName_; }
    std::string getDob() const { return dob_; }
    std::string getGender() const { return gender_; }
    std::string getMobileNumber() const { return mobileNumber_; }
    std::string getEmail() const { return email_; }
    std::string getAddress() const { return address_; }
    std::string getAadhaarNumber() const { return aadhaarNumber_; }
    std::string getPANNumber() const { return panNumber_; }
    const std::vector<Account*>& getAccounts() const { return accounts_; }
    const std::vector<Loan*>& getLoans() const { return loans_; }

    void addAccount(Account* account) { accounts_.push_back(account); }
    void addLoan(Loan* loan) { loans_.push_back(loan); }
    void displayCustomerInfo() const;
};
