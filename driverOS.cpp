/*
Implementation file for driver.cpp
This is a way of seperating functions that are used inside the driver to manipulate
the user, account, and object data.
*/

void initialBankSetup(tableSet &allTables);
bool isValidOption(string input, int upperBound);
int getUserOption(int upperBound);
void userLoginReset(tableSet &allTables);
void memberLogin(tableSet &allTables);
void officialLogin(tableSet &allTables);
void adminLogin(tableSet &allTables);

void initialBankSetup(tableSet &allTables)
{
    allTables.accountTable.buildTable("Tables/AccountTable.txt");
    allTables.firstNameTable.buildTree("Tables/FirstNameTable.txt");
    allTables.lastNameTable.buildTree("Tables/LastNameTable.txt");
    allTables.phoneNumTable.buildTree("Tables/PhoneTable.txt");
    allTables.addressTable.buildTree("Tables/AddressTable.txt");
    allTables.userTable.buildTree("Tables/UserTable.txt");

    allTables.accountTable.refreshInfo(); //need to finish function, but will do the interest computation

    //ensuring hard coded user account types of each; if already saved in .txt file, will have no effect
    string hashedPw = EncryptionBox::hash("password1");
    vector<string> adminInfo = {hashedPw, "admin"};
    vector<string> offInfo = {hashedPw, "official"};
    vector<string> memInfo = {hashedPw, "member"};
    allTables.userTable.insertWithList("admin", adminInfo);
    allTables.userTable.insertWithList("official", offInfo);
    allTables.userTable.insertWithList("house", memInfo);
}

//validates the user's input is within a range of options; max possible range is 1-9, for now
bool isValidOption(string input, int upperBound)
{
    if (input.length() > 1)
    {
        return false;
    }
    else
    {
        try
        {
            int option = stoi(input); //since only 1 char is ensured, if 'a' is entered, will throw an error to catch block
            bool returnValue = ( (option <= upperBound) && (option >= 1) ) ? true : false;
            return returnValue;
        }

        catch (...)
        {
            return false;
        }
    }   
}

//upperBound is the max option possible, must be 1-9
int getUserOption(int upperBound)
{
    string userInput = "";
    bool isValid = false;
    while (true)
    {
        getline(cin, userInput);
        isValid = isValidOption(userInput, upperBound);
        if (!isValid)
        {
            cout << "Invalid option! Try again: ";
        }
        else
        {
            break;
        }
    }
    return stoi(userInput);
}

void userLoginReset(tableSet &allTables)
{
    cout << "Is your User ID or Password causing the issue?" << endl << endl;
    string resetOptions = "[1] The User ID\n[2] The Password";
    cout << resetOptions << endl << "Option: ";
    int userResetOption = getUserOption(2);
    cout << endl;

    switch (userResetOption)
    {
        case 1: //User ID reset
        {
            cout << "In order to provide you with your User ID, one of your Account Numbers is required." << endl;
            cout << "Do you know any of your Account Numbers?" << endl << endl;
            string accountNumPrompt = "[1] Yes I do\n[2] No I don't";
            cout << accountNumPrompt << endl << "Option: ";
            int knowsAccountNums = getUserOption(2);
            cout << endl;

            if (knowsAccountNums == 1)
            {
                string accountNum = "", accountInfo = "";
                cout << "Please enter your Account Number: ";
                getline(cin, accountNum);
                cout << endl;

                accountInfo = allTables.accountTable.search(accountNum);
                if (accountInfo != "false")
                {
                    //give user the username seperated out from info!
                }
                else
                {
                    cout << "The Account Number could not be found." << endl << endl;
                    cout << "We apologize for the automated system not being able to assist you further.\nPlease seek further help at your nearest branch office." << endl;
                }
            }
            else
            {
                cout << "The Automated System requires the Account Number to provide you with the User ID." << endl;
                cout << "Please visit a branch office for further assistence." << endl;
            }
            
            break;
        }
        case 2: //Password Reset
        {
            cout << "To reset your hashedPw, your User ID is required." << endl;
            cout << "Please enter your User ID: ";
            string userID = "", newPassword = "";
            getline(cin, userID);
            cout << endl;

            vector<string> userLoginInfo = allTables.userTable.returnMappedItems(userID);
            if (userLoginInfo.size() > 0)
            {
                cout << "Enter your new hashedPw: ";
                getline(cin, newPassword);
                /*userLoginInfo[0] = EncryptionBox::hash(newPassword);
                allTables.userTable.swapMappedItems(userID, userLoginInfo); //adds new list of login info to AVLTree

                Have an admin function to do something similiar to above*/
            }
            else
            {
                cout << "The online account could not be located." << endl << endl;
                cout << "We apologize for our automated system not being able to assist you further.\nPlease seek further help at your nearest branch office." << endl;
            }
           
            break;
        }
    }
}

void memberLogin(tableSet &allTables)
{
    //Display last login date up here!

    string memberInterface = "[1] View Account Info\n[2] Deposit Into Account\n[3] Withdraw From Account\n[4] Deposit Into External Account\n[5] View Account History\n[6] Change Information";
    cout << memberInterface << endl << "Option: ";
}

void officialLogin(tableSet &allTables)
{
    string officialInterface = "";
}

void adminLogin(tableSet &allTables)
{
    string adminInterface = "";
}
