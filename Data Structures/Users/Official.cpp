#include "Official.h"

Official::Official()
{
	setRecentLogin(DateTools().getCurrentDate().ToString());
}

Official::~Official()
{
	
}

Official::Official(string name, string ID, string pw, string type, string login) : User(name, ID, pw, type, login)
{
	setRecentLogin(DateTools().getCurrentDate().ToString());
}

string Official::getState()
{
	return officialState;
}

void Official::setState(string newState)
{
	officialState = newState;
}