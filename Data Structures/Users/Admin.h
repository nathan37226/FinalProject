#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User
{
public:
	Admin();
	~Admin();

	void resetPassword(string userID, string newPassword, string userType);

private:

};

#include "Admin.cpp"
#endif