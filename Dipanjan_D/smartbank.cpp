#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <exception>

using namespace std;

//Exception classes

class InsufficientBalanceException : public exception {
public:
    const char* what() const noexcept override {
        return "Insufficient balance";
    }
};

class InvalidPINException : public exception {
public:
    const char* what() const noexcept override {
        return "Invalid PIN";
    }
};

class AccountBlockedException : public exception {
public:
    const char* what() const noexcept override {
        return "Account or Card is blocked";
    }
};

class LoanRejectedException : public exception {
public:
    const char* what() const noexcept override {
        return "Loan rejected";
    }
};

//Notification System

class Notification {
public:
    virtual void send(string message) = 0;
    virtual ~Notification() {}
};

class SMSNotification : public Notification {
    string phoneNumber;

public:
    SMSNotification(string phone) {
        phoneNumber = phone;
    }

    void send(string message) override {
        cout << "SMS to " << phoneNumber << ": " << message << endl;
    }
};

class EmailNotification : public Notification {
    string emailAddress;

public:
    EmailNotification(string email) {
        emailAddress = email;
    }

    void send(string message) override {
        cout << "Email to " << emailAddress << ": " << message << endl;
    }
};

//Account Hierarchy

class Account {
protected:
    long accountNumber;
    double balance;
    string status;

public:
    Account(long accNo, double bal) {
        accountNumber = accNo;
        balance = bal;
        status = "Active";
    }

    virtual void deposit(double amount) {
        if (status == "Blocked") {
            throw AccountBlockedException();
        }
        balance += amount;
    }

    virtual void withdraw(double amount) = 0;

    double getBalance() {
        return balance;
    }

    string getStatus() {
        return status;
    }

    void blockAccount() {
        status = "Blocked";
    }

    virtual void display() = 0;

    virtual ~Account() {}
};

class SavingsAccount : public Account {
    double minimumBalance;
    double interestRate;

public:
    SavingsAccount(long accNo, double bal)
        : Account(accNo, bal) {
        minimumBalance = 1000;
        interestRate = 4.0;
    }

    void withdraw(double amount) override {
        if (status == "Blocked") {
            throw AccountBlockedException();
        }

        if (balance - amount < minimumBalance) {
            throw InsufficientBalanceException();
        }

        balance -= amount;
    }

    void display() override {
        cout << "Savings Account\n";
        cout << "Account No: " << accountNumber << endl;
        cout << "Balance: " << balance << endl;
        cout << "Interest Rate: " << interestRate << "%" << endl;
    }
};

class CurrentAccount : public Account {
    double overdraftLimit;

public:
    CurrentAccount(long accNo, double bal)
        : Account(accNo, bal) {
        overdraftLimit = 5000;
    }
 
    void withdraw(double amount) override {
        if (status == "Blocked") {
            throw AccountBlockedException();
        }

        if (balance + overdraftLimit < amount) {
            throw InsufficientBalanceException();
        }

        balance -= amount;
    }

    void display() override {
        cout << "Current Account\n";
        cout << "Account No: " << accountNumber << endl;
        cout << "Balance: " << balance << endl;
        cout << "Overdraft Limit: " << overdraftLimit << endl;
    }
};

class FixedDepositAccount : public Account {
    int tenureMonths;
    double interestRate;

public:
    FixedDepositAccount(long accNo, double bal, int tenure)
        : Account(accNo, bal) {
        tenureMonths = tenure;
        interestRate = 6.5;
    }

    void withdraw(double amount) override {
        throw AccountBlockedException();
    }

    double calculateMaturityAmount() {
        return balance + (balance * interestRate * tenureMonths) / (12 * 100);
    }

    void display() override {
        cout << "Fixed Deposit Account\n";
        cout << "Account No: " << accountNumber << endl;
        cout << "Deposit Amount: " << balance << endl;
        cout << "Tenure: " << tenureMonths << " months" << endl;
        cout << "Maturity Amount: " << calculateMaturityAmount() << endl;
    }
};

//Loan

class Loan {
    string loanType;
    double loanAmount;
    double interestRate;
    int tenureYears;
    double emi;

public:
    Loan(string type, double amount, double rate, int years) {
        loanType = type;
        loanAmount = amount;
        interestRate = rate;
        tenureYears = years;

        double monthlyRate = interestRate / (12 * 100);
        int months = tenureYears * 12;

        emi = loanAmount * monthlyRate *
              pow(1 + monthlyRate, months) /
              (pow(1 + monthlyRate, months) - 1);
    }

    double getEMI() {
        return emi;
    }

    void displayLoan() {
        cout << "Loan Type: " << loanType << endl;
        cout << "Loan Amount: " << loanAmount << endl;
        cout << "EMI: " << emi << endl;
    }
};

//Customer

class Customer {
    int customerId;
    string fullName;
    string mobileNumber;
    string email;

    vector<Account*> accounts;
    vector<Loan*> loans;

public:
    Customer(int id, string name, string mobile, string mail) {
        customerId = id;
        fullName = name;
        mobileNumber = mobile;
        email = mail;
    }

    void addAccount(Account* acc) {
        accounts.push_back(acc);
    }

    void addLoan(Loan* loan) {
        loans.push_back(loan);
    }

    string getMobile() {
        return mobileNumber;
    }

    string getEmail() {
        return email;
    }

    void showAccounts() {
        cout << "\nCustomer Accounts:\n";
        for (int i = 0; i < accounts.size(); i++) {
            accounts[i]->display();
            cout << "----------------------\n";
        }
    }
};
//Employee


