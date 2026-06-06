#include <iostream>
#include <string>
#include <list>
#include <ctime>
#include <cmath>

using namespace std;

class Branch;
class Customer;
class Employee;
class Transaction;
class Account;
class Loan;

// ========================= CUSTOMER CLASS =========================

class Customer
{
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
    list<Account*> accounts;
    list<Loan*> loans;

public:
    Customer()
    {
    }

    Customer(int id,
             string name,
             string d,
             string g,
             string mobile,
             string mail,
             string addr,
             string aadhaar,
             string pan)
    {
        customerId = id;
        fullName = name;
        dob = d;
        gender = g;
        mobileNumber = mobile;
        email = mail;
        address = addr;
        aadhaarNumber = aadhaar;
        PANNumber = pan;
    }

    int getCustomerId()
    {
        return customerId;
    }

    string getCustomerName()
    {
        return fullName;
    }

    string getMobileNumber()
    {
        return mobileNumber;
    }

    string getEmail()
    {
        return email;
    }

// ================= ACCOUNT FUNCTIONS =================

void addAccount(Account* a)
{
    accounts.push_back(a);
}

list<Account*> getAccounts()
{
    return accounts;
}

// ================= LOAN FUNCTIONS =================

void addLoan(Loan* l)
{
    loans.push_back(l);
}

list<Loan*> getLoans()
{
    return loans;
}


};





// ========================= EMPLOYEE CLASS =========================

class Employee
{
private:
    int employeeId;
    string employeeName;
    string designation;
    double salary;

    Branch* branch;

public:
    Employee()
    {
        branch = NULL;
    }

    Employee(int id,
             string name,
             string desig,
             double sal)
    {
        employeeId = id;
        employeeName = name;
        designation = desig;
        salary = sal;

        branch = NULL;
    }

    void assignBranch(Branch* b)
    {
        branch = b;
    }

    int getEmployeeId()
    {
        return employeeId;
    }

    string getEmployeeName()
    {
        return employeeName;
    }

    string getDesignation()
    {
        return designation;
    }

    double getSalary()
    {
        return salary;
    }
};




// ========================= BRANCH CLASS =========================

class Branch
{
private:
    int branchId;
    string branchName;
    string IFSCCode;
    string address;

    list<Customer*> customers;
    list<Employee*> employees;
    list<Account*> accounts;


public:
    Branch()
    {
    }

    Branch(int id,
           string name,
           string ifsc,
           string addr)
    {
        branchId = id;
        branchName = name;
        IFSCCode = ifsc;
        address = addr;
    }

    void addCustomer(Customer* c)
    {
        customers.push_back(c);
    }

    void addEmployee(Employee* e)
    {
        employees.push_back(e);

        e->assignBranch(this);
    }

    int getBranchId()
    {
        return branchId;
    }

    string getBranchName()
    {
        return branchName;
    }

    string getIFSCCode()
    {
        return IFSCCode;
    }

    string getAddress()
    {
        return address;
    }

    list<Customer*> getCustomers()
    {
        return customers;
    }

    list<Employee*> getEmployees()
    {
        return employees;
    }

// ================= ACCOUNT FUNCTIONS =================

void addAccount(Account* a)
{
    accounts.push_back(a);
}

list<Account*> getAccounts()
{
    return accounts;
}

};






// ========================= BANK CLASS =========================

class Bank
{
private:
    int bankId;
    string bankName;

    list<Branch*> branches;
    list<Customer*> customers;
    list<Employee*> employees;

public:
    Bank()
    {
    }

    Bank(int id, string name)
    {
        bankId = id;
        bankName = name;
    }

    void addBranch(Branch* b)
    {
        branches.push_back(b);
    }

    void addCustomer(Customer* c)
    {
        customers.push_back(c);
    }

    void addEmployee(Employee* e)
    {
        employees.push_back(e);
    }

    int getBankId()
    {
        return bankId;
    }

    string getBankName()
    {
        return bankName;
    }

    list<Branch*> getBranches()
    {
        return branches;
    }

    list<Customer*> getCustomers()
    {
        return customers;
    }

    list<Employee*> getEmployees()
    {
        return employees;
    }
};




//////////////////////////////////////////////////////////////////////////////


