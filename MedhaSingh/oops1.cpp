#include<iostream>
#include<vector>
#include<string>
using namespace std;

class Branch;
class Customer;
class Account;
class Transaction;
class Loan;
class Employee;
class Notification;
class SMSNotification;
class EmailNotification;

class Date{
    int day;
    int month;
    int year;

    public:
    Date(int day=0, int month=0, int year=0){
        this->day=day;
        this->month=month;
        this->year=year;
    }
    void setDate(int day, int month, int year){
        this->day=day;
        this->month=month;
        this->year=year;
    }
    void inputDate(){
        cout<<"Enter day: "<<endl;
        cin>>day;
        cout<<"Enter month: "<<endl;
        cin>>month;
        cout<<"Enter year: "<<endl;
        cin>>year;
    }
    void showDate(){
        cout<<day<<"/"<<month<<"/"<<year<<endl;
    }

    bool isAfter(Date other){
        if (year!=other.year){return year>other.year;}
        else if (month!=other.month) {return month>other.month;}
        return day>other.day;
    }

    bool isBefore(Date other){
        if (year!=other.year){return year<other.year;}
        else if (month!=other.month) {return month<other.month;}
        return day<other.day;
    }

    bool isEqual(Date other){
        return day==other.day && month==other.month && year==other.year;
    }
};

class Bank{
    int bankId;
    
    public:
    vector<Branch*> branches;
    vector<Customer*> customers;
    vector<Employee*> employees;
    string bankName;
    Bank(int id, string name){
        bankId=id;
        bankName=name;
    }
    void addBranch(Branch* b);
    void showBranches();
    void addCustomer(Customer* c){
        customers.push_back(c);
    }
    void addEmployee(Employee* e){
        employees.push_back(e);
    }

};

class Branch{
    int branchId;
    string IFSCCode;
    string address;
    vector<Account*> accounts;
    vector<Employee*> employees;

    public:
    Bank* bank;
    string branchName;
    Branch(int id, string name, string code, string adrs){
        branchId=id;
        branchName=name;
        IFSCCode=code;
        address=adrs;
    }

    Branch(){
        cout<<"Enter branch id: ";
        cin>>branchId;
        cout<<"Enter branch name: ";
        cin>>branchName;
        cout<<"Enter IFSC code: ";
        cin>>IFSCCode;
        cout<<"Enter address: ";
        cin>>address;
    }
    int getBranchId(){
        return branchId;
    }
    void addAccount(Account* account){
        accounts.push_back(account);
    }
    void showAccounts();
    void findAccount(long accountNumber);

    void addEmployee(Employee* e);
        
};

void Bank::addBranch(Branch* b){
    branches.push_back(b);
    b->bank=this;
}

void Bank::showBranches(){
    cout<<"Branches in "<<bankName<<":"<<endl;
    for(int i=0; i<branches.size(); i++){
        cout<<"\nSno.:"<<i+1<<endl;
        cout<<"Branch id: "<<branches[i]->getBranchId()<<endl;
        cout<<"Branch name: "<<branches[i]->branchName<<endl;
    }
}

class Customer{
    int customerId;
    Date dob;
    string gender;
    string mobileNumber;
    string email;
    string address;
    string aadhaarNumber;
    vector<Account*> accounts;
    vector<Loan*> loans;
    vector<Notification*> notifications;

    public:
    string fullName;
    // Customer(int id, string name,/*Date dob,*/ string gender, string mob, string mail, string adrs, string ano){
    //     customerId=id;
    //     fullName=name;
    //     //this->dob=dob;
    //     this->gender=gender;
    //     mobileNumber=mob;
    //     email=mail;
    //     address=adrs;
    //     aadhaarNumber=ano;
    // }

