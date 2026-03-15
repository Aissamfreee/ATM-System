# 🏧 ATM System (C++)

A console-based ATM simulation built in C++ that allows 
clients to perform basic banking operations.

## ✨ Features

- 🔐 Secure login with Account Number & PIN Code
- ⚡ Quick Withdraw (preset amounts: 20, 50, 100, 200, 400, 600, 800, 1000)
- 💵 Normal Withdraw (any amount, must be multiple of 5)
- 💰 Deposit
- 📊 Check Balance
- 🚪 Logout

## 🛠️ Technologies

- Language : C++
- Storage  : Text file (Clients.txt)
- Concepts : Structs, Enums, Vectors, File I/O

## 🚀 How to Run

1. Clone the repository
   git clone https://github.com/YOUR_USERNAME/ATM-System.git

2. Compile
   g++ main.cpp -o ATM

3. Run
   ./ATM

> ⚠️ Make sure Clients.txt exists in the same directory.

## 📁 Clients.txt Format

AccountNumber#//#PinCode#//#Name#//#Phone#//#Balance

Example:
A100#//#1234#//#Ahmed Ali#//#0501234567#//#5000

## 📌 Notes

- Balances are saved automatically after every transaction
- Invalid login attempts loop until correct credentials are entered
