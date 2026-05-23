#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <cmath>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

using namespace std;

// ------------- Forward Declarations ---------------
class Account;
class Branch;
class Customer;
class Transaction;

// ---------------------------- Aesthetics --------------------
void successMessage(string message){
    cout << endl;
    cout << "\033[1;3;32m" << message << "\033[0m" << endl;
}

// ------------------- Transaction Class -------------------
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
    Transaction(string transactionType, double amount, Account* senderAccount, Account* receiverAccount, string status) {
        static int globalTxId = 500001;
        this->transactionId = globalTxId++;
        this->transactionType = transactionType;
        this->amount = amount;
        this->senderAccount = senderAccount;
        this->receiverAccount = receiverAccount;
        this->status = status;
    }
};

// ------------------- Base Account Class -------------------
class Account {
protected:
    static long nextAccountNumber;
    long accountNumber;
    string accountType;
    double balance;
    string dateOpened;
    string status; 
    Branch* branch;
    Customer* customer;
    vector<Transaction*> transactions;

public:
    Account() {
        balance = 0.0;
        status = "ACTIVE";
    }
    virtual ~Account() {}
    virtual bool Withdraw(double amount) = 0;
    virtual bool Deposit(double amount) = 0;
    virtual void displayAccountDetails(long AccountNumber) = 0;
    virtual long GetAccountNumber(long AccountNumber) = 0;
    
    virtual void addTransaction(Transaction* t){
        transactions.push_back(t);
    }
};

// ------------------- Savings Account Class -------------------
class SavingsAccount : public Account {
private:
    double interestRate;
    double minimumBalance;

public:
    SavingsAccount() {
        accountType = "SAVINGS";
        interestRate = 4.0;
        minimumBalance = 1000.0;
    }

    bool Withdraw(double amount) override {
        if(this->balance < amount){
            cout << "Insufficient Balance! Your current balance is: " << this->balance << endl;
            return false;
        }
        this->balance -= amount;
        cout << "\033[1;3;32mRs.\033[0m" << amount << "\033[1;3;32m Withdrawn Successfully\033[0m" << endl;
        cout << "Your current balance is: " << this->balance << endl;

        Transaction* t = new Transaction("WITHDRAWAL", amount, this, nullptr, "SUCCESS");
        addTransaction(t);
        return true;
    }

    bool Deposit(double amount) override {
        this->balance += amount;
        successMessage("Amount Deposited Successfully!");
        cout << "Your current balance is: " << this->balance << endl;

        Transaction* t = new Transaction("DEPOSIT", amount, nullptr, this, "SUCCESS");
        addTransaction(t);
        return true;
    }

    void displayAccountDetails(long AccountNumber) override {
        cout << "Savings Account Number: " << accountNumber << " | Balance: " << balance << endl;
    }

    long GetAccountNumber(long accountNumber) override {
        return accountNumber;
    }
};

// ------------------- Current Account Class -------------------
class CurrentAccount : public Account {
private:
    double overdraftLimit; 
    string businessName;

public:
    CurrentAccount() {
        accountType = "CURRENT";
        overdraftLimit = 50000.0;
    }

    bool Withdraw(double amount) override {
        if((this->balance + overdraftLimit) >= amount){
            this->balance -= amount;
            cout << "\033[1;3;32mRs.\033[0m" << amount << "\033[1;3;32m Withdrawn Successfully\033[0m" << endl;
            cout << "Your current balance is: " << this->balance << endl;
            return true;
        } else {
            cout << "Insufficient Balance! Your current balance is: " << this->balance << endl;
            return false;
        } 
    }

    bool Deposit(double amount) override {
        this->balance += amount;
        successMessage("Amount Deposited Successfully!");
        cout << "Your current balance is: " << this->balance << endl;
        return true;
    }

    void displayAccountDetails(long AccountNumber) override {
        cout << "Current Account Number: " << accountNumber << " | Balance: " << balance << endl;
    }

    long GetAccountNumber(long accountNumber) override {
        return accountNumber;
    }
};

// ------------------- Fixed Deposit Class -------------------
class FixedDepositAccount : public Account {
private:
    double FDAmount;
    string maturityDate;
    double FDInterestRate;
    int tenureMonths;
    static map<int, double> InterestRate;

public:
    FixedDepositAccount(double FDAmount, int tenureMonths) {
        this->FDAmount = FDAmount;
        this->tenureMonths = tenureMonths;
        accountType = "FIXED_DEPOSIT";
        
        InterestRate[1] = 3.00;
        InterestRate[2] = 3.05;
        InterestRate[6] = 4.90;
        InterestRate[12] = 5.90;
        InterestRate[24] = 6.25;
        InterestRate[36] = 6.40;
        InterestRate[60] = 6.30;
        InterestRate[120] = 6.05;
    }

    bool Withdraw(double amount) override { return false; } 
    bool Deposit(double amount) override { return false; }
    void displayAccountDetails(long AccountNumber) override {}
    long GetAccountNumber(long accountNumber) override { return accountNumber; }
};
map<int, double> FixedDepositAccount::InterestRate;

