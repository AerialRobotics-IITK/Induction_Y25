#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <vector>

using namespace std;
using TimePoint = std::chrono::system_clock::time_point;

class InsufficientBalanceException : public exception {
public: string message; InsufficientBalanceException(string msg) : message(msg) {}
};
class InvalidPINException : public exception {
public: string message; InvalidPINException(string msg) : message(msg) {}
};
class AccountBlockedException : public exception {
public: string message; AccountBlockedException(string msg) : message(msg) {}
};
class LoanRejectedException : public exception {
public: string message; LoanRejectedException(string msg) : message(msg) {}
};

class Branch;
class Customer;
class Employee;
class Transaction;
class Loan;
class ATMCard;

// Strategy Interface
class Notification {
public:
    string message;
    virtual ~Notification() = default;
    virtual void send() = 0;
};

// strategy 1
class SMSNotification : public Notification {
public:
    string phoneNumber;
    string deliveryStatus;
    SMSNotification(string phone) : phoneNumber(phone) { deliveryStatus = "Pending"; }
    void send() override { 
        deliveryStatus = "Sent";
        cout << "  [SMS to " << phoneNumber << "]: " << message << "\n"; 
    }
};

// strategy 2
class EmailNotification : public Notification {
public:
    string emailAddress;
    string subject;
    string deliveryStatus;
    EmailNotification(string email, string subj) : emailAddress(email), subject(subj) { deliveryStatus = "Pending"; }
    void send() override { 
        deliveryStatus = "Sent";
        cout << "  [Email to " << emailAddress << " | " << subject << "]: " << message << "\n"; 
    }
};

class Account {
protected:
    static long nextAccountNumber;
    long accountNumber;
    string accountType;
    double balance;
    TimePoint dateOpened;
    string status;
    Branch* branch;
    Customer* customer;
    vector<Transaction*> transactions;
    Notification* notificationStrategy; 

public:
    Account(string type, double bal) : accountNumber(++nextAccountNumber), accountType(type), balance(bal), dateOpened(std::chrono::system_clock::now()), status("Active"), branch(nullptr), customer(nullptr), notificationStrategy(nullptr) {}
    virtual ~Account() { delete notificationStrategy; }

    virtual string getAccountType() const = 0;
    virtual void withdraw(double amount) = 0;
    virtual void deposit(double amount) { balance += amount; }

    long getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }
    const string& getStatus() const { return status; }
    Branch* getBranch() const { return branch; }
    Customer* getCustomer() const { return customer; }
    
    void setStatus(const string& newStatus) { status = newStatus; }
    void setBranch(Branch* b) { branch = b; }
    void setCustomer(Customer* c) { customer = c; }
    void addTransaction(Transaction* t) { transactions.push_back(t); }
    void adjustBalance(double delta) { balance += delta; }
    
    // set a strategy
    void setNotificationStrategy(Notification* strategy) {
        if (notificationStrategy) delete notificationStrategy;
        notificationStrategy = strategy;
    }

    void notify(const string& msg) {
        if (notificationStrategy) {
            notificationStrategy->message = msg;
            notificationStrategy->send();
        }
    }
};
long Account::nextAccountNumber = 100000;

class SavingsAccount : public Account {
public:
    double interestRate;
    double minimumBalance;
    SavingsAccount(double bal, double interest, double minBal) : Account("Savings", bal), interestRate(interest), minimumBalance(minBal) {}
    string getAccountType() const override { return "Savings"; }
    void withdraw(double amount) override {
        if (getBalance() - amount < minimumBalance) throw InsufficientBalanceException("Insufficient balance to maintain minimum balance");
        adjustBalance(-amount);
    }
};

class CurrentAccount : public Account {
public:
    double overdraftLimit;
    string businessName;
    CurrentAccount(double bal, double overdraft, string business) : Account("Current", bal), overdraftLimit(overdraft), businessName(business) {}
    string getAccountType() const override { return "Current"; }
    void withdraw(double amount) override {
        if (getBalance() - amount < -overdraftLimit) throw InsufficientBalanceException("Overdraft limit exceeded");
        adjustBalance(-amount);
    }
};

