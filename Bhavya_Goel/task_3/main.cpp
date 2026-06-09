#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "bank.cpp" 

using namespace std;

int main() {
    Bank myBank(1, "IITK Bank");
    Branch defaultBranch(101, "IIT Kanpur Main Branch", "IITK00101", "Kanpur, India");
    myBank.addBranch(&defaultBranch);

    map<int, Customer*> customerMap;
    map<long, Account*> accountMap;

    int nextCustomerId = 1;
    long nextAccountNumber = 10001;
    int nextLoanId = 5001;

    int choice;
    do {
        cout << "\n=========================================\n";
        cout << "            IITK BANK SYSTEM             \n";
        cout << "=========================================\n";
        cout << "1. Register New Customer\n";
        cout << "2. Open New Account\n";
        cout << "3. Deposit Funds\n";
        cout << "4. Withdraw Funds\n";
        cout << "5. Transfer Funds\n";
        cout << "6. Apply for a Loan\n";
        cout << "7. View Customer Portfolio\n";
        cout << "8. Send Notification\n";
        cout << "9. Exit\n";
        cout << "=========================================\n";
        cout << "Enter your choice: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {

            case 1: {
                string name, dob, gender, mobile, email, address, aadhaar, pan;
                cout << "\n--- Register Customer (IITK Bank) ---\n";
                cout << "Name (Use_Underscores_for_spaces): "; cin >> name;
                cout << "DOB (DD/MM/YYYY): "; cin >> dob;
                cout << "Gender: "; cin >> gender;
                cout << "Mobile Number: "; cin >> mobile;
                cout << "Email: "; cin >> email;
                cout << "Address: "; cin >> address;
                cout << "Aadhaar Number: "; cin >> aadhaar;
                cout << "PAN Number: "; cin >> pan;

                Customer* newCustomer = new Customer(
                    nextCustomerId, name, dob, gender, mobile, email, address, aadhaar, pan
                );
                
                customerMap[nextCustomerId] = newCustomer;
                myBank.addCustomer(newCustomer);
                
                cout << "-> Customer Registered Successfully in IITK Bank! Customer ID: " 
                     << nextCustomerId << "\n";
                nextCustomerId++;
                break;
            }

            case 2: {
                int cId, typeChoice;
                cout << "\n--- Open New Account (IITK Bank) ---\n";
                cout << "Enter Customer ID: "; cin >> cId;

                if (customerMap.find(cId) == customerMap.end()) {
                    cout << "-> Error: Customer Not Found in IITK Bank.\n";
                    break;
                }
                
                Customer* cust = customerMap[cId];

                cout << "Account Type (1: Savings, 2: Current, 3: Fixed Deposit): "; 
                cin >> typeChoice;

                Account* newAccount = nullptr;
                double initialDeposit;
                cout << "Enter Initial Deposit Amount: "; cin >> initialDeposit;

                if (typeChoice == 1) {
                    newAccount = AccountFactory::createSavingsAccount(
                        nextAccountNumber, initialDeposit, 1000.0, 4.0,100000, "12-05-2026",
                        &defaultBranch, cust
                    );
                } else if (typeChoice == 2) {
                    string bizName;
                    cout << "Enter Business Name: "; cin >> bizName;
                    newAccount = AccountFactory::createCurrentAccount(
                        nextAccountNumber, initialDeposit, 50000.0, bizName,
                        "12-05-2026", &defaultBranch, cust
                    );
                } else if (typeChoice == 3) {
                    int tenure;
                    cout << "Enter Tenure (in months): "; cin >> tenure;
                    newAccount = AccountFactory::createFDAccount(
                        nextAccountNumber, initialDeposit, 7.5, tenure,
                        "12-05-2027", "12-05-2026", &defaultBranch, cust
                    );
                } else {
                    cout << "-> Error: Invalid Account Type.\n";
                    break;
                }

                accountMap[nextAccountNumber] = newAccount;
                cust->addAccount(newAccount);
                defaultBranch.addAccount(newAccount);

                cout << "-> Account Opened Successfully in IITK Bank! Account Number: " 
                     << nextAccountNumber << "\n";
                nextAccountNumber++;
                break;
            }

            case 3: {
                long accNo;
                double amount;
                cout << "\n--- Deposit Funds (IITK Bank) ---\n";
                cout << "Enter Account Number: "; cin >> accNo;
                
                if (accountMap.find(accNo) != accountMap.end()) {
                    cout << "Enter Amount to Deposit: "; cin >> amount;
                    try {
                        accountMap[accNo]->deposit(amount);
                        cout << "-> Deposit Successful. Current Balance: "
                             << accountMap[accNo]->getBalance() << "\n";
                    } catch (const exception& e) {
                        cout << "-> Transaction Failed: " << e.what() << "\n";
                    }
                } else {
                    cout << "-> Error: Account Not Found in IITK Bank.\n";
                }
                break;
            }

            case 4: {
                long accNo;
                double amount;
                cout << "\n--- Withdraw Funds (IITK Bank) ---\n";
                cout << "Enter Account Number: "; cin >> accNo;

                if (accountMap.find(accNo) != accountMap.end()) {
                    cout << "Enter Amount to Withdraw: "; cin >> amount;
                    try {
                        accountMap[accNo]->withdraw(amount);
                        cout << "-> Withdrawal Successful. Current Balance: "
                             << accountMap[accNo]->getBalance() << "\n";
                    } catch (const exception& e) {
                        cout << "-> Transaction Failed: " << e.what() << "\n";
                    }
                } else {
                    cout << "-> Error: Account Not Found.\n";
                }
                break;
            }

            case 5: {
                long senderAcc, receiverAcc;
                double amount;
                cout << "\n--- Transfer Funds (IITK Bank) ---\n";
                cout << "Enter Sender Account Number: "; cin >> senderAcc;
                cout << "Enter Receiver Account Number: "; cin >> receiverAcc;
                
                if (accountMap.find(senderAcc) != accountMap.end() &&
                    accountMap.find(receiverAcc) != accountMap.end()) {
                    
                    cout << "Enter Amount to Transfer: "; cin >> amount;
                    try {
                        TransactionService::transfer(
                            accountMap[senderAcc],
                            accountMap[receiverAcc],
                            amount
                        );
                        cout << "-> Transfer Complete in IITK Bank.\n";
                    } catch (const exception& e) {
                        cout << "-> Transfer Failed: " << e.what() << "\n";
                    }
                } else {
                    cout << "-> Error: One or both accounts not found.\n";
                }
                break;
            }

            case 6: {
                int cId, loanType;
                cout << "\n--- Apply for Loan (IITK Bank) ---\n";
                cout << "Enter Customer ID: "; cin >> cId;

                if (customerMap.find(cId) == customerMap.end()) {
                    cout << "-> Error: Customer Not Found.\n";
                    break;
                }

                cout << "Loan Type (1: Home, 2: Car, 3: Personal): "; cin >> loanType;
                double amount, rate;
                int years;

                cout << "Enter Loan Amount: "; cin >> amount;
                cout << "Enter Interest Rate (%): "; cin >> rate;
                cout << "Enter Tenure (Years): "; cin >> years;

                Loan* newLoan = nullptr;

                if (loanType == 1) {
                    string property;
                    cout << "Enter Property Address: "; cin >> property;
                    newLoan = new HomeLoan(nextLoanId, amount, rate, years,
                                           customerMap[cId], property);
                } else if (loanType == 2) {
                    string model;
                    cout << "Enter Car Model: "; cin >> model;
                    newLoan = new CarLoan(nextLoanId, amount, rate, years,
                                         customerMap[cId], model);
                } else if (loanType == 3) {
                    string purpose;
                    cout << "Enter Purpose: "; cin >> purpose;
                    newLoan = new PersonalLoan(nextLoanId, amount, rate, years,
                                              customerMap[cId], purpose);
                }

                if (newLoan) {
                    customerMap[cId]->addLoan(newLoan);
                    newLoan->approveLoan();
                    cout << "-> Loan Approved by IITK Bank! Loan ID: " 
                         << nextLoanId << "\n";
                    newLoan->displayLoan();
                    nextLoanId++;
                }
                break;
            }

            case 7: {
                int cId;
                cout << "\n--- Customer Portfolio (IITK Bank) ---\n";
                cout << "Enter Customer ID: "; cin >> cId;
                
                if (customerMap.find(cId) != customerMap.end()) {
                    customerMap[cId]->showAccounts();
                } else {
                    cout << "-> Error: Customer Not Found.\n";
                }
                break;
            }

            case 8: {
                int notifType;
                string target,sub, message;
                cout << "\n--- Send Notification (IITK Bank) ---\n";
                cout << "1. SMS\n2. Email\nSelect type: "; cin >> notifType;
                
                cout << "Enter Recipient: "; cin >> target;
                cout << "Enter Subject: "; cin >> sub;
                cout << "Enter Message: "; cin >> message;

                Notification* notif = nullptr;

                if (notifType == 1)
                    notif = new SMSNotification(target);
                else
                    notif = new EmailNotification(target,sub);

                notif->send(message);
                delete notif;
                break;
            }

            case 9:
                cout << "Exiting IITK Bank System. Goodbye!\n";
                break;

            default:
                cout << "-> Invalid option. Try again.\n";
        }

    } while (choice != 9);

    return 0;
}