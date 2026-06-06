#include <iostream>
#include <vector>
#include <exception>
#include <string>
#include <cmath>
using namespace std;

//CUSTOM BANKING EXCEPTIONS
class InsufficientBalanceException : public exception {
    private:
        string message;
    public:
        InsufficientBalanceException(string msg) : message("Insufficient Balance Error: " + msg) {}
        const char* what() const noexcept override {
            return message.c_str();
        }
};

class InvalidPINException : public exception {
private:
    string message;
public:
    InvalidPINException(string msg) : message("Security Error: " + msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class AccountBlockedException : public exception {
private:
    string message;
public:
    AccountBlockedException(string msg) : message("Account Status Error: " + msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class LoanRejectedException : public exception {
private:
    string message;
public:
    LoanRejectedException(string msg) : message("Loan Processing Error: " + msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// CLASS DECLARATIONS
class Bank;
class Branch;
class Customer;
class Employee;
class Account;
class Transaction;
class Loan;

// UTILITY CLASS
class Date {
public:
    int day;
    int month;
    int year;

    Date() {
        day = 0;
        month = 0;
        year = 0;
    }
    Date(int d, int m, int y) {
        day = d;
        month = m;
        year = y;
    } 
};

// MAIN ENTITY CLASSES
class Bank {
public:
    int bankId;
    string bankName;
    vector <Branch*> branches;
    vector <Customer*> customers;
    vector <Employee*> employees;

    Bank(int id, string name) :
        bankId(id),
        bankName(name) {}
};

class Branch {
public:
    int branchId;
    string branchName;
    string IFSCCode;
    string address;
    vector <Account*> accounts;
    vector <Employee*> employees;

    Branch(int id, string name, string ifsc, string addr) :
        branchId(id),
        branchName(name),
        IFSCCode(ifsc),
        address(addr) {}
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
    vector <Account*> accounts;
    vector <Loan*> loans;

    static int nextLoanId;

    Customer(int id, string name, Date dob, string gen, string mno, string mail, string addr, string aadhar, string pan) :
        customerId(id),
        fullName(name),
        dob(dob),
        gender(gen),
        mobileNumber(mno),
        email(mail),
        address(addr),
        aadharNumber(aadhar),
        PANNumber(pan) {}
    
    void applyForLoan(string type, double amount, double rate, int years);
};

int Customer::nextLoanId = 10001;

class Transaction {
public:
    int transactionId;
    string transactionType;
    double amount;
    Date transactionDate;
    Account* senderAccount;
    Account* receiverAccount;
    string status;

    Transaction(int id, string type, double amt, Account* sender, Account* receiver) :
    transactionId(id),
    transactionType(type),
    amount(amt),
    senderAccount(sender),
    receiverAccount(receiver),
    status("Success") {
        transactionDate = Date(0,0,0);
    }
};

class Account {
public:
    long accountNumber;
    string accountType;
    double balance;
    Date dateOpened;
    string status;
    Branch* branch;
    Customer* customer;
    vector <Transaction*> transactions;

    static int nextTransactionId;

    Account(long uniqueNum, string type, Customer* cust, Branch* br, double start_balance) :
    accountNumber(uniqueNum),
    accountType(type),
    customer(cust),
    branch(br),
    balance(start_balance) {
        dateOpened = Date(0,0,0);
        status = "Active";
    }
    virtual ~Account() {}

    //DEPOSIT
    virtual void deposit(double amount) {
        if (status == "Blocked") {
            throw AccountBlockedException("Deposit failed " + to_string(accountNumber) + " is frozen.");
        }
        balance += amount;
        Transaction* t = new Transaction(nextTransactionId++, "Deposit", amount, nullptr, this);
        transactions.push_back(t);
        cout << "Successfully deposited " << amount << " into account " << accountNumber << endl;
    }

    //WITHDRAW
    virtual void withdraw(double amount) {
        if (status == "Blocked") {
            throw AccountBlockedException("Withdrawal failed. Account " + to_string(accountNumber) + " is frozen.");
        }
        if (amount > balance) {
            throw InsufficientBalanceException("Withdrawal failed. Requested " + to_string(amount) + " but available amount is " + to_string(balance));
        }
        balance -= amount;
        Transaction* t = new Transaction(nextTransactionId++, "Withdrawal", amount, this, nullptr);
        transactions.push_back(t);
        cout << "Successfully withdrew " << amount << " from Account " << accountNumber << endl;
    }

    //TRANSFER
    virtual void transfer(Account* receiver, double amount) {
        if (status == "Blocked") {
            throw AccountBlockedException("Transfer failed. Sender account " + to_string(accountNumber) + " is frozen.");
        }
        if (receiver->status == "Blocked") {
            throw AccountBlockedException("Transfer failed. Receiver account " + to_string(receiver->accountNumber) + " is frozen.");
        }
        if (amount > balance) {
            throw InsufficientBalanceException("Transfer failed. Needed " + to_string(amount) + " but available only " + to_string(balance));
        }
        balance -= amount;
        receiver->balance += amount;

        Transaction* t = new Transaction(nextTransactionId++, "Transfer", amount, this, receiver);
        this->transactions.push_back(t);
        receiver->transactions.push_back(t);

        cout << "Successfully transferred " << amount << " from Account " << accountNumber << " to Account " << receiver->accountNumber << endl;
    }
};

int Account::nextTransactionId = 1;

class SavingsAccount : public Account {
public:
    double interestRate;
    double minimumBalance;

    SavingsAccount(long uniqueNum, Customer* cust, Branch* br, double start_balance) :
        Account(uniqueNum, "Savings", cust, br, start_balance),
        interestRate(4.0), // Default 4%
        minimumBalance(1000.0) {}

        void withdraw(double amount) override {
            if(status == "Blocked") {
                throw AccountBlockedException("Withdraw failed. Account " + to_string(accountNumber) + " is frozen.");
            }
            if (balance - amount < minimumBalance) {
                throw InsufficientBalanceException("Withdraw denied. Savings accounts must maintain a minimum balance of " + to_string(minimumBalance));
            }
            balance -= amount;
            Transaction* t = new Transaction(nextTransactionId++, "Withdrawal", amount, this, nullptr);
            transactions.push_back(t);
            cout << "Successfully withdrew " << amount << " from Savings Account " << accountNumber << endl;
        }
};

class CurrentAccount : public Account {
public:
    double overdraftLimit;
    string businessName;

    CurrentAccount(long uniqueNum, Customer* cust, Branch* br, double start_balance) :
        Account(uniqueNum, "Current", cust, br, start_balance),
        overdraftLimit(50000.00),
        businessName("") {}

    void withdraw(double amount) override {
        if (status == "Blocked") {
            throw AccountBlockedException("Withdraw failed. Account " + to_string(accountNumber) + " is frozen.");
        }
        if (balance - amount < -overdraftLimit) {
            throw InsufficientBalanceException("Withdraw denied. Transaction exceeded overdraft limit of " + to_string(overdraftLimit));
        }

        balance -= amount;
        Transaction* t = new Transaction(nextTransactionId++, "Withdrawal", amount, this, nullptr);
        transactions.push_back(t);
        cout << "Successfully withdrew " << amount << " from Current Account " << to_string(accountNumber) << " (Current Balance: " << balance << ")" << endl;
    }
};

class FixedDepositAccount : public Account {
public:
    double FDAmount;
    Date maturityDate;
    double FDInterestRate;
    int tenureMonths;

    FixedDepositAccount(long uniqueNum, Customer* cust, Branch* br, double start_balance) :
        Account(uniqueNum, "FixedDeposit", cust, br, start_balance),
        FDAmount(start_balance),
        FDInterestRate(5),
        tenureMonths(12) {
            maturityDate = Date(0,0,0);
        }
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
    Customer* customer;

    Loan(int id, string type, double amt, double rate, int yrs, Customer* cust) :
    loanId(id),
    loanType(type),
    loanAmount(amt),
    interestRate(rate),
    tenureYears(yrs),
    loanStatus("Applied"),
    customer(cust) {
        double monthlyRate = rate/(12*100);
        int totalMonths = yrs*12;
        if (monthlyRate > 0) {
            EMIAmount = (amt * monthlyRate * pow(1 + monthlyRate, totalMonths))/(pow(1 + monthlyRate, totalMonths) - 1);
        }
        else {
            EMIAmount = amt / totalMonths;
        }
    }
};

void Customer::applyForLoan(string type, double amount, double rate, int years) {
bool trustable = false;
        for (Account* acc : accounts) {
            if (acc->status == "Active" && acc->balance > 0) {
                trustable = true;
                break;
            }
        }
        if (!trustable) {
            throw LoanRejectedException("Loan denied for " + fullName + ". Applicant doesn't have an active account with positive balance.");
        }
        Loan* newLoan = new Loan(nextLoanId++, type, amount, rate, years, this);
        newLoan->loanStatus = "Approved";
        loans.push_back(newLoan);

        cout << "LOAN APPLICATION APPROVED\nLoan ID: " << newLoan->loanId << " | Type: " << type << endl;
        cout << "Principal: " << amount << " over " << years << " years." << endl;
        cout << "Calculated Monthly EMI: $" << round(newLoan->EMIAmount * 100) / 100 << endl;
    }

class ATMCard {
    int failedAttempts;
public:
    long cardNumber;
    int CVV;
    Date expiryDate;
    int PIN;
    string cardType;
    string cardStatus;
    Account* linkedAccount;

    ATMCard(long cardnum, int cvv, Date expiry, int pin, string type, Account* acc) :
        cardNumber(cardnum),
        CVV(cvv),
        expiryDate(expiry),
        PIN(pin),
        cardType(type),
        cardStatus("Active"),
        linkedAccount(acc),
        failedAttempts(0) {}

    void authenticatePIN(int enteredPIN) {
        if (cardStatus == "Blocked" || linkedAccount->status == "Blocked") {
            throw AccountBlockedException("Access Denied. This card or its linked account is blocked.");
        }
        if (enteredPIN != PIN) {
            failedAttempts++;
        if (failedAttempts >= 3) {
            cardStatus = "Blocked";
            linkedAccount->status = "Blocked";
            throw AccountBlockedException("Security Alert: Too many incorrect PIN attempts. Card and Account have been BLOCKED.");
        }
        throw InvalidPINException("Incorrect PIN entered. Attempts remaining: " + to_string(3 - failedAttempts));
    }

        failedAttempts = 0;
        cout << "PIN Authentication Successful for Card No. " << cardNumber << endl;
    }
};

class Employee {
public:
    int employeeId;
    string employeeName;
    string designation;
    double salary;
    Branch* branch;
};

// POLYMORPHIC NOTIFICATION SYSTEM
class Notification {
protected:
    string message;
public:
    Notification(string message) {
        this->message = message;
    }
    virtual void sendNotification() = 0;
    virtual ~Notification() {}
};

class SMSNotification : public Notification {
    string phoneNumber;
    string deliveryStatus;
public:
SMSNotification(string phone, string msg) : Notification(msg), phoneNumber(phone), deliveryStatus("Pending") {}
    void sendNotification() override {
        cout << "Sending SMS to " << phoneNumber << ": " << message << endl;
        deliveryStatus = "Sent";
    }
};

class EmailNotification : public Notification {
    string emailAddress;
    string subject;
    string deliveryStatus;
public:
    EmailNotification(string email, string sub, string msg) : Notification(msg), emailAddress(email), subject(sub), deliveryStatus("Pending") {}
    void sendNotification() override {
        cout << "Sending Email to " << emailAddress << "\nSubject : " << subject << "\nBody: " << message << endl;
        deliveryStatus = "Sent";
    }
};

class AccountGenerator {
    private:
        static long nextAccountNumber;
    public:
        static Account* createAccount(string type, Customer* cust, Branch* br, double initialBalance) {
            long uniqueNum = nextAccountNumber++;
            Account* newAcc = nullptr;
            if (type == "Savings") {
                newAcc = new SavingsAccount(uniqueNum, cust, br, initialBalance);
            }
            else if (type == "Current") {
                newAcc = new CurrentAccount(uniqueNum, cust, br, initialBalance);
            }
            else {
                newAcc = new FixedDepositAccount(uniqueNum, cust, br, initialBalance);
            }
        if (newAcc != nullptr) {
            cust->accounts.push_back(newAcc);
            br->accounts.push_back(newAcc);
        }
        return newAcc;
    }
};

long AccountGenerator::nextAccountNumber = 10001;

int main() {
    return 0;
}