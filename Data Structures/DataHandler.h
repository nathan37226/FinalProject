#include "Admin.h"
#include "Official.h"
#include "Client.h"
#include "Account.h"

class DataHandler
{
public:
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
	vector<Admin> getAdminData();//do it later
	vector<Official> getOfficialData();
	vector<Client> getClientData();
	vector<Account> getAccountData();
	vector<AccountType> getTypeData();

private:
	DataHandler();
	~DataHandler();

private:
	vector<Admin> adminList; //do it later
	vector<Official> officialList;
	vector<Client> clientList;
	vector<Account> accountList;
	vector<AccountType> accountTypeList;
	
};