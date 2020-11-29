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

void DataHandler::initialSetup()
{
    allTables.accountTable.buildTable("Tables/AccountTable.txt");
    allTables.firstNameTable.buildTree("Tables/FirstNameTable.txt");
    allTables.lastNameTable.buildTree("Tables/LastNameTable.txt");
    allTables.phoneNumTable.buildTree("Tables/PhoneTable.txt");
    allTables.addressTable.buildTree("Tables/AddressTable.txt");
    allTables.userTable.buildTree("Tables/UserTable.txt");

    allTables.accountTable.refreshInfo(); //need to finish function, but will do the interest computation

    //ensuring hard coded user account types of each; if already saved in .txt file, will have no effect
    string hashedPw = EncryptionBox::hash("password1");
    vector<string> adminInfo = {hashedPw, "admin"};
    vector<string> offInfo = {hashedPw, "official"};
    vector<string> memInfo = {hashedPw, "member"};
    allTables.userTable.insertWithList("admin", adminInfo);
    allTables.userTable.insertWithList("official", offInfo);
    allTables.userTable.insertWithList("house", memInfo);
}