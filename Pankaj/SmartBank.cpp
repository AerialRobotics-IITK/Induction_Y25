#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <ctime>
#include <map>
#include <exception>
#include <cmath>
#include <algorithm>

using namespace std;

// Forward Declarations
class Transaction;
class Branch;
class Customer;
class Account;
class Loan;
class Employee;

// --- Exceptions.h ---
class BankException : public std::exception {
protected:
    std::string message;
public:
    BankException(const std::string& msg) : message(msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class InsufficientBalanceException : public BankException {
public:
    InsufficientBalanceException() : BankException("Error: Insufficient balance in account.") {}
};

class InvalidPINException : public BankException {
public:
    InvalidPINException() : BankException("Error: Invalid ATM PIN entered.") {}
};

class AccountBlockedException : public BankException {
public:
    AccountBlockedException() : BankException("Error: Account is currently blocked.") {}
};

class LoanRejectedException : public BankException {
public:
    LoanRejectedException(const std::string& reason) : BankException("Loan Rejected: " + reason) {}
};

// --- Notification.h ---
class Notification {
public:
    virtual ~Notification() {}
    virtual void sendNotification(const std::string& recipient, const std::string& message) = 0;
};

class SMSNotification : public Notification {
public:
    void sendNotification(const std::string& recipient, const std::string& message) override {
        std::cout << "[SMS sent to " << recipient << "]: " << message << std::endl;
    }
};

class EmailNotification : public Notification {
public:
    void sendNotification(const std::string& recipient, const std::string& message) override {
        std::cout << "[Email sent to " << recipient << "]: " << message << std::endl;
    }
};

// --- Transaction.h ---
class Transaction {
private:
    std::string transactionId;
    std::string transactionType;
    double amount;
    std::string transactionDate;
    std::string status;
    std::string senderAccNum;
    std::string receiverAccNum;

public:
    Transaction(std::string id, std::string type, double amt, std::string date, std::string sender = "", std::string receiver = "");
    void processTransaction();
    void displayTransaction() const;
    std::string getType() const { return transactionType; }
    double getAmount() const { return amount; }
};

// --- Account.h ---
class Account {
protected:
    std::string accountNumber;
    std::string accountType;
    double balance;
    std::string dateOpened;
    std::string status;
    Branch* branch;
    Customer* customer;
    std::vector<std::shared_ptr<Transaction>> transactions;

public:
    Account(std::string accNum, std::string type, double bal, Branch* br, Customer* cust);
    virtual ~Account() {}

    virtual void deposit(double amount);
    virtual void withdraw(double amount) = 0;
    virtual void calculateInterest() = 0;
    
    void transferFunds(std::shared_ptr<Account> receiver, double amount);
    void viewBalance() const;
    void printStatement() const;
    
    std::string getAccountNumber() const { return accountNumber; }
    std::string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }
    std::string getStatus() const { return status; }
    void addTransaction(std::shared_ptr<Transaction> t);
    void setStatus(std::string s) { status = s; }
};

// --- SavingsAccount.h ---
class SavingsAccount : public Account {
private:
    double interestRate;
    double minimumBalance;
    double dailyWithdrawalLimit;

public:
    SavingsAccount(std::string accNum, double bal, Branch* br, Customer* cust);
    void withdraw(double amount) override;
    void calculateInterest() override;
};

// --- CurrentAccount.h ---
class CurrentAccount : public Account {
private:
    double overdraftLimit;
    std::string businessName;

public:
    CurrentAccount(std::string accNum, double bal, Branch* br, Customer* cust, std::string bName);
    void withdraw(double amount) override;
    void calculateInterest() override;
};

// --- FixedDepositAccount.h ---
class FixedDepositAccount : public Account {
private:
    double FDAmount;
    std::string maturityDate;
    double FDInterestRate;
    int tenureMonths;

public:
    FixedDepositAccount(std::string accNum, double amt, Branch* br, Customer* cust, int months);
    void withdraw(double amount) override;
    void calculateInterest() override;
};

// --- Customer.h ---
class Customer {
private:
    std::string customerId;
    std::string fullName;
    std::string dob;
    std::string gender;
    std::string mobileNumber;
    std::string email;
    std::string address;
    std::string aadhaarNumber;
    std::string PANNumber;
    std::vector<std::shared_ptr<Account>> accounts;
    std::vector<std::shared_ptr<Loan>> loans;

public:
    Customer(std::string id, std::string name, std::string phone, std::string emailAddr);
    void openAccount(std::shared_ptr<Account> acc);
    void applyLoan(std::shared_ptr<Loan> ln);
    void viewAccounts() const;
    
