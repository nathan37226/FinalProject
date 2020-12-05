/*
Nathan Obert M03134502
All functions related to a Client logging into Bear Bank
*/

void clientLogin(string userID);
void clientMakeAccountChanges(Client &user, int option);
void clientHelpAccessAccount(Client &user, int option, vector<string> acctList);
string getAccountFromList(vector<string> acctList);


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
                    int accountInterfaceOption = 0;
                    while(accountInterfaceOption != 5)
                    {
                        string accountInterface = "[1] Deposit Into Account\n[2] Withdraw From Account\n[3] Deposit Into External Account\n[4] View an Accounts History\n[5] Go Back";
                        cout << accountInterface << endl << "Option: ";
                        accountInterfaceOption = getUserOption(5);
                        cout << endl;
                        
                        if (accountInterfaceOption != 5)
                        {
                            vector<string> acctList = DataHandler::clientGetAccountList(userID);
                            clientHelpAccessAccount(user, accountInterfaceOption, acctList);
                        }
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
                int option = 0;
                while(option != 6)
                {
                    cout << "Which piece of information would you like to change?" << endl;
                    cout << "[1] First Name\n[2] Last Name\n[3] Address\n[4] Phone Number\n[5] Password\n[6] Go Back" << endl;
                    cout << "Option: ";
                    option = getUserOption(6);
                    if (option != 6)
                    {
                        cout << endl;
                        clientMakeAccountChanges(user, option);
                    }
                    cout << endl;
                }
                break;
            }
            case 4: //open new acct
            {
                //Make sure to display all types of accts currently offered
                //Upon user choosing, send request off to an official for confirmation!
                //have all officials be able to access this list of requests!
                //Each request needs: UserID requesting and acct type
                //Upon accpeting request or denying request for new acct, update client recent activity message to display the choice

                int maxOption = DataHandler::accountTypeList.size() + 1;
                DataHandler::displayAccountTypes();
                cout << "[" + to_string(maxOption) + "] Go Back" << endl;
                cout << "Option: ";
                int accountTypeOption = getUserOption(maxOption);
                cout << endl;

                if (accountTypeOption != maxOption)
                {
                    string acctType = DataHandler::accountTypeList[accountTypeOption - 1].getAccountTypeName();
                    DataHandler::clientRequestNewAccount(userID, acctType);
                    user.setRecentActivity("Requested New Account: " + acctType);
                    cout << "Please be Aware that a Bear Bank Official may take Several Business Days to Review this Request." << endl << endl;
                }
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

void clientHelpAccessAccount(Client &user, int option, vector<string> acctList)
{
    switch (option)
    {
        case 1: //deposit
        {
            // prompt user to pick account
            Account acct = Account(getAccountFromList(acctList));
            cout << "Enter Deposit amount: " << endl << "$";
            string depAmount;
            cin >> depAmount;
            try
            {
                double amount = stod(depAmount);
                // deposit
                acct.deposit(amount);
            }
            catch (const std::invalid_argument& ia){
                std::cerr << "Invalid argument: " << ia.what() << '\n';
                break;
            }
            catch (const std::out_of_range& oor){
                std::cerr << "Out of Range error: " << oor.what() << '\n';
                break;
            }
            // save transaction
            acct.saveToFile();
            DataHandler::updateAccountInfo(acct.getAccountNumber(), acct.getAccountTableInfo());
            user.setRecentActivity("Deposited: $" + depAmount + " into account: " + acct.getAccountNumber()); //save user activity
            user.saveUser();//save user
            break;
        }
        case 2: //withdraw
        {
            // prompt user to chose an account
            Account acct = Account(getAccountFromList(acctList));
            cout << "Enter Withdraw amount: " << endl << "$";
            string witAmount;
            cin >> witAmount;
            try
            {
                double amount = stod(witAmount);
                // withdraw
                acct.withdraw(amount);
            }
            catch (const std::invalid_argument& ia){
                std::cerr << "Invalid argument: " << ia.what() << '\n';
                break;
            }
            catch (const std::out_of_range& oor){
                std::cerr << "Out of Range error: " << oor.what() << '\n';
                break;
            }
            // save transaction
            acct.saveToFile();
            DataHandler::updateAccountInfo(acct.getAccountNumber(), acct.getAccountTableInfo());
            user.setRecentActivity("Withdrew: $" + witAmount + " from account: " + acct.getAccountNumber()); //save user activity
            user.saveUser();//save user
            break;
        }
        case 3: //deposit into another acct in Bear Bank
        {
            cout << "Enter the account number you wish to deposit into: " << endl;
            string otherAcctNum;
            cin >> otherAcctNum;

            if(DataHandler::getAccountInfo(otherAcctNum) != "false")
            {
                Account otherAcct = Account(otherAcctNum);
                cout << "Enter Deposit amount: " << endl << "$";
                string depAmount;
                cin >> depAmount;
                try
                {
                    double amount = stod(depAmount);
                    otherAcct.deposit(amount);
                }
                catch (const std::invalid_argument& ia){
                    std::cerr << "Invalid argument: " << ia.what() << '\n';
                    break;
                }
                catch (const std::out_of_range& oor){
                    std::cerr << "Out of Range error: " << oor.what() << '\n';
                    break;
                }
                // save transaction
                otherAcct.saveToFile();
                DataHandler::updateAccountInfo(otherAcct.getAccountNumber(), otherAcct.getAccountTableInfo());
                user.setRecentActivity("Deposited: $" + depAmount + " into account: " + otherAcct.getAccountNumber()); //save user activity
                user.saveUser();//save user
                break;
            }
            else
            {
                cout << "Account: " << otherAcctNum << " does not exist." << endl;
                break;
            } 
        }
        case 4: //view history
        {
            user.getAccountHistory(acctList);
            break;
        }
    }
}

string getAccountFromList(vector<string> acctList)
{
    string accountNum = "";
	bool isPresent = false;
	cout << endl << "Enter the Account Number: ";
	getline(cin, accountNum);
	
	for (int i = 2; i < acctList.size(); i++) //normal iteration--a BST is not needed here since so few possible options
	{
		if (accountNum == acctList[i])
		{
			isPresent = true;
			break;
		}
	}
	while (!isPresent)
	{
		cout << "Invalid Account Number." << endl << "Enter Again: ";
		getline(cin, accountNum);

		for (int i = 2; i < acctList.size(); i++) //normal iteration--a BST is not needed here since so few possible options
		{
			if (accountNum == acctList[i])
			{
				isPresent = true;
				break;
			}
		}
	}
    return accountNum;
}