#include <iostream>
#include <cmath>
#include <vector>
#include <string>
using namespace std;

struct Date 
{
    int day;
    int month;
    int year;
};

class Bank;
class Branch;
class Account;
class Customer;
class Transaction;
class Loan;
class ATMCard;
class Employee;
class Notification;

class Branch{
public:
    int branchId;
    string branchName;
    string IFSCCode;
    string address;
private:
    vector<Account*> accounts;
    vector<Employee*> employees;
public:
    Branch(
        int id,
        string name,
        string ifsc,
        string addr
    ) {

        branchId = id;
        branchName = name;
        IFSCCode = ifsc;
        address = addr;
    }

    void displayBranch() {

        cout << "\nBranch Details\n";

        cout << "Branch ID: " << branchId << endl;
        cout << "Branch Name: " << branchName << endl;
        cout << "IFSC Code: " << IFSCCode << endl;
        cout << "Address: " << address << endl;
    }
    void addAccount(Account* acc) {
        accounts.push_back(acc);
    }
    void addEmployee(Employee* emp)
    {
        employees.push_back(emp);
    }

};

class Customer{
public:
    int customerId;
    string fullName;
    Date dob;
    string gender;
    string mobileNumber;
    string email;
    string address;
    string aadhaarNumber;
    string PANNumber;

    Customer(
        int id,
        string name,
        Date birth,
        string gen,
        string mobile,
        string mail,
        string addr,
        string aadhaar,
        string pan
    ) {

        customerId = id;
        fullName = name;
        dob = birth;
        gender = gen;
        mobileNumber = mobile;
        email = mail;
        address = addr;
        aadhaarNumber = aadhaar;
        PANNumber = pan;
    }

    void displayCust()
    {
    cout << "\nCustomer Details\n";
    cout << "Customer Name: " << this->fullName << endl;
    cout << "Mobile Number: " << this->mobileNumber << endl;
    cout << "Email: " << this->email << endl;
    }
private:
    vector<Account*> accounts;
    vector<Loan*> loans;
public:
    void addAcc(Account* acc)
    {
        accounts.push_back(acc);
    }
    void addLoan(Loan* custloan)
    {
        loans.push_back(custloan);
    }
    void displayAllAcc();
    void displayAllLoans();
};

class Bank{
public:
    int bankId;
    string bankName;
    
    void displayBank()
    {
        cout<<"Bank Id: "<<bankId<<endl;
        cout<<"Bank Name: "<<bankName<<endl;
    }
private:
    vector<Branch> branches;
    vector<Customer> customers;
    vector<Employee*> employees;
public:
    void addCustomer(Customer c) {

        customers.push_back(c);
    }
    void displayAllCustomers() {

        cout << "\n====== CUSTOMER LIST ======\n";

        for(int i = 0; i < customers.size(); i++) {

            customers[i].displayCust();
        }
    }
    
    void addBranch(Branch b) {

        branches.push_back(b);
    }

    void displayAllBranches() {

        cout << "\n====== BRANCH LIST ======\n";

        for(int i = 0; i < branches.size(); i++) {

            branches[i].displayBranch();
        }
    }
    void addEmp(Employee* emp) {

        employees.push_back(emp);
    }
    void displayAllEmployees();
};

class Notification{
public: 
    string message;
    void sendNotification(string msg)
    {
        message= msg;
    }
    virtual void display()=0;
};

class SMSNotification: public Notification{
public:
    string phoneNumber;
    string message;
    string deliveryStatus;
    void display()
    {
        cout<< "SMS: "<< Notification::message<< endl;
        deliveryStatus= "Sent";
    }
};

class EmailNotification: public Notification{
public:
    string emailAddress;
    string message;
    string deliveryStatus;
    string subject;
    void display()
    {
        subject= Notification::message;
        cout<< "Email: "<< subject<< endl;
        deliveryStatus= "Sent";
    }
};

class Account{
public:
    long accountNumber;
    string accountType;
    double balance;
    Date dateOpened;
    string status;
    Branch* branch;
    Customer* customer;
private:
    vector<Transaction*> transactions;
public:
    Account(long accno, string type, double bal, Date date, string stat, Branch* b, Customer* cust)
    {
        accountNumber=accno;
        accountType=type;
        balance=bal;
        dateOpened=date;
        status= stat;
        branch=b;
        customer=cust;
        if (status == "Blocked")
        {
            SMSNotification n3;

            n3.phoneNumber = customer->mobileNumber;

            n3.sendNotification("Account Blocked");

            n3.display();
        }
    }
    void displayAcc()
    {
        cout << "\nAccount Details\n";
        cout << "Account Number: " << this->accountNumber << endl;
        cout << "Account Type: " << this->accountType << endl;
        cout << "Balance: " << this->balance << endl;
        cout << "Status: " << this->status << endl;
    }
    void addTransaction(Transaction* t)
    {
        transactions.push_back(t);
    }

};

