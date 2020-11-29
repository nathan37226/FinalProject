#include "Admin.h"

Admin::Admin()
{
	setRecentLogin(DateTools().getCurrentDate().ToString());
}

Admin::Admin(string name, string ID, string pw, string type, string login) : User(name, ID, pw, type, login)
{
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