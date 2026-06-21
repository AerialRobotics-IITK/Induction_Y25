#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <memory>
#include <ctime>
#include <cmath>


class Branch;
class Customer;
class Transaction;



class InsufficientBalanceException : public std::runtime_error {
public:
    InsufficientBalanceException(const std::string& msg = "Transaction Failed: Insufficient balance.")
        : std::runtime_error(msg) {}
};

class InvalidPINException : public std::runtime_error {
public:
    InvalidPINException(const std::string& msg = "Security Alert: Invalid PIN entered.")
        : std::runtime_error(msg) {}
};

class AccountBlockedException : public std::runtime_error {
public:
    AccountBlockedException(const std::string& msg = "Access Denied: Account is currently blocked.")
        : std::runtime_error(msg) {}
};

class LoanRejectedException : public std::runtime_error {
public:
    LoanRejectedException(const std::string& msg = "Loan Application Rejected: Criteria not met.")
        : std::runtime_error(msg) {}
};


class Notification {
public:
    virtual ~Notification() = default;
    virtual void send(const std::string& message) = 0;
};

class SMSNotification : public Notification {
private:
    std::string phoneNumber;
    std::string deliveryStatus;
public:
    SMSNotification(const std::string& phone)
        : phoneNumber(phone), deliveryStatus("Pending") {}

    void send(const std::string& message) override {
        std::cout << "[SMS to " << phoneNumber << "]: " << message << "\n";
        deliveryStatus = "Sent";
    }
};

class EmailNotification : public Notification {
private:
    std::string emailAddress;
    std::string subject;
    std::string deliveryStatus;
public:
    EmailNotification(const std::string& email, const std::string& sub)
        : emailAddress(email), subject(sub), deliveryStatus("Pending") {}

    void send(const std::string& message) override {
        std::cout << "[Email to " << emailAddress << " | Sub: " << subject << "]: " << message << "\n";
        deliveryStatus = "Sent";
    }
};



class Transaction {
private:
    int transactionId;
    std::string transactionType;
    double amount;
    std::string transactionDate;
    std::string status;

   
    std::weak_ptr<class Account> senderAccount;
    std::weak_ptr<class Account> receiverAccount;

public:
    Transaction(int id, std::string type, double amt,
                std::shared_ptr<class Account> sender = nullptr,
                std::shared_ptr<class Account> receiver = nullptr);

    void execute();

    std::string getStatus() const { return status; }
    int getId()            const { return transactionId; }
    std::string getType()  const { return transactionType; }
    double getAmount()     const { return amount; }
    std::string getDate()  const { return transactionDate; }
};


class Account {
protected:
    long long accountNumber;         
    std::string accountType;
    double balance;
    std::string dateOpened;
    std::string status;              
    std::weak_ptr<Branch>   linkedBranch;
    std::weak_ptr<Customer> owner;
    std::vector<std::shared_ptr<Transaction>> transactions; 

public:
    Account(long long accNum, std::string type, double bal)
        : accountNumber(accNum), accountType(type), balance(bal), status("Active") {
        time_t now = time(nullptr);
        dateOpened = std::string(ctime(&now));
        dateOpened.pop_back();
    }

    virtual ~Account() = default;

    void setLinkedBranch(std::shared_ptr<Branch> b)   { linkedBranch = b; }
    void setOwner(std::shared_ptr<Customer> c)        { owner = c; }

    void recordTransaction(std::shared_ptr<Transaction> t) {
        transactions.push_back(t);
    }

    void printTransactionHistory() const {
        std::cout << "  Transaction history for account " << accountNumber << ":\n";
        if (transactions.empty()) {
            std::cout << "    (none)\n";
            return;
        }
        for (const auto& t : transactions) {
            std::cout << "    [" << t->getId() << "] "
                      << t->getType() << " | Rs." << t->getAmount()
                      << " | " << t->getStatus()
                      << " | " << t->getDate() << "\n";
        }
    }

    virtual void deposit(double amount) {
        if (status == "Blocked") throw AccountBlockedException();
        balance += amount;
        std::cout << "Deposited Rs." << amount << " to account "
                  << accountNumber << ". New Balance: Rs." << balance << "\n";
    }

    virtual void withdraw(double amount) = 0;

    long long   getAccountNumber() const { return accountNumber; }
    double      getBalance()       const { return balance; }
    std::string getStatus()        const { return status; }
    std::string getType()          const { return accountType; }

    void blockAccount() { status = "Blocked"; }
};


class SavingsAccount : public Account {
private:
    double interestRate;
    double minimumBalance;
public:
    SavingsAccount(long long accNum, double bal,
                   double intRate = 4.0, double minBal = 1000.0)
        : Account(accNum, "Savings", bal),
          interestRate(intRate), minimumBalance(minBal) {}

    void withdraw(double amount) override {
        if (status == "Blocked") throw AccountBlockedException();
        if (balance - amount < minimumBalance)
            throw InsufficientBalanceException(
                "Cannot withdraw: Minimum balance limit reached.");
        balance -= amount;
        std::cout << "Withdrew Rs." << amount << " from Savings "
                  << accountNumber << ". New Balance: Rs." << balance << "\n";
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;
    std::string businessName;
public:
    CurrentAccount(long long accNum, double bal,
                   std::string bizName, double odLimit = 50000.0)
        : Account(accNum, "Current", bal),
          overdraftLimit(odLimit), businessName(bizName) {}

    void withdraw(double amount) override {
        if (status == "Blocked") throw AccountBlockedException();
        if (balance + overdraftLimit < amount)
            throw InsufficientBalanceException(
                "Cannot withdraw: Overdraft limit exceeded.");
        balance -= amount;
        std::cout << "Withdrew Rs." << amount << " from Current "
                  << accountNumber << ". New Balance: Rs." << balance << "\n";
    }
};

class FixedDepositAccount : public Account {
private:
    double FDAmount;
    std::string maturityDate;
    double FDInterestRate;
    int tenureMonths;
public:
    FixedDepositAccount(long long accNum, double amount,
                        double rate, int months)
        : Account(accNum, "FixedDeposit", amount),
          FDAmount(amount), FDInterestRate(rate), tenureMonths(months) {
        status = "Locked";
    }

    void deposit(double amount) override {
        throw AccountBlockedException(
            "Cannot deposit into an active Fixed Deposit.");
    }

    void withdraw(double amount) override {
        throw AccountBlockedException(
            "Cannot withdraw from active Fixed Deposit before maturity.");
    }
};



Transaction::Transaction(int id, std::string type, double amt,
                         std::shared_ptr<Account> sender,
                         std::shared_ptr<Account> receiver)
    : transactionId(id), transactionType(type), amount(amt),
      senderAccount(sender), receiverAccount(receiver), status("Pending") {
    time_t now = time(nullptr);
    transactionDate = std::string(ctime(&now));
    transactionDate.pop_back();
}

void Transaction::execute() {
    try {
        auto sender   = senderAccount.lock();
        auto receiver = receiverAccount.lock();

        if (transactionType == "Transfer" && sender && receiver) {
            sender->withdraw(amount);
            receiver->deposit(amount);
        } else if (transactionType == "Deposit" && receiver) {
            receiver->deposit(amount);
        } else if (transactionType == "Withdrawal" && sender) {
            sender->withdraw(amount);
        }
        status = "Success";
    } catch (const std::exception&) {
        status = "Failed";
        throw;
    }
}


class AccountFactory {
public:
    static std::shared_ptr<Account> createAccount(
            const std::string& type, long long accNum,
            double initialBal, const std::string& extraArg = "") {
        if (type == "Savings")
            return std::make_shared<SavingsAccount>(accNum, initialBal);
        if (type == "Current")
            return std::make_shared<CurrentAccount>(accNum, initialBal, extraArg);
        if (type == "FixedDeposit")
            return std::make_shared<FixedDepositAccount>(accNum, initialBal, 6.5, 12);
        return nullptr;
    }
};



class ATMCard {
private:
    long long cardNumber;      
    int CVV;
    std::string expiryDate;
    int PIN;
    std::string cardType;
    std::string cardStatus;
    std::shared_ptr<Account> linkedAccount;
public:
    ATMCard(long long num, int cvv, int pin,
            std::string type, std::shared_ptr<Account> acc)
        : cardNumber(num), CVV(cvv), PIN(pin),
          cardType(type), linkedAccount(acc), cardStatus("Active") {}

    void verifyAndWithdraw(int inputPin, double amount) {
        if (cardStatus == "Blocked")
            throw AccountBlockedException("ATM Card is blocked.");
        if (inputPin != PIN)
            throw InvalidPINException();
        linkedAccount->withdraw(amount);
    }

    void blockCard() { cardStatus = "Blocked"; }
};


class Loan {
private:
    int loanId;
    std::string loanType;
    double loanAmount;
    double interestRate;
    int tenureYears;
    double EMIAmount;
    std::string loanStatus;
    std::weak_ptr<Customer> customer;   
public:
    Loan(int id, std::string type, double amt, double rate, int years)
        : loanId(id), loanType(type), loanAmount(amt),
          interestRate(rate), tenureYears(years), loanStatus("Pending") {
        calculateEMI();
    }

    void setCustomer(std::shared_ptr<Customer> c) { customer = c; }

    void calculateEMI() {
        double r = interestRate / (12.0 * 100.0);
        int n = tenureYears * 12;
        EMIAmount = (loanAmount * r * pow(1 + r, n)) / (pow(1 + r, n) - 1);
    }

    void approveLoan() {
        if (loanAmount > 5000000)
            throw LoanRejectedException("Amount exceeds maximum threshold.");
        loanStatus = "Approved";
        std::cout << "Loan " << loanId << " (" << loanType
                  << ") Approved. Monthly EMI: Rs." << EMIAmount << "\n";
    }

    std::string getStatus() const { return loanStatus; }
};


class Employee {
private:
    int employeeId;
    std::string employeeName;
    std::string designation;
    double salary;
    std::weak_ptr<Branch> assignedBranch;  
public:
    Employee(int id, std::string name, std::string desig, double sal)
        : employeeId(id), employeeName(name), designation(desig), salary(sal) {}

    void setAssignedBranch(std::shared_ptr<Branch> b) { assignedBranch = b; }

    void printInfo() const {
        std::cout << "  Employee [" << employeeId << "] "
                  << employeeName << " | " << designation
                  << " | Salary: Rs." << salary << "\n";
    }
};



class Customer : public std::enable_shared_from_this<Customer> {
private:
    int customerId;
    std::string fullName;
    std::string dob;            
    std::string gender;         
    std::string mobileNumber;
    std::string email;
    std::string address;        
    std::string aadhaarNumber;  
    std::string PANNumber;      
    std::vector<std::shared_ptr<Account>> accounts;
    std::vector<std::shared_ptr<Loan>>    loans;
    std::vector<std::shared_ptr<Notification>> notificationStrategies;

public:
    Customer(int id, std::string name, std::string mobile, std::string mail,
             std::string dateOfBirth = "", std::string gen = "",
             std::string addr = "", std::string aadhaar = "", std::string pan = "")
        : customerId(id), fullName(name), mobileNumber(mobile), email(mail),
          dob(dateOfBirth), gender(gen), address(addr),
          aadhaarNumber(aadhaar), PANNumber(pan) {}

    void addAccount(std::shared_ptr<Account> acc) {
        acc->setOwner(shared_from_this());  
        accounts.push_back(acc);
    }

    void addLoan(std::shared_ptr<Loan> loan) {
        loan->setCustomer(shared_from_this()); 
        loans.push_back(loan);
    }

    void addNotificationStrategy(std::shared_ptr<Notification> strategy) {
        notificationStrategies.push_back(strategy);
    }

    void notify(const std::string& msg) {
        for (auto& strategy : notificationStrategies)
            strategy->send(msg);
    }

    void printInfo() const {
        std::cout << "Customer [" << customerId << "] " << fullName
                  << " | Mobile: " << mobileNumber
                  << " | Email: "  << email << "\n";
        std::cout << "  Accounts: " << accounts.size()
                  << " | Loans: "   << loans.size() << "\n";
    }
};



class Branch : public std::enable_shared_from_this<Branch> {
private:
    int branchId;
    std::string branchName;
    std::string IFSCCode;
    std::string address;        // FIX: added per spec
    std::vector<std::shared_ptr<Account>>  handledAccounts;
    std::vector<std::shared_ptr<Employee>> staffMembers;
public:
    Branch(int id, std::string name, std::string ifsc, std::string addr = "")
        : branchId(id), branchName(name), IFSCCode(ifsc), address(addr) {}

    void addEmployee(std::shared_ptr<Employee> emp) {
        emp->setAssignedBranch(shared_from_this()); 
        staffMembers.push_back(emp);
    }

    void registerAccount(std::shared_ptr<Account> acc) {
        acc->setLinkedBranch(shared_from_this());   
        handledAccounts.push_back(acc);
    }

    void printInfo() const {
        std::cout << "Branch [" << branchId << "] " << branchName
                  << " | IFSC: " << IFSCCode << "\n";
        std::cout << "  Employees: " << staffMembers.size()
                  << " | Accounts: " << handledAccounts.size() << "\n";
    }
};


class Bank {
private:
    int bankId;
    std::string bankName;
    std::vector<std::shared_ptr<Branch>>   branches;
    std::vector<std::shared_ptr<Customer>> customers;
    std::vector<std::shared_ptr<Employee>> employees; 
public:
    Bank(int id, std::string name) : bankId(id), bankName(name) {}

    void addBranch(std::shared_ptr<Branch> branch)     { branches.push_back(branch); }
    void addCustomer(std::shared_ptr<Customer> cust)   { customers.push_back(cust); }
    void addEmployee(std::shared_ptr<Employee> emp)    { employees.push_back(emp); }

    void printSummary() const {
        std::cout << "\n=== Bank: " << bankName << " ===\n";
        std::cout << "Branches: "  << branches.size()
                  << " | Customers: " << customers.size()
                  << " | Employees: " << employees.size() << "\n";
    }
};


int main() {
    std::cout << "========================================\n";
    std::cout << "   Global Finance Bank — System Boot\n";
    std::cout << "========================================\n\n";


    Bank mainBank(1, "Global Finance Bank");
    auto mainBranch = std::make_shared<Branch>(
        101, "Downtown Branch", "GFB000101", "123 Main Street, New York");
    mainBank.addBranch(mainBranch);

    auto manager  = std::make_shared<Employee>(501, "Alice Smith", "Branch Manager", 85000.0);
    auto cashier  = std::make_shared<Employee>(502, "Bob Jones",   "Cashier",        45000.0);
    mainBranch->addEmployee(manager);
    mainBranch->addEmployee(cashier);
    mainBank.addEmployee(manager);   
    mainBank.addEmployee(cashier);
    manager->printInfo();
    cashier->printInfo();

    auto customer1 = std::make_shared<Customer>(
        1001, "John Doe", "+1-555-0199", "john.doe@email.com",
        "1990-05-15", "Male", "456 Oak Avenue, NY",
        "1234-5678-9012", "ABCDE1234F");
    mainBank.addCustomer(customer1);

    customer1->addNotificationStrategy(
        std::make_shared<SMSNotification>("+1-555-0199"));
    customer1->addNotificationStrategy(
        std::make_shared<EmailNotification>("john.doe@email.com", "Account Alert"));

    auto savingsAcc = AccountFactory::createAccount("Savings",      9988776655LL, 5000.0);
    auto currentAcc = AccountFactory::createAccount("Current",      1122334455LL, 10000.0, "Doe Enterprises");
    auto fdAcc      = AccountFactory::createAccount("FixedDeposit", 5566778899LL, 50000.0);

    customer1->addAccount(savingsAcc);  
    customer1->addAccount(currentAcc);
    customer1->addAccount(fdAcc);
    mainBranch->registerAccount(savingsAcc);  
    mainBranch->registerAccount(currentAcc);
    mainBranch->registerAccount(fdAcc);

    customer1->printInfo();
    mainBranch->printInfo();

    ATMCard myCard(4111222233334444LL, 123, 4040, "Debit", savingsAcc);

    std::cout << "\n--- Test 1: Valid ATM Withdrawal ---\n";
    try {
        myCard.verifyAndWithdraw(4040, 1000.0);
        auto t = std::make_shared<Transaction>(1, "Withdrawal", 1000.0, savingsAcc);
        savingsAcc->recordTransaction(t);   
        customer1->notify("Rs.1000 withdrawn via ATM.");
    } catch (const std::exception& e) {
        std::cerr << "Caught: " << e.what() << "\n";
    }

    std::cout << "\n--- Test 2: Invalid PIN ---\n";
    try {
        myCard.verifyAndWithdraw(9999, 500.0);
    } catch (const InvalidPINException& e) {
        std::cerr << "Caught InvalidPINException: " << e.what() << "\n";
    }

    std::cout << "\n--- Test 3: Minimum Balance Breach ---\n";
    try {
        savingsAcc->withdraw(3500.0);   
    } catch (const InsufficientBalanceException& e) {
        std::cerr << "Caught InsufficientBalanceException: " << e.what() << "\n";
    }

    std::cout << "\n--- Test 4: Blocked Account ---\n";
    savingsAcc->blockAccount();
    try {
        savingsAcc->deposit(100.0);
    } catch (const AccountBlockedException& e) {
        std::cerr << "Caught AccountBlockedException: " << e.what() << "\n";
    }
    
    std::cout << "\n--- Test 5: Inter-Account Transfer ---\n";
    auto t1 = std::make_shared<Transaction>(2, "Transfer", 2000.0, currentAcc, currentAcc);
    
    auto savingsAcc2 = AccountFactory::createAccount("Savings", 1111111111LL, 3000.0);
    auto t2 = std::make_shared<Transaction>(3, "Transfer", 2000.0, currentAcc, savingsAcc2);
    try {
        t2->execute();
        currentAcc->recordTransaction(t2);
        savingsAcc2->recordTransaction(t2);
        customer1->notify("Transferred Rs.2000 from Current to Savings.");
    } catch (const std::exception& e) {
        std::cerr << "Transfer failed: " << e.what() << "\n";
    }

    std::cout << "\n--- Test 6: FD Early Withdrawal ---\n";
    try {
        fdAcc->withdraw(1000.0);
    } catch (const AccountBlockedException& e) {
        std::cerr << "Caught AccountBlockedException (FD): " << e.what() << "\n";
    }

    std::cout << "\n--- Test 7: Loan Approval ---\n";
    auto carLoan  = std::make_shared<Loan>(7001, "Car",      25000.0,   8.5, 5);
    auto homeLoan = std::make_shared<Loan>(7002, "Home",     6000000.0, 7.5, 20); 
    auto persLoan = std::make_shared<Loan>(7003, "Personal", 100000.0,  12.0, 3);

    customer1->addLoan(carLoan);
    customer1->addLoan(homeLoan);
    customer1->addLoan(persLoan);

    try { carLoan->approveLoan(); }
    catch (const LoanRejectedException& e) {
        std::cerr << "Loan rejected: " << e.what() << "\n";
    }

    try { homeLoan->approveLoan(); }   
    catch (const LoanRejectedException& e) {
        std::cerr << "Caught LoanRejectedException: " << e.what() << "\n";
    }

    try { persLoan->approveLoan(); }
    catch (const LoanRejectedException& e) {
        std::cerr << "Loan rejected: " << e.what() << "\n";
    }

    std::cout << "\n--- Transaction History ---\n";
    savingsAcc->printTransactionHistory();
    currentAcc->printTransactionHistory();

    mainBank.printSummary();

    std::cout << "\n========================================\n";
    std::cout << "   System Demo Complete\n";
    std::cout << "========================================\n";

    return 0;
}
