#include <stdio.h>
#include "account.h"

//definition for the same.
Account* findAccount(long acc_number, std::vector<Account>& accounts)
{
    for(auto& acc : accounts)
    {
        if(acc.getAccountNumber() == acc_number) return &acc;
    }
    return nullptr;
}
