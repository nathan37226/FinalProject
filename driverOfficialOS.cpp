/*
Nathan Obert M03134502
All functions related to an Official logging into Bear Bank
*/

void officialLogin(string userID);
void officialOpenAccounts(Official &officialUser);
void officialSearch(Official &officialUser);
void officialAlterAccount(Official &officialUser, string acctNum);


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
                    string acctNumToClose = "", acctToCloseInfo = "";
                    cout << "Enter the Account Number: ";
                    getline(cin, acctNumToClose);
                    acctToCloseInfo = DataHandler::getAccountInfo(acctNumToClose);
                    cout << endl;

                    if (acctToCloseInfo == "false")
                    {
                        cout << "Invalid Account Number Entered" << endl;
                    }
                    else
                    {
                        if (acctToCloseInfo.substr(0, 1) == "C")
                        {
                            cout << "The Account is Already Closed" << endl;
                        }
                        else //actually closing the account
                        {
                            Account acctToClose(acctNumToClose);
                            acctToClose.setCloseDate( time(0) );
                            acctToClose.setOpenStatus(false); //actually closing
                            acctToClose.setAccountClosedBy(user.getName() + ", ID: " + user.getID());
                            acctToClose.saveToFile();
                            string clientID = acctToClose.getAccountHolderUserID();

                            Client clientUser;
                            clientUser.buildUser("UserData/" + clientID + ".txt");
                            clientUser.setRecentActivity("Account: " + acctNumToClose + " has been Closed");
                            clientUser.saveUser();
                            user.setRecentActivity("Closed Account: " + acctNumToClose);
                            user.saveUser();

                            DataHandler::updateAccountInfo(acctNumToClose, acctToClose.getAccountTableInfo()); //refreshes current info of acct inside table

                            cout << "Account: " + acctNumToClose + " has been Closed" << endl;
                        }
                    }
                    cout << endl;
                    break;
                }
                case 3: //Alter existing acct - changing things like interest rate, term length, maturity date, open status, etc etc
                {
                    string acctNum = "", acctInfo = "";
                    cout << "Enter the Account Number: ";
                    getline(cin, acctNum);
                    acctInfo = DataHandler::getAccountInfo(acctNum);

                    if (acctInfo == "false")
                    {
                        cout << "The Account Could not be Found" << endl;
                    }
                    else if (acctInfo.substr(0, 1) == "C")
                    {
                        cout << "The Account is Closed and Cannot be Altered" << endl;
                    }
                    else
                    {
                        officialAlterAccount(user, acctNum);
                    }
                    cout << endl;
                    break;
                }
                case 4: //Deposit into acct - needs user confirmation
                {
                    string acctNum = "", acctInfo = "";
                    cout << "Enter the Account Number: ";
                    getline(cin, acctNum);
                    acctInfo = DataHandler::getAccountInfo(acctNum);
                    cout << endl;

                    if (acctInfo == "false")
                    {
                        cout << "The Account Could not be Found" << endl;
                    }
                    else if (acctInfo.substr(0, 1) == "C")
                    {
                        cout << "The Account is Closed" << endl;
                    }
                    else //making the deposit
                    {
                        Account clientAcct(acctNum);
                        string enteredClientPw = "", clientHashedPw = DataHandler::clientGetAccountList(clientAcct.getAccountHolderUserID())[0]; // formatted {hashedPw, user type, acct1, acct2, etc}, so 0th index is what we want
                        cout << "Enter the Client's Password: ";
                        getline(cin, enteredClientPw);

                        if (EncryptionBox::hash(enteredClientPw) != clientHashedPw)
                        {
                            cout << endl << "This Deposit does not have the Requiste Authorization" << endl << "Deposit Cancelled" << endl;
                        }
                        else //valid pw!
                        {
                            string depositAmount = "";
                            cout << "Enter the Amount to Deposit:" << endl << "$";
                            getline(cin, depositAmount);

                            bool isValidAmount = isValidNumber(depositAmount);
                            if ( (!isValidAmount) || (Account::roundNum(stod(depositAmount), 2) != stod(depositAmount)) )
                            {
                                cout << "Invalid Amount Entered" << endl << "Deposit Cancelled" << endl;
                            }                           
                            else //making deposit happen!
                            {
                                Account clientAcct(acctNum);
                                clientAcct.deposit( stod(depositAmount) );
                                clientAcct.saveToFile();
                                DataHandler::updateAccountInfo(acctNum, clientAcct.getAccountTableInfo());

                                Client clientUser;
                                clientUser.buildUser("UserData/" + clientAcct.getAccountHolderUserID() + ".txt");
                                clientUser.setRecentActivity("Official: " + user.getID() + " Deposited $" + depositAmount + " into: " + acctNum);
                                clientUser.saveUser();
                                user.setRecentActivity("Deposited $" + depositAmount + " into: " + acctNum);
                                user.saveUser();
                                cout << endl << "Successfully Deposited $" + depositAmount << endl;
                            }
                        }
                    }
                    cout << endl;
                    break;
                }
                case 5: //Withdraw from acct - needs user confirmation
                {
                    string acctNum = "", acctInfo = "";
                    cout << "Enter the Account Number: ";
                    getline(cin, acctNum);
                    acctInfo = DataHandler::getAccountInfo(acctNum);
                    cout << endl;

                    if (acctInfo == "false")
                    {
                        cout << "The Account Could not be Found" << endl;
                    }
                    else if (acctInfo.substr(0, 1) == "C")
                    {
                        cout << "The Account is Closed" << endl;
                    }
                    else //making the withdrawal
                    {
                        Account clientAcct(acctNum);
                        string enteredClientPw = "", clientHashedPw = DataHandler::clientGetAccountList(clientAcct.getAccountHolderUserID())[0]; // formatted {hashedPw, user type, acct1, acct2, etc}, so 0th index is what we want
                        cout << "Enter the Client's Password: ";
                        getline(cin, enteredClientPw);

                        if (EncryptionBox::hash(enteredClientPw) != clientHashedPw)
                        {
                            cout << endl << "This Withdrawal does not have the Requiste Authorization" << endl << "withdrawal Cancelled" << endl;
                        }
                        else //valid pw!
                        {
                            string withdrawalAmount = "";
                            cout << "Enter the Amount to Withdraw:" << endl << "$";
                            getline(cin, withdrawalAmount);

                            bool isValidAmount = isValidNumber(withdrawalAmount);
                            if ( (!isValidAmount) || (Account::roundNum(stod(withdrawalAmount), 2) != stod(withdrawalAmount)) )
                            {
                                cout << "Invalid Amount Entered" << endl << "withdrawal Cancelled" << endl;
                            }                           
                            else //making withdrawal happen!
                            {
                                Account clientAcct(acctNum);
                                string withdrawalMsg = clientAcct.withdraw( stod(withdrawalAmount) );
                                DataHandler::updateAccountInfo(acctNum, clientAcct.getAccountTableInfo());

                                Client clientUser;
                                clientUser.buildUser("UserData/" + clientAcct.getAccountHolderUserID() + ".txt");

                                //Several cases can occur when withdrawing from an acct

                                if (withdrawalMsg == "Insufficient Funds")
                                {
                                    clientUser.setRecentActivity("Attempted withdrawal for: $" + withdrawalAmount + " Failed for Insufficient Funds");
                                    user.setRecentActivity("Attempted to Help Client: " + clientUser.getID() + " withdraw $" + withdrawalAmount + "from: " + acctNum);
                                    cout << endl << withdrawalMsg << endl << "withdrawal Cancelled" << endl;
                                }
                                else if (withdrawalMsg == "Overdraft Penalty")
                                {
                                    clientUser.setRecentActivity("Attempted withdrawal for: $" + withdrawalAmount + " Failed for Insufficient Funds and Incurred an Overdraft Penalty");
                                    user.setRecentActivity("Attempted to Help Client: " + clientUser.getID() + " withdraw $" + withdrawalAmount + "from: " + acctNum);
                                    cout << endl << withdrawalMsg << endl << "withdrawal Cancelled" << endl;
                                }
                                else if (withdrawalMsg == "Amount Withdrawn")
                                {
                                    clientUser.setRecentActivity("Withdrew $" + withdrawalAmount);
                                    user.setRecentActivity("Assisted Client: " + clientUser.getID() + " withdraw $" + withdrawalAmount + "from: " + acctNum);
                                    cout << endl << withdrawalMsg << endl << "Withdrawal Successful" << endl;
                                    if (clientAcct.getServiceFee() > 0.00)
                                    {
                                        clientAcct.doServiceCharge();
                                        cout << "You have been Charged a Service Fee of: $" << Account::getDisplayNum(clientAcct.getServiceFee()) << endl;
                                    }
                                }
                                else if (withdrawalMsg == "Account Restricted")
                                {
                                    clientUser.setRecentActivity("Attempted withdrawal for: $" + withdrawalAmount + " Failed due to being a Restricted Account");
                                    user.setRecentActivity("Attempted to Help Client: " + clientUser.getID() + " withdraw $" + withdrawalAmount + "from: " + acctNum);
                                    cout << endl << withdrawalMsg << endl << "withdrawal Cancelled" << endl;
                                }
                                else
                                {
                                    cout << "An Error Occurred" << endl << "withdrawal Cancelled" << endl;
                                }
                                clientUser.saveUser();
                                user.saveUser();
                                clientAcct.saveToFile();
                            }
                        }
                    }
                    cout << endl;
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
    string openAcctInterface = "[1] Open a new Account\n[2] View User Requested Accounts\n[3] Open a Closed Account\n[4] Go Back";
    string requestedAcctInterface = "[1] Approve Request\n[2] Deny Request\n[3] Go Back";
    bool wantsToExit = false;
    
    while (!wantsToExit)
    {
        cout << openAcctInterface << endl << "Option: ";
        int mainOption = getUserOption(4);
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
                            double newBal = newAccount.getMinimumBalance();
                            if (newBal > 0)
                            {
                                newAccount.deposit(newBal);
                            }
                            
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

                        cout << request << endl << endl;

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
                                double newBal = newAccount.getMinimumBalance();
                                if (newBal > 0)
                                {
                                    newAccount.deposit(newBal);
                                }

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
            case 3: //Open an already closed acct
            {
                string closedAcctNum = "", closedAcctInfo = "";
                cout << "Enter the Account Number: ";
                getline(cin, closedAcctNum);
                closedAcctInfo = DataHandler::getAccountInfo(closedAcctNum);
                cout << endl;

                if (closedAcctInfo == "false")
                {
                    cout << "Invalid Account Number Entered" << endl;
                }
                else
                {
                    if (closedAcctInfo.substr(0, 1) == "O")
                    {
                        cout << "The Account is Already Open" << endl;
                    }
                    else //openning the account
                    {
                        Account acctToOpen(closedAcctNum);
                        acctToOpen.setCloseDate( 0 );
                        acctToOpen.setOpenStatus(true); //actually closing
                        acctToOpen.setAccountClosedBy("N/A");
                        acctToOpen.saveToFile();
                        string clientID = acctToOpen.getAccountHolderUserID();

                        Client clientUser;
                        clientUser.buildUser("UserData/" + clientID + ".txt");
                        clientUser.setRecentActivity("Account: " + closedAcctNum + " has been Opened");
                        clientUser.saveUser();
                        officialUser.setRecentActivity("Opened Account: " + closedAcctNum);
                        officialUser.saveUser();

                        DataHandler::updateAccountInfo(closedAcctNum, acctToOpen.getAccountTableInfo()); //refreshes current info of acct inside table

                        cout << "Account: " + closedAcctNum + " has been Opened" << endl;
                    }
                }
                cout << endl;
                break;
            }
            case 4: //Go back
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
        cout << endl;

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
            case 7: //closed acct display
            {
                string acctNum = "";
                cout << "Enter an Account Number: ";
                getline(cin, acctNum);
                cout << endl;
                officialUser.searchForClosedAcct(acctNum);
                officialUser.setRecentActivity("Searched for a Closed Bank Account");
                officialUser.saveUser();
                cout << endl;
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

void officialAlterAccount(Official &officialUser, string acctNum)
{
    string clientID = "", alterInterface = "[1] Alter Interest Rate\n[2] Alter Restricted Status\n[3] Alter Monthly Fee\n[4] Alter Service Fee\n[5] Go Back";
    bool wantsToExit = false;

    Account clientAcct(acctNum);
    clientID = clientAcct.getAccountHolderUserID();

    Client clientUser;
    clientUser.buildUser("UserData/" + clientID + ".txt");

    cout << endl;
    while (!wantsToExit)
    {
        cout << alterInterface << endl << "Option: ";
        int alterOption = getUserOption(5);

        switch (alterOption)
        {
            case 1: //int rate
            {
                string newRate = "";
                cout << endl << "Enter the New Interest Rate: ";
                getline(cin, newRate);
                bool isValidRate = isValidNumber(newRate);
                if (!isValidRate)
                {
                    cout << "Invalid Rate Entered" << endl;
                }
                else
                {
                    isValidRate = clientAcct.setInterestRate( stod(newRate) ); //only occur between 0 and 5, returns boolean value if occured or not
                    if (isValidRate)
                    {
                        officialUser.setRecentActivity("Altered Interest Rate on: " + acctNum + " to: " + newRate);
                        clientUser.setRecentActivity("New Interest Rate of: " + newRate + " on: " + acctNum);
                        officialUser.saveUser();
                        clientUser.saveUser();
                        cout << "Interest Rate Altered" << endl;
                    }
                    else
                    {
                        cout << "The Interest Rate Must be Between 0% and 5%" << endl;
                    }
                }
                cout << endl;
                break;
            }
            case 2: //status
            {
                string accountStatusOptions = "[1] Make Unrestricted\n[2] Make Restricted\n[3] Go Back";
                cout << endl << accountStatusOptions << endl << "Option: ";
                int statusOption = getUserOption(3);

                if (statusOption != 3)
                {
                    bool newStatus = (statusOption == 1) ? false : true;
                    bool existingStatus = clientAcct.getRestrictedStatus();

                    if (newStatus == existingStatus)
                    {
                        cout << endl << "The Account is in that Status Already" << endl;
                    }
                    else if (!newStatus) //if 0, or false
                    {
                        clientAcct.setRestrictedStatus(false);
                        clientAcct.saveToFile();
                        officialUser.setRecentActivity("Unrestricted: " + acctNum);
                        clientUser.setRecentActivity("Account: " + acctNum + " has been Unrestricted");
                        officialUser.saveUser();
                        clientUser.saveUser();
                        cout << endl << "The Account: " + acctNum + " has been Unrestricted" << endl;
                    }
                    else
                    {
                        clientAcct.setRestrictedStatus(true);
                        clientAcct.saveToFile();
                        officialUser.setRecentActivity("Restricted: " + acctNum);
                        clientUser.setRecentActivity("Account: " + acctNum + " has been Restricted");
                        officialUser.saveUser();
                        clientUser.saveUser();
                        cout << endl << "The Account: " + acctNum + " has been Restricted" << endl;
                    }
                }
                cout << endl;
                break;
            }
            case 3: //monthly fee
            {
                string newFee = "";
                cout << endl << "Enter the New Monthly Fee: ";
                getline(cin, newFee);
                bool isValidFee = isValidNumber(newFee);
                if (!isValidFee)
                {
                    cout << "Invalid Fee Entered" << endl;
                }
                else
                {
                    double fee = Account::roundNum(stod(newFee), 2);
                    if (fee != stod(newFee))
                    {
                        cout << "Invalid Fee Entered" << endl;
                    }
                    else if (fee > 30)
                    {
                        cout << "Monthly Fee Alteration can only Occur for Amounts Between $0.00 and $30.00, Inclusive" << endl;
                    }
                    else //change fee!
                    {
                        clientAcct.setMonthlyFee(fee);
                        officialUser.setRecentActivity("Altered Monthly Fee on: " + acctNum + " to: " + newFee); //newFee is str version of fee
                        clientUser.setRecentActivity("New Monthly Fee of: " + newFee + " on: " + acctNum);
                        officialUser.saveUser();
                        clientUser.saveUser();
                        cout << "Monthly Fee Altered" << endl;
                    }  
                }
                cout << endl;
                break;
            }
            case 4: //service fee
            {
                string newFee = "";
                cout << endl << "Enter the New Service Fee: ";
                getline(cin, newFee);
                bool isValidFee = isValidNumber(newFee);
                if (!isValidFee)
                {
                    cout << "Invalid Fee Entered" << endl;
                }
                else
                {
                    double fee = Account::roundNum(stod(newFee), 2);
                    if (fee != stod(newFee))
                    {
                        cout << "Invalid Fee Entered" << endl;
                    }
                    else if (fee > 15)
                    {
                        cout << "Service Fee Alteration can only Occur for Amounts Between $0.00 and $15.00, Inclusive" << endl;
                    }
                    else //change fee!
                    {
                        clientAcct.setServiceFee(fee);
                        officialUser.setRecentActivity("Altered Service Fee on: " + acctNum + " to: " + newFee); //newFee is str version of fee
                        clientUser.setRecentActivity("New Service Fee of: " + newFee + " on: " + acctNum);
                        officialUser.saveUser();
                        clientUser.saveUser();
                        cout << "Service Fee Altered" << endl;
                    }  
                }
                cout << endl;
                break;
            }
            case 5: //go back
            {
                wantsToExit = true;
                break;
            }
        }
    }

    DataHandler::updateAccountInfo(acctNum, clientAcct.getAccountTableInfo());
    clientAcct.saveToFile();
}
