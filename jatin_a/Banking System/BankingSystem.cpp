#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>
#include <stdexcept>

using namespace std;

/*  EXCEPTIONS  */

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
        return "Account is blocked!";
    }
};

class LoanRejectedException : public exception {
public:
    const char* what() const noexcept override {
        return "Loan Rejected!";
    }
};

/* PRE DECLARATIONS */
class Account;
class Customer;

/* NOTIFICATION */

class Notification {
public:
    virtual void sendNotification(string msg) = 0;
};

class SMSNotification : public Notification {
public:
    void sendNotification(string msg) override {
        cout << "[SMS] " << msg << endl;
    }
};

class EmailNotification : public Notification {
public:
    void sendNotification(string msg) override {
        cout << "[EMAIL] " << msg << endl;
    }
};

/* TRANSACTION */

class Transaction {
public:
    int id; //transaction id for uniqueness
    string type; //deposit withdraw transfer
    double amount;
    Account* sender;
    Account* receiver;

    Transaction(int id, string type, double amount,
                Account* s, Account* r)
        : id(id), type(type), amount(amount),
          sender(s), receiver(r) {}
};

/* ACCOUNT */

class Account {
protected:
    long accNo;
    double balance;
    string status; //active/blocked
    Customer* customer; //owner
    vector<Transaction*> history;

public:
    Account(long accNo, double bal, Customer* cust)
        : accNo(accNo), balance(bal), customer(cust) {
        status = "Active";
    }

    virtual void withdraw(double amt) = 0;

    void deposit(double amt) {
        if(status == "Blocked")
            throw AccountBlockedException();
        balance += amt;
    }

    double getBalance() { return balance; }

    void addTransaction(Transaction* t) {
        history.push_back(t);
    }

    string getStatus() { return status; }

    void block() { status = "Blocked"; }

    Customer* getCustomer() { return customer; }
};

/* SAVINGS */

class SavingsAccount : public Account {
    double minBalance;

public:
    SavingsAccount(long accNo, double bal,
                   double minBal, Customer* cust)
        : Account(accNo, bal, cust),
          minBalance(minBal) {}

    void withdraw(double amt) override {
        if(balance - amt < minBalance)
            throw InsufficientBalanceException();
        balance -= amt;
    }
};

/* CURRENT */

class CurrentAccount : public Account {
    double overdraft;

public:
    CurrentAccount(long accNo, double bal,
                   double od, Customer* cust)
        : Account(accNo, bal, cust),
          overdraft(od) {}

    void withdraw(double amt) override {
        if(balance + overdraft < amt)
            throw InsufficientBalanceException();
        balance -= amt;
    }
};

/* FIXED DEPOSIT */

class FixedDepositAccount : public Account {
    time_t maturity;

public:
    FixedDepositAccount(long accNo, double bal,
                        int months, Customer* cust)
        : Account(accNo, bal, cust) {
        maturity = time(0) + months * 30 * 24 * 60 * 60;
    }

    void withdraw(double amt) override {
        if(time(0) < maturity)
            throw runtime_error("FD not matured");
        if(amt > balance)
            throw InsufficientBalanceException();
        balance -= amt;
    }
};

/*  CUSTOMER  */

class Customer {
public:
    string name;
    vector<Account*> accounts;

    Customer(string name) : name(name) {}

    void addAccount(Account* acc) {
        accounts.push_back(acc);
    }
};

/*  LOAN  */

class Loan {
    double amount;
    double rate;
    int years;
    double emi;

public:
    Loan(double amt, double rate, int yrs)
        : amount(amt), rate(rate), years(yrs) {
        double r = rate / (12 * 100);
        int n = years * 12;

        emi = (amount * r * pow(1+r,n)) /
              (pow(1+r,n) - 1);
    }

    void showEMI() {
        cout << "EMI: " << emi << endl;
    }
};

/*  ATM CARD  */

class ATMCard {
    int pin;
    Account* acc;
    string status;

public:
    ATMCard(int pin, Account* acc)
        : pin(pin), acc(acc), status("Active") {}

    void validate(int p) {
        if(p != pin)
            throw InvalidPINException();
    }

    void withdraw(int p, double amt) {
        if(status == "Blocked")
            throw runtime_error("Card blocked");

        validate(p);
        acc->withdraw(amt);

        cout << "ATM Withdrawal successful\n";
    }

    Account* getAccount() { return acc; }
};

/*  TRANSACTION MANAGER */

class TransactionManager {
    static int counter; //unique transaction ids

public:
    static void deposit(Account* acc, double amt) {
        acc->deposit(amt);

        Transaction* t =
            new Transaction(++counter,"Deposit",amt,nullptr,acc);

        acc->addTransaction(t);

        SMSNotification().sendNotification("Deposit successful");
    }

    static void withdraw(Account* acc, double amt) {
        acc->withdraw(amt);

        Transaction* t =
            new Transaction(++counter,"Withdraw",amt,acc,nullptr);

        acc->addTransaction(t);

        EmailNotification().sendNotification("Withdrawal successful");
    }

    static void transfer(Account* a, Account* b, double amt) {
        a->withdraw(amt);
        b->deposit(amt);

        Transaction* t =
            new Transaction(++counter,"Transfer",amt,a,b);

        a->addTransaction(t);
        b->addTransaction(t);

        SMSNotification().sendNotification("Transfer successful");
    }
};

int TransactionManager::counter = 0;

/* FACTORY */

class AccountFactory {
public:
    static Account* createSavings(long id,double bal,double min,Customer* c) {
        return new SavingsAccount(id,bal,min,c);
    }

    static Account* createCurrent(long id,double bal,double od,Customer* c) {
        return new CurrentAccount(id,bal,od,c);
    }

    static Account* createFD(long id,double bal,int months,Customer* c) {
        return new FixedDepositAccount(id,bal,months,c);
    }
};

int main(){

    return 0;
}