# Induction-Y25

Make a fork of this repo and create a branch in the fork with the following name: `[firstname]_[lastname_initial]`
Example: `vivek_m` , `soumya_j`

## Task 1: GitHub

Hello Y25s!

If you are new to version control, definitely start with this video:
[Complete Git and GitHub Tutorial for Beginners](https://youtu.be/RDxQEzXN8AU?si=pI4vuro-mP_JI9Ep)

For those of you who are already familiar with Git and GitHub, check out this game:
[Learn Git Branching](https://learngitbranching.js.org/)

Please go through these resources. If you have any doubts or get stuck at any point, feel free to reach out to any of the Y24s.

### Submission

**Form link:** [Submission Form](https://docs.google.com/forms/d/e/1FAIpQLScUVQPhsgVe0gdkjR8wKvAXFTa7bDcjEgVHHpi9sFcYN2RRAA/viewform?usp=publish-editor)

Complete the GitHub game by 7th April EOD and fill out the form. Make sure you do this, as you all will be assessed based on these tasks moving forward.

---

## Task 2: Command Line (Bandit)

Hey Y25s! Hope you all did well in the GitHub assignment.
Next up: getting comfortable with the command line. Your next task is to solve the Bandit wargames: [Bandit Wargame](https://overthewire.org/wargames/bandit/)
Linux is very important in our work, so please take this seriously and work through it honestly.

### Requirements

*   Solve **30 levels** from this game.
*   Create **notes for each level** describing what you did and how you arrived at the solution/command (we may ask you about it as well!!).

### Submission

Please open your **Pull Requests (PRs)** by the **14th April EOD**.(Hope you have done the GitHub well !!)

## Task 3 : SmartBank Enterprise System

Hey Y25s ! Hope you all did well in the OvertheWire assignment . Next Up : Working with real life applications of OOPS(Object Oriented Programming)

## Project Overview
The goal of this project is to build a modular banking system where:

1.Customers can manage accounts
2.Transactions can be performed securely
3.Loans and ATM services can be handled
4.Banking operations are modeled using proper class relationship
##  Features

- **Multi-Level Hierarchy:** Manage multiple branches under a single Bank entity.
- **Diverse Account Types:** Supports Savings, Current, and Fixed Deposit accounts with specific business logic (interest rates, overdrafts, maturity).
- **Transaction Engine:** Securely handle deposits, withdrawals, and transfers between accounts with history tracking.
- **Loan Management:** Process various loan types (Home, Car, Personal) with EMI calculations.
- **ATM Services:** Integrated ATM card management with status tracking and PIN security.
- **Notification System:** Polymorphic notification implementation supporting both SMS and Email alerts.

##  System Architecture (UML)

The system is designed with a strong focus on class relationships:

- **Inheritance:** `Account` is an abstract base class for `SavingsAccount`, `CurrentAccount`, and `FixedDepositAccount`.
- **Interfaces:** `Notification` is implemented by `SMSNotification` and `EmailNotification`.
- **Associations:** 
    - A `Customer` can own multiple `Accounts`.
    - A `Branch` manages multiple `Employees` and `Accounts`.
    - `Transactions` link a sender and a receiver `Account`.

##  Class Structure

### Core Entities
| Class | Responsibility |
| :--- | :--- |
| **Bank** | Root entity managing branches, customers, and overall organization. |
| **Branch** | Local office handling specific accounts and staff. |
| **Customer** | Stores personal details and linked financial products. |
| **Employee** | Represents staff members with specific designations (Manager, Cashier). |

### Financial Products
- **Savings Account:** Features daily withdrawal limits and minimum balance requirements.
- **Current Account:** Supports overdraft limits and business-specific charges.
- **Fixed Deposit:** Tracks tenure, interest rates, and maturity amounts.
- **Loan:** Manages principal, interest, and EMI status.
- **ATM Card:** Handles card security, expiry, and status (Active/Blocked).

### Main Classes and Attributes
Main Classes and Attributes



### 1. Bank Class
Represents the complete banking organization.

| Attribute | Type | Description |
|---|---|---|
| bankId | int | Unique bank ID |
| bankName | String | Name of bank |
| branches | List<Branch> | All branches |
| customers | List<Customer> | Registered customers |
| employees | List<Employee> | Bank staff |

---

### 2. Branch Class
Represents a bank branch.

| Attribute | Type | Description |
|---|---|---|
| branchId | int | Unique branch ID |
| branchName | String | Branch name |
| IFSCCode | String | Branch IFSC |
| address | String | Branch address |
| accounts | List<Account> | Accounts handled |
| employees | List<Employee> | Staff members |

---

### 3. Customer Class
Stores customer information.

| Attribute | Type | Description |
|---|---|---|
| customerId | int | Unique customer ID |
| fullName | String | Customer name |
| dob | Date | Date of birth |
| gender | String | Gender |
| mobileNumber | String | Contact number |
| email | String | Email |
| address | String | Residential address |
| aadhaarNumber | String | National ID |
| PANNumber | String | Tax ID |
| accounts | List<Account> | Customer accounts |
| loans | List<Loan> | Customer loans |

---

### 4. Abstract Account Class
Base class for all account types.

| Attribute | Type | Description |
|---|---|---|
| accountNumber | long | Unique account number |
| accountType | String | Savings/current |
| balance | double | Current balance |
| dateOpened | Date | Opening date |
| status | String | Active/blocked |
| branch | Branch | Linked branch |
| customer | Customer | Owner |
| transactions | List<Transaction> | Transaction history |

### 5. SavingsAccount Class
Inherits from Account.
| Attribute|Type| Description|
|---|---|---|
| interestRate   | double | Interest percentage      |
| minimumBalance | double | Minimum balance required |


### 6. CurrentAccount Class
Inherits from Account.
|Attribute|Type|Description|
|---|---|---|
| overdraftLimit | double | Allowed overdraft    |
| businessName   | String | Business holder name |

### 7.FixedDepositAccount Class
Inherits from Account.
| Attribute|Type|Description|
|---|---|---|
| FDAmount       | double | Deposit amount   |
| maturityDate   | Date   | Maturity date    |
| FDInterestRate | double | FD interest      |
| tenureMonths   | int    | Deposit duration |

### 8.Transaction Class
Represents all banking transactions.
|Attribute|Type|Description|
| --------------- | ------- | --------------------- |
| transactionId   | int     | Unique transaction ID |
| transactionType | String  | Deposit/withdraw      |
| amount          | double  | Transaction amount    |
| transactionDate | Date    | Date/time             |
| senderAccount   | Account | Sender                |
| receiverAccount | Account | Receiver              |
| status          | String  | Success/pending       |

### 9.Loan Class
Represents customer loans.
|Attribute|Type|Description|
|---|---|---|
| loanId       | int      | Unique loan ID    |
| loanType     | String   | Home/car/personal |
| loanAmount   | double   | Principal amount  |
| interestRate | double   | Loan interest     |
| tenureYears  | int      | Duration          |
| EMIAmount    | double   | Monthly EMI       |
| loanStatus   | String   | Approved/pending  |
| customer     | Customer | Loan owner        |

### 10.ATM Card Class
Represents ATM/debit cards.
|Attribute|Type|Description|
|---|---|---|
| cardNumber    | long    | ATM card number    |
| CVV           | int     | Security code      |
| expiryDate    | Date    | Card expiry        |
| PIN           | int     | ATM PIN            |
| cardType      | String  | Debit/Credit       |
| cardStatus    | String  | Active/blocked     |
| linkedAccount | Account | Associated account |

### 11.Employee Class
Represents bank employees.
|Attribute|Type|Description|
|---|---|---|
| employeeId   | int    | Employee ID     |
| employeeName | String | Employee name   |
| designation  | String | Manager/cashier |
| salary       | double | Monthly salary  |
| branch       | Branch | Assigned branch |

### 12. Notification Class
Used for polymorphism.
| Attribute | Type   | Description          |
| --------- | ------ | -------------------- |
| message   | String | Notification content |


### 13.  SMSNotification Class
Implements Notification interface.
| Attribute|Type| Description|
|---|---|---|
| phoneNumber    | String | Recipient number |
| message        | String | SMS content      |
| deliveryStatus | String | Sent/pending     |

### 14. EmailNotification Class
Implements Notification interface.
|Attribute|Type|Description|
|---|---|---|
| emailAddress   | String | Recipient email |
| subject        | String | Email subject   |
| message        | String | Email body      |
| deliveryStatus | String | Delivery status |

## Exception Handling

The system includes custom exceptions to ensure data integrity and security:
- `InsufficientBalanceException`
- `InvalidPINException`
- `AccountBlockedException`
- `LoanRejectedException`

##  Technical Details
- **Language:** C++
- **Design Pattern:** Strategy Pattern (for Notifications), Factory Pattern (for Account Creation).

##  Submission
Fork the repository and submit your PR(Pull Requests by 19th May, EOD) .

## Task 4: Assignment 4.1 (Dockerize OOPS Assignment)

Hey Y25s! Now that you have built the SmartBank Enterprise System, it's time to learn how to package and distribute your applications. Your next task is to learn **Docker** and containerize your OOPS assignment.

Docker allows developers to package applications with all of their dependencies into a single, standardized unit called a container. This ensures that your code runs exactly the same way on any machine—no more "it works on my machine!" excuses.

### Requirements

*   **Dockerize** your SmartBank Enterprise System.
*   Create a `Dockerfile` in the root of your project directory.
*   The `Dockerfile` must:
    1. Use an appropriate base image (e.g., `ubuntu`, `gcc`, or `alpine`).
    2. Copy your C++ source code into the container.
    3. Compile your C++ code during the image build process.
    4. Define the command to run your compiled executable when the container starts.
*   Ensure that running `docker build -t smartbank .` and `docker run -it smartbank` successfully launches your banking system.

### Frequently Asked Questions (FAQs)

**Q: I've never used Docker before. Where do I start?**
A: Start by reading the official [Docker documentation](https://docs.docker.com/get-started/) or checking out introductory tutorials on YouTube that were sent on Discord channel. Learn about what a `Dockerfile` is, and the basic commands: `docker build`, `docker images`, `docker run`, and `docker ps`.

**Q: Do I need to change my C++ code?**
A: No! You should not have to change the logic of your SmartBank application. You are simply writing a configuration file (`Dockerfile`) that tells Docker how to compile and run your existing code in an isolated environment.

**Q: How do I handle multiple `.cpp` and `.h` files in the Dockerfile?**
A: You can copy your project files into the container (using `COPY`), and then run your usual compilation command (like `g++ *.cpp -o bank`) using the `RUN` directive.

**Q: How do I interact with the program if it takes user input?**
A: When you run the container, make sure to use the `-it` flags (e.g., `docker run -it smartbank`). This runs the container interactively, allowing you to provide inputs to the terminal.

### Submission

Please push your `Dockerfile` (and any other necessary build scripts) to your branch and open your **Pull Requests (PRs)** by the **25th May EOD**.

