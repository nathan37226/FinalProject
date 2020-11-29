#include "Client.h"

Client::Client()
{
}

Client::Client(string name, string ID, string pw, string type, string login) : User(name, ID, pw, type, login)
{
	setRecentLogin(DateTools().getCurrentDate().ToString());
}

Client::~Client()
{
}