class SavingsAccount: public Account{
public:
    double interestRate;
    double minimumBalance;
    SavingsAccount(
        long accno,
        string type,
        double bal,
        Date date,
        string stat,
        Branch* b,
        Customer* cust,
        double rate,
        double minbal
    ): Account(accno, type, bal, date, stat, b, cust)
    {
        interestRate= rate;
        minimumBalance=minbal;
    }
    void displaySav()
    {
    cout << "Interest Rate: " << this->interestRate << "%" << endl;
    cout << "Minimum Balance: " << this->minimumBalance << endl;
    }
    int calcInterest()
    {
        int i= (this->balance)*(this->interestRate)/100.0;
        return i;
    }
};

class CurrentAccount: public Account{
public:
    double overdraftLimit;
    string businessName;
    void displayCurr()
    {
    cout << "Overdraft Limit: " << this->overdraftLimit << endl;
    cout << "Business Name: " << this->businessName<< endl;
    }
};

class FixedDepositAccount: public Account{
public:
    double FDAmount;
    double FDInterestRate;
    Date maturityDate;
    int tenureMonths;
    FixedDepositAccount(
        long accno,
        string type,
        double bal,
        Date date,
        string stat,
        Branch* b,
        Customer* cust,
        double rate,
        double amount,
        int months,
        Date maturity
    ): Account(accno, type, bal, date, stat, b, cust)
    {
        FDInterestRate= rate;
        FDAmount=amount;
        maturityDate=maturity;
        tenureMonths=months;
    }
    void displayFD()
    {
        cout << "Fixed Deposit Amount: " << this->FDAmount << "%" << endl;
        cout << "Interest Rate: " << this->FDInterestRate << endl;
        cout << "Tenure Months: " << this->tenureMonths << endl;
        cout << "Maturity Amount: " << (this->FDAmount)*pow((1+(this->FDInterestRate)/100),(this->tenureMonths)/12);

    }
};