// ========================= ABSTRACT ACCOUNT CLASS =========================

class Account
{
protected:
    long accountNumber;
    string accountType;
    double balance;
    string dateOpened;
    string status;

    Branch* branch;
    Customer* customer;

    list<Transaction*> transactions;

public:

    // Default Constructor
    Account()
    {
        accountNumber = 0;
        accountType = "";
        balance = 0;

        dateOpened = "";
        status = "Active";

        branch = NULL;
        customer = NULL;
    }

    // Parameterized Constructor
    Account(long accNo,
            string accType,
            double bal,
            string openDate,
            string stat,
            Branch* b,
            Customer* c)
    {
        accountNumber = accNo;
        accountType = accType;
        balance = bal;

        dateOpened = openDate;
        status = stat;

        branch = b;
        customer = c;
    }

    // Virtual Destructor
    virtual ~Account()
    {
    }

    // ===================================================

    long getAccountNumber()
    {
        return accountNumber;
    }

    string getAccountType()
    {
        return accountType;
    }

    double getBalance()
    {
        return balance;
    }

    string getDateOpened()
    {
        return dateOpened;
    }

    string getStatus()
    {
        return status;
    }

    Branch* getBranch()
    {
        return branch;
    }

    Customer* getCustomer()
    {
        return customer;
    }

    list<Transaction*> getTransactions()
    {
        return transactions;
    }

    // ===================================================

    void setStatus(string stat)
    {
        status = stat;
    }

    void setBranch(Branch* b)
    {
        branch = b;
    }

    void setCustomer(Customer* c)
    {
        customer = c;
    }

    // ========================= TRANSACTION METHODS =========================

    virtual void deposit(double amount)
    {
        balance = balance + amount;
    }

    virtual void withdraw(double amount)
    {
        if(amount <= balance)
        {
            balance = balance - amount;
        }
        else
        {
            cout << "Insufficient Balance" << endl;
        }
    }

    void addTransaction(Transaction* t)
    {
        transactions.push_back(t);
    }

    // ========================= PURE VIRTUAL FUNCTION =========================

    virtual void calculateInterest() = 0;
};






// ========================= TRANSACTION CLASS =========================

class Transaction
{
private:
    int transactionId;
    string transactionType;

    double amount;
    string transactionDate;

    Account* senderAccount;
    Account* receiverAccount;

    string status;

public:

    // Default Constructor
    Transaction()
    {
        transactionId = 0;
        transactionType = "";

        amount = 0;
        transactionDate = "";

        senderAccount = NULL;
        receiverAccount = NULL;

        status = "Pending";
    }

    // Parameterized Constructor
    Transaction(int id,
                string type,
                double amt,
                string date,
                Account* sender,
                Account* receiver,
                string stat)
    {
        transactionId = id;
        transactionType = type;

        amount = amt;
        transactionDate = date;

        senderAccount = sender;
        receiverAccount = receiver;

        status = stat;
    }

    // ===================================================

    int getTransactionId()
    {
        return transactionId;
    }

    string getTransactionType()
    {
        return transactionType;
    }

    double getAmount()
    {
        return amount;
    }

    string getTransactionDate()
    {
        return transactionDate;
    }

    Account* getSenderAccount()
    {
        return senderAccount;
    }

    Account* getReceiverAccount()
    {
        return receiverAccount;
    }

    string getStatus()
    {
        return status;
    }

    // ===================================================

    void setStatus(string stat)
    {
        status = stat;
    }

    // ========================= TRANSACTION OPERATIONS =========================

    void processTransaction()
    {
        if(status == "Blocked")
        {
            cout << "Transaction Blocked" << endl;
            return;
        }

        // Deposit Transaction
        if(transactionType == "Deposit")
        {
            if(receiverAccount != NULL)
            {
                receiverAccount->deposit(amount);

                receiverAccount->addTransaction(this);

                status = "Success";
            }
        }

        // Withdraw Transaction
        else if(transactionType == "Withdraw")
        {
            if(senderAccount != NULL)
            {
                senderAccount->withdraw(amount);

                senderAccount->addTransaction(this);

                status = "Success";
            }
        }

        // Transfer Transaction
        else if(transactionType == "Transfer")
        {
            if(senderAccount != NULL && receiverAccount != NULL)
            {
                if(senderAccount->getBalance() >= amount)
                {
                    senderAccount->withdraw(amount);

                    receiverAccount->deposit(amount);

                    senderAccount->addTransaction(this);

                    receiverAccount->addTransaction(this);

                    status = "Success";
                }
                else
                {
                    status = "Failed";

                    cout << "Insufficient Balance for Transfer" << endl;
                }
            }
        }

        else
        {
            status = "Failed";

            cout << "Invalid Transaction Type" << endl;
        }
    }
};




