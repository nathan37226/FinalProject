#ifndef CLIENT_H
#define CLIENT_H

#include "User.h"
//#include "Account.h"

class Client : public User
{
public:
	Client();
	~Client();
private:
};

#include "Client.cpp"
#endif