class Employee {
    int employeeId;
    string employeeName;
    string designation;
    double salary;

public:
    Employee(int id, string name, string role, double sal) {
        employeeId = id;
        employeeName = name;
        designation = role;
        salary = sal;
    }

    void displayEmployee() {
        cout << "Employee ID: " << employeeId << endl;
        cout << "Name: " << employeeName << endl;
        cout << "Designation: " << designation << endl;
        cout << "Salary: " << salary << endl;
    }
};

//Branch

class Branch {
    int branchId;
    string branchName;
    string ifscCode;

    vector<Account*> accounts;
    vector<Employee*> employees;

public:
    Branch(int id, string name, string ifsc) {
        branchId = id;
        branchName = name;
        ifscCode = ifsc;
    }

    void addAccount(Account* acc) {
        accounts.push_back(acc);
    }

    void addEmployee(Employee* emp) {
        employees.push_back(emp);
    }

    void displayBranch() {
        cout << "\nBranch Details\n";
        cout << "Branch ID: " << branchId << endl;
        cout << "Branch Name: " << branchName << endl;
        cout << "IFSC Code: " << ifscCode << endl;
    }
};

//Bank

class Bank {
    string bankName;

    vector<Branch*> branches;
    vector<Customer*> customers;

public:
    Bank(string name) {
        bankName = name;
    }

    void addBranch(Branch* branch) {
        branches.push_back(branch);
    }

    void addCustomer(Customer* customer) {
        customers.push_back(customer);
    }

    void displayBank() {
        cout << "\nBank Name: " << bankName << endl;
    }
};

//Transaction

class Transaction {
public:
    static void deposit(Account* acc, double amount, Notification* notify) {
        acc->deposit(amount);

        if (notify != NULL) {
            notify->send("Deposit successful");
        }
    }

    static void withdraw(Account* acc, double amount, Notification* notify) {
        acc->withdraw(amount);

        if (notify != NULL) {
            notify->send("Withdrawal successful");
        }
    }

    static void transfer(Account* sender,
                         Account* receiver,
                         double amount,
                         Notification* notify) {
        sender->withdraw(amount);
        receiver->deposit(amount);

        if (notify != NULL) {
            notify->send("Transfer successful");
        }
    }
};

//Loan Service

class LoanService {
public:
    static void approveLoan(Loan* loan, double monthlyIncome) {
        if (loan->getEMI() > monthlyIncome * 0.4) {
            throw LoanRejectedException();
        }

        cout << "Loan approved successfully.\n";
    }
};

//ATM Card

class ATMCard {
    long cardNumber;
    int pin;
    string status;
    int wrongAttempts;
    Account* linkedAccount;

public:
    ATMCard(long number, int enteredPin, Account* acc) {
        cardNumber = number;
        pin = enteredPin;
        linkedAccount = acc;
        status = "Active";
        wrongAttempts = 0;
    }

    void withdrawCash(int enteredPin, double amount) {
        if (status == "Blocked") {
            throw AccountBlockedException();
        }

        if (enteredPin != pin) {
            wrongAttempts++;

            if (wrongAttempts >= 3) {
                status = "Blocked";
            }

            throw InvalidPINException();
        }

        linkedAccount->withdraw(amount);

        cout << "ATM withdrawal successful.\n";
    }
};

//Account Factory

class AccountFactory {
public:
    static Account* createAccount(string type,
                                  long accNo,
                                  double balance) {
        if (type == "Savings") {
            return new SavingsAccount(accNo, balance);
        }

        if (type == "Current") {
            return new CurrentAccount(accNo, balance);
        }

        return NULL;
    }

    static Account* createFD(long accNo,
                             double balance,
                             int tenure) {
        return new FixedDepositAccount(accNo, balance, tenure);
    }
};

//Main function

int main() {
        Bank bank("SmartBank");

        Branch* mainBranch =
            new Branch(101, "Main Branch", "SBIN000123");

        bank.addBranch(mainBranch);

        Employee* manager =
            new Employee(1, "Rahul", "Manager", 60000);

        mainBranch->addEmployee(manager);

        Customer* customer =
            new Customer(1,
                         "Dipanjan",
                         "9999999999",
                         "test@mail.com");

        bank.addCustomer(customer);

        Account* savings =
            AccountFactory::createAccount(
                "Savings",
                1001,
                10000
            );

        Account* current =
            AccountFactory::createAccount(
                "Current",
                1002,
                5000
            );

        Account* fd =
            AccountFactory::createFD(
                1003,
                50000,
                24
            );

        customer->addAccount(savings);
        customer->addAccount(current);
        customer->addAccount(fd);

        mainBranch->addAccount(savings);
        mainBranch->addAccount(current);
        mainBranch->addAccount(fd);

        SMSNotification sms(customer->getMobile());
        EmailNotification email(customer->getEmail());

        Transaction::deposit(savings, 2000, &sms);

        Transaction::withdraw(current, 1000, &sms);

        Transaction::transfer(
            savings,
            current,
            1500,
            &email
        );

        Loan* homeLoan =
            new Loan("Home",
                1000000,
                8.5,
                15
            );

        LoanService::approveLoan(homeLoan, 100000);

        customer->addLoan(homeLoan);

        ATMCard atm(
            123456789012,
            4321,
            savings
        );

        atm.withdrawCash(4321, 1000);

        customer->showAccounts();

        homeLoan->displayLoan();

    return 0;
}
