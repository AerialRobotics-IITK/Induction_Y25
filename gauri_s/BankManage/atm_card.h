#pragma once
#include <string>
#include <ctime>
#include <iostream>
#include "exceptions.h"

class Account;

class ATMCard {
private:
    static long numCounter;
    int PIN;          // stored internally; not displayed
public:
    long cardNumber;
    int CVV;
    std::time_t expiryDate;
    std::string cardType;     // "Debit" | "Credit"
    std::string cardStatus;   // "Active" | "Blocked"
    Account*    linkedAccount;

    ATMCard(int pin, int cvv, Account* account, const std::string& type = "Debit", int validityYears = 5)
        : PIN(pin),
          cardNumber(4000000000000000L + (++numCounter)),
          CVV(cvv),
          cardType(type),
          cardStatus("Active"),
          linkedAccount(account) {
        expiryDate = std::time(nullptr) + (long long)validityYears * 365 * 24 * 3600;
    }

    bool verifyPIN(int enteredPIN) const {
        if (cardStatus == "Blocked")
            throw AccountBlockedException("Card is blocked.");
        if (enteredPIN != PIN)
            throw InvalidPINException();
        return true;
    }

    void changePin(int oldPIN, int newPIN) {
        verifyPIN(oldPIN);
        PIN = newPIN;
        std::cout << "  PIN changed successfully.\n";
    }

    void block()    { cardStatus = "Blocked"; std::cout << "  Card blocked.\n"; }
    void unblock()  { cardStatus = "Active";  std::cout << "  Card unblocked.\n"; }

    bool isExpired() const {
        return std::time(nullptr) > expiryDate;
    }

    void display() const {
        std::cout << "  Card #" << cardNumber
                  << " | Type: " << cardType
                  << " | Status: " << cardStatus << "\n";
    }
};

long ATMCard::numCounter = 0;
