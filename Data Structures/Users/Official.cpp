#include "Official.h"

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