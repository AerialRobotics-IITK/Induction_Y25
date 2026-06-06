#include <bits/stdc++.h>
using namespace std;

class Account;
class Transaction;
class Employee;
class Branch;
class Customer;
class Loan;

class InsufficientBalanceException : public exception {
public:
    const char* what() const noexcept override {
        return "Insufficient Balance!";
    }
};

class AccountBlockedException:public exception{
public:

    const char* what() const noexcept override{
        return "Account Blocked";
    }
};

class InvalidPINException:public exception{
public:
    const char* what() const noexcept override{
        return "Invalid PIN";
    }
};

class LoanRejectedException:public exception{
public:

    const char* what() const noexcept override{
        return "Loan Rejected";
    }
};

class PrematureWithdrawalException : public exception {
public:
    const char* what() const noexcept override {
        return "Premature withdrawal not allowed!";
    }
};

class Customer {
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

        vector<Account*> accounts;
        vector<Loan*> loans;

    public:
        Customer(int id,
                 string name,
                 string d,
                 string g,
                 string mobile,
                 string mail,
                 string addr,
                 string aadhaar,
                 string pan){

            customerId=id;
            fullName=name;
            dob=d;
            gender=g;
            mobileNumber=mobile;
            email=mail;
            address=addr;
            aadhaarNumber=aadhaar;
            PANNumber=pan;
        }

        void addAccount(Account* acc){
            accounts.push_back(acc);
        }

        void addLoan(Loan* loan){
            loans.push_back(loan);
        }

        int getCustomerId(){
            return customerId;
        }

        string getName(){
            return fullName;
        }

        void showAccounts();
};

class Transaction{
private:
    int transactionId;
    string transactionType;
    double amount;
    string transactionDate;
    string status;

    Account* senderAccount;
    Account* receiverAccount;

public:
    Transaction(int id,
                string type,
                double amt,
                string date="",
                string stat="Success",
                Account* sender=nullptr,
                Account* receiver=nullptr){

        transactionId=id;
        transactionType=type;
        amount=amt;
        transactionDate=date;
        status=stat;

        senderAccount=sender;
        receiverAccount=receiver;
    }

    int getTransactionId(){
        return transactionId;
    }

    string getTransactionType(){
        return transactionType;
    }

    double getAmount(){
        return amount;
    }

    void displayTransaction(){
        cout<<"Transaction ID: "<<transactionId<<endl;
        cout<<"Transaction Type: "<<transactionType<<endl;
        cout<<"Amount: "<<amount<<endl;
        cout<<"Date: "<<transactionDate<<endl;
        cout<<"Status: "<<status<<endl;
    }
};

class Account {
    protected:
        long accountNumber;
        double balance;

        string accountType;
        string dateOpened;
        string status;

        Branch* branch;
        Customer* customer;

        vector<Transaction> transactions;

    public:
        Account(long accNo,
                double bal,
                string type,
                string date,
                Branch* b,
                Customer* cust){

            accountNumber=accNo;
            balance=bal;

            accountType=type;
            dateOpened=date;
            status="Active";

            branch=b;
            customer=cust;
        }

        virtual void deposit (double amount){
            if(status=="Blocked"){
                throw AccountBlockedException();
            }
            balance += amount;

            Transaction t(1,
                          "Deposit",
                          amount);

            transactions.push_back(t);
        }

        virtual void withdraw(double amount)=0;

        double getBalance(){
            return balance;
        }

        void addTransaction(Transaction t){
             transactions.push_back(t);
        }

        void showTransactions(){
            for(Transaction t:transactions){
                t.displayTransaction();
                cout<<endl;
            }
        }

        void blockAccount(){
            status="Blocked";
        }

        void activateAccount(){
            status="Active";
        }

        string getStatus(){
            return status;
        }

        virtual void displayAccount()=0;

        virtual ~Account() {}
};



class SavingsAccount : public Account {
    private:
        double minBalance;
        double interestRate;
        double dailyWithdrawalLimit;

    public:
        SavingsAccount(long accNo,
                       double bal,
                       double minBal,
                       double rate,
                       double dailyLimit,
                       string date,
                       Branch* b,
                       Customer* cust)

        : Account(accNo,
                  bal,
                  "Savings",
                  date,
                  b,
                  cust){

            minBalance=minBal;
            interestRate=rate;
            dailyWithdrawalLimit = dailyLimit;
        }