////////////////////////////////////////////////////////////////////////////

// ========================= SAVINGS ACCOUNT CLASS =========================

class SavingsAccount : public Account
{
private:
    double interestRate;
    double minimumBalance;

public:

    // Default Constructor
    SavingsAccount() : Account()
    {
        interestRate = 0;
        minimumBalance = 0;
    }

    // Parameterized Constructor
    SavingsAccount(long accNo,
                   double bal,
                   string openDate,
                   string stat,
                   Branch* b,
                   Customer* c,
                   double rate,
                   double minBal)

        : Account(accNo,
                  "Savings",
                  bal,
                  openDate,
                  stat,
                  b,
                  c)
    {
        interestRate = rate;
        minimumBalance = minBal;
    }

    // ====================================================

    double getInterestRate()
    {
        return interestRate;
    }

    double getMinimumBalance()
    {
        return minimumBalance;
    }

    // ========================= ACCOUNT FUNCTIONS =========================

    void calculateInterest()
    {
        double interest = (balance * interestRate) / 100;

        balance = balance + interest;
    }

    void withdraw(double amount)
    {
        if(balance - amount >= minimumBalance)
        {
            balance = balance - amount;
        }
        else
        {
            cout << "Minimum Balance Requirement Violated" << endl;
        }
    }
};



// ========================= CURRENT ACCOUNT CLASS =========================

class CurrentAccount : public Account
{
private:
    double overdraftLimit;
    string businessName;

public:

    // Default Constructor
    CurrentAccount() : Account()
    {
        overdraftLimit = 0;
        businessName = "";
    }

    // Parameterized Constructor
    CurrentAccount(long accNo,
                   double bal,
                   string openDate,
                   string stat,
                   Branch* b,
                   Customer* c,
                   double limit,
                   string business)

        : Account(accNo,
                  "Current",
                  bal,
                  openDate,
                  stat,
                  b,
                  c)
    {
        overdraftLimit = limit;
        businessName = business;
    }

    // =================================================

    double getOverdraftLimit()
    {
        return overdraftLimit;
    }

    string getBusinessName()
    {
        return businessName;
    }

    // ========================= ACCOUNT FUNCTIONS =========================

    void calculateInterest()
    {
        // Usually Current Accounts do not provide interest

        cout << "No Interest for Current Account" << endl;
    }

    void withdraw(double amount)
    {
        if(balance + overdraftLimit >= amount)
        {
            balance = balance - amount;
        }
        else
        {
            cout << "Overdraft Limit Exceeded" << endl;
        }
    }
};



// ========================= FIXED DEPOSIT ACCOUNT CLASS =========================

class FixedDepositAccount : public Account
{
private:
    double FDAmount;

    string maturityDate;

    double FDInterestRate;

    int tenureMonths;

public:

    // Default Constructor
    FixedDepositAccount() : Account()
    {
        FDAmount = 0;

        maturityDate = "";

        FDInterestRate = 0;

        tenureMonths = 0;
    }

    // Parameterized Constructor
    FixedDepositAccount(long accNo,
                        double bal,
                        string openDate,
                        string stat,
                        Branch* b,
                        Customer* c,
                        double fdAmt,
                        string maturity,
                        double fdRate,
                        int tenure)

        : Account(accNo,
                  "Fixed Deposit",
                  bal,
                  openDate,
                  stat,
                  b,
                  c)
    {
        FDAmount = fdAmt;

        maturityDate = maturity;

        FDInterestRate = fdRate;

        tenureMonths = tenure;
    }

    // =================================================

    double getFDAmount()
    {
        return FDAmount;
    }

