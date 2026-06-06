#include <iostream>
#include <string>
#include <list>
#include <stdexcept>

using namespace std;

class Branch;
class Customer;
class Account;

class InsufficientBalanceException : public runtime_error {
public:
    InsufficientBalanceException(const string& msg) : runtime_error(msg) { }
};

class InvalidPINException : public runtime_error {
public:
    InvalidPINException(const string& msg) : runtime_error(msg) { }
};

class AccountBlockedException : public runtime_error {
public:
    AccountBlockedException(const string& msg) : runtime_error(msg) { }
};

class LoanRejectedException : public runtime_error {
public:
    LoanRejectedException(const string& msg) : runtime_error(msg) { }
};

class Notification
{
protected:
    string message;
public:
    Notification(string msg) : message(msg) { }
    virtual ~Notification() {}
    virtual void send() = 0;
};

class SMSNotification : public Notification
{
private:
    string phoneNumber;
    string deliveryStatus;
public:
    SMSNotification(string phone, string msg)
        : Notification(msg), phoneNumber(phone), deliveryStatus("Pending") {
    }

    void send()
    {
        deliveryStatus = "Sent";
        cout << "[SMS Alert to " << phoneNumber << "]: " << message << " (Status: " << deliveryStatus << ")\n";
    }
};

class EmailNotification : public Notification
{
private:
    string emailAddress;
    string subject;
    string deliveryStatus;
public:
    EmailNotification(string email, string subj, string msg)
        : Notification(msg), emailAddress(email), subject(subj), deliveryStatus("Pending") {
    }

    void send()
    {
        deliveryStatus = "Delivered";
        cout << "[Email Alert to " << emailAddress << "] Subject: " << subject << "\nBody: " << message << "\n";
    }
};

class Transaction
{
public:
    int transactionId;
    string transactionType;
    double amount;
    string transactionDate;
    Account* senderAccount;
    Account* receiverAccount;
    string status;

    Transaction(int id, string type, double amt, Account* src, Account* dest, string stat)
    {
        transactionId = id;
        transactionType = type;
        amount = amt;
        senderAccount = src;
        receiverAccount = dest;
        transactionDate = "19-05-2026";
        status = stat;
    }
};

class Loan
{
protected:
    // Protected financial values to prevent unauthorized modification
    double loanAmount;
    double interestRate;
    double EMIAmount;

public:
    int loanId;
    string loanType;
    int tenureYears;
    string loanStatus;
    Customer* customer;

    Loan(int id, string type, double amt, double rate, int years, Customer* cust)
    {
        if (amt > 10000000)
        {
            throw LoanRejectedException("Loan Request Rejected: Exceeds Maximum Branch Risk Allowance.");
        }
        loanId = id;
        loanType = type;
        loanAmount = amt;
        interestRate = rate;
        tenureYears = years;
        customer = cust;
        loanStatus = "Approved";
        calculateEMI();
    }

    void calculateEMI()
    {
        double totalAmount = loanAmount + (loanAmount * (interestRate / 100) * tenureYears);
        EMIAmount = totalAmount / (tenureYears * 12);
    }

    // Public Getters for Read-Only parameters
    double getLoanAmount() const { return loanAmount; }
    double getInterestRate() const { return interestRate; }
    double getEMIAmount() const { return EMIAmount; }
};

class ATMCard
{
private:
    int PIN;
public:
    long cardNumber;
    int CVV;
    string expiryDate;
    string cardType;
    string cardStatus;
    Account* linkedAccount;

    ATMCard(long cardNum, int c, int p, string type, Account* acc) {
        cardNumber = cardNum;
        CVV = c;
        PIN = p;
        cardType = type;
        linkedAccount = acc;
        expiryDate = "12/2030";
        cardStatus = "Active";
    }

    void verifyPIN(int inputPIN)
    {
        if (cardStatus == "Blocked") {
            throw AccountBlockedException("ATM Card Action Refused: This card is currently blocked.");
        }
        if (PIN != inputPIN) {
            throw InvalidPINException("Security Alert: Invalid ATM PIN Code Entered.");
        }
        cout << "PIN Verified Successfully!\n";
    }
};

class Employee
{
public:
    int employeeId;
    string employeeName;
    string designation;
    double salary;
    Branch* branch;

    Employee(int id, string name, string desig, double sal, Branch* br) {
        employeeId = id;
        employeeName = name;
        designation = desig;
        salary = sal;
        branch = br;
    }
};

