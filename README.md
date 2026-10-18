# 🏦 Bank Management System V2 — With Permissions

A console-based Bank Management System built with C++. The project simulates everyday banking administration tasks, including client records, deposits and withdrawals, user management, and permission-based access control. Data is stored locally in text files, so the application does not require a database server.
🎯 Project goal: Practice core C++ programming concepts by building a menu-driven application with file handling, structured records, CRUD operations, input validation, and role/permission checks.

✨ Features
👥 Client Management
- 📋 Display a formatted list of clients.
- ➕ Add a new client.
- 🔎 Find a client using their account number.
- ✏️ Update client details.
- 🗑️ Delete a client after confirmation.
- 🚫 Prevent duplicate account numbers.
- 💾 Save client records to a local text file.
💳 Banking Transactions
- 💰 Deposit money into a client account.
- 💸 Withdraw money from a client account.
- 🧮 Display every client's balance and calculate the total balance across accounts.
- ✅ Ask for confirmation before completing a transaction.
- 🛡️ Validate transaction amounts and prevent withdrawals greater than the available balance.
🔐 User Management
- 📋 List registered users.
- ➕ Add new users.
- 🔎 Find users by username.
- ✏️ Update a user's password and permissions.
- 🗑️ Delete users after confirmation.
- 🚫 Prevent duplicate usernames.
- 🔒 Hide passwords in user-list displays.
- 🛑 Prevent deleting the currently logged-in user.
🧩 Permission-Based Access Control
Users can be assigned full access or selected permissions. The application checks permissions before protected operations.
Permission	Value	Allows the user to
pListClients	1	View the client list
pAddNewClient	2	Add clients
pDeleteClient	4	Delete clients
pUpdateClient	8	Update clients
pFindClient	16	Find clients
pTransactions	32	Access deposits, withdrawals, and total balances
pManageUsers	64	Manage users
eAll	-1	Full access


The permission values are bit flags. They can be combined to grant a user more than one permission.
✅ Input and File Handling
- Checks menu choices and re-prompts when a choice is invalid.
- Validates that entered amounts are non-negative and that transaction amounts are greater than zero.
- Handles file-opening and file-writing failures with error messages.
- Skips malformed records when loading client or user data.
- Uses reusable helper functions for reading input, splitting records, converting records, and saving data.
🛠️ Technologies and Concepts
- Language: C++
- Application type: Console application
- Standard libraries: <iostream>, <string>, <vector>, <fstream>, <iomanip>, <limits>, and <cstdlib>
- Core concepts practiced:
  - Structures (struct)
  - Enumerations (enum)
  - Functions and reusable helper methods
  - Vectors and record searching
  - File input/output (ifstream, ofstream)
  - String parsing and record serialization
  - Bitwise operations for permissions
  - Input validation and exception handling
  - Menu-driven program flow
📁 Data Files
The application uses two text files in its current working directory:
Clients.txt
Stores client data in this field order:
AccountNumber#//#Name#//#PinCode#//#Phone#//#AccountBalance
Example record:
1001#//#Ahmed Ali#//#1234#//#01000000000#//#2500.000000
Users.txt
Stores user data in this field order:
UserName#//#Password#//#Permission
Example record:
admin#//#1234#//#-1
The delimiter used between fields is #//#. Do not change the delimiter or field order unless you also update the parsing and saving functions.
💡 If Users.txt has no valid user records, the application attempts to create an initial administrator account automatically.

🚀 Getting Started
Requirements
- A C++ compiler.
- Visual Studio on Windows, or another C++ development environment that supports standard C++.
Run with Visual Studio
1. Open the project in Visual Studio.
2. Make sure the source file is included in the project.
3. Build the solution using Build → Build Solution or press Ctrl + Shift + B.
4. Run without debugging using Ctrl + F5.
5. On the first run, if no valid user is found, the application creates the default administrator account described below.
Initial Administrator Account
When the user file is empty or contains no valid user records, the program attempts to create:
- Username: admin
- Password: 1234
- Permissions: Full access
After launching, sign in with those credentials if the initial account was created successfully. The files are created relative to the program's working directory, which may differ from the folder containing the .cpp source file. In Visual Studio, check the project's debugging/working-directory settings if you cannot find the text files.
🧭 How to Use the Application
After signing in, use the main menu to choose an operation:
1. Show client list — view saved client records.
2. Add new client — enter a unique account number and the client's details.
3. Delete client — find a client and confirm deletion.
4. Update client — edit the selected client's information.
5. Find client — search by account number.
6. Transactions — choose deposit, withdrawal, or total balances.
7. Manage users — list, add, delete, update, or find users if your account has permission.
8. Logout — return to the login flow.
Menu options and protected actions are checked against the current user's permissions.
🔒 Security Notes
This is an educational project, not a production banking system.
- ⚠️ Passwords are stored as plain text in Users.txt; they are not hashed or encrypted.
- ⚠️ The default administrator credentials are predictable. Change them before sharing a working copy, and do not use them for real accounts.
- ⚠️ PIN codes and financial data are stored in local text files without encryption.
- ⚠️ Permission checks in a console application are a learning demonstration, not a substitute for production-grade authentication, secure storage, auditing, or database access controls.
- ✅ Do not store real customer or banking information in this project.
For a production application, use a database, password hashing, stronger authentication, secure secret management, transaction integrity, and appropriate audit logging.
🗺️ Possible Future Improvements
- 🗄️ Replace text-file storage with SQL Server or another database.
- 🔐 Hash passwords using a suitable password-hashing algorithm.
- 🧾 Add transaction history and audit logs.
- 🧪 Add unit tests for permission checks, record parsing, and transaction validation.
- 📊 Add reports and date-based transaction summaries.
- 🧰 Separate the code into modules or classes to improve maintainability.
- 💱 Add currency support and account statements.
👩‍💻 About the Project
Bank Management System — C++ is a learning project focused on practicing C++ fundamentals through a small, interactive banking workflow. It combines client CRUD operations, transaction handling, local file persistence, and permission-controlled user features in one console application.
📄 License
No license has been specified for this project yet. Add a license file if you intend to define how others may use, modify, or distribute the code.
If you find this project useful for learning C++ programming, file handling, or bitwise permission systems, feel free to explore the repository and give it a star!

---

*Built as a learning project to practice C++, file handling, and permission-based access control.*