        void withdraw(double amount) override {

            if(status=="Blocked"){
                throw AccountBlockedException();
            }
            if(amount > dailyWithdrawalLimit) {
                cout << "Error: Amount exceeds daily withdrawal limit of " << dailyWithdrawalLimit << endl;
                return;
            }
            if(balance - amount < minBalance) {
                throw InsufficientBalanceException();
            }

            else {
                balance -= amount;

                Transaction t(2,
                              "Withdraw",
                              amount);

                transactions.push_back(t);
            }
        }

        void displayAccount() override {
            cout << "Account Number: "
                 << accountNumber << endl;

            cout << "Balance: "
                 << balance << endl;

            cout << "Minimum Balance: "
                 << minBalance << endl;

            cout << "Interest Rate: "
                 << interestRate << endl;

            cout << "Status: "
                 << status << endl;
        }
};

class CurrentAccount:public Account{
private:
    double overdraftLimit;
    string businessName;

public:
    CurrentAccount(long accNo,
                   double bal,
                   double limit,
                   string business,
                   string date,
                   Branch* b,
                   Customer* cust)

        : Account(accNo,
                  bal,
                  "Current",
                  date,
                  b,
                  cust){

        overdraftLimit=limit;
        businessName=business;
    }

    void withdraw(double amount) override{

        if(status=="Blocked"){
            throw AccountBlockedException();
        }

        if(balance+overdraftLimit<amount){
            throw InsufficientBalanceException();
        }

        else{
            balance-=amount;

            Transaction t(3,
                          "Withdraw",
                          amount);

            transactions.push_back(t);
        }
    }

    void displayAccount() override{

        cout<<"Account Number: "
            <<accountNumber<<endl;

        cout<<"Balance: "
            <<balance<<endl;

        cout<<"Overdraft Limit: "
            <<overdraftLimit<<endl;

        cout<<"Business Name: "
            <<businessName<<endl;

        cout<<"Status: "
            <<status<<endl;
    }
};

class TransactionService{
public:
    static void transfer(Account* sender,
                         Account* receiver,
                         double amount){

        sender->withdraw(amount);

        receiver->deposit(amount);

        Transaction senderTransaction(
            10,
            "Transfer Sent",
            amount,
            "12-05-2026",
            "Success",
            sender,
            receiver);

        Transaction receiverTransaction(
            11,
            "Transfer Received",
            amount,
            "12-05-2026",
            "Success",
            sender,
            receiver);

        sender->addTransaction(
            senderTransaction);

        receiver->addTransaction(
            receiverTransaction);

        cout<<"Transfer Successful"<<endl;
    }
};

class Branch{
private:
    int branchId;
    string branchName;
    string IFSCCode;
    string address;

    vector<Employee*> employees;
    vector<Account*> accounts;

public:
    Branch(int id,
           string name,
           string ifsc,
           string addr){

        branchId=id;
        branchName=name;
        IFSCCode=ifsc;
        address=addr;
    }

    void addAccount(Account* acc){
        accounts.push_back(acc);
    }

    void showAccounts(){

        cout<<"Branch ID: "<<branchId<<endl;

        cout<<"Branch Name: "<<branchName<<endl;

        cout<<"IFSC: "<<IFSCCode<<endl;

        cout<<"Address: "<<address<<endl;

        cout<<endl;

        for(Account* acc:accounts){
            acc->displayAccount();
            cout<<endl;
        }
    }

    void addEmployee(Employee* emp){
        employees.push_back(emp);
    }

    void showEmployees();
};

class Bank{
private:
    int bankId;
    string bankName;

    vector<Branch*> branches;
    vector<Customer*> customers;
    vector<Employee*> employees;

public:
    Bank(int id,string name){
        bankId=id;
        bankName=name;
    }

    void addBranch(Branch* branch){
        branches.push_back(branch);
    }

    void addCustomer(Customer* customer){
        customers.push_back(customer);
    }

    void addEmployee(Employee* emp){
        employees.push_back(emp);
    }

    void showBranches(){

        cout<<"Bank: "<<bankName<<endl;

        cout<<endl;

        for(Branch* branch:branches){
            branch->showAccounts();
            cout<<endl;
        }
    }

