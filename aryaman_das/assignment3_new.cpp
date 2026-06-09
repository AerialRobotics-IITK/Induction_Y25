#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
using namespace std;

// def exceptions (inherit from runtime_error)
class InsufficientBalanceException : public runtime_error {
public:
    InsufficientBalanceException() : runtime_error("Insufficient balance") {}
};

class InvalidPINException : public runtime_error {
public:
    InvalidPINException() : runtime_error("Invalid pin") {}
};

class AccountBlockedException : public runtime_error {
public:
    AccountBlockedException() : runtime_error("Account blocked") {}
};

class LoanRejectedException : public runtime_error {
public:
    LoanRejectedException() : runtime_error("Loan rejected") {}
};

class Branch;
class Customer;
class Account;
class Loan;

class Notification {
public:
    virtual void send() = 0;
};

class SMSNotification: public Notification {
    string phoneNumber;
    string message;
public:
    SMSNotification(string phone, string msg) {
        phoneNumber = phone;
        message = msg;
    }
    void send() {
        cout << "sending sms " << phoneNumber << ": " << message << endl;
    }
};

class EmailNotification: public Notification {
    string emailAddress;
    string subject;
    string message;
    int deliveryStatus;
public:
    EmailNotification(string email, string sub, string msg) {
        emailAddress = email;
        subject = sub;
        message = msg;
        deliveryStatus = 0;
    }
    void send() {
        cout << "sending email to " << emailAddress << endl << "Subject - " << subject << endl << "Message - " << message << endl;
    }
};

class Account {
protected:
    long accountNumber;
    string accountType;
    double balance;
    string status;
public:
    Account(long accNo, string type, double initial) {
        accountNumber = accNo;
        accountType = type;
        balance = initial;
        status = "Active";
    }
    virtual void deposit(double amount) {
        if (status != "Active") { throw AccountBlockedException(); }
        if (amount <= 0) { return; }
        balance += amount;
    }
    virtual void withdraw(double amount) = 0;
    double getBalance() { return balance; }
    string getType() { return accountType; }
};

class SavingsAccount: public Account {
    double minimumBalance;
public:
    SavingsAccount(long accNo, double initialBalance, double minBalance)
        : Account(accNo, "Savings", initialBalance) {
        minimumBalance = minBalance;
    }
    void withdraw(double amount) {
        if (status != "Active") { throw AccountBlockedException(); }
        if (balance - amount < minimumBalance) { throw InsufficientBalanceException(); }
        balance -= amount;
    }
};

class CurrentAccount: public Account {
    double overdraftLimit;
    string businessName;
public:
    CurrentAccount(long accNo, double initialBalance, double overdraft, string name)
        : Account(accNo, "Current", initialBalance) {
        overdraftLimit = overdraft;
        businessName = name;
    }
    void withdraw(double amount) {
        if (status != "Active") { throw AccountBlockedException(); }
        if (balance - amount < -overdraftLimit) { throw InsufficientBalanceException(); }
        balance = balance - amount;
    }
};

class FixedDepositAccount: public Account {
    double interestRate;
    int tenureMonths;
public:
    FixedDepositAccount(long accNo, double amount, double rate, int months)
        : Account(accNo, "FixedDeposit", amount) {
        interestRate = rate;
        tenureMonths = months;
    }
    void withdraw(double amount) {
        if (status != "Active") { throw AccountBlockedException(); }
        if (amount > balance) { throw InsufficientBalanceException(); }
        balance = balance - amount;
    }
};

class Customer {
    int customerId;
    string fullName;
    string dob;
    string gender;
    string mobileNumber;
    string email;
    string address;
    string aadharNumber;
    string panNumber;
    vector<Loan*> loans;
    vector<Account*> accounts;
public:
    Customer(int id, string name, string dateOfBirth, string gen, string mobile, string mail, string addr, string aadhar, string pan) {
        customerId = id;
        fullName = name;
        dob = dateOfBirth;
        gender = gen;
        mobileNumber = mobile;
        email = mail;
        address = addr;
        aadharNumber = aadhar;
        panNumber = pan;
    }
    void addAccount(Account* acc) { accounts.push_back(acc); }
};

