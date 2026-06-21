#include "Branch.h"
#include "Employee.h"
#include "Account.h"
#include "SavingsAccount.h"
#include "CurrentAccount.h"
#include "FixedDepositAccount.h"
#include <iostream>
#include "Customer.h"
#include "ATMCard.h"

// creator and destroyer of cards and accounts

int Branch::idGenerator=0;

Branch::Branch(std::string name, std::string code, std::string Address){
    branchId = ++idGenerator;
    branchName = name;
    IFSCCode = code;
    address = Address;
}

void Branch::addEmployee(Employee* emp){
    employees.push_back(emp);
}
int Branch::removeEmployee(int id){
    for (auto it = employees.begin(); it != employees.end(); it++){
        Employee *emp = *it;
        if ((emp->getId()) == id){
            employees.erase(it);
            return 0;
        }
    }
    return -1;
}

int Branch::getId(){
    return branchId;
}
std::string Branch::getName(){
    return branchName;
}
std::string Branch::getIFSCCode(){
    return IFSCCode;
}
std::string Branch::getAddress(){
    return address;
}

int Branch::addAccount(std::string Type, double Balance, Date DateOpened, std::string Status, Branch *Branche, Customer * Cus){
    Account *a;
    if (Type == "Savings")
    {
        double Bal;
        std::cout << "Enter Bal; ";
        std::cin >> Bal;
        double Rate;
        std::cout << "Enter Rate; ";
        std::cin >> Rate;
        a = new SavingsAccount(Type, Balance, DateOpened, Status, Branche, Cus, Bal, Rate);
    } else if (Type == "Current")
    {
        double limit;
        std::string name;

        std::cout << "Enter limit; ";
        std::cin >> limit;
        std::cout << "Enter business name; ";
        std::cin >> name;
        a = new CurrentAccount(Type, Balance, DateOpened, Status, Branche, Cus, limit, name);
    } else if (Type == "Fixed Deposit")
    {
        double fdamount;
        Date maturity;
        double interest;
        int tenure;

        std::cout << "Enter amount: ";
        std::cin >> fdamount;
        // initialise maturity somehow
        std::cout << "Enter interest: ";
        std::cin >> interest;
        std::cout << "Enter tenure: ";
        std::cin >> tenure;
        
        a = new FixedDepositAccount(Type, Balance, DateOpened, Status, Branche, Cus, fdamount, maturity, interest, tenure);
    }
    accounts.push_back(a);
    Cus->addAccount(a);

    return 0;
}

int Branch::addCard(Date ExpiryDate, std::string type, std::string status, Account* acc){
    for (Account * a : accounts)
    {
        if (a == acc){
            ATMCard* card = new ATMCard(ExpiryDate, type, status, acc);
            (a->ATMcards).push_back(card);
            return 0;
        }
    }
    return -1;
}

int Branch::removeCard(long CardNum){
    for (Account * a : accounts) a->removeCard(CardNum);
    return 0;
}

int Branch::removeAccount(long AccNum){
    for (auto it = accounts.begin(); it != accounts.end(); it++)
    {
        Account *a = *it;
        if (a->accountNumber == AccNum){
            for (ATMCard * c : a->ATMcards)
            {
                delete c;
            }
            (a->ATMcards).clear();
            
            delete a;
            accounts.erase(it);
            return 0;
        }
    }
    return -1;
}

int Branch::setAccountBalance(long AccNum, int bal){
    for (Account *a : accounts)
    {
        if (a->accountNumber == AccNum){
            a->balance = bal;
        }
    }

    return 0;
}

int Branch::setAccountStatus(long AccNum, std::string status){
    for (Account *a : accounts)
    {
        if (a->accountNumber == AccNum){
            a->status = status;
        }
    }

    return 0;
}

Branch::~Branch(){
    for (Account *a : accounts)
    {
        for (ATMCard * c : a->ATMcards)
        {
            delete c;
        }
        (a->ATMcards).clear();
        
        delete a;
    }
    accounts.clear();
}