#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>

using namespace std;

//exceptions
class LowBal : public exception {
    const char* what() const throw() { return "balance too low"; }
};

class WrongPin : public exception {
    const char* what() const throw() { return "incorrect pin"; }
};

class Notification {
public:
    virtual void send(string m) = 0;
    virtual ~Notification() {}
};

class SMS : public Notification {
    string phone;
public:
    SMS(string p) { phone = p; }
    void send(string m) override { 
        cout << "SMS to " << phone << ": " << m << "\n"; 
    }
};

class Email : public Notification {
    string mail_id; 
public:
    Email(string e) : mail_id(e) {} 
    void send(string m) override { 
        cout << "Email to " << mail_id << ": " << m << "\n"; 
    }
};

//base account
class Account {
protected:
    long long acc_no;
    double balance;
    int status; // 1->active

public:
    Account(long id, double b) : acc_no(id), balance(b), status(1) {}
    virtual ~Account() {}

    virtual void withdraw(double amt) = 0; 
    
    virtual void deposit(double amt) {
        if(status == 0) throw runtime_error("acc blocked");
        balance += amt;
    }

    double getBal() { return balance; }
    void blockAcc() { status = 0; }
    
    virtual void display() = 0;
};

//inherited class
class Savings : public Account {
    double min_bal = 1000;
public:
    Savings(long id, double b) : Account(id, b) {}
    
    void withdraw(double amt) override {
        if(status == 0) throw runtime_error("acc blocked");
        if(balance - amt < min_bal) throw LowBal();
        balance -= amt;
    }

    void display() override { 
        cout << "Savings Acc: " << acc_no << " | Bal: " << balance << "\n"; 
    }
};

class CurrentAcc : public Account {
    double od_limit;
public:
    CurrentAcc(long id, double b) : Account(id, b) { 
        od_limit = 5000; 
    }
    
    void withdraw(double amt) override {
        if(status == 0) throw runtime_error("acc blocked");
        if(balance + od_limit < amt) throw LowBal();
        balance -= amt;
    }

    void display() override { 
        cout << "Current Acc: " << acc_no << " | Bal: " << balance << "\n"; 
    }
};

class FD : public Account {
    int duration_months;
public:
    FD(long id, double b, int d) : Account(id, b) { duration_months = d; }
    
    void withdraw(double amt) override { 
        throw runtime_error("cant withdraw from FD directly"); 
    }

    void display() override {
        double final_amt = balance + (balance * 6.5 * duration_months) / 1200;
        cout << "FD Acc: " << acc_no << " | Principal: " << balance 
             << " | Maturity: " << final_amt << "\n";
    }
};

class Loan {
public:
    string type;
    double amount;
    double rate;
    int yrs;
    double emi;

    Loan(string t, double a, double r, int y) {
        type = t; amount = a; rate = r; yrs = y;
        double monthly = rate / 1200;
        int total_months = yrs * 12;
        emi = amount * monthly * pow(1 + monthly, total_months) / (pow(1 + monthly, total_months) - 1);
    }
};

class Customer {
    int id;
    string name;
    string phone;
    string email;
    
    vector<Account*> accounts;
    vector<Loan*> loans;

public:
    Customer(int i, string n, string p, string e) {
        id = i; name = n; phone = p; email = e;
    }

    void addAccount(Account* a) { accounts.push_back(a); }
    void addLoan(Loan* l) { loans.push_back(l); }
    
    string getPhone() { return phone; }
    string getEmail() { return email; }

    void showDashboard() {
        cout << "\n--- Accounts for " << name << " ---\n";
        for(auto acc : accounts) {
            acc->display();
        }
        cout << "\n";
    }
};

class Employee {
    int empId;
    string name;
    string role;
    double salary;
public:
    Employee(int id, string n, string r, double s) : empId(id), name(n), role(r), salary(s) {}
};

class Branch {
    int bCode;
    string bName;
    string ifsc;
    vector<Account*> bAccs;
    vector<Employee*> bStaff;

public:
    Branch(int c, string n, string code) {
        bCode = c; bName = n; ifsc = code;
    }
    void addAcc(Account* a) { bAccs.push_back(a); }
    void addEmp(Employee* e) { bStaff.push_back(e); }
};

class Bank {
    string bankName;
    vector<Branch*> branches;
    vector<Customer*> customers;
public:
    Bank(string n) { bankName = n; }
    void addBranch(Branch* b) { branches.push_back(b); }
    void addCustomer(Customer* c) { customers.push_back(c); }
};
 
class Transaction {
public:
    static void deposit(Account* a, double amt, Notification* n = nullptr) {
        a->deposit(amt);
        if(n) n->send("credited " + to_string(amt));
    }
    
    static void withdraw(Account* a, double amt, Notification* n = nullptr) {
        a->withdraw(amt);
        if(n) n->send("debited " + to_string(amt));
    }
    
    static void transfer(Account* a1, Account* a2, double amt, Notification* n = nullptr) {
        a1->withdraw(amt);
        a2->deposit(amt);
        if(n) n->send("transferred " + to_string(amt));
    }
};

class ATM {
    long long card_num;
    int pin;
    int fails;
    Account* linked_acc;
public:
    ATM(long long c, int p, Account* a) {
        card_num = c; pin = p; linked_acc = a; fails = 0;
    }

    void swipe(int entered_pin, double amt) {
        if(fails >= 3) throw runtime_error("atm card blocked");
        
        if(entered_pin != pin) {
            fails++;
            throw WrongPin();
        }
        
        fails = 0;
        linked_acc->withdraw(amt);
        cout << "ATM cash out: " << amt << "\n";
    }
};


void approveLoan(Loan* l, double income) {
    if(l->emi > income * 0.4) {
        throw runtime_error("loan rejected - emi too high");
    }
    cout << "Loan Approved.\n";
}

int main() {
    Branch* b1 = nullptr;
    Employee* e1 = nullptr;
    Customer* c1 = nullptr;
    Account* s = nullptr;
    Account* c = nullptr;
    Account* fd = nullptr;
    Loan* carLoan = nullptr;

    try {
        Bank sbi("SBI Bank");
        
        b1 = new Branch(101, "SBI Main", "SBIN0001020");
        sbi.addBranch(b1);

        e1 = new Employee(55, "Ramesh", "Manager", 65000);
        b1->addEmp(e1);

        c1 = new Customer(1, "Krishna", "9876123450", "krishna@mail.com");
        sbi.addCustomer(c1);

        s = new Savings(100111, 15000);
        c = new CurrentAcc(100112, 8000);
        fd = new FD(100113, 40000, 12);

        c1->addAccount(s);
        c1->addAccount(c);
        c1->addAccount(fd);

        b1->addAcc(s);
        b1->addAcc(c);
        b1->addAcc(fd);

        SMS smsAlert(c1->getPhone());
        Email emailAlert(c1->getEmail());

        Transaction::deposit(s, 3000, &smsAlert);
        Transaction::withdraw(c, 1500, &emailAlert);
        Transaction::transfer(s, c, 2000, &smsAlert);

        carLoan = new Loan("Car", 500000, 9.5, 5);
        approveLoan(carLoan, 85000);
        c1->addLoan(carLoan);

        ATM myAtm(555544443333, 1122, s);
        myAtm.swipe(1122, 500);

        c1->showDashboard();
        cout << "Loan details -> Type: " << carLoan->type << " | EMI: " << carLoan->emi << "\n";

    } catch (exception& e) {
        cout << "error: " << e.what() << "\n";
    }

    return 0; 
}