class Transaction{
public:
    int transactionId;
    string transactionType;
    double amount;
    Date transactionDate;
    Account* senderAccount;
    Account* receiverAccount;
    string status;
    Transaction(int id, string type, double amt, Date date, Account* sacc, Account* racc)
    {
        transactionId= id;
        transactionType= type;
        amount= amt;
        transactionDate= date;
        senderAccount= sacc;
        receiverAccount= racc;
    }
    void verifyTran(ATMCard* card, int enteredPIN);
    void transaction(){
        if (transactionType== "withdraw"){
        if (senderAccount->balance>=amount)
        {
            senderAccount->balance-=amount;
            status= "Successful";
            cout << "\nTransaction successful!\n"<< endl;
        }
        else
        {
            status= "Pending";
            cout<< "Insufficient Balance"<< endl;

        }
    }
        else if (transactionType=="deposit")
        {
            receiverAccount->balance+=amount;
            status= "Successful";

            cout << "\nTransaction successful!\n"<< endl;
        }
        else{
            if (senderAccount->balance>=amount)
        {
            senderAccount->balance-=amount;
            receiverAccount->balance+=amount;
            status= "Successful";

            cout << "\nTransaction successful!\n"<< endl;
        }
        else
        {
            status= "Pending";
            cout<< "Insufficient Balance"<< endl;

        }
        }
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

    Loan(
        int id,
        string type,
        double amt,
        double rate,
        int years,
        double emi,
        string status,
        Customer* cust
    )
    {
        loanId= id;
        loanType= type;
        loanAmount = amt;
        interestRate= rate;
        tenureYears= years;
        EMIAmount= emi;
        loanStatus=status;
        customer = cust;
        if (loanStatus== "Rejected")
        {
            EmailNotification n1;

            n1.emailAddress = customer->email;

            n1.subject = "Loan Status";

            n1.sendNotification("Loan Rejected");

            n1.display();
        }
    }
    void displayLoan()
    {
        cout << "\nLoan Details\n";
        cout << "Loan Id: " << this->loanId << endl;
        cout << "Loan Type: " << this->loanType << endl;
        cout << "Loan Amount: " << this->loanAmount<< endl;
        cout << "EMI Amount: " << this->EMIAmount << endl;
        cout << "Status: " << this->loanStatus << endl;
    }
};

class ATMCard{
protected:
    long cardNumber;
    int CVV;
    Date expiryDate;
    int PIN;
public:
    string cardType;
    string cardStatus;
    Account* linkedAccount;
    ATMCard(long cardNo, int cvv, Date expiry, int pin, string type, string status, Account* acc)
    {
        cardNumber= cardNo;
        CVV= cvv;
        expiryDate= expiry;
        PIN= pin;
        cardType= type;
        cardStatus= status;
        linkedAccount= acc;
    }
    bool verifyPIN(int pin)
    {
        return pin==this->PIN;
    }
};

class Employee{
public: 
    int employeeId;
    string employeeName;
    string designation;
    double salary;
    Branch* branch;
    Employee(int id, string name, string desig, double sal, Branch* b)
    {
        employeeId=id;
        employeeName= name;
        designation=desig;
        salary=sal;
        branch=b;
    }
    void displayEmp()
    {
        cout << "\nEmployee Details\n";
        cout << "Employee Id: " << this->employeeId << endl;
        cout << "Employee Name: " << this->employeeName << endl;
        cout << "Designation: " << this->designation<< endl;
        cout << "Salary: " << this->salary << endl;
        cout << "Branch: " << branch->branchName << endl;
    }
};


void Customer::displayAllAcc()
{
    cout << "\n" << this->fullName <<":"  <<endl;
    cout << "====== ACCOUNTS LIST ======\n";

    for(Account* acc : accounts)
    {
        acc->displayAcc();
    }
}
void Customer::displayAllLoans()
{
    cout << "\n" << this->fullName <<":"  <<endl;
    cout << "====== LIST OF LOANS ======\n";

    for(Loan* loan : loans)
    {
        loan->displayLoan();
    }
}
void Bank::displayAllEmployees()
{
    cout << "\n====== EMPLOYEES LIST ======\n";

    for(Employee* emp : employees)
    {
        emp->displayEmp();
    }
}
void Transaction::verifyTran(ATMCard* card, int enteredPIN)
{
    if(!(card->verifyPIN(enteredPIN)))
    {
        SMSNotification n2;

        n2.phoneNumber = card->linkedAccount->customer->mobileNumber;

        n2.sendNotification("Invalid PIN");

        n2.display();
    }
    else
    {
        transaction();
    }
}
int main(){
    Bank SBI;
    SBI.bankId = 101;
    SBI.bankName = "State Bank of India";
    SBI.displayBank();

    Branch branch1(10101, "MG Road Branch", "SBI0001234", "Bangalore");
    Branch branch2(10102, "JP Nagar Branch", "SBI0009876", "Bangalore");
    
    SBI.addBranch(branch1);
    SBI.addBranch(branch2);

    branch1.displayBranch();
    branch2.displayBranch();
    
    Customer A1(111, "Rahul Sharma", {25,11,2000}, "Male", "9894598909", "rahul@gmail.com", "Bangalore", "123245638980", "AHBHBB9890F");
    Customer A2(112, "Glen Holland", {9,8,1976}, "Male", "8965536279", "glen@gmail.com", "Jamshedpur", "143289544509", "HGDUT8987A");
    
    SBI.addCustomer(A1);
    SBI.addCustomer(A2);
    
    SavingsAccount acc1(77987437892, "Savings", 50000, {15, 01, 2025}, "Active", &branch1, &A1, 3.5, 1000);
    FixedDepositAccount acc2(66907873932, "Fixed Deposit", 200000, {12,10,2024}, "Active", &branch2, &A2, 4.5, 200000,
    18, {12,4,2025});

    cout<<"\nInterest on account 1: "<<acc1.calcInterest()<<endl;
    branch1.addAccount(&acc1);
    branch2.addAccount(&acc2);
    A1.addAcc(&acc1);
    A2.addAcc(&acc2);
    A1.displayAllAcc();
    A2.displayAllAcc();

    ATMCard A1_card(20948976, 789, {31, 1, 2030}, 4532, "Debit", "Active", &acc1);
    ATMCard A2_card(98347523, 245, {30, 9, 2047}, 8436, "Credit", "Active", &acc2);

    Transaction t1(455501, "Transfer", 25000, {21, 04, 2026}, &acc1, &acc2);

    t1.verifyTran(&A1_card, 4532);
    acc1.addTransaction(&t1);
    acc2.addTransaction(&t1);
    A1.displayAllAcc();
    A2.displayAllAcc();

    Loan l1(43092, "Home", 400000, 8, 5, 7000, "Accepted", &A2);
    l1.displayLoan();
    A2.addLoan(&l1);
    A2.displayAllLoans();

    Employee emp1(90992, "Gary Williamson", "Manager", 30000, &branch1);
    emp1.displayEmp();
    branch1.addEmployee(&emp1);
    SBI.addEmp(&emp1);

    SBI.displayAllBranches();
    SBI.displayAllCustomers();
    SBI.displayAllEmployees();
    
    return 0;
}