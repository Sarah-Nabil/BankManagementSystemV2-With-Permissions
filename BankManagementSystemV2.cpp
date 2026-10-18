#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cstdlib>

using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";
const string RecordSeparator = "#//#";

enum enMainMenuOptions
{
    eListClients = 1,
    eAddNewClient,
    eDeleteClient,
    eUpdateClient,
    eFindClient,
    eTransactions,
    eManageUsers,
    eLogout
};

enum enTransactionsOptions
{
    eDeposit = 1,
    eWithdraw,
    eShowTotalBalance,
    eTransactionsMainMenu
};

enum enManageUsersOptions
{
    eListUsers = 1,
    eAddNewUser,
    eDeleteUser,
    eUpdateUser,
    eFindUser,
    eUsersMainMenu
};

enum enPermissions
{
    eAll = -1,
    pListClients = 1,
    pAddNewClient = 2,
    pDeleteClient = 4,
    pUpdateClient = 8,
    pFindClient = 16,
    pTransactions = 32,
    pManageUsers = 64
};

struct stClient
{
    string AccountNumber;
    string Name;
    string PinCode;
    string Phone;
    double AccountBalance = 0.0;
    bool MarkForDelete = false;
};

struct stUser
{
    string UserName;
    string Password;
    int Permission = 0;
    bool MarkForDelete = false;
};

stUser CurrentUser;

// -------------------- General input helpers --------------------

void ClearInputLine()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string ReadText(const string& Prompt)
{
    string Value;
    cout << Prompt;
    getline(cin >> ws, Value);
    return Value;
}

string ReadAccountNumber()
{
    return ReadText("Please enter Account Number: ");
}

string ReadUserName()
{
    return ReadText("Please enter User Name: ");
}

int ReadChoice(const string& Prompt, int MinValue, int MaxValue)
{
    int Choice;
    while (true)
    {
        cout << Prompt;
        if (cin >> Choice && Choice >= MinValue && Choice <= MaxValue)
            return Choice;

        cout << "Invalid choice. Please enter a number from "
            << MinValue << " to " << MaxValue << ".\n";
        ClearInputLine();
    }
}

double ReadNonNegativeAmount(const string& Prompt)
{
    double Amount;
    while (true)
    {
        cout << Prompt;
        if (cin >> Amount && Amount >= 0.0)
            return Amount;

        cout << "Invalid amount. Please enter a non-negative number.\n";
        ClearInputLine();
    }
}

bool Confirm(const string& Prompt)
{
    char Answer;
    cout << Prompt << " (y/n): ";
    cin >> Answer;
    return Answer == 'y' || Answer == 'Y';
}

void Pause()
{
#ifdef _WIN32
    system("pause");
#else
    cout << "\nPress Enter to continue...";
    string Dummy;
    getline(cin >> ws, Dummy);
#endif
}

void ClearScreen()
{
    // Keep the console output portable when running outside Windows.
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// -------------------- String and record conversion --------------------

vector<string> SplitString(const string& Line, const string& Delimiter = RecordSeparator)
{
    vector<string> Parts;
    size_t Start = 0;
    size_t Position;

    while ((Position = Line.find(Delimiter, Start)) != string::npos)
    {
        Parts.push_back(Line.substr(Start, Position - Start));
        Start = Position + Delimiter.length();
    }

    Parts.push_back(Line.substr(Start));
    return Parts;
}

bool ConvertLineToClient(const string& Line, stClient& Client)
{
    vector<string> Fields = SplitString(Line);
    if (Fields.size() != 5)
        return false;

    try
    {
        Client.AccountNumber = Fields[0];
        Client.Name = Fields[1];
        Client.PinCode = Fields[2];
        Client.Phone = Fields[3];
        Client.AccountBalance = stod(Fields[4]);
        Client.MarkForDelete = false;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool ConvertLineToUser(const string& Line, stUser& User)
{
    vector<string> Fields = SplitString(Line);
    if (Fields.size() != 3)
        return false;

    try
    {
        User.UserName = Fields[0];
        User.Password = Fields[1];
        User.Permission = stoi(Fields[2]);
        User.MarkForDelete = false;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

string ConvertClientToLine(const stClient& Client)
{
    return Client.AccountNumber + RecordSeparator + Client.Name + RecordSeparator +
        Client.PinCode + RecordSeparator + Client.Phone + RecordSeparator +
        to_string(Client.AccountBalance);
}

string ConvertUserToLine(const stUser& User)
{
    return User.UserName + RecordSeparator + User.Password + RecordSeparator +
        to_string(User.Permission);
}

// -------------------- File operations --------------------

vector<stClient> LoadClientDataFromFile(const string& FileName = ClientsFileName)
{
    vector<stClient> Clients;
    ifstream MyFile(FileName);
    string Line;

    while (getline(MyFile, Line))
    {
        if (Line.empty())
            continue;

        stClient Client;
        if (ConvertLineToClient(Line, Client))
            Clients.push_back(Client);
        else
            cerr << "Warning: skipped an invalid client record in " << FileName << ".\n";
    }

    return Clients;
}

vector<stUser> LoadUserDataFromFile(const string& FileName = UsersFileName)
{
    vector<stUser> Users;
    ifstream MyFile(FileName);
    string Line;

    while (getline(MyFile, Line))
    {
        if (Line.empty())
            continue;

        stUser User;
        if (ConvertLineToUser(Line, User))
            Users.push_back(User);
        else
            cerr << "Warning: skipped an invalid user record in " << FileName << ".\n";
    }

    return Users;
}

bool AppendLineToFile(const string& FileName, const string& Line)
{
    ofstream MyFile(FileName, ios::out | ios::app);
    if (!MyFile.is_open())
    {
        cerr << "Error: could not open " << FileName << " for writing.\n";
        return false;
    }

    MyFile << Line << '\n';
    return MyFile.good();
}

bool SaveClientsToFile(const vector<stClient>& Clients, const string& FileName = ClientsFileName)
{
    ofstream MyFile(FileName, ios::out | ios::trunc);
    if (!MyFile.is_open())
    {
        cerr << "Error: could not open " << FileName << " for writing.\n";
        return false;
    }

    for (const stClient& Client : Clients)
    {
        if (!Client.MarkForDelete)
            MyFile << ConvertClientToLine(Client) << '\n';
    }

    return MyFile.good();
}

bool SaveUsersToFile(const vector<stUser>& Users, const string& FileName = UsersFileName)
{
    ofstream MyFile(FileName, ios::out | ios::trunc);
    if (!MyFile.is_open())
    {
        cerr << "Error: could not open " << FileName << " for writing.\n";
        return false;
    }

    for (const stUser& User : Users)
    {
        if (!User.MarkForDelete)
            MyFile << ConvertUserToLine(User) << '\n';
    }

    return MyFile.good();
}

// -------------------- Lookup helpers --------------------

int FindClientIndexByAccountNumber(const string& AccountNumber, const vector<stClient>& Clients)
{
    for (size_t i = 0; i < Clients.size(); ++i)
    {
        if (Clients[i].AccountNumber == AccountNumber)
            return static_cast<int>(i);
    }
    return -1;
}

int FindUserIndexByUserName(const string& UserName, const vector<stUser>& Users)
{
    for (size_t i = 0; i < Users.size(); ++i)
    {
        if (Users[i].UserName == UserName)
            return static_cast<int>(i);
    }
    return -1;
}

bool ClientExistsByAccountNumber(const string& AccountNumber)
{
    return FindClientIndexByAccountNumber(AccountNumber, LoadClientDataFromFile()) >= 0;
}

bool UserExistsByUserName(const string& UserName)
{
    return FindUserIndexByUserName(UserName, LoadUserDataFromFile()) >= 0;
}

// -------------------- Display helpers --------------------

void PrintClientCard(const stClient& Client)
{
    cout << "\n----------------------------------------------\n";
    cout << "Account Number  : " << Client.AccountNumber << '\n';
    cout << "Name            : " << Client.Name << '\n';
    cout << "PIN Code        : " << Client.PinCode << '\n';
    cout << "Phone           : " << Client.Phone << '\n';
    cout << "Account Balance : " << fixed << setprecision(2) << Client.AccountBalance << '\n';
    cout << "----------------------------------------------\n";
}

void PrintClientRecordLine(const stClient& Client)
{
    cout << '|' << left << setw(18) << Client.AccountNumber
        << '|' << left << setw(28) << Client.Name
        << '|' << left << setw(15) << Client.PinCode
        << '|' << left << setw(18) << Client.Phone
        << '|' << right << setw(14) << fixed << setprecision(2) << Client.AccountBalance << '\n';
}

void PrintUserCard(const stUser& User)
{
    cout << "\n----------------------------------------------\n";
    cout << "User Name   : " << User.UserName << '\n';
    cout << "Password    : ******** (hidden)\n";
    cout << "Permissions : " << User.Permission << '\n';
    cout << "----------------------------------------------\n";
}

void PrintClientsTable(const vector<stClient>& Clients)
{
    cout << "\nClients List (" << Clients.size() << ")\n";
    cout << string(98, '-') << '\n';
    cout << '|' << left << setw(18) << "Account Number"
        << '|' << left << setw(28) << "Name"
        << '|' << left << setw(15) << "PIN Code"
        << '|' << left << setw(18) << "Phone"
        << '|' << right << setw(14) << "Balance" << '\n';
    cout << string(98, '-') << '\n';

    if (Clients.empty())
    {
        cout << "No clients are available.\n";
        return;
    }

    for (const stClient& Client : Clients)
        PrintClientRecordLine(Client);

    cout << string(98, '-') << '\n';
}

// -------------------- Permission checks --------------------

bool CheckAccessPermission(int Permission)
{
    if (CurrentUser.Permission == eAll)
        return true;

    return (CurrentUser.Permission & Permission) == Permission;
}

bool RequirePermission(int Permission)
{
    if (CheckAccessPermission(Permission))
        return true;

    cout << "\n----------------------------------------------\n";
    cout << "Access denied. You do not have permission to perform this action.\n";
    cout << "Please contact your administrator.\n";
    cout << "----------------------------------------------\n";
    return false;
}

// -------------------- Client operations --------------------

void ShowAllClientsScreen()
{
    if (!RequirePermission(pListClients))
        return;

    PrintClientsTable(LoadClientDataFromFile());
}

void ShowAddNewClient()
{
    if (!RequirePermission(pAddNewClient))
        return;

    stClient Client;
    Client.AccountNumber = ReadAccountNumber();

    while (ClientExistsByAccountNumber(Client.AccountNumber))
    {
        cout << "This account number already exists. Please enter another one.\n";
        Client.AccountNumber = ReadAccountNumber();
    }

    Client.Name = ReadText("Name: ");
    Client.PinCode = ReadText("PIN Code: ");
    Client.Phone = ReadText("Phone: ");
    Client.AccountBalance = ReadNonNegativeAmount("Opening balance: ");

    if (AppendLineToFile(ClientsFileName, ConvertClientToLine(Client)))
        cout << "\nClient added successfully.\n";
}

void ShowUpdateClient()
{
    if (!RequirePermission(pUpdateClient))
        return;

    vector<stClient> Clients = LoadClientDataFromFile();
    string AccountNumber = ReadAccountNumber();
    int Index = FindClientIndexByAccountNumber(AccountNumber, Clients);

    if (Index < 0)
    {
        cout << "Client with account number " << AccountNumber << " was not found.\n";
        return;
    }

    PrintClientCard(Clients[Index]);
    if (!Confirm("Are you sure you want to update this client?"))
        return;

    Clients[Index].Name = ReadText("New name: ");
    Clients[Index].PinCode = ReadText("New PIN Code: ");
    Clients[Index].Phone = ReadText("New phone: ");
    Clients[Index].AccountBalance = ReadNonNegativeAmount("New account balance: ");

    if (SaveClientsToFile(Clients))
        cout << "Client updated successfully.\n";
}

void ShowDeleteClient()
{
    if (!RequirePermission(pDeleteClient))
        return;

    vector<stClient> Clients = LoadClientDataFromFile();
    string AccountNumber = ReadAccountNumber();
    int Index = FindClientIndexByAccountNumber(AccountNumber, Clients);

    if (Index < 0)
    {
        cout << "Client with account number " << AccountNumber << " was not found.\n";
        return;
    }

    PrintClientCard(Clients[Index]);
    if (!Confirm("Are you sure you want to delete this client?"))
        return;

    Clients.erase(Clients.begin() + Index);
    if (SaveClientsToFile(Clients))
        cout << "Client deleted successfully.\n";
}

void ShowFindClient()
{
    if (!RequirePermission(pFindClient))
        return;

    vector<stClient> Clients = LoadClientDataFromFile();
    string AccountNumber = ReadAccountNumber();
    int Index = FindClientIndexByAccountNumber(AccountNumber, Clients);

    if (Index < 0)
        cout << "Client with account number " << AccountNumber << " was not found.\n";
    else
        PrintClientCard(Clients[Index]);
}

// -------------------- Transactions --------------------

void ShowTotalBalance()
{
    if (!RequirePermission(pTransactions))
        return;

    vector<stClient> Clients = LoadClientDataFromFile();
    double Total = 0.0;

    cout << "\nBalances List (" << Clients.size() << ")\n";
    cout << string(62, '-') << '\n';
    cout << '|' << left << setw(18) << "Account Number"
        << '|' << left << setw(28) << "Client Name"
        << '|' << right << setw(14) << "Balance" << '\n';
    cout << string(62, '-') << '\n';

    if (Clients.empty())
        cout << "No clients are available.\n";

    for (const stClient& Client : Clients)
    {
        cout << '|' << left << setw(18) << Client.AccountNumber
            << '|' << left << setw(28) << Client.Name
            << '|' << right << setw(14) << fixed << setprecision(2) << Client.AccountBalance << '\n';
        Total += Client.AccountBalance;
    }

    cout << string(62, '-') << '\n';
    cout << "Total balances: " << fixed << setprecision(2) << Total << '\n';
}

void PerformTransaction(bool IsDeposit)
{
    if (!RequirePermission(pTransactions))
        return;

    vector<stClient> Clients = LoadClientDataFromFile();
    string AccountNumber = ReadAccountNumber();
    int Index = FindClientIndexByAccountNumber(AccountNumber, Clients);

    if (Index < 0)
    {
        cout << "Client with account number " << AccountNumber << " was not found.\n";
        return;
    }

    PrintClientCard(Clients[Index]);
    double Amount = ReadNonNegativeAmount(IsDeposit ? "Deposit amount: " : "Withdraw amount: ");

    if (Amount <= 0.0)
    {
        cout << "The transaction amount must be greater than zero.\n";
        return;
    }

    if (!IsDeposit && Amount > Clients[Index].AccountBalance)
    {
        cout << "Amount exceeds the available balance (" << fixed << setprecision(2)
            << Clients[Index].AccountBalance << ").\n";
        return;
    }

    if (!Confirm("Are you sure you want to perform this transaction?"))
    {
        cout << "Transaction cancelled.\n";
        return;
    }

    if (IsDeposit)
        Clients[Index].AccountBalance += Amount;
    else
        Clients[Index].AccountBalance -= Amount;

    if (SaveClientsToFile(Clients))
    {
        cout << "Transaction completed successfully. New balance: "
            << fixed << setprecision(2) << Clients[Index].AccountBalance << '\n';
    }
}

void ShowTransactionsMenue()
{
    if (!RequirePermission(pTransactions))
        return;

    while (true)
    {
        cout << "\n===========================================\n";
        cout << "\tTransactions Menu\n";
        cout << "===========================================\n";
        cout << "[1] Deposit\n";
        cout << "[2] Withdraw\n";
        cout << "[3] Total balances\n";
        cout << "[4] Main menu\n";
        cout << "===========================================\n";

        int Choice = ReadChoice("Choose an option [1-4]: ", 1, 4);
        switch (Choice)
        {
        case eDeposit:
            PerformTransaction(true);
            break;
        case eWithdraw:
            PerformTransaction(false);
            break;
        case eShowTotalBalance:
            ShowTotalBalance();
            break;
        case eTransactionsMainMenu:
            return;
        }

        Pause();
    }
}

// -------------------- User permissions and operations --------------------

int ReadPermissionsToSet()
{
    if (Confirm("Give full access?"))
        return eAll;

    int Permissions = 0;
    cout << "\nSelect permissions for this user.\n";
    if (Confirm("Allow showing the client list?")) Permissions |= pListClients;
    if (Confirm("Allow adding clients?")) Permissions |= pAddNewClient;
    if (Confirm("Allow deleting clients?")) Permissions |= pDeleteClient;
    if (Confirm("Allow updating clients?")) Permissions |= pUpdateClient;
    if (Confirm("Allow finding clients?")) Permissions |= pFindClient;
    if (Confirm("Allow transactions?")) Permissions |= pTransactions;
    if (Confirm("Allow managing users?")) Permissions |= pManageUsers;

    return Permissions;
}

void ShowListUsers()
{
    if (!RequirePermission(pManageUsers))
        return;

    vector<stUser> Users = LoadUserDataFromFile();
    cout << "\nUsers List (" << Users.size() << ")\n";
    cout << string(54, '-') << '\n';
    cout << '|' << left << setw(24) << "User Name"
        << '|' << left << setw(16) << "Password"
        << '|' << left << setw(12) << "Permission" << '\n';
    cout << string(54, '-') << '\n';

    if (Users.empty())
        cout << "No users are available.\n";

    for (const stUser& User : Users)
    {
        cout << '|' << left << setw(24) << User.UserName
            << '|' << left << setw(16) << "********"
            << '|' << left << setw(12) << User.Permission << '\n';
    }
    cout << string(54, '-') << '\n';
}

void ShowAddNewUser()
{
    if (!RequirePermission(pManageUsers))
        return;

    stUser User;
    User.UserName = ReadUserName();

    while (UserExistsByUserName(User.UserName))
    {
        cout << "This username already exists. Please enter another one.\n";
        User.UserName = ReadUserName();
    }

    User.Password = ReadText("Password: ");
    User.Permission = ReadPermissionsToSet();

    if (AppendLineToFile(UsersFileName, ConvertUserToLine(User)))
        cout << "User added successfully.\n";
}

void ShowDeleteUser()
{
    if (!RequirePermission(pManageUsers))
        return;

    vector<stUser> Users = LoadUserDataFromFile();
    string UserName = ReadUserName();
    int Index = FindUserIndexByUserName(UserName, Users);

    if (Index < 0)
    {
        cout << "User " << UserName << " was not found.\n";
        return;
    }

    if (Users[Index].UserName == CurrentUser.UserName)
    {
        cout << "You cannot delete the currently logged-in user.\n";
        return;
    }

    PrintUserCard(Users[Index]);
    if (!Confirm("Are you sure you want to delete this user?"))
        return;

    Users.erase(Users.begin() + Index);
    if (SaveUsersToFile(Users))
        cout << "User deleted successfully.\n";
}

void ShowUpdateUser()
{
    if (!RequirePermission(pManageUsers))
        return;

    vector<stUser> Users = LoadUserDataFromFile();
    string UserName = ReadUserName();
    int Index = FindUserIndexByUserName(UserName, Users);

    if (Index < 0)
    {
        cout << "User " << UserName << " was not found.\n";
        return;
    }

    PrintUserCard(Users[Index]);
    if (!Confirm("Are you sure you want to update this user?"))
        return;

    Users[Index].Password = ReadText("New password: ");
    Users[Index].Permission = ReadPermissionsToSet();

    if (!SaveUsersToFile(Users))
        return;

    // Keep the in-memory current user consistent if their own record was edited.
    if (Users[Index].UserName == CurrentUser.UserName)
        CurrentUser = Users[Index];

    cout << "User updated successfully.\n";
}

void ShowFindUser()
{
    if (!RequirePermission(pManageUsers))
        return;

    vector<stUser> Users = LoadUserDataFromFile();
    string UserName = ReadUserName();
    int Index = FindUserIndexByUserName(UserName, Users);

    if (Index < 0)
        cout << "User " << UserName << " was not found.\n";
    else
        PrintUserCard(Users[Index]);
}

void ShowManageUsersMenue()
{
    if (!RequirePermission(pManageUsers))
        return;

    while (true)
    {
        cout << "\n====================================================\n";
        cout << "\tManage Users Menu\n";
        cout << "====================================================\n";
        cout << "[1] List users\n";
        cout << "[2] Add new user\n";
        cout << "[3] Delete user\n";
        cout << "[4] Update user\n";
        cout << "[5] Find user\n";
        cout << "[6] Main menu\n";
        cout << "====================================================\n";

        int Choice = ReadChoice("Choose an option [1-6]: ", 1, 6);
        switch (Choice)
        {
        case eListUsers:
            ShowListUsers();
            break;
        case eAddNewUser:
            ShowAddNewUser();
            break;
        case eDeleteUser:
            ShowDeleteUser();
            break;
        case eUpdateUser:
            ShowUpdateUser();
            break;
        case eFindUser:
            ShowFindUser();
            break;
        case eUsersMainMenu:
            return;
        }

        Pause();
    }
}

// -------------------- Login and main menu --------------------

bool LoadUserInfo(const string& UserName, const string& Password)
{
    vector<stUser> Users = LoadUserDataFromFile();
    for (const stUser& User : Users)
    {
        if (User.UserName == UserName && User.Password == Password)
        {
            CurrentUser = User;
            return true;
        }
    }
    return false;
}

bool Login()
{
    while (true)
    {
        ClearScreen();
        cout << "\n================================================\n";
        cout << "\t\tLogin Screen\n";
        cout << "================================================\n";

        string UserName = ReadUserName();
        string Password = ReadText("Password: ");

        if (LoadUserInfo(UserName, Password))
        {
            cout << "\nLogin successful. Welcome, " << CurrentUser.UserName << ".\n";
            return true;
        }

        cout << "\nInvalid username or password.\n";
        Pause();
    }
}

void ShowMainMenue()
{
    while (true)
    {
        cout << "\n=============================================\n";
        cout << "\t\tMain Menu Screen\n";
        cout << "=============================================\n";
        cout << "[1] Show client list\n";
        cout << "[2] Add new client\n";
        cout << "[3] Delete client\n";
        cout << "[4] Update client\n";
        cout << "[5] Find client\n";
        cout << "[6] Transactions\n";
        cout << "[7] Manage users\n";
        cout << "[8] Logout\n";
        cout << "=============================================\n";

        int Choice = ReadChoice("Choose an option [1-8]: ", 1, 8);
        switch (Choice)
        {
        case eListClients:
            ShowAllClientsScreen();
            break;
        case eAddNewClient:
            ShowAddNewClient();
            break;
        case eDeleteClient:
            ShowDeleteClient();
            break;
        case eUpdateClient:
            ShowUpdateClient();
            break;
        case eFindClient:
            ShowFindClient();
            break;
        case eTransactions:
            ShowTransactionsMenue();
            break;
        case eManageUsers:
            ShowManageUsersMenue();
            break;
        case eLogout:
            CurrentUser = stUser();
            return;
        }

        Pause();
    }
}

int main()
{
    // For a fresh project, create an initial full-access account if Users.txt is empty.
    // Credentials: admin / 1234. Change these after first login in a real application.
    vector<stUser> Users = LoadUserDataFromFile();
    if (Users.empty())
    {
        stUser Admin;
        Admin.UserName = "admin";
        Admin.Password = "1234";
        Admin.Permission = eAll;

        if (AppendLineToFile(UsersFileName, ConvertUserToLine(Admin)))
        {
            cout << "Initial admin user created. Username: admin | Password: 1234\n";
            cout << "This is a learning project; do not use plain-text passwords in production.\n";
            Pause();
        }
    }

    while (true)
    {
        Login();
        ShowMainMenue();
    }

    return 0;
}
