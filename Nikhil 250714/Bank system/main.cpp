#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <cmath>

using namespace std;

class Customer;
class Branch;
class Account;

// ================= EXCEPTIONS =================

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
        return "Account is blocked!";
    }
};

class LoanRejectedException : public exception {
public:
    const char* what() const noexcept override {
        return "Loan Rejected!";
    }
};

// ================= ACCOUNT =================

class Account {

protected:
    long accountNumber;
    string accountType;
    double balance;
    string status;

    Customer* customer;
    Branch* branch;

public:

    Account(long accNo,
            string type,
            double bal,
            Customer* cust,
            Branch* br) {

        accountNumber = accNo;
        accountType = type;
        balance = bal;
        customer = cust;
        branch = br;

        status = "Active";
    }

    virtual void deposit(double amount) {

        balance += amount;

        cout << amount
             << " deposited successfully.\n";
    }

    virtual void withdraw(double amount) = 0;

    virtual void display() {

        cout << "\nAccount Number: "
             << accountNumber << endl;

        cout << "Account Type: "
             << accountType << endl;

        cout << "Balance: "
             << balance << endl;

        cout << "Status: "
             << status << endl;
    }

    long getAccountNumber() {
        return accountNumber;
    }

    virtual ~Account() {}
};

// ================= SAVINGS ACCOUNT =================

class SavingsAccount : public Account {

private:
    double interestRate;
    double minimumBalance;

public:

    SavingsAccount(long accNo,
                   double bal,
                   Customer* cust,
                   Branch* br)
        : Account(accNo,
                  "Savings",
                  bal,
                  cust,
                  br) {

        interestRate = 4.5;
        minimumBalance = 1000;
    }

    void withdraw(double amount) override {

        if(status == "Blocked")
            throw AccountBlockedException();

        if(balance - amount < minimumBalance)
            throw InsufficientBalanceException();

        balance -= amount;

        cout << amount
             << " withdrawn successfully.\n";
    }

    void display() override {

        Account::display();

        cout << "Interest Rate: "
             << interestRate << endl;

        cout << "Minimum Balance: "
             << minimumBalance << endl;
    }
};

// ================= CURRENT ACCOUNT =================

class CurrentAccount : public Account {

private:
    double overdraftLimit;
    string businessName;

public:

    CurrentAccount(long accNo,
                   double bal,
                   Customer* cust,
                   Branch* br,
                   string business)
        : Account(accNo,
                  "Current",
                  bal,
                  cust,
                  br) {

        overdraftLimit = 50000;
        businessName = business;
    }

    void withdraw(double amount) override {

        if(status == "Blocked")
            throw AccountBlockedException();

        if(balance - amount < -overdraftLimit)
            throw InsufficientBalanceException();

        balance -= amount;

        cout << amount
             << " withdrawn successfully.\n";
    }

    void display() override {

        Account::display();

        cout << "Overdraft Limit: "
             << overdraftLimit << endl;

        cout << "Business Name: "
             << businessName << endl;
    }
};

// ================= FIXED DEPOSIT =================

class FixedDepositAccount : public Account {

private:
    double FDInterestRate;
    int tenureMonths;

public:

    FixedDepositAccount(long accNo,
                        double bal,
                        Customer* cust,
                        Branch* br)
        : Account(accNo,
                  "Fixed Deposit",
                  bal,
                  cust,
                  br) {

        FDInterestRate = 7.5;
        tenureMonths = 24;
    }

    void withdraw(double amount) override {

        throw runtime_error(
            "Cannot withdraw from Fixed Deposit before maturity!"
        );
    }

    void display() override {

        Account::display();

        cout << "FD Interest Rate: "
             << FDInterestRate << endl;

        cout << "Tenure Months: "
             << tenureMonths << endl;
    }
};

// ================= EMPLOYEE =================

class Employee {

private:
    int employeeId;
    string employeeName;
    string designation;
    double salary;

    Branch* branch;

public:

    Employee(int id,
             string name,
             string desig,
             double sal,
             Branch* br) {

        employeeId = id;
        employeeName = name;
        designation = desig;
        salary = sal;

        branch = br;
    }

    void display() {

        cout << "\nEmployee ID: "
             << employeeId << endl;

        cout << "Employee Name: "
             << employeeName << endl;

        cout << "Designation: "
             << designation << endl;

        cout << "Salary: "
             << salary << endl;
    }
};

// ================= BRANCH =================

class Branch {

private:
    int branchId;
    string branchName;
    string IFSCCode;
    string address;

    vector<Account*> accounts;
    vector<Employee*> employees;

public:

    Branch(int id,
           string name,
           string ifsc,
           string addr) {

        branchId = id;
        branchName = name;
        IFSCCode = ifsc;
        address = addr;
    }

    void addAccount(Account* acc) {

        accounts.push_back(acc);
    }

