#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User
{
public:
	Admin();
	Admin(string name, string ID, string pw, string type, string login);
	~Admin();

	void resetPassword(string userID, string newPassword, string userType);
	string returnUserID(string acctNum);

private:
	
};

#include "Admin.cpp"
#endif