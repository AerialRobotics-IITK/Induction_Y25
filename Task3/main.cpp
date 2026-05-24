#include <iostream>
#include <vector>
#include "account.h"
#include "customer.h"

//function to get balance of a given account number.
void getBalance(long acc_number, std::vector<Account>& accounts)
{
    Account* tempptr{};
    if((tempptr = findAccount(acc_number, accounts)) == nullptr)
    {
         std::cout << "Your account could not be found!\n";
    }
    else
    {
        tempptr->printBalance();
    }
}



int main()
{
/*  This is the start of the program. We greet the user with a welcome message and
    give them options to interact with the program.
    
*/
    std::cout << "Welcome to Banking Interface! Type one of the commands (in lowercase)\n"
              << "(At any point you may type 'list' to get all available commands.)\n"
              << "* create account\n* check balance\n* deposit\n* withdraw\n"
              << "* list\n* exit\n";

    //declaring a string which takes input from the terminal.
    std::string inputcmd{};

    //Now we take input and figure out which command it was. then we give further steps appropriately.
    //We'll be giving the accounts a no. The variable globalacc handles that.
    //Create a vector of accounts and operate it like a stack.
    //tempaccountnumber is used for taking account number inputs from various commands.
    std::vector<Account> accounts{};
    long globalacc{100};
    long tempaccountnumber{};

    while(1)
    {
        std::getline(std::cin, inputcmd);
        //std::cout << '[' << inputcmd << "]\n";

        //ignore blank cmd(result of pressing enter)
        if(inputcmd == "") continue;

        //Case: command to create a account. Ask for account type and initial deposit.
        if(inputcmd == "create account")
        {
            std::cout << "Please enter the type and initial deposit for your account: ";
            std::string type{};
            double dep{};
            std::cin >> type >> dep;
            accounts.push_back(Account{globalacc, type, dep});
            globalacc++;
            //at this point the constructor of Account prints successful creation of account.
        }

        //Case: command to deposit money into account. ask for acc number.
        else if(inputcmd == "deposit")
        {
            std::cout << "Please type your account number: ";
            std::cin >> tempaccountnumber;
            Account* tempptr{};
            if((tempptr = findAccount(tempaccountnumber, accounts)) == nullptr)
            {
                std::cout << "Your account could not be found!\n";
            }
            else
            {
                std::cout << "Please enter the amount you would like to deposit: ";
                double money{};
                std::cin >> money;
                tempptr->deposit(money);
            }
        }

        //Case: command to withdraw money.
        else if(inputcmd == "withdraw")
        {
            std::cout << "Please type your account number: ";
            std::cin >> tempaccountnumber;
            Account* tempptr{};
            if((tempptr = findAccount(tempaccountnumber, accounts)) == nullptr)
            {
                std::cout << "Your account could not be found!\n";
            }
            else
            {
                std::cout << "Please enter the amount you would like to withdraw: ";
                double money{};
                std::cin >> money;
                tempptr->withdraw(money);
            }
        }

        //Case: command input was to check balance in the account
        //      ask for account no. and match it with an existing account. 
        else if(inputcmd == "check balance")
        {
            std::cout << "Please type your account number: ";
            std::cin >> tempaccountnumber;
            getBalance(tempaccountnumber, accounts);
        }

        //Case: command to list all commands.
        else if(inputcmd == "list")
        {
            std::cout << "Available commands are:\n"
                      << "* create account\n* check balance\n* deposit\n* withdraw\n"
                      << "* list\n* exit\n";
        }

        //Case: command to exit the interfact.
        else if(inputcmd == "exit")
        {
            std::cout << "Visit soon!";
            break;
        }

        //Case: invalid command.
        else
        {
            std::cout << "Unrecognised command. Type 'list' to see available commands.\n";
        }
    }
    return 0;
}