    Customer(){
        cout<<"Enter customer id: ";
        cin>>customerId;
        cout<<"Enter full name: ";
        cin>>fullName;
        cout<<"Enter dob:"<<endl;
        dob.inputDate();
        cout<<"Enter gender: ";
        cin>>gender;
        cout<<"Enter mobile number: ";
        cin>>mobileNumber;
        cout<<"Enter email: ";
        cin>>email;
        cout<<"Enter address: ";
        cin>>address;
        cout<<"Enter aadhaar number: ";
        cin>>aadhaarNumber;
    }
    int getId(){
        return customerId;
    }
    void addAccount(Account* account){
        accounts.push_back(account);
    }
    void showAccounts();
    void addNotification(Notification* n);
    void displayNotifications();
    void applyLoan(Loan* l);
    string getMobile(){
        return mobileNumber;
    }

    string getEmail(){
        return email;
    }
};

class Account{
    protected:
    long accountNumber;
    string accountType;
    double balance;
    Date dateOpened;
    string status;
    Branch* branch;
    Customer* customer;
    vector<Transaction*> transactions;
    bool smsEnabled=false;
    bool emailEnabled=false;
    //Notification* notifier=nullptr;

    public:
    Account(){
        cout<<"Enter account number: ";
        cin>>accountNumber;
        cout<<"Enter balance: ";
        cin>>balance;
        cout<<"Date opened:"<<endl;
        dateOpened.inputDate();
        status="Active";
    }

    void addTransaction(Transaction* transaction){
        transactions.push_back(transaction);
    }
    void enableSMS(){
        smsEnabled=true;
    }

    void enableEmail(){
        emailEnabled=true;
    }
    // void addNotification(Notification* n){
    //     notifier=n;
    // };
    virtual void withdraw(double)=0;
    
    void deposit(double amt);

    void showBalance(){
        cout<<"current balance: Rs."<<balance<<endl;
    }
    double getBalance(){
        return balance;
    }
    long getAccountNumber(){
        return accountNumber;
    }
    string getAccountType(){
        return accountType;
    };
    void showdetails(){
        cout<<"Account number: "<<accountNumber<<endl;
        cout<<"Account holder: "<<customer->fullName<<endl;
        cout<<"Account type: "<<getAccountType()<<endl;
        cout<<"Balance: "<<getBalance()<<endl;
    }
    void setCustomer(Customer* customer){
        this->customer=customer;
        customer->addAccount(this);
    }
    void setBranch(Branch* br){
        branch=br;
        int x=0;
        for(int i=0; i<(branch->bank->customers.size()); i++){
            if(branch->bank->customers[i]->getId()==customer->getId()){
                x=1;
                break;
            }
        }
        if(x==0){
            branch->bank->addCustomer(customer);
        }  
        branch->addAccount(this);      
    }
    void showTransactions();  
    void blockAccount() { status = "Blocked"; }
    string getStatus() { return status; }
    
};

class SavingsAccount : public Account{
    double interestRate;
    double minimumBalance;
    public:
    SavingsAccount(double minbal=500){
        accountType="Savings";
        minimumBalance=minbal;
    }
    virtual void withdraw(double amt);
};

class CurrentAccount : public Account{
    double overdraftLimit;
    string businessName;
    public:
    CurrentAccount(double od=1000){
        accountType="Current";
        cout<<"Enter business name: ";
        cin>>businessName;
        overdraftLimit=od;
    }

    virtual void withdraw(double amt);
    
};

class FixedDepositAccount : public Account{
    double FDAmount;
    Date maturityDate;
    double FDInterestRate;
    int tenureMonths;
    public:
    FixedDepositAccount(double interest){
        accountType="Fixed Deposit";
        FDAmount=balance;
        FDInterestRate=interest;
        cout<<"Enter maturity date: "<<endl;
        maturityDate.inputDate();
        cout<<"Enter tenure (in months): "<<endl;
        cin>>tenureMonths;
    }
    double calculateMaturityAmount(){
        double t=tenureMonths/12.0;
        return FDAmount*(1+(FDInterestRate/100.0)*t);
    }

