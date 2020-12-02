// Implementation file for Account class
#include "Account.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "EncryptionBox.h"

using namespace std;

void accountInit();
void accountExit();

int main()
{
    accountInit();
    cout << endl;
    //Account a("checking","dom14","Dominic","Zucchini","417-551-2422","1234 N Street Place Springfield, MO, 65805",0,0.0);
    Account a("C000000001");
    a.deposit(200.001);
    a.withdraw(100.5);
    cout << a.getAccountHolderUserID() << endl;
    cout << Account::getDisplayNum(a.getAccountBalance()) << endl;
    a.displayHistory("12/01/2020 00:00:00", "12/02/2020 00:00:00");
    cout << a.saveToFile() << endl;
    accountExit();
}

void accountInit()
{
    cout << "Creating default account types..." << endl;
    AccountType checkingAccount("checking", 0.0, 0.0, 0.0, -50.0);
    AccountType savingsAccount("savings", 0.0, 0.0, 2.0, 0.0);
    AccountType CD("CD", 0.0, 0.0, 5.0, 0.0);
    cout << "Loading Data..." << endl;
    ifstream inFile;
    inFile.open("AccountData/nextAccountNumbers.txt");
    if(inFile)
    {
        string line;
        getline(inFile,line);
        Account::nextCheckingAccountNumber = line.substr(0,line.find("\r"));
        getline(inFile,line);
        Account::nextSavingsAccountNumber = line.substr(0,line.find("\r"));
        getline(inFile,line);
        Account::nextCDAccountNumber = line.substr(0,line.find("\r"));
        getline(inFile,line);
        Account::nextUniqueAccountNumber = line.substr(0,line.find("\r"));
    }
    else
    {
        Account::nextCheckingAccountNumber = "C000000000";
        Account::nextSavingsAccountNumber = "S000000000";
        Account::nextCDAccountNumber = "D000000000";
        Account::nextUniqueAccountNumber = "U000000000";
    }
    inFile.close();
}

void accountExit()
{
    Account::saveNextAccountNumbers();
}

/**********************************************************
/ AccountType
*//////////////////////////////////////////////////////////
double AccountType::penaltyFee;
/**********************************************************
/ Constructor
*//////////////////////////////////////////////////////////
AccountType::AccountType(string acctTypeName, double monFee, double servFee, double interestR, double minBalance)
{
    if(acctTypeName == "blank")
    {
        monthlyFee = 0.0;
        serviceFee = 0.0;
        interestRate = 0.0;
        minimumBalance = 0.0;
        accountTypeName = "";
    }
    else
    {
        ifstream inFile;
        //attempt to open account type
        inFile.open("AccountData/"+acctTypeName+".txt");
        if(inFile)
        {
            string line;
            getline(inFile,line);
            monthlyFee = stod(line.substr(0,line.find("\r")));
            getline(inFile,line);
            serviceFee = stod(line.substr(0,line.find("\r")));
            getline(inFile,line);
            interestRate = stod(line.substr(0,line.find("\r")));
            getline(inFile,line);
            minimumBalance = stod(line.substr(0,line.find("\r")));
            accountTypeName = acctTypeName;
            inFile.close();
        }
        else // create new account type
        {
            inFile.close();
            monthlyFee = monFee;
            serviceFee = servFee;
            interestRate = interestR;
            minimumBalance = minBalance;
            accountTypeName = acctTypeName;
            // save account type to file
            ofstream outFile("AccountData/"+acctTypeName+".txt", ofstream::trunc);
            outFile << getDisplayNum(monFee) << endl;
            outFile << getDisplayNum(servFee) << endl;
            outFile << getDisplayNum(interestR) << endl;
            outFile << getDisplayNum(minBalance) << endl;
            outFile.close();
        }
    }
}
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

