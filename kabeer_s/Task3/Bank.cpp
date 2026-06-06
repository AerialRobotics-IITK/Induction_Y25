#include "Bank.h"
#include "Employee.h"
#include "Customer.h"
#include "Branch.h"
#include "Loan.h"
#include "Account.h"
#include "Transaction.h"

// creator and destroyer of branches, customers, employees, loans, transactions
// make a destructor for bank

int Bank::idGenerator = 0;

Bank::Bank(std::string name){
    bankId = ++idGenerator;
    bankName = name;
}

Bank::~Bank(){
    while (!loans.empty()) removeLoan(loans.front()->loanId);
    while (!employees.empty()) removeEmployee(employees.front()->id);
    while (!customers.empty()) removeCustomer(customers.front()->customerId);
    while (!branches.empty()) removeBranch(branches.front()->branchId);

    for (Transaction *a : transactionHistory)
    {   
        delete a;
    }
    transactionHistory.clear();


}

int Bank::getId(){
    return bankId;
}
std::string Bank::getName(){
    return bankName;
}

int Bank::addEmployee(std::string fullName, std::string designation, double salary){
    Employee *emp = new Employee(fullName, designation, salary);
    employees.push_back(emp);
    return emp->id;
}

int Bank::removeEmployee(int id){
    for (Branch * branch : branches){
        branch->removeEmployee(id);
    }

    for (auto it = employees.begin(); it!=employees.end(); it++){
        Employee *emp = *it;
        if ((emp->getId()) == id){
            delete emp;
            employees.erase(it);
            return 0;
        }
    }

    return -1;
}

int Bank::addCustomer(std::string fullName, Date dob, std::string gender, std::string mobileNumber,std::string email, std::string address, std::string aadhaarNumber, std::string PANNumber)
{
    Customer *cus = new Customer(fullName, dob, gender, mobileNumber, email, address, aadhaarNumber, PANNumber);
    customers.push_back(cus);
    return cus->customerId;
}

int Bank::removeCustomer(int id){
    Customer *cus;
    for (Customer *temp : customers) if (temp->customerId == id) cus = temp;

    for (Account* acc : cus->accounts) {
        Branch* owner = acc->getBranch();
        owner->removeAccount(acc->getAccountNumber());
    }

    for (Loan* loan : cus->loans) {
        delete loan;
    }

    for (auto it = customers.begin(); it != customers.end(); it++){
        Customer *c = *it;
        if (c->customerId == id){
            delete c;
            customers.erase(it);
            return 0;
        }
    }
    return -1;
}

int Bank::addBranch(std::string name, std::string code, std::string address){
    Branch *b = new Branch(name, code, address);
    branches.push_back(b);
    return b->branchId;
}

int Bank::removeBranch(int id){
    for (auto it = branches.begin(); it != branches.end(); it++){
        Branch *b = *it;
        if (b->getId() == id){
            delete b;
            branches.erase(it);
            return 0;
        }
    }
    return -1;
}

int Bank::addLoan(std::string LoanType, double LoanAmount, double InterestRate, int TenureYears, double eMIAmount, Customer *cus, std::string LoanStatus){
    Loan *l = new Loan(LoanType, LoanAmount, InterestRate, TenureYears, eMIAmount, cus, LoanStatus);
    loans.push_back(l);

    cus->addLoan(l);
    return l->loanId;
}

int Bank::removeLoan(int id){
    for (auto it = loans.begin(); it != loans.end(); it++){
        Loan* l = *it;
            if ((l->getId())==id){
            Customer *cus = l->getCustomer();
            cus->removeLoan(l->getId());
            delete l;
            loans.erase(it);
            return 0;
        }
    }
    return -1;
}

int Bank::addTransaction(std::string transactionType, double amount, Date transactionDate, Account *senderAccount, 
    Account *receiverAccount, std::string status){

    Transaction *tran = new Transaction(transactionType, amount, transactionDate, senderAccount, receiverAccount, status);
    transactionHistory.push_back(tran);

    senderAccount->addTransaction(tran);
    receiverAccount->addTransaction(tran);

    return tran->transactionId;
}