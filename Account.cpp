// Implementation file for Account class
#include "Account.h"
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;
/*
int main()
{
    cout << "Enter start date in the format: 'MM/DD/YYYY'" << endl;
    string date;
    cin >> date;
    cout << Account::displayHistoryHelper(date + " 00:00:00") << endl;
}
*/
/**********************************************************
/ AccountType
*//////////////////////////////////////////////////////////
double AccountType::penaltyFee;
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

void AccountType::setPenaltyFee(double fee)
{
    penaltyFee = fee;
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

void AccountType::setMinimumBalance(double amount)
{
    minimumBalance = roundNum(amount, 2);
}

/**********************************************************
/ Getters
*//////////////////////////////////////////////////////////

double AccountType::getMonthlyFee()
{
    return monthlyFee;
}

double AccountType::getServiceFee()
{
    return serviceFee;
}

double AccountType::getPenaltyFee()
{
    return penaltyFee;
}

double AccountType::getInterestRate()
{
    return interestRate;
}

double AccountType::getMinimumBalance()
{
    return minimumBalance;
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
double AccountType::roundNum(double value, int precision)
{
    double multiplier = pow(10, precision);
    value = value * multiplier; //makes so the integer part will have all the numbers wanted

    int tempValue = value; //cuts off extra decimal part, like a floor function
    int higherTempValue = tempValue + 1; //in case it needed to be rounded to the next higher number

    double firstDist = abs(value - tempValue);  //finding distance from value to lower rounding
    double secondDist = abs(value - higherTempValue); //distance from value to higher rounding

    tempValue = (firstDist < secondDist) ? tempValue : higherTempValue; //returns value w smaller distance, i.e. the closest one
    value = tempValue / multiplier; //dividing to get real decimal

    return value;
}

/**********************************************************
/ Account
*//////////////////////////////////////////////////////////
const string Account::routingNumber = "133769420";
/**********************************************************
/ Setters
*//////////////////////////////////////////////////////////
void Account::setAccountHolderFirstName(string name)
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
string Account::getAccountNumber()
{
    return accountNumber;
}

string Account::getAccountHolderFirstName()
{
    return accountHolderFirstName;
}

string Account::getAccountHolderLastName()
{
    return accountHolderLastName;
}

string Account::getAccountHolderFullName()
{
    return accountHolderFirstName + " " + accountHolderLastName;
}

string Account::getAccountHolderPhoneNumber()
{
    return accountHolderPhoneNumber;
}

string Account::getAccountHolderAddress()
{
    return accountHolderAddress;
}

string Account::getAccountHolderInfo()
{
    return accountHolderFirstName+" "+accountHolderLastName+" "+accountHolderPhoneNumber+" "+accountHolderAddress;
}

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
/ convertTime (private) converts the ctime integer time_t
/ to a string of the exact date in the format:
/   "<day of the week> <month> <day> <time> <year>"
/
/ parameters:
/   inputTime : time_t
/
/ returns:
/   string
*//////////////////////////////////////////////////////////
string Account::convertTimeToString(time_t inputTime)
{
    return ctime(&inputTime);
}

/**********************************************************
/ incrementAcctNum (private) increments the last used
/ account number so the next new account can be assigned
/ the correct number
/
/ parameters:
/   lastAcctNum : string
/
/ returns:
/   string
*//////////////////////////////////////////////////////////
string Account::incrementAcctNum(string lastAcctNum)
{
    string accountPrefix = lastAcctNum.substr(0, 1); //if lastAcctNum is C123456789, this pulls out the C
    int acctNum = stoi( lastAcctNum.substr(1, string::npos) );
    acctNum++;

    string newAcctNum = to_string(acctNum);
    while (newAcctNum.length() < 9) //all account numbers will have 9 digits in them
    {
        newAcctNum = "0" + newAcctNum;
    }
    newAcctNum = accountPrefix + newAcctNum; //adding prefix back

    return newAcctNum;
}

/**********************************************************
/ deposit (public) adds the amount specified rounded to 
/ two digits after the decimal point
/
/ parameters:
/   amount : double
/
/ returns:
/   string
*//////////////////////////////////////////////////////////
string Account::deposit(double amount)
{
    accountBalance += roundNum(amount, 2);
    return "Amount added.";
}

/**********************************************************
/ withdraw (public) deducts the amount specified rounded to 
/ two digits after the decimal point
/
/ parameters:
/   amount : double
/
/ returns:
/   string
*//////////////////////////////////////////////////////////
string Account::withdraw(double amount)
{
    if(!getRestrictedStatus())
    {
        double tempBalance = accountBalance - roundNum(amount, 2);
        if(tempBalance < getMinimumBalance())
        {
            return "Insufficient Funds.";
        }
        if(tempBalance < 0 && tempBalance > getMinimumBalance())
        {
            accountBalance = tempBalance - getPenaltyFee();
            return "Overdraft Penalty.";
        }
    }
    else
        return "Account Restricted.";
    return "Something else happened.";
}

/**********************************************************
/ displayHistory (public) displays the transaction history
/ of an account between the specified dates
/
/ parameters:
/   startDate : string, endDate : string
/
/ returns:
/   void
*//////////////////////////////////////////////////////////
void Account::displayHistory(string startDate, string endDate)
{
    // Convert date strings in to time_t to use in AVL search tree
    time_t beginning = displayHistoryHelper(startDate);
    time_t ending = displayHistoryHelper(endDate);

    // Display AVL tree in order if (beginning < node < ending)
    //TODO
}

/**********************************************************
/ displayHistoryHelper (private) converts date string
/ formated as: "MM/DD/YYYY HH:MM:SS" to <ctime> time_t
/
/ parameters:
/   date : string
/
/ returns:
/   time_t
*//////////////////////////////////////////////////////////
time_t Account::displayHistoryHelper(string date)
{
    // parse out time info from string
    int month = stoi(date.substr(0,2));
    int day = stoi(date.substr(3,5));
    int year = stoi(date.substr(6,10));
    int hour = stoi(date.substr(11,13));
    int minute = stoi(date.substr(14,16));
    int second = stoi(date.substr(17,19));

    // build struct tm
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = day;
    timeinfo->tm_hour = hour;
    timeinfo->tm_min = minute;
    timeinfo->tm_sec = second;
    mktime(timeinfo); // automatically set the rest of timeinfo

    timeinfo->tm_hour = hour; // fix hour if daylight saving time
    // call mktime to convert to time_t
    return mktime(timeinfo);
}

/**********************************************************
/ saveToFile (private) save the account to file. Returns
/ a string saying what happened.
/
/ parameters:
/   none
/
/ returns:
/   string
*//////////////////////////////////////////////////////////
string Account::saveToFile()
{
    ofstream outFile;

    if(outFile.open("AccountData/"+accountNumber+".txt", ofstream::trunc)) // attempt to open file with intent to overwirite existing data
    {
        outFile << accountNumber << endl;
        outFile << routingNumber << endl;
    }

}