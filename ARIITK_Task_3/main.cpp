#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
//Done including all the necessary Libraries
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <cmath>

using namespace std;

//-------------Forward declaration of classes---------------
class Account;
class Employee;
class Loan;
class Branch;
class Customer;
class Account;
class Transaction;
//----------------------------------------------------------

//----------------------------Aesthetics--------------------
void successMessage(string message){

    cout << endl;
    cout << "\033[1;3;32m"
         << message
         << "\033[0m";
}
//----------------------------------------------------------
class Bank{

private:
    int BankId;
    string bankName;
    vector<Branch*> branches; //A vector having addresses of objects in Branch class
    vector<Customer*> customers;
    vector<Employee*> employees;

public:
    Bank(int BankID, string bankName){
        this->BankId = BankID;
        this->bankName = bankName; 
        successMessage("Bank Added Successfully!");     
    }

    void addBranch(Branch* branch){
        branches.push_back(branch);
        successMessage("Branch Added Successfully!");
    }

    void addCustomer(Customer* customer){
        customers.push_back(customer);
        successMessage("Customer Added Successfully!");
    }
    void addEmployee(Employee* employee){
        employees.push_back(employee);
        successMessage("Employee Added Successfully!");
    }
};


class Branch{
private:
    int branchID;
    string branchName;
    string IFSCCode;
    string address;
    vector<Account*> accounts;
    vector<Employee*> employees;

public:
    Branch(int branchID, string branchName, string IFSCCode, string address){
        this->branchID = branchID;
        this->branchName = branchName;
        this->IFSCCode = IFSCCode;
        this->address = address;
    }

    void addAccount(Account* account){
        accounts.push_back(account);
        successMessage("Account Added Successfully!");
    }

    void addEmployee(Employee* employee){
        employees.push_back(employee);
        successMessage("Employee Added Successfully!");
    }
};


class Customer{

private:
    static int nextCID;
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
    Customer(){
        customerId = nextCID++;
        successMessage("CustomerID Created Successfully!");
        cout << endl;
        cout << "Your CustomerID is: " << customerId << endl;
    }

    void addCustomerDetails(){
        cout << "Enter your full Name: ";
        cin.ignore();
        getline(cin, fullName);
         
        cout << "Enter DOB (DD/MM/YYYY): ";
        getline(cin, dob);

        cout << "Enter your Gender: ";
        getline(cin, gender);

        cout << "Enter your Mobile Number (+91): ";
        getline(cin, mobileNumber);

        cout << "Enter your E-Mail address: ";
        getline(cin, email);

        cout << "Enter your Address: ";
        getline(cin, address);

        cout << "Enter your Aadhaar Number: ";
        getline(cin, aadhaarNumber);

        cout << "Enter your PAN Number: ";
        getline(cin, PANNumber);

        successMessage("Customer Added Successfully!");

    }

    void addAccount(Account* account){
        accounts.push_back(account);
    }

    void addLoan(Loan* loan){
        loans.push_back(loan);
    }

    void requestLoan(string type, double amount, int tenure){
        static int globalLoanId = 700001;

        Loan* newLoan = new Loan(globalLoanId++, type, amount, tenure, this);
        newLoan->loanStatus = "PENDING";
        

        double r = (newLoan->interestRate[type])/1200;
        int n = 12*tenure; 
        newLoan->EMIAmount = (amount * r * pow(1+r, n)) / (pow(1+r, n) - 1);

        loans.push_back(newLoan);

        cout << "\n\033[1;33mLoan Request Submitted!\033[0m" << endl;
        cout << "Loan ID: " << newLoan->loanId << " is currently PENDING review." << endl;
        cout << "Loan Type: " << newLoan->loanType << endl;
        cout << "Estimated Monthly EMI: Rs." << newLoan->EMIAmount << endl;
        cout << "Tenure: " << tenure << "years";
    }
        
    
};

//----------Initialised the nextID---------------
int Customer::nextCID = 250001;
//-----------------------------------------------