class FixedDepositAccount : public Account {
public:
    double FDAmount;
    TimePoint maturityDate;
    double FDInterestRate;
    int tenureMonths;
    FixedDepositAccount(double amount, TimePoint maturity, double interest, int tenure) : Account("FixedDeposit", amount), FDAmount(amount), maturityDate(maturity), FDInterestRate(interest), tenureMonths(tenure) {}
    string getAccountType() const override { return "FixedDeposit"; }
    void withdraw(double amount) override {
        if (std::chrono::system_clock::now() < maturityDate) throw AccountBlockedException("Cannot withdraw before maturity date");
        if (getBalance() - amount < 0) throw InsufficientBalanceException("Insufficient balance");
        adjustBalance(-amount);
    }
};
// Factory Pattern for Account Creation
class AccountFactory {
public:
    static Account* createSavingsAccount(double balance, double interestRate, double minimumBalance) {
        return new SavingsAccount(balance, interestRate, minimumBalance);
    }
    static Account* createCurrentAccount(double balance, double overdraftLimit, string businessName) {
        return new CurrentAccount(balance, overdraftLimit, businessName);
    }
    static Account* createFixedDepositAccount(double amount, TimePoint maturityDate, double interestRate, int tenureMonths) {
        return new FixedDepositAccount(amount, maturityDate, interestRate, tenureMonths);
    }
};

class Branch {
public:
    int branchId;
    string branchName;
    string address;
    string IFSCCode;
    vector<Account*> accounts;
    vector<Employee*> employees;
    Branch(int id, string name, string addr, string ifsc) : branchId(id), branchName(name), address(addr), IFSCCode(ifsc) {}
};

class Customer {
public:
    static int idCounter;
    int customerId;
    string fullName;
    TimePoint dob;
    string gender;
    string mobileNumber;
    string email;
    string address;
    string aadhaarNumber;
    string PANNumber;
    vector<Account*> accounts;
    vector<ATMCard*> cards;
    vector<Loan*> loans;

    Customer(string name, TimePoint dob, string gender, string mobile, string email, string address, string aadhaar, string pan) 
        : customerId(++idCounter), fullName(name), dob(dob), gender(gender), mobileNumber(mobile), email(email), address(address), aadhaarNumber(aadhaar), PANNumber(pan) {}
};
int Customer::idCounter = 0;

class Employee {
public:
    int employeeId;
    string employeeName;
    string designation;
    double salary;
    Branch* branch;
    Employee(int id, string name, string desig, double sal, Branch* b) : employeeId(id), employeeName(name), designation(desig), salary(sal), branch(b) {}
};

class Transaction {
public:
    int transactionId;
    string transactionType;
    double amount;
    TimePoint transactionDate;
    Account* senderAccount;
    Account* receiverAccount;
    string status;
    static int nextTxId;
    Transaction(string type, double amt, Account* sender, Account* receiver, string stat)
        : transactionId(++nextTxId), transactionType(type), amount(amt), transactionDate(std::chrono::system_clock::now()), senderAccount(sender), receiverAccount(receiver), status(stat) {}
};
int Transaction::nextTxId = 0;

class Loan {
public:
    static int nextLoanId;
    int loanId;
    string loanType;
    double loanAmount;
    double interestRate;
    int tenureYears;
    double EMIAmount;
    string loanStatus;
    Customer* customer;
    Loan(string type, double amt, double rate, int tenure, double emi, Customer* cust)
        : loanId(++nextLoanId), loanType(type), loanAmount(amt), interestRate(rate), tenureYears(tenure), EMIAmount(emi), loanStatus("Approved"), customer(cust) {}
};
int Loan::nextLoanId = 0;