// ------------------- Transaction Service -------------------
class TransactionService {
public:
    void Transfer(Account* sender, Account* receiver, double amount) {
        if(!sender->Withdraw(amount)){
            cout << "Transaction Failed!\nInsufficient Balance!\n";
            Transaction* t = new Transaction("TRANSFER", amount, sender, receiver, "FAILED");
            sender->addTransaction(t);
            return;
        }
        else if(!receiver->Deposit(amount)) {
            sender->Deposit(amount); 
            cout << "Transaction Failed during deposit!\n";
            Transaction* t = new Transaction("TRANSFER", amount, sender, receiver, "FAILED");
            sender->addTransaction(t);
            return;
        }
        else {
            Transaction* t = new Transaction("TRANSFER", amount, sender, receiver, "SUCCESS");
            sender->addTransaction(t);
            receiver->addTransaction(t);
        }
    }
};

// ------------------- Loan Class -------------------
class Loan {
public:
    static map<string, double> interestRate;

    int loanId;
    string loanType;
    double loanAmount; 
    int tenureYears;
    double EMIAmount;
    string loanStatus;
    Customer* customer;

    Loan(int loanID, string loanType, double amount, int tenureYears, Customer* customer) {
        this->loanId = loanID;
        this->loanType = loanType;
        this->loanAmount = amount;
        this->tenureYears = tenureYears;
        this->customer = customer;

        interestRate["Personal"] = 9.99;
        interestRate["Vehicle"] = 7.45;
        interestRate["Education"] = 7.45;
        interestRate["Gold"] = 10.2;
        interestRate["Home"] = 7.1;
        interestRate["Business"] = 9.5;
        interestRate["Agriculture"] = 5.5;
    }
};
map<string, double> Loan::interestRate;

// ------------------- Customer Class -------------------
class Customer {
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
    Customer() {
        customerId = nextCID++;
        successMessage("CustomerID Created Successfully!");
        cout << "Your CustomerID is: " << customerId << endl;
    }

    void addCustomerDetails() {
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

    void addAccount(Account* account) { accounts.push_back(account); }
    void addLoan(Loan* loan) { loans.push_back(loan); }

    void requestLoan(string type, double amount, int tenure) {
        static int globalLoanId = 700001;

        Loan* newLoan = new Loan(globalLoanId++, type, amount, tenure, this);
        newLoan->loanStatus = "PENDING";
        
        double r = (Loan::interestRate[type]) / 1200;
        int n = 12 * tenure; 
        newLoan->EMIAmount = (amount * r * pow(1 + r, n)) / (pow(1 + r, n) - 1);

        loans.push_back(newLoan);

        cout << "\n\033[1;33mLoan Request Submitted!\033[0m" << endl;
        cout << "Loan ID: " << newLoan->loanId << " is currently PENDING review." << endl;
        cout << "Loan Type: " << newLoan->loanType << endl;
        cout << "Estimated Monthly EMI: Rs." << newLoan->EMIAmount << endl;
        cout << "Tenure: " << tenure << " years" << endl;
    }
};
int Customer::nextCID = 250001;

// ------------------- Branch Class -------------------
class Branch {
private:
    int branchID;
    string branchName;
    string IFSCCode;
    string address;
    vector<Account*> accounts;

public:
    Branch(int branchID, string branchName, string IFSCCode, string address) {
        this->branchID = branchID;
        this->branchName = branchName;
        this->IFSCCode = IFSCCode;
        this->address = address;
    }

    void addAccount(Account* account) {
        accounts.push_back(account);
        successMessage("Account Added Successfully!");
    }
};

// ------------------- Employee Class -------------------
class Employee {
private:
    static int nextEID;
    int employeeId;
    string employeeName;
    string designation;
    double salary;
    Branch* branch;
public:
    Employee(Branch* b = nullptr) {
        employeeId = nextEID++;
        this->branch = b;

        cout << "Enter Employee Name: ";
        getline(cin, employeeName);
        cout << "Enter Designation: ";
        getline(cin, designation);
        cout << "Enter Salary: ";
        cin >> salary;
        cin.ignore();
        successMessage("Employee Added Successfully!");
    }
    
    void setBranch(Branch* b) {
        this->branch = b;
        successMessage("Branch Added to the Employee Successfully!");
    }
};
int Employee::nextEID = 910001;

// ------------------- ATMCard Class -------------------
class ATMCard {
private:
    static int tries; 
    long cardNumber;
    int CVV;
    string expiryDate;
    int PIN;
    string cardType;
    string cardStatus;
    Account* linkedAccount;

public:
    ATMCard() {
        tries = 5;
        cardNumber = 0;
        CVV = 0;
        PIN = 0;
        cardStatus = "ACTIVE";
        linkedAccount = nullptr;
    }

    void RequestATMCard(string type) {
        long AccountNum;
        cout << "Enter your Account Number: ";
        cin >> AccountNum;
        this->cardType = type;
        successMessage("ATM Card Requested Successfully!");
    }

    void TransactionExecution(string type, string TransactionType) {
        if(TransactionType == "ATM") {
            if(type == "Credit") {
                cout << "Processing Credit Card ATM Transaction..." << endl;
            } else {
                cout << "Processing Debit Card ATM Transaction..." << endl;
            }
        } else if (TransactionType == "Online") {
            cout << "Processing Online Gateway Transaction..." << endl;
        }
    }
};
int ATMCard::tries = 5;

// ------------------- Main Thread execution -------------------
int main() {
    cout << "\033[1;3;32mWELCOME TO SMARTBANK!\033[0m" << endl;
    return 0;
}