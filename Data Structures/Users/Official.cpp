#include "Official.h"
#include "Client.h"

Official::Official()
{
	;
}

Official::~Official()
{
	;
}

Official::Official(string name, string ID, string pw, string type, string login) : User(name, ID, pw, type, login)
{
	;
}

Official::Official(string name, string ID, string pw, string type) : User(name, ID, pw, type)
{
	;
}

string Official::getState()
{
	return officialState;
}

void Official::setState(string newState)
{
	officialState = newState;
}

void Official::addNewClientUser(Client &user)
{
	string userID = user.getID(), hashedPw = user.getPassword();
	vector<string> userData = {hashedPw, "client"}; //no accts at first
	DataHandler::allTables.userTable.insertWithList(userID, userData); //inserts into AVLtree
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