    void addEmployee(Employee* emp) {

        employees.push_back(emp);
    }

    void display() {

        cout << "\nBranch Name: "
             << branchName << endl;

        cout << "IFSC Code: "
             << IFSCCode << endl;
    }
};

// ================= LOAN =================

class Loan {

private:
    int loanId;
    string loanType;
    double loanAmount;
    double interestRate;
    int tenureYears;

    double EMIAmount;
    string loanStatus;

    Customer* customer;

public:

    Loan(int id,
         string type,
         double amount,
         double rate,
         int years,
         Customer* cust) {

        loanId = id;
        loanType = type;
        loanAmount = amount;
        interestRate = rate;
        tenureYears = years;

        customer = cust;

        loanStatus = "Approved";

        double monthlyRate =
            interestRate / (12 * 100);

        int months =
            tenureYears * 12;

        EMIAmount =
            (loanAmount * monthlyRate *
             pow(1 + monthlyRate, months))
            /
            (pow(1 + monthlyRate, months) - 1);
    }

    void display() {

        cout << "\nLoan Type: "
             << loanType << endl;

        cout << "Loan Amount: "
             << loanAmount << endl;

        cout << "EMI Amount: "
             << EMIAmount << endl;

        cout << "Loan Status: "
             << loanStatus << endl;
    }
};

// ================= CUSTOMER =================

class Customer {

public:

    string fullName;
    string dob;
    string gender;
    string mobileNumber;
    string email;
    string address;
    string aadhaarNumber;
    string PANNumber;

    vector<Account*> accounts;
    vector<Loan*> loans;

    Customer(string name,
             string d,
             string g,
             string mobile,
             string mail,
             string addr,
             string aadhaar,
             string pan) {

        fullName = name;
        dob = d;
        gender = g;
        mobileNumber = mobile;
        email = mail;
        address = addr;
        aadhaarNumber = aadhaar;
        PANNumber = pan;
    }

    void addAccount(Account* acc) {

        accounts.push_back(acc);
    }

    void addLoan(Loan* loan) {

        loans.push_back(loan);
    }
};

// ================= ATM CARD =================

class ATMCard {

private:
    long cardNumber;
    int PIN;
    int CVV;

    string expiryDate;
    string cardType;
    string cardStatus;

    Account* linkedAccount;
    Customer* customer;

public:

    ATMCard(long cardNo,
            Account* acc,
            Customer* cust,
            int pin,
            int cvv,
            string expiry,
            string type) {

        cardNumber = cardNo;
        linkedAccount = acc;
        customer = cust;

        PIN = pin;
        CVV = cvv;

        expiryDate = expiry;
        cardType = type;

        cardStatus = "Active";
    }

    bool verifyPIN(int enteredPIN) {

        if(cardStatus == "Blocked")
            throw AccountBlockedException();

        if(PIN != enteredPIN)
            throw InvalidPINException();

        return true;
    }

    void display() {

        cout << "\nCard Number: "
             << cardNumber << endl;

        cout << "Card Type: "
             << cardType << endl;

        cout << "Expiry Date: "
             << expiryDate << endl;
    }
};

// ================= NOTIFICATION =================

class Notification {

public:
    virtual void sendNotification() = 0;
};

// ================= SMS =================

class SMSNotification : public Notification {

private:
    string phoneNumber;
    string message;

public:

    SMSNotification(string phone,
                    string msg) {

        phoneNumber = phone;
        message = msg;
    }

    void sendNotification() override {

        cout << "\nSMS SENT\n";

        cout << "Phone Number: "
             << phoneNumber << endl;

        cout << "Message: "
             << message << endl;
    }
};

// ================= EMAIL =================

class EmailNotification : public Notification {

private:
    string emailAddress;
    string subject;
    string message;

public:

    EmailNotification(string email,
                      string sub,
                      string msg) {

        emailAddress = email;
        subject = sub;
        message = msg;
    }

    void sendNotification() override {

        cout << "\nEMAIL SENT\n";

        cout << "Email: "
             << emailAddress << endl;

        cout << "Subject: "
             << subject << endl;

        cout << "Message: "
             << message << endl;
    }
};

// ================= BANK =================

class Bank {

private:
    int bankId;
    string bankName;

    vector<Branch*> branches;
    vector<Customer*> customers;
    vector<Employee*> employees;

public:

    Bank(int id,
         string name) {

        bankId = id;
        bankName = name;
    }

    void addBranch(Branch* branch) {

        branches.push_back(branch);
    }

    void addCustomer(Customer* customer) {

        customers.push_back(customer);
    }

    void addEmployee(Employee* employee) {

        employees.push_back(employee);
    }

