#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

class Bank;
class Branch;
class Customer;
class Account;
class SavingsAccount;
class CurrentAccount;
class FixedDepositAccount;
class Transaction;
class Loan;
class ATMCard;
class Employee;
class Notification;
class SMSNotification;
class EmailNotification;
class InsufficientBalanceException;
class InvalidPINException;
class AccountBlockedException;
class LoanRejectedException;

class InsufficientBalanceException: public runtime_error {
    public:
        InsufficientBalanceException(): runtime_error("Error: Insufficient balance") {}
};

class InvalidPINException: public runtime_error {
    public:
        InvalidPINException(): runtime_error("Error: Invalid pin"){}
};

class AccountBlockedException: public runtime_error {
    public:
        AccountBlockedException(): runtime_error("Error: This account has been blocked"){}
};

class LoanRejectedException: public runtime_error {
    public:
        LoanRejectedException(): runtime_error("Error: Loan was rejected"){}
};

class Bank {
    public:
        int bankId;
        string bankName;
        vector<Branch*> branches;
        vector<Customer*> customers;
        vector<Employee*> employees;
};

class Branch {
    public:
        int branchId;
        string branchName;
        string IFSCCode;
        string address;
        vector<Account*> accounts;
        vector<Employee*> employees;
};

class Customer {
    public:
        int customerId;
        string fullName;
        tm dob;
        string gender;
        string mobileNumber;
        string email;
        string address;
        string aadhaarNumber;
        string PANNumber;
        vector<Account*> accounts;
        vector<Loan*> loans;
        double cibilScore;
};

class Account {
    public:
        long accountNumber;
        string accountType;
        double balance;
        tm dateOpened;
        string status;
        Branch* branch;
        Customer* customer;
        vector<Transaction*> transactions;

        virtual void deposit(double amt) {
            balance += amt;
        };

        virtual void withdraw(double amt) = 0;

        void addTransaction(Transaction* t) {
            transactions.push_back(t);
        };

        virtual void showDetails() {
            cout << "Account Number: " << accountNumber << endl;
            cout << "Balance: " << balance << endl;
        };
};

class SavingsAccount: public Account {
    public:
        double interestRate;
        double minimumBalance;

        void withdraw(double amt) override {
            if (balance - amt < minimumBalance) {throw InsufficientBalanceException();}
            else {
                balance -= amt;
            }
        };

        void showDetails() override {
            Account::showDetails();
            cout << "Interest rate: " << interestRate << endl;
            cout << "Minimum balance" << minimumBalance << endl;
        };
};

class CurrentAccount: public Account {
    public:
        double overdraftLimit;
        string businessName;

        void withdraw(double amt) override {
            if (balance - amt < -overdraftLimit) {throw InsufficientBalanceException();}
            else {
                balance -= amt;
            }
        };

        void showDetails() override {
            Account::showDetails();
            cout << "Overdraft limit: " << overdraftLimit << endl;
            cout << "Business name: " << businessName << endl;
        };
};

class FixedDepositAccount: public Account {
    public:
        double FDAmount;
        tm maturityDate;
        double FDInterestRate;
        int tenureMonths;

        void withdraw(double amt) override {
            if (amt > FDAmount) {throw InsufficientBalanceException();};
            if (mktime(&maturityDate) < time(nullptr)) {
                balance += FDAmount * pow((1 + 1.0*FDInterestRate/100), tenureMonths/12);
                balance -= amt;
                status = "Closed";
            }
            else {
                FDAmount -= amt;
            };
        };

        void showDetails() override {
            Account::showDetails();
            cout << "FD Amount: " << FDAmount << endl;
            cout << "FD Interest Rate: " << FDInterestRate << endl;
            cout << "Tenure: " << tenureMonths << " months" << endl;
        };
};

class Transaction {
    public:
        int transactionId;
        string transactionType;
        double amount;
        tm transactionDate;
        Account* senderAccount = nullptr;
        Account* receiverAccount = nullptr;
        string status = "Pending";

