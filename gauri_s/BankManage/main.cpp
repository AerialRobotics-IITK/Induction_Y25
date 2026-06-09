
#include <iostream>
#include <iomanip>
#include "bank.h"
#include "notification.h"


int main() {
    std::cout << std::fixed << std::setprecision(2);

    // ── 1. Setup Bank & Branches ─────────────────────────────────────────────
    std::cout << "║  1. Bank & Branch Setup ║\n";
    Bank myBank("National Savings Bank of India");
    myBank.display();

    Branch* mainBranch   = myBank.addBranch("Main Branch",   "NSBI0001001", "MG Road, Lucknow");
    Branch* noidaBranch  = myBank.addBranch("Noida Branch",  "NSBI0001002", "Sector 18, Noida");
    myBank.showAllBranches();

    // ── 2. Add Employees ─────────────────────────────────────────────────────
    std::cout << "║  2. Employee Registration ║\n";
    myBank.addEmployee("Rajan Mehta",   "Manager", 85000.0, mainBranch);
    myBank.addEmployee("Priya Sharma",  "Cashier",  45000.0, mainBranch);
    myBank.addEmployee("Anil Kumar",    "Manager", 80000.0, noidaBranch);
    mainBranch->showEmployees();

    // ── 3. Register Customers ─────────────────────────────────────────────────
        std::cout << "║  3. Customer Registration ║\n";

    Customer* alice = myBank.addCustomer(
        "Alice", "1995-04-12", "Female",
        "9876543210", "alice@email.com",
        "12 Gandhi Nagar, Lucknow",
        "1234-5678-9012", "ABCPV1234D");

    Customer* bob = myBank.addCustomer(
        "Bob", "1988-11-30", "Male",
        "9123456780", "bob@email.com",
        "45 Nehru Place, Noida",
        "9876-5432-1098", "XYZBS5678E");

    myBank.showAllCustomers();

    // -- 4. Open Accounts ---------------------------------
    std::cout << "║  4. Account Opening (Factory) ║\n";

    SavingsAccount* aliceSavings = dynamic_cast<SavingsAccount*>(
        myBank.openAccount("Savings", 10000.0, mainBranch, alice));

    CurrentAccount* bobCurrent = dynamic_cast<CurrentAccount*>(
        myBank.openAccount("Current", 50000.0, noidaBranch, bob, "Bob Enterprises"));

    FixedDepositAccount* aliceFD = dynamic_cast<FixedDepositAccount*>(
        myBank.openAccount("FixedDeposit", 100000.0, mainBranch, alice));

    std::cout << "\nAlice's accounts:\n";
    alice->showAccounts();
    std::cout << "\nBob's accounts:\n";
    bob->showAccounts();

    // -- 5. Transactions -----------------------------------
    std::cout << "║  5. Transactions ║\n";

    std::cout << "\n-- Alice deposits ₹5,000 --\n";
    aliceSavings->deposit(5000.0);

    std::cout << "\n-- Alice withdraws ₹2,000 --\n";
    aliceSavings->withdraw(2000.0);

    std::cout << "\n-- Alice transfers ₹3,000 to Bob --\n";
    aliceSavings->transfer(bobCurrent, 3000.0);

    std::cout << "\n-- Bob withdraws ₹60,000 (uses overdraft) --\n";
    bobCurrent->withdraw(60000.0);

    std::cout << "\nAlice's statement:\n";
    aliceSavings->printStatement();

    // -- 6. Interest Application ---------------------------------
        std::cout << "║  6. Interest / Fee Application ║\n";

    std::cout << "\n-- Monthly interest for Alice's Savings --\n";
    aliceSavings->applyInterest();
    std::cout << "\n-- Monthly fee for Bob's Current account --\n";
    bobCurrent->applyInterest();
    std::cout << "\n-- FD interest accrual for Alice --\n";
    aliceFD->applyInterest();
    aliceFD->displayInfo();

    // -- 7. Loan Management -------------------------------
        std::cout << "║  7. Loan Management ║\n";


    Loan* homeLoan    = myBank.applyLoan(alice, "Home",     3500000.0, 8.5, 20);
    Loan* carLoan     = myBank.applyLoan(bob,   "Car",       700000.0, 9.0,  5);
    Loan* bigLoan     = myBank.applyLoan(bob,   "Personal", 15000000.0, 12.0, 3); // will be rejected
    (void)homeLoan; (void)carLoan; (void)bigLoan;

    std::cout << "\nAlice's loans:\n";
    alice->showLoans();
    std::cout << "\nBob's loans:\n";
    bob->showLoans();

    // -- 8. ATM Card Management -------------------------------
        std::cout << "║  8. ATM Card Management ║\n";


    ATMCard* aliceCard = myBank.issueCard(aliceSavings, 1234, 456);
    ATMCard* bobCard   = myBank.issueCard(bobCurrent,   5678, 789);

    std::cout << "\nAlice's card:\n";
    aliceCard->display();

    // Correct PIN
    std::cout << "\n-- Alice verifies PIN (1234) --\n";
    try {
        aliceCard->verifyPIN(1234);
        std::cout << "  PIN verified successfully.\n";
    } catch (InvalidPINException& e) {
        std::cerr << "  Error: " << e.what() << "\n";
    }

    // Wrong PIN
    std::cout << "\n-- Alice enters wrong PIN (9999) --\n";
    try {
        aliceCard->verifyPIN(9999);
    } catch (InvalidPINException& e) {
        std::cerr << "  Caught: " << e.what() << "\n";
    }

    // Change PIN
    std::cout << "\n-- Alice changes PIN --\n";
    aliceCard->changePin(1234, 4321);

    // Block card
    std::cout << "\n-- Bob's card gets blocked --\n";
    bobCard->block();
    try {
        bobCard->verifyPIN(5678);
    } catch (AccountBlockedException& e) {
        std::cerr << "  Caught: " << e.what() << "\n";
    }

    // -- 9. Notifications (Strategy Pattern) -----------------------
        std::cout << "║  9. Notification System (Strategy) ║\n";


    SMSNotification   sms(alice->mobileNumber,
                          "Dear Alice, Rs. 5,000 credited to your account.");
    EmailNotification email(alice->email,
                            "Account Credit Alert",
                            "Dear Alice, your account has been credited with ₹5,000.");

    std::cout << "\n-- Sending SMS notification --\n";
    alice->notify(&sms);

    std::cout << "\n-- Sending Email notification --\n";
    alice->notify(&email);

    // -- 10. Exception Handling Demo --------------------------
    std::cout << "║  10. Exception Handling ║\n";

    std::cout << "\n-- Attempting to breach minimum balance --\n";
    try {
        aliceSavings->withdraw(100000.0);
    } catch (InsufficientBalanceException& e) {
        std::cerr << "  Caught InsufficientBalance: " << e.what() << "\n";
    } catch (std::runtime_error& e) {
        std::cerr << "  Caught RuntimeError: " << e.what() << "\n";
    }

    std::cout << "\n-- Attempting FD premature withdrawal --\n";
    try {
        aliceFD->withdraw(50000.0);
    } catch (std::runtime_error& e) {
        std::cerr << "  Caught: " << e.what() << "\n";
    }

    std::cout << "\n-- Blocking Alice's account and attempting deposit --\n";
    aliceSavings->blockAccount();
    try {
        aliceSavings->deposit(1000.0);
    } catch (AccountBlockedException& e) {
        std::cerr << "  Caught: " << e.what() << "\n";
    }
    aliceSavings->activateAccount();   // re-activate

    // -- Final Summary ---------------------------------
        std::cout << "║  System Summary ║\n";

    myBank.display();
    std::cout << "\nFinal Balances:\n";
    alice->showAccounts();
    bob->showAccounts();

    //All objects cleaned up via destructors.
    return 0;
}
