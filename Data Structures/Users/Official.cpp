Official::Official()
{
	;
}

Official::Official(string name, string ID, string pw, string type, string login) : User(name, ID, pw, type, login)
{
	officialState = "active";
}

Official::Official(string name, string ID, string pw, string type) : User(name, ID, pw, type)
{
	officialState = "active";
}

string Official::getState()
{
	return officialState;
}

void Official::setState(string newState)
{
	officialState = newState;
}

void Official::saveUser() const
{
	EncryptionBox::positionInFile = 0;
	ofstream outFile("UserData/" + userID + ".txt");
	if (outFile)
	{
	outFile << EncryptionBox::encrypt(userID) << endl;
	outFile << EncryptionBox::encrypt(userName) << endl;
	outFile << EncryptionBox::encrypt(userPassword) << endl;
	outFile << EncryptionBox::encrypt(userType) << endl;
	outFile << EncryptionBox::encrypt(mostRecentLogin) << endl;
	outFile << EncryptionBox::encrypt(recentActivity) << endl;
	outFile << EncryptionBox::encrypt(officialState) << endl;
	}
	else
	{
		cout << "Could not save to file at: " << endl << "UserData/" + userID + ".txt" << endl;
	}

	outFile.close();
}

void Official::buildUser(string filename)
{
	EncryptionBox::positionInFile = 0;
	ifstream inFile;
	inFile.open(filename);

	try
	{	
		if (inFile)
		{
			string line = "";
			vector<string> userInfo = {};

			while (getline(inFile, line))
			{
				line = line.substr(0, line.rfind("\r")); //removing carriage return from .txt file
				line = EncryptionBox::decrypt(line);
				userInfo.push_back(line);
			}
			if (userInfo.size() < 7)
			{
				throw 1;
			}

			userID = userInfo[0]; //setting user attributes
			userName = userInfo[1];
			userPassword = userInfo[2];
			userType = userInfo[3];
			mostRecentLogin = userInfo[4];
			recentActivity = userInfo[5];
			officialState = userInfo[6];
		}
		else
		{
			cout << "Could not read from file at: " << endl << filename << endl;
		}
	}
	catch (...) //this is for the case that the default Official, Automated Official, gets deleted for some reason
	{
		recentActivity = ""; //everything else is preset in the initial login function in driver
	}
	
	inFile.close();
}

void Official::addNewClientUser(Client &user)
{
	string userID = user.getID(), hashedPw = user.getPassword();
	vector<string> userData = {hashedPw, "client"}; //no accts at first
	DataHandler::allTables.userTable.insertWithList(userID, userData); //inserts into AVLtree
}

void Official::searchByUserID(string ID)
{
	vector<string> userInfo = DataHandler::allTables.userTable.returnMappedItems(ID);

	if (userInfo[1] == "client")
	{
		for (int i = 2; i < userInfo.size(); i++) //starts at 2 since userInfo is {hashedPw, acct type, acc1, acc2, acc3, etc}
		{
			string accountInfo = DataHandler::allTables.accountTable.search(userInfo[i]); //returns short info about acct
			cout << userInfo[i] << endl << accountInfo << endl << endl;
		}
	}
	else
	{
		cout << "The Entered User ID does not Belong to a Client" << endl;
	}
}

void Official::searchByFirstName(string firstName)
{
	vector<string> searchResults = DataHandler::allTables.firstNameTable.returnMappedItems(firstName);

	if (searchResults.size() == 0) //i.e. no associated accts, or just firstName not found
	{
		cout << "No results found" << endl << endl;
	}
	else
	{
		for (int i = 0; i < searchResults.size(); i++) 
		{
			string accountInfo = DataHandler::allTables.accountTable.search(searchResults[i]); //returns short info about acct
			cout << searchResults[i] << endl << accountInfo << endl << endl;
		}
	}	
}

void Official::searchByLastName(string lastName)
{
	vector<string> searchResults = DataHandler::allTables.lastNameTable.returnMappedItems(lastName);

	if (searchResults.size() == 0) //i.e. no associated accts, or just lastName not found
	{
		cout << "No results found" << endl << endl;
	}
	else
	{
		for (int i = 0; i < searchResults.size(); i++) 
		{
			string accountInfo = DataHandler::allTables.accountTable.search(searchResults[i]); //returns short info about acct
			cout << searchResults[i] << endl << accountInfo << endl << endl;
		}
	}
}

void Official::searchByAccountNum(string acctNum)
{
	string accountInfo = DataHandler::allTables.accountTable.search(acctNum);
	if (accountInfo == "false")
	{
		cout << "No results found" << endl << endl;
	}
	else
	{
		cout << accountInfo << endl << endl;
	}
}

void Official::searchByPhoneNum(string phoneNum)
{
	vector<string> searchResults = DataHandler::allTables.phoneNumTable.returnMappedItems(phoneNum);

	if (searchResults.size() == 0) //i.e. no associated accts, or just phoneNum not found
	{
		cout << "No results found" << endl << endl;
	}
	else
	{
		for (int i = 0; i < searchResults.size(); i++) 
		{
			string accountInfo = DataHandler::allTables.accountTable.search(searchResults[i]); //returns short info about acct
			cout << searchResults[i] << endl << accountInfo << endl << endl;
		}
	}
}

void Official::searchByAddress(string address)
{
	vector<string> searchResults = DataHandler::allTables.addressTable.returnMappedItems(address);

	if (searchResults.size() == 0) //i.e. no associated accts, or just address not found
	{
		cout << "No results found" << endl << endl;
	}
	else
	{
		for (int i = 0; i < searchResults.size(); i++) 
		{
			string accountInfo = DataHandler::allTables.accountTable.search(searchResults[i]); //returns short info about acct
			cout << searchResults[i] << endl << accountInfo << endl << endl;
		}
	}
}

void Official::addAccountToClient(Client &user, Account &acct)
{
	DataHandler::addClientAccountToRecords(user, acct);
}

void Official::searchForClosedAcct(string acctNum)
{
	string accountInfo = DataHandler::allTables.accountTable.search(acctNum);
	if (accountInfo == "false")
	{
		cout << "No results found" << endl << endl;
	}
	else
	{
		if (accountInfo.substr(0, 1) == "C")
		{
			Account closedAcct(acctNum);
			cout << accountInfo.substr(0, accountInfo.rfind(" ")) << endl;
			cout << "Opened at: " << closedAcct.getOpenDate() << endl;
			cout << "Closed at: " << closedAcct.getCloseDate() << endl;
			cout << "Closed by: " << closedAcct.getAccountClosedBy() << endl;
		}
		else
		{
			cout << "The Account Entered is not Closed" << endl;
		}
	}
}