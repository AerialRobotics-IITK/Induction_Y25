#include "Branch.h"
#include "Account.h"
#include "Employee.h"

int Branch::nextBranchId_ = 1;

Branch::Branch(const std::string& name, const std::string& ifsc, const std::string& address)
    : branchId_(nextBranchId_++),
      branchName_(name),
      ifscCode_(ifsc),
      address_(address) {}

void Branch::addEmployee(Employee* employee) {
    employees_.push_back(employee);
    employee->setBranch(this);
}

void Branch::displayBranchInfo() const {
    std::cout << "--- Branch ---\n"
              << "  ID       : " << branchId_ << "\n"
              << "  Name     : " << branchName_ << "\n"
              << "  IFSC     : " << ifscCode_ << "\n"
              << "  Address  : " << address_ << "\n"
              << "  Accounts : " << accounts_.size() << "\n"
              << "  Staff    : " << employees_.size() << "\n";
}
