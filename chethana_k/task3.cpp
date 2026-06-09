// SmartBank Enterprise System - OOP Based Banking Project
// Language: C++
// Concepts Used:
// 1. Encapsulation
// 2. Inheritance
// 3. Abstraction
// 4. Polymorphism
// 5. Composition & Association
// 6. Exception Handling
// 7. Factory Pattern
// 8. Strategy Pattern

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <stdexcept>

using namespace std;


// CUSTOM EXCEPTIONS

class InsufficientBalanceException : public exception {
public:
    const char* what() const noexcept override {
        return "Insufficient Balance!";
    }
};

class InvalidPINException : public exception {
public:
    const char* what() const noexcept override {
        return "Invalid PIN!";
    }
};

class AccountBlockedException : public exception {
public:
    const char* what() const noexcept override {
        return "Account is Blocked!";
    }
};

class LoanRejectedException : public exception {
public:
    const char* what() const noexcept override {
        return "Loan Rejected!";
    }
};


// NOTIFICATION STRATEGY PATTERN

class Notification {
public:
    virtual void sendNotification(string msg) = 0;
    virtual ~Notification() {}
};

class SMSNotification : public Notification {
private:
    string phoneNumber;

public:
    SMSNotification(string phone) {
        phoneNumber = phone;
    }

    void sendNotification(string msg) override {
        cout << "[SMS] Sent to " << phoneNumber << " : " << msg << endl;
    }
};

class EmailNotification : public Notification {
private:
    string email;

public:
    EmailNotification(string e) {
        email = e;
    }

    void sendNotification(string msg) override {
        cout << "[EMAIL] Sent to " << email << " : " << msg << endl;
    }
};

// FORWARD DECLARATIONS

class Customer;
class Branch;
class Account;


// TRANSACTION CLASS

class Transaction {
private:
    int transactionId;
    string transactionType;
    double amount;
    string status;

public:
    Transaction(int id, string type, double amt, string stat) {
        transactionId = id;
        transactionType = type;
        amount = amt;
        status = stat;
    }

    void displayTransaction() {
        cout << "Transaction ID: " << transactionId << endl;
        cout << "Type: " << transactionType << endl;
        cout << "Amount: " << amount << endl;
        cout << "Status: " << status << endl;
    }
};

// ABSTRACT ACCOUNT CLASS

class Account {
protected:
    long accountNumber;
    string accountType;
    double balance;
    string status;

    Customer* customer;
    Branch* branch;

    vector<Transaction> transactions;

public:
    Account(long accNo, string type, double bal) {
        accountNumber = accNo;
        accountType = type;
        balance = bal;
        status = "Active";
    }

    virtual void deposit(double amount) {
        balance += amount;

        transactions.push_back(
            Transaction(1, "Deposit", amount, "Success"));

        cout << amount << " deposited successfully.\n";
    }

    virtual void withdraw(double amount) {
        if (status == "Blocked")
            throw AccountBlockedException();

        if (amount > balance)
            throw InsufficientBalanceException();

        balance -= amount;

        transactions.push_back(
            Transaction(2, "Withdraw", amount, "Success"));

        cout << amount << " withdrawn successfully.\n";
    }

    virtual void calculateInterest() = 0;

    double getBalance() {
        return balance;
    }

    long getAccountNumber() {
        return accountNumber;
    }

    virtual void displayAccount() {
        cout << "\nAccount Number: " << accountNumber << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Balance: " << balance << endl;
        cout << "Status: " << status << endl;
    }

    virtual ~Account() {}
};


// SAVINGS ACCOUNT

class SavingsAccount : public Account {
private:
    double interestRate;
    double minimumBalance;

public:
    SavingsAccount(long accNo, double bal)
        : Account(accNo, "Savings", bal) {

        interestRate = 4;
        minimumBalance = 1500;
    }

    void calculateInterest() override {
        double interest = balance * interestRate / 100;
        cout << "Savings Interest: " << interest << endl;
    }

    void withdraw(double amount) override {

        if ((balance - amount) < minimumBalance) {
            throw InsufficientBalanceException();
        }

        balance -= amount;

        cout << "Withdrawal successful from Savings Account.\n";
    }
};


// CURRENT ACCOUNT

class CurrentAccount : public Account {
private:
    double overdraftLimit;
    string businessName;

public:
    CurrentAccount(long accNo,
                   double bal,
                   string business)
        : Account(accNo, "Current", bal) {

        overdraftLimit = 45000;
        businessName = business;
    }

    void calculateInterest() override {
        cout << "No interest for Current Account.\n";
    }

    void withdraw(double amount) override {

        if ((balance + overdraftLimit) < amount) {
            throw InsufficientBalanceException();
        }

        balance -= amount;

        cout << "Withdrawal successful from Current Account.\n";
    }
};


// FIXED DEPOSIT ACCOUNT

class FixedDepositAccount : public Account {
private:
    double FDAmount;
    int tenureMonths;
    double FDInterestRate;

public:
    FixedDepositAccount(long accNo,
                        double amount,
                        int tenure)
        : Account(accNo, "Fixed Deposit", amount) {

        FDAmount = amount;
        tenureMonths = tenure;
        FDInterestRate = 7;
    }

    void calculateInterest() override {

        double maturity =
            FDAmount +
            (FDAmount * FDInterestRate * tenureMonths) / (100 * 12);

        cout << "FD Maturity Amount: " << maturity << endl;
    }
};

// CUSTOMER CLASS

class Customer {
private:
    int customerId;
    string fullName;
    string mobileNumber;
    string email;

    vector<Account*> accounts;

public:
    Customer(int id,
             string name,
             string mobile,
             string mail) {

        customerId = id;
        fullName = name;
        mobileNumber = mobile;
        email = mail;
    }

