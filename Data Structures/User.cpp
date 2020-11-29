#include "User.h"

User::User()
{
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

bool User::isValidLogin(string newName, string newPassword, string loginTime)
{
	if (userName == newName && userPassword == newPassword)
	{
		mostRecentLogin = loginTime;
		return true;
	}
	return false;
}
