#include <iostream>
#include <string>
#include <vector>
#include <exception>
using namespace std;

class Account{
    protected:
    long accountNumber;
    string accountType;
    double balance;
    string dateOpened;
    string status;
    public:
    //constructor
    Account(long accNo, string type, double bal, string date) {
        accountNumber = accNo;
        accountType = type;
        balance = bal;
        dateOpened = date;
        status = "Active";
    }
    //virtual indicates the fn can be overwritten in child class
    virtual void show_details(){
        cout<<"Account Number: "<<accountNumber<<endl;
        cout<<"Account Type: "<<accountType<<endl;
        cout<<"Balance: "<<balance<<endl;
        cout << "Date Opened: " << dateOpened << endl;
        cout<<"Status: "<<status<<endl;
    }
    virtual void deposit(double amount){
        balance+=amount;
        cout << amount << " deposited " << endl;
        
    }
    virtual void withdraw(double amount)=0;

    long getAccountNumber() {
    return accountNumber;
    }
};
// Custom expections
class InsufficientBalanceException : public exception {
public:
    //const char*-return type, what()-gives the error message
    //const-doesnt change the obj
    //noexept-this fn will not throw an execption
    //overiddes parent execption class
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
        return "Account is blocked";
    }
};

class LoanRejectedException : public exception {
public:
    const char* what() const noexcept override {
        return "Loan rejected";
    }
};
//kyuki we need to define customer in loan 
class Customer;

class Loan{
    int loanId;
    string loanType;
    double loanAmount;
    double interestRate;
    int tenureYears;
    double EMIAmount;
    string loanStatus;
    Customer* customer;//stores address of an object customer to link

    public:
    //constructor
    Loan(int id, string type, double amount, double rate, int years,double emi,Customer* cust) {
        loanId = id;
        loanType = type;
        loanAmount = amount;
        interestRate = rate;
        tenureYears = years;
        loanStatus = "Approved";
        EMIAmount = emi;
        customer= cust;
    }

    void show_details() {
        cout << "Loan ID: " << loanId << endl;
        cout << "Loan Type: " << loanType << endl;
        cout << "Loan Amount: " << loanAmount << endl;
        cout << "Interest Rate: " << interestRate << endl;
        cout << "Tenure Years: " << tenureYears << endl;
        cout << "EMI Amount: " << EMIAmount << endl;
        cout << "Loan Status: " << loanStatus << endl;
    }
};
class Customer{
    private:
    int customerId;
    string fullName;
    string dob;
    string gender;
    string mobileNumber;
    string email;
    string address;
    string aadhaarNumber;
    string PANNumber;

    vector <Account*> accounts;//indicates list
    vector <Loan*> loans;
    public:
     Customer(int id, string name, string mobile, string mail) {
        customerId = id;
        fullName = name;
        mobileNumber = mobile;
        email = mail;
    }

    void add_account(Account* acc){
            accounts.push_back(acc);
        }
    void add_loan(Loan* loan) {
            loans.push_back(loan);
        }

        //vector commands
        //push_back() adds new item at the end of the list.
        //v.size()-tells how many elements are there
        //pop_back() removes last element
        //empty() checks whether vector is empty
        //clear() removes all elements
        //v.front() gives first element

    void show_details(){
        cout << "Customer ID: " << customerId << endl;
        cout << "Full Name: " << fullName << endl;
        cout << "Mobile Number: " << mobileNumber << endl;
        cout << "Email: " << email << endl;

        cout << "\nAccounts owned by customer:" << endl;
        for (int i = 0; i < accounts.size(); i++) {
            accounts[i]->show_details();
        } 
         cout << "\nLoans taken by customer:" << endl;

        for (int i = 0; i < loans.size(); i++) {
            loans[i]->show_details();
        }
    }

    
};

class SavingsAccount : public Account {
    private:
    double interestRate;
    double minimumBalance;
    public:
     SavingsAccount(long accNo, double bal, string date, double rate, double minBal)
        : Account(accNo, "Savings", bal, date) {
        interestRate = rate;
        minimumBalance = minBal;
    }
    void withdraw(double amount){
        if (status == "Blocked") {
        throw AccountBlockedException();
    }
        if(balance-amount<minimumBalance){
            throw InsufficientBalanceException();
        }
        else{
            balance-=amount;
            cout<< amount<<" withdrawn"<<endl;
        }
    }
    void show_details(){
        Account::show_details();// :: is used for class and . is used for objects
        cout << "Interest Rate: " << interestRate << endl;
        cout << "Minimum Balance: " << minimumBalance << endl;
    }

};
class CurrentAccount : public Account {
private:
    double overdraftLimit;
    string businessName;

public:
    CurrentAccount(long accNo, double bal, string date, double overdraft, string business)
        : Account(accNo, "Current", bal, date) {
        overdraftLimit = overdraft;
        businessName = business;
    }

