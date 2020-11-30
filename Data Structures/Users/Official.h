#ifndef OFFICIAL_H
#define OFFICIAL_H

#include "User.h"
#include "Client.h"

class Official : public User
{
public:
	Official();
	Official(string name, string ID, string pw, string type, string login);
	Official(string name, string ID, string pw, string type);
	~Official();

	void addNewClientUser(Client &user);

	string getState();
	void setState(string newState);

private:
	string officialState;
};

#include "Official.cpp"
#endif