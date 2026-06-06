#include "Customer.h"
#include "Account.h"
#include "Loan.h"

int Customer::idGenerator = 0;

Customer::Customer(std::string FullName, Date DOB, std::string Gender, std::string MobileNumber, std::string EMail,  std::string Address,  std::string AadhaarNumber, std::string panNumber)
{
    customerId = ++idGenerator;
    fullName = FullName;
    dob = DOB;
    gender = Gender;
    mobileNumber = MobileNumber;
    email = EMail;
    address = Address;
    aadhaarNumber = AadhaarNumber;
    PANNumber = panNumber;
}

void Customer::addAccount(Account * acc){
    accounts.push_back(acc);
}

int Customer::removeAccount(long accountNumber){
    for (auto it = accounts.begin(); it != accounts.end(); it++){
        Account *acc = *it;
        if ((acc->getAccountNumber()) == accountNumber) {
            accounts.erase(it);
            return 0;
        }
    }
    return -1;
}

int Customer::addLoan(Loan * l){
    loans.push_back(l);
    return l->getId();
}

int Customer::removeLoan(int id){
    for (auto it = loans.begin(); it != loans.end(); it++){
        Loan *l = *it;
        if ((l->getId()) == id) {
            loans.erase(it);
            return 0;
        }
    }
    return -1;
}

std::string Customer::getName(){
    return fullName;
}
Date Customer::getDob(){
    return dob;
}
std::string Customer::getGender(){
    return gender;
}
std::string Customer::getMobileNumber(){
    return mobileNumber;
}
std::string Customer::getEmail(){
    return email;
}
std::string Customer::getAddress(){
    return address;
}
std::string Customer::getAadhaarNumber(){
    return aadhaarNumber;
}
std::string Customer::getPANNumber(){
    return PANNumber;
}