        bool executeTransaction() {
            try {
                if (transactionType == "Withdraw") {
                    senderAccount->withdraw(amount);
                }
                else if (transactionType == "Deposit") {
                    receiverAccount->deposit(amount);
                }
                else if (transactionType == "Transfer") {
                    senderAccount->withdraw(amount);
                    receiverAccount->deposit(amount);
                };
                status = "Success";

                if (senderAccount != nullptr) {
                    senderAccount->transactions.push_back(this);
                }
                if (receiverAccount != nullptr) {
                    receiverAccount->transactions.push_back(this);
                }

                return true;

            } catch (exception& e) {
                status = "Failed";
                cout << e.what() << endl;

                if (senderAccount != nullptr) {
                    senderAccount->transactions.push_back(this);
                }
                if (receiverAccount != nullptr) {
                    receiverAccount->transactions.push_back(this);
                }

                return false;
            };
        };
};

class Loan {
    public:
        int loanId;
        string loanType;
        double loanAmount;
        double interestRate;
        int tenureMonths;
        double EMIAmount;
        string loanStatus = "Pending";
        Customer* customer;

        void apply() {
            if (customer->cibilScore < 600 || customer->loans.size() > 0) {
                loanStatus = "Rejected";
                throw LoanRejectedException();
            };

            loanStatus = "Approved";

            double emi;
            emi = (loanAmount * interestRate/12/100 * pow(1+interestRate/12/100, tenureMonths))/(pow(1+interestRate/12/100, tenureMonths)-1);

            customer->loans.push_back(this);
            cout << "Loan accepted" << endl;
            cout << "Amount: " << loanAmount << " " << "EMI: " << emi << endl;
        }
};

class ATMCard {
    public:
        long cardNumber;
        int CVV;
        tm expiryDate;
        int PIN;
        string cardType;
        string cardStatus = "Active";
        Account* linkedAccount = nullptr;

        void checkStatus() {
            if (linkedAccount->balance < 500) {
                cardStatus = "Blocked";
                cout << "ATM card blocked due to low balance" << endl;
            } 
        }

        void checkPin(int enteredPin) {
            checkStatus();
            if (cardStatus == "Blocked") {
                throw AccountBlockedException();
            };

            if (enteredPin == PIN) {
                cout << "Successfully verified pin" << endl;
                linkedAccount->showDetails();
            }
            else {
                cout << "Wrong pin" << endl;
            };
        };
};

class Employee {
    public:
        int employeeId;
        string employeeName;
        string designation;
        double salary;
        Branch branch;
};

class Notification {
    public:
        string message;

    virtual void send() = 0;
};

class SMSNotification: public Notification {
    public:
        string phoneNumber;
        string message;
        string deliveryStatus = "Pending";

        void send() override {
            deliveryStatus = "Sent";
            cout << "Notification - SMS sent to " << phoneNumber << endl;
            cout << message << endl;
        }
};

class EmailNotification: public Notification {
    public:
        string emailAddress;
        string subject;
        string message;
        string deliveryStatus = "Pending";

        void send() override {
            deliveryStatus = "Sent";
            cout << "Notification - Email sent to " << emailAddress << endl;
            cout << subject << endl;
            cout << message << endl;
        }
};

int main() {

    Customer* cust1 = new Customer();
    cust1->customerId = 1001;
    cust1->fullName = "abc";
    cust1->aadhaarNumber = "1234567890";
    cust1->cibilScore = 750; 

    SavingsAccount* savings = new SavingsAccount();
    savings->accountNumber = 987654321;
    savings->balance = 5000.0;
    savings->minimumBalance = 1000.0;
    savings->interestRate = 4.0;
    savings->customer = cust1;
    
    cust1->accounts.push_back(savings);

    savings->showDetails();

    Transaction* t1 = new Transaction();
    t1->transactionId = 1;
    t1->transactionType = "Deposit";
    t1->amount = 2000.0;
    t1->receiverAccount = savings;
    
    if(t1->executeTransaction()) {
        savings->showDetails();
    }

    Transaction* t2 = new Transaction();
    t2->transactionId = 2;
    t2->transactionType = "Withdraw";
    t2->amount = 10000.0;
    t2->senderAccount = savings;
    
    t2->executeTransaction();

    Loan* autoLoan = new Loan();
    autoLoan->loanId = 55;
    autoLoan->loanAmount = 150000.0;
    autoLoan->interestRate = 9.5;
    autoLoan->tenureMonths = 36;
    autoLoan->customer = cust1;

    try {
        autoLoan->apply();
    } catch (exception& e) {
        cout << e.what() << endl;
    }

    delete cust1;
    delete savings;
    delete t1;
    delete t2;
    delete autoLoan;

    return 0;
}