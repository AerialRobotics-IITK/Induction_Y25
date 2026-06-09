#include "ATMCard.h"
#include "Exceptions.h"
#include <cstdlib>

long ATMCard::nextCardNumber_ = 4000000000000001L;

ATMCard::ATMCard(const std::string& expiryDate, int pin, const std::string& cardType,
                 Account* linkedAccount)
    : cardNumber_(nextCardNumber_++),
      cvv_(100 + std::rand() % 900),
      expiryDate_(expiryDate),
      pin_(pin),
      cardType_(cardType),
      cardStatus_("Active"),
      linkedAccount_(linkedAccount) {}

void ATMCard::validatePIN(int enteredPIN) const {
    if (cardStatus_ == "Blocked")
        throw AccountBlockedException("ATM Card " + std::to_string(cardNumber_) + " is blocked");
    if (enteredPIN != pin_)
        throw InvalidPINException("Invalid PIN for card " + std::to_string(cardNumber_));
}

void ATMCard::blockCard() {
    cardStatus_ = "Blocked";
    std::cout << "  Card " << cardNumber_ << " has been blocked.\n";
}

void ATMCard::activateCard() {
    cardStatus_ = "Active";
    std::cout << "  Card " << cardNumber_ << " is now active.\n";
}

void ATMCard::changePIN(int oldPIN, int newPIN) {
    validatePIN(oldPIN);
    pin_ = newPIN;
    std::cout << "  PIN changed successfully for card " << cardNumber_ << ".\n";
}

void ATMCard::displayCardInfo() const {
    std::cout << "--- ATM Card ---\n"
              << "  Card Number : " << cardNumber_ << "\n"
              << "  Card Type   : " << cardType_ << "\n"
              << "  Expiry      : " << expiryDate_ << "\n"
              << "  Status      : " << cardStatus_ << "\n";
}
