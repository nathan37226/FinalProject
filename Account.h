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

protected:
    double roundNum(double amount, int precision);

public:
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

};

class Account : public AccountType
{
private:
    static const string routingNumber;
    string accountNumber;
    string accountHolderFirstName;
    string accountHolderLastName;
    string accountHolderPhoneNumber;
    string accountHolderAddress;
    time_t openDate;
    time_t closeDate;
    time_t maturityDate;
    double accountBalance;
    bool restrictedStatus;
    bool openStatus;

    static string nextCheckingAccountNumber;
    static string nextSavingsAccountNumber;
    static string nextCDAccountNumber;
    static string nextUniqueAccountNumber;

    void setAccountBalance(double amount);
    string convertTimeToString(time_t inputTime);
    string incrementAcctNum(string lastAcctNum);
    string saveToFile();

public:
    Account();
    Account(string accountNumber);
    ~Account();
    string getAccountNumber();
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
    time_t getOpenDate();
    void setCloseDate(time_t date); // same as just closing?
    time_t getMaturityDate();
    time_t getTermLength();
    double getAccountBalance();
    void setRestrictedStatus(bool status);
    void setOpenStatus(bool status);
    bool getRestrictedStatus();
    bool getOpenStatus();
    string deposit(double amount); // returns string of what happened
    string withdraw(double amount);
    void displayHistory(string beginning, string ending);
    static time_t displayHistoryHelper(string date);
};
#endif