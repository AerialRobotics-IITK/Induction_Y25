#include "Customer.h"
#include "Account.h"
#include "Loan.h"

int Customer::nextCustomerId_ = 1;

Customer::Customer(const std::string& fullName, const std::string& dob,
                   const std::string& gender, const std::string& mobile,
                   const std::string& email, const std::string& address,
                   const std::string& aadhaar, const std::string& pan)
    : customerId_(nextCustomerId_++),
      fullName_(fullName), dob_(dob), gender_(gender),
      mobileNumber_(mobile), email_(email), address_(address),
      aadhaarNumber_(aadhaar), panNumber_(pan) {}

void Customer::displayCustomerInfo() const {
    std::cout << "--- Customer ---\n"
              << "  ID       : " << customerId_ << "\n"
              << "  Name     : " << fullName_ << "\n"
              << "  DOB      : " << dob_ << "\n"
              << "  Gender   : " << gender_ << "\n"
              << "  Mobile   : " << mobileNumber_ << "\n"
              << "  Email    : " << email_ << "\n"
              << "  Address  : " << address_ << "\n"
              << "  Aadhaar  : " << aadhaarNumber_ << "\n"
              << "  PAN      : " << panNumber_ << "\n"
              << "  Accounts : " << accounts_.size() << "\n"
              << "  Loans    : " << loans_.size() << "\n";
}
