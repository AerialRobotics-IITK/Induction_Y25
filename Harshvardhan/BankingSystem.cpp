#include <iostream>
#include <vector>
#include <string>   
#include <stdexcept>
#include <cmath>

using namespace std;

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
        return "Account is Blocked!";
    }
};
class LoanRejectedException : public exception {
public:
    const char* what() const noexcept override {
        return "Loan Rejected!";
    }
};

class Branch;
class Customer;

class Transaction{
    private:
        int transactionId;
        string transactionType;
        double amount;
        string transactionDate;
        string status;
        Account* senderAcc;
        Account* receiverAcc;
    public:
        Transaction(int id, string transType, double amt, string transDate, string stat, Account* sender, Account* receiver):
            transactionId(id), transactionType(transType), amount(amt), transactionDate(transDate), status(stat), senderAcc(sender), receiverAcc(receiver) {}

        void displayTransaction() {
            cout << "Transaction ID: " << transactionId << endl;
            cout << "Transaction Type: " << transactionType << endl;
            cout << "Amount: " << amount << endl;
            cout << "Transaction Date: " << transactionDate << endl;
            cout << "Status: " << status << endl;
        }
}; 
class Account{
    protected:
        long accountNumber;
        string accountType;
        double balance;
        string dateOpened;
        string status;
        Branch* branch;
        Customer* customer;
        vector<Transaction> transactions;   
    public:
        Account(long accNo, string accType, double bal, string date, string stat, Branch* branch, Customer* cust):
            accountNumber(accNo), accountType(accType), balance(bal), dateOpened(date), status(stat), branch(branch), customer(cust) {};

        virtual void deposit(double amount) {
            if (status == "Blocked") {
                throw runtime_error("Account is blocked. Cannot perform transactions.");
            }
            balance += amount;
            cout << amount << " deposited successfully. New Balance: " << balance << endl;
        }

        virtual void withdraw(double amount) = 0;

        double getBalance() {
            return balance;
        }

        long getAccountNumber() {
            return accountNumber;
        }
        void blockAccount() {
            status = "Blocked";
        }

        void activateAccount() {
            status = "Active";
        }

        string getStatus() const {
            return status;
        }
      
        void transfer(int id,Account* receiver, double amount) {
            if(status == "Blocked")
                throw AccountBlockedException();

            withdraw(amount);
            receiver->deposit(amount);
            transactions.push_back(Transaction(id,"Transfer",amount,"19-05-2026","Success",this,receiver));
            cout << "Transfer successful.\n";
        }

        void showAccount() {
            cout << "Account Number: " << accountNumber << endl;
            cout << "Account Type: " << accountType << endl;
            cout << "Balance: " << balance << endl;
            cout << "Date Opened: " << dateOpened << endl;
            cout << "Status: " << status << endl;
        };

        virtual ~Account() {};
};
class SavingsAccount : public Account {
    private:
        double interestRate;
        double minimumBalance;
    public:
        SavingsAccount(long accNo, double bal,string date,Branch* branch, Customer* customer, double interest, double minBal):
            Account(accNo, "Savings", bal, date , "Active", branch, customer), interestRate(interest), minimumBalance(minBal) {}
        void withdraw(double amount) override {
            if (status == "Blocked")
                throw AccountBlockedException();

            if (balance - amount < minimumBalance)
                throw InsufficientBalanceException();

            balance -= amount;
            cout << amount<< " withdrawn from Savings Account. New Balance: " << balance << endl;
        }

};
class CurrentAccount : public Account {
    private:
        double overdraftLimit;
        string businessName;
    public:
        CurrentAccount(long accNo, double bal, string date, Branch* branch, Customer* customer, double overdraft, string business):
            Account(accNo, "Current", bal, date, "Active", branch, customer), overdraftLimit(overdraft), businessName(business) {}
        void withdraw(double amount) override {
            if (status == "Blocked")
                throw AccountBlockedException();

            if (balance + overdraftLimit < amount)
                throw InsufficientBalanceException();

            balance -= amount;
            cout << amount<< " withdrawn from Current Account. New Balance: " << balance << endl;
        }
};
class FixedDepositAccount : public Account {
    private:
        double FDAmount;
        string maturityDate;
        double FDInterestRate;
        int tenureMonths;
    public:
        FixedDepositAccount(long accNo, double amount, string date,Branch* branch, Customer* customer, double interest, int tenure):
            Account(accNo, "Fixed Deposit", amount, date, "Active",branch, customer), FDAmount(amount), maturityDate("01-01-2028"), FDInterestRate(interest), tenureMonths(tenure) {}
        
        
        double calculateMaturityAmount() {
            return FDAmount +(FDAmount *FDInterestRate *tenureMonths / 1200);
        }

