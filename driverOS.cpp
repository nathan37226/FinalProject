/*
Implementation file for driver.cpp
This is a way of seperating functions that are used inside the driver to manipulate
the user, account, and object data.
*/

void initialSetup();
void savingTables();
bool isValidOption(string input, int upperBound);
int getUserOption(int upperBound);

void userLoginReset();
bool isValidUserID(string userID);
void clientLogin(string userID);
void makeClientAccountChanges(Client &user, int option);
void helpClientAccessAccount(Client &user, int option, vector<string> acctList);

void officialLogin(string userID);
void openAccounts(Official &user);

void adminLogin(string userID);


/************************************************
Initial helper functions
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
    house.setAddress("1234 Made Up Avenue, Springfield, MO");
    house.setPhoneNum("417-555-1234");
    house.setID("house");
    house.setPassword(EncryptionBox::hash("password1"));
    house.setUserType("client");

    admin.saveUser();
    official.saveUser();
    house.saveUser();
}

void savingTables()
{
    DataHandler::allTables.accountTable.saveInfo("Tables/AccountTable.txt");
    DataHandler::allTables.firstNameTable.saveInfo("Tables/FirstNameTable.txt");
    DataHandler::allTables.lastNameTable.saveInfo("Tables/LastNameTable.txt");
    DataHandler::allTables.phoneNumTable.saveInfo("Tables/PhoneTable.txt");
    DataHandler::allTables.addressTable.saveInfo("Tables/AddressTable.txt");
    DataHandler::allTables.userTable.saveInfo("Tables/UserTable.txt");
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

                accountInfo = DataHandler::allTables.accountTable.search(accountNum);
                if (accountInfo != "false") //i.e. found valid account info from the acctNum
                {
                    Admin admin; //creating the Automated Admin obj to change the password
                    admin.buildUser("UserData/admin.txt");
                    admin.setRecentLogin(DateTools().getCurrentTime()); //setting most recent login date as today

                    string userID = admin.returnUserID(accountNum); //Implement Later!!!!!
                    cout << "Your User ID is: " << userID << endl;

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

            vector<string> userInfo = DataHandler::allTables.userTable.returnMappedItems(userID); // {hashedPW, user type, accts...} is returned
            if (userInfo.size() > 0)
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
                    cout << "Your password has been reset." << endl;
                }
                else if (userInfo[1] == "official")
                {
                    Official user;
                    user.buildUser("UserData/"+userID+".txt");
                    user.setPassword(EncryptionBox::hash(newPassword)); //changing user's record
                    user.setRecentActivity("Password was Reset by the Automated System Administrator");
                    user.saveUser();
                    cout << "Your password has been reset." << endl;
                }
                else
                {
                    cout << "Admin Accounts Must Manually Reset Their Password." << endl;
                }
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

bool isValidUserID(string userID)
{
    bool value = (DataHandler::allTables.userTable.returnMappedItems(userID).size() == 0) ? true : false;
    return value;  //above ternary will return a vect of {} is userID not found, so if size == 0 then userID not found, which means the userID is avaliable
}


/************************************************
Start of Client Login
************************************************/


