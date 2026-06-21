#include<bits/stdc++.h>
using namespace std;

class Date{
    private:
        int day;
        int month;
        int year;
    
    public:
        Date(int d,int m, int y){
            day=d;
            month=m;
            year=y;
        }
        Date(){
            time_t now=time(0);
            tm* localTime=localtime(&now);
            day = localTime->tm_mday;
            month = localTime->tm_mon + 1;  
            year = localTime->tm_year + 1900;
        }


};


class InsufficientBalanceException : public runtime_error{
    public:
        InsufficientBalanceException()
            :runtime_error("Transaction failed: Insufficent Balance"){}
};

class InvalidPINException : public runtime_error {
    public:
        InvalidPINException() 
            : runtime_error("Authentication Failed: Invalid PIN entered.") {}
};

class AccountBlockedException : public runtime_error {
    public:
        AccountBlockedException() 
            : runtime_error("Access Denied: This account is currently blocked.") {}
};

class LoanRejectedException : public runtime_error {
    public:
        LoanRejectedException() 
            : runtime_error("Application Denied: Loan does not meet approval criteria.") {}
};

class Branch;
class Customer;
class Transaction;
class Employee;
class Loan;

class Account {
    protected:
        long accountNumber;
        string accountType;
        double balance;
        Date dateOpened;
        string status;
        Branch* linkedBranch;
        Customer* owner;
        list<Transaction>transaction;


    public:
    Account(){
        balance=0;
    }
        virtual bool withdraw(double amount)=0;
        virtual void deposit(double amount){
            if(status=="Blocked"){
                throw AccountBlockedException();
            }

            balance+=amount;
            cout<<"Deposit of "<<amount <<" successful. New Balance "<<balance<<endl;

        }


};



class Branch{
    private:
        int branchId;
        list<Account*>accounts;
    
    public:
        string branchName;
        string IFSCCode;
        string address;
        list<Employee>employees;


};




class Customer{
    private:
        int customerId;
        string fullName;
        Date dob;
        string gender;
        string mobileNumber;
        string email;
        string address;
        string aadhaarNumber;
        string PANNumber;
        list<Account*>accounts;
        list<Loan>loans;

};



class Transaction {
    private:
        int transactionId;
        string transactionType;
        double amount;
        Date transactionDate;
        Account* senderAccount;
        Account* receiverAccount;
        string status;



    public:
        Transaction(int id,string type,double amt, Date date,Account* sender,Account* receiver){
            transactionId=id;
            transactionType=type;
            amount=amt;
            transactionDate=date;
            senderAccount=sender;
            receiverAccount=receiver;
            status="Pending";
        }


};

class Employee{
    private:
        int employeeId;
        string employeeName;
        string designation;
        double salary;
        Branch* assignedBranch;

    
    public:
        Employee(int id,string name,string role,double pay,Branch*branch){
            employeeId = id;
            employeeName = name;
            designation = role;
            salary = pay;
            assignedBranch = branch;
        }

};

class Loan{
    private:
        int loanId;
        string loanType;
        double loanAmount;
        double interestRate;
        int tenureMonths;
        double EMIAmount;
        string loanStatus;
        Customer* owner;


    public:
            Loan(int id,string type,double amt,double rate,int tenure, Customer* custo){
                loanId=id;
                loanType = type;
                loanAmount = amt;
                interestRate = rate;
                tenureMonths = tenure;
                owner = custo;
                loanStatus="Pending";


            }


};

class Bank{
    private:
        int bankId;
      
    public:
        string bankName;
        list<Branch>branches;
        list<Customer>customers;
        list<Employee>employees;    
    
    Bank() {}

    Bank(string name, int id){
        bankId=id;
        bankName=name;
    }

    void transfer(Account* sender, Account* receiver,double amount){
        try{
            sender->withdraw(amount);
            receiver->deposit(amount);
            Date today;
            Transaction t(0,"Transfer",amount,today,sender,receiver);
            cout<<"Transfer Completed"<<endl;
        }
        catch (const runtime_error& e) {
                cout << e.what() << "\n";
                cout << "Transfer Aborted."<<endl;
            }
    }

    

};




