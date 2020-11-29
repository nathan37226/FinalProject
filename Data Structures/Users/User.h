#ifndef _USER_H_
#define _USER_H_

#include "Tools.h"
#include <fstream>

class User
{
public:
	User();
	User(string name, string ID, string pw, string type, string login);
	User(string name, string ID, string pw, string type);
	virtual ~User();

	string getName() const;
	string getID() const;
	string getPassword() const;
	string getRecentLogin() const;
	string getUserType() const;
	string getRecentActivity() const;

	void setName(string newName);
	void setID(string newID);
	void setPassword(string newPassword);
	void setRecentLogin(string RecentLogin);
	void setUserType(string type);
	void setRecentActivity(string description);

	bool isValidLogin(string newName, string newPassword, string loginTime);

	void saveUser() const;
	void buildUser(string filename);

private:
	string userName;//name of user
	string userID;//id of user
	string userPassword;
	string userType;
	string mostRecentLogin;
	string recentActivity;
};

#include "User.cpp"
#endif // !_USER_H_