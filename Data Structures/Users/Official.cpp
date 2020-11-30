#include "Official.h"
#include "Client.h"

Official::Official()
{
	;
}

Official::~Official()
{
	;
}

Official::Official(string name, string ID, string pw, string type, string login) : User(name, ID, pw, type, login)
{
	;
}

Official::Official(string name, string ID, string pw, string type) : User(name, ID, pw, type)
{
	;
}

string Official::getState()
{
	return officialState;
}

void Official::setState(string newState)
{
	officialState = newState;
}

void Official::addNewClientUser(Client &user)
{
	string userID = user.getID(), hashedPw = user.getPassword();
	vector<string> userData = {hashedPw, "client"}; //no accts at first
	DataHandler::allTables.userTable.insertWithList(userID, userData); //inserts into AVLtree
}
