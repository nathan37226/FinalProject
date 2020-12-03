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

		/*
		build account from .txt file with acctList[i] as the acctNum
		insert new first name
		save to .txt file
		get new info for acct from built account object
		insert new info into with command: allTables.accountTable.updateInfo(<acctNum>, <newInfo>);
		*/
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

		/*
		build account from .txt file with acctList[i] as the acctNum
		insert new last name
		save to .txt file
		get new info for acct from built account object
		insert new info into with command: allTables.accountTable.updateInfo(<acctNum>, <newInfo>);
		*/
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

		/*
		build account from .txt file with acctList[i] as the acctNum
		insert new address
		save to .txt file
		get new info for acct from built account object
		insert new info into with command: 
		allTables.accountTable.updateInfo(<acctNum>, <newInfo>);
		*/
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

		/*
		build account from .txt file with acctList[i] as the acctNum
		insert new phone number
		save to .txt file
		get new info for acct from built account object
		insert new info into with command: 
		allTables.accountTable.updateInfo(<acctNum>, <newInfo>);
		*/
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
			cout << acctList[i] << endl;
			cout << allTables.accountTable.search(acctList[i]) << endl; //short snippet of acct
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

void DataHandler::displayAccountTypes()
{
	for (int i = 0; i < accountTypeList.size(); i++)
	{
		string line = "[" + to_string(i + 1) + "]" + " " + accountTypeList[i].getAccountTypeName();
		cout << line << endl;
	}
}
