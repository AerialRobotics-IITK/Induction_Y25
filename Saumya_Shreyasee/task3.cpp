#include <iostream>
#include <list>
#include <string>
#include <memory>
#include <exception>

using namespace std;

class Bank;
class Branch;
class Customer;
class Account;
class SavingsAccount;
class CurrentAccount;
class FixedDepositAccount;
class Transaction;
class Loan;
class ATMCard;
class Employee;
class Notification;
class EmailNotification;
class SMSNotification;

//excpetion handling
class InsufficientBalanceException : public exception {
private:
    string message;
public:
    InsufficientBalanceException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidPINException : public exception {
private:
    string message;
public:
    InvalidPINException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class AccountLockedException : public exception {
private:
    string message;
public:
    AccountLockedException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class LoanRejectedException : public exception {
private:
    string message;
public:
    LoanRejectedException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

//classes
class Date {
private:
    int day;
    int month;
    int year;
public:
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    void getDate() const {
        cout << day << "-" << month << "-" << year << endl;
    }
};

class Notification { 
public:
    virtual ~Notification() = default;
    virtual string getmessage() {
        return "message"; 
    }
};

class SMSNotification : public Notification {
private:
    string phoneNumber;
    string message;
    string deliveryStatus;
public:
    SMSNotification(string phone, string msg) 
        : phoneNumber(phone), message(msg), deliveryStatus("Pending") {}

    string getmessage() override { return message; }
    string getnumber() { return phoneNumber; }
    string getstatus() { return deliveryStatus; }  
};

class EmailNotification : public Notification {
private:
    string emailAddress;
    string subject;
    string message;
    string deliveryStatus;
public:
    EmailNotification(string email, string subj, string msg) 
        : emailAddress(email), subject(subj), message(msg), deliveryStatus("Pending") {}

    string getmessage() override { return message; }
    string getaddress() { return emailAddress; }
    string getstatus() { return deliveryStatus; }  
    string getsubject() { return subject; }
};

class Customer {
public:
    int customerId;
    string fullName;
    Date dob;
    string gender;
    string mobileNumber;
    string email;
    string address;
    string aadharNumber;
    string PANNumber;
    std::list<std::shared_ptr<Account>> accounts;
    std::list<std::shared_ptr<Loan>> loans;

    Customer(int id, string name, Date date, string gen, string mobile, string mail, string addr, string aadhar, string pan)
        : customerId(id), fullName(name), dob(date), gender(gen), mobileNumber(mobile), 
          email(mail), address(addr), aadharNumber(aadhar), PANNumber(pan) {}
};

class Account {
protected: 
    long accountNumber;
    string accountType;
    double balance;
    Date dateOpened;
    string status;
    std::shared_ptr<Branch> branch; 
    std::shared_ptr<Customer> customer;
    std::list<std::shared_ptr<Transaction>> transactions;

public:
    Account(long acc, string type, double bal, Date openDate, std::shared_ptr<Branch> br, std::shared_ptr<Customer> cust)
        : accountNumber(acc), accountType(type), balance(bal), dateOpened(openDate), status("Active"), branch(br), customer(cust) {}
        
    virtual ~Account() = default;
    
    virtual void deposit(double amount) { 
        balance += amount; 
    }
    
    virtual void withdraw(double amount) = 0; 
    
    double getBalance() const { return balance; }
    long getAccountNumber() const { return accountNumber; }
};


class SavingsAccount : public Account {
public:
    double interestRate;
    double minimumBalance;

    SavingsAccount(long acc, double bal, Date openDate, std::shared_ptr<Branch> br, std::shared_ptr<Customer> cust, double rate, double minBal)
        : Account(acc, "Savings", bal, openDate, br, cust), interestRate(rate), minimumBalance(minBal) {}
    
    void withdraw(double amount) override {
        if (balance - amount < minimumBalance) {
            throw InsufficientBalanceException("Transaction Denied: Withdrawal violates minimum balance threshold.");
        }
        balance -= amount;
    }
};

class CurrentAccount : public Account {
public:
    double overdraftLimit;
    string businessName;

    CurrentAccount(long acc, double bal, Date openDate, std::shared_ptr<Branch> br, std::shared_ptr<Customer> cust, double limit, string bName)
        : Account(acc, "Current", bal, openDate, br, cust), overdraftLimit(limit), businessName(bName) {}

    void withdraw(double amount) override {
        if (balance - amount < -overdraftLimit) {
            throw InsufficientBalanceException("Transaction Denied: Absolute overdraft limit reached.");
        }
        balance -= amount;
    }
};

class FixedDepositAccount : public Account {
public:
    double FDAmount;
    Date maturityDate;
    double FDInterestRate;
    int tenureMonths;

    FixedDepositAccount(long acc, double bal, Date openDate, std::shared_ptr<Branch> br, std::shared_ptr<Customer> cust, double fdAmt, Date matDate, double rate, int months)
        : Account(acc, "FixedDeposit", bal, openDate, br, cust), FDAmount(fdAmt), maturityDate(matDate), FDInterestRate(rate), tenureMonths(months) {}

    void withdraw(double amount) override {
        throw InsufficientBalanceException("Transaction Denied: Term deposits cannot be partially withdrawn via general requests.");
    }
};


class Branch {
public:
    int branchId;
    string branchName;
    string IFSCCode;
    string address;
    std::list<std::shared_ptr<Account>> accounts; 
    std::list<std::shared_ptr<Employee>> employees;

    Branch(int id, string name, string ifsc, string addr)
        : branchId(id), branchName(name), IFSCCode(ifsc), address(addr) {}
};

class Bank {
public:
    int bankId;
    string bankName;
    std::list<std::shared_ptr<Branch>> branches;
    std::list<std::shared_ptr<Customer>> customers;
    std::list<std::shared_ptr<Employee>> employees;

    Bank(int id, string name) : bankId(id), bankName(name) {}
};

class Transaction {
public:
    int transactionId;
    string transactionType;
    double amount;
    Date transactionDate;
    std::shared_ptr<Account> senderAccount;
    std::shared_ptr<Account> receiverAccount;
    string status;

    Transaction(int id, string type, double amt, Date date, std::shared_ptr<Account> sender, std::shared_ptr<Account> receiver)
        : transactionId(id), transactionType(type), amount(amt), transactionDate(date), 
          senderAccount(sender), receiverAccount(receiver), status("Pending") {}
};

class Loan {
public:
    int loanId;
    string loanType;
    double loanAmount;
    double interestRate;
    int tenureYears;
    double EMIAmount;
    string loanStatus;
    std::shared_ptr<Customer> customer;

    Loan(int id, string type, double amt, double rate, int years, double emi, std::shared_ptr<Customer> cust)
        : loanId(id), loanType(type), loanAmount(amt), interestRate(rate), tenureYears(years), 
          EMIAmount(emi), loanStatus("Pending"), customer(cust) {}
};

class ATMCard {
public:
    long cardNumber;
    int CVV;
    Date expiryDate;
    int PIN;
    string cardType;
    string cardStatus;
    std::shared_ptr<Account> linkedAccount;

    ATMCard(long num, int cvv, Date exp, int pin, string type, std::shared_ptr<Account> acc)
        : cardNumber(num), CVV(cvv), expiryDate(exp), PIN(pin), cardType(type), 
          cardStatus("Active"), linkedAccount(acc) {}
};

class Employee {
public:
    int employeeId;
    string employeeName;
    string designation;
    double salary;
    std::shared_ptr<Branch> branch;

    Employee(int id, string name, string desig, double sal, std::shared_ptr<Branch> br)
        : employeeId(id), employeeName(name), designation(desig), salary(sal), branch(br) {}
};


int main() {
    auto myBank = std::make_shared<Bank>(1, "Axis Bank");

    auto branchA = std::make_shared<Branch>(101, "Main Campus Branch", "AXIS0000101", "123 Academic Road");
    myBank->branches.push_back(branchA);

    Date customerDOB(1, 1, 2005);
    auto customer = std::make_shared<Customer>(
        10743, "Amit Singh", customerDOB, "Male", 
        "+919876543210", "amit3242@email.com", "Sector 20", 
        "1234-5678-9012", "ABCDE1234F"
    );
    myBank->customers.push_back(customer);

    Date today(21, 5, 2026);
    
    auto savings = std::make_shared<SavingsAccount>(
        987654321, 5000.0, today, branchA, customer, 4.0, 1000.0
    );
    
    auto current = std::make_shared<CurrentAccount>(
        123456789, 2000.0, today, branchA, customer, 10000.0, "Amit Enterprises"
    );

    branchA->accounts.push_back(savings);
    branchA->accounts.push_back(current);
    customer->accounts.push_back(savings);
    customer->accounts.push_back(current);

    cout << "System initialized successfully." << endl;
    cout << "Savings Account initial balance: INR " << savings->getBalance() << endl;
    cout << "Current Account initial balance: INR " << current->getBalance() << endl;


    cout << "Depositing INR 1500 into Savings..." << endl;
    savings->deposit(1500.0);
    cout << "New Savings Balance: INR " << savings->getBalance() << endl;

    SMSNotification smsAlert(customer->mobileNumber, "Your account has been credited with INR 1500.00");
    cout << "[Notification Triggered] " << smsAlert.getnumber() << " -> " << smsAlert.getmessage() << endl;

    try {
        cout << "Attempting to withdraw INR 6000 from Savings (Current Balance: " << savings->getBalance() 
             << ", Min Required: " << savings->minimumBalance << ")..." << endl;
             
        savings->withdraw(6000.0); 
    } 
    catch (const InsufficientBalanceException& e) {
        cout << ">>> Caught expected exception: " << e.what() << endl;
        
        EmailNotification emailAlert(customer->email, "Security Warning: Transaction Failed", "A withdrawal request failed due to insufficient funds.");
        cout << "[Polymorphic Alert Sent] " << emailAlert.getaddress() << " Subject: " << emailAlert.getsubject() << endl;
    }
    catch (const std::exception& e) {
        cout << "Caught generic exception: " << e.what() << endl;
    }

    try {
        cout << "\nAttempting to withdraw INR 15000 from Current Account (Balance: " << current->getBalance() 
             << ", Overdraft Limit: " << current->overdraftLimit << ")..." << endl;
             
        current->withdraw(15000.0);
} 
    catch (const InsufficientBalanceException& e) {
        cout << ">>> Caught expected exception: " << e.what() << endl;
    }

    try {
        Date maturity(21, 5, 2027);
        auto fdAccount = std::make_shared<FixedDepositAccount>(
            555666777, 50000.0, today, branchA, customer, 50000.0, maturity, 7.5, 12
        );
        
        cout << "\nAttempting to execute an early partial withdrawal on a Fixed Deposit..." << endl;
        fdAccount->withdraw(5000.0); 
    }
    catch (const InsufficientBalanceException& e) {
        cout << ">>> Caught expected exception: " << e.what() << endl;
    }

    return 0;
}