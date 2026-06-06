#ifndef BRANCH_H
#define BRANCH_H

#include <string>
#include <list>
#include "Bank.h"

class Employee;
class Account;

class Branch{
    friend Bank;
    private:
    int branchId;
    std::string branchName;
    std::string IFSCCode;
    std::string address;
    std::list<Account *> accounts;
    std::list<Employee *> employees;

    static int idGenerator;

    Branch(std::string name, std::string code, std::string address);
    ~Branch();

    public:
    int getId();
    std::string getName();
    std::string getIFSCCode();
    std::string getAddress();

    void addEmployee(Employee* emp);
    int removeEmployee(int id); // returns success code
    
    int addAccount(std::string Type, double Balance, Date DateOpened, std::string Status, Branch *Branche, Customer * Cus);
    int removeAccount(long AccNum);
    int setAccountStatus(long AccNum, std::string status);
    int setAccountBalance(long AccNum, int bal);
    int addCard(Date ExpiryDate, std::string type, std::string status, Account* acc);
    int removeCard(long CardNum);
};

#endif