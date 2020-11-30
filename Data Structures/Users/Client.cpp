#include "Client.h"

Client::Client()
{
	;
}

Client::Client(string name, string ID, string pw, string type, string login) : User(name, ID, pw, type, login)
{
	;
}
Client::Client(string name, string ID, string pw, string type) : User(name, ID, pw, type)
{
	;
}

Client::~Client()
{
}

//Needs Finishing!!
void Client::getAccountHistory(vector<string> acctList) const
{
	//acctList is formatted {hashedPw, user type, acct 1, acct 2, etc}
	string accountNum = "";
	bool isPresent = false;
	cout << endl << "Enter the Account Number: ";
	getline(cin, accountNum);
	
	for (int i = 2; i < acctList.size(); i++) //normal iteration--a BST is not needed here since so few possible options
	{
		if (accountNum == acctList[i])
		{
			isPresent = true;
			break;
		}
	}
	while (!isPresent)
	{
		cout << "Invalid Account Number." << endl << "Enter Again: ";
		getline(cin, accountNum);

		for (int i = 2; i < acctList.size(); i++) //normal iteration--a BST is not needed here since so few possible options
		{
			if (accountNum == acctList[i])
			{
				isPresent = true;
				break;
			}
		}
	}

	string startDate = "", endDate = "";
	//Display Account Info Here!!
	//Account userAcct(accountNum); //to build acct
	//get start and end date from user
	//userAcct.displayHistory(startDate, endDate);
}