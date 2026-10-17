#pragma once
#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>

// Forward declaration
class Account;

// ─── Transaction ──────────────────────────────────────────────────────────────
class Transaction {
private:
    static int idCounter;
public:
    int transactionId;
    std::string transactionType;   // "Deposit", "Withdrawal", "Transfer"
    double amount;
    std::time_t transactionDate;
    Account* senderAccount;
    Account* receiverAccount;
    std::string status;            // "Success", "Pending", "Failed"

    Transaction(const std::string& type,
                double             amt,
                Account*           sender   = nullptr,
                Account*           receiver = nullptr,
                const std::string& stat     = "Success")
        : transactionId(++idCounter),
          transactionType(type),
          amount(amt),
          transactionDate(std::time(nullptr)),
          senderAccount(sender),
          receiverAccount(receiver),
          status(stat) {}

    void display() const {
        std::cout << std::fixed << std::setprecision(2)
                  << "  [TXN #" << transactionId << "] "
                  << transactionType
                  << " | Amount: Rs." << amount
                  << " | Status: " << status << "\n";
    }
};

int Transaction::idCounter = 0;
