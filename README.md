# 🏦 Bank Management System V2 — With Permissions

A **C++ Console-Based Bank Management System** designed to manage client accounts, financial transactions, and user access through a permission-based authorization system.

This version focuses on **user management, granular permissions, secure operation checks, and persistent file storage**, making it more than a basic client management application.

---

## 📌 Table of Contents

- [🚀 Overview](#-overview)
- [✨ Features](#-features)
- [🔐 Authentication and Authorization](#-authentication-and-authorization)
- [🧮 Permission System](#-permission-system)
- [👥 Client Management](#-client-management)
- [💰 Financial Transactions](#-financial-transactions)
- [👤 User Management](#-user-management)
- [📂 Data Storage](#-data-storage)
- [🛠️ Technologies Used](#️-technologies-used)
- [📁 Project Structure](#-project-structure)
- [▶️ Getting Started](#️-getting-started)
- [🔑 Initial Admin Account](#-initial-admin-account)
- [⚠️ Security Notes](#️-security-notes)
- [🔮 Future Improvements](#-future-improvements)
- [👩‍💻 Author](#-author)

---

## 🚀 Overview

The Bank Management System V2 is a console application developed using **C++** to simulate essential banking operations and demonstrate practical programming concepts.

Unlike a basic client management program, this version includes a dedicated **user authentication and permission system** that controls which operations each user can perform.

The application supports:

- Managing client records.
- Processing deposits and withdrawals.
- Calculating total client balances.
- Creating and managing system users.
- Assigning full or customized permissions.
- Saving and retrieving data using text files.

The project emphasizes structured programming, reusable functions, input validation, file handling, and bitwise operations.

## ✨ Features

### 🏦 Client Management

- 📋 Display all client records in a formatted table.
- ➕ Add new client accounts.
- ✏️ Update existing client information.
- 🗑️ Delete client accounts after confirmation.
- 🔎 Search for a client by account number.
- 🚫 Prevent duplicate account numbers.
- 💵 Validate non-negative opening balances.

### 💰 Financial Transactions

- 📥 Deposit money into a client account.
- 📤 Withdraw money from a client account.
- 🛑 Prevent withdrawals that exceed the available balance.
- ✅ Require confirmation before processing transactions.
- 💳 Display the updated balance after a successful transaction.
- 📊 Calculate and display the total balances of all clients.

### 👥 User Management

- 📋 List registered system users.
- ➕ Add new users.
- ✏️ Update user passwords and permissions.
- 🗑️ Delete users after confirmation.
- 🔍 Find users by username.
- 🚫 Prevent duplicate usernames.
- 🛡️ Prevent users from deleting their own currently logged-in account.
- 🔑 Assign full access or individual permissions.

### 🧰 Input Validation and User Experience

- Validate menu choices and reject out-of-range input.
- Handle invalid numeric input.
- Validate transaction amounts.
- Display clear success and error messages.
- Ask for confirmation before destructive operations.
- Provide separate menus for banking transactions and user management.

---

## 🔐 Authentication and Authorization

The application implements a login system that validates the entered username and password against the stored user records.

After successful authentication, the logged-in user's information is stored in the `CurrentUser` variable and used to determine access rights.

Before performing protected operations, the application checks the current user's permissions.

If the user lacks the required permission, the operation is denied and an access-denied message is displayed.

**Authentication** answers: *Who is logging in?*

**Authorization** answers: *What is this user allowed to do?*

## 🧮 Permission System

One of the main features of this version is its use of **bitwise operators to manage individual permissions**.

Each permission is represented by a different bit flag:

| Permission | Value | Description |
|---|---:|---|
| `pListClients` | `1` | Display the client list |
| `pAddNewClient` | `2` | Add new clients |
| `pDeleteClient` | `4` | Delete clients |
| `pUpdateClient` | `8` | Update client information |
| `pFindClient` | `16` | Search for clients |
| `pTransactions` | `32` | Access financial transactions |
| `pManageUsers` | `64` | Manage system users |
| `eAll` | `-1` | Full access |

### ⚙️ How It Works

The application uses bitwise OR (`|`) to combine permissions and bitwise AND (`&`) to check whether a user has a required permission.

For example:

```cpp
int Permissions = 0;

Permissions |= pListClients;
Permissions |= pTransactions;
```

This grants permission to view clients and perform transactions.

The access check is implemented as follows:

```cpp
bool CheckAccessPermission(int Permission)
{
    if (CurrentUser.Permission == eAll)
        return true;

    return (CurrentUser.Permission & Permission) == Permission;
}
```

This design allows each user to receive a customized set of permissions without requiring a separate Boolean variable for every permission.

The `RequirePermission()` function checks access before protected operations and displays an error message when permission is denied.

### 🛡️ Full Access or Customized Access

When creating or updating a user, the administrator can choose between:

- **Full Access:** Grants all permissions.
- **Customized Access:** Allows individual permissions to be selected independently.

This demonstrates how bit flags can be used to implement a lightweight permission system in C++.

---

## 👥 Client Management

Client records contain the following information:

- Account Number
- Full Name
- PIN Code
- Phone Number
- Account Balance

The application loads client records into a `vector<stClient>` when performing operations.

Client lookup is performed using the account number, and changes are persisted to `Clients.txt`.

## 💰 Financial Transactions

The application supports deposits and withdrawals through a dedicated Transactions Menu.

### Deposit

1. Find the client by account number.
2. Display the client's current information.
3. Enter a transaction amount.
4. Validate the amount.
5. Confirm the operation.
6. Save the updated balance.

### Withdrawal

1. Find the client by account number.
2. Display the current balance.
3. Enter a withdrawal amount.
4. Validate the amount.
5. Ensure the withdrawal does not exceed the available balance.
6. Confirm the operation and save the new balance.

### Total Balances

The application displays each client's account number, name, and balance, then calculates the combined balance of all client accounts.

---

## 👤 User Management

User records contain:

- Username
- Password
- Permission flags

The user management menu provides operations for listing, adding, updating, deleting, and searching users.

For privacy in console output, passwords are displayed as asterisks when showing user records. However, passwords are currently stored as plain text in the data file; see the security notes below.

The application also prevents deletion of the currently logged-in user to avoid removing the active account during a session.

## 📂 Data Storage

This project uses **text-file storage** rather than a database.

| File | Purpose |
|---|---|
| `Clients.txt` | Stores client account records |
| `Users.txt` | Stores user credentials and permission values |

Records use the custom separator:

```text
#//
```

More precisely, the separator used by the application is:

```text
#//
```

The application reads and parses records using `SplitString()`, converts them into structures, and writes updates back to the files.

The project uses `ifstream` for reading and `ofstream` for writing and appending records.

### 💾 Data Persistence

Data remains available between application runs because client and user records are stored in files on disk.

The program also checks for invalid records and skips records that cannot be parsed successfully.

---

## 🛠️ Technologies Used

- **C++** — Core application logic.
- **Standard Template Library (STL)** — `vector` and `string`.
- **File Handling** — `ifstream` and `ofstream`.
- **Bitwise Operators** — Permission flags and access checks.
- **Structures and Enumerations** — Organized data models and menu options.
- **Input Validation** — Reliable handling of user input.
- **Visual Studio** — Project development and execution.

## 📁 Project Structure

```text
BankManagementSystemV2-With-Permissions/
│
├── BankManagementSystemV2.cpp
├── BankManagementSystemV2.sln
├── BankManagementSystemV2.vcxproj
├── BankManagementSystemV2.vcxproj.filters
├── Clients.txt
├── Users.txt
├── .gitignore
└── README.md
```

## ▶️ Getting Started

### Prerequisites

- Windows.
- Visual Studio with the C++ desktop development workload.
- A compatible C++ compiler.

### Run Using Visual Studio

1. Clone or download the repository.
2. Open `BankManagementSystemV2.sln` in Visual Studio.
3. Select a suitable build configuration, such as `Debug` or `Release`.
4. Build the solution.
5. Run the application.

### Clone the Repository

```bash
git clone https://github.com/Sarah-Nabil/BankManagementSystemV2-With-Permissions.git
```

Navigate to the project directory and open the solution file in Visual Studio.

## 🔑 Initial Admin Account

When `Users.txt` contains no valid user records, the application creates an initial administrator account.

| Field | Value |
|---|---|
| Username | `admin` |
| Password | `1234` |
| Permissions | Full access |

**Important:** These are default learning-project credentials. Change them before using the application with any sensitive data.

## ⚠️ Security Notes

This project is intended for **educational purposes** and is not suitable for production banking.

Current limitations include:

- Passwords are stored as plain text in `Users.txt`.
- Client PIN codes are stored as plain text in `Clients.txt`.
- The default administrator credentials are predictable.
- File-based storage does not provide database transactions or concurrent-access protection.
- Permission checks are implemented within the application and are not a substitute for production-grade security controls.
- Financial operations are simulated and do not connect to real banking infrastructure.

The project demonstrates permission-based access control concepts, but it should not be used to store real financial information or real credentials.

## 🔮 Future Improvements

Potential enhancements include:

- 🔒 Hash passwords using a suitable password-hashing algorithm.
- 🗄️ Replace text files with a database.
- 🧾 Introduce transaction history and audit logs.
- 🧱 Refactor the application into separate modules and classes.
- 🧪 Add automated unit tests.
- 💱 Add transfer operations between client accounts.
- 📅 Add transaction timestamps and reports.
- 🛡️ Strengthen authorization and input validation.

## 👩‍💻 Author

**Sarah Nabil**

GitHub: [Sarah-Nabil](https://github.com/Sarah-Nabil)

### ⭐ Support

If you find this project useful for learning C++ programming, file handling, or bitwise permission systems, feel free to explore the repository and give it a star!

---

*Built as a learning project to practice C++, file handling, and permission-based access control.*