class Account
{
protected:
    double balance;

public:
    long accountNumber;
    string accountType;
    string dateOpened;
    string status;
    Branch* branch;
    Customer* customer;
    list<Transaction> transactions;

    Account(long accNum, string type, double initialBalance, Branch* br, Customer* cust)
    {
        accountNumber = accNum;
        accountType = type;
        balance = initialBalance;
        branch = br;
        customer = cust;
        dateOpened = "19-05-2026";
        status = "Active";
    }

    virtual ~Account() {}

    double getBalance() const 
    {
        return balance;
    }

    void deposit(double amount)
    {
        if (status == "Blocked")
        {
            throw AccountBlockedException("Deposit Denied: Target account is currently blocked.");
        }
        balance += amount;
        transactions.push_back(Transaction(2001, "Deposit", amount, nullptr, this, "Success"));
    }

    virtual void withdraw(double amount) = 0;
};

class SavingsAccount : public Account
{
protected:
    double interestRate;
    double minimumBalance;

public:
    SavingsAccount(long accNum, double initialBalance, Branch* br, Customer* cust)
        : Account(accNum, "Savings", initialBalance, br, cust)
    {
        interestRate = 4.0;
        minimumBalance = 1000.0;
    }

    void withdraw(double amount) override
    {
        if (status == "Blocked") throw AccountBlockedException("Withdrawal Denied: Account is Blocked.");
        if (balance - amount < minimumBalance)
        {
            transactions.push_back(Transaction(2002, "Withdrawal Failed", amount, this, nullptr, "Failed"));
            throw InsufficientBalanceException("Transaction Denied: Account must maintain a minimum balance of INR " + to_string(minimumBalance));
        }
        balance -= amount;
        transactions.push_back(Transaction(2003, "Withdrawal", amount, this, nullptr, "Success"));
    }

    // Public getters for sensitive regulatory metrics
    double getInterestRate() const { return interestRate; }
    double getMinimumBalance() const { return minimumBalance; }
};

class CurrentAccount : public Account
{
protected:
    double overdraftLimit;

public:
    string businessName;

    CurrentAccount(long accNum, double initialBalance, Branch* br, Customer* cust, string bName)
        : Account(accNum, "Current", initialBalance, br, cust)
    {
        overdraftLimit = 10000.0;
        businessName = bName;
    }

    void withdraw(double amount) override
    {
        if (status == "Blocked") throw AccountBlockedException("Withdrawal Denied: Account is Blocked.");
        if (balance + overdraftLimit < amount)
        {
            transactions.push_back(Transaction(2004, "Withdrawal Failed", amount, this, nullptr, "Failed"));
            throw InsufficientBalanceException("Transaction Denied: Withdrawal amount exceeds total available Overdraft Limit.");
        }
        balance -= amount;
        transactions.push_back(Transaction(2005, "Withdrawal", amount, this, nullptr, "Success"));
    }

    double getOverdraftLimit() const { return overdraftLimit; }
};

class FixedDepositAccount : public Account
{
protected:
    double FDAmount;
    double FDInterestRate;

public:
    string maturityDate;
    int tenureMonths;

    FixedDepositAccount(long accNum, double initialBalance, Branch* br, Customer* cust, int months)
        : Account(accNum, "Fixed Deposit", initialBalance, br, cust)
    {
        FDAmount = initialBalance;
        tenureMonths = months;
        FDInterestRate = 7.5;
        maturityDate = "19-05-2027";
    }

    void withdraw(double amount) override
    {
        throw runtime_error("Invalid Operations: Premature withdrawals on Term Fixed Deposits cannot be processed via basic channels.");
    }

    double getFDAmount() const { return FDAmount; }
    double getFDInterestRate() const { return FDInterestRate; }
};

class Customer
{
public:
    int customerId;
    string fullName;
    string dob;
    string gender;
    string mobileNumber;
    string email;
    string address;
    string aadhaarNumber;
    string PANNumber;
    list<Account*> accounts;
    list<Loan*> loans;

    Customer(int id, string name, string phone, string mail)
    {
        customerId = id;
        fullName = name;
        mobileNumber = phone;
        email = mail;
        dob = "01-01-2000";
        gender = "M";
        address = "IIT Kanpur Campus";
        aadhaarNumber = "[Aadhaar Redacted]";
        PANNumber = "ABCDE1234Z";
    }
};

