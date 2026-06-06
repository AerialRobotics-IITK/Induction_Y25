#include "Transaction.h"

int Transaction::idGenerator = 0;

Transaction::Transaction(std::string TransactionType, double Amount, Date TransactionDate, Account *SenderAccount, Account *ReceiverAccount, std::string Status){
    transactionId = ++idGenerator;
    transactionType = TransactionType;
    amount = Amount;
    transactionDate = TransactionDate;
    senderAccount = SenderAccount;
    receiverAccount = ReceiverAccount;
    status = Status;
}

void Transaction::editStatus(std::string Status){
    status = Status;
}

std::string Transaction::getTransactionType(){
    return transactionType;
}
double Transaction::getAmount(){
    return amount;
}
Date Transaction::getTransactionDate(){
    return transactionDate;
}
Account *Transaction::getSenderAccount(){
    return senderAccount;
}
Account *Transaction::getReceiverAccount(){
    return receiverAccount;
}
std::string Transaction::getStatus(){
    return status;
}