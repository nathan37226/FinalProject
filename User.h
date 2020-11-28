#ifndef _USER_H_
#define _USER_H_

#include "Tools.h"

class User
{
public:
	User();
	virtual ~User();

	string getName();
	string getID();
	string getPassword();
	string getRecentLogin();

	void setName(string newName);
	void setID(string newID);
	void setPassword(string newPassword);
	void setRecentLogin(string RecentLogin);

	bool isValidLogin(string newName, string newPassword, string loginTime);

private:
	string userName;//name of user
	string userID;//id of user
	string userPassword;
	string mostRecentLogin;
};

#endif // !_USER_H_