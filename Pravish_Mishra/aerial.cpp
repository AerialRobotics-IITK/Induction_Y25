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
    Date(int day, int month, int year) : day(day), month(month), year(year) {}

    int getDay()   const { return day;   }
    int getMonth() const { return month; }
    int getYear()  const { return year;  }

    void setDay(int d)   { day   = d; }
    void setMonth(int m) { month = m; }
    void setYear(int y)  { year  = y; }

    void display() const {
        cout << day << "/" << month << "/" << year;
    }
};

class Branch;
class Customer;
class Account;
class Loan;
class Transaction;

class Employee {
private:
    int     employeeId;
    string  employeeName;
    string  designation;
    double  salary;
    Branch* branch;
public:
    Employee() : employeeId(0), salary(0.0), branch(nullptr) {}

    int     getEmployeeId()   const { return employeeId;   }
    string  getEmployeeName() const { return employeeName; }
    string  getDesignation()  const { return designation;  }
    double  getSalary()       const { return salary;       }
    Branch* getBranch()       const { return branch;       }

    void setEmployeeId(int id)            { employeeId   = id; }
    void setEmployeeName(const string& n) { employeeName = n;  }
    void setDesignation(const string& d)  { designation  = d;  }
    void setSalary(double s)              { salary       = s;  }
    void setBranch(Branch* b)             { branch       = b;  }
};

class Branch {
private:
    int    branchId;
    string branchName;
    string IFSCCode;
    string address;
    vector<Account*>  accounts;
    vector<Employee*> employees;
public:
    Branch() : branchId(0) {}

    int    getBranchId()   const { return branchId;   }
    string getBranchName() const { return branchName; }
    string getIFSCCode()   const { return IFSCCode;   }
    string getAddress()    const { return address;    }
    const vector<Account*>&  getAccounts()  const { return accounts;  }
    const vector<Employee*>& getEmployees() const { return employees; }

    void setBranchId(int id)            { branchId   = id; }
    void setBranchName(const string& n) { branchName = n;  }
    void setIFSCCode(const string& i)   { IFSCCode   = i;  }
    void setAddress(const string& a)    { address    = a;  }

    void addAccount(Account* acc)   { accounts.push_back(acc);  }
    void addEmployee(Employee* emp) { employees.push_back(emp); }
};

class Transaction {
private:
    int      transactionId;
    string   transactionType;
    double   amount;
    Date     transactionDate;
    Account* senderAccount;
    Account* receiverAccount;
    string   status;
public:
    Transaction() : transactionId(0), amount(0.0),
                    senderAccount(nullptr), receiverAccount(nullptr),
                    status("Pending") {}

    int      getTransactionId()   const { return transactionId;   }
    string   getTransactionType() const { return transactionType; }
    double   getAmount()          const { return amount;          }
    Date     getTransactionDate() const { return transactionDate; }
    Account* getSenderAccount()   const { return senderAccount;   }
    Account* getReceiverAccount() const { return receiverAccount; }
    string   getStatus()          const { return status;          }

    void setStatus(const string& s) { status = s; }

    void execute() {
        status = "Success";
        cout << transactionId << transactionType<< amount << "Status: " << status << endl;
    }

    void displayInfo() const {
        cout << transactionId << transactionType << "Amount: " << amount << "Status: " << status << endl;
    }
};

class Account {
protected:
    long      accountNumber;
    string    accountType;
    double    balance;
    Date      dateOpened;
    string    status;
    Branch*   branch;
    Customer* customer;
    vector<Transaction*> transactions;
public:
    Account() : accountNumber(0), accountType(" "), balance(0.0),
                status(" "), branch(nullptr), customer(nullptr) {}

    virtual void displayInfo() = 0;

    long      getAccountNumber() const { return accountNumber; }
    string    getAccountType()   const { return accountType;   }
    Date      getDateOpened()    const { return dateOpened;    }
    string    getStatus()        const { return status;        }
    Branch*   getBranch()        const { return branch;        }
    Customer* getCustomer()      const { return customer;      }
    double    getBalance()       const { return balance;       }

    void addTransaction(Transaction* txn) {
        transactions.push_back(txn);
    }

    void getTransactionHistory() const {
        cout << "Transaction History" << accountNumber<< endl;
        for (auto* t : transactions) {
            t->displayInfo();
        }
    }

    void setDate(const Date& d)     { dateOpened = d; }
    void setStatus(const string& s) { status     = s; }
    void setBranch(Branch* b)       { branch     = b; }
    void setCustomer(Customer* c)   { customer   = c; }

    virtual ~Account() {}
};

class SavingsAccount : public Account {
private:
    double interestRate;
    double minimumBalance;
public:
    SavingsAccount() : interestRate(0.0), minimumBalance(0.0) {}

    void displayInfo() override {
        cout << "Savings Account: " << accountNumber << "Balance: " << balance << endl;
    }

    double getInterestRate()   const { return interestRate;   }
    double getMinimumBalance() const { return minimumBalance; }