class Employee{
private:
    static int nextEID;
    int employeeId;
    string employeeName;
    string designation;
    double salary;
    Branch* branch;
public:
    Employee(Branch* branch = nullptr){
        employeeId = nextEID++;

        this->branch = branch;

        cout << "Enter Employee Name: ";
        getline(cin, employeeName);
        cout << "Enter Designation: ";
        getline(cin, designation);
        cout << "Enter Salary: ";
        cin >> salary;
        cin.ignore();
        successMessage("Employee Added Successfully!");
    }
    
    void setBranch(Branch* branch){//In case if nothing passed to the constructor
        this->branch = branch;
        successMessage("Branch Added to the Employee Successfully!");
    }

    void processLoan(Customer* customer ){
        


    }


};
//---------------------------------------------
int Employee::nextEID = 910001;
//---------------------------------------------

class Account{ //An abstract class for Different accounts
protected:
    static long nextAccountNumber;
    long accountNumber;
    string accountType;
    double balance;
    string dateOpened;
    string status; //Active or blocked
    Branch* branch;
    Customer* customer;
    vector<Transaction*> transactions;

public:
    // virtual void CreateAccount() = 0;
    virtual bool Withdraw(double amount) = 0;
    virtual bool Deposit(double amount) = 0;
    virtual void displayAccountDetails(long AccountNumber) = 0;
    virtual void addTransaction(Transaction* t){
        transactions.push_back(t);
    }
    virtual long GetAccountNumber(long AccountNumber) = 0;
    



    //Account Blocked exception
};



class SavingsAccount : public Account{
private:
    double interestRate;
    double minimumBalance;

public:
    bool Withdraw(double amount){

        if(this->balance < amount){
            cout << "Insufficient Balance!";
            cout << "Your current balance is: " << this->balance;
            return false;
        }

        this->balance = this->balance - amount;
        cout << "\033[1;3;32mRs.\033[0m" << amount << "\033[1;3;32mWithdrawn Successfully\033[0m";
        cout << "Your current balance is: " << this->balance;

        Transaction* t = new Transaction("WITHDRAWAL", amount, this, nullptr, "SUCCESS");
        transactions.push_back(t);

        return true;

        
    }

    bool Deposit(double amount){
        this->balance = this->balance + amount;
        successMessage("Amount Deposited Successfully!");
        cout << "Your current balance is: " << this->balance;

        Transaction* t = new Transaction("DEPOSIT", amount, nullptr, this, "SUCCESS");
        transactions.push_back(t);

        return true;
    }

    long GetAccountNumber(long accountNumber){


        return accountNumber;
    }
};


class CurrentAccount : public Account{ //used by business entities
private:
    double overdraftLimit; //short term bank loan
    string businessName;

public:
    bool Withdraw(double amount){
        if((this->balance + overdraftLimit) >= amount){
            this->balance = this->balance - amount;
            cout << "\033[1;3;32mRs.\033[0m" << amount << "\033[1;3;32mWithdrawn Successfully\033[0m";
            cout << "Your current balance is: " << this->balance;
        
        } else {
            cout << "Insufficient Balance!";
            cout << "Your current balance is: " << this->balance;
        } 
        
    }

    bool Deposit(double amount){
        this->balance = this->balance + amount;
        successMessage("Amount Deposited Successfully!");
        cout << "Your current balance is: " << this->balance;
    }


};


class FixedDepositAccount : public Account{
private:
    double FDAmount;
    string maturityDate;
    double FDInterestRate;
    int tenureMonths;
    
    static map<int, double> InterestRate;

public:
    FixedDepositAccount(double FDAmount, int tenureMonths){
        InterestRate[1] = 3.00;
        InterestRate[2] = 3.05;
        InterestRate[6] = 4.90;
        InterestRate[12] = 5.90;
        InterestRate[24] = 6.25;
        InterestRate[36] = 6.40;
        InterestRate[60] = 6.30;
        InterestRate[120] = 6.05;
    }
};



class Transaction{//Insufficient Balance and PIN exception
private:
    int transactionId;
    string transactionType;
    double amount;
    string transactionDate;
    Account* senderAccount;
    Account* receiverAccount;
    string status;

public:
    Transaction(string transactionType, double amount, Account* senderAccount, Account* receiverAccount, string status){

    }



};


