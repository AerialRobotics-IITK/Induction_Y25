#pragma once
#include <string>
#include <vector>
#include <iostream>

class Account;
class Employee;

class Branch {
    int branchId_;
    std::string branchName_;
    std::string ifscCode_;
    std::string address_;
    std::vector<Account*> accounts_;
    std::vector<Employee*> employees_;

    static int nextBranchId_;
public:
    Branch(const std::string& name, const std::string& ifsc, const std::string& address);

    int getBranchId() const { return branchId_; }
    std::string getBranchName() const { return branchName_; }
    std::string getIFSCCode() const { return ifscCode_; }
    std::string getAddress() const { return address_; }
    const std::vector<Account*>& getAccounts() const { return accounts_; }
    const std::vector<Employee*>& getEmployees() const { return employees_; }

    void addAccount(Account* account) { accounts_.push_back(account); }
    void addEmployee(Employee* employee);
    void displayBranchInfo() const;
};
