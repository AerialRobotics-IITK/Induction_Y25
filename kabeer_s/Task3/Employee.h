#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include "Bank.h"

class Employee{
    private:
    int id;
    std::string name;
    std::string designation;
    double salary;

    static int idGenerator;

    // operated by bank only
    Employee(
        std::string fullName,
        std::string designation,
        double salary
    );
    ~Employee(){;}

    friend int Bank::addEmployee(std::string fullName, std::string designation, double salary);
    friend int Bank::removeEmployee(int id);
    friend Bank::~Bank();

    public:
    int getId();
    std::string getName();
    std::string getDesignation();
    double getSalary();
};

#endif