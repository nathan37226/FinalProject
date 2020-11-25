#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <ctime>
using namespace std;

class Account
{
private:
    string accountHolderFirstName;
    string accountHolderLastName;
    string accountHolderPhoneNumber;
    string accountHolderAddress;
    string accountNumber;
    double monthlyFee;
    double serviceFee;
    static double penaltyFee;
    double interestRate;
    time_t openDate;
    time_t closeDate;
    time_t maturityDate;
    double accountBalance;
    double minimumBalance;
    bool restrictedStatus;
    bool openStatus;

    void setAccountBalance(double amount);

protected:
    double roundNum(double amount, int precision);
    string convertTimeToString();
    time_t displayHistoryHelper(string date);

public:
    void setAccountHolderFirstName(string name);
    void setAccountHolderLastName(string name);
    void setAccountHolderPhoneNumber(string phoneNum);
    void setAccountHolderAddress(string address);
    string getAccountHolderInfo();
    string getAccountNumber();
    void setMonthlyFee(double fee);
    double getMothlyFee();
    void setServiceFee(double fee);
    double getServiceFee();
    bool setInterestRate(double rate);
    double getInterestRate();
    time_t getOpenDate();
    time_t setCloseDate(); // same as just closing?
    time_t getMaturityDate();
    time_t getTermLength();
    double getAccountBalance();
    string deposit(double amount); // returns string of what happened
    string withdraw(double amount);
    void displayHistory(string beginning, string ending);

};
#endif