    std::string getName() const { return fullName; }
    std::string getId() const { return customerId; }
    std::string getEmail() const { return email; }
    std::string getMobile() const { return mobileNumber; }
    std::vector<std::shared_ptr<Account>> getAccounts() { return accounts; }
};

// --- Employee.h ---
class Employee {
private:
    std::string employeeId;
    std::string employeeName;
    std::string designation;
    double salary;
    Branch* branch;

public:
    Employee(std::string id, std::string name, std::string desig, double sal, Branch* br);
    void displayEmployeeDetails() const;
    std::string getName() const { return employeeName; }
};

// --- Branch.h ---
class Branch {
private:
    std::string branchId;
    std::string branchName;
    std::string IFSCCode;
    std::string address;
    std::vector<std::shared_ptr<Account>> accounts;
    std::vector<std::shared_ptr<Employee>> employees;

public:
    Branch(std::string id, std::string name, std::string ifsc, std::string addr);
    void addAccount(std::shared_ptr<Account> acc);
    void addEmployee(std::shared_ptr<Employee> emp);
    void displayBranchDetails() const;
    
    std::string getName() const { return branchName; }
    std::string getIFSC() const { return IFSCCode; }
};

// --- Bank.h ---
class Bank {
private:
    std::string bankId;
    std::string bankName;
    std::vector<std::shared_ptr<Branch>> branches;
    std::vector<std::shared_ptr<Customer>> customers;
    std::vector<std::shared_ptr<Employee>> employees;

public:
    Bank(std::string id, std::string name);
    void addBranch(std::shared_ptr<Branch> br);
    void registerCustomer(std::shared_ptr<Customer> cust);
    void hireEmployee(std::shared_ptr<Employee> emp);
    void displayBankDetails() const;
    