    void showMaturityDetails() {
        cout << "FD Amount: Rs." << FDAmount << endl;
        cout << "Interest Rate: " << FDInterestRate << "%" << endl;
        cout << "Tenure: " << tenureMonths << " months" << endl;
        cout << "Maturity Amount: Rs." << calculateMaturityAmount() << endl;
        cout << "Maturity Date: ";
        maturityDate.showDate();
    }

    virtual void withdraw(double amt);
};

class Transaction{
    int transactionId;
    string transactionType;
    double amount;
    Date transactionDate;
    Account* senderAccount;
    Account* recieverAccount;
    string status;

    public:
    Transaction(string type, double amt, string st="Successful"){
        transactionType=type;
        amount=amt;
        status=st;
        cout<<"date of transaction:"<<endl;
        transactionDate.inputDate();
    }
    void transfer(Account* sender, Account* reciever, double amt){
        double ogBalance=sender->getBalance();
        sender->withdraw(amt);
        if (sender->getBalance()!=ogBalance){
            reciever->deposit(amt);
            senderAccount=sender;
            recieverAccount=reciever;
        }
        else{
            cout<<"Transfer failed"<<endl;
        }

    }
    void showTransaction(){
        cout<<"Type: "<<transactionType<<endl;
        cout<<"Amount: "<<amount<<endl;
        cout<<"Status: "<<status<<endl;
        cout<<"Date of transaction: ";
        transactionDate.showDate();
        cout<<endl;
    }
};

void Customer::showAccounts(){
    cout<<"\nAccounts of "<<this->fullName<<"(Customer id: "<<this->getId()<<")"<<" are as follows: "<<endl;    
    for(int i=0; i<accounts.size(); i++){
        cout<<"Sno.: "<<i+1<<endl;
        accounts[i]->showdetails();
        cout<<endl;
    }
}

void Branch::showAccounts(){
    cout<<"\nAccounts in Branch "<<this->branchName<<" are as follows: "<<endl;    
    for(int i=0; i<accounts.size(); i++){
        cout<<"Sno.: "<<i+1<<endl;
        accounts[i]->showdetails();
        cout<<endl;
    }
}

void Branch::findAccount(long accountNumber){
    for(int i=0; i<accounts.size(); i++){
        if(accounts[i]->getAccountNumber()==accountNumber){
            cout<<"Account found!"<<endl;
            accounts[i]->showdetails();
            return;
        }
    }
    cout<<"No such account found."<<endl;
}

void Account::showTransactions(){
    for(int i=0; i<transactions.size(); i++){
        transactions[i]->showTransaction();
        cout<<endl;
    }
}

class Loan{
    int loanId;
    string loanType;
    double loanAmount;
    double interestRate;
    int tenureYears;
    double EMIAmount;
    string loanStatus;
    Customer* customer;

    public:
    void calculateEMI(){
        EMIAmount=(loanAmount + (loanAmount*interestRate*tenureYears/100))/(tenureYears*12);
    }
    Loan(Customer* c, int id, string type, double amt, double interest, int yrs){
        customer=c;
        c->applyLoan(this);
        loanId=id;
        loanType=type;
        loanAmount=amt;
        interestRate=interest;
        tenureYears=yrs;
        calculateEMI();
    }
    
    void showLoan(){
        cout<<"Loan ID: "<<loanId<<endl;
        cout<<"Loan Type: "<<loanType<<endl;
        cout<<"Loan Amount: "<<loanAmount<<endl;
        cout<<"EMI: "<<EMIAmount<<endl;
    }

};

void Customer::applyLoan(Loan* l){
    loans.push_back(l);
};

class ATMCard{
    long cardNumber;
    int CVV;
    Date expiryDate;
    int PIN;
    string cardType;
    string cardStatus="Active";
    Account* linkedAccount;

