#include "Transaction.h"
#include "Account.h"

int Transaction::nextTransactionId_ = 1;

Transaction::Transaction(const std::string& type, double amount, const std::string& date,
                         Account* sender, Account* receiver)
    : transactionId_(nextTransactionId_++),
      transactionType_(type),
      amount_(amount),
      transactionDate_(date),
      senderAccount_(sender),
      receiverAccount_(receiver),
      status_("Success") {}

void Transaction::displayTransaction() const {
    std::cout << "  TXN#" << transactionId_
              << " | " << transactionType_
              << " | Rs." << amount_
              << " | " << transactionDate_
              << " | Sender: "
              << (senderAccount_ ? std::to_string(senderAccount_->getAccountNumber()) : "N/A")
              << " | Receiver: "
              << (receiverAccount_ ? std::to_string(receiverAccount_->getAccountNumber()) : "N/A")
              << " | " << status_ << std::endl;
}
