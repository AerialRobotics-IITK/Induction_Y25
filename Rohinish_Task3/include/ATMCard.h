#pragma once
#include <string>
#include <iostream>

class Account;

class ATMCard {
    long cardNumber_;
    int cvv_;
    std::string expiryDate_;
    int pin_;
    std::string cardType_;
    std::string cardStatus_;
    Account* linkedAccount_;

    static long nextCardNumber_;
public:
    ATMCard(const std::string& expiryDate, int pin, const std::string& cardType,
            Account* linkedAccount);

    long getCardNumber() const { return cardNumber_; }
    int getCVV() const { return cvv_; }
    std::string getExpiryDate() const { return expiryDate_; }
    std::string getCardType() const { return cardType_; }
    std::string getCardStatus() const { return cardStatus_; }
    Account* getLinkedAccount() const { return linkedAccount_; }

    void validatePIN(int enteredPIN) const;
    void blockCard();
    void activateCard();
    void changePIN(int oldPIN, int newPIN);
    void displayCardInfo() const;
};
