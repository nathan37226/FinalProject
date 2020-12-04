/*
Nathan Obert M03134502
All functions related to an Admin logging into Bear Bank
*/

void adminLogin(string userID);
void adminModifyOfficial(Admin &admin);
void adminModifyAdmin(Admin &admin);
void adminModifyAccountTypes(Admin &admin);
void adminViewTableStats(Admin &admin);
void adminRetrieveUserID(Admin &admin);



/************************************************
Start of Admin Login
************************************************/


void adminLogin(string userID)
{
    Admin admin;
    admin.buildUser("UserData/" + userID + ".txt");
    cout << "Welcome, " << admin.getName() << endl;
    cout << "Last Activity: " << admin.getRecentActivity() << endl;
    cout << "Last Login: " << admin.getRecentLogin() << endl << endl;
    admin.setRecentLogin(DateTools().getCurrentTime());

    string adminInterface = "[1] Modify Official Account\n[2] Modify an Admin Account\n[3] Modify Account Types\n[4] Retrieve a User ID\n[5] Change a Password\n[6] View Table Statistics\n[7] Log Out";
    bool wantsToExit = false;

    while(!wantsToExit)
    {
        cout << adminInterface << endl << "Option: ";
        int initialOption = getUserOption(7);
        cout << endl;

        switch (initialOption)
        {
            case 1: //Modify Official Account
            {
                adminModifyOfficial(admin);
                break;
            }
            case 2: //Modify Admin
            {
                adminModifyAdmin(admin);
                break;
            }
            case 3: //Modify Account Types
            {
                adminModifyAccountTypes(admin);
                break;
            }
            case 4: //Retrieve User ID
            {
                adminRetrieveUserID(admin);
                break;
            }
            case 5: //Change pw
            {
                string userName = "", newPassword = "", userType = "";
                cout << "Enter the User ID: ";
                getline(cin, userName);

                if (DataHandler::isValidUserID(userName))
                {
                    cout << "Enter the New Password: ";
                    getline(cin, newPassword);
                    admin.resetPassword(userName, newPassword);
                    admin.setRecentActivity("Reset Password for: " + userName);
                    admin.saveUser();

                    //now to reflect update in other user acct
                    userType = DataHandler::getLoginInfo(userName)[1];

                    if (userType == "client")
                    {
                        Client user;
                        user.buildUser("UserData/" + userName + ".txt");
                        user.setRecentActivity("Password reset by: " + userID);
                        user.saveUser();
                    }
                    else if (userType == "official")
                    {
                        Official user;
                        user.buildUser("UserData/" + userName + ".txt");
                        user.setRecentActivity("Password reset by: " + userID);
                        user.saveUser();
                    }
                    else
                    {
                        Admin user;
                        user.buildUser("UserData/" + userName + ".txt");
                        user.setRecentActivity("Password reset by: " + userID);
                        user.saveUser();
                    }
                    cout << endl << "The Password has been Changed!" << endl << endl;;
                }
                else
                {
                    cout << "Invalid User ID Entered" << endl << endl;
                }
                
                break;
            }
            case 6: //view statistics
            {
                adminViewTableStats(admin);
                break;
            }
            case 7: //Log Out
            {
                wantsToExit = true;
                admin.saveUser();
                break;
            }
        }
    }
}


/************************************************
Start of Admin Login Helper Functions
************************************************/


