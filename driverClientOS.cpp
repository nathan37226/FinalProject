/*
Nathan Obert M03134502
All functions related to a Client logging into Bear Bank
*/

void clientLogin(string userID);
void clientMakeAccountChanges(Client &user, int option);
void clientHelpAccessAccount(Client &user, int option, vector<string> acctList);


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

    string clientInterface = "[1] Access Accounts\n[2] View Personal Information\n[3] Change Information\n[4] Open New Account\n[5] Log Out";
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
                bool doAccountsExist = DataHandler::clientDisplayAccounts(userID);

                if (!doAccountsExist) //won't display anything
                {
                    cout << "There is no account data to access" << endl << endl;
                }
                else //accounts have been displayed by DataHandler already
                {
                    string accountInterface = "[1] Deposit Into Account\n[2] Withdraw From Account\n[3] Deposit Into External Account\n[4] View an Accounts History\n[5] Go Back";
                    cout << accountInterface << endl << "Option: ";
                    int accountInterfaceOption = getUserOption(5);
                    cout << endl;
                    
                    if (accountInterfaceOption != 5)
                    {
                        vector<string> acctList = DataHandler::clientGetAccountList(userID);
                        clientHelpAccessAccount(user, accountInterfaceOption, acctList);
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
                    clientMakeAccountChanges(user, option);
                }
                cout << endl;
                break;
            }
            case 4: //open new acct
            {
                string acctType = "a cool account type";
                //Make sure to display all types of accts currently offered
                //Upon user choosing, send request off to an official for confirmation!
                //have all officials be able to access this list of requests!
                //Each request needs: UserID requesting and acct type
                //Upon accpeting request or denying request for new acct, update client recent activity message to display the choice

                DataHandler::clientRequestNewAccount(userID, acctType);
                user.setRecentActivity("Requested New Account: " + acctType);
                cout << "Please be aware that a Bear Bank Official may take several business days to review this request." << endl << endl;
                break;
            }
            case 5: //Log out
            {
                wantsToExit = true;
                user.saveUser();
            }
        }
    }
}


/************************************************
Start of Client Login Helper Functions
************************************************/


//Needs Finishing!
void clientMakeAccountChanges(Client &user, int option)
{
    switch (option)
    {
        case 1: //change first name
        {
            string name = "", oldName = user.getName(), lastName = user.getName(), userID = user.getID();
            oldName = oldName.substr(0, oldName.find(" "));
            lastName = lastName.substr(lastName.find(" ") + 1, string::npos);

            //changing name in user obj
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
            admin.setRecentLogin(DateTools().getCurrentTime()); //setting most recent login time as now
            admin.resetPassword(user.getID(), newPassword);
            admin.setRecentActivity("Assisted Client '" + user.getID() + "' Change Password in Settings");

            user.setRecentActivity("Password was Reset in Settings");
            user.setPassword(EncryptionBox::hash(newPassword)); //reflecting change inside user obj
            user.saveUser();
            cout << "Your password has been reset." << endl;
            break;
        }
    }
    user.saveUser();
}

//Needs Finishing!
void clientHelpAccessAccount(Client &user, int option, vector<string> acctList)
{
    // promt user to chose account 
    cout << "Choose from the following accounts: " << endl;
    for(int i = 2; i < acctList.length(); i++)
        cout << "[" << i-1 << "] " << acctList[i] << endl;

    int acctChoice = getUserOption(acctList.length());

    Account acct(acctList[acctChoice+1]);

    cout << acct.getAccountInfo() << endl;

    
    
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
            //user.getAccountHistory(acct); //Needs to be finished!!!
            break;
        }
    }
}

