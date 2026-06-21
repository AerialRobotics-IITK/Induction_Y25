# SmartBank Enterprise System

A comprehensive Bank Management System implemented in C++.

## Overview

The SmartBank Enterprise System is a consolidated C++ application that manages various banking operations including:
- Customer Registration
- Account Management (Savings, Current, Fixed Deposit)
- Transactions (Deposit, Withdrawal, Transfer)
- Loan Processing
- ATM Card Generation
- Notifications (SMS/Email)

## File Structure

- `SmartBank.cpp`: The single source file containing all class definitions and logic.
- `README.md`: Project documentation.

## How to Compile and Run

To compile the project, use a C++ compiler like `g++`:

```bash
g++ -o SmartBank SmartBank.cpp
```

To run the application:

```bash
./SmartBank
```

## Features

- **Object-Oriented Design**: Utilizes classes, inheritance, and polymorphism.
- **Exception Handling**: Robust error handling for banking logic.
- **Factory Pattern**: Used for account creation.
- **Strategy Pattern**: Used for notification systems.
