#include "Admin.h"

Admin::Admin()
{
	;
}

Admin::Admin(string name, string ID, string pw, string type, string login) : User(name, ID, pw, type, login)
{
	;
}

Admin::Admin(string name, string ID, string pw, string type) : User(name, ID, pw, type)
{
	;
}

Admin::~Admin()
{
	;
}
 
//requires userType so that searching through table is not required again to find it
void Admin::resetPassword(string userID, string newPassword, string userType)
{
	vector<string> userLoginInfo = {EncryptionBox::hash(newPassword), userType};
	DataHandler::allTables.userTable.insertWithList(userID, userLoginInfo); //adds new list of login info to AVLTree
}

//get funct to return from accountTable the userID in the info part of the acct entry
string Admin::returnUserID(string acctNum)
{
	return "Must implement this later";
}