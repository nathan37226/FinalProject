#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Official.h"

class Admin : public User
{
public:
	Admin();
	Admin(string name, string ID, string pw, string type, string login);
	Admin(string name, string ID, string pw, string type);
	~Admin();

	void resetPassword(string ID, string newPassword);
	string returnUserID(string acctNum);
	void createOfficial(string fullName, string ID, string password);
	void setOfficialActive(Official officialUser);
	void setOfficialInactive(Official officialUser);
	void deleteOfficial(Official officialUser);
	

protected:
	void clearFile(string filename);

private:
	
};

#include "Admin.cpp"
#endif