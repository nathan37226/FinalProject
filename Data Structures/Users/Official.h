#ifndef OFFICIAL_H
#define OFFICIAL_H

#include "User.h"

class Official : public User
{
public:
	string getState();
	void setState(string newState);

private:
	string officialState;
};

#include "Official.cpp"
#endif