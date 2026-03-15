#include <iostream>
#include <fstream> 
#include <string> 
#include <vector> 
#include <iomanip> 

using namespace std;

enum enMainMenueOptions {
    eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3,
    eCheckBalance = 4, eLogout = 5,
};

const string ClientsFileName = "Clients.txt";

void ShowMainMenue();
void ShowQuickWithdrawScreen();
void Login();

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sClient CurrentClient;

vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable   

    // use find() function to get the position of the delimiters   
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word    
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string. 
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double 


    return Client;

}

string ConvertRecordToLine(sClient &Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite 

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.   
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode 

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

string ReadPinCode()
{
    string PinCode = "";

    cout << "\nPlease enter PinCode? ";
    cin >> PinCode;
    return PinCode;

}

bool ClientExistsByAccountNumberAndPinCode(string AccountNumber, string PinCode, string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode 

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber && Client.PinCode == PinCode)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

bool Deposit(double Amount, vector <sClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == CurrentClient.AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                CurrentClient.AccountBalance = C.AccountBalance;
                return true;
            }
        }
    }
    return false;
}

void ShowDepositScreen()
{

    sClient Client;
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n===================================\n";
    cout << "\tDeposit Screen";
    cout << "\n===================================\n";

    double Amount = 0;
    do
    {
        cout << "\nEnter a positive Deposit amount? ";
        cin >> Amount;
    } while (Amount < 0);

    Deposit(Amount, vClients);
}

bool Withdraw(double Amount, vector <sClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == CurrentClient.AccountNumber)
            {
                C.AccountBalance -= Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                CurrentClient.AccountBalance = C.AccountBalance;

                return true;
            }
        }
    }
    return false;

}

short ReadQuickWithdrawOption()
{
    cout << "\nChoose what to withraw from[1] to [8]?";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void QuickWithraw(double Amount)
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    if (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice: ";
        cout << "\nPress Anykey to continue...";
        system("pause>0");
        system("cls");
        ShowQuickWithdrawScreen();
    }
    Withdraw(Amount, vClients);
}

void  ShowNormalWithrawScreen()
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n===================================\n";
    cout << "\tNormal Withdraw Screen";
    cout << "\n===================================\n";

    double Amount = 0;
    do
    {
        cout << "\nEnter an amount multiple of 5's? ";
        cin >> Amount;
    } while ((int)Amount % 5 != 0 || Amount <= 0);

    if (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice: ";
        cout << "\nPress Anykey to continue...";
        system("pause>0");
        system("cls");
        ShowNormalWithrawScreen();
    }
    else
    {
        Withdraw(Amount, vClients);
    }
    
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}

void PerfromQuickWithdrawOption(short choice)
{
    double Amount[] = {0,20,50,100,200,400,600,800,1000};
    if (choice == 9)
        ShowMainMenue();
    if (choice >= 1 && choice <= 8)
    {
        QuickWithraw(Amount[choice]);
        GoBackToMainMenue();
    }
        
}

void ShowQuickWithdrawScreen()
{
    cout << "\n===================================\n";
    cout << "\tQuick Withdraw";
    cout << "\n===================================\n";
    cout << "        [1] 20        [2] 50         \n";
    cout << "        [3] 100       [4] 200         \n";
    cout << "        [5] 400       [6] 600         \n";
    cout << "        [7] 800       [8] 1000         \n";
    cout << "        [9] Exist                       \n";
    cout << "\n===================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance;
    PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}

void ShowBalanceClientScreen()
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    cout << "\n===================================\n";
    cout << "\tCheck Balance Screen";
    cout << "\n===================================\n";
        cout << "Your Balance is " << CurrentClient.AccountBalance;
    
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eQuickWithdraw:
    {
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eNormalWithdraw:
        system("cls");
        ShowNormalWithrawScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eCheckBalance:
        system("cls");
        ShowBalanceClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eLogout:
        system("cls");
        Login();

        break;
    }

}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdarw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";


    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

bool  LoadClientInfo(string AccountNumber, string PinCode)
{

    if (ClientExistsByAccountNumberAndPinCode(AccountNumber, PinCode, ClientsFileName))
        return true;
    else
        return false;

}

void Login()
{
    bool LoginFaild = false;
    string AccountNumber, PinCode;
    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invlaid Username/Password!\n";
        }

        cout << "Enter AccountNumber? ";
        cin >> AccountNumber;

        cout << "Enter PinCode? ";
        cin >> PinCode;

        LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFaild);

    FindClientByAccountNumber(AccountNumber, LoadCleintsDataFromFile(ClientsFileName), CurrentClient);
    ShowMainMenue();

}

int main()
{
    Login();
    system("pause>0");
    return 0;
}