class ATMCard {
public:
    static long nextCardNumber;
    long cardNumber;
    int CVV;
    TimePoint expiryDate;
    int PIN;
    string cardType;
    string cardStatus;
    Account* linkedAccount;

    ATMCard(TimePoint expiry, int pin, string type, Account* acc)
        : cardNumber(++nextCardNumber), CVV(100 + (nextCardNumber % 900)), expiryDate(expiry), PIN(pin), cardType(type), cardStatus("Active"), linkedAccount(acc) {}

    bool isActive() const { return cardStatus == "Active" && std::chrono::system_clock::now() < expiryDate; }
    
    bool validatePIN(int inputPin) const {
        if (!isActive()) throw AccountBlockedException("Card is blocked or expired");
        if (PIN != inputPin) throw InvalidPINException("Invalid PIN");
        return true;
    }
};
long ATMCard::nextCardNumber = 4000000000000000;

class TransactionManager {
public:
    static void execute(string type, Account* from, Account* to, double amt) {
        if (from && from->getStatus() == "Closed") throw AccountBlockedException("Sender account is closed");
        if (to && to->getStatus() == "Closed") throw AccountBlockedException("Receiver account is closed");

        if (from) {
            from->withdraw(amt);
            from->notify("Debit Alert: " + type + " of " + to_string(amt) + " successful. Balance: " + to_string(from->getBalance()));
        }
        if (to) {
            to->deposit(amt);
            to->notify("Credit Alert: " + type + " of " + to_string(amt) + " successful. Balance: " + to_string(to->getBalance()));
        }

        Transaction* t = new Transaction(type, amt, from, to, "Success");
        if (from) from->addTransaction(t);
        if (to) to->addTransaction(t);
    }
};

class Bank {
public:
    int bankId;
    string bankName;
    vector<Branch*> branches;
    vector<Account*> accounts;
    vector<Customer*> customers;
    vector<Employee*> employees;

    Bank(int id, string name) : bankId(id), bankName(name) {}

    void addBranch(Branch* b) { branches.push_back(b); }
    void addCustomer(Customer* c) { customers.push_back(c); }

    Account* linkAccount(Account* acc, Customer* cust, Branch* branch) {
        acc->setCustomer(cust);
        acc->setBranch(branch);
        accounts.push_back(acc);
        branch->accounts.push_back(acc);
        cust->accounts.push_back(acc);
        return acc;
    }

    Loan* applyLoan(Customer* cust, string type, double amt, double rate, int tenure) {
        if (amt > 5000000) throw LoanRejectedException("Amount exceeds maximum limit of 50 Lakhs");
        double monthlyRate = rate / 1200.0;
        double emi = (monthlyRate == 0) ? (amt / (tenure * 12)) : (amt * monthlyRate * pow(1 + monthlyRate, tenure * 12) / (pow(1 + monthlyRate, tenure * 12) - 1));
        Loan* loan = new Loan(type, amt, rate, tenure, emi, cust);
        cust->loans.push_back(loan);
        return loan;
    }
};

