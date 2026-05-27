// could not undesrstood what to put in main so kept it empty so part of the code is written using gpt because it was too repetative 



#include <stdio.h>
#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
using namespace std;

class Notification {
public:
    virtual void send() = 0;
};

class SMSNotification : public Notification {
public:
    string a;


    string b;

    SMSNotification(string x, string y) {
    
    
        a = x;
        b = y;
    }

    void send() {
        cout << "SMS Sent To " << a << " : " << b << endl;
    }
};

class EmailNotification : public Notification {
public:
    string a;
    string b;

    EmailNotification(string x, string y) {
        a = x;
        b = y;
    }

    void send() {
        cout << "Email Sent To " << a << " : " << b << endl;
    }
};

class InsufficientBalanceException : public exception {
public:
    const char* what() const throw() {
        return "Insufficient Balance";
    }
};

class InvalidPINException : public exception {
public:
    const char* what() const throw() {
        return "Invalid PIN";
    }
};

class AccountBlockedException : public exception {
public:
    const char* what() const throw() {
        return "Blocked Account";
    }
};

class Transaction {
public:
    int a;

    string b;
    double c;
    string d;

    Transaction() {}

    Transaction(int x, string y, double z, string k) {
        a = x;
        b = y;
        c = z;
        d = k;
    }

    void show() {
        cout << "Transaction ID : " << a << endl;
        cout << "Type : " << b << endl;
        cout << "Amount : " << c << endl;
        cout << "Status : " << d << endl;
    }
};

class Account;

class Customer {
public:
    int a;
    string b;
    string c;
    string d;

    Account* e[10];
    int f;

    Customer() {}

    Customer(int x, string y, string z, string k) {
        a = x;
        b = y;
        c = z;
        d = k;
        f = 0;
    }

    void addAccount(Account* g) {
        e[f] = g;
        f++;
    }
};

class Employee {
public:
    int a;
    string b;
    string c;
    double d;

    Employee() {}

    Employee(int x, string y, string z, double k) {
        a = x;
        b = y;
        c = z;
        d = k;
    }

    void show() {
        cout << b << " " << c << " " << d << endl;
    }
};

class Branch {
public:
    int a;
    string b;
    string c;

    Employee d[20];
    int e;

    Branch() {}

    Branch(int x, string y, string z) {
        a = x;
        b = y;
        c = z;
        e = 0;
    }

    void addEmployee(Employee f) {
        d[e] = f;
        e++;
    }
};

class Account {
public:
    long long a;
    string b;
    double c;
    string d;
    Customer* e;

    Transaction f[100];
    int g;

    Account() {}

    Account(long long x, string y, double z, Customer* k) {
        a = x;
        b = y;
        c = z;
        d = "Active";
        e = k;
        g = 0;
    }

    virtual void deposit(double h) {
        if(d == "Blocked") {
            throw AccountBlockedException();
        }

        c += h;
        f[g] = Transaction(rand()%1000, "Deposit", h, "Success");
        g++;
    }

    virtual void withdraw(double h) {
        if(d == "Blocked") {
            throw AccountBlockedException();
        }

        if(c < h) {
            throw InsufficientBalanceException();
        }

        c -= h;
        f[g] = Transaction(rand()%1000, "Withdraw", h, "Success");
        g++;
    }

    void transfer(Account* h, double i) {
        withdraw(i);
        h->deposit(i);

        f[g] = Transaction(rand()%1000, "Transfer", i, "Success");
        g++;
    }

    virtual void update() = 0;
};

class SavingsAccount : public Account {
public:
    double h;
    double i;

    SavingsAccount(long long x, double y, Customer* z, double k, double l)
    : Account(x, "Savings", y, z) {
        h = k;
        i = l;
    }

    void withdraw(double j) {
        if(c - j < i) {
            throw InsufficientBalanceException();
        }

        c -= j;

        f[g] = Transaction(rand()%1000, "Withdraw", j, "Success");
        g++;
    }

    void update() {
        c += (c * h)/100;
    }
};

class CurrentAccount : public Account {
public:
    double h;
    string i;

    CurrentAccount(long long x, double y, Customer* z, double k, string l)
    : Account(x, "Current", y, z) {
        h = k;
        i = l;
    }

    void withdraw(double j) {
        if(c + h < j) {
            throw InsufficientBalanceException();
        }

        c -= j;

        f[g] = Transaction(rand()%1000, "Withdraw", j, "Success");
        g++;
    }

    void update() {
        c -= 100;
    }
};

class FixedDepositAccount : public Account {
public:
    double h;
    int i;

    FixedDepositAccount(long long x, double y, Customer* z, double k, int l)
    : Account(x, "FD", y, z) {
        h = k;
        i = l;
    }

    void update() {
        c += (c * h * i)/100;
    }
};

class Loan {
public:
    int a;
    string b;
    double c;
    double d;
    int e;
    double f;

    Loan(int x, string y, double z, double k, int l) {
        a = x;
        b = y;
        c = z;
        d = k;
        e = l;

        double m = d/(12*100);
        int n = e*12;

        f = (c*m*pow(1+m,n))/(pow(1+m,n)-1);
    }

    void show() {
        cout << "Loan Type : " << b << endl;
        cout << "EMI : " << f << endl;
    }
};

class ATMCard {
public:
    long long a;
    int b;
    Account* c;

    ATMCard(long long x, int y, Account* z) {
        a = x;
        b = y;
        c = z;
    }

    void verify(int d) {
        if(b != d) {
            throw InvalidPINException();
        }

        cout << "PIN Verified" << endl;
    }
};

class Bank {
public:
    int a;
    string b;




    Branch c[20];
    int d;

    Customer e[50];

    int f;

    Bank(int x, string y) {
        a = x;


        b = y;
        d = 0;
        f = 0;
    }

    void addBranch(Branch g) {
        c[d] = g;
        d++;
    }

    void addCustomer(Customer g) {
        e[f] = g;
        f++;
    }
};




class AccountFactory {
public:

    static Account* createAccount(int a, long long b, double c, Customer* d) {
        if(a == 1) {



            
            return new SavingsAccount(b, c, d, 4, 1000);
        
        
        }

        else if(a == 2) {


            return new CurrentAccount(b, c, d, 5000, "ABC Traders");
        }

        else {

            return new FixedDepositAccount(b, c, d, 7, 2);
        }
    }
};

int main() {


    return 0;
}

