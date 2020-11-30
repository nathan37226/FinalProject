#ifndef CLIENT_H
#define CLIENT_H

#include "User.h"
//#include "Account.h"

class Client : public User
{
public:
	Client();
	Client(string name, string ID, string pw, string type, string login, string address, string num);
	Client(string name, string ID, string pw, string type, string address, string num);
	~Client();

	string getAddress() const;
	string getPhoneNum() const;
	void setAddress(string address);
	void setPhoneNum(string num);

	void saveUser() const;
	void buildUser(string filename);
	void getAccountHistory(vector<string> acctList) const;
	void displayInfo() const;
private:
	string clientAddress;
	string clientPhoneNum;
	
};

#include "Client.cpp"
#endif