    void withdraw(double amount) {
        if (status == "Blocked") {
        throw AccountBlockedException();
        }
        if (balance - amount < -overdraftLimit) {
            throw InsufficientBalanceException();
        } 
        else {
            balance -= amount;
            cout << amount << " withdrawn" << endl;
        }
    }

    void show_details(){
        Account::show_details();
        cout << "Overdraft Limit: " << overdraftLimit << endl;
        cout << "Business Name: " << businessName << endl;
    }
};
class FixedDepositAccount : public Account {
private:
    double FDAmount;
    string maturityDate;
    double FDInterestRate;
    int tenureMonths;

public:
    FixedDepositAccount(long accNo, double amount, string date, string maturity, double rate, int tenure)
        : Account(accNo, "Fixed Deposit", amount, date) {
        FDAmount = amount;
        maturityDate = maturity;
        FDInterestRate = rate;
        tenureMonths = tenure;
    }

    void withdraw(double amount) {
        throw InsufficientBalanceException();
    }

    void show_details() override {
        Account::show_details();
        cout << "FD Amount: " << FDAmount << endl;
        cout << "Maturity Date: " << maturityDate << endl;
        cout << "FD Interest Rate: " << FDInterestRate << endl;
        cout << "Tenure Months: " << tenureMonths << endl;
    }
};

class Transaction {
private:
    int transactionId;
    string transactionType;
    double amount;
    string transactionDate;
    Account* senderAccount;
    Account* receiverAccount;
    string status;

public:
    Transaction(int id, string type, double amt, string date, Account* sender, Account* receiver, string stat) {
        transactionId = id;
        transactionType = type;
        amount = amt;
        transactionDate = date;
        senderAccount = sender;
        receiverAccount = receiver;
        status = stat;
    }

    void show_details() {
        cout << "Transaction ID: " << transactionId << endl;
        cout << "Transaction Type: " << transactionType << endl;
        cout << "Amount: " << amount << endl;
        cout << "Date: " << transactionDate << endl;
        cout << "Status: " << status << endl;

        if (senderAccount != NULL) {
            cout << "Sender Account: " << senderAccount->getAccountNumber() << endl;
        }

        if (receiverAccount != NULL) {
            cout << "Receiver Account: " << receiverAccount->getAccountNumber() << endl;
        }
    }
};

class Employee {
private:
    int employeeId;
    string employeeName;
    string designation;
    double salary;

public:
    Employee(int id, string name, string des, double sal) {
        employeeId = id;
        employeeName = name;
        designation = des;
        salary = sal;
    }

    void show_details() {
        cout << "Employee ID: " << employeeId << endl;
        cout << "Employee Name: " << employeeName << endl;
        cout << "Designation: " << designation << endl;
        cout << "Salary: " << salary << endl;
    }
};

class Branch {
private:
    int branchId;
    string branchName;
    string IFSCCode;
    string address;

    vector <Account*> accounts;
    vector <Employee*> employees;
public:
    Branch(int id, string name, string ifsc, string addr) {
        branchId = id;
        branchName = name;
        IFSCCode = ifsc;
        address = addr;
    } 

    void add_account(Account* acc) {
        accounts.push_back(acc);
    }
    void add_employee(Employee* emp) {
    employees.push_back(emp);
    }

    void show_details() {
        cout << "\nBranch Details:" << endl;
        cout << "Branch ID: " << branchId << endl;
        cout << "Branch Name: " << branchName << endl;
        cout << "IFSC Code: " << IFSCCode << endl;
        cout << "Address: " << address << endl;

        cout << "\nAccounts in this branch:" << endl;
        for (int i = 0; i < accounts.size(); i++) {
            accounts[i]->show_details();
        }
        cout << "\nEmployees in this branch:" << endl;

        for (int i = 0; i < employees.size(); i++) {
        employees[i]->show_details();
        }
    }
};
class Bank {
private:
    int bankId;
    string bankName;
    vector <Branch*> branches;
    vector <Customer*> customers;
    vector <Employee*> employees;

public:
    Bank(int id, string name) {
        bankId = id;
        bankName = name;
    }

    void add_branch(Branch* branch) {
        branches.push_back(branch);
    }

    void add_customer(Customer* customer) {
        customers.push_back(customer);
    }

    void add_employee(Employee* employee) {
        employees.push_back(employee);
    }

