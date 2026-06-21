#include "Account.h"
#include "ATMCard.h"
#include "Transaction.h"

long Account::numberGenerator = 0;

Account::Account(std::string Type, double Balance, Date DateOpened, std::string Status, Branch *Branche, Customer * Cus){
    accountNumber = ++numberGenerator;
    accountType = Type;
    balance = Balance;
    dateOpened = DateOpened;
    status = Status;
    branch = Branche;
    customer = Cus;
}

Account::~Account(){
    ;
}



void Account::setStatus(std::string Status){
    status = Status;
}

void Account::setBalance(double b){
    balance = b;
}

void Account::addCard(ATMCard * card){
    ATMcards.push_back(card);
}

int Account::removeCard(long Number){
    for (auto it = ATMcards.begin(); it != ATMcards.end(); it++){
        ATMCard* card = *it;
        if ((card->getCardNumber()) == Number){
            ATMcards.erase(it);
            return 0;
        }
    }
    return -1;
}

void Account::addTransaction(Transaction * t){
    transactionHistory.push_back(t);
}

long Account::getAccountNumber(){
    return accountNumber;
}
std::string Account::getAccountType(){
    return accountType;
}
double Account::getBalance(){
    return balance;
}
Date Account::getDateOpened(){
    return dateOpened;
}
std::string Account::getStatus(){
    return status;
}
Branch* Account::getBranch(){
    return branch;
}
Customer* Account::getCustomer(){
    return customer;
}
std::list<Transaction *> Account::getTransactionHistory(){
    return transactionHistory;
}