void runTests() {
    int passed = 0, failed = 0;
    auto assertTest = [&](bool condition, const string& testName) {
        if (condition) { cout << "[PASS] " << testName << "\n"; passed++; }
        else { cout << "[FAIL] " << testName << "\n"; failed++; }
    };
    //helper lambda func to count pass and fail

    cout << "=== Initializing SmartBank Enterprise System ===\n";
    Bank bank(1, "SmartBank");
    Branch* mainBranch = new Branch(1, "Main", "123 Tech Park", "SMTB0001");
    bank.addBranch(mainBranch);
    
    Customer* cust1 = new Customer("Alice Smith", TimePoint{}, "F", "9876543210", "alice@email.com", "45 Avenue", "[Aadhaar Redacted]", "ABCDE1234F");
    Customer* cust2 = new Customer("Bob Jones", TimePoint{}, "M", "9123456789", "bob@email.com", "89 Boulevard", "[Aadhaar Redacted]", "XYZAB9876C");
    bank.addCustomer(cust1);
    bank.addCustomer(cust2);

    Account* savings = bank.linkAccount(AccountFactory::createSavingsAccount(1000.0, 4.0, 500.0), cust1, mainBranch);
    Account* current = bank.linkAccount(AccountFactory::createCurrentAccount(5000.0, 2000.0, "Alice Tech"), cust1, mainBranch);
    TimePoint futureDate = std::chrono::system_clock::now() + std::chrono::hours(24 * 365);
    Account* fd = bank.linkAccount(AccountFactory::createFixedDepositAccount(10000.0, futureDate, 7.0, 12), cust2, mainBranch);
    
    // Strategy Pattern Setup
    savings->setNotificationStrategy(new SMSNotification(cust1->mobileNumber));
    current->setNotificationStrategy(new EmailNotification(cust1->email, "SmartBank Transaction Alert"));

    cout << "\n=== Testing Transactions (Automated Strategy Notifications) ===\n";
    TransactionManager::execute("Deposit", nullptr, savings, 500.0);
    assertTest(savings->getBalance() == 1500.0, "Standard deposit successful");

    TransactionManager::execute("Transfer", savings, current, 200.0);
    assertTest(savings->getBalance() == 1300.0 && current->getBalance() == 5200.0, "Fund transfer between accounts successful");

    cout << "\n=== Testing Constraints & Exceptions ===\n";
    try {
        TransactionManager::execute("Withdraw", savings, nullptr, 1000.0);
        assertTest(false, "Savings minimum balance constraint failed");
    } catch (const InsufficientBalanceException&) {
        assertTest(true, "Savings minimum balance constraint enforced");
    }

    try {
        TransactionManager::execute("Withdraw", current, nullptr, 6000.0);
        assertTest(current->getBalance() == -800.0, "Current account overdraft utilized properly");
    } catch (const InsufficientBalanceException&) {
        assertTest(false, "Current account overdraft failed");
    }

    try {
        TransactionManager::execute("Withdraw", fd, nullptr, 1000.0);
        assertTest(false, "Fixed Deposit allowed premature withdrawal");
    } catch (const AccountBlockedException&) {
        assertTest(true, "Fixed Deposit premature withdrawal blocked");
    }

    cout << "\n=== Testing ATM Card Logic ===\n";
    TimePoint validExpiry = std::chrono::system_clock::now() + std::chrono::hours(24 * 365 * 3);
    ATMCard* card = new ATMCard(validExpiry, 1234, "Debit", savings);
    
    assertTest(card->validatePIN(1234), "ATM Card valid PIN accepted");
    
    try {
        card->validatePIN(9999);
        assertTest(false, "ATM Card invalid PIN failed to throw");
    } catch (const InvalidPINException&) {
        assertTest(true, "ATM Card invalid PIN rejected");
    }

    card->cardStatus = "Blocked";
    try {
        card->validatePIN(1234);
        assertTest(false, "Blocked ATM Card failed to throw");
    } catch (const AccountBlockedException&) {
        assertTest(true, "Blocked ATM Card transactions rejected");
    }

    cout << "\n=== Testing Loan System ===\n";
    Loan* loan = bank.applyLoan(cust1, "Home", 1000000.0, 8.5, 10);
    assertTest(loan->loanStatus == "Approved" && loan->EMIAmount > 10000, "Valid loan application approved and EMI calculated");

    try {
        bank.applyLoan(cust1, "Business", 6000000.0, 10.0, 5);
        assertTest(false, "Loan exceeding max limit was approved");
    } catch (const LoanRejectedException&) {
        assertTest(true, "Loan exceeding max limit was rejected");
    }

    cout << "\n=== Test Summary ===\n";
    cout << passed << " Passed | " << failed << " Failed\n";
    if (failed > 0) throw runtime_error("Test suite failed.");
}

int main() {
    try {
        runTests();
    } catch (const exception& e) {
        cout << "Fatal System Error: " << e.what() << "\n";
    }
    return 0;
}