    Account* openAccount(long accNo,
                         string type,
                         double bal,
                         Customer* cust,
                         Branch* branch,
                         string business = "") {

        Account* acc;

        if(type == "Savings") {

            acc = new SavingsAccount(
                accNo,
                bal,
                cust,
                branch
            );
        }

        else if(type == "Current") {

            acc = new CurrentAccount(
                accNo,
                bal,
                cust,
                branch,
                business
            );
        }

        else {

            acc = new FixedDepositAccount(
                accNo,
                bal,
                cust,
                branch
            );
        }

        cust->addAccount(acc);

        branch->addAccount(acc);

        return acc;
    }

    Loan* applyLoan(int id,
                    string type,
                    double amount,
                    double rate,
                    int years,
                    Customer* cust,
                    bool approved) {

        if(!approved)
            throw LoanRejectedException();

        Loan* loan =
            new Loan(id,
                     type,
                     amount,
                     rate,
                     years,
                     cust);

        cust->addLoan(loan);

        return loan;
    }

    ATMCard* issueCard(long cardNo,
                       Account* acc,
                       Customer* cust,
                       int pin,
                       int cvv,
                       string expiry,
                       string type) {

        ATMCard* card =
            new ATMCard(cardNo,
                        acc,
                        cust,
                        pin,
                        cvv,
                        expiry,
                        type);

        return card;
    }
};

// ================= MAIN =================

int main(){

    Bank smartBank(1234,"SmartBank India Pvt. Ltd.");

    Branch* mainBranch =
        new Branch(
            12341,
            "Main Branch - Delhi",
            "SMRT0001001",
            "Connaught Place, New Delhi"
        );

    Branch* mumbaiBranch =
        new Branch(
            12342,
            "Mumbai Branch",
            "SMRT0002001",
            "Nariman Point, Mumbai"
        );

    smartBank.addBranch(mainBranch);
    smartBank.addBranch(mumbaiBranch);

    Employee* mgr =
        new Employee(
            123411,
            "Rahul",
            "Branch Manager",
            90000.0,
            mainBranch
        );

    Employee* cashier=
        new Employee(
            123412,
            "Ragav",
            "Cashier",
            35000.0,
            mainBranch
        );

    mainBranch->addEmployee(mgr);
    mainBranch->addEmployee(cashier);

    smartBank.addEmployee(mgr);
    smartBank.addEmployee(cashier);

    mgr->display();
    cashier->display();

    Customer* nikhil =
        new Customer(
            "nikhil",
            "200-02-20",
            "Male",
            "8700934395",
            "nikhil@email.com",
            "12, south Street, Delhi",
            "1234-5678-9012",
            "ABCPF1234Z"
        );

    Customer* ansh =
        new Customer(
            "ansh",
            "1973-11-30",
            "Male",
            "9123456780",
            "ansh@biz.com",
            "45, MG Road, Mumbai",
            "9876-5432-1098",
            "XYZPM9876Q"
        );

    smartBank.addCustomer(nikhil);
    smartBank.addCustomer(ansh);

    Account* nikhilSavings =
        smartBank.openAccount(
            123456789,
            "Savings",
            50000.0,
            nikhil,
            mainBranch
        );

    Account* nikhilFD =
        smartBank.openAccount(
            234567891,
            "FixedDeposit",
            100000.0,
            nikhil,
            mainBranch
        );

    Account* anshCurrent =
        smartBank.openAccount(
            345678912,
            "Current",
            200000.0,
            ansh,
            mumbaiBranch,
            "Mehta Exports Pvt. Ltd."
        );

    nikhilSavings->deposit(5000.0);

    nikhilSavings->withdraw(2000.0);

    anshCurrent->deposit(10000.0);

    anshCurrent->withdraw(5000.0);

    try {

        nikhilFD->withdraw(5000.0);

    } catch (const std::exception& ex) {

        cerr << "Expected error: "
             << ex.what() << "\n";
    }

    Loan* homeLoan =
        smartBank.applyLoan(
            5678,
            "Home",
            5000000.0,
            8.5,
            20,
            nikhil,
            true
        );

    homeLoan->display();

    ATMCard* nikhilCard =
        smartBank.issueCard(
            13579,
            nikhilSavings,
            nikhil,
            1234,
            456,
            "12/28",
            "Debit"
        );

    nikhilCard->display();

    try {

        if (nikhilCard->verifyPIN(1234))

            cout << "PIN verified successfully.\n";

    } catch (const std::exception& ex) {

        cerr << ex.what() << "\n";
    }

    Notification* smsAlert =
        new SMSNotification(
            nikhil->mobileNumber,
            "Your account has a new transaction."
        );

    Notification* emailAlert =
        new EmailNotification(
            nikhil->email,
            "Transaction Alert",
            "Dear nikhil, your home loan has been approved."
        );

    smsAlert->sendNotification();

    emailAlert->sendNotification();

    delete smsAlert;
    delete emailAlert;

    return 0;
}   