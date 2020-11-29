#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "Admin.h"
#include "Official.h"
#include "Client.h"
#include "Account/Account.h"

class DataHandler
{
public:
	static tableSet allTables;

	static DataHandler* GetInstance();

	//admin operation
	void createOfficialAccount(string userName, string userID, string userPassword, string lastLogin);
	bool changeOfficialStatus(string userID, string newState);//active or inactive
	bool deleteOfficialAccount(string userID);

	void createAccountType(double monthlyFee, double serviceFee, double penaltyFee, double interestRate, double minimumBalance);
	void deleteAccountType(string typeName);
	void alterAccountType(string typeName, double monthlyFee, double serviceFee, double penaltyFee, double interestRate, double minimumBalance);

	bool alterUserPassword(string userID, string userPassword);

	//official operation
	void openMemberAccount(string accountType);
	bool closeMemberAccount(string accountNumber);
	bool depositIntoAccount(string accountNumber, double amount);
	bool withdrawlFromAccount(string accountNumber, double amount);
	vector<Account> searchForNumber(string accountNumber);
	vector<Account> searchForName(string clientName);
	vector<Account> searchForPhone(string phoneNumber);

	void saveAccount();

	//client operation
	bool changeClientPassword(string clientID, string newPassword);
	void queryAccountHistory(string clientID, string beginning, string ending);

	//login function
	bool adminLogin(string number, string password);

	//data interface
	static void initialSetup();
	vector<AccountType> getTypeData();

private:
	DataHandler();
	~DataHandler();

private:
	vector<AccountType> accountTypeList;
	
};

tableSet DataHandler::allTables = tableSet(); //static initialization

#include "DataHandler.cpp"
#endif