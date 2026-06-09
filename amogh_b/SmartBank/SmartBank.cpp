#include <iostream>
#include <vector>
#include <string>
using namespace std;


class InsufficientBalanceException : public exception {
public:
    const char* what() const noexcept override { return "Insufficient Balance!"; }
};

class InvalidPINException : public exception {
public:
    const char* what() const noexcept override { return "Invalid PIN!"; }
};

class AccountBlockedException : public exception {
public:
    const char* what() const noexcept override { return "Account is blocked!"; }
};

class LoanRejectedException : public exception {
public:
    const char* what() const noexcept override { return "Loan Rejected!"; }
};


class Account;
class Customer;
class Branch;



class Notification {
public:
    virtual void send(string msg) = 0;
    virtual ~Notification() {}
};

class SMSNotification : public Notification {
public:
    void send(string msg) override { cout << "[SMS] " << msg << endl; }
};

class EmailNotification : public Notification {
public:
    void send(string msg) override { cout << "[EMAIL] " << msg << endl; }
};



class Transaction {
public:
    int id;
    string type;        
    double amt;
    Account* sender;
    Account* receiver;

    Transaction(int id, string type, double amt, Account* s, Account* r)
        : id(id), type(type), amt(amt), sender(s), receiver(r) {}
};



class Account {
protected:
    long accNo;
    double bal;
    string status;      
    Customer* cust;
    Branch* branch;
    vector<Transaction*> history;

public:
    Account(long accNo, double bal, Customer* cust)
        : accNo(accNo), bal(bal), status("Active"), cust(cust), branch(nullptr) {}

    virtual ~Account() {}

    virtual void withdraw(double a) = 0;

    void deposit(double a) {
        if (status == "Blocked") throw AccountBlockedException();
        bal += a;
    }

    double getBal() { return bal; }
    string getStatus() { return status; }
    void block() { status = "Blocked"; }
    void setBranch(Branch* b) { branch = b; }
    Customer* getCust() { return cust; }
    void addTxn(Transaction* t) { history.push_back(t); }
};



class SavingsAccount : public Account {
    double minBal;
    double rate;
public:
    SavingsAccount(long accNo, double bal, double minBal, Customer* c)
        : Account(accNo, bal, c), minBal(minBal), rate(4.0) {}

    void withdraw(double a) override {
        if (status == "Blocked") throw AccountBlockedException();
        if (bal - a < minBal) throw InsufficientBalanceException();
        bal -= a;
    }
};



class CurrentAccount : public Account {
    double overdraft;
    string bizName;
public:
    CurrentAccount(long accNo, double bal, double od, Customer* c, string biz = "")
        : Account(accNo, bal, c), overdraft(od), bizName(biz) {}

    void withdraw(double a) override {
        if (status == "Blocked") throw AccountBlockedException();
        if (bal + overdraft < a) throw InsufficientBalanceException();
        bal -= a;
    }
};



class FixedDepositAccount : public Account {
    double rate;
    int months;
public:
    FixedDepositAccount(long accNo, double bal, int months, Customer* c)
        : Account(accNo, bal, c), rate(7.0), months(months) {}

    double maturityAmount() {
        return bal + (bal * rate * months / 1200.0);  
    }

    void withdraw(double a) override {
        throw runtime_error("Cannot withdraw from FD before maturity");
    }
};



class Customer {
public:
    int id;
    string name;
    vector<Account*> accounts;
    vector<class Loan*> loans;

    Customer(int id, string name) : id(id), name(name) {}
    void addAccount(Account* a) { accounts.push_back(a); }
    void addLoan(class Loan* l) { loans.push_back(l); }
};



class Loan {
    int id;
    string type;        
    double amt;
    double rate;
    int years;
    double emi;
    string status;
    Customer* cust;
public:
    Loan(int id, string type, double amt, double rate, int years, Customer* c)
        : id(id), type(type), amt(amt), rate(rate), years(years),
          status("Pending"), cust(c) {
        double interest = amt * rate * years / 100.0;
        emi = (amt + interest) / (years * 12);
    }

    void approve() {
        if (amt > 5000000) throw LoanRejectedException();
        status = "Approved";
    }

    double getEMI() { return emi; }
    string getStatus() { return status; }
};



class ATMCard {
    long cardNo;
    int pin;
    string status;      
    Account* acc;
public:
    ATMCard(long cardNo, int pin, Account* acc)
        : cardNo(cardNo), pin(pin), status("Active"), acc(acc) {}

    void block() { status = "Blocked"; }

    void withdraw(int p, double a) {
        if (status == "Blocked") throw runtime_error("Card blocked");
        if (p != pin) throw InvalidPINException();
        acc->withdraw(a);
    }

    Account* getAccount() { return acc; }
};