class TransactionService{//Need to handle duplicate logging with Deposit, Withdraw, Transfer

public:
    void Transfer(Account* sender, Account* receiver, double amount){

        if(!sender->Withdraw(amount)){
            cout << "Transaction Failed!\n";
            cout << "Insufficient Balance!";

            Transaction* t = new Transaction("TRANSFER", amount, sender, receiver, "FAILED");
            sender->addTransaction(t);
            receiver->addTransaction(t);

            return;
        }
        else if(!receiver->Deposit(amount)) {
            sender->Deposit(amount); // rollback
            cout << "Transaction Failed during deposit!\n";

            Transaction* t = new Transaction("TRANSFER", amount, sender, receiver, "FAILED");
            sender->addTransaction(t);
            receiver->addTransaction(t);
            
            return;
        }
        
        else {
            
            Transaction* t = new Transaction("TRANSFER", amount, sender, receiver, "SUCCESS");
            sender->addTransaction(t);
            receiver->addTransaction(t);
        }
    }

};


    class Loan{//Loan rejected exception
    public:
        static map<string, double> interestRate;

        int loanId;
        string loanType;
        double loanAmount; 
        int tenureYears;
        double EMIAmount;
        string loanStatus;
        Customer* customer;


        Loan(int loanID, string loanType, double amount, int tenureYears, Customer* customer){
            interestRate["Personal"] = 9.99;
            interestRate["Vehicle"] = 7.45;
            interestRate["Education"] = 7.45;
            interestRate["Gold"] = 10.2;
            interestRate["Home"] = 7.1;
            interestRate["Business"] = 9.5;
            interestRate["Agricultre"] = 5.5;
        }
    };


class Notification {
public:
    string message;
    string deliveryStatus;
    string phoneNumber;
    string emailAddress;
    string subject;
    string mode; 

   
    Notification(string phoneNumber, string message) {
        mode = "SMS";
        deliveryStatus = "PENDING";
        this->phoneNumber = phoneNumber;
        this->message = message;

        sendMessage();
    }

    // Constructor 2: For EMAIL
    Notification(string emailAddress, string subject, string message) {
        mode = "EMAIL";
        deliveryStatus = "PENDING";
        this->emailAddress = emailAddress;
        this->subject = subject;
        this->message = message;

        sendMessage();
    }

    
    void sendMessage() {//Thinking to check for a valid number
        deliveryStatus = "SENT!";
        
        if (mode == "SMS") {
            cout << "\nSMS sent To: " << phoneNumber << "\n";
            cout << "Message: " << message << "\n";
            cout << "Status: " << deliveryStatus << "\n";
        } 
        else if (mode == "EMAIL") {
            cout << "\nEmail sent To: " << emailAddress << endl;
            cout << "Subject: " << subject << endl;
            cout << "Body: " << message << endl;
            cout << "Status: " << deliveryStatus << endl;
        }
    }
};


class ATMCard{

private:
    static int tries; //TO manage unauthorized access and blocking account

    long cardNumber;
    int CVV;
    string expiryDate;
    int PIN;
    string cardType;
    string cardStatus;
    Account* linkedAccount;

    public:

    ATMCard(){
        tries = 5;
    }

    void RequestATMCard(string cardType){
        long AccountNum;
        cout << "Enter your Account Number: ";
        cin >> AccountNum;

        vector<Account*> accounts;
        
        for(auto acc : accounts){
            if(acc->GetAccountNumber == AccountNum){
                
            }
        }
        
    }

    void Transaction(string cardType, string TransactionType){
        if(TransactionType == "ATM"){
            if(cardType == "Credit"){
                cout << "Enter Credit Card"

            } else if (){

            }

        }

        else if (TransactionType == "Online"){

        }

    }

};









int main(){
    
    // cout << "\033[1;3;32mWELCOME TO SMARTBANK!\033[0m" << endl;
    // sndPlaySound(TEXT("C:\\Users\\DELL 5420\\OneDrive\\Desktop\\ARIITK Task 3\\khachink.wav"), SND_ASYNC);
    return 0;
}