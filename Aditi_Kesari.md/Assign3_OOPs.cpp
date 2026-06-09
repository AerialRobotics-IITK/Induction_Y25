#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#pragma once
#include <stdexcept>
using namespace std;
class InsufficientBalanceException : public std::runtime_error{
    public:
        InsufficientBalanceException(const string& msg = "insufficient balance in account")
            : std::runtime_error(msg) {}
};
class InvalidPINException : public std::runtime_error{
    public:
    InvalidPINException(const string& msg = "Invalid pin entered")
        : std::runtime_error(msg) {}
};
class AccountBlockedException : public std::runtime_error{
    public:
        AccountBlockedException(const string& msg = "Account is blocked")
            : std::runtime_error(msg) {}
};
class LoanRejectedException : public std::runtime_error{
    public:
        LoanRejectedException (const string& msg = "Loan application has been rejectedd")
            : std::runtime_error(msg) {}
};
using namespace std;
class Branch;
class Customer;
class Account;
class Transaction;
class Loan;
class Employee;
class Notification{
    public:
        virtual void sendNotification()= 0;
        virtual ~Notification() = default;
};
class SMSNotification : public Notification{
public:
    string phoneNumber;
    string message;
    string deliveryStatus;
     SMSNotification(const string& phone, const string& msg) {
        phoneNumber    = phone;
        message        = msg;
        deliveryStatus = "Pending";
    }
    void sendNotification(){
       cout << "SMS sent to:" << phoneNumber <<"\n" << "message: " << message << "\n" << endl;
       deliveryStatus = "sent";
    }
};
class EmailNotification : public Notification{
public:
    string emailAddress;
    string subject;
    string message;
    string deliveryStatus;
    EmailNotification(const string& email,
                      const string& subj,
                      const string& msg) {
        emailAddress   = email;
        subject        = subj;
        message        = msg;
        deliveryStatus = "Pending";
    }
    void sendNotification(){
        cout << "email sent to: " << emailAddress << endl;
        deliveryStatus = "sent";
    }
};
class Employee{
public:
    int employeeId;
    string employeeName;
    string designation;
    double salary;
    Branch* branch;
    Employee(int id, string name, string desig, double sal, Branch* br)
        : employeeName(name),
          designation(desig),
          salary(sal),
          branch(br),
          employeeId(id) {}
    void display() const {
        std::cout << "EmpID: "     << employeeId
                  << " | Name: "   << employeeName
                  << " | Role: "   << designation
                  << " | Salary: " << salary << "\n";
    }
};
class Loan{
public:
    int loanId;
    string loanType;
    double loanAmount;
    double interestRate;
    int tenureYears;
    double EMIAmount;
    string loanStatus;
    Customer* customer;
     Loan(int id, const string& type,
         double amount,
         double rate,
         int tenure,
         Customer* cust)
        : loanId(id),
          loanType(type),
          loanAmount(amount),
          interestRate(rate),
          tenureYears(tenure),
          loanStatus("Pending"),
          customer(cust)
    {
        EMIAmount = calculateEMI();
    }
    double calculateEMI() const {
        return (loanAmount + (loanAmount*interestRate/100))/(tenureYears*12);
    }
    void approve(){
        loanStatus = "Approved";
        cout << "loan is approved\n" << endl ;
    }
     void display() const {
        std::cout << "LoanID: "     << loanId
                  << " | Type: "    << loanType
                  << " | Amount: "  << loanAmount
                  << " | Rate: "    << interestRate << "%"
                  << " | Tenure: "  << tenureYears  << " yrs"
                  << " | EMI: "     << EMIAmount
                  << " | Status: "  << loanStatus   << "\n";
    }
};
class Branch{
public:
    int branchId;
    string branchName;
    string IFSCCode;
    string address;
    vector<Account*> accounts;
    vector<Employee*> employees;
    Branch(int id, string name, string code, string addr){
        branchId = id;
        IFSCCode = code;
        branchName = name;
        address = addr;
        cout << "branch created!\n" << endl;
    }
    void addAccount(Account* acc)  { accounts.push_back(acc);}
    void addEmployee(Employee* emp){ employees.push_back(emp);}

    void display(){
        cout << "Branch: " << branchName
             << " | IFSC: "<< IFSCCode
             << " | Address: "<< address << endl;
    }
};
class Customer{
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
    vector<Account*> accounts;
    vector<Loan*> loans;
    Customer(string n, string d, string g, string num, string e, string add, string adh, string pan){
        cout << "customer created" << endl;
        fullName = n;
        dob = d;
        gender = g;
        mobileNumber = num;
        email = e;
        address = add;
        aadhaarNumber = adh;
        PANNumber = pan;
    }
     void addAccount(Account* acc) { accounts.push_back(acc); }
     void addLoan(Loan* loan)      { loans.push_back(loan);   }
     void display(){
        cout << "CustomerID: " << customerId
             << " | Name: " << fullName
             << " | Mobile: " << mobileNumber
             << " | Email: " << email
             << " | address: " << address << "\n" << endl;

     }   
};
class Account{
public:
    long accountNumber;
    string accountType;
    double balance;
    string dateOpened;
    string status;
    Branch* branch;
    Customer* customer;
    vector<Transaction*> transactions;
    //constructor
    Account(long no, double initialBalance, Branch* br, Customer* cust)
        : accountNumber(no),
          balance(initialBalance),
          status("Active"),
          branch(br),
          customer(cust){
            time_t now = time(nullptr);
            char buf[26];
            ctime_s(buf, sizeof(buf), &now);
            dateOpened = string(buf);
            if (!dateOpened.empty() && dateOpened.back() == '\n')
            dateOpened.pop_back();
          }
    virtual ~Account() = default;
    void checkactive(){
        if(status == "blocked"){
            throw AccountBlockedException("Account is blocked");
        }
    }
    void deposit(double amount){
        checkactive();
        balance += amount;
        cout << "Deposited" << amount << "to account" << accountNumber << "New Balance" << balance <<"\n" << endl;
    }
    virtual void withdraw(double amount)=0;
    virtual void display() const {
        std::cout << "Account#: " << accountNumber
                  << " | Type: "   << accountType
                  << " | Balance: " << balance
                  << " | Status: "  << status << "\n";
    }

};
class SavingsAccount : public Account{
public:
    double interestRate;
    double minimumBalance;
     SavingsAccount(long no, double initialBalance,
                   Branch* br, Customer* cust,
                   double rate = 4.0,
                   double minBal = 1000.0)
        : Account(no, initialBalance, br, cust),
          interestRate(rate),
          minimumBalance(minBal)
    {
        accountType = "Savings";
    }
    void withdraw(double amount){
        checkactive();
        if(balance-amount < minimumBalance){
            throw InsufficientBalanceException("Insufficient balance");
        }
        else{
            balance -= amount;
            cout << "withdrawl successful, current balance:" << balance << endl;
        }
        
    }
     void applyInterest() {
        double interest = balance * interestRate / 100.0;
        balance += interest;
        std::cout << "Interest of " << interest << " applied to account " << accountNumber
                  << ". New balance: " << balance << "\n";
    }
    void display() const override {
        Account::display();
        std::cout << "  InterestRate: " << interestRate
                  << "% | MinBalance: "  << minimumBalance << "\n" << endl;
    }
};
class CurrentAccount : public Account{
public:
    double overdraftLimit;
    string businessName;
    CurrentAccount(long no, double initialBalance,
                   Branch* br, Customer* cust,
                   const string& bizName,
                   double odLimit = 50000.0)
        : Account(no, initialBalance, br, cust),
          overdraftLimit(odLimit),
          businessName(bizName)
    {
        accountType = "current";
    }
    void withdraw(double amount){
        checkactive();
        if(balance + overdraftLimit >= amount){
            balance -= amount;
            cout << "withdrawl successful, current balance:" << balance << endl;
        }
        else{
            throw InsufficientBalanceException("Exceeds overdraft limit");
        }
        
    }
    void display() const override {
        Account::display();
        std::cout << "  Business: " << businessName
                  << " | OverdraftLimit: " << overdraftLimit << "\n";
    }
};
class FixedDepositAccount : public Account{
public:
    double FDAmount;
    string maturityDate;
    double FDInterestRate;
    int tenureMonths;
     FixedDepositAccount(long no, double fdAmount,
                        Branch* br, Customer* cust,
                        int tenure = 12,
                        double fdRate = 7.5)
        : Account(no, fdAmount, br, cust),
          FDAmount(fdAmount),
          FDInterestRate(fdRate),
          tenureMonths(tenure)
    {
        accountType = "Fixed Deposit";
        time_t now = time(nullptr);
        now += static_cast<time_t>(tenure) * 30 * 24 * 3600;
        char buf[26];
        ctime_s(buf, sizeof(buf), &now);
        maturityDate = std::string(buf);
        if (!maturityDate.empty() && maturityDate.back() == '\n')
            maturityDate.pop_back();
    }


    void withdraw(double amount){
        throw runtime_error("Fixed Deposit cannot be withdrawn before maturity.");
    }
    double maturityAmount() const {
        return FDAmount* pow(1.0 + FDInterestRate/100.0, tenureMonths/12);
    }
    void display() const override {
        Account::display();
        std::cout << "  FDAmount: "    << FDAmount
                  << " | Rate: "       << FDInterestRate
                  << " | Tenure: "    << tenureMonths << " months"
                  << " | Maturity: "   << maturityDate
                  << " | MaturityAmt: " << maturityAmount() << "\n";
    }
};
class AccountFactory {
public:
    static Account* createAccount(long no, const std::string& type,
                                  double amount,
                                  Branch* br,
                                  Customer* cust,
                                  const std::string& extra = "")
    {
        if (type == "Savings")
            return new SavingsAccount(no, amount, br, cust);
        if (type == "Current")
            return new CurrentAccount(no, amount, br, cust, extra);
        if (type == "FixedDeposit")
            return new FixedDepositAccount(no, amount, br, cust);
        throw std::invalid_argument("Unknown account type: " + type);
    }
};
class Transaction{
public:
    int transactionId;
    string transactionType;
    double amount;
    string transactionDate;
    Account* senderAccount;
    Account* receiverAccount;
    string status;
    Transaction(int id, const std::string& type,
                double             amt,
                Account*           sender   = nullptr,
                Account*           receiver = nullptr)
        : transactionId(id),
          transactionType(type),
          amount(amt),
          senderAccount(sender),
          receiverAccount(receiver),
          status("Success")
    {
        time_t now = time(nullptr);
        char buf[26];
        ctime_s(buf, sizeof(buf), &now);
        transactionDate = std::string(buf);
        if (!transactionDate.empty() && transactionDate.back() == '\n')
            transactionDate.pop_back();
    }
    void display() const {
        std::cout << "  TxnID: "   << transactionId
                  << " | Type: "   << transactionType
                  << " | Amt: "    << amount
                  << " | Status: " << status
                  << " | Date: "   << transactionDate << "\n";
    }
};
class ATMCard{
    int PIN;
    int failedAttempts;
    static const int MAX_ATTEMPTS = 3;
public:
    long cardNumber;
    int CVV;
    string expiryDate;
    string cardType;
    string cardStatus;
    Account* linkedAccount;
    ATMCard(int pin, int cvv,long number, string date, string type, Account* account){
        PIN= pin;
        CVV= cvv;
        expiryDate = date;
        cardNumber= number;
        cardType= type;
        cardStatus= "active";
        failedAttempts= 0;

    }
    bool verifyPIN(int enteredPIN){
        if(cardStatus == "blocked"){
            throw AccountBlockedException("Card is blocked");
        }
        if(enteredPIN == PIN){
            failedAttempts = 0;
            return true;
        }
        else{
            failedAttempts++;
            if (failedAttempts>=MAX_ATTEMPTS){
                cardStatus= "blocked";
                throw InvalidPINException("card blocked after 3 failed attempts");
            }
            else{
                throw InvalidPINException("incorrect PIN. Retry");
            }
        }
    }
    void blockCard()    { cardStatus = "Blocked"; }
    void activateCard() { cardStatus = "Active"; failedAttempts = 0; }
    void display(){
        cout << "card :" << cardNumber
             << "| Type: " << cardType
             << "| Expiry: " << expiryDate
             << "| Status: " << cardStatus << "\n" << endl ;
    }
};
class Bank{
public:
    int bankId;
    string bankName;
    vector <Branch*> branches;
    vector <Customer*> customers;
    vector <Employee*> employees;
    Bank(int idd, string namee){
        bankId = idd;
        bankName = namee;
    }
    ~Bank(){
        for(auto* b : branches) delete b;
        for(auto* c : customers) delete c;
    }
    void addBranch(Branch* br)     { branches.push_back(br);}
    void addCustomer(Customer* c)  { customers.push_back(c);}
    void addEmployee(Employee* e)  { employees.push_back(e);}
     Account* openAccount(long no, const std::string& type,
                         double             initialDeposit,
                         Customer*          cust,
                         Branch*            br,
                         const std::string& extra = "")
    {
        Account* acc = AccountFactory::createAccount(no, type, initialDeposit, br, cust, extra);
        cust->addAccount(acc);
        br->addAccount(acc);
        std::cout << "Opened " << type << " account " << acc->accountNumber
                  << " for " << cust->fullName << " at branch " << br->branchName << "\n";
        return acc;
    }
     Loan* applyLoan(int id, const std::string& type,
                    double amount, double rate, int tenure,
                    Customer* cust,
                    bool approve = true)
    {
        Loan* loan = new Loan(id, type, amount, rate, tenure, cust);
        cust->addLoan(loan);
        if (approve)
            loan->approve();
        else
            cout << "Criteria not met" << endl;
        return loan;
    }
    ATMCard* issueCard(long no, Account* acc, Customer* cust,
                       int pin, int cvv,
                       const std::string& expiry,
                       const std::string& type = "Debit")
    {
        ATMCard* card = new ATMCard(pin, cvv, no, expiry, type, acc);
        std::cout << "Issued " << type << " card " << card->cardNumber
                  << " to " << cust->fullName << "\n";
        return card;
    }
 
    void display() const {
        std::cout << "========== " << bankName << " ==========\n";
        std::cout << "Branches(" << branches.size() << "):\n";
    }
};
int main(){
    Bank smartBank(1234,"SmartBank India Pvt. Ltd.");
    Branch* mainBranch = new Branch(12341, "Main Branch - Delhi", "SMRT0001001", "Connaught Place, New Delhi");
    Branch* mumbaiBranch = new Branch(12342, "Mumbai Branch", "SMRT0002001", "Nariman Point, Mumbai");
    smartBank.addBranch(mainBranch);
    smartBank.addBranch(mumbaiBranch);
    Employee* mgr = new Employee(123411, "Rohit Singh", "Branch Manager", 90000.0, mainBranch);
    Employee* cashier= new Employee(123412, "Priya Sharma",  "Cashier", 35000.0, mainBranch);
    mainBranch->addEmployee(mgr);
    mainBranch->addEmployee(cashier);
    smartBank.addEmployee(mgr);
    smartBank.addEmployee(cashier);
    mgr->display();
    cashier->display();
    Customer* alice = new Customer(
        "Alice Fernandes", "1995-04-12", "Female",
        "9876543210", "alice@email.com",
        "12, Rose Street, Delhi",
        "1234-5678-9012", "ABCPF1234Z");
 
    Customer* bob = new Customer(
        "Bob Mehta", "1988-11-30", "Male",
        "9123456780", "bob@biz.com",
        "45, MG Road, Mumbai",
        "9876-5432-1098", "XYZPM9876Q");
 
    smartBank.addCustomer(alice);
    smartBank.addCustomer(bob);
    Account* aliceSavings = smartBank.openAccount(123456789, "Savings",     50000.0, alice, mainBranch);
    Account* aliceFD      = smartBank.openAccount(234567891, "FixedDeposit",100000.0, alice, mainBranch);
    Account* bobCurrent   = smartBank.openAccount(345678912, "Current",     200000.0, bob, mumbaiBranch, "Mehta Exports Pvt. Ltd.");
    aliceSavings->deposit(5000.0);
    aliceSavings->withdraw(2000.0);

    bobCurrent->deposit(10000.0);
    bobCurrent->withdraw(5000.0);

    try {
        aliceFD->withdraw(5000.0);   // should throw
    } catch (const std::exception& ex) {
        std::cerr << "Expected error: " << ex.what() << "\n";
    }
    Loan* homeLoan = smartBank.applyLoan( 5678, "Home", 5000000.0, 8.5, 20, alice, true);
    homeLoan->display();
    ATMCard* aliceCard = smartBank.issueCard(13579, aliceSavings, alice,
                                             1234, 456, "12/28", "Debit");
    aliceCard->display();
 
    // Correct PIN
    try {
        if (aliceCard->verifyPIN(1234))
            std::cout << "PIN verified successfully.\n";
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << "\n";
    }
    Notification* smsAlert = new SMSNotification(
        alice->mobileNumber,
        "Your account " + std::to_string(aliceSavings->accountNumber) + " has a new transaction.");
 
    Notification* emailAlert = new EmailNotification(
        alice->email,
        "Transaction Alert",
        "Dear " + alice->fullName + ", your home loan of Rs 50,00,000 has been approved.");
 
    smsAlert->sendNotification();
    emailAlert->sendNotification();
 
    delete smsAlert;
    delete emailAlert;
    return 0;
}
