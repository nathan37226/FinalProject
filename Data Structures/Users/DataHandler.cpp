#include "DataHandler.h"

DataHandler* DataHandler::GetInstance()
{
	static DataHandler instance;
	return &instance;
}

/*
void DataHandler::createOfficialAccount(string userName, string userID, string userPassword, string lastLogin)
{
	Official newOfficial;
	newOfficial.setName(userName);
	newOfficial.setID(userID);
	newOfficial.setPassword(userPassword);
	newOfficial.setRecentLogin(lastLogin);
	officialList.push_back(newOfficial);
}

bool DataHandler::changeOfficialStatus(string userID, string newState)
{
	for (int i = 0; i < officialList.size(); i++)
	{
		if (officialList[i].getID() == userID)
		{
			officialList[i].setState(newState);
			return true;
		}
	}
	return false;
}

bool DataHandler::deleteOfficialAccount(string userID)
{
	for (int i = 0; i < officialList.size(); i++)
	{
		if (officialList[i].getID() == userID)
		{
			officialList.erase(officialList.begin() + i);
			return true;
		}
	}
	return false;
}
*/

DataHandler::DataHandler()
{
}

DataHandler::~DataHandler()
{
}

vector<AccountType> DataHandler::getTypeData()
{
	return accountTypeList;
}

/*
vector<Account> DataHandler::searchForNumber(string accountNumber)
{
	vector<Account> searchResult;
	for (int i = 0; i < accountList.size(); i++)
	{
		if (accountList[i].getAccountNumber() == accountNumber)
		{
			searchResult.push_back(accountList[i]);
		}
	}
	return searchResult;
}

vector<Account> DataHandler::searchForName(string clientName) //search for full name
{
	vector<Account> searchResult;
	for (int i = 0; i < accountList.size(); i++)
	{
		if (accountList[i].getAccountHolderFirstName() == clientName)
		{
			searchResult.push_back(accountList[i]);
		}
	}
	return searchResult;
}

vector<Account> DataHandler::searchForPhone(string phoneNumber)
{
	vector<Account> searchResult;
	for (int i = 0; i < accountList.size(); i++)
	{
		if (accountList[i].getAccountHolderPhoneNumber() == phoneNumber)
		{
			searchResult.push_back(accountList[i]);
		}
	}
	return searchResult;
}

void DataHandler::queryAccountHistory(string clientID, string beginning, string ending)
{
	for (int i = 0; i < accountList.size(); i++)
	{
		if (accountList[i].getAccountHolderFullName() == clientID)
		{
			accountList[i].displayHistory(beginning, ending);
		}
	}
}
*/



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

/************************************************
Functs called by Client
************************************************/

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

	//accountEntry newEntry(acct.getAccountNumber(), acct.getAccountInfo()); //implemenent getAcctInfo later on!
	//allTables.accountTable.insert(newEntry);
}