    std::vector<std::shared_ptr<Branch>> getBranches() { return branches; }
    std::vector<std::shared_ptr<Customer>> getCustomers() { return customers; }
};

// --- Loan.h ---
class Loan {
private:
    std::string loanId;
    std::string loanType;
    double loanAmount;
    double interestRate;
    int tenureYears;
    double EMIAmount;
    std::string loanStatus;
    Customer* customer;

public:
    Loan(std::string id, std::string type, double amt, double rate, int years, Customer* cust);
    double calculateEMI();
    void approveLoan();
    void rejectLoan();
    void displayLoanDetails() const;
    std::string getStatus() const { return loanStatus; }
};

// --- ATMCard.h ---
class ATMCard {
private:
    std::string cardNumber;
    int CVV;
    std::string expiryDate;
    int PIN;
    std::string cardType;
    std::string cardStatus;
    std::shared_ptr<Account> linkedAccount;

public:
    ATMCard(std::string num, int cvv, std::string expiry, int pin, std::string type, std::shared_ptr<Account> acc);
    bool validatePIN(int enteredPIN);
    void blockCard();
    void activateCard();
    void withdrawCash(double amount, int enteredPIN);
};

// --- AccountFactory.h ---
class AccountFactory {
public:
    static std::shared_ptr<Account> createAccount(const std::string& type, std::string accNum, double initialBal, Branch* br, Customer* cust, std::string extra = "", int months = 0) {
        if (type == "Savings") {
            return std::make_shared<SavingsAccount>(accNum, initialBal, br, cust);
        } else if (type == "Current") {
            return std::make_shared<CurrentAccount>(accNum, initialBal, br, cust, extra);
        } else if (type == "FixedDeposit") {
            return std::make_shared<FixedDepositAccount>(accNum, initialBal, br, cust, months);
        }
        return nullptr;
    }
};

// ==========================================
// Definitions
// ==========================================

// --- Transaction.cpp ---
Transaction::Transaction(std::string id, std::string type, double amt, std::string date, std::string sender, std::string receiver)
    : transactionId(id), transactionType(type), amount(amt), transactionDate(date), status("Completed"), senderAccNum(sender), receiverAccNum(receiver) {}

void Transaction::processTransaction() {
    status = "Completed";
}

void Transaction::displayTransaction() const {
    std::cout << "[" << transactionDate << "] " << transactionType << ": " << amount 
              << " | ID: " << transactionId << " | Status: " << status << std::endl;
}

// --- Account.cpp ---
Account::Account(std::string accNum, std::string type, double bal, Branch* br, Customer* cust)
    : accountNumber(accNum), accountType(type), balance(bal), status("Active"), branch(br), customer(cust) {
    dateOpened = "2026-05-12"; 
}

void Account::deposit(double amount) {
    balance += amount;
    auto t = std::make_shared<Transaction>("T" + std::to_string(rand() % 10000), "Deposit", amount, "2026-05-12");
    addTransaction(t);
}

void Account::transferFunds(std::shared_ptr<Account> receiver, double amount) {
    if (balance >= amount) {
        this->withdraw(amount);
        receiver->deposit(amount);
        auto t = std::make_shared<Transaction>("T" + std::to_string(rand() % 10000), "Transfer", amount, "2026-05-12", this->accountNumber, receiver->getAccountNumber());
        addTransaction(t);
        std::cout << "Transfer of " << amount << " successful to " << receiver->getAccountNumber() << std::endl;
    } else {
        throw InsufficientBalanceException();
    }
}

void Account::viewBalance() const {
    std::cout << "Account: " << accountNumber << " | Balance: " << balance << std::endl;
}

void Account::printStatement() const {
    std::cout << "\n--- Statement for Account: " << accountNumber << " ---" << std::endl;
    for (const auto& t : transactions) {
        t->displayTransaction();
    }
    std::cout << "--------------------------------------" << std::endl;
}

void Account::addTransaction(std::shared_ptr<Transaction> t) {
    transactions.push_back(t);
}

// --- SavingsAccount.cpp ---
SavingsAccount::SavingsAccount(std::string accNum, double bal, Branch* br, Customer* cust)
    : Account(accNum, "Savings", bal, br, cust), interestRate(0.04), minimumBalance(500.0), dailyWithdrawalLimit(20000.0) {}

void SavingsAccount::withdraw(double amount) {
    if (balance - amount < minimumBalance) {
        throw InsufficientBalanceException();
    }
    balance -= amount;
    auto t = std::make_shared<Transaction>("T" + std::to_string(rand() % 10000), "Withdrawal", amount, "2026-05-12");
    addTransaction(t);
}

void SavingsAccount::calculateInterest() {
    double interest = balance * interestRate;
    balance += interest;
    std::cout << "Interest of " << interest << " applied to Savings Account " << accountNumber << std::endl;
}

// --- CurrentAccount.cpp ---
CurrentAccount::CurrentAccount(std::string accNum, double bal, Branch* br, Customer* cust, std::string bName)
    : Account(accNum, "Current", bal, br, cust), overdraftLimit(50000.0), businessName(bName) {}

void CurrentAccount::withdraw(double amount) {
    if (balance + overdraftLimit < amount) {
        throw InsufficientBalanceException();
    }
    balance -= amount;
    auto t = std::make_shared<Transaction>("T" + std::to_string(rand() % 10000), "Withdrawal", amount, "2026-05-12");
    addTransaction(t);
}

void CurrentAccount::calculateInterest() {
    std::cout << "Current Accounts do not earn interest." << std::endl;
}

// --- FixedDepositAccount.cpp ---
FixedDepositAccount::FixedDepositAccount(std::string accNum, double amt, Branch* br, Customer* cust, int months)
    : Account(accNum, "FixedDeposit", amt, br, cust), FDAmount(amt), FDInterestRate(0.07), tenureMonths(months) {
    maturityDate = "2027-05-12"; 
}

void FixedDepositAccount::withdraw(double amount) {
    std::cout << "Withdrawal blocked: Fixed Deposit accounts can only be closed at maturity." << std::endl;
}

void FixedDepositAccount::calculateInterest() {
    double interest = FDAmount * FDInterestRate * (tenureMonths / 12.0);
    std::cout << "Projected Maturity Interest: " << interest << std::endl;
}

// --- Customer.cpp ---
Customer::Customer(std::string id, std::string name, std::string phone, std::string emailAddr)
    : customerId(id), fullName(name), mobileNumber(phone), email(emailAddr) {}

void Customer::openAccount(std::shared_ptr<Account> acc) {
    accounts.push_back(acc);
}

void Customer::applyLoan(std::shared_ptr<Loan> ln) {
    loans.push_back(ln);
}

void Customer::viewAccounts() const {
    std::cout << "\n--- Accounts for " << fullName << " ---" << std::endl;
    for (const auto& acc : accounts) {
        acc->viewBalance();
    }
}

// --- Employee.cpp ---
Employee::Employee(std::string id, std::string name, std::string desig, double sal, Branch* br)
    : employeeId(id), employeeName(name), designation(desig), salary(sal), branch(br) {}

void Employee::displayEmployeeDetails() const {
    std::cout << "Employee ID: " << employeeId << " | Name: " << employeeName 
              << " | Designation: " << designation << " | Salary: " << salary << std::endl;
}

// --- Branch.cpp ---
Branch::Branch(std::string id, std::string name, std::string ifsc, std::string addr)
    : branchId(id), branchName(name), IFSCCode(ifsc), address(addr) {}

void Branch::addAccount(std::shared_ptr<Account> acc) {
    accounts.push_back(acc);
}

void Branch::addEmployee(std::shared_ptr<Employee> emp) {
    employees.push_back(emp);
}

void Branch::displayBranchDetails() const {
    std::cout << "Branch: " << branchName << " | IFSC: " << IFSCCode << " | Address: " << address << std::endl;
    std::cout << "Number of Accounts: " << accounts.size() << " | Number of Employees: " << employees.size() << std::endl;
}

// --- Bank.cpp ---
Bank::Bank(std::string id, std::string name) : bankId(id), bankName(name) {}

void Bank::addBranch(std::shared_ptr<Branch> br) {
    branches.push_back(br);
}

void Bank::registerCustomer(std::shared_ptr<Customer> cust) {
    customers.push_back(cust);
}

void Bank::hireEmployee(std::shared_ptr<Employee> emp) {
    employees.push_back(emp);
}

void Bank::displayBankDetails() const {
    std::cout << "Bank: " << bankName << " (ID: " << bankId << ")" << std::endl;
    std::cout << "Total Branches: " << branches.size() << " | Total Customers: " << customers.size() << std::endl;
}

// --- Loan.cpp ---
Loan::Loan(std::string id, std::string type, double amt, double rate, int years, Customer* cust)
    : loanId(id), loanType(type), loanAmount(amt), interestRate(rate), tenureYears(years), loanStatus("Pending"), customer(cust) {
    EMIAmount = calculateEMI();
}

double Loan::calculateEMI() {
    double r = interestRate / (12 * 100);
    int n = tenureYears * 12;
    return (loanAmount * r * pow(1 + r, n)) / (pow(1 + r, n) - 1);
}

void Loan::approveLoan() {
    loanStatus = "Approved";
    std::cout << "Loan " << loanId << " approved for " << customer->getName() << std::endl;
}

void Loan::rejectLoan() {
    loanStatus = "Rejected";
    std::cout << "Loan " << loanId << " rejected." << std::endl;
}

void Loan::displayLoanDetails() const {
    std::cout << "Loan ID: " << loanId << " | Type: " << loanType << " | Amount: " << loanAmount 
              << " | EMI: " << EMIAmount << " | Status: " << loanStatus << std::endl;
}

// --- ATMCard.cpp ---
ATMCard::ATMCard(std::string num, int cvv, std::string expiry, int pin, std::string type, std::shared_ptr<Account> acc)
    : cardNumber(num), CVV(cvv), expiryDate(expiry), PIN(pin), cardType(type), cardStatus("Active"), linkedAccount(acc) {}

bool ATMCard::validatePIN(int enteredPIN) {
    if (PIN != enteredPIN) {
        throw InvalidPINException();
    }
    return true;
}

void ATMCard::blockCard() {
    cardStatus = "Blocked";
}

void ATMCard::activateCard() {
    cardStatus = "Active";
}

void ATMCard::withdrawCash(double amount, int enteredPIN) {
    if (cardStatus != "Active") {
        throw AccountBlockedException();
    }
    if (validatePIN(enteredPIN)) {
        linkedAccount->withdraw(amount);
        std::cout << "Cash withdrawal of " << amount << " successful from ATM." << std::endl;
    }
}

// ==========================================
// Main Function
// ==========================================

void displayMenu() {
    cout << "\n==========================================" << endl;
    cout << "     SMARTBANK ENTERPRISE SYSTEM" << endl;
    cout << "==========================================" << endl;
    cout << "1. Create Customer" << endl;
    cout << "2. Open Account" << endl;
    cout << "3. Deposit Money" << endl;
    cout << "4. Withdraw Money" << endl;
    cout << "5. Transfer Funds" << endl;
    cout << "6. Apply for Loan" << endl;
    cout << "7. Generate ATM Card" << endl;
    cout << "8. View Transaction History" << endl;
    cout << "9. Send Notifications" << endl;
    cout << "10. View Bank Details" << endl;
    cout << "11. Exit" << endl;
    cout << "==========================================" << endl;
    cout << "Select an option: ";
}

int main() {
    srand(time(0));
    
    // Initialize Bank and Branch
    auto smartBank = make_shared<Bank>("B001", "SmartBank Enterprise");
    auto mainBranch = make_shared<Branch>("BR01", "Downtown Branch", "SBEN0001", "123 Wall St");
    smartBank->addBranch(mainBranch);

    int choice;
    while (true) {
        displayMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        try {
            if (choice == 1) {
                string id, name, phone, email;
                cout << "Enter Customer ID: "; cin >> id;
                cout << "Enter Full Name: "; cin.ignore(); getline(cin, name);
                cout << "Enter Phone: "; cin >> phone;
                cout << "Enter Email: "; cin >> email;
                auto cust = make_shared<Customer>(id, name, phone, email);
                smartBank->registerCustomer(cust);
                cout << "Customer Registered Successfully!" << endl;

            } else if (choice == 2) {
                if (smartBank->getCustomers().empty()) {
                    cout << "Register a customer first!" << endl;
                    continue;
                }
                cout << "Select Customer (Index 0-" << smartBank->getCustomers().size()-1 << "): ";
                int cIdx; cin >> cIdx;
                auto cust = smartBank->getCustomers()[cIdx];

                cout << "Select Account Type (1: Savings, 2: Current, 3: FixedDeposit): ";
                int type; cin >> type;
                string accType = (type == 1) ? "Savings" : (type == 2) ? "Current" : "FixedDeposit";
                
                string accNum = "ACC" + to_string(rand() % 10000);
                double initialBal; cout << "Enter Initial Balance: "; cin >> initialBal;

                shared_ptr<Account> acc;
                if (type == 2) {
                    string bName; cout << "Enter Business Name: "; cin >> bName;
                    acc = AccountFactory::createAccount(accType, accNum, initialBal, mainBranch.get(), cust.get(), bName);
                } else if (type == 3) {
                    int months; cout << "Enter Tenure (months): "; cin >> months;
                    acc = AccountFactory::createAccount(accType, accNum, initialBal, mainBranch.get(), cust.get(), "", months);
                } else {
                    acc = AccountFactory::createAccount(accType, accNum, initialBal, mainBranch.get(), cust.get());
                }

                cust->openAccount(acc);
                mainBranch->addAccount(acc);
                cout << "Account " << accNum << " opened successfully!" << endl;

            } else if (choice == 3) {
                string accNum; double amt;
                cout << "Enter Account Number: "; cin >> accNum;
                cout << "Enter Amount: "; cin >> amt;
                bool found = false;
                for (auto& c : smartBank->getCustomers()) {
                    for (auto& a : c->getAccounts()) {
                        if (a->getAccountNumber() == accNum) {
                            a->deposit(amt);
                            cout << "Deposit Successful!" << endl;
                            found = true;
                            break;
                        }
                    }
                    if (found) break;
                }
                if (!found) cout << "Account not found!" << endl;

            } else if (choice == 4) {
                string accNum; double amt;
                cout << "Enter Account Number: "; cin >> accNum;
                cout << "Enter Amount: "; cin >> amt;
                bool found = false;
                for (auto& c : smartBank->getCustomers()) {
                    for (auto& a : c->getAccounts()) {
                        if (a->getAccountNumber() == accNum) {
                            a->withdraw(amt);
                            cout << "Withdrawal Successful!" << endl;
                            found = true;
                            break;
                        }
                    }
                    if (found) break;
                }
                if (!found) cout << "Account not found!" << endl;

            } else if (choice == 5) {
                string fromAcc, toAcc; double amt;
                cout << "Enter Source Account: "; cin >> fromAcc;
                cout << "Enter Destination Account: "; cin >> toAcc;
                cout << "Enter Amount: "; cin >> amt;
                
                shared_ptr<Account> src, dest;
                for (auto& c : smartBank->getCustomers()) {
                    for (auto& a : c->getAccounts()) {
                        if (a->getAccountNumber() == fromAcc) src = a;
                        if (a->getAccountNumber() == toAcc) dest = a;
                    }
                }
                if (src && dest) {
                    src->transferFunds(dest, amt);
                } else {
                    cout << "One or both accounts not found!" << endl;
                }

            } else if (choice == 6) {
                if (smartBank->getCustomers().empty()) {
                    cout << "Register a customer first!" << endl;
                    continue;
                }
                int cIdx; cout << "Select Customer (Index): "; cin >> cIdx;
                auto cust = smartBank->getCustomers()[cIdx];
                
                double amt, rate; int years; string type;
                cout << "Enter Loan Type (Home/Car/Personal): "; cin >> type;
                cout << "Enter Amount: "; cin >> amt;
                cout << "Enter Rate (%): "; cin >> rate;
                cout << "Enter Tenure (Years): "; cin >> years;
                
                auto loan = make_shared<Loan>("L" + to_string(rand()%1000), type, amt, rate, years, cust.get());
                cust->applyLoan(loan);
                loan->approveLoan();
                loan->displayLoanDetails();

            } else if (choice == 7) {
                string accNum; int pin;
                cout << "Enter Account Number to link: "; cin >> accNum;
                cout << "Set 4-digit PIN: "; cin >> pin;
                
                shared_ptr<Account> target;
                for (auto& c : smartBank->getCustomers()) {
                    for (auto& a : c->getAccounts()) {
                        if (a->getAccountNumber() == accNum) target = a;
                    }
                }
                if (target) {
                    auto card = make_shared<ATMCard>("4532-" + to_string(rand()%9000+1000), 123, "12/30", pin, "Visa", target);
                    cout << "ATM Card Generated Successfully!" << endl;
                } else {
                    cout << "Account not found!" << endl;
                }

            } else if (choice == 8) {
                string accNum;
                cout << "Enter Account Number: "; cin >> accNum;
                for (auto& c : smartBank->getCustomers()) {
                    for (auto& a : c->getAccounts()) {
                        if (a->getAccountNumber() == accNum) {
                            a->printStatement();
                        }
                    }
                }

            } else if (choice == 9) {
                if (smartBank->getCustomers().empty()) {
                    cout << "Register a customer first!" << endl;
                    continue;
                }
                int cIdx; cout << "Select Customer (Index): "; cin >> cIdx;
                auto cust = smartBank->getCustomers()[cIdx];
                
                Notification* notifier;
                cout << "Select Type (1: SMS, 2: Email): ";
                int t; cin >> t;
                if (t == 1) notifier = new SMSNotification();
                else notifier = new EmailNotification();
                
                notifier->sendNotification(cust->getEmail(), "Welcome to SmartBank Enterprise System!");
                delete notifier;

            } else if (choice == 10) {
                smartBank->displayBankDetails();
                mainBranch->displayBranchDetails();

            } else if (choice == 11) {
                cout << "Exiting system. Goodbye!" << endl;
                break;
            } else {
                cout << "Invalid Option!" << endl;
            }
        } catch (const exception& e) {
            cout << "EXCEPTION: " << e.what() << endl;
        }
    }

    return 0;
}