        void withdraw(double amount) override {
            throw runtime_error("Withdrawal not allowed before maturity.");
        }

};


class Loan{
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
        Loan(int id, string loanType, double amount, double interest, int tenureYrs, Customer* cust):
            loanId(id), loanType(loanType), loanAmount(amount), interestRate(interest), tenureYears(tenureYrs), EMIAmount(0.0), loanStatus("Approved"), customer(cust) {
                calculateEMI();
            }
        void calculateEMI() {
            double monthlyRate = interestRate / (12 * 100);
            int months = tenureYears * 12;
            EMIAmount = (loanAmount * monthlyRate * pow(1 + monthlyRate, months)) / (pow(1 + monthlyRate, months) - 1);
        }
        void displayLoan() {
            cout << "Loan ID: " << loanId << endl;
            cout << "Loan Type: " << loanType << endl;
            cout << "Loan Amount: " << loanAmount << endl;
            cout << "Interest Rate: " << interestRate << "%" << endl;
            cout << "Tenure: " << tenureYears << " years" << endl;
            cout << "EMI Amount: " << EMIAmount << endl;
            cout << "Loan Status: " << loanStatus << endl;
        }
};
class ATMCard{
    private:
        long cardNumber;
        int CVV;
        string expiryDate;
        int PIN;
        string cardType;
        string cardStatus;
        Account* linkedAccount;
    public:
        ATMCard(long cardNo, int cvv, string expiry, int pin, string type, Account* acc):
            cardNumber(cardNo), CVV(cvv), expiryDate(expiry), PIN(pin), cardType(type), cardStatus("Active"), linkedAccount(acc) {}

        void verifyPIN(int enteredPIN) {
            if(cardStatus == "Blocked")
                throw runtime_error("Card Blocked.");

            if(cardStatus == "Expired")
                throw runtime_error("Card Expired.");

            if(enteredPIN != PIN)
                throw InvalidPINException();

            cout << "PIN Verified Successfully."<< endl;
        }
        void expireCard() {
            cardStatus = "Expired";
            cout << "Card has expired." << endl;
        } 
        void blockCard() {
            cardStatus = "Blocked";
            cout << "Card has been blocked." << endl;
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
        Employee(int id, string name, string desig, double sal, Branch* branch):
            employeeId(id), employeeName(name), designation(desig), salary(sal), branch(branch) {}

        void displayEmployee() {
            cout << "Employee ID: " << employeeId << endl;
            cout << "Employee Name: " << employeeName << endl;
            cout << "Designation: " << designation << endl;
            cout << "Salary: $" << salary << endl;
        }
};

class Notification{
    public:
        string message;
        Notification(string msg): message(msg) {}
        virtual void sendNotification() = 0;   
        virtual ~Notification() {}
        
};
class SMSNotification : public Notification {
    private:
        string phoneNumber;
        string deliveryStatus;
    public:
        SMSNotification(string phone, string msg,string status): Notification(msg), phoneNumber(phone), deliveryStatus(status) {}
        void sendNotification() override{
            deliveryStatus = "Sent";
            cout << "[SMS] To: " << phoneNumber << endl;
            cout << "Message: " << message << endl;
            cout << "Status: " << deliveryStatus << endl;
        }
};
class EmailNotification : public Notification {
    private:
        string emailAddress;
        string subject;
        string deliveryStatus;
    public:
        EmailNotification(string email, string sub, string msg, string status): Notification(msg), emailAddress(email), subject(sub), deliveryStatus(status) {}
        void sendNotification() override {
            deliveryStatus = "Sent";
            cout << "[EMAIL] To: " << emailAddress << endl;
            cout << "Subject: " << subject << endl;
            cout << "Message: " << message << endl;
            cout << "Status: " << deliveryStatus << endl;
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
        string aadharNumber;
        string PANnumber;
        vector<Account*> accounts;
        vector<Loan*> loans;
    public:
        Customer(int id, string name, string dateOfBirth, string gen, string mobile, string mail, string addr, string aadhar, string pan):
            customerId(id), fullName(name), dob(dateOfBirth), gender(gen), mobileNumber(mobile), email(mail), address(addr), aadharNumber(aadhar), PANnumber(pan) {} 
        
        void addAccount(Account* account) {
            accounts.push_back(account);
        }
        void takeLoan(Loan* loan) {
            loans.push_back(loan);
        }
        void displayCustomer() {
            cout << "Customer ID: " << customerId << endl;
            cout << "Customer Name: " <<fullName << endl;
            cout << "Email: " << email << endl;
            cout << "Phone Number: " << mobileNumber << endl;
            cout << "Date of Birth: " << dob << endl;
            cout << "Gender: " << gender << endl;
            cout << "Address: " << address << endl;
            cout << "Aadhar Number: " << aadharNumber << endl;
            cout << "PAN Number: " << PANnumber << endl;
        }
};

class Branch{
    private:
        int branchId;
        string branchName;
        string IFSCCode;
        string address;

        vector<Account*> accounts;
        vector<Employee*> employees;
    public:
        Branch(int id, string name, string IFSC,string address): branchId(id), branchName(name), IFSCCode(IFSC), address(address) {} 
        void addAccount(Account* account) {
            accounts.push_back(account);
        }
        void addEmployee(Employee* employee) {
            employees.push_back(employee);
        }
        void displayBranch() { 
                cout << "Branch ID: " << branchId << endl;
                cout << "Branch Name: " << branchName << endl; 
                cout << "IFSC Code: " << IFSCCode << endl;
                cout << "Address: " << address << endl;
        }
};

class Bank{
    private:
        int bankId;
        string bankName;
        vector<Branch*> branches;
        vector<Customer*> customers;
        vector<Employee*> employees;
    public:
        Bank(int id, string name): bankId(id), bankName(name) {}

        void addBranch(Branch* branch) {
            branches.push_back(branch);
        }
        void addCustomer(Customer* customer) {
            customers.push_back(customer);
        }
        void addEmployee(Employee* employee) {
            employees.push_back(employee);
        }
        void displayBank() {
            cout << "Bank ID: " << bankId << endl;
            cout << "Bank Name: " << bankName << endl;
        }
};

class AccountFactory{
    public:
        static Account* createSavingsAccount(long accNo, double bal,string date,Branch* branch, Customer* customer, double interest, double minBal) {
            return new SavingsAccount(accNo,bal,date,branch,customer, interest, minBal);   
        }

        static Account* createCurrentAccount(long accNo, double bal, string date,Branch* branch, Customer* customer, double overdraft, string business) {
            return new CurrentAccount(accNo,bal,date,branch,customer,overdraft,business);
        }

        static Account* createFDAccount(long accNo, double amount, string date,Branch *branch, Customer* customer, double interest, int tenure) {
            return new FixedDepositAccount(accNo, amount, date,branch, customer, interest, tenure);
        }
};


int main(){
    return 0;
}