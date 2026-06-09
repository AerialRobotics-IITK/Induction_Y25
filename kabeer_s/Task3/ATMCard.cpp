#include "ATMCard.h"
#include <cstdlib>

int ATMCard::numberGenerator = 0;

ATMCard::ATMCard(Date ExpiryDate, std::string type, std::string status, Account* acc){
    cardNumber = ++numberGenerator;
    PIN = rand() % 9000 + 1000;
    CVV = rand() % 900 + 100;
    expiryDate = ExpiryDate;
    cardType = type;
    cardStatus = status;
    linkedAccount = acc;
}

long ATMCard::getCardNumber(){
    return cardNumber;
}
std::string ATMCard::getCardType(){
    return cardType;
}
std::string ATMCard::getCardStatus(){
    return cardStatus;
}
Date ATMCard::getExpiryDate(){
    return expiryDate;
}
Account* ATMCard::getLinkedAccount(){
    return linkedAccount;
}

bool ATMCard::verifyPIN(int inputPIN){
    if (PIN == inputPIN) return 1;
    else return 0;
}

void ATMCard::blockCard(){
    cardStatus = "blocked";
}