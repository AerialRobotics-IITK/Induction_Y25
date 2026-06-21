#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include "Bank.h"
#include "Exceptions.h"
#include "SMSNotification.h"
#include "EmailNotification.h"

static std::string fmtRs(double val) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << val;
    return oss.str();
}

static void divider(const std::string& title) {
    std::cout << "\n╔══════════════════════════════════════════════════╗\n"
              << "║  " << title;
    for (size_t i = title.size(); i < 47; ++i) std::cout << ' ';
    std::cout << "║\n"
              << "╚══════════════════════════════════════════════════╝\n";
}

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "SmartBank Enterprise System — Demo\n";

    // ── 1. Create the Bank ──────────────────────────────────────
    divider("1. Bank Setup");
    Bank bank("SmartBank India");
    bank.displayBankInfo();

    // ── 2. Add Branches ─────────────────────────────────────────
    divider("2. Creating Branches");
    Branch* mumbai  = bank.addBranch("Mumbai Central", "SBIN0001234", "101 MG Road, Mumbai");
    Branch* delhi   = bank.addBranch("Delhi Connaught Place", "SBIN0005678", "45 CP, New Delhi");
    mumbai->displayBranchInfo();
    delhi->displayBranchInfo();

    // ── 3. Add Employees ────────────────────────────────────────
    divider("3. Adding Employees");
    Employee* mgr   = bank.addEmployee("Rajesh Kumar",  "Manager", 120000, mumbai);
    Employee* cash  = bank.addEmployee("Priya Sharma",  "Cashier",  45000, mumbai);
    Employee* mgr2  = bank.addEmployee("Amit Verma",    "Manager", 115000, delhi);
    Employee* cash2 = bank.addEmployee("Neha Gupta",    "Cashier",  42000, delhi);
    mgr->displayEmployeeInfo();
    cash->displayEmployeeInfo();
    mgr2->displayEmployeeInfo();
    cash2->displayEmployeeInfo();

    // ── 4. Register Customers ───────────────────────────────────
    divider("4. Registering Customers");
    Customer* alice = bank.addCustomer(
        "Alice Mehta", "1995-03-15", "Female",
        "9876543210", "alice@email.com", "12 Marine Drive, Mumbai",
        "1234-5678-9012", "ABCDE1234F");
    Customer* bob = bank.addCustomer(
        "Bob Singh", "1990-07-22", "Male",
        "9123456780", "bob@email.com", "78 Janpath, Delhi",
        "9876-5432-1098", "FGHIJ5678K");
    alice->displayCustomerInfo();
    bob->displayCustomerInfo();

    // ── 5. Open Accounts (Factory Pattern) ──────────────────────
    divider("5. Opening Accounts (Factory Pattern)");
    std::cout << "Using AccountFactory::createAccount() to create accounts...\n\n";

    Account* aliceSavings = bank.openAccount("Savings",      50000, "2025-01-10", mumbai, alice);
    Account* aliceFD      = bank.openAccount("FixedDeposit", 200000, "2025-01-10", mumbai, alice);
    Account* bobCurrent   = bank.openAccount("Current",      100000, "2025-02-01", delhi,  bob);
    Account* bobSavings   = bank.openAccount("Savings",      30000, "2025-02-01", delhi,  bob);

    aliceSavings->displayAccountInfo();
    std::cout << "\n";
    aliceFD->displayAccountInfo();
    std::cout << "\n";
    bobCurrent->displayAccountInfo();
    std::cout << "\n";
    bobSavings->displayAccountInfo();

    // ── 6. Transactions ─────────────────────────────────────────
    divider("6. Transactions");

    std::cout << "\n>> Deposit Rs.10000 into Alice's Savings\n";
    Transaction* t1 = bank.performDeposit(aliceSavings, 10000, "2025-03-01");
    t1->displayTransaction();

    std::cout << "\n>> Withdraw Rs.5000 from Bob's Current\n";
    Transaction* t2 = bank.performWithdrawal(bobCurrent, 5000, "2025-03-02");
    t2->displayTransaction();

    std::cout << "\n>> Transfer Rs.15000 from Alice's Savings -> Bob's Savings\n";
    Transaction* t3 = bank.performTransfer(aliceSavings, bobSavings, 15000, "2025-03-05");
    t3->displayTransaction();

    // ── 7. Interest Calculation ─────────────────────────────────
    divider("7. Interest Calculation");
    std::cout << "  Alice Savings interest : Rs." << aliceSavings->calculateInterest() << "\n";
    std::cout << "  Alice FD interest      : Rs." << aliceFD->calculateInterest() << "\n";
    std::cout << "  Bob Current interest   : Rs." << bobCurrent->calculateInterest() << "\n";
    std::cout << "  Bob Savings interest   : Rs." << bobSavings->calculateInterest() << "\n";

    // ── 8. Loan Management ──────────────────────────────────────
    divider("8. Loan Management");
    Loan* homeLoan = bank.applyForLoan("Home", 5000000, 8.5, 20, alice);
    homeLoan->displayLoanInfo();
    std::cout << "\n  Approving home loan...\n";
    homeLoan->approve();
    homeLoan->displayLoanInfo();

    Loan* carLoan = bank.applyForLoan("Car", 800000, 9.0, 5, bob);
    carLoan->displayLoanInfo();
    carLoan->approve();

    Loan* personalLoan = bank.applyForLoan("Personal", 10000000, 12.0, 3, bob);
    personalLoan->displayLoanInfo();
    std::cout << "\n  Attempting to reject personal loan...\n";
    try {
        personalLoan->reject("Amount exceeds eligibility");
    } catch (const LoanRejectedException& e) {
        std::cout << "  CAUGHT: " << e.what() << "\n";
    }

    // ── 9. ATM Card Services ────────────────────────────────────
    divider("9. ATM Card Services");
    ATMCard* aliceCard = bank.issueATMCard("2028-01-01", 1234, "Debit", aliceSavings);
    ATMCard* bobCard   = bank.issueATMCard("2028-06-01", 5678, "Debit", bobCurrent);
    aliceCard->displayCardInfo();
    bobCard->displayCardInfo();

    std::cout << "\n  Validating Alice's PIN (correct)...\n";
    try {
        aliceCard->validatePIN(1234);
        std::cout << "  PIN validated successfully.\n";
    } catch (const InvalidPINException& e) {
        std::cout << "  CAUGHT: " << e.what() << "\n";
    }

    std::cout << "\n  Validating Alice's PIN (wrong)...\n";
    try {
        aliceCard->validatePIN(9999);
        std::cout << "  PIN validated successfully.\n";
    } catch (const InvalidPINException& e) {
        std::cout << "  CAUGHT: " << e.what() << "\n";
    }

    std::cout << "\n  Blocking Bob's card...\n";
    bobCard->blockCard();

    std::cout << "\n  Trying to use blocked card...\n";
    try {
        bobCard->validatePIN(5678);
    } catch (const AccountBlockedException& e) {
        std::cout << "  CAUGHT: " << e.what() << "\n";
    }

    std::cout << "\n  Re-activating Bob's card...\n";
    bobCard->activateCard();

    std::cout << "\n  Changing Alice's PIN...\n";
    aliceCard->changePIN(1234, 4321);

    // ── 10. Exception Handling Showcase ─────────────────────────
    divider("10. Exception Handling");

    std::cout << "\n>> InsufficientBalanceException:\n";
    try {
        bank.performWithdrawal(aliceSavings, 999999, "2025-04-01");
    } catch (const InsufficientBalanceException& e) {
        std::cout << "  CAUGHT: " << e.what() << "\n";
    }

    std::cout << "\n>> AccountBlockedException:\n";
    try {
        aliceSavings->setStatus("Blocked");
        bank.performWithdrawal(aliceSavings, 100, "2025-04-01");
    } catch (const AccountBlockedException& e) {
        std::cout << "  CAUGHT: " << e.what() << "\n";
        aliceSavings->setStatus("Active");
    }

    std::cout << "\n>> InvalidPINException:\n";
    try {
        aliceCard->validatePIN(0000);
    } catch (const InvalidPINException& e) {
        std::cout << "  CAUGHT: " << e.what() << "\n";
    }

    std::cout << "\n>> LoanRejectedException:\n";
    try {
        Loan* badLoan = bank.applyForLoan("Personal", 50000000, 15.0, 1, bob);
        badLoan->reject("Credit score too low");
    } catch (const LoanRejectedException& e) {
        std::cout << "  CAUGHT: " << e.what() << "\n";
    }

    // ── 11. Notification System (Strategy Pattern) ──────────────
    divider("11. Notifications (Strategy Pattern)");

    std::cout << "\n  Setting strategy to SMS...\n";
    bank.getNotificationService().setStrategy(
        std::make_unique<SMSNotification>("9876543210", ""));
    bank.getNotificationService().notify(
        "Dear Alice, Rs.10000 deposited into A/C " +
        std::to_string(aliceSavings->getAccountNumber()));

    std::cout << "\n  Switching strategy to Email...\n";
    bank.getNotificationService().setStrategy(
        std::make_unique<EmailNotification>("alice@email.com", "Transaction Alert", ""));
    bank.getNotificationService().notify(
        "Dear Alice, Rs.15000 transferred from your A/C " +
        std::to_string(aliceSavings->getAccountNumber()));

    std::cout << "\n  Sending SMS to Bob...\n";
    bank.getNotificationService().setStrategy(
        std::make_unique<SMSNotification>("9123456780", ""));
    bank.getNotificationService().notify(
        "Dear Bob, your Car Loan of Rs.800000 has been approved. EMI: Rs." +
        fmtRs(carLoan->getEMIAmount()));

    // ── 12. Transaction History ─────────────────────────────────
    divider("12. Transaction History");

    std::cout << "\nAlice's Savings Account transactions:\n";
    for (const auto* txn : aliceSavings->getTransactions())
        txn->displayTransaction();

    std::cout << "\nBob's Current Account transactions:\n";
    for (const auto* txn : bobCurrent->getTransactions())
        txn->displayTransaction();

    std::cout << "\nBob's Savings Account transactions:\n";
    for (const auto* txn : bobSavings->getTransactions())
        txn->displayTransaction();

    // ── 13. Final Summary ───────────────────────────────────────
    divider("13. Final Bank Summary");
    bank.displayBankInfo();

    std::cout << "\nBranch details:\n";
    mumbai->displayBranchInfo();
    delhi->displayBranchInfo();

    std::cout << "\nCustomer details:\n";
    alice->displayCustomerInfo();
    bob->displayCustomerInfo();

    std::cout << "\n✓ SmartBank Enterprise System demo complete.\n";
    return 0;
}
