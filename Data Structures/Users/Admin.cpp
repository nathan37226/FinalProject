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
void Admin::resetPassword(string userID, string newPassword, vector<string> userInfo) //userInfo is formatted {hashedPw, user type, accts...}
{
	userInfo[0] = EncryptionBox::hash(newPassword);
	DataHandler::allTables.userTable.insertWithList(userID, userInfo); //replaces node in AVLtree to one with updated list
}

//get funct to return from accountTable the userID in the info part of the acct entry
string Admin::returnUserID(string acctNum)
{
	return "Must implement this later";
}