    string getMaturityDate()
    {
        return maturityDate;
    }

    double getFDInterestRate()
    {
        return FDInterestRate;
    }

    int getTenureMonths()
    {
        return tenureMonths;
    }

    // ========================= ACCOUNT FUNCTIONS =========================

    void calculateInterest()
    {
        double interest;

        interest = (FDAmount * FDInterestRate * tenureMonths) / (12 * 100);

        balance = FDAmount + interest;
    }

    void withdraw(double amount)
    {
        cout << "Withdrawal Not Allowed Before Maturity" << endl;
    }
};





// ========================= LOAN CLASS =========================

class Loan
{
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
    Loan()
    {
        loanId = 0;
        loanType = "";

        loanAmount = 0.0;
        interestRate = 0.0;

        tenureYears = 0;

        EMIAmount = 0.0;

        loanStatus = "Pending";

        customer = NULL;
    }

    Loan(int id,
         string type,
         double amount,
         double rate,
         int tenure,
         string status,
         Customer* c)
    {
        loanId = id;
        loanType = type;

        loanAmount = amount;
        interestRate = rate;

        tenureYears = tenure;

        loanStatus = status;

        customer = c;

        EMIAmount = calculateEMI();
    }

    // ================= EMI CALCULATION =================

    double calculateEMI()
    {
        double monthlyRate = interestRate / (12 * 100);

        int months = tenureYears * 12;

        double emi;

        emi = (loanAmount * monthlyRate);

        emi = emi * pow((1 + monthlyRate), months);

        emi = emi / (pow((1 + monthlyRate), months) - 1);

        return emi;
    }

    // ================= GET=================

    int getLoanId()
    {
        return loanId;
    }

    string getLoanType()
    {
        return loanType;
    }

    double getLoanAmount()
    {
        return loanAmount;
    }

    double getInterestRate()
    {
        return interestRate;
    }

    int getTenureYears()
    {
        return tenureYears;
    }

    double getEMIAmount()
    {
        return EMIAmount;
    }

    string getLoanStatus()
    {
        return loanStatus;
    }

    Customer* getCustomer()
    {
        return customer;
    }

    // ================= SET=================

    void setLoanStatus(string status)
    {
        loanStatus = status;
    }
};








// ========================= ATM CARD CLASS =========================

class ATMCard
{
private:
    long cardNumber;
    int CVV;

    string expiryDate;

    int PIN;

    string cardType;
    string cardStatus;

    Account* linkedAccount;

public:
    ATMCard()
    {
        cardNumber = 0;
        CVV = 0;

        expiryDate = "";

        PIN = 0;

        cardType = "";
        cardStatus = "Active";

        linkedAccount = NULL;
    }

    ATMCard(long cardNo,
            int cvv,
            string expiry,
            int pin,
            string type,
            string status,
            Account* acc)
    {
        cardNumber = cardNo;
        CVV = cvv;

        expiryDate = expiry;

        PIN = pin;

        cardType = type;
        cardStatus = status;

        linkedAccount = acc;
    }

    // ================= PIN FUNCTIONS =================

    bool validatePIN(int enteredPIN)
    {
        if(cardStatus == "Blocked")
        {
            cout << "Card is Blocked" << endl;
            return false;
        }

        if(PIN == enteredPIN)
        {
            return true;
        }

        return false;
    }

    void changePIN(int oldPIN, int newPIN)
    {
        if(validatePIN(oldPIN))
        {
            PIN = newPIN;

            cout << "PIN Changed Successfully" << endl;
        }
        else
        {
            cout << "Invalid Old PIN" << endl;
        }
    }

    // ================= CARD STATUS =================

    void blockCard()
    {
        cardStatus = "Blocked";
    }

    void activateCard()
    {
        cardStatus = "Active";
    }

    // ================= GET =================

    long getCardNumber()
    {
        return cardNumber;
    }

    int getCVV()
    {
        return CVV;
    }

    string getExpiryDate()
    {
        return expiryDate;
    }

    string getCardType()
    {
        return cardType;
    }

    string getCardStatus()
    {
        return cardStatus;
    }

    Account* getLinkedAccount()
    {
        return linkedAccount;
    }
};