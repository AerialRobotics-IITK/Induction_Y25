#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Date {
private:
    int day;
    int month;
    int year;

public:
    
    Date() : day(1), month(1), year(2000) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    void displayDate() const {
        cout << day << "/" << month << "/" << year;
    }
};

class Bank {
    public:
        int bankID;
        string bankName;
        std::vector<branch> branches; 
        std::vector<Customer> customers;
        std::vector<employee> employees;

        void addBranch(const branch& newBranch) {
            branches.push_back(newBranch);
        }

        void addCustomer(const Customer& newCustomer) {
            customers.push_back(newCustomer);
        }

        void addEmployee(const employee& newEmployee) {
            employees.push_back(newEmployee);
        }
};

class branch {
    public:
        int branchId;
        string branchName;
        string address;
        string IFSCCode;
        std::vector<account> accounts;
        std::vector<employee> employees;

        void addAccount(const account& newAccount) {
            accounts.push_back(newAccount);
        }

        void addEmployee(const employee& newEmployee) {
            employees.push_back(newEmployee);
        }
};

class Customer {
    public:
        int customerId;
        string fullName;
        Date dob;
        string gender;
        string mobileNumber;
        string email;
        string address;
        string aadharNumber;
        string PANNumber;
        std::vector<account> accounts;
        std::vector<loan> loans;

        void addAccount(const account& newAccount) {
            accounts.push_back(newAccount);
        }

        void addLoan(const loan& newLoan) {
            loans.push_back(newLoan);
        }
};

class account {
    public:
        long accountNumber;
        string accountType;
        double balance;
        Date dateOpened;
        string status;
        branch* Branch;
        Customer* customer;
        std::vector<Transaction> transactions;

        void addTransaction(const Transaction& newTransaction, string transactionType, string transactionStatus) {
            transactions.push_back(newTransaction);
            if (transactionStatus == "Success") {
                if (transactionType == "Deposit") {
                    balance += newTransaction.amount;
                }
                else if (transactionType == "Withdrawal") {
                    balance -= newTransaction.amount;
                }
            }
        }
};

class SavingsAccount : public account {
    public:
        double interestRate;
        double minimumBalance;
};

class CurrentAccount : public account {
    public:
        double overdraftLimit;
        string businessName;
};

class FixedDepositAccount : public account {
    public:
        double FDAmount;
        double FDInterestRate;
        Date maturityDate;
        int tenureMonths;
};

class Transaction {
    public:
        int transactionId;
        string transactionType;
        double amount;
        Date transactionDate;
        account* senderAccount;
        account* receiverAccount;
        string status;

        void processTransaction() {
            if (transactionType == "Deposit") {
                senderAccount->balance += amount;
            }
            else if (transactionType == "Withdrawal") {
                senderAccount->balance -= amount;
            }
            else if (transactionType == "Transfer") {
                senderAccount->balance -= amount;
                receiverAccount->balance += amount;
            }
        }
};

class loan {
    public:
        int loanId;
        string loanType;
        double loanAmount;
        double interestRate;
        int tenureYears;
        double EMIAmount;
        string loanStatus;
        Customer* customer;
};

class ATMCard {
    private:
        long cardNumber;
        string cardType;
        Date expiryDate;
        int CVV;
        int PIN;
        string cardStatus;
        account* linkedAccount;
};

class employee {
    public:
        string employeeName;
        int employeeId;
        string designation;
        double salary;
        branch* Branch;
};

class Notification {
    public:
        string message;
};

class SMSNotification : public Notification {
    public:
        string phoneNumber;
        string deliveryStatus;
};

class EmailNotification : public Notification {
    public:
        string emailAddress;
        string subject;
        string deliveryStatus;
};