    void setInterestRate(double r)   { interestRate   = r; }
    void setMinimumBalance(double m) { minimumBalance = m; }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;
    string businessName;
public:
    CurrentAccount() : overdraftLimit(0.0), businessName(" ") {}

    void displayInfo() override {
        cout << "Current Account: " << accountNumber << " | Balance: " << balance << endl;
    }

    double getOverdraftLimit() const { return overdraftLimit; }
    string getBusinessName()   const { return businessName;   }

    void setOverdraftLimit(double o)      { overdraftLimit = o; }
    void setBusinessName(const string& b) { businessName   = b; }
};

class FixedDepositAccount : public Account {
private:
    double FDAmount;
    Date   maturityDate;
    double FDInterestRate;
    int    tenureMonths;
public:
    FixedDepositAccount() : FDAmount(0.0), FDInterestRate(0.0), tenureMonths(0) {}

    void displayInfo() override {
        cout << "Fixed Deposit Account: " << accountNumber << " | Balance: " << balance << endl;
    }

    double getFDAmount()       const { return FDAmount;       }
    Date   getMaturityDate()   const { return maturityDate;   }
    double getFDInterestRate() const { return FDInterestRate; }
    int    getTenureMonths()   const { return tenureMonths;   }

    void setFDAmount(double amt)        { FDAmount       = amt; }
    void setMaturityDate(const Date& d) { maturityDate   = d;   }
    void setFDInterestRate(double r)    { FDInterestRate = r;   }
    void setTenureMonths(int t)         { tenureMonths   = t;   }
};

class Loan {
private:
    int       loanId;
    string    loanType;
    double    loanAmount;
    double    interestRate;
    int       tenureYears;
    double    EMIAmount;
    string    loanStatus;
    Customer* customer;
public:
    Loan() : loanId(0), loanType(" "), loanAmount(0.0), interestRate(0.0),
             tenureYears(0), EMIAmount(0.0), loanStatus(" "), customer(nullptr) {}

    int       getLoanId()       const { return loanId;       }
    string    getLoanType()     const { return loanType;     }
    double    getLoanAmount()   const { return loanAmount;   }
    double    getInterestRate() const { return interestRate; }
    int       getTenureYears()  const { return tenureYears;  }
    double    getEMIAmount()    const { return EMIAmount;    }
    string    getLoanStatus()   const { return loanStatus;   }
    Customer* getCustomer()     const { return customer;     }

    void setLoanId(int id)             { loanId       = id; }
    void setLoanType(const string& t)  { loanType     = t;  }
    void setLoanAmount(double a)       { loanAmount   = a;  }
    void setInterestRate(double r)     { interestRate = r;  }
    void setTenureYears(int y)         { tenureYears  = y;  }
    void setEMIAmount(double e)        { EMIAmount    = e;  }
    void setLoanStatus(const string& s){ loanStatus   = s;  }
    void setCustomer(Customer* c)      { customer     = c;  }
};

class Customer {
private:
    int    customerId;
    string fullName;
    Date   dob;
    string gender;
    string mobileNumber;
    string email;
    string address;
    string aadhaarNumber;
    string PANNumber;
    vector<Account*> accounts;
    vector<Loan*>    loans;
public:
    Customer() : customerId(0) {}

    Customer(int id, const string& name, const Date& dob,
             const string& gender, const string& mobile,
             const string& email, const string& addr,
             const string& aadhaar, const string& pan)
        : customerId(id), fullName(name), dob(dob),
          gender(gender), mobileNumber(mobile), email(email),
          address(addr), aadhaarNumber(aadhaar), PANNumber(pan) {}

    int    getCustomerId()    const { return customerId;    }
    string getFullName()      const { return fullName;      }
    Date   getDOB()           const { return dob;           }
    string getGender()        const { return gender;        }
    string getMobileNumber()  const { return mobileNumber;  }
    string getEmail()         const { return email;         }
    string getAddress()       const { return address;       }
    string getAadhaarNumber() const { return aadhaarNumber; }
    string getPANNumber()     const { return PANNumber;     }

    const vector<Account*>& getAccounts() const { return accounts; }
    const vector<Loan*>&    getLoans()    const { return loans;    }

    void setCustomerId(int id)               { customerId    = id; }
    void setFullName(const string& n)        { fullName      = n;  }
    void setDOB(const Date& d)               { dob           = d;  }
    void setGender(const string& g)          { gender        = g;  }
    void setMobileNumber(const string& m)    { mobileNumber  = m;  }
    void setEmail(const string& e)           { email         = e;  }
    void setAddress(const string& a)         { address       = a;  }
    void setAadhaarNumber(const string& aa)  { aadhaarNumber = aa; }
    void setPANNumber(const string& pan)     { PANNumber     = pan;}

    void addAccount(Account* acc) { accounts.push_back(acc); }
    void addLoan(Loan* loan)      { loans.push_back(loan);   }
};

class ATMCard {
private:
    long     cardNumber;
    int      CVV;
    Date     expiryDate;
    int      PIN;
    string   cardType;
    string   cardStatus;
    Account* linkedAccount;
public:
    ATMCard() : cardNumber(0), CVV(0), PIN(0),
                cardStatus("Active"), linkedAccount(nullptr) {}

