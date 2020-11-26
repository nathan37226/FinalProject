// Implementation file for Account class
#include "Account.h"

using namespace std;

/**********************************************************
/ AccountType
*//////////////////////////////////////////////////////////
/**********************************************************
/ Setters
*//////////////////////////////////////////////////////////
void AccountType::setMonthlyFee(double fee)
{
    monthlyFee = fee;
}

void AccountType::setServiceFee(double fee)
{
    serviceFee = fee;
}

bool AccountType::setInterestRate(double rate)
{
    if(rate > 0 && rate < 5.0){
        interestRate = rate;
        return true;
    }
    else
        return false;
}

/**********************************************************
/ Getters
*//////////////////////////////////////////////////////////
string Account::getAccountHolderInfo()
{
    return accountHolderFirstName+" "+accountHolderLastName+" "+accountHolderPhoneNumber+" "+accountHolderAddress;
}

string Account::getAccountNumber()
{
    return accountNumber;
}

double AccountType::getMonthlyFee()
{
    return monthlyFee;
}

double AccountType::getServiceFee()
{
    return serviceFee;
}

double AccountType::getInterestRate()
{
    return interestRate;
}

/**********************************************************
/ Account
*//////////////////////////////////////////////////////////
/**********************************************************
/ Setters
*//////////////////////////////////////////////////////////
void Accout::setAccountHolderFirstName(string name)
{
    // possibly add check for valid inputs
    accountHolderFirstName = name;
}

void Account::setAccountHolderLastName(string name)
{
    // possibly add check for valid inputs
    accountHolderLastName = name;
}

void Account::setAccountHolderPhoneNumber(string phoneNum)
{
    // possibly add check for valid inputs
    accountHolderPhoneNumber = phoneNum;
}

void Account::setAccountHolderAddress(string address)
{
    // possibly add check for valid inputs
    accountHolderAddress = address;
}

void Account::setCloseDate(time_t date)
{
    closeDate = date;
}

void Account::setAccountBalance(double amount)
{
    accountBalance = roundNum(amount, 2);
}

void Account::setRestrictedStatus(bool status)
{
    restrictedStatus = status;
}

void Account::setOpenStatus(bool status)
{
    openStatus = status;
}

/**********************************************************
/ Getters
*//////////////////////////////////////////////////////////
time_t Account::getOpenDate()
{
    return openDate;
}

time_t Account::getMaturityDate()
{
    return maturityDate;
}

time_t Account::getTermLength()
{
    return maturityDate - openDate;
}

double Account::getAccountBalance()
{
    return accountBalance;
}

bool Account::getRestrictedStatus()
{
    return restrictedStatus;
}

bool Account::getOpenStatus()
{
    return openStatus;
}

/**********************************************************
/ roundNum (protected) rounds a floating point number to
/ a specified number of decimals
/
/ parameters:
/   amount : double, precision : int
/
/ returns:
/   double
*//////////////////////////////////////////////////////////
double Account::roundNum(double amount, int precision)
{
    // implementation by nathan
}

/**********************************************************
/ convertTime (protected) converts the ctime integer time_t
/ to a string of the exact date in the format:
/   "<day of the week> <month> <day> <time> <year>"
/
/ parameters:
/   inputTime : time_t
/
/ returns:
/   string
*//////////////////////////////////////////////////////////
string Account::convertTime(time_t inputTime)
{
    return ctime(&inputTime);
}
