#include "DataHandler.h"

DataHandler* DataHandler::GetInstance()
{
	static DataHandler instance;
	return &instance;
}

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

DataHandler::DataHandler()
{
}

DataHandler::~DataHandler()
{
}

vector<Official> DataHandler::getOfficialData()
{
	return officialList;
}

vector<Client> DataHandler::getClientData()
{
	return clientList;
}

vector<Account> DataHandler::getAccountData()
{
	return accountList;
}

vector<AccountType> DataHandler::getTypeData()
{
	return accountTypeList;
}

bool DataHandler::alterUserPassword(string userID, string userPassword)
{
	for (int i = 0; i < clientList.size(); i++)
	{
		if (clientList[i].getID() == userID)
		{
			clientList[i].setPassword(userPassword);
			return true;
		}
	}
	return false;
}

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

bool DataHandler::changeClientPassword(string clientID, string newPassword)
{
	for (int i = 0; i < clientList.size(); i++)
	{
		if (clientList[i].getID() == clientID)
		{
			clientList[i].setPassword(newPassword);
			return true;
		}
	}
	return false;
}

void DataHandler::queryAccountHistory(string clientID, string beginning, string ending)
{
	for (int i = 0; i < accountList.size(); i++)
	{
		if (accountList[i].getAccountHolderID() == clientID)
		{
			accountList[i].displayHistory(beginning, ending);
		}
	}
}