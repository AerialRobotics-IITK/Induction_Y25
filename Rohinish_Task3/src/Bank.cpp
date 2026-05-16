#include "Bank.h"
#include "Exceptions.h"

int Bank::nextBankId_ = 1;

Bank::Bank(const std::string& name)
    : bankId_(nextBankId_++), bankName_(name) {}

Bank::~Bank() {
    for (auto* p : atmCards_)     delete p;
    for (auto* p : loans_)       delete p;
    for (auto* p : transactions_) delete p;
    for (auto* p : accounts_)    delete p;
    for (auto* p : employees_)   delete p;
    for (auto* p : customers_)   delete p;
    for (auto* p : branches_)    delete p;
}

Branch* Bank::addBranch(const std::string& name, const std::string& ifsc,
                        const std::string& address) {
    auto* branch = new Branch(name, ifsc, address);
    branches_.push_back(branch);
    return branch;
}

Customer* Bank::addCustomer(const std::string& fullName, const std::string& dob,
                            const std::string& gender, const std::string& mobile,
                            const std::string& email, const std::string& address,
                            const std::string& aadhaar, const std::string& pan) {
    auto* customer = new Customer(fullName, dob, gender, mobile, email, address, aadhaar, pan);
    customers_.push_back(customer);
    return customer;
}

Employee* Bank::addEmployee(const std::string& name, const std::string& designation,
                            double salary, Branch* branch) {
    auto* emp = new Employee(name, designation, salary, branch);
    employees_.push_back(emp);
    if (branch) branch->addEmployee(emp);
    return emp;
}

Account* Bank::openAccount(const std::string& type, double initialBalance,
                           const std::string& dateOpened, Branch* branch, Customer* customer) {
    Account* account = AccountFactory::createAccount(type, initialBalance, dateOpened,
                                                     branch, customer);
    accounts_.push_back(account);
    if (branch) branch->addAccount(account);
    if (customer) customer->addAccount(account);
    return account;
}

Transaction* Bank::performDeposit(Account* account, double amount, const std::string& date) {
    account->deposit(amount);
    auto* txn = new Transaction("Deposit", amount, date, nullptr, account);
    transactions_.push_back(txn);
    account->addTransaction(txn);
    return txn;
}

Transaction* Bank::performWithdrawal(Account* account, double amount, const std::string& date) {
    account->withdraw(amount);
    auto* txn = new Transaction("Withdrawal", amount, date, account, nullptr);
    transactions_.push_back(txn);
    account->addTransaction(txn);
    return txn;
}

Transaction* Bank::performTransfer(Account* sender, Account* receiver,
                                   double amount, const std::string& date) {
    sender->withdraw(amount);
    receiver->deposit(amount);
    auto* txn = new Transaction("Transfer", amount, date, sender, receiver);
    transactions_.push_back(txn);
    sender->addTransaction(txn);
    receiver->addTransaction(txn);
    return txn;
}

Loan* Bank::applyForLoan(const std::string& type, double amount, double interestRate,
                         int tenureYears, Customer* customer) {
    auto* loan = new Loan(type, amount, interestRate, tenureYears, customer);
    loans_.push_back(loan);
    if (customer) customer->addLoan(loan);
    return loan;
}

ATMCard* Bank::issueATMCard(const std::string& expiryDate, int pin,
                            const std::string& cardType, Account* linkedAccount) {
    auto* card = new ATMCard(expiryDate, pin, cardType, linkedAccount);
    atmCards_.push_back(card);
    return card;
}

void Bank::displayBankInfo() const {
    std::cout << "========================================\n"
              << "  Bank       : " << bankName_ << " (ID: " << bankId_ << ")\n"
              << "  Branches   : " << branches_.size() << "\n"
              << "  Customers  : " << customers_.size() << "\n"
              << "  Employees  : " << employees_.size() << "\n"
              << "  Accounts   : " << accounts_.size() << "\n"
              << "  Loans      : " << loans_.size() << "\n"
              << "  ATM Cards  : " << atmCards_.size() << "\n"
              << "========================================\n";
}