    void show_details() {
        cout << "\nBank Details:" << endl;
        cout << "Bank ID: " << bankId << endl;
        cout << "Bank Name: " << bankName << endl;

        cout << "\nBranches:" << endl;
        for (int i = 0; i < branches.size(); i++) {
            branches[i]->show_details();
        }

        cout << "\nCustomers:" << endl;
        for (int i = 0; i < customers.size(); i++) {
            customers[i]->show_details();
        }

        cout << "\nEmployees:" << endl;
        for (int i = 0; i < employees.size(); i++) {
            employees[i]->show_details();
        }
    }
};
class ATMCard {
private:
    long cardNumber;
    int CVV;
    string expiryDate;
    int PIN;
    string cardType;
    string cardStatus;
    Account* linkedAccount;

public:
    ATMCard(long cardNo, int cvv, string expiry, int pin, string type, Account* acc) {
        cardNumber = cardNo;
        CVV = cvv;
        expiryDate = expiry;
        PIN = pin;
        cardType = type;
        cardStatus = "Active";
        linkedAccount = acc;
    }

    void withdraw_atm(double amount, int enteredPIN) {
        if (cardStatus == "Blocked") {
            throw AccountBlockedException();
        }

        if (enteredPIN!= PIN) {
            throw InvalidPINException();
        }

        linkedAccount->withdraw(amount);
    }

    void block_card() {
        cardStatus = "Blocked";
        cout << "Card blocked" << endl;
    }

    void show_details() {
        cout << "Card Number: " << cardNumber << endl;
        cout << "Expiry Date: " << expiryDate << endl;
        cout << "Card Type: " << cardType << endl;
        cout << "Card Status: " << cardStatus << endl;
    }
};

class Notification {
public:
    virtual void send(string message) = 0;
};
class SMSNotification : public Notification {
private:
    string phoneNumber;
    string deliveryStatus;

public:
    SMSNotification(string phone) {
        phoneNumber = phone;
        deliveryStatus = "Pending";
    }

    void send(string message) {
        deliveryStatus = "Sent";
        cout << "SMS sent to " << phoneNumber << ": " << message << endl;
    }
};

class EmailNotification : public Notification {
private:
    string emailAddress;
    string subject;
    string deliveryStatus;

public:
    EmailNotification(string email, string sub) {
        emailAddress = email;
        subject = sub;
        deliveryStatus = "Pending";
    }

    void send(string message){
        deliveryStatus = "Sent";
        cout << "Email sent to " << emailAddress << endl;
        cout << "Subject: " << subject << endl;
        cout << "Message: " << message << endl;
    }
};



int main() {
 
    Bank bank1(1, "SBank");
    Branch branch1(101, "IITK", "1234", "IIT Kanpur");
    Customer customer1(1, "J", "9999999999", "js.com");
    Employee emp1(201, "R", "Manager", 60000);
    Employee emp2(202, "S", "Cashier", 35000);

    SavingsAccount savings1(1001, 5000, "18-05-2026", 4.5, 1000);
    CurrentAccount current1(1002, 10000, "18-05-2026", 5000, "ABC");
    FixedDepositAccount fd1(1003, 20000, "18-05-2026", "18-05-2027", 7.0, 12);
    //customer linked accounts 
    customer1.add_account(&savings1);
    customer1.add_account(&current1);
    customer1.add_account(&fd1);

    Loan loan1(501, "Home Loan", 1000000, 8.5, 10, 12000, &customer1);
    customer1.add_loan(&loan1);

    //branch linked with accounts
    branch1.add_account(&savings1);
    branch1.add_account(&current1);
    branch1.add_account(&fd1);

    //employee linked with branch
    branch1.add_employee(&emp1);
    branch1.add_employee(&emp2);

    //bank linked with branch
    bank1.add_branch(&branch1);
    bank1.add_customer(&customer1);
    bank1.add_employee(&emp1);
    bank1.add_employee(&emp2);

    savings1.deposit(2000);

    //trying out errors
    try {
        savings1.withdraw(3000);
    }
    catch (exception &e) {
        cout << "Savings Error: " << e.what() << endl;
    }

    try {
        current1.withdraw(12000);
    }
    catch (exception &e) {
        cout << "Current Error: " << e.what() << endl;
    }

    try {
        fd1.withdraw(5000);
    }
    catch (exception &e) {
        cout << "FD Error: " << e.what() << endl;
    }
    ATMCard card1(123456789, 123, "12-2030", 4321, "Debit", &savings1);

    //wrong pin
    try {
        card1.withdraw_atm(500, 1111);
    }
    catch (exception &e) {
        cout << "ATM Error: " << e.what() << endl;
    }

    //Blocked card
    card1.block_card();

    try {
        card1.withdraw_atm(500, 4321);
    }
    catch (exception &e) {
        cout << "ATM Error: " << e.what() << endl;
    }

    //Transactions
    Transaction t1(1, "Deposit", 2000, "18-05-2026", NULL, &savings1, "Success");
    Transaction t2(2, "Transfer", 1000, "18-05-2026", &savings1, &current1, "Success");
    
    t1.show_details();
    t2.show_details();

    //Notifications
    SMSNotification sms("9999999999");
    EmailNotification email("js.com", "Bank Alert");
    sms.send("yaayy");
    email.send("yayyy");

    customer1.show_details();
    branch1.show_details();
    bank1.show_details();
    return 0;
}