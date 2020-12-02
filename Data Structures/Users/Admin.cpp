Admin::Admin()
{
	;
}

Admin::Admin(string name, string ID, string pw, string type, string login) : User(name, ID, pw, type, login)
{
	;
}

Admin::Admin(string name, string ID, string pw, string type) : User(name, ID, pw, type)
{
	;
}

//requires userType so that searching through table is not required again to find it
void Admin::resetPassword(string ID, string newPassword) 
{
	vector<string> userInfo = DataHandler::allTables.userTable.returnMappedItems(ID); //formatted {hashedPw, user type, accts...}
	userInfo[0] = EncryptionBox::hash(newPassword);
	DataHandler::allTables.userTable.insertWithList(ID, userInfo); //replaces node in AVLtree to one with updated list
}

//get funct to return from accountTable the userID in the info part of the acct entry
string Admin::returnUserID(string acctNum)
{
	return "Must implement this later";
}

void Admin::createOfficial(string fullName, string ID, string password)
{
	password = EncryptionBox::hash(password);
	Official newOfficial(fullName, ID, password, "official"); //puts state as active initially
	newOfficial.setRecentActivity("Account created by: " + userID);
	newOfficial.saveUser();
	DataHandler::addOfficialToRecords(password, ID); //adds official to table
}

void Admin::setOfficialActive(Official &officialUser)
{
	officialUser.setState("active");
	officialUser.setRecentActivity("Set Active by: " + userID);
	officialUser.saveUser();
}

void Admin::setOfficialInactive(Official &officialUser)
{
	officialUser.setState("inactive");
	officialUser.setRecentActivity("Locked out by: " + userID);
	officialUser.saveUser();
}

void Admin::clearFile(string filename)
{
	ofstream outFile(filename);
	if (outFile)
	{
		outFile << "";
	}
	else
	{
		cout << "File could not be cleared at: " << endl << filename << endl;
	}
	outFile.close();
}

void Admin::deleteOfficial(Official &officialUser)
{
	//officials' records are only inside UserData and the userTable
	clearFile("UserData/" + officialUser.getID() + ".txt"); //clears all record of official's data
	DataHandler::allTables.userTable.del(officialUser.getID()); //deletes entire node
}

void Admin::createAdmin(string fullName, string ID, string password)
{
	password = EncryptionBox::hash(password);
	Admin newAdmin(fullName, ID, password, "admin"); //puts state as active initially
	newAdmin.setRecentActivity("Account created by: " + userID);
	newAdmin.saveUser();
	DataHandler::addAdminToRecords(password, ID); //adds official to table
}

void Admin::deleteAdmin(string ID)
{
	clearFile("UserData/" + ID + ".txt"); //clears file with record
	DataHandler::allTables.userTable.del(ID); //deletes entire node, so no more log in ability
}

void Admin::displayAllOfficials()
{
	DataHandler::allTables.userTable.display(3);
}

void Admin::displayAllAdmins()
{
	DataHandler::allTables.userTable.display(2);
}