void adminModifyOfficial(Admin &admin)
{
    string modifyOfficialInterface = "[1] Create a New Official\n[2] Edit an Existing Official\n[3] Go Back";
    bool wantsToExit = false;

    while (!wantsToExit)
    {
        cout << modifyOfficialInterface << endl << "Option: ";
        int modifyOfficialOption = getUserOption(3);
        cout << endl;

        switch (modifyOfficialOption)
        {
            case 1: //Create New Official
            {
                string fullName = "", userID = "", password = "";
                bool isUserIDAvailable = false;
                cout << "Enter the Official's first and last name: ";
                getline(cin, fullName);

                while (!isUserIDAvailable)
                {
                    cout << "Enter the Official's User ID: ";
                    getline(cin, userID);
                    isUserIDAvailable = DataHandler::isAvaliableUserID(userID);
                    if (!isUserIDAvailable)
                    {
                        cout << "Entered User ID not Available" << endl;
                    }
                }
                
                cout << "Enter the Official's password: ";
                getline(cin, password);

                admin.createOfficial(fullName, userID, password);
                admin.setRecentActivity("Created new official: " + userID);
                admin.saveUser();
                cout << "A New Official Account for: " << fullName << " has been created!" << endl << endl;                
                break;
            }
            case 2: //Edit Existing official
            {
                string officialUserID = "";
                cout << "Enter the User ID of the official: ";
                getline(cin, officialUserID);
                cout << endl;

                if (officialUserID == "official")
                {
                    cout << "Cannot edit the Automated Official" << endl;
                }
                else
                {
                    vector<string> userInfo = DataHandler::clientGetAccountList(officialUserID);

                    if (userInfo.size() == 0) //no results found from searching, i.e. returned vect was {}
                    {
                        cout << "User ID not found" << endl << endl;
                    }
                    else if (userInfo[1] != "official") 
                    {
                        cout << officialUserID << " is not a Bear Bank Official" << endl << endl;
                    }
                    else //found an official!
                    {
                        cout << "Official: " << officialUserID << " was found!" << endl << endl;
                        Official officialUser;
                        officialUser.buildUser("UserData/" + officialUserID + ".txt");

                        string editOfficialInterface = "[1] Set Active\n[2] Set Inactive\n[3] Delete Official\n[4] Go Back";
                        cout << editOfficialInterface << endl << "Option: ";
                        int editOfficialOption = getUserOption(4);
                        cout << endl;

                        switch (editOfficialOption)
                        {
                            case 1: //Set active
                            {
                                admin.setOfficialActive(officialUser);
                                admin.setRecentActivity("Set Official Active: " + officialUserID);
                                admin.saveUser();
                                cout << officialUserID << " has been set to: Active" << endl;
                                cout << endl;
                                break;
                            }
                            case 2: //Set inactive
                            {
                                admin.setOfficialInactive(officialUser);
                                admin.setRecentActivity("Locked out Official: " + officialUserID);
                                admin.saveUser();
                                cout << officialUserID << " has been set to: Inactive" << endl;
                                cout << endl;
                                break;
                            }
                            case 3: //Delete official
                            {
                                admin.deleteOfficial(officialUser);
                                admin.setRecentActivity("Deleted Official: " + officialUserID);
                                admin.saveUser();
                                cout << officialUserID << " has been deleted" << endl;
                                cout << endl;
                                break;
                            }
                            case 4: //Go back
                            {
                                break;
                            }
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

void adminModifyAdmin(Admin &admin)
{
    string modifyAdmin = "[1] Create a New Admin\n[2] Delete an Existing Admin\n[3] Go Back";
    bool wantsToExit = false;

    while (!wantsToExit)
    {
        cout << modifyAdmin << endl << "Option: ";
        int modifyAdminOption = getUserOption(3);
        cout << endl;

        switch (modifyAdminOption)
        {
            case 1: //new admin
            {
                string fullName = "", userID = "", password = "";
                bool isUserIDAvailable = false;
                cout << "Enter the Admin's first and last name: ";
                getline(cin, fullName);

                while (!isUserIDAvailable)
                {
                    cout << "Enter the Admin's User ID: ";
                    getline(cin, userID);
                    isUserIDAvailable = DataHandler::isAvaliableUserID(userID);
                    if (!isUserIDAvailable)
                    {
                        cout << "Entered User ID not Available" << endl;
                    }
                }
                
                cout << "Enter the Admin's password: ";
                getline(cin, password);
                admin.createAdmin(fullName, userID, password);
                admin.setRecentActivity("Created new admin: " + userID);
                admin.saveUser();
                cout << endl << "A New Admin Account for: " << fullName << " has been created!" << endl << endl;                
                break;
            }
            case 2: //delete existing admin
            {
                string userID = "";
                cout << "Enter the User ID of the Admin you want to Delete: ";
                getline(cin, userID);
                vector<string> userInfo = DataHandler::getLoginInfo(userID);

                if (userID == "admin")
                {
                    cout << endl << "Cannot delete the Automated Admin" << endl << endl;
                }
                else if (userInfo.size() == 0)
                {
                    cout << endl << "Invalid User ID" << endl << endl;
                }
                else if (userInfo[1] != "admin")
                {
                    cout << endl << "User ID does not Belong to an Admin" << endl << endl;
                }
                else
                {
                    admin.deleteAdmin(userID);
                    admin.setRecentActivity("Deleted admin: " + userID);
                    admin.saveUser();
                    cout << endl << "The Admin Account: " + userID + " has been deleted" << endl << endl;
                }
                break;
            }
            case 3: //go back
            {
                wantsToExit = true;
                break;
            }
        }
    }
}

void adminModifyAccountTypes(Admin &admin)
{
    string modifyAcctTypeInterface = "[1] View All Account Types\n[2] Create New Account Type\n[3] Delete Existing Account Type\n[4] Go Back";
    bool wantsToExit = false;

    while (!wantsToExit)
    {
        cout << modifyAcctTypeInterface << endl << "Option: ";
        int modifyAcctTypeOption = getUserOption(4);
        cout << endl;
        
        switch (modifyAcctTypeOption)
        {
            case 1: //View all
            {
                break;
            }
            case 2: //Create new
            {
                break;
            }
            case 3: //Delete existing
            {
                break;
            }
            case 4:
            {
                wantsToExit = true;
                break;
            }
        }
    }
}

void adminViewTableStats(Admin &admin)
{
    string tableStatsInterface = "[1] View User Table Statistics\n[2] View First Name Table Statistics\n[3] View Last Name Table Statistics\n[4] View Phone Number Table Statistics\n[5] View Address Table Statistics\n[6] View Account Table Statistics\n[7] Go Back";
    bool wantsToExit = false;

    while (!wantsToExit)
    {
        cout << tableStatsInterface << endl << "Option: ";
        int tableStatsOption = getUserOption(7);
        
        switch (tableStatsOption)
        {
            case 1:
            {
                DataHandler::allTables.userTable.getStatistics();
                break;
            }
            case 2: //First name
            {
                DataHandler::allTables.firstNameTable.getStatistics();
                break;
            }
            case 3: //Last name
            {
                DataHandler::allTables.lastNameTable.getStatistics();
                break;
            }
            case 4: //Phone num
            {
                DataHandler::allTables.phoneNumTable.getStatistics();
                break;
            }
            case 5: //Address
            {
                DataHandler::allTables.addressTable.getStatistics();
                break;
            }
            case 6: //Accounts
            {
                DataHandler::allTables.accountTable.getStatistics();
                break;
            }
            case 7: //Go back
            {
                wantsToExit = true;
                break;
            }
        }
        cout << endl;
    }
}

void adminRetrieveUserID(Admin &admin)
{
    string retrieveIDInterface = "[1] Display All Client User IDs\n[2] Display All Official User IDs\n[3] Display All Admin User IDs\n[4] Search for Client User ID by an Account Number\n[5] Go Back";
    bool wantsToExit = false;
    while (!wantsToExit)
    {
        cout << retrieveIDInterface << endl << "Option: ";
        int retrieveIDOption = getUserOption(5);
        cout << endl;

        switch (retrieveIDOption)
        {
            case 1:
            {
                admin.displayAllClients();
                admin.setRecentActivity("Displayed All Client User IDs");
                admin.saveUser();
                cout << endl;
                break;
            }
            case 2: //display all Officials
            {
                admin.displayAllOfficials();
                admin.setRecentActivity("Displayed All Official User IDs");
                admin.saveUser();
                cout << endl;
                break;
            }
            case 3: //display all Admins
            {
                admin.displayAllAdmins();
                admin.setRecentActivity("Displayed All Admin User IDs");
                admin.saveUser();
                cout << endl;
                break;
            }
            case 4: //Search For Client by acct num
            {
                string acctNum = "", acctInfo = "";
                cout << "Enter the Account Number: ";
                getline(cin, acctNum);
                acctInfo = DataHandler::getAccountInfo(acctNum);

                if (acctInfo == "false")
                {
                    cout << "No Results were Found" << endl;
                }
                else
                {
                    string clientID = admin.returnUserID(acctNum);
                    cout << "The User ID of " + acctNum + " is: " + clientID << endl;
                    admin.setRecentActivity("Searched for User ID through Account Number");
                    admin.saveUser();
                }
                

                cout << endl;
                break;
            }
            case 5:
            {
                wantsToExit = true;
                break;
            }
        }
    }
}
