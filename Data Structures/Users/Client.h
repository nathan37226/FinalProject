#ifndef CLIENT_H
#define CLIENT_H

#include "User.h"
//#include "Account.h"

class Client : public User
{
public:
	Client();
	Client(string name, string ID, string pw, string type, string login);
	~Client();
private:
};

#include "Client.cpp"
#endif