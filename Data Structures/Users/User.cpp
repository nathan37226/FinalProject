#include "User.h"

User::User()
{
}

User::User(string name, string ID, string pw, string type, string login)
{
	userName = name;
	userID = ID;
	userPassword = pw;
	userType = type;
	mostRecentLogin = login;
}

User::~User()
{
}

string User::getName()
{
	return userName;
}

string User::getID()
{
	return userID;
}

string User::getPassword()
{
	return userPassword;
}

string User::getRecentLogin()
{
	return mostRecentLogin;
}

string User::getUserType()
{
	return userType;
}

void User::setName(string newName)
{
	userName = newName;
}

void User::setID(string newID)
{
	userID = newID;
}

void User::setPassword(string newPassword)
{
	userPassword = newPassword;
}

void User::setRecentLogin(string RecentLogin)
{
	mostRecentLogin = RecentLogin;
}

void User::setUserType(string type)
{
	userType = type;
}

bool User::isValidLogin(string newName, string newPassword, string loginTime)
{
	if (userName == newName && userPassword == newPassword)
	{
		mostRecentLogin = loginTime;
		return true;
	}
	return false;
}

void User::saveUser() const
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
	}
	else
	{
		cout << "Could not save to file at: " << endl << "UserData/" + userID + ".txt" << endl;
	}

	outFile.close();
}

void User::buildUser(string filename)
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
	}
	else
	{
		cout << "Could not read from file at: " << endl << filename << endl;
	}
	
	inFile.close();
}