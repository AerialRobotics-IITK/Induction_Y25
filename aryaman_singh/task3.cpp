#include <bits/stdc++.h>
using namespace std;
// geno singh
class InsufficientBalanceException{
public:
    void what() {cout << "Error: Insufficient balance.\n";}
};
class InvalidPINException{
public:
    void what() {cout << "Error: Invalid PIN.\n";}
};
 
class AccountBlockedException{
public:
    void what() {cout << "Error: Account is blocked.\n";}
};
 
class LoanRejectedException{
public:
    void what() {cout << "Error: Loan rejected.\n";}
};
 
class Notification{
public:
    string message;
    virtual void send() = 0;  
    virtual ~Notification(){}
};
 
class SMSNotification : public Notification{
public:
    string phoneNumber;
    string deliveryStatus;
    SMSNotification(string msg, string phone){
        message       = msg;
        phoneNumber   = phone;
        deliveryStatus = "pending";
    }
    void send() override{
        cout << "[SMS to " << phoneNumber << "]: " << message << "\n";
        deliveryStatus = "sent";
    }
};
class EmailNotification : public Notification{
public:
    string emailAddress;
    string subject;
    string deliveryStatus;
    EmailNotification(string msg, string email, string subj){
        message       = msg;
        emailAddress  = email;
        subject       = subj;
        deliveryStatus = "pending";
    }
    void send() override{
        cout << "[Email to " << emailAddress << " | " << subject << "]: " << message << "\n";
        deliveryStatus = "sent";
    }
};
class Transaction{
public:
    int transactionId;
    string transactionType;
    double amount;
    string status;
    Transaction(int id, string type, double amt, string stat){
        transactionId   = id;
        transactionType = type;
        amount          = amt;
        status          = stat;
    }
    // basically display info for(if reqd) debugging on the frontend side
    void display(){
        cout << "  Txn#" << transactionId
             << " | " << transactionType
             << " | Rs." << amount
             << " | " << status << "\n";
    }
};

class Account{
public:
    long   accountNumber;
    string accountType;
    double balance;
    string status;  
    vector<Transaction> transactions;
    int txnCounter = 1;
    // status can be either blocked or active
    Account(long accNo, string type, double bal){
        accountNumber = accNo;
        accountType   = type;
        balance       = bal;
        status        = "active";
    }
    void deposit(double amount){
        if (status == "blocked") throw AccountBlockedException();
        balance += amount;
        transactions.push_back(Transaction(txnCounter++, "deposit", amount, "success"));
        cout << "Deposited Rs." << amount << " | New balance: Rs." << balance << "\n";
    }
 