    public:
    ATMCard(long cno, int CVV, int PIN, string type){
        cardNumber=cno;
        this->CVV=CVV;
        this->PIN=PIN;
        cardType=type;
        cout<<"Enter expiry date: "<<endl;
        expiryDate.inputDate();
    }

    ATMCard(){
        cout<<"Enter card number: ";
        cin>>cardNumber;
        cout<<"Enter CVV: ";
        cin>>CVV;
        cout<<"Enter PIN: ";
        cin>>PIN;
        cout<<"Enter card type: ";
        cin>>cardType;
        cout<<"Enter expiry date: "<<endl;
        expiryDate.inputDate();
    }
    void setAccount(Account* a){
        linkedAccount=a;
    }
    bool verifyPIN(int p){
        return PIN==p;
    }
    bool isExpired() {
        Date today;
        cout << "Enter today's date:" << endl;
        today.inputDate();
        if(today.isAfter(expiryDate)){
            cardStatus="Expired";
        }
        else{
            cardStatus="Active";
        }
        return today.isAfter(expiryDate);
    }
    void withdrawCash(int p, double amt){
        if(!verifyPIN(p)){
            cout<<"Wrong PIN"<<endl;
        }
        else if(isExpired()){
            cout<<"Card is Expired"<<endl;
        }
        else{
            linkedAccount->withdraw(amt);
        }
    }
    void checkBalance(int p){
        if(!verifyPIN(p)){
            cout<<"Wrong PIN"<<endl;
        }
        else if(isExpired()){
            cout<<"Card is Expired"<<endl;
        }
        else{
            linkedAccount->showBalance();
        }
    }

};

class Employee{
    int employeeId;
    string employeeName;
    string designation;
    double salary;
    Branch* branch;

    public:
    Employee(int id, string name, string designation, double salary=0){
        employeeId=id;
        employeeName=name;
        this->designation=designation;
        this->salary=salary;
    }

    Employee(){
        cout<<"Enter employee id: ";
        cin>>employeeId;
        cout<<"Enter employee name: ";
        cin>>employeeName;
        cout<<"Enter designation: ";
        cin>>designation;
        cout<<"Enter salary: ";
        cin>>salary;
    }
    void setBranch(Branch* b){
        branch=b;
    }
    void showEmployee(){
        cout<<"Employee ID: "<<employeeId<<endl;
        cout<<"Name: "<<employeeName<<endl;
        cout<<"Designation: "<<designation<<endl;
    }
};

void Branch::addEmployee(Employee* e){
    e->setBranch(this);
    employees.push_back(e);
    this->bank->addEmployee(e);
}

class Notification{
    protected:
    string message;
    string deliveryStatus;

    public:
    virtual void send()=0;
    virtual void display()=0;
};

class SMSNotification : public Notification{
    string phoneNumber;

    public:
    SMSNotification(string msg, string no){
        message=msg;
        phoneNumber=no;
    }
    virtual void send(){
        cout<<"SMS Notification '"<<message<<"' sent to phone number "<<phoneNumber<<endl;
    }
    virtual void display(){
        cout<<"Notification type: SMS"<<endl;
        cout<<"Message: "<<message<<endl;
    }
};

class EmailNotification : public Notification{
    string emailAddress;
    string subject;

    public:
    EmailNotification(string msg, string email, string sub){
        message=msg;
        emailAddress=email;
        subject=sub;
    }
    virtual void send(){
        cout<<"Email Notification sent to: "<<emailAddress<<endl;
        cout<<"Subject: "<<subject<<endl;
        cout<<message<<endl;
    }
    virtual void display(){
        cout<<"Notification type: Email"<<endl;
        cout<<"Subject: "<<subject<<endl;
        cout<<"Message: "<<message<<endl;
    }
};

void Customer::addNotification(Notification* n){
    notifications.push_back(n);
}
void Customer::displayNotifications(){
    for (int i=0; i<notifications.size(); i++){
        cout<<"Sno.: "<<i+1<<endl;
        notifications[i]->display();
        cout<<endl;
    }
}

