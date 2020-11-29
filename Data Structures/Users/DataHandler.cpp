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
