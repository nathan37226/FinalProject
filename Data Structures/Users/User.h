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

	virtual void saveUser() const; //virtual because clients have additional attributes
	virtual void buildUser(string filename);

protected: //since need access for Client
	string userName;//name of user
	string userID;//id of user
	string userPassword; //used in Official adding user funct
	string userType;
	string mostRecentLogin;
	string recentActivity;
};

#include "User.cpp"
#endif // !_USER_H_