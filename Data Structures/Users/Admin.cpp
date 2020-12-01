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
void Admin::resetPassword(string ID, string newPassword) 
{
	vector<string> userInfo = DataHandler::allTables.userTable.returnMappedItems(ID); //formatted {hashedPw, user type, accts...}
	userInfo[0] = EncryptionBox::hash(newPassword);
	DataHandler::allTables.userTable.insertWithList(ID, userInfo); //replaces node in AVLtree to one with updated list
}

//get funct to return from accountTable the userID in the info part of the acct entry
string Admin::returnUserID(string acctNum)
{
	return "Must implement this later";
}

void Admin::createOfficial(string fullName, string ID, string password)
{
	password = EncryptionBox::hash(password);
	Official newOfficial(fullName, ID, password, "official"); //puts state as active initially
	newOfficial.setRecentActivity("Account created by: " + userID);
	newOfficial.saveUser();

	vector <string> userData = {password, "official"};
	DataHandler::allTables.userTable.insertWithList(ID, userData); //creates record of Official inside table; allows login to occur
}

void Admin::setOfficialActive(Official officialUser)
{
	officialUser.setState("active");
	officialUser.setRecentActivity("Set Active by: " + userID);
	officialUser.saveUser();
}

void Admin::setOfficialInactive(Official officialUser)
{
	officialUser.setState("inactive");
	officialUser.setRecentActivity("Locked out by: " + userID);
	officialUser.saveUser();
}

void Admin::clearFile(string filename)
{
	ofstream outFile(filename);
	if (outFile)
	{
		outFile << "";
	}
	else
	{
		cout << "File could not be cleared at: " << endl << filename << endl;
	}
	outFile.close();
}

void Admin::deleteOfficial(Official officialUser)
{
	//officials' records are only inside UserData and the userTable
	clearFile("UserData/" + officialUser.getID() + ".txt"); //clears all record of official's data
	DataHandler::allTables.userTable.del(officialUser.getID()); //deletes entire node
}