//Needs Work still!!
void clientLogin(string userID)
{
    Client user;
    user.buildUser("UserData/" + userID + ".txt");
    cout << "Welcome, " << user.getName() << endl;
    cout << "Last Activity: " << user.getRecentActivity() << endl;
    cout << "Last Login: " << user.getRecentLogin() << endl << endl;
    user.setRecentLogin(DateTools().getCurrentTime()); //since we just logged in, now need to update time

    string clientInterface = "[1] Access Accounts\n[2] View Personal Information\n[3] Change Information\n[4] Open New Account\n[5] Exit";
    bool wantsToExit = false;
    
    while (!wantsToExit)
    {
        cout << clientInterface << endl << "Option: ";
        int initialOption = getUserOption(5);
        cout << endl;

        switch (initialOption)
        {
            case 1: //Accounts
            {
                vector<string> acctList = DataHandler::allTables.userTable.returnMappedItems(userID); //formatted {hashedPw, user type, acct 1, acct 2, etc}
                if (acctList.size() == 2)
                {
                    cout << "There is no account data to access" << endl << endl;
                }
                else
                {
                    for (int i = 2; i < acctList.size(); i++)
                    {
                        cout << acctList[i] << endl;
                        cout << DataHandler::allTables.accountTable.search(acctList[i]) << endl;
                    }
                    cout << endl;

                    string accountInterface = "[1] Deposit Into Account\n[2] Withdraw From Account\n[3] Deposit Into External Account\n[4] View an Accounts History\n[5] Go Back";
                    cout << accountInterface << endl << "Option: ";
                    int accountInterfaceOption = getUserOption(5);
                    cout << endl;
                    
                    if (accountInterfaceOption != 5)
                    {
                        helpClientAccessAccount(user, accountInterfaceOption, acctList);
                    }
                }
                break;
            }
            case 2: //Display user info, i.e. name, address, phone num
            {
                user.displayInfo();
                user.setRecentActivity("Displayed Personal Information");
                break;
            }
            case 3: //change info
            {
                cout << "Which piece of information would you like to change?" << endl;
                cout << "[1] First Name\n[2] Last Name\n[3] Address\n[4] Phone Number\n[5] Password\n[6] Go Back" << endl;
                cout << "Option: ";
                int option = getUserOption(6);
                if (option != 6)
                {
                    cout << endl;
                    makeClientAccountChanges(user, option);
                }
                cout << endl;
                break;
            }
            case 4: //open new acct
            {
                string acctType = "";
                //Make sure to display all types of accts currently offered
                //Upon user choosing, send request off to an official for confirmation!
                //have all officials be able to access this list of requests!
                //Each request needs: UserID requesting and acct type
                //Upon accpeting request or denying request for new acct, update client recent activity message to display the choice

                DataHandler::clientRequestNewAccount(userID, acctType);
                user.setRecentActivity("Requested New Account: " + acctType);
                cout << "Please be aware that a Bear Bank Official may take several business days to review this request." << endl;
                break;
            }
            case 5: //exit
            {
                wantsToExit = true;
                user.saveUser();
            }
        }
    }
}

//Needs Finishing!
void makeClientAccountChanges(Client &user, int option)
{
    switch (option)
    {
        case 1: //change first name
        {
            string name = "", oldName = user.getName(), lastName = user.getName(), userID = user.getID();
            oldName = oldName.substr(0, oldName.find(" "));
            lastName = lastName.substr(lastName.find(" ") + 1, string::npos);
            cout << "Enter your new first name: ";
            getline(cin, name);
            name = name + " " + lastName;
            user.setName(name);
            user.setRecentActivity("Changed First Name");

            //need to reflect changes on all accts and tables as well
            DataHandler::changeClientFirstName(userID, oldName, name); //needs finishing

            break;
        }
        case 2: //change last name
        {
            string firstName = user.getName(), name = "", oldName = user.getName(), userID = user.getID();
            firstName = firstName.substr(0, firstName.find(" "));
            oldName = oldName.substr(oldName.find(" ") + 1, string::npos);

            //changing name in user obj
            cout << "Enter your new last name: ";
            getline(cin, name);
            name = firstName + " " + name;
            user.setName(name);
            user.setRecentActivity("Changed Last Name");

            //changing name inside the tables
            DataHandler::changeClientLastName(userID, oldName, name); //needs finishing

            break;
        }
        case 3: //change address
        {
            string newAddress = "", oldAddress = user.getAddress();

            //chaning address in user obj
            cout << "Enter your new address: ";
            getline(cin, newAddress);
            user.setAddress(newAddress);
            user.setRecentActivity("Changed Address");

            //changing address in table
            DataHandler::changeClientAddress(user.getID(), oldAddress, newAddress); //needs finishing

            break;
        }
        case 4: //change phone number
        {
            string newNum = "", oldNum = user.getPhoneNum();

            //chaning address in user obj
            cout << "Enter your new phone number: ";
            getline(cin, newNum);
            user.setAddress(newNum);
            user.setRecentActivity("Changed Phone Number");

            //changing address in table
            DataHandler::changeClientPhoneNum(user.getID(), oldNum, newNum); //needs finishing
            break;
        }
        case 5: //change password
        {
            string newPassword = "";
            cout << "Enter your new password: ";
            getline(cin, newPassword);

            Admin admin; //creating the Automated Admin obj to change the password
            admin.buildUser("UserData/admin.txt");
            admin.setRecentLogin(DateTools().getCurrentTime()); //setting most recent login date as today
            admin.resetPassword(user.getID(), newPassword);
            admin.setRecentActivity("Assisted Client '" + user.getID() + "' Change Password in Settings");

            user.setRecentActivity("Password was Reset in Settings");
            user.saveUser();
            cout << "Your password has been reset." << endl;
            break;
        }
    }
    user.saveUser();
}