class Employee {
public:
    int id;
    string name;
    string designation;     
    double salary;
    Branch* branch;

    Employee(int id, string name, string desig, double salary)
        : id(id), name(name), designation(desig), salary(salary), branch(nullptr) {}
};


class Branch {
public:
    int id;
    string name;
    string ifsc;
    vector<Account*> accounts;
    vector<Employee*> employees;

    Branch(int id, string name, string ifsc) : id(id), name(name), ifsc(ifsc) {}
    void addAccount(Account* a) { accounts.push_back(a); }
    void addEmployee(Employee* e) { employees.push_back(e); }
};



class Bank {
public:
    int id;
    string name;
    vector<Branch*> branches;
    vector<Customer*> customers;
    vector<Employee*> employees;

    Bank(int id, string name) : id(id), name(name) {}
    void addBranch(Branch* b) { branches.push_back(b); }
    void addCustomer(Customer* c) { customers.push_back(c); }
    void addEmployee(Employee* e) { employees.push_back(e); }
};



class TransactionManager {
    static int counter;
public:
    static void deposit(Account* acc, double a) {
        acc->deposit(a);
        acc->addTxn(new Transaction(++counter, "Deposit", a, nullptr, acc));
        SMSNotification().send("Deposit successful");
    }

    static void withdraw(Account* acc, double a) {
        acc->withdraw(a);
        acc->addTxn(new Transaction(++counter, "Withdraw", a, acc, nullptr));
        EmailNotification().send("Withdrawal successful");
    }

    static void transfer(Account* a, Account* b, double amt) {
        a->withdraw(amt);
        b->deposit(amt);
        Transaction* t = new Transaction(++counter, "Transfer", amt, a, b);
        a->addTxn(t);
        b->addTxn(t);
        SMSNotification().send("Transfer successful");
    }
};

int TransactionManager::counter = 0;



class AccountFactory {
public:
    static Account* createSavings(long id, double bal, double min, Customer* c) {
        return new SavingsAccount(id, bal, min, c);
    }
    static Account* createCurrent(long id, double bal, double od, Customer* c) {
        return new CurrentAccount(id, bal, od, c);
    }
    static Account* createFD(long id, double bal, int months, Customer* c) {
        return new FixedDepositAccount(id, bal, months, c);
    }
};


int main() {

    Bank bank(1, "SmartBank");
    Branch* branch = new Branch(101, "Kanpur Main", "SMRT0001");
    bank.addBranch(branch);
 
    Employee* mgr = new Employee(1, "Ravi", "Manager", 90000);
    branch->addEmployee(mgr);
    bank.addEmployee(mgr);

    Customer* amogh = new Customer(1, "Amogh");
    Customer* neha  = new Customer(2, "Neha");
    bank.addCustomer(amogh);
    bank.addCustomer(neha);
 
    Account* sav = AccountFactory::createSavings(5001, 5000, 1000, amogh);
    Account* cur = AccountFactory::createCurrent(5002, 10000, 50000, neha);
    Account* fd  = AccountFactory::createFD(5003, 100000, 12, amogh);
 
    amogh->addAccount(sav);
    amogh->addAccount(fd);
    neha->addAccount(cur);
    branch->addAccount(sav);
    branch->addAccount(cur);
    branch->addAccount(fd);
    sav->setBranch(branch);
    cur->setBranch(branch);
    fd->setBranch(branch);
 
    cout << "--- Transactions ---" << endl;
    TransactionManager::deposit(sav, 2000);
    TransactionManager::withdraw(sav, 1000);
    cout << "Savings balance: " << sav->getBal() << endl;
 
    TransactionManager::withdraw(cur, 15000);   
    cout << "Current balance: " << cur->getBal() << endl;
 

    cout << "\n--- Transfer ---" << endl;
    TransactionManager::transfer(sav, cur, 1000);
    cout << "Savings: " << sav->getBal() << " | Current: " << cur->getBal() << endl;
 

    cout << "\n--- Fixed Deposit ---" << endl;
    cout << "Maturity amount: "
         << ((FixedDepositAccount*)fd)->maturityAmount() << endl;


    cout << "\n--- Loan ---" << endl;
    Loan* home = new Loan(1, "Home", 2000000, 8.5, 20, amogh);
    amogh->addLoan(home);
    home->approve();
    cout << "Status: " << home->getStatus()
         << " | EMI: " << home->getEMI() << endl;
 
    cout << "\n--- ATM ---" << endl;
    ATMCard card(123456789, 4567, sav);
    try {
        card.withdraw(1111, 500);   
    } catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    card.withdraw(4567, 500);       
    cout << "Savings balance: " << sav->getBal() << endl;
 

 
    delete home;
    delete sav; delete cur; delete fd;
    delete amogh; delete neha;
    delete mgr; delete branch;
 
    return 0;
}