void AccountType::setAccountTypeName(string name)
{
    accountTypeName = name;
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

string AccountType::getAccountTypeName()
{
    return accountTypeName;
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
/ getDisplayNum (public) convert double to string for
/ displaying to console and writing to file
/
/ parameters:
/   input : double
/
/ returns:
/   string
*//////////////////////////////////////////////////////////
string AccountType::getDisplayNum(double input)
{
    string num = to_string(input);
    return num.substr(0, num.length() - 4); //cuts off the last 0000 of the double
}

/**********************************************************
/ Account
*//////////////////////////////////////////////////////////
const string Account::routingNumber = "133769420";
string Account::nextCheckingAccountNumber;
string Account::nextSavingsAccountNumber;
string Account::nextCDAccountNumber;
string Account::nextUniqueAccountNumber;
/**********************************************************
/ Constructor for new account (account type must already exist)
*//////////////////////////////////////////////////////////
Account::Account(string acctTypeName, string userID, string acctFirstName, string acctLastName, string acctPhoneNumber, string acctAddress, time_t mDate, double acctBalance)
: AccountType(acctTypeName)
{
    // Set Account Number
    if(acctTypeName == "checking")
    {
        accountNumber = nextCheckingAccountNumber;
        nextCheckingAccountNumber = incrementAcctNum(nextCheckingAccountNumber);
    }
    else if(acctTypeName == "savings")
    {
        accountNumber = nextSavingsAccountNumber;
        nextSavingsAccountNumber = incrementAcctNum(nextSavingsAccountNumber);
    }
    else if(acctTypeName == "CD")
    {
        accountNumber = nextCDAccountNumber;
        nextCDAccountNumber = incrementAcctNum(nextCDAccountNumber);
    }
    else
    {
        accountNumber = nextUniqueAccountNumber;
        nextUniqueAccountNumber = incrementAcctNum(nextUniqueAccountNumber);
    }

    // Set account holder info
    accountHolderUserID = userID;
    accountHolderFirstName = acctFirstName;
    accountHolderLastName = acctLastName;
    accountHolderPhoneNumber = acctPhoneNumber;
    accountHolderAddress = acctAddress;

    // set dates
    time(&openDate); // set open date as current time
    closeDate = 0;
    maturityDate = mDate;

    // set status
    restrictedStatus = false;
    openStatus = true;

    // set accountBalance
    accountBalance = acctBalance;

    // set last time interest was calculated
    time(&lastInterestCalculation);

}
/**********************************************************
/ Constructor for existing account
*//////////////////////////////////////////////////////////
Account::Account(string acctNum) : AccountType("blank")
{
    buildFromFile(acctNum);
}
/**********************************************************
/ Setters
*//////////////////////////////////////////////////////////
void Account::setAccountNumber(string acctNum)
{
    accountNumber = acctNum;
}

void Account::setAccountHolderUserID(string userID)
{
    accountHolderUserID = userID;
}

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

string Account::getAccountHolderUserID()
{
    return accountHolderUserID;
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
    string date = ctime(&inputTime);
    return date.substr(0,date.find("\n"));
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
    saveTransaction("Deposit",amount);
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
        else if(tempBalance < 0 && tempBalance > getMinimumBalance())
        {
            accountBalance = tempBalance - getPenaltyFee();
            saveTransaction("Withdrawl",amount);
            saveTransaction("Overdraft",-1 * getPenaltyFee());
            return "Overdraft Penalty.";
        }
        else
        {
            accountBalance = tempBalance;
            saveTransaction("Withdrawl",amount);
            return "Amount Withdrawn.";
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
void Account::displayHistory(string beginning, string ending)
{
    // Convert date strings in to time_t to use in AVL search tree
    time_t startDate = displayHistoryHelper(beginning);
    time_t endDate = displayHistoryHelper(ending);
    time_t tempDate;

    if(endDate < startDate)
    {
        cout << "Invalid Dates" << endl;
        return;
    }

    string line;
    ifstream inFile;
    inFile.open("AccountData/"+accountNumber+"History.txt");
    while(getline(inFile,line))
    {
        tempDate = stoi(line.substr(0,line.find(" ")));
        if(tempDate > startDate && tempDate < endDate)
            cout << convertTimeToString(tempDate) + " " + line.substr(line.find(" ")+1,line.length()) << endl;
    }
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
    EncryptionBox::positionInFile = 0;
    ofstream outFile;

    outFile.open("AccountData/"+accountNumber+".txt", ofstream::trunc); // attempt to open file with intent to overwirite existing data
    outFile << EncryptionBox::encrypt(accountNumber) << endl;
    outFile << EncryptionBox::encrypt(accountHolderUserID) << endl;
    outFile << EncryptionBox::encrypt(accountHolderFirstName) << endl;
    outFile << EncryptionBox::encrypt(accountHolderLastName) << endl;
    outFile << EncryptionBox::encrypt(accountHolderPhoneNumber) << endl;
    outFile << EncryptionBox::encrypt(accountHolderAddress) << endl;
    outFile << EncryptionBox::encrypt(to_string(openDate)) << endl;
    outFile << EncryptionBox::encrypt(to_string(closeDate)) << endl;
    outFile << EncryptionBox::encrypt(to_string(maturityDate)) << endl;
    outFile << EncryptionBox::encrypt(getDisplayNum(accountBalance)) << endl;
    if(restrictedStatus)
        outFile << EncryptionBox::encrypt("True") << endl;
    else
        outFile << EncryptionBox::encrypt("False") << endl;
    if(openStatus)
        outFile << EncryptionBox::encrypt("True") << endl;
    else
        outFile << EncryptionBox::encrypt("False") << endl;
    
    // Store accountType data (must use getters since inherited atributes are private)
    outFile << EncryptionBox::encrypt(getDisplayNum(getMonthlyFee())) << endl;
    outFile << EncryptionBox::encrypt(getDisplayNum(getServiceFee())) << endl;
    outFile << EncryptionBox::encrypt(getDisplayNum(getInterestRate())) << endl;
    outFile << EncryptionBox::encrypt(getDisplayNum(getMinimumBalance())) << endl;
    outFile << EncryptionBox::encrypt(getAccountTypeName()) << endl;

    outFile.close();
    return "Saved";
}

/**********************************************************
/ buildFromFile (private) build account data from file
/
/ parameters:
/   acctNum : string
/
/ returns:
/   void
*//////////////////////////////////////////////////////////
void Account::buildFromFile(string acctNum)
{
    EncryptionBox::positionInFile = 0;
    ifstream inFile;
    string line;
    inFile.open("AccountData/"+acctNum+".txt");
    if(inFile)
    {
        getline(inFile, line);
        accountNumber = EncryptionBox::decrypt(line.substr(0,line.find("\r")));
        getline(inFile, line);
        accountHolderUserID = EncryptionBox::decrypt(line.substr(0,line.find("\r")));
        getline(inFile, line);
        accountHolderFirstName = EncryptionBox::decrypt(line.substr(0,line.find("\r")));
        getline(inFile, line);
        accountHolderLastName = EncryptionBox::decrypt(line.substr(0,line.find("\r")));
        getline(inFile, line);
        accountHolderPhoneNumber = EncryptionBox::decrypt(line.substr(0,line.find("\r")));
        getline(inFile, line);
        accountHolderAddress = EncryptionBox::decrypt(line.substr(0,line.find("\r")));
        getline(inFile, line);
        openDate = stoi(EncryptionBox::decrypt(line.substr(0,line.find("\r"))));
        getline(inFile, line);
        closeDate = stoi(EncryptionBox::decrypt(line.substr(0,line.find("\r"))));
        getline(inFile, line);
        maturityDate = stoi(EncryptionBox::decrypt(line.substr(0,line.find("\r"))));
        getline(inFile, line);
        accountBalance = stod(EncryptionBox::decrypt(line.substr(0,line.find("\r"))));
        getline(inFile, line);
        if(EncryptionBox::decrypt(line.substr(0,line.find("\r"))) == "True")
            restrictedStatus = true;
        else
            restrictedStatus = false;
        getline(inFile, line);
        if(EncryptionBox::decrypt(line.substr(0,line.find("\r"))) == "True")
            openStatus = true;
        else
            openStatus = false;

        // defind accountType attributes (must use setters since attributes are inaccessible)
        getline(inFile, line);
        setMonthlyFee(stod(EncryptionBox::decrypt(line.substr(0,line.find("\r")))));
        getline(inFile, line);
        setServiceFee(stod(EncryptionBox::decrypt(line.substr(0,line.find("\r")))));
        getline(inFile, line);
        setInterestRate(stod(EncryptionBox::decrypt(line.substr(0,line.find("\r")))));
        getline(inFile, line);
        setMinimumBalance(stod(EncryptionBox::decrypt(line.substr(0,line.find("\r")))));
        getline(inFile, line);
        setAccountTypeName(EncryptionBox::decrypt(line.substr(0,line.find("\r"))));
    }
    else
    {
        cout << "File not found." << endl;
    }
    inFile.close();
}

/**********************************************************
/ saveNextAccountNumbers (public) save the next account
/ numbers to a file.
/
/ parameters:
/   none
/
/ returns:
/   void
*//////////////////////////////////////////////////////////
void Account::saveNextAccountNumbers()
{
    // save account numbers
    ofstream outFile;
    outFile.open("AccountData/nextAccountNumbers.txt", ofstream::trunc);
    outFile << nextCheckingAccountNumber << endl;
    outFile << nextSavingsAccountNumber << endl;
    outFile << nextCDAccountNumber << endl;
    outFile << nextUniqueAccountNumber << endl;
    outFile.close();
}

/**********************************************************
/ saveTransaction (private) append the tansaction to file
/
/ parameters:
/   type : string, amount : double
/
/ returns:
/   void
*//////////////////////////////////////////////////////////
void Account::saveTransaction(string type, double amount)
{
    time_t date;
    time(&date);
    ofstream outFile;
    outFile.open("AccountData/"+accountNumber+"History.txt", ofstream::app);
    outFile << to_string(date) + " " + getDisplayNum(amount) + " " + type << endl;
    outFile.close();
}