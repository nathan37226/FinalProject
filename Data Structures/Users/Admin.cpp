#include "Admin.h"

Admin::Admin() //now just one admin
{
	setName("admin");
	setID("0000");
	setPassword("admin");
	setRecentLogin(DateTools().getCurrentDate().ToString());
}

Admin::~Admin()
{
}