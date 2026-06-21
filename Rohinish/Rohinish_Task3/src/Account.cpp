#include "Account.h"

long Account::nextAccountNumber_ = 1000000001L;

Account::Account(const std::string& type, double initialBalance,
                 const std::string& dateOpened, Branch* branch, Customer* customer)
    : accountNumber_(nextAccountNumber_++),
      accountType_(type),
      balance_(initialBalance),
      dateOpened_(dateOpened),
      status_("Active"),
      branch_(branch),
      customer_(customer) {}