    void withdraw(double amount){
        if (status == "blocked") throw AccountBlockedException();
        if (amount > balance)    throw InsufficientBalanceException();
        balance -= amount;
        transactions.push_back(Transaction(txnCounter++, "withdraw", amount, "success"));
        cout << "Withdrawn Rs." << amount << " | New balance: Rs." << balance << "\n";
    }
    void printTransactions(){
        cout << "Transactions for account " << accountNumber << ":\n";
        for (auto& t : transactions) t.display();
    }
    virtual void applyInterest()    = 0;   
    virtual void displayDetails()   = 0;
    virtual ~Account() {}
};
class SavingsAccount : public Account{
public:
    double interestRate;
    double minimumBalance;
    SavingsAccount(long accNo, double bal, double rate, double minBal)
        : Account(accNo, "Savings", bal) {
        interestRate   = rate;
        minimumBalance = minBal;
    }
    void applyInterest() override{
        double interest = balance * (interestRate / 100);
        balance += interest;
        cout << "Interest of Rs." << interest << " applied. New balance: Rs." << balance << "\n";
    }
    // basically display info for(if reqd) debugging on the frontend side
    void displayDetails() override{
        cout << "-- Savings Account --\n"
             << "  Acc#    : " << accountNumber  << "\n"
             << "  Balance : Rs." << balance     << "\n"
             << "  Rate    : " << interestRate << "%\n"
             << "  Min Bal : Rs." << minimumBalance << "\n"
             << "  Status  : " << status         << "\n";
    }
};
class CurrentAccount : public Account{
public:
    double overdraftLimit;
    string businessName;
    CurrentAccount(long accNo, double bal, double odLimit, string bizName)
        : Account(accNo, "Current", bal){
        overdraftLimit = odLimit;
        businessName   = bizName;
    }
    void applyInterest() override{
        cout << "No interest for current accounts.\n";
    }
    // basically display info for(if reqd) debugging on the frontend side
    void displayDetails() override{
        cout << "-- Current Account --\n"
             << "  Acc#      : " << accountNumber  << "\n"
             << "  Balance   : Rs." << balance     << "\n"
             << "  Overdraft : Rs." << overdraftLimit << "\n"
             << "  Business  : " << businessName   << "\n"
             << "  Status    : " << status         << "\n";
    }
};
class FixedDepositAccount : public Account{
public:
    double FDAmount;
    double FDInterestRate;
    int    tenureMonths;
    FixedDepositAccount(long accNo, double fdAmt, double fdRate, int tenure)
        : Account(accNo, "Fixed Deposit", fdAmt){
        FDAmount       = fdAmt;
        FDInterestRate = fdRate;
        tenureMonths   = tenure;
    }
    void applyInterest() override{
        // used a standard formula for maturity, fd
        double maturity = FDAmount * (1 + (FDInterestRate / 100) * (tenureMonths / 12.0));
        cout << "FD maturity amount: Rs." << maturity << "\n";
        balance = maturity;
    }
    // basically display info for(if reqd) debugging on the frontend side
    void displayDetails() override{
        cout << "-- Fixed Deposit --\n"
             << "  Acc#    : " << accountNumber    << "\n"
             << "  Amount  : Rs." << FDAmount      << "\n"
             << "  Rate    : " << FDInterestRate << "%\n"
             << "  Tenure  : " << tenureMonths << " months\n"
             << "  Status  : " << status           << "\n";
    }
};
class Loan{
public:
    int    loanId;
    string loanType;
    double loanAmount;
    double interestRate;
    int    tenureYears;
    double EMIAmount;
    string loanStatus;
    Loan(int id, string type, double amount, double rate, int tenure){
        loanId      = id;
        loanType    = type;
        loanAmount  = amount;
        interestRate = rate;
        tenureYears  = tenure;
        loanStatus   = "pending";
        EMIAmount    = 0;
    }
    void approveLoan(){
        if (loanAmount <= 0) throw LoanRejectedException();
        // used a simple standard formula for emi
        EMIAmount  = (loanAmount + (loanAmount * interestRate / 100 * tenureYears))
                     / (tenureYears * 12);
        loanStatus = "approved";
        cout << "Loan approved! EMI: Rs." << EMIAmount << "/month\n";
    }
    // basically display info for(if reqd) debugging on the frontend side
    void display(){
        cout << "-- Loan --\n"
             << "  ID      : " << loanId    << "\n"
             << "  Type    : " << loanType  << "\n"
             << "  Amount  : Rs." << loanAmount << "\n"
             << "  EMI     : Rs." << EMIAmount  << "\n"
             << "  Status  : " << loanStatus   << "\n";
    }
};
class ATMCard{
public:
    long   cardNumber;
    int    CVV;
    int    PIN;
    string cardType;
    string cardStatus;
    Account* linkedAccount;
    ATMCard(long num, int cvv, int pin, string type, Account* acc){
        cardNumber     = num;
        CVV            = cvv;
        PIN            = pin;
        cardType       = type;
        cardStatus     = "active";
        linkedAccount  = acc;
    }
    void withdrawFromATM(int enteredPIN, double amount){
        if (cardStatus == "blocked") throw AccountBlockedException();
        if (enteredPIN != PIN)       throw InvalidPINException();
        linkedAccount->withdraw(amount);
    }
    // basically display info for(if reqd) debugging on the frontend side
    void display(){
        cout << "-- ATM Card --\n"
             << "  Card#  : " << cardNumber << "\n"
             << "  Type   : " << cardType   << "\n"
             << "  Status : " << cardStatus << "\n";
    }
};
class Employee{
public:
    int    employeeId;
    string employeeName;
    string designation;
    double salary;
    Employee(int id, string name, string desg, double sal){
        employeeId   = id;
        employeeName = name;
        designation  = desg;
        salary       = sal;
    }
    // basically display info for(if reqd) debugging on the frontend side
    void display(){
        cout << "-- Employee --\n"
             << "  ID   : " << employeeId   << "\n"
             << "  Name : " << employeeName << "\n"
             << "  Role : " << designation  << "\n"
             << "  Salary: Rs." << salary   << "\n";
    }
};
class Customer{
public:
    int    customerId;
    string fullName;
    string mobileNumber;
    string email;
    string aadhaarNumber;
    string PANNumber;
    vector<Account*> accounts;
    vector<Loan*>    loans;
 
    Customer(int id, string name, string mobile, string mail,
             string aadhaar, string pan){
        customerId    = id;
        fullName      = name;
        mobileNumber  = mobile;
        email         = mail;
        aadhaarNumber = aadhaar;
        PANNumber     = pan;
    }
    // basically display info for(if reqd) debugging on the frontend side
    void display(){
        cout << "-- Customer --\n"
             << "  ID     : " << customerId   << "\n"
             << "  Name   : " << fullName     << "\n"
             << "  Mobile : " << mobileNumber << "\n"
             << "  Email  : " << email        << "\n";
    }
};
class Branch{
public:
    int    branchId;
    string branchName;
    string IFSCCode;
    string address;
    vector<Account*>  accounts;
    vector<Employee*> employees;
    Branch(int id, string name, string ifsc, string addr){
        branchId   = id;
        branchName = name;
        IFSCCode   = ifsc;
        address    = addr;
    }
    // basically display info for(if reqd) debugging on the frontend side
    void display() {
        cout << "-- Branch --\n"
             << "  ID   : " << branchId   << "\n"
             << "  Name : " << branchName << "\n"
             << "  IFSC : " << IFSCCode   << "\n"
             << "  Addr : " << address    << "\n";
    }
};
class Bank{
public:
    int    bankId;
    string bankName;
    vector<Branch*>   branches;
    vector<Customer*> customers;
    vector<Employee*> employees;
    Bank(int id, string name){
        bankId   = id;
        bankName = name;
    }
    // basically display info for(if reqd) debugging on the frontend side
    void display(){
        cout << "-- Bank --\n"
             << "  Name      : " << bankName          << "\n"
             << "  Branches  : " << branches.size()  << "\n"
             << "  Customers : " << customers.size() << "\n";
    }
};
