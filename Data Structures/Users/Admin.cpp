#include "Admin.h"

Admin::Admin() //now just one admin
{
	setName("admin");
	setID("0000");
	setPassword("admin");
	setRecentLogin(DateTools().getCurrentDate().ToString());
}

Admin::~Admin()
{
}

void Admin::resetPassword(string userID, string newPassword, string userType)
{
	vector<string> userLoginInfo = {EncryptionBox::hash(newPassword), userType};
	DataHandler::allTables.userTable.insertWithList(userID, userLoginInfo); //adds new list of login info to AVLTree
}