class SavingsAccount : public Account{
    private:
        double interestRate;
        double minimumBalance;

    public:
        SavingsAccount(double minBal, double intRate){
            interestRate=intRate;
            minimumBalance=minBal;
            status="Active";
        }

        bool withdraw(double amount) override{
            if(status=="Blocked"){
                throw AccountBlockedException();
            }

            if((balance-amount)<minimumBalance){
                throw InsufficientBalanceException();
            }
            balance-=amount;
            cout<<"Withdrawl Successfull. New Balance:"<<balance<<endl;
            return true;
        }   

};

class CurrentAccount : public Account{
    private:
        double overdraftLimit;
        string businessName;

    public:
        CurrentAccount(double limit,string name){
            overdraftLimit=limit;
            businessName=name;
            status="Active";
        }
        bool withdraw(double amount) override{
            if(status=="Blocked"){
                throw AccountBlockedException();
            }

            if(amount>(balance+overdraftLimit)){
                throw InsufficientBalanceException();
            }
            balance-=amount;
            cout<<"Withdrawl Successfull. New Balance:"<<balance<<endl;
            return true;

        }



};

class FixedDepositAccount: public Account{
    private:
        double FDAmount;
        Date maturityDate;
        double FDInterestRate;
        int tenureMonths;

     public:
        FixedDepositAccount(double amt,double intRate,int months){
            FDAmount=amt;
            FDInterestRate=intRate;
            tenureMonths=months;
        }   
        bool withdraw(double amount) override{
            throw runtime_error("Transaction Failed: Cannot withdraw directly from FD.");
        }


};




class ATMCard {
    private:
        long cardNumber;
        int CVV;
        Date expiryDate;
        int PIN;
        string cardType;
        string cardStatus;
        Account* linkedAccount;

    
    public:
        ATMCard(long cardNum,int cvv,Date expiry, int pin , string type, Account* acc){
            cardNumber = cardNum;
            CVV =cvv;
            expiryDate=expiry;  
            PIN = pin;
            cardType = type;
            linkedAccount = acc;
            cardStatus="Active";

        }

        bool ATMWithdrawl(int pinEnter, double amount){
            if(cardStatus=="Blocked"){
                throw AccountBlockedException();
            }

            if(pinEnter!=PIN){
                throw InvalidPINException();
            }

            return linkedAccount->withdraw(amount);

        }



};

class Notification{
    protected:
        string message;
    
    public:
        Notification(string msg){
            message=msg;
        }

    virtual void send()=0;


};

class SMSNotification : public Notification{
    private:
        string phoneNumber;
        string deliveryStatus;

    public:
        SMSNotification(string number, string msg) : Notification(msg) {
            phoneNumber = number;
            deliveryStatus = "Pending";
        }
    
        void send() override{
            cout<<"Sending sms to "<<phoneNumber<<" :"<<message<<endl;
            deliveryStatus="Sent";
        }


};

class EmailNotification : public Notification{
    private:
        string emailAddress;
        string subject;
        string deliveryStatus;

    public:
        EmailNotification(string email,string sub,string msg) : Notification(msg){
            emailAddress=email;
            subject=sub;
            deliveryStatus="Pending";
        }
        
        void send() override{
            cout << "Sending Email to " << emailAddress << "\n";
            cout << "Subject: " << subject << "\n";
            cout << "Body: " << message << "\n";
            deliveryStatus = "Sent";
        }

};

int main(){
    Bank b1;
    b1.bankName="DholakpurFunds";

    SavingsAccount* bheemAcc=new SavingsAccount(1000,4.5);
    CurrentAccount* rajuAcc=new CurrentAccount(5000,"Udhari");

    bheemAcc->deposit(6000); 
    rajuAcc->deposit(2000);    

    b1.transfer(bheemAcc,rajuAcc,1500);
    b1.transfer(bheemAcc,rajuAcc,4000);

    delete bheemAcc;
    delete rajuAcc;
    return 0;



}