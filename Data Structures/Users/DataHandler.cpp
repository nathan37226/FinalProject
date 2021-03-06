DataHandler::DataHandler()
{
	;
}

string DataHandler::isValidLogin(string userID, string password)
{
	vector<string> userInfo = DataHandler::allTables.userTable.returnMappedItems(userID);   //userInfo is formatted: username -> {hashedPassword, userType, accts...}
	
	string hashedInputPassword = "", savedHashPassword = "", userType = "";
	if (userInfo.size() == 0) //i.e. no info returned from userTable for that username
	{
		return "false";
	}
	else
	{
		savedHashPassword = userInfo[0]; 
		userType = userInfo[1];
		hashedInputPassword = EncryptionBox::hash(password); 

		if (savedHashPassword == hashedInputPassword) //successful login!
		{
			return userType;
		}
		else
		{
			return "false";
		}
	}
}

bool DataHandler::isValidUserID(string userID)
{
	bool value = (DataHandler::allTables.userTable.returnMappedItems(userID).size() == 0) ? false : true;
    return value;  //above ternary will return a vect of {} is userID not found, so if size == 0 then userID not found, which means the userID is avaliable
}

bool DataHandler::isAvaliableUserID(string userID)
{
	bool value = (DataHandler::allTables.userTable.returnMappedItems(userID).size() == 0) ? true : false;
    return value; //if the found vector is {}, then userID not present in table, so userID is avaliable
}

/************************************************
Functs called by Client
************************************************/

string DataHandler::getAccountInfo(string acctNum)
{
	string acctInfo = allTables.accountTable.search(acctNum); //for easier access to data
	return acctInfo;
}

void DataHandler::changeClientFirstName(string userID, string oldName, string newName)
{
	//Need to update first name table and account table
	vector<string> acctList = allTables.userTable.returnMappedItems(userID);

	for (int i = 2; i < acctList.size(); i++) //vector is formatted {hashedPw, user type, acct1, acct2, etc} for however many accts a client has
	{
		allTables.firstNameTable.delMappedItem(oldName, acctList[i]); //removes acct from old name node
		allTables.firstNameTable.insertWithItem(newName, acctList[i]); //inserts to new name node the acct

		Account acct = Account(acctList[i]); //build account from .txt file with acctList[i] as the acctNum
		acct.setAccountHolderFirstName(newName);
		acct.saveToFile(); //save to .txt file
		allTables.accountTable.updateInfo(acctList[i], acct.getAccountTableInfo());
	}
}

void DataHandler::changeClientLastName(string userID, string oldName, string newName)
{
	//Need to update first name table and account table
	vector<string> acctList = allTables.userTable.returnMappedItems(userID);

	for (int i = 2; i < acctList.size(); i++) //vector is formatted {hashedPw, user type, acct1, acct2, etc} for however many accts a client has
	{
		allTables.lastNameTable.delMappedItem(oldName, acctList[i]); //removes acct from old name node
		allTables.lastNameTable.insertWithItem(newName, acctList[i]); //inserts to new name node the acct

		Account acct = Account(acctList[i]); //build account from .txt file with acctList[i] as the acctNum
		acct.setAccountHolderLastName(newName);
		acct.saveToFile(); //save to .txt file
		allTables.accountTable.updateInfo(acctList[i], acct.getAccountTableInfo());
	}
}

void DataHandler::changeClientAddress(string userID, string oldAddress, string newAddress)
{
	//need to change inside each acct obj as well as the addressTable
	vector<string> acctList = allTables.userTable.returnMappedItems(userID);
	 for (int i = 2; i < acctList.size(); i++)
	 {
		allTables.addressTable.delMappedItem(oldAddress, acctList[i]);
		allTables.addressTable.insertWithItem(newAddress, acctList[i]);

		Account acct = Account(acctList[i]); //build account from .txt file with acctList[i] as the acctNum
		acct.setAccountHolderAddress(newAddress);
		acct.saveToFile(); //save to .txt file
		allTables.accountTable.updateInfo(acctList[i], acct.getAccountTableInfo());
	 }
}

void DataHandler::changeClientPhoneNum(string userID, string oldNum, string newNum)
{
	//need to change inside each acct obj as well as the phoneNumTable
	vector<string> acctList = allTables.userTable.returnMappedItems(userID);
	 for (int i = 2; i < acctList.size(); i++)
	 {
		allTables.phoneNumTable.delMappedItem(oldNum, acctList[i]);
		allTables.phoneNumTable.insertWithItem(newNum, acctList[i]);

		Account acct = Account(acctList[i]); //build account from .txt file with acctList[i] as the acctNum
		acct.setAccountHolderAddress(newNum);
		acct.saveToFile(); //save to .txt file
		allTables.accountTable.updateInfo(acctList[i], acct.getAccountTableInfo());
	 }
}

void DataHandler::clientRequestNewAccount(string userID, string acctType)
{
	AccountQueue queue;
	queue.buildQueue(); //preserves encryption, o.w. just append to .txt file
	string newEntry =  userID + " requests a new: " + acctType;
	queue.enqueue(newEntry);
	queue.saveQueue(); //rewrites textfile with new entry
}

bool DataHandler::clientDisplayAccounts(string userID)
{
	vector<string> acctList = DataHandler::allTables.userTable.returnMappedItems(userID); //formatted {hashedPw, user type, acct 1, acct 2, etc}
	if (acctList.size() == 2) //i.e. no present accounts
	{
		return false;
	}
	else
	{
		for (int i = 2; i < acctList.size(); i++)
		{
			string acctInfo = allTables.accountTable.search(acctList[i]);
			acctInfo = acctInfo.substr(0, acctInfo.find(".") + 3); //shortening so extra info is not added
			cout << acctList[i] << endl;
			cout << acctInfo << endl; //short snippet of acct
		}
		cout << endl;
		return true;
	}
}

vector<string> DataHandler::clientGetAccountList(string userID)
{
	vector<string> accountInfo = allTables.userTable.returnMappedItems(userID); //formatted {hashedPw, user type, acct 1, acct 2, etc} 
	return accountInfo;
}

/************************************************
Functs called by Official
************************************************/

void DataHandler::addClientAccountToRecords(Client &user, Account &acct)
{
	allTables.firstNameTable.insertWithItem(acct.getAccountHolderFirstName(), acct.getAccountNumber());
	allTables.lastNameTable.insertWithItem(acct.getAccountHolderLastName(), acct.getAccountNumber());
	allTables.addressTable.insertWithItem(user.getAddress(), acct.getAccountNumber());
	allTables.phoneNumTable.insertWithItem(user.getPhoneNum(), acct.getAccountNumber());
	allTables.userTable.insertWithItem(user.getID(), acct.getAccountNumber());

	accountEntry newEntry(acct.getAccountNumber(), acct.getAccountTableInfo()); //implemenent getAcctInfo later on!
	allTables.accountTable.insert(newEntry);
}

void DataHandler::updateAccountInfo(string acctNum, string newInfo)
{
	allTables.accountTable.updateInfo(acctNum, newInfo);
}

/************************************************
Functs called by Admin
************************************************/

void DataHandler::addOfficialToRecords(string hashedPw, string ID)
{
	vector<string> userData = {hashedPw, "official"};
	allTables.userTable.insertWithList(ID, userData); //creates record of Official inside table; allows login to occur
}

void DataHandler::addAdminToRecords(string hashedPw, string ID)
{
	vector<string> userData = {hashedPw, "admin"};
	allTables.userTable.insertWithList(ID, userData); //creates record of Official inside table; allows login to occur
}

vector<string> DataHandler::getLoginInfo(string userID)
{
	vector<string> accountInfo = allTables.userTable.returnMappedItems(userID); //formatted {hashedPw, user type} 
	return accountInfo;
}

/************************************************
AccountType operations
************************************************/

void DataHandler::createInitialAccountTypes()
{
	AccountType checkingAccount("Basic Checking", 0.0, 0.0, 0.0, -50.0);
    AccountType savingsAccount("Entry Saving", 0.0, 0.0, 2.0, 0.0);
    AccountType CD("Certificate of Deposit", 0.0, 0.0, 5.0, 0.0);
    DataHandler::accountTypeList.push_back(checkingAccount);
    DataHandler::accountTypeList.push_back(savingsAccount);
    DataHandler::accountTypeList.push_back(CD);
}

void DataHandler::displayAccountTypes()
{
	for (int i = 0; i < accountTypeList.size(); i++)
	{
		string line = "[" + to_string(i + 1) + "]" + " " + accountTypeList[i].getAccountTypeName();
		cout << line << endl;
	}
}

void DataHandler::saveAccountTypes()
{
	ofstream outFile("AccountData/AccountTypes.txt");
	if (outFile)
	{
		for (int i = 0; i < accountTypeList.size(); i++)
		{
			EncryptionBox::positionInFile = 0;
			outFile << EncryptionBox::encrypt(accountTypeList[i].getAccountTypeName()) << endl;
		}
	}
	else
	{
		cerr << "The file could not be openned at: " << endl << "AccountData/AccountTypes.txt" << endl;
	}
	outFile.close();
}

void DataHandler::buildAccountTypesFomeFile()
{
	ifstream inFile;
	inFile.open("AccountData/AccountTypes.txt");
	if (inFile)
	{
		string line = "";
		while (getline(inFile, line))
		{
			EncryptionBox::positionInFile = 0;
			line = line.substr(0, line.rfind("\r"));
			line = EncryptionBox::decrypt(line);
			AccountType acctType(line);
			accountTypeList.push_back(acctType);
		}
	}
	else //nothing saved, so create initial ones!
	{
		createInitialAccountTypes();
	}
	inFile.close();
}

void DataHandler::createAccountType(string acctTypeName, double monFee, double servFee, double interestR, double minBalance)
{
	AccountType newType(acctTypeName, monFee, servFee, interestR, minBalance);
	accountTypeList.push_back(newType);
}

bool DataHandler::deleteAccountType(string acctTypeName)
{
	if(acctTypeName=="Basic Checking"||acctTypeName=="Entry Saving"||acctTypeName=="Certificate of Deposit")
	{
		cout<<"Cannot delete it!"<<endl;
		return false;
	}
	for (int i = 0; i < accountTypeList.size(); i++)
	{
		if (accountTypeList[i].getAccountTypeName() == acctTypeName)
		{
			accountTypeList.erase(accountTypeList.begin() + i);
			cout << "Delete Success!" << endl;
			return true;
		}
	}
	cout<<"The Account Type does not Exist! "<<endl;
	return false;
}

bool DataHandler::checkAccountTypeName(string acctTypeName)
{
	for (int i = 0; i < accountTypeList.size(); i++)
	{
		if (accountTypeList[i].getAccountTypeName() == acctTypeName)
		{
			return true;
		}
	}
	return false;
}

bool DataHandler::alterAccountType(string acctTypeName, double monFee, double servFee, double interestR, double minBalance)
{
	for (int i = 0; i < accountTypeList.size(); i++)
	{
		if (accountTypeList[i].getAccountTypeName() == acctTypeName)
		{
			accountTypeList[i].setMonthlyFee(monFee);
			accountTypeList[i].setServiceFee(servFee);
			accountTypeList[i].setInterestRate(interestR);
			accountTypeList[i].setMinimumBalance(minBalance);
			return true;
		}
	}
	return false;
}

bool DataHandler::nameValid(string acctTypeName)
{
	for (int i=0;i<acctTypeName.size();i++)
	{
		if(!isalpha(acctTypeName[i])&&acctTypeName[i]!=' ')
		{
			cout<<"Invalid Account Type Name"<<endl;
			return false;
		}
	}
	return true;
}