//Needs Finishing!
void helpClientAccessAccount(Client &user, int option, vector<string> acctList)
{
    switch (option)
    {
        case 1: //deposit
        {
            break;
        }
        case 2: //withdraw
        {
            break;
        }
        case 3: //deposit into another acct in Bear Bank
        {
            break;
        }
        case 4: //view history
        {
            user.getAccountHistory(acctList); //Needs to be finished!!!
            break;
        }
    }
}


/************************************************
Start of Official Login
************************************************/


void officialLogin(string userID)
{
    Official user;
    user.buildUser("UserData/" + userID + ".txt");
    cout << "Welcome, " << user.getName() << endl;
    cout << "Last Activity: " << user.getRecentActivity() << endl;
    cout << "Last Login: " << user.getRecentLogin() << endl << endl;
    user.setRecentLogin(DateTools().getCurrentTime());

    string officialInterface = "[1] Open Account\n[2] Close Account\n[3] Deposit into Account\n[4] Withdraw from Account\n[5] Search for Accounts\n[6] Exit";
    bool wantsToExit = false;
    
    while (!wantsToExit)
    {
        cout << officialInterface << endl << "Option: ";
        int initialOption = getUserOption(6);

        switch (initialOption)
        {
            case 1: //Open acct - look at queue or just open new acct for specified user
            {
                openAccounts(user); //abstracted due to needed while loop
                break;
            }
            case 2: //Close acct
            {
                break;
            }
            case 3: //Deposit into acct - needs user confirmation
            {
                break;
            }
            case 4: //Withdraw from acct - needs user confirmation
            {
                break;
            }
            case 5: //Search for acct by first name, last name, phone num, address, acct num
            {
                break;
            }
            case 6: //Exit
            {
                wantsToExit = true;
                user.saveUser();
                break;
            }
        }
    }
}

void openAccounts(Official &user)
{
    string openAcctInterface = "[1] Open a new Account\n[2] View User Requested Accounts\n[3] Go Back";
    string requestedAcctInterface = "[1] Approve Request\n[2] Deny Request\n[3] Go Back";
    bool wantsToExit = false;
    
    while (!wantsToExit)
    {
        cout << openAcctInterface << endl << "Option: ";
        int mainOption = getUserOption(3);
        
        switch (mainOption)
        {
            case 1: //Open new acct
            {
                //get userID where acct will be added
                //display acct types
                //get selection
                //add acct to userTable and accountTable
                break;
            }
            case 2: //View acct request queue
            {
                AccountQueue queue;
                queue.buildQueue();
                string request = "", userID = "", acctType = "";
                bool doneReviewing = false;
    
                while (!doneReviewing)
                {
                    queue.peekFirst(request); //request takes on first item in queues value, queue retains order
                    userID = request.substr(0, request.find(" ")); //request is formatted: "<userID>  requests a new: <acctType>"
                    acctType = request.substr(request.find(":") + 2, string::npos); //finds the : inside "a new: " and gets whatever is after
                    Client user;
                    user.buildUser("UserData/" + userID + ".txt");

                    cout << requestedAcctInterface << endl << "Option: ";
                    int reviewOption = getUserOption(3);

                    switch (reviewOption)
                    {
                        case 1: //Approve
                        {  
                            string acctNum = "", dummyStr = "";
                            //create a new acct obj of specified type
                            //Account newAccount;
                            //save new acct obj to create a record in files
                            //update acctNum!

                            //DataHandler::addClientAccountToRecords(user, newAccount); //updates all tables with new info!
                            user.setRecentActivity("Request for: " + acctType + " was approved!");
                            user.saveUser();
                            queue.dequeue(dummyStr); //dequeue needs a string& passed, so this just allows a value to be removed
                            break;
                        }
                        case 2: //Deny
                        {
                            string dummyStr = "";
                            queue.dequeue(dummyStr);
                            break;
                        }
                        case 3: //Go Back
                        {
                            doneReviewing = true;
                            //save queue back to file
                            break;
                        }
                    }
                }

                break;
            }
            case 3: //Go back
            {
                wantsToExit = true;
                break;
            }
        }
    }
}



/************************************************
Start of Admin Login
************************************************/


void adminLogin(string userID)
{
    Admin user;
    user.buildUser("UserData/" + userID + ".txt");
    cout << "Welcome, " << user.getName() << endl;
    cout << "Last Activity: " << user.getRecentActivity() << endl;
    cout << "Last Login: " << user.getRecentLogin() << endl << endl;
    user.setRecentLogin(DateTools().getCurrentTime());

    string adminInterface = "";
}
