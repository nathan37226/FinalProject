#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <ctime>
using namespace std;

class AccountType
{
private:
    string accountTypeName;
    double monthlyFee;
    double serviceFee;
    static double penaltyFee;
    double interestRate;
    double minimumBalance;


    

public:
    // Constructor for new account type
    AccountType(string acctTypeName, double monFee = 0.0, double servFee = 0.0, double interestR = 0.0, double minBalance = 0.0);
    // Constructor for existing account type
    //AccountType(string acctTypeName);
    void setMonthlyFee(double fee);
    double getMonthlyFee();
    void setServiceFee(double fee);
    double getServiceFee();
    static void setPenaltyFee(double fee);
    static double getPenaltyFee();
    bool setInterestRate(double rate);
    double getInterestRate();
    void setMinimumBalance(double amount);
    double getMinimumBalance();
    void setAccountTypeName(string name);
    string getAccountTypeName();
    static string getDisplayNum(double input);
    static double roundNum(double amount, int precision);

};

class Account : public AccountType
{
private:
    static const string routingNumber;
    string accountNumber;
    string accountHolderUserID;
    string accountHolderFirstName;
    string accountHolderLastName;
    string accountHolderPhoneNumber;
    string accountHolderAddress;
    string accountClosedBy;
    time_t openDate;
    time_t closeDate;
    time_t maturityDate;
    double accountBalance;
    bool restrictedStatus;
    bool openStatus;
    time_t lastInterestCalculation;

    static string nextCheckingAccountNumber;
    static string nextSavingsAccountNumber;
    static string nextCDAccountNumber;
    static string nextUniqueAccountNumber;

    void setAccountBalance(double amount);
    string convertTimeToString(time_t inputTime);
    string incrementAcctNum(string lastAcctNum);
    void saveTransaction(string type, double amount);
    void interestCalc();

    friend void accountInit();

public:
    // Constructor for new account
    Account(string acctTypeName, string userID, string acctFirstName, string acctLastName, string acctPhoneNumber = "", string acctAddress = "", time_t mDate = 0, double acctBalance = 0.0);
    // Constructor for existing account
    Account(string acctNum);
    //Account(string accountNumber);
    //~Account();
    void setAccountNumber(string acctNum);
    string getAccountNumber();
    void setAccountHolderUserID(string userID);
    string getAccountHolderUserID();
    void setAccountHolderFirstName(string name);
    string getAccountHolderFirstName();
    void setAccountHolderLastName(string name);
    string getAccountHolderLastName();
    string getAccountHolderFullName();
    void setAccountHolderPhoneNumber(string phoneNum);
    string getAccountHolderPhoneNumber();
    void setAccountHolderAddress(string address);
    string getAccountHolderAddress();
    string getAccountHolderInfo();
    string getAccountTableInfo();
    void setAccountClosedBy(string name);
    string getAccountClosedBy();
    string getOpenDate();
    void setCloseDate(time_t date); // same as just closing?
    string getCloseDate();
    time_t getMaturityDate();
    time_t getTermLength();
    double getAccountBalance();
    void setRestrictedStatus(bool status);
    void setOpenStatus(bool status);
    bool getRestrictedStatus();
    bool getOpenStatus();
    string deposit(double amount); // returns string of what happened
    string withdraw(double amount);
    void displayHistory(string startDate, string endDate);
    static time_t displayHistoryHelper(string date);
    string saveToFile();
    void buildFromFile(string acctNum);
    static void saveNextAccountNumbers();

};

#include "Account.cpp"
#endif