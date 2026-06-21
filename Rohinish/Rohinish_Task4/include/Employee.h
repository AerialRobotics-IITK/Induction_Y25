#pragma once
#include <string>
#include <iostream>

class Branch;

class Employee {
    int employeeId_;
    std::string employeeName_;
    std::string designation_;
    double salary_;
    Branch* branch_;

    static int nextEmployeeId_;
public:
    Employee(const std::string& name, const std::string& designation,
             double salary, Branch* branch = nullptr);

    int getEmployeeId() const { return employeeId_; }
    std::string getEmployeeName() const { return employeeName_; }
    std::string getDesignation() const { return designation_; }
    double getSalary() const { return salary_; }
    Branch* getBranch() const { return branch_; }

    void setBranch(Branch* branch) { branch_ = branch; }
    void displayEmployeeInfo() const;
};
