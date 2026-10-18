🏦 Bank Management System V2 — With Permissions
A C++ console application for managing bank clients, basic transactions, and users with permission-based access control. Client and user records are stored in local text files, so no database server is required.
✨ Features

Client Management
- Add, view, search, update, and delete clients.
- Prevent duplicate account numbers.
- Store account number, name, PIN, phone, and balance.
- 
Banking Transactions
- Deposit and withdraw money.
- Check client balances and calculate the total balance.
- Confirm transactions and validate amounts.
- Prevent withdrawals that exceed the available balance.
- 
User Management & Permissions
- Add, list, search, update, and delete users.
- Assign full access or specific permissions.
- Hide passwords in user-list displays.
- Prevent deleting the currently logged-in user.
  
Permission	Value	Access
pListClients	1	View clients
pAddNewClient	2	Add clients
pDeleteClient	4	Delete clients
pUpdateClient	8	Update clients
pFindClient	16	Find clients
pTransactions	32	Deposits, withdrawals, and balances
pManageUsers	64	Manage users
eAll	-1	Full access


Permission values are bit flags and can be combined to grant multiple permissions to a user.
🛠️ Technologies & Concepts
- Language: C++
- Application: Console application
- Concepts: struct, enum, functions, vectors, file handling (ifstream / ofstream), string parsing, input validation, and bitwise operations.
  
📁 Data Storage

The application uses two text files in its working directory:
- Clients.txt — AccountNumber#//#Name#//#PinCode#//#Phone#//#AccountBalance
- Users.txt — UserName#//#Password#//#Permission
  
The delimiter is #//#; keep the field order unchanged unless you also update the parsing and saving code. If Users.txt has no valid user records, the application attempts to create a default administrator account.
🚀 Getting Started
1. Open the project in Visual Studio or another C++ development environment.
2. Build the solution (Ctrl + Shift + B in Visual Studio).
3. Run without debugging (Ctrl + F5).
4. If the default administrator was created, sign in with:
   - Username: admin
   - Password: 1234
   - Permissions: Full access
The text files are created relative to the program's working directory, which may differ from the source-code folder.
🔒 Security Note
This is an educational project, not a production banking system. Passwords and client information are stored in plain text, so do not use real customer or banking data. A production system should use secure password hashing, protected storage, a database, and appropriate auditing.
Built to practice C++ fundamentals, file handling, CRUD operations, and permission-based access control.