class Branch
{
public:
    int branchId;
    string branchName;
    string IFSCCode;
    string address;
    list<Account*> accounts;
    list<Employee*> employees;

    Branch(int id, string name, string ifsc, string addr)
    {
        branchId = id;
        branchName = name;
        IFSCCode = ifsc;
        address = addr;
    }
};

class Bank
{
public:
    int bankId;
    string bankName;
    list<Branch*> branches;
    list<Customer*> customers;
    list<Employee*> employees;

    Bank(int id, string name)
    {
        bankId = id;
        bankName = name;
    }

    void transferFunds(Account* from, Account* to, double amount)
    {
        if (from->status == "Blocked" || to->status == "Blocked")
        {
            throw AccountBlockedException("Transfer Failed: One or both accounts involved are currently Blocked.");
        }

        from->withdraw(amount);
        to->deposit(amount);

        cout << "[System Transfer Success]: Moved INR " << amount << " between Accounts.\n";

        SMSNotification sms(from->customer->mobileNumber, "Your account was debited.");
        sms.send();

        EmailNotification email(to->customer->email, "Credit Alert", "Your account was credited.");
        email.send();
    }
};

int main()
{
    Bank coreBank(1, "ChingChong Corporate");
    Branch* cityBranch = new Branch(301, "IITK Branch", "SBIN0001161", "IIT Kanpur");
    coreBank.branches.push_back(cityBranch);

    Customer* user1 = new Customer(501, "Alice Smith", "+919876543210", "alice@iitk.ac.in");
    Customer* user2 = new Customer(502, "Bob Jones", "+918765432109", "bob@iitk.ac.in");
    coreBank.customers.push_back(user1);
    coreBank.customers.push_back(user2);

    Account* aliceSavings = new SavingsAccount(99901, 3000.0, cityBranch, user1);
    Account* bobCurrent = new CurrentAccount(99902, 1000.0, cityBranch, user2, "Bob Logistics");

    user1->accounts.push_back(aliceSavings);
    user2->accounts.push_back(bobCurrent);

    ATMCard* aliceCard = new ATMCard(123456789012LL, 412, 4321, "Debit", aliceSavings);

    // Testing and verifying display functionality via public getters
    cout << "--- Initialization Check: Verified through getter interfaces ---\n";
    cout << "Alice Savings Balance: INR " << aliceSavings->getBalance() << "\n";

    // Using static_cast to access subclass-specific protected fields safely through public getters
    SavingsAccount* savingsPtr = static_cast<SavingsAccount*>(aliceSavings);
    cout << "Savings Minimum Balance Restriction: INR " << savingsPtr->getMinimumBalance() << "\n";
    cout << "Savings Base Yield Interest Rate: " << savingsPtr->getInterestRate() << "%\n\n";

    // Triggering an Insufficient Balance Exception
    cout << "--- Transaction Test 1: Intentionally dropping below Minimum Savings Balance ---\n";
    try {
        aliceSavings->withdraw(2500.0);
    }
    catch (const InsufficientBalanceException& e) {
        cout << "Caught Expected Exception: [ " << e.what() << " ]\n\n";
    }

    // Triggering an Invalid PIN Exception
    cout << "--- Transaction Test 2: Simulating Wrong Card PIN Input ---\n";
    try {
        aliceCard->verifyPIN(0000);
    }
    catch (const InvalidPINException& e) {
        cout << "Caught Expected Exception: [ " << e.what() << " ]\n\n";
    }

    // Triggering an Account Blocked Exception
    cout << "--- Transaction Test 3: Processing transactions on Blocked Accounts ---\n";
    bobCurrent->status = "Blocked";
    try
    {
        coreBank.transferFunds(aliceSavings, bobCurrent, 200.0);
    }
    catch (const AccountBlockedException& e)
    {
        cout << "Caught Expected Exception: [ " << e.what() << " ]\n\n";
    }

    // Triggering a Loan Rejected Exception
    cout << "--- Transaction Test 4: Requesting High Risk Corporate Loans ---\n";
    try
    {
        Loan* extremeLoan = new Loan(7005, "Business Growth Loan", 550000000.0, 10.5, 7, user1);
    }
    catch (const LoanRejectedException& e)
    {
        cout << "Caught Expected Exception: [ " << e.what() << " ]\n\n";
    }

    delete cityBranch; delete user1; delete user2;
    delete aliceSavings; delete bobCurrent; delete aliceCard;

    return 0;
}