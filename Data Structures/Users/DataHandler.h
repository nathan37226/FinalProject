#ifndef DATAHANDLER_H
#define DATAHANDLER_H

struct tableSet
{
	AVLTree<string> firstNameTable, lastNameTable, phoneNumTable, addressTable, userTable;
	AccountTable accountTable;
};

class DataHandler
{
public:
	DataHandler();

	//All Data Storage
	static tableSet allTables;
	vector<AccountType> accountTypeList;

	//login function
	static string isValidLogin(string userID, string password); //success, returns user type, failure, returns "false"
	static bool isValidUserID(string userID);
	static bool isAvaliableUserID(string userID);

	//client operations
	static string getAccountInfo(string acctNum);
	static void changeClientFirstName(string userID, string oldName, string newName); //updates the necessary tables to reflect name change
	static void changeClientLastName(string userID, string oldName, string newName);
	static void changeClientAddress(string userID, string oldAddress, string newAddress);
	static void changeClientPhoneNum(string userID, string oldNum, string newNum);
	static void clientRequestNewAccount(string userID, string acctType);
	static bool clientDisplayAccounts(string userID);
	static vector<string> clientGetAccountList(string userID);

	//admin operations
	static void addOfficialToRecords(string hashedPw, string ID);
	static void createAccountType(double monthlyFee, double serviceFee, double penaltyFee, double interestRate, double minimumBalance);
	static void deleteAccountType(string typeName);
	static void alterAccountType(string typeName, double monthlyFee, double serviceFee, double penaltyFee, double interestRate, double minimumBalance);
	static void addAdminToRecords(string hashedPw, string ID);
	static vector<string> getLoginInfo(string userID);

	//official operations
	static void addClientAccountToRecords(Client &user, Account &acct);

	//AccountType operations

};

tableSet DataHandler::allTables = tableSet(); //static initialization

#include "DataHandler.cpp"
#endif