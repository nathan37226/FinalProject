/*
Nathan Obert M03134502
All functions related to an Official logging into Bear Bank
*/

void officialLogin(string userID);
void officialOpenAccounts(Official &officialUser);
void officialSearch(Official &officialUser);
void officialAlterAccount(Official &officialUser);


/************************************************
Start of Official Login
************************************************/


void officialLogin(string userID)
{
    Official user;
    user.buildUser("UserData/" + userID + ".txt");

    if (user.getState() != "active")
    {
        cout << "This account has been locked." << endl << endl;
    }
    else
    {
        cout << "Welcome, " << user.getName() << endl;
        cout << "Last Activity: " << user.getRecentActivity() << endl;
        cout << "Last Login: " << user.getRecentLogin() << endl << endl;
        user.setRecentLogin(DateTools().getCurrentTime());

        string officialInterface = "[1] Open Account\n[2] Close Account\n[3] Alter Existing Account\n[4] Deposit into Account\n[5] Withdraw from Account\n[6] Search for Bank Accounts\n[7] Log Out";
        bool wantsToExit = false;
        
        while (!wantsToExit)
        {
            cout << officialInterface << endl << "Option: ";
            int initialOption = getUserOption(7);
            cout << endl;

            switch (initialOption)
            {
                case 1: //Open acct - look at queue or just open new acct for specified user
                {
                    officialOpenAccounts(user); //abstracted due to needed while loop
                    break;
                }
                case 2: //Close acct
                {
                    break;
                }
                case 3: //Alter existing acct - changing things like interest rate, term length, maturity date, open status, etc etc
                {
                    //build acct
                    //check if open
                    //make changes, if applicable to acct type
                    //update DataHandler.allTables.accountTable to have new info
                    //save acct
                    officialAlterAccount(user);
                    break;
                }
                case 4: //Deposit into acct - needs user confirmation
                {
                    break;
                }
                case 5: //Withdraw from acct - needs user confirmation
                {
                    break;
                }
                case 6: //Search for acct by userID, first name, last name, phone num, address, acct num
                {
                    officialSearch(user);
                    break;
                }
                case 7: //Exit
                {
                    wantsToExit = true;
                    user.saveUser();
                    break;
                }
            }
        }
    }
}


/************************************************
Start of Official Login Helper Functions
************************************************/


void officialOpenAccounts(Official &officialUser)
{
    string openAcctInterface = "[1] Open a new Account\n[2] View User Requested Accounts\n[3] Go Back";
    string requestedAcctInterface = "[1] Approve Request\n[2] Deny Request\n[3] Go Back";
    bool wantsToExit = false;
    
    while (!wantsToExit)
    {
        cout << openAcctInterface << endl << "Option: ";
        int mainOption = getUserOption(3);
        cout << endl;
        
        switch (mainOption)
        {
            case 1: //Open new acct
            {
                cout << "Enter the User ID of the Recipient of this Account: ";
                string clientID = "";
                getline(cin, clientID);
                cout << endl;

                if (!DataHandler::isValidUserID(clientID))
                {
                    cout << "Entered User ID is Invalid" << endl << endl;
                }
                else
                {
                    string userType = DataHandler::clientGetAccountList(clientID)[1];
                    if (userType != "client")
                    {
                        cout << "Entered User ID does not Belong to a Client" << endl << endl;
                    }
                    else
                    {
                        int maxOption = DataHandler::accountTypeList.size() + 1;
                        DataHandler::displayAccountTypes();
                        cout << "[" + to_string(maxOption) + "] Go Back" << endl;
                        cout << "Enter an Option: ";
                        int accountTypeOption = getUserOption(maxOption);
                        cout << endl;

                        if (accountTypeOption != maxOption)
                        {
                            Client clientUser;
                            clientUser.buildUser("UserData/" + clientID + ".txt");
                            string name = clientUser.getName(), firstName = "", lastName = "", address = clientUser.getAddress(), phoneNum = clientUser.getPhoneNum(), acctType = DataHandler::accountTypeList[accountTypeOption - 1].getAccountTypeName();
                            firstName = name.substr(0, name.find(" "));
                            lastName = name = name.substr(name.find(" ") + 1, string::npos);
                            Account newAccount(acctType, clientID, firstName, lastName, phoneNum, address); //creating new account according to prexisting account type
                            
                            officialUser.addAccountToClient(clientUser, newAccount);
                            officialUser.setRecentActivity("Added Account: " + acctType + " to User: " + clientID);
                            officialUser.saveUser();
                            clientUser.setRecentActivity("Account: " + acctType + " added!");
                            clientUser.saveUser();
                            newAccount.saveToFile();

                            cout << "Account: " + acctType + " was created!" << endl << endl;
                        }
                    }
                }
                break;
            }
            case 2: //View acct request queue
            {
                AccountQueue queue;
                queue.buildQueue();
                string request = "", userID = "", acctType = "";
                bool doneReviewing = false;

                if (queue.isEmpty())
                {
                    cout << "There are no account openning requests to view" << endl << endl;
                    queue.saveQueue();
                    break;
                }
                else
                {
                    while (!doneReviewing)
                    {
                        queue.peekFirst(request); //request takes on first item in queues value, queue retains order
                        userID = request.substr(0, request.find(" ")); //request is formatted: "<userID>  requests a new: <acctType>"
                        acctType = request.substr(request.find(":") + 2, string::npos); //finds the : inside "a new: " and gets whatever is after
                        Client clientUser;
                        clientUser.buildUser("UserData/" + userID + ".txt");

                        cout << request << endl;

                        cout << requestedAcctInterface << endl << "Option: ";
                        int reviewOption = getUserOption(3);

                        switch (reviewOption)
                        {
                            case 1: //Approve
                            {  
                                string name = clientUser.getName(), firstName = "", lastName = "", address = clientUser.getAddress(), phoneNum = clientUser.getPhoneNum(), dummyStr = "";
                                firstName = name.substr(0, name.find(" "));
                                lastName = name = name.substr(name.find(" ") + 1, string::npos);
                                
                                Account newAccount(acctType, userID, firstName, lastName, phoneNum, address);

                                officialUser.addAccountToClient(clientUser, newAccount); //updates record in all tables!
                                clientUser.setRecentActivity("Request for: " + acctType + " was approved!");
                                clientUser.saveUser();
                                officialUser.setRecentActivity("Approved Request from: " + userID + " for: " + acctType);
                                officialUser.saveUser();
                                newAccount.saveToFile(); //creates initial text file record for the account

                                queue.dequeue(dummyStr); //dequeue needs a string& passed, so this just allows a value to be removed
                                cout << endl << "Approved Request" << endl;

                                cout << endl;
                                break;
                            }
                            case 2: //Deny
                            {
                                string dummyStr = "";
                                clientUser.setRecentActivity("Request for: " + acctType + " was denied.");
                                clientUser.saveUser();
                                officialUser.setRecentActivity("Denied Request from: " + userID + " for: " + acctType);
                                officialUser.saveUser();
                                queue.dequeue(dummyStr);
                                cout << endl << "Denied Request" << endl;

                                cout << endl;
                                break;
                            }
                            case 3: //Go Back
                            {
                                doneReviewing = true;
                                //save queue back to file
                                queue.saveQueue();
                                cout << endl;
                                break;
                            }
                        }
                        if (queue.isEmpty()) //break condiditon while reviewing
                        {
                            cout << "There are no more Requests to View" << endl << endl;
                            doneReviewing = true;
                            queue.saveQueue();
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

void officialSearch(Official &officialUser)
{
    string searchInterface = "[1] Search by User ID\n[2] Search by First Name\n[3] Search by Last Name\n[4] Search by Account Number\n[5] Search by Phone Number\n[6] Search by Address\n[7] View a Closed Account\n[8] Go Back";
    bool wantsToExit = false;

    while (!wantsToExit)
    {
        cout << searchInterface << endl << "Option: ";
        int searchOption = getUserOption(8);

        switch (searchOption)
        {
            case 1: //user id
            {
                string userID = "";
                cout << "Enter a User ID: ";
                getline(cin, userID);
                cout << endl;
                bool isValid = DataHandler::isValidUserID(userID);

                if (isValid)
                {
                    officialUser.searchByUserID(userID);
                    officialUser.setRecentActivity("Searched for Bank Accounts by a User ID");
                    officialUser.saveUser();
                }
                else
                {
                    cout << "The Entered User ID is Invalid" << endl;
                }
                
                break;
            }
            case 2: //first name
            {
                string firstName = "";
                cout << "Enter a first name: ";
                getline(cin, firstName);
                cout << endl;
                officialUser.searchByFirstName(firstName);
                officialUser.setRecentActivity("Searched for Bank Accounts by a First Name");
                officialUser.saveUser();
                break;
            }
            case 3: //last name
            {
                string lastName = "";
                cout << "Enter a last name: ";
                getline(cin, lastName);
                cout << endl;
                officialUser.searchByLastName(lastName);
                officialUser.setRecentActivity("Searched for Bank Accounts by a Last Name");
                officialUser.saveUser();
                break;
            }
            case 4: //acct num
            {
                string acctNum = "";
                cout << "Enter an Account Number: ";
                getline(cin, acctNum);
                cout << endl;
                officialUser.searchByAccountNum(acctNum);
                officialUser.setRecentActivity("Searched for a Bank Account by an Account Number");
                officialUser.saveUser();
                break;
            }
            case 5: //phone num
            {
                string phoneNum = "";
                cout << "Enter a phone number: ";
                getline(cin, phoneNum);
                cout << endl;
                officialUser.searchByPhoneNum(phoneNum);
                officialUser.setRecentActivity("Searched for Bank Accounts by a Phone Number");
                officialUser.saveUser();
                break;
            }
            case 6: //address
            {
                string address = "";
                cout << "Enter an address: ";
                getline(cin, address);
                cout << endl;
                officialUser.searchByAddress(address);
                officialUser.setRecentActivity("Searched for Bank Accounts by an Address");
                officialUser.saveUser();
                break;
            }
            case 7: //closed acct display -- must implement!!
            {
                string acctNum = "";
                cout << "Enter an Account Number: ";
                getline(cin, acctNum);
                cout << endl;
                officialUser.searchForClosedAcct(acctNum);
                officialUser.setRecentActivity("Searched for a Closed Bank Account");
                officialUser.saveUser();
                break; 
            }
            case 8: //go back
            {
                wantsToExit = true;
                cout << endl;
                break;
            }
        }
    }
}

void officialAlterAccount(Official &officialUser)
{
    string alterInterface = "[1] ";
    bool wantsToExit = false;

    while (!wantsToExit)
    {
        cout << alterInterface << endl << "Option: ";
        int alterOption = getUserOption(-2);

    }
}