void Account::deposit(double amt){
    balance+=amt;
    // Transaction t("Deposit",amt);
    // addTransaction(t);
    Transaction* t=new Transaction("Deposit",amt);
    addTransaction(t);
    // SMS Notification
    if(smsEnabled){

        Notification* n = new SMSNotification(
            "Rs." + to_string((int)amt) +
            " deposited successfully.",
            customer->getMobile()
        );

        n->send();

        customer->addNotification(n);
    }

    // Email Notification
    if(emailEnabled){

        Notification* n = new EmailNotification(
            "Rs." + to_string((int)amt) +
            " deposited into your account.",
            customer->getEmail(),
            "Deposit Alert"
        );

        n->send();

        customer->addNotification(n);
    }
}

void SavingsAccount::withdraw(double amt){
    if(amt<=balance && (balance-amt)>=(minimumBalance)){
        balance-=amt;
        // Transaction t("Withdrawal",amt);
        // addTransaction(t);
        Transaction* t=new Transaction("Withdrawal",amt);
        addTransaction(t);
        // SMS Notification
        if(smsEnabled){

            Notification* n = new SMSNotification(
                "Rs." + to_string((int)amt) +
                " withdrawn successfully.",
                customer->getMobile()
            );

            n->send();

            customer->addNotification(n);
        }

        // Email Notification
        if(emailEnabled){

            Notification* n = new EmailNotification(
                "Rs." + to_string((int)amt) +
                " withdrawn from your account.",
                customer->getEmail(),
                "Withdrawal Alert"
            );

            n->send();

            customer->addNotification(n);
        }
    }
    else{
        cout<<"insufficient balance"<<endl;
        // Transaction t("Withdrawal",amt,"Failed");
        // addTransaction(t);
        Transaction* t=new Transaction("Withdrawal",amt,"Failed");
        addTransaction(t);
    }
}

void CurrentAccount::withdraw(double amt){
    if(amt<=(balance+overdraftLimit)){
        balance-=amt;
        // Transaction t("Withdrawal",amt);
        // addTransaction(t);
        Transaction* t=new Transaction("Withdrawal",amt);
        addTransaction(t);
        // SMS Notification
        if(smsEnabled){

            Notification* n = new SMSNotification(
                "Rs." + to_string((int)amt) +
                " withdrawn successfully.",
                customer->getMobile()
            );

            n->send();

            customer->addNotification(n);
        }

        // Email Notification
        if(emailEnabled){

            Notification* n = new EmailNotification(
                "Rs." + to_string((int)amt) +
                " withdrawn from your account.",
                customer->getEmail(),
                "Withdrawal Alert"
            );

            n->send();

            customer->addNotification(n);
        }
    }
    else{
        cout<<"insufficient balance"<<endl;
        Transaction* t=new Transaction("Withdrawal",amt,"Failed");
        addTransaction(t);
    }
}

void FixedDepositAccount::withdraw(double amt) {
    Date today;
    cout << "Enter today's date:" << endl;
    today.inputDate();

    if (today.isAfter(maturityDate) || today.isEqual(maturityDate)) {
        if(amt<balance){
            balance -= amt;
            cout << "FD withdrawal successful." << endl;
            // Transaction t("FD Withdrawal",amt);
            // addTransaction(t);
            Transaction* t=new Transaction("FD Withdrawal",amt);
            addTransaction(t);
            // SMS Notification
            if(smsEnabled){

                Notification* n = new SMSNotification(
                    "Rs." + to_string((int)amt) +
                    " withdrawn successfully.",
                    customer->getMobile()
                );

                n->send();

                customer->addNotification(n);
            }

            // Email Notification
            if(emailEnabled){

                Notification* n = new EmailNotification(
                    "Rs." + to_string((int)amt) +
                    " withdrawn from your account.",
                    customer->getEmail(),
                    "Withdrawal Alert"
                );

                n->send();

                customer->addNotification(n);
            } 
        }
        else{
            cout<<"Insufficient Balance"<<endl;
            // Transaction t("FD Withdrawal",amt,"Failed");
            // addTransaction(t);
            Transaction* t=new Transaction("FD Withdrawal",amt,"Failed");
            addTransaction(t);
        } 
    } 
    else {
        cout << "Cannot withdraw before maturity date." << endl;
        // Transaction t("FD Withdrawal",amt,"Failed");
        // addTransaction(t);
        Transaction* t=new Transaction("FD Withdrawal",amt,"Failed");
        addTransaction(t);
    }
}


class AccountMaker{
    public:
    static Account* createAccount(string type) {
        if (type == "Savings") return new SavingsAccount();
        if (type == "Current") return new CurrentAccount();
        if (type == "FD") {
            double rate;
            cout << "Enter FD interest rate: ";
            cin >> rate;
            return new FixedDepositAccount(rate);
        }
        cout << "Unknown account type" << endl;
        return nullptr;
    }
};

int main(){

    Bank bank(1,"National Bank");

    Branch* b1=new Branch(101,"Delhi Main","NATL0001","Delhi");
    Branch* b2=new Branch(102,"Mumbai Central","NATL0002","Mumbai");

    bank.addBranch(b1);
    bank.addBranch(b2);

    Employee* e1=new Employee(1,"Ramesh","Manager",80000);
    Employee* e2=new Employee(2,"Priya","Cashier",40000);

    b1->addEmployee(e1);
    b1->addEmployee(e2);

    Customer* currentCustomer=nullptr;
    Account* currentAccount=nullptr;
    ATMCard* currentCard=nullptr;

    int choice;

    do{

        cout<<"\n========== NATIONAL BANK MENU ==========\n";
        cout<<"1. Create Customer\n";
        cout<<"2. Create Account\n";
        cout<<"3. Enable SMS Notification\n";
        cout<<"4. Enable Email Notification\n";
        cout<<"5. Deposit\n";
        cout<<"6. Withdraw\n";
        cout<<"7. Show Balance\n";
        cout<<"8. Show Transactions\n";
        cout<<"9. Show Customer Accounts\n";
        cout<<"10. Show Notifications\n";
        cout<<"11. Apply Loan\n";
        cout<<"12. Create ATM Card\n";
        cout<<"13. ATM Withdraw\n";
        cout<<"14. ATM Balance Check\n";
        cout<<"15. Show Branches\n";
        cout<<"16. Show Branch Accounts\n";
        cout<<"17. Find Account\n";
        cout<<"18. Show Employees\n";
        cout<<"19. Show FD Details\n";
        cout<<"0. Exit\n";

        cout<<"Enter choice: ";
        cin>>choice;

        if(choice==1){

            currentCustomer=new Customer();

            cout<<"Customer created successfully.\n";
        }

        else if(choice==2){

            if(currentCustomer==nullptr){
                cout<<"Create customer first.\n";
                continue;
            }

            string type;

            cout<<"Enter account type(Savings/Current/FD): ";
            cin>>type;

            currentAccount=AccountMaker::createAccount(type);

            if(currentAccount!=nullptr){

                currentAccount->setCustomer(currentCustomer);

                currentAccount->setBranch(b1);

                cout<<"Account created successfully.\n";
            }
        }

        else if(choice==3){

            if(currentAccount==nullptr){
                cout<<"Create account first.\n";
                continue;
            }

            currentAccount->enableSMS();

            cout<<"SMS notification enabled.\n";
        }

        else if(choice==4){

            if(currentAccount==nullptr){
                cout<<"Create account first.\n";
                continue;
            }

            currentAccount->enableEmail();

            cout<<"Email notification enabled.\n";
        }

        else if(choice==5){

            if(currentAccount==nullptr){
                cout<<"Create account first.\n";
                continue;
            }

            double amt;

            cout<<"Enter amount: ";
            cin>>amt;

            currentAccount->deposit(amt);
        }

        else if(choice==6){

            if(currentAccount==nullptr){
                cout<<"Create account first.\n";
                continue;
            }

            double amt;

            cout<<"Enter amount: ";
            cin>>amt;

            currentAccount->withdraw(amt);
        }

        else if(choice==7){

            if(currentAccount==nullptr){
                cout<<"Create account first.\n";
                continue;
            }

            currentAccount->showBalance();
        }

        else if(choice==8){

            if(currentAccount==nullptr){
                cout<<"Create account first.\n";
                continue;
            }

            currentAccount->showTransactions();
        }

        else if(choice==9){

            if(currentCustomer==nullptr){
                cout<<"Create customer first.\n";
                continue;
            }

            currentCustomer->showAccounts();
        }

        else if(choice==10){

            if(currentCustomer==nullptr){
                cout<<"Create customer first.\n";
                continue;
            }

            currentCustomer->displayNotifications();
        }

        else if(choice==11){

            if(currentCustomer==nullptr){
                cout<<"Create customer first.\n";
                continue;
            }

            int id;
            int yrs;

            string type;

            double amt;
            double interest;

            cout<<"Enter loan id: ";
            cin>>id;

            cout<<"Enter loan type: ";
            cin>>type;

            cout<<"Enter loan amount: ";
            cin>>amt;

            cout<<"Enter interest rate: ";
            cin>>interest;

            cout<<"Enter tenure(years): ";
            cin>>yrs;

            Loan* l=new Loan(currentCustomer,id,type,amt,interest,yrs);

            l->showLoan();
        }

        else if(choice==12){

            if(currentAccount==nullptr){
                cout<<"Create account first.\n";
                continue;
            }

            long cno;

            int cvv;
            int pin;

            string type;

            cout<<"Enter card number: ";
            cin>>cno;

            cout<<"Enter cvv: ";
            cin>>cvv;

            cout<<"Enter pin: ";
            cin>>pin;

            cout<<"Enter card type: ";
            cin>>type;

            currentCard=new ATMCard(cno,cvv,pin,type);

            currentCard->setAccount(currentAccount);

            cout<<"ATM card created successfully.\n";
        }

        else if(choice==13){

            if(currentCard==nullptr){
                cout<<"Create ATM card first.\n";
                continue;
            }

            int pin;
            double amt;

            cout<<"Enter pin: ";
            cin>>pin;

            cout<<"Enter amount: ";
            cin>>amt;

            currentCard->withdrawCash(pin,amt);
        }

        else if(choice==14){

            if(currentCard==nullptr){
                cout<<"Create ATM card first.\n";
                continue;
            }

            int pin;

            cout<<"Enter pin: ";
            cin>>pin;

            currentCard->checkBalance(pin);
        }

        else if(choice==15){

            bank.showBranches();
        }

        else if(choice==16){

            b1->showAccounts();
        }

        else if(choice==17){

            long accNo;

            cout<<"Enter account number: ";
            cin>>accNo;

            b1->findAccount(accNo);
        }

        else if(choice==18){

            e1->showEmployee();

            cout<<endl;

            e2->showEmployee();
        }

        else if(choice==19){

            if(currentAccount==nullptr){
                cout<<"Create account first.\n";
                continue;
            }

            if(currentAccount->getAccountType()=="Fixed Deposit"){

                FixedDepositAccount* fd=
                dynamic_cast<FixedDepositAccount*>(currentAccount);

                if(fd!=nullptr){
                    fd->showMaturityDetails();
                }
            }

            else{
                cout<<"Current account is not FD.\n";
            }
        }

        else if(choice!=0){

            cout<<"Invalid choice.\n";
        }

    }while(choice!=0);

    cout<<"Thank you for using National Bank.\n";

    return 0;
}