    void addAccount(Account* acc) {
        accounts.push_back(acc);
    }

    void displayCustomer() {
        cout << "\nCustomer ID: " << customerId << endl;
        cout << "Name: " << fullName << endl;
        cout << "Mobile: " << mobileNumber << endl;
        cout << "Email: " << email << endl;
    }

    string getMobile() {
        return mobileNumber;
    }

    string getEmail() {
        return email;
    }
};


// EMPLOYEE CLASS

class Employee {
private:
    int employeeId;
    string employeeName;
    string designation;
    double salary;

public:
    Employee(int id,
             string name,
             string desig,
             double sal) {

        employeeId = id;
        employeeName = name;
        designation = desig;
        salary = sal;
    }

    void displayEmployee() {
        cout << "\nEmployee ID: " << employeeId << endl;
        cout << "Name: " << employeeName << endl;
        cout << "Designation: " << designation << endl;
    }
};


// BRANCH CLASS

class Branch {
private:
    int branchId;
    string branchName;
    string IFSCCode;

    vector<Account*> accounts;
    vector<Employee> employees;

public:
    Branch(int id,
           string name,
           string ifsc) {

        branchId = id;
        branchName = name;
        IFSCCode = ifsc;
    }

    void addAccount(Account* acc) {
        accounts.push_back(acc);
    }

    void addEmployee(Employee emp) {
        employees.push_back(emp);
    }

    void displayBranch() {
        cout << "\nBranch ID: " << branchId << endl;
        cout << "Branch Name: " << branchName << endl;
        cout << "IFSC: " << IFSCCode << endl;
    }
};

// LOAN CLASS

class Loan {
private:
    int loanId;
    string loanType;
    double loanAmount;
    double interestRate;
    int tenureYears;
    double EMIAmount;
    string loanStatus;

public:
    Loan(int id,
         string type,
         double amount,
         int tenure) {

        loanId = id;
        loanType = type;
        loanAmount = amount;
        tenureYears = tenure;

        interestRate = 9;
        loanStatus = "Approved";

        EMIAmount =
            (loanAmount +
             (loanAmount * interestRate * tenureYears / 100))
            / (tenureYears * 12);
    }

    void displayLoan() {
        cout << "\nLoan ID: " << loanId << endl;
        cout << "Loan Type: " << loanType << endl;
        cout << "Loan Amount: " << loanAmount << endl;
        cout << "EMI: " << EMIAmount << endl;
    }
};

// ATM CARD CLASS

class ATMCard {
private:
    long cardNumber;
    int CVV;
    int PIN;
    string cardStatus;

    Account* linkedAccount;

public:
    ATMCard(long cardNo,
            int cvv,
            int pin,
            Account* acc) {

        cardNumber = cardNo;
        CVV = cvv;
        PIN = pin;
        linkedAccount = acc;

        cardStatus = "Active";
    }

    void verifyPIN(int enteredPIN) {

        if (enteredPIN != PIN)
            throw InvalidPINException();

        cout << "PIN Verified Successfully.\n";
    }

    void displayCard() {
        cout << "\nATM Card Number: " << cardNumber << endl;
        cout << "Status: " << cardStatus << endl;
    }
};


// FACTORY PATTERN FOR ACCOUNT CREATION

class AccountFactory {
public:
    static Account* createAccount(int choice) {

        if (choice == 1) {
            return new SavingsAccount(1001, 5000);
        }

        else if (choice == 2) {
            return new CurrentAccount(
                2001,
                10000,
                "ABC Pvt Ltd");
        }

        else if (choice == 3) {
            return new FixedDepositAccount(
                3001,
                50000,
                24);
        }

        return nullptr;
    }
};


// BANK CLASS

class Bank {
private:
    int bankId;
    string bankName;

    vector<Branch> branches;
    vector<Customer> customers;

public:
    Bank(int id, string name) {
        bankId = id;
        bankName = name;
    }

    void addBranch(Branch branch) {
        branches.push_back(branch);
    }

    void addCustomer(Customer customer) {
        customers.push_back(customer);
    }

    void displayBank() {
        cout << "\n===== BANK DETAILS =====\n";
        cout << "Bank ID: " << bankId << "\n"
        cout << "Bank Name: " << bankName << endl;
    }
};

// MAIN FUNCTION

int main() {

    try {

        // BANK
        Bank bank(1, "SmartBank");

        // BRANCH
        Branch branch1(101,
                       "Anantapur Main Branch",
                       "SBIN000123");

        // CUSTOMER
        Customer customer1(
            1,
            "Chethana Kolta",
            "9014604879",
            "chethanakotla1003@gmail.com");

        // FACTORY PATTERN
        Account* acc1 =
            AccountFactory::createAccount(1);

        // ADD ACCOUNT TO CUSTOMER
        customer1.addAccount(acc1);

        // ACCOUNT OPERATIONS
        acc1->displayAccount();

        acc1->deposit(2000);

        acc1->withdraw(1000);

        acc1->calculateInterest();

        // ATM CARD
        ATMCard atm(
            123456789012,
            456,
            1234,
            acc1);

        atm.verifyPIN(1234);

        // LOAN
        Loan loan1(
            5001,
            "Home Loan",
            1000000,
            20);

        loan1.displayLoan();

        // NOTIFICATIONS
        Notification* sms =
            new SMSNotification("9876543210");

        sms->sendNotification(
            "Transaction Successful");

        Notification* email =
            new EmailNotification(
                "rahul@gmail.com");

        email->sendNotification(
            "Loan Approved");

        delete sms;
        delete email;
        delete acc1;
    }

    catch (exception& e) {
        cout << "\nException: "
             << e.what() << endl;
    }

    return 0;
}