    void blockCard() {
        cardStatus = "Blocked";
        cout << "Card " << cardNumber << " has been blocked." << endl;
    }

    void unblockCard() {
        cardStatus = "Active";
        cout << "Card " << cardNumber << " has been unblocked." << endl;
    }

    long     getCardNumber()    const { return cardNumber;    }
    int      getCVV()           const { return CVV;           }
    Date     getExpiryDate()    const { return expiryDate;    }
    string   getCardType()      const { return cardType;      }
    string   getCardStatus()    const { return cardStatus;    }
    Account* getLinkedAccount() const { return linkedAccount; }

    void setCardStatus(const string& s) { cardStatus    = s;      }
    void setPIN(int newPIN)             { PIN           = newPIN; }
    void setLinkedAccount(Account* a)   { linkedAccount = a;      }
};

class Notification {
protected:
    string message;
public:
    Notification() : message(" ") {}

    virtual void sendNotification() = 0;

    string getMessage() const { return message; }
    void setMessage(const string& msg) { message = msg; }

    virtual ~Notification() {}
};

class SMSNotification : public Notification {
private:
    string phoneNumber;
    string deliveryStatus;
public:
    SMSNotification() : phoneNumber(" "), deliveryStatus("Pending") {}

    void sendNotification() override {
        cout << "[SMS] To: " << phoneNumber << message << endl;
        deliveryStatus = "Sent";
    }

    string getPhoneNumber()    const { return phoneNumber;    }
    string getDeliveryStatus() const { return deliveryStatus; }

    void setPhoneNumber(const string& p) { phoneNumber = p; }
};

class EmailNotification : public Notification {
private:
    string emailAddress;
    string subject;
    string deliveryStatus;
public:
    EmailNotification() : emailAddress(" "), subject(" "),
                          deliveryStatus("Pending") {}

    void sendNotification() override {
        cout << "[Email] To: " << emailAddress << "Subject:" << subject << message << endl;
        deliveryStatus = "Sent";
    }

    string getEmailAddress()   const { return emailAddress;   }
    string getSubject()        const { return subject;        }
    string getDeliveryStatus() const { return deliveryStatus; }

    void setEmailAddress(const string& e) { emailAddress = e; }
    void setSubject(const string& s)      { subject      = s; }
};

class Bank {
private:
    int    bankId;
    string bankName;
    vector<Branch*>   branches;
    vector<Customer*> customers;
    vector<Employee*> employees;
public:
    Bank() : bankId(0) {}

    int    getBankId()   const { return bankId;   }
    string getBankName() const { return bankName; }
    const vector<Branch*>&   getBranches()  const { return branches;  }
    const vector<Customer*>& getCustomers() const { return customers; }
    const vector<Employee*>& getEmployees() const { return employees; }

    void setBankId(int id)            { bankId   = id; }
    void setBankName(const string& n) { bankName = n;  }

    void addBranch(Branch* b)     { branches.push_back(b);  }
    void addCustomer(Customer* c) { customers.push_back(c); }
    void addEmployee(Employee* e) { employees.push_back(e); }
};

int main() {


    Bank bank;
    bank.setBankId(1);
    string BankName;
    cin >> BankName;
    bank.setBankName(BankName);

    Branch* b1 = new Branch();
    int branchId;
    cin >> branchId;
    b1->setBranchId(branchId);
    string BranchName;
    cin >> BranchName;
    b1->setBranchName(BranchName);
    string IFSC;
    cin >> IFSC;
    b1->setIFSCCode(IFSC);
    string address;
    cin >> address;
    b1->setAddress(address);
    bank.addBranch(b1);
    int dobm, dobd, doby;
    cin >> dobd >> dobm >> doby;

    Date dob(dobd, dobm, doby);
    Customer* c1 = new Customer(
        1001, "Pravish Mishra", dob, "Male",
        "9140785394", "pravishmi25@iitk.ac.in",
        "Kanpur", "1234-5678-9012", "ABCDE1234"
    );
    bank.addCustomer(c1);

    SavingsAccount* sa = new SavingsAccount();
    sa->setDate(Date(18, 5, 2026));
    sa->setStatus("Active");
    sa->setBranch(b1);
    sa->setCustomer(c1);
    sa->setInterestRate(4.0);
    sa->setMinimumBalance(1000.0);
    b1->addAccount(sa);
    c1->addAccount(sa);
    sa->displayInfo();

    Transaction* t1 = new Transaction();
    t1->execute();
    sa->addTransaction(t1);
    sa->getTransactionHistory();

    SMSNotification* sms = new SMSNotification();
    sms->setPhoneNumber("9140785394");
    sms->setMessage("Your account was credited Rs.69000.");
    sms->sendNotification();

    EmailNotification* email = new EmailNotification();
    email->setEmailAddress("pravishmi25@iitk.ac.in");
    email->setSubject("Account Alert");
    email->setMessage("Dear Pravish, your account was credited.");
    email->sendNotification();

    cout << "\nBank: " << bank.getBankName() << endl;

    return 0;
}
