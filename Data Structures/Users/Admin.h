#ifndef ADMIN_H
#define ADMIN_H

class Admin : public User
{
public:
	Admin();
	Admin(string name, string ID, string pw, string type, string login);
	Admin(string name, string ID, string pw, string type);

	void resetPassword(string ID, string newPassword);
	string returnUserID(string acctNum);
	void createOfficial(string fullName, string ID, string password);
	void setOfficialActive(Official &officialUser);
	void setOfficialInactive(Official &officialUser);
	void deleteOfficial(Official &officialUser);
	void createAdmin(string fullName, string ID, string password);
	void deleteAdmin(string ID);
	void displayAllOfficials();
	void displayAllAdmins();
	void displayAllClients();
	

protected:
	void clearFile(string filename);

private:
	
};

#include "Admin.cpp"
#endif