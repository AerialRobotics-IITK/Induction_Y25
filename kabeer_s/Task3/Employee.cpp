#include "Employee.h"

int Employee::idGenerator = 0;

Employee::Employee(std::string Name, std::string Designation, double Salary){
    name = Name;
    designation = Designation;
    salary = Salary;
    id = ++idGenerator;
}

int Employee::getId(){
    return id;
}

std::string Employee::getName(){
    return name;
}

std::string Employee::getDesignation(){
    return designation;
}

double Employee::getSalary(){
    return salary;
}