    void showCustomers(){

        for(Customer* customer:customers){
            customer->showAccounts();
            cout<<endl;
        }
    }
};

class Employee{
private:
    int employeeId;
    string employeeName;
    string designation;
    double salary;

    Branch* branch;

public:
    Employee(int id,
             string name,
             string role,
             double sal,
             Branch* b){

        employeeId=id;
        employeeName=name;
        designation=role;
        salary=sal;

        branch=b;
    }

    void displayEmployee(){

        cout<<"Employee ID: "<<employeeId<<endl;

        cout<<"Employee Name: "<<employeeName<<endl;

        cout<<"Designation: "<<designation<<endl;

        cout<<"Salary: "<<salary<<endl;
    }
};

void Branch::showEmployees(){

    for(Employee* emp:employees){
        emp->displayEmployee();
        cout<<endl;
    }
}

void Customer::showAccounts(){

    cout<<"Customer ID: "<<customerId<<endl;

    cout<<"Customer Name: "<<fullName<<endl;

    for(Account* acc:accounts){
        acc->displayAccount();
        cout<<endl;
    }
}



class ATMCard{
private:
    long cardNumber;
    int cvv;
    int pin;

    string expiryDate;
    string cardStatus;
    string cardType;

    Account* linkedAccount;

public:
    ATMCard(long cno,
            int c,
            int p,
            string exp,
            string type,
            Account* acc){

        cardNumber=cno;
        cvv=c;
        pin=p;

        expiryDate=exp;
        cardStatus="Active";
        cardType=type;

        linkedAccount=acc;
    }

    bool validatePIN(int enteredPIN){

        if(cardStatus=="Blocked"){
            cout<<"Card Blocked"<<endl;
            return false;
        }

        if(pin!=enteredPIN){
            throw InvalidPINException();
        }

        return true;
    }

    void withdrawCash(int enteredPIN,double amount){

        try{

            if(validatePIN(enteredPIN)){

                linkedAccount->withdraw(amount);

                cout<<"Cash Withdrawn"<<endl;
            }
        }

        catch(exception &e){
            cout<<e.what()<<endl;
        }
    }

    void blockCard(){
        cardStatus="Blocked";
    }

    void displayCard(){

        cout<<"Card Number: "<<cardNumber<<endl;

        cout<<"Expiry Date: "<<expiryDate<<endl;

        cout<<"Card Type: "<<cardType<<endl;

        cout<<"Card Status: "<<cardStatus<<endl;
    }
};

class Notification{
public:
    virtual void send(string message)=0;

    virtual ~Notification(){}
};

class SMSNotification:public Notification{
private:
    string phoneNumber;
    string deliveryStatus;

public:
    SMSNotification(string phone){

        phoneNumber=phone;

        deliveryStatus="Pending";
    }

    void send(string message) override{

        deliveryStatus="Sent";

        cout<<"SMS Sent To: "
            <<phoneNumber<<endl;

        cout<<"Message: "
            <<message<<endl;

        cout<<"Delivery Status: "
            <<deliveryStatus<<endl;
    }
};

class EmailNotification:public Notification{
private:
    string emailAddress;
    string deliveryStatus;
    string subject;

public:
    EmailNotification(string email,string sub){
        emailAddress=email;
        subject=sub;
        deliveryStatus="Pending";
    }

    void send(string message) override{

        deliveryStatus="Sent";

        cout<<"Email Sent To: " <<emailAddress<<endl;

        cout << "Subject: " << subject << endl;

        cout<<"Message: "<<message<<endl;

        cout<<"Delivery Status: " <<deliveryStatus<<endl;
    }
};

class Loan{
protected:
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
         Customer* cust){

        loanId=id;

        loanType=type;

        loanAmount=amount;

        interestRate=rate;

        tenureYears=years;

        customer=cust;

        loanStatus="Pending";

        calculateEMI();
    }

    virtual ~Loan(){}

    virtual void calculateEMI(){

        double monthlyRate=
            interestRate/(12*100);

        int months=
            tenureYears*12;

        EMIAmount=
        (loanAmount*monthlyRate*
        pow(1+monthlyRate,months))/
        (pow(1+monthlyRate,months)-1);
    }

    void approveLoan(){
        loanStatus="Approved";
    }
    void rejectLoan(){
        loanStatus="Rejected";
        throw LoanRejectedException();
    }

    void displayLoan(){

        cout<<"Loan ID: "<<loanId<<endl;

        cout<<"Loan Type: "<<loanType<<endl;

        cout<<"Loan Amount: "<<loanAmount<<endl;

        cout<<"Interest Rate: "<<interestRate<<endl;

        cout<<"Tenure: "<<tenureYears<<" Years"<<endl;

        cout<<"EMI: "<<EMIAmount<<endl;

        cout<<"Status: "<<loanStatus<<endl;
    }
};

class HomeLoan:public Loan{
private:
    string propertyAddress;

public:
    HomeLoan(int id,
             double amount,
             double rate,
             int years,
             Customer* cust,
             string address)

    : Loan(id,
           "Home Loan",
           amount,
           rate,
           years,
           cust){

        propertyAddress=address;
    }

    void displayLoan(){

        cout<<"Home Loan"<<endl;

        Loan::displayLoan();

        cout<<"Property Address: "
            <<propertyAddress<<endl;
    }
};

class CarLoan:public Loan{
private:
    string carModel;

public:
    CarLoan(int id,
            double amount,
            double rate,
            int years,
            Customer* cust,
            string model)

    : Loan(id,
           "Car Loan",
           amount,
           rate,
           years,
           cust){

        carModel=model;
    }

    void displayLoan(){

        cout<<"Car Loan"<<endl;

        Loan::displayLoan();

        cout<<"Car Model: "
            <<carModel<<endl;
    }
};

class PersonalLoan:public Loan{
private:
    string purpose;

public:
    PersonalLoan(int id,
                 double amount,
                 double rate,
                 int years,
                 Customer* cust,
                 string p)

    : Loan(id,
           "Personal Loan",
           amount,
           rate,
           years,
           cust){

        purpose=p;
    }

    void displayLoan(){

        cout<<"Personal Loan"<<endl;

        Loan::displayLoan();

        cout<<"Purpose: "
            <<purpose<<endl;
    }
};

class FixedDepositAccount:public Account{
private:
    double FDAmount;
    string maturityDate;
    double FDInterestRate;
    int tenureMonths;

public:
    FixedDepositAccount(long accNo,
                        double amount,
                        double rate,
                        int tenure,
                        string maturity,
                        string date,
                        Branch* b,
                        Customer* cust)

    : Account(accNo,
              amount,
              "Fixed Deposit",
              date,
              b,
              cust){

        FDAmount=amount;
        FDInterestRate=rate;
        tenureMonths=tenure;
        maturityDate=maturity;
    }

    void withdraw(double amount) override{
        throw PrematureWithdrawalException();
    }

    double calculateMaturityAmount(){

        double maturityAmount=
        FDAmount*
        pow((1+FDInterestRate/100),
        ((double)tenureMonths/12));

        return maturityAmount;
    }

    void displayAccount() override{

        cout<<"Account Number: "
            <<accountNumber<<endl;

        cout<<"FD Amount: "
            <<FDAmount<<endl;

        cout<<"FD Interest Rate: "
            <<FDInterestRate<<endl;

        cout<<"Tenure Months: "
            <<tenureMonths<<endl;

        cout<<"Maturity Date: "
            <<maturityDate<<endl;

        cout<<"Maturity Amount: "
            <<calculateMaturityAmount()
            <<endl;

        cout<<"Status: "
            <<status<<endl;
    }
};

class AccountFactory{
public:

    static Account* createSavingsAccount(
        long accNo,
        double bal,
        double minBal,
        double rate,
        double dailyLimit,
        string date,
        Branch* b,
        Customer* cust){

        return new SavingsAccount(
            accNo,
            bal,
            minBal,
            rate,
            dailyLimit,
            date,
            b,
            cust);
    }

    static Account* createCurrentAccount(
        long accNo,
        double bal,
        double limit,
        string business,
        string date,
        Branch* b,
        Customer* cust){

        return new CurrentAccount(
            accNo,
            bal,
            limit,
            business,
            date,
            b,
            cust);
    }

    static Account* createFDAccount(
    long accNo,
    double amount,
    double rate,
    int tenure,
    string maturity,
    string date,
    Branch* b,
    Customer* cust){

    return new FixedDepositAccount(
        accNo,
        amount,
        rate,
        tenure,
        maturity,
        date,
        b,
        cust);
}
};

