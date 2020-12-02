#ifndef OFFICIAL_H
#define OFFICIAL_H

class Official : public User
{
public:
	Official();
	Official(string name, string ID, string pw, string type, string login);
	Official(string name, string ID, string pw, string type);

	void saveUser() const;
	void buildUser(string filename);

	void addNewClientUser(Client &user);
	void searchByUserID(string ID);
	void searchByFirstName(string firstName);
	void searchByLastName(string lastName);
	void searchByAccountNum(string acctNum);
	void searchByPhoneNum(string phoneNum);
	void searchByAddress(string address);

	

	string getState();
	void setState(string newState);

private:
	string officialState;
};

#include "Official.cpp"
#endif