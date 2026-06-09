#ifndef ATMCARD_H
#define ATMCARD_H

#include "Date.h"
#include <string>

class Account;
class Branch;

class ATMCard{
    friend class Branch;
    private:
    long cardNumber;
    int CVV;
    Date expiryDate;
    int PIN;
    std::string cardType;
    std::string cardStatus;
    Account *linkedAccount;

    static int numberGenerator;

    // managed by branch which owns the account
    ATMCard(Date ExpiryDate, std::string type, std::string status, Account* acc);
    ~ATMCard(){;}

    public:
    long getCardNumber();
    std::string getCardType();
    std::string getCardStatus();
    Date getExpiryDate();
    Account* getLinkedAccount();
    
    bool verifyPIN(int inputPIN);
    void blockCard();
};

#endif