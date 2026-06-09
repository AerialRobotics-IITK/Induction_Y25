#include "Loan.h"
#include "Exceptions.h"
#include <iomanip>

int Loan::nextLoanId_ = 1;

double Loan::computeEMI() const {
    double monthlyRate = interestRate_ / (12.0 * 100.0);
    int months = tenureYears_ * 12;
    if (monthlyRate == 0.0) return loanAmount_ / months;
    double factor = std::pow(1.0 + monthlyRate, months);
    return loanAmount_ * monthlyRate * factor / (factor - 1.0);
}

Loan::Loan(const std::string& loanType, double loanAmount, double interestRate,
           int tenureYears, Customer* customer)
    : loanId_(nextLoanId_++),
      loanType_(loanType),
      loanAmount_(loanAmount),
      interestRate_(interestRate),
      tenureYears_(tenureYears),
      loanStatus_("Pending"),
      customer_(customer) {
    emiAmount_ = computeEMI();
}

void Loan::approve() {
    loanStatus_ = "Approved";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Loan#" << loanId_ << " (" << loanType_ << ") approved. EMI: Rs."
              << emiAmount_ << "/month\n";
}

void Loan::reject(const std::string& reason) {
    loanStatus_ = "Rejected";
    throw LoanRejectedException("Loan#" + std::to_string(loanId_) + " rejected" +
                                (reason.empty() ? "" : ": " + reason));
}

void Loan::displayLoanInfo() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "--- Loan Details ---\n"
              << "  Loan ID       : " << loanId_ << "\n"
              << "  Type          : " << loanType_ << "\n"
              << "  Amount        : Rs." << loanAmount_ << "\n"
              << "  Interest Rate : " << interestRate_ << "%\n"
              << "  Tenure        : " << tenureYears_ << " years\n"
              << "  EMI           : Rs." << emiAmount_ << "/month\n"
              << "  Status        : " << loanStatus_ << "\n";
}
