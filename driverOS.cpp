/*
Nathan Obert M03134502
Implementation file for driver.cpp, further broken down with each User Type having its own OS .cpp file
This is a way of seperating functions that are used inside the driver to manipulate
the user, account, and object data.
*/

void initialSetup();
void savingTables();
bool isValidOption(string input, int upperBound);
int getUserOption(int upperBound);
void userLoginReset();
bool isValidNumber(string inputNum);


/************************************************
Initial helper functions for main
************************************************/


//must add chking for house account!!
void initialSetup()
{
    DataHandler::allTables.accountTable.buildTable("Tables/AccountTable.txt");
    DataHandler::allTables.firstNameTable.buildTree("Tables/FirstNameTable.txt");
    DataHandler::allTables.lastNameTable.buildTree("Tables/LastNameTable.txt");
    DataHandler::allTables.phoneNumTable.buildTree("Tables/PhoneTable.txt");
    DataHandler::allTables.addressTable.buildTree("Tables/AddressTable.txt");
    DataHandler::allTables.userTable.buildTree("Tables/UserTable.txt");

    AccountQueue queue; //where user requested accounts are stored, inside a specific .txt file
    queue.buildQueue();
    queue.saveQueue(); //so that if file's not there, won't display that message during runtime

    DataHandler::allTables.accountTable.refreshInfo();

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
    admin.buildUser("UserData/admin.txt"); //this will preserve recent history, if there
    admin.setName("Automated Admin"); //hard coding everything else abount the account
    admin.setID("admin");
    admin.setPassword(EncryptionBox::hash("password1"));
    admin.setUserType("admin");

    Official official;
    official.buildUser("UserData/official.txt");
    official.setName("Automated Official");
    official.setID("official");
    official.setPassword(EncryptionBox::hash("password1"));
    official.setUserType("official");
    official.setState("active");

    Client house;
    house.buildUser("UserData/house.txt");
    house.setName("House Account");
    house.setAddress("1234 Made Up Avenue, Springfield, MO");
    house.setPhoneNum("417-555-1234");
    house.setID("house");
    house.setPassword(EncryptionBox::hash("password1"));
    house.setUserType("client");

    admin.saveUser();
    official.saveUser();
    house.saveUser();

    accountInit();

    DataHandler::buildAccountTypesFomeFile(); //will auto initialize the default 3 if no file is found
}

void savingBank()
{
    DataHandler::allTables.accountTable.saveInfo("Tables/AccountTable.txt");
    DataHandler::allTables.firstNameTable.saveInfo("Tables/FirstNameTable.txt");
    DataHandler::allTables.lastNameTable.saveInfo("Tables/LastNameTable.txt");
    DataHandler::allTables.phoneNumTable.saveInfo("Tables/PhoneTable.txt");
    DataHandler::allTables.addressTable.saveInfo("Tables/AddressTable.txt");
    DataHandler::allTables.userTable.saveInfo("Tables/UserTable.txt");
    accountExit(); //defined inside the account classs
}

//validates the user's input is within a range of options; max possible range is 1-99, for now
bool isValidOption(string input, int upperBound)
{
    if (input.length() > 2)
    {
        return false;
    }
    else
    {
        try
        {
            int option = stoi(input); //ensures input like a1 will fail

            if ( input.length() == to_string(option).length() ) //this comparison catches inputs like 1a, where they start with an int but not all is an int
            {
                bool returnValue = ( (option <= upperBound) && (option >= 1) ) ? true : false;
                return returnValue;
            }
            
            return false;
        }

        catch (...)
        {
            return false;
        }
    }   
}

//upperBound is the max option possible, must be 1-99
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
        case 1: //User ID display -- Will not work for admin or officials, by design
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

                accountInfo = DataHandler::getAccountInfo(accountNum);
                if (accountInfo != "false") //i.e. found valid account info from the acctNum
                {
                    Admin admin; //creating the Automated Admin obj to change the password
                    admin.buildUser("UserData/admin.txt");
                    admin.setRecentLogin(DateTools().getCurrentTime()); //setting most recent login date as today

                    string userID = admin.returnUserID(accountNum);
                    cout << "Your User ID is: " << userID << endl << endl;

                    string description = "Provided user '" + userID + "' the User ID to the online account";
                    admin.setRecentActivity(description);
                    admin.saveUser(); //saving change to file

                    //now to reflect changes made in user account!
                    Client user;
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

            vector<string> userInfo = DataHandler::clientGetAccountList(userID); // {hashedPW, user type, accts...} is returned

            if (userInfo.size() > 0) //a user ID was found
            {
                cout << "Enter your new password: ";
                getline(cin, newPassword);

                Admin admin; //creating the Automated Admin obj to change the password
                admin.buildUser("UserData/admin.txt");
                admin.setRecentLogin(DateTools().getCurrentTime()); //setting most recent login date as today
                admin.resetPassword(userID, newPassword); //updates tables
                admin.setRecentActivity("Assisted Client '" + userID + "' Change Password");
                admin.saveUser();

                if (userInfo[1] == "client")
                {
                    Client user;
                    user.buildUser("UserData/"+userID+".txt");
                    user.setPassword(EncryptionBox::hash(newPassword)); //changing user's record
                    user.setRecentActivity("Password was Reset by the Automated System Administrator");
                    user.saveUser();
                    cout << "Your password has been reset." << endl << endl;
                }
                else if (userInfo[1] == "official")
                {
                    Official user;
                    user.buildUser("UserData/"+userID+".txt");
                    user.setPassword(EncryptionBox::hash(newPassword)); //changing user's record
                    user.setRecentActivity("Password was Reset by the Automated System Administrator");
                    user.saveUser();
                    cout << "Your password has been reset." << endl << endl;
                }
                else
                {
                    cout << "Admin Accounts Must Manually Reset Their Password." << endl << endl;
                }
            }
           
            else //invalid user id
            {
                cout << "The online account could not be located." << endl << endl;
                cout << "We apologize for our automated system not being able to assist you further.\nPlease seek further help at your nearest branch office." << endl;
            }
           
            break;
        }
    }
}

bool isValidNumber(string inputNum)
{
    string validNumbers = "0123456789.";
    bool match = false; 
    int count = 0;   

    for (int n = 0; n < inputNum.length(); n++)
    {
        if (validNumbers.find( inputNum[n] ) == string::npos)
        {
            return false;
        }
        else if (inputNum[n] == validNumbers[10]) //if it's a decimal
        {
            count++; //keeps track of how many decimals there are
        }   
    }   
    bool returnValue = (count <= 1) ? true : false; //can only be <=1 decimals in a valid number
    return returnValue;
}
