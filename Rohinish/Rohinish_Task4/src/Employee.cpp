#include "Employee.h"

int Employee::nextEmployeeId_ = 1;

Employee::Employee(const std::string& name, const std::string& designation,
                   double salary, Branch* branch)
    : employeeId_(nextEmployeeId_++),
      employeeName_(name),
      designation_(designation),
      salary_(salary),
      branch_(branch) {}

void Employee::displayEmployeeInfo() const {
    std::cout << "  EMP#" << employeeId_
              << " | " << employeeName_
              << " | " << designation_
              << " | Salary: Rs." << salary_ << "\n";
}