class Employee {
    int employeeId;
    string employeeName;
    string designation;
    double salary;
    Branch* branch;
public:
    Employee(int id, string name, string desig, double sal, Branch* b) {
        employeeId = id;
        employeeName = name;
        designation = desig;
        salary = sal;
        branch = b;
    }
};

class Branch {
    int branchId;
    string branchName;
    string ifscode;
    string address;
    vector<Account*> accounts;
    vector<Employee*> employees;
public:
    Branch(int id, string name, string ifsc, string addr) {
        branchId = id;
        branchName = name;
        ifscode = ifsc;
        address = addr;
    }
    void addAccount(Account* acc) { accounts.push_back(acc); }
    void addEmployee(Employee* emp) { employees.push_back(emp); }
};

class Bank {
    int bankId;
    string bankName;
    vector<Branch*> branches;
    vector<Customer*> customers;
    vector<Employee*> employees;
public:
    Bank(int id, string name) {
        bankId = id;
        bankName = name;
    }
    void addBranch(Branch* b) { branches.push_back(b); }
    void addCustomer(Customer* c) { customers.push_back(c); }
};

class Loan {
    int loanId;
    string loanType;
    Customer* borrower;
    double loanAmount;
    double interestRate;
    int tenureYears;
    int loanStatus;
    double EMIAmount;
public:
    Loan(int id, string type, Customer* b, double amount, double rate, int years) {
        loanId = id;
        loanType = type;
        borrower = b;
        loanAmount = amount;
        interestRate = rate;
        tenureYears = years;
        loanStatus = 0;
        EMIAmount = 0;
    }
    void approve() {
        loanStatus = 1;
        int months = tenureYears * 12;
        double r = interestRate / 12.0 / 100.0;
        EMIAmount = loanAmount * r * pow(1 + r, months) / (pow(1 + r, months) - 1);
    }
};

class ATMCard {
    long cardNumber;
    string expiryDate;
    string cardType;
    string cardStatus;
    Account* linkedAccount;
protected:
    int PIN;
    int cvv;
public:
    ATMCard(long number, int pin, Account* acc, int cvvCode, string expiry, string type) {
        cardNumber = number;
        PIN = pin;
        cvv = cvvCode;
        expiryDate = expiry;
        cardType = type;
        linkedAccount = acc;
        cardStatus = "Active";
    }
    void validatePIN(int enteredPIN) {
        if (cardStatus != "Active") { throw AccountBlockedException(); }
        if (enteredPIN != PIN) { throw InvalidPINException(); }
    }
};

class Transaction {
public:
    string transactionType;
    string transactionId;
    string transactionDate;
    double amount;
    string status;
    Account* sender;
    Account* receiver;

    Transaction(string type, string id, string date, double amt, Account* senderAcc, Account* receiverAcc) {
        transactionType = type;
        transactionId = id;
        transactionDate = date;
        amount = amt;
        status = "Pending";
        sender = senderAcc;
        receiver = receiverAcc;
    }

    void execute() {
        try {
            sender->withdraw(amount);
            receiver->deposit(amount);
            status = "Success";
            cout << "SUCCESS" << endl;
        } catch (...) {
            status = "Failed";
            throw;
        }
    }
};

int main() {
    try {
        // Create two accounts
        SavingsAccount acc1(1001, 5000, 1000);
        CurrentAccount acc2(2001, 2000, 5000, "ABC Traders");
        Transaction t1(
            "Transfer",
            "t1",
            "date",
            1500,
            &acc1,
            &acc2
        );

        t1.execute();

        cout << "After Transaction:\n";
        cout << "Savings: " << acc1.getBalance() << endl;
        cout << "Current: " << acc2.getBalance() << endl;

        //test pin
        ATMCard card(123456789, 1234, &acc1, 111, "12/30", "Visa");

        card.validatePIN(1234);
        cout << "\nPIN verified successfully\n";
        //notif
        SMSNotification sms("9876543210", "Transaction successful");
        sms.send();

    } catch (exception& e) {
        cout << "Error" << endl;
    }

    return 0;
}