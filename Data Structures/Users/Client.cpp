#include "Client.h"
#include <fstream>

Client::Client()
{
	;
}

Client::Client(string name, string ID, string pw, string type, string login, string address, string num) : User(name, ID, pw, type, login)
{
	clientAddress = address;
	clientPhoneNum = num;
}
Client::Client(string name, string ID, string pw, string type, string address, string num) : User(name, ID, pw, type)
{
	clientAddress = address;
	clientPhoneNum = num;
}

void Client::setAddress(string address)
{
	clientAddress = address;
}

void Client::setPhoneNum(string num)
{
	clientPhoneNum = num;
}

string Client::getAddress() const
{
	return clientAddress;
}

string Client::getPhoneNum() const
{
	return clientPhoneNum;
}

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
	Account userAcct(accountNum); //to build acct
	//get start and end date from user
	cout << endl << "Enter oldest date you want the transaction history from: MM/DD/YYYY" << endl << "Date: ";
	getline(cin, startDate);
	cout << "Enter most recent date: MM/DD/YYYY" << endl << "Date: ";
	getline(cin, endDate);
	cout << endl;
	try
	{
		userAcct.displayHistory(startDate + " 00:00:00", endDate + " 00:00:00");
	}
	catch(...)
	{
		cout << "Invalid Date Entry" << endl;
	}
}

void Client::saveUser() const
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
	outFile << EncryptionBox::encrypt(clientAddress) << endl;
	outFile << EncryptionBox::encrypt(clientPhoneNum) << endl;
	}
	else
	{
		cout << "Could not save to file at: " << endl << "UserData/" + userID + ".txt" << endl;
	}

	outFile.close();
}

void Client::buildUser(string filename)
{
	EncryptionBox::positionInFile = 0;
	ifstream inFile;
	inFile.open(filename);
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

		userID = userInfo[0]; //setting user attributes
		userName = userInfo[1];
		userPassword = userInfo[2];
		userType = userInfo[3];
		mostRecentLogin = userInfo[4];
		recentActivity = userInfo[5];
		clientAddress = userInfo[6];
		clientPhoneNum = userInfo[7];
	}
	else
	{
		cout << "Could not read from file at: " << endl << filename << endl;
	}
	
	inFile.close();
}

void Client::displayInfo() const
{
	string name = userName, firstName = "", lastName = "";
	firstName = name.substr(0, name.find(" "));
	lastName = name.substr(name.find(" ") + 1, string::npos);
	cout << "First Name: " << firstName << endl;
	cout << "Last Name: " << lastName << endl;
	cout << "Phone Number: " << clientPhoneNum << endl;
	cout << "Address: " << clientAddress << endl;
	cout << endl;
}