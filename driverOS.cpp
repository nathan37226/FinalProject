/*
Implementation file for driver.cpp
This is a way of seperating functions that are used inside the driver to manipulate
the user, account, and object data.
*/

void initialSetup();
bool isValidOption(string input, int upperBound);
int getUserOption(int upperBound);
void userLoginReset();
void clientLogin(string userID);
void officialLogin(string userID);
void adminLogin(string userID);

//must add chking for house account!!
void initialSetup()
{
    DataHandler::allTables.accountTable.buildTable("Tables/AccountTable.txt");
    DataHandler::allTables.firstNameTable.buildTree("Tables/FirstNameTable.txt");
    DataHandler::allTables.lastNameTable.buildTree("Tables/LastNameTable.txt");
    DataHandler::allTables.phoneNumTable.buildTree("Tables/PhoneTable.txt");
    DataHandler::allTables.addressTable.buildTree("Tables/AddressTable.txt");
    DataHandler::allTables.userTable.buildTree("Tables/UserTable.txt");

    DataHandler::allTables.accountTable.refreshInfo(); //need to finish function, but will do the interest computation

    //ensuring hard coded user account types of each; if already saved in .txt file, will have no effect
    string hashedPw = EncryptionBox::hash("password1");
    vector<string> adminInfo = {hashedPw, "admin"};
    vector<string> offInfo = {hashedPw, "official"};
    vector<string> clientInfo = {hashedPw, "client"};
    DataHandler::allTables.userTable.insertWithList("admin", adminInfo); //rewrites the admin acct to have the pw as password1
    DataHandler::allTables.userTable.insertWithList("official", offInfo);
    DataHandler::allTables.userTable.insertWithList("house", clientInfo);

	//creating record of these default users, i.e. saving to .txt files
	Admin admin;
    admin.buildUser("UserData/admin.txt");
    admin.setName("Automated Admin");
    admin.setID("admin");
    admin.setPassword(EncryptionBox::hash("password1"));
    admin.setUserType("admin");

    Official official;
    official.buildUser("UserData/official.txt");
    official.setName("Automated Official");
    official.setID("official");
    official.setPassword(EncryptionBox::hash("password1"));
    official.setUserType("official");

    Client house;
    house.buildUser("UserData/house.txt");
    house.setName("House Account");
    house.setID("house");
    house.setPassword(EncryptionBox::hash("password1"));
    house.setUserType("client");

    admin.saveUser();
    official.saveUser();
    house.saveUser();
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

void userLoginReset()
{
    cout << "Is your User ID or Password causing the issue?" << endl << endl;
    string resetOptions = "[1] The User ID\n[2] The Password";
    cout << resetOptions << endl << "Option: ";
    int userResetOption = getUserOption(2);
    cout << endl;

    switch (userResetOption)
    {
        case 1: //User ID display
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

                accountInfo = DataHandler::allTables.accountTable.search(accountNum);
                if (accountInfo != "false") //i.e. found valid account info from the acctNum
                {
                    Admin admin; //creating the Automated Admin obj to change the password
                    admin.buildUser("UserData/admin.txt");
                    admin.setRecentLogin(DateTools().getCurrentDate().ToString()); //setting most recent login date as today

                    string userID = admin.returnUserID(accountNum); //implement later!!!
                    cout << "Your User ID is: " << userID << endl;

                    string description = "Provided user '" + userID + "' the User ID to the online account";
                    admin.setRecentActivity(description);
                    admin.saveUser(); //saving change to file

                    //now to reflect changes made in user account!
                    User user; //doesn't matter which type of user!
                    user.buildUser("UserData/"+userID+".txt");
                    user.setRecentActivity("Recieved User ID from Automated System Administrator");
                    user.saveUser();
                }
                else
                {
                    cout << "The Account Number could not be found." << endl << endl;
                    cout << "We apologize for the Automated System Administrator not being able to assist you further.\nPlease seek further help at your nearest branch office." << endl;
                }
            }
            else
            {
                cout << "The Automated System Administrator requires the Account Number to provide you with the User ID." << endl;
                cout << "Please visit a branch office for further assistence." << endl;
            }
            
            break;
        }
        case 2: //Password Reset
        {
            cout << "To reset your password, your User ID is required." << endl;
            cout << "Please enter your User ID: ";
            string userID = "", newPassword = "";
            getline(cin, userID);
            cout << endl;

            vector<string> userLoginInfo = DataHandler::allTables.userTable.returnMappedItems(userID); // {hashedPW, user type} is returned
            if (userLoginInfo.size() > 0)
            {
                cout << "Enter your new password: ";
                getline(cin, newPassword);

                Admin admin; //creating the Automated Admin obj to change the password
                admin.buildUser("UserData/admin.txt");
                admin.setRecentLogin(DateTools().getCurrentDate().ToString()); //setting most recent login date as today
                admin.resetPassword(userID, newPassword, userLoginInfo[1]);

                User user;
                user.buildUser("UserData/"+userID+".txt");
                user.setRecentActivity("Password was Reset by the Automated System Administrator");
                user.saveUser();
                cout << "Your password has been reset." << endl;
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

void clientLogin(string userID)
{
    Client user;
    user.buildUser("UserData/" + userID + ".txt");
    cout << "Welcome, " << user.getName() << endl;
    cout << "Last Activity: " << user.getRecentActivity() << endl;
    cout << "Last Login: " << user.getRecentLogin() << endl << endl;
    user.setRecentLogin(DateTools().getCurrentDate().ToString());
    //Display last login date up here!

    string clientInterface = "[1] View Account Info\n[2] Deposit Into Account\n[3] Withdraw From Account\n[4] Deposit Into External Account\n[5] View Account History\n[6] Change Information";
    cout << clientInterface << endl << "Option: ";
}

void officialLogin(string userID)
{
    Official user;
    user.buildUser("UserData/" + userID + ".txt");
    cout << "Welcome, " << user.getName() << endl;
    cout << "Last Activity: " << user.getRecentActivity() << endl;
    cout << "Last Login: " << user.getRecentLogin() << endl << endl;
    user.setRecentLogin(DateTools().getCurrentDate().ToString());

    string officialInterface = "";
}

void adminLogin(string userID)
{
    Admin user;
    user.buildUser("UserData/" + userID + ".txt");
    cout << "Welcome, " << user.getName() << endl;
    cout << "Last Activity: " << user.getRecentActivity() << endl;
    cout << "Last Login: " << user.getRecentLogin() << endl << endl;
    user.setRecentLogin(DateTools().getCurrentDate().ToString());

    string adminInterface = "";
}
