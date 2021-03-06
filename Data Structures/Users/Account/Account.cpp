// Implementation file for Account class
#include "Account.h"
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

void accountInit();
void accountExit();

void accountInit()
{
    AccountType checkingAccount("Basic Checking", 0.0, 0.0, 0.0, -50.0, false);
    AccountType savingsAccount("Entry Saving", 0.0, 0.0, 2.0, 5.0, false);
    AccountType CD("Certificate of Deposit", 0.0, 0.0, 5.0, 0.0, true);
    AccountType::setPenaltyFee(25.00);
    ifstream inFile;
    inFile.open("AccountData/nextAccountNumbers.txt");
    if(inFile)
    {
        EncryptionBox::positionInFile = 0;
        string line = "";

        getline(inFile,line);
        line = line.substr(0, line.rfind("\r"));
        Account::nextCheckingAccountNumber = EncryptionBox::decrypt(line);

        getline(inFile,line);
        line = line.substr(0, line.rfind("\r"));
        Account::nextSavingsAccountNumber = EncryptionBox::decrypt(line);

        getline(inFile,line);
        line = line.substr(0, line.rfind("\r"));
        Account::nextCDAccountNumber = EncryptionBox::decrypt(line);

        getline(inFile,line);
        line = line.substr(0, line.rfind("\r"));
        Account::nextUniqueAccountNumber = EncryptionBox::decrypt(line);
    }
    else
    {
        Account::nextCheckingAccountNumber = "C000000001";
        Account::nextSavingsAccountNumber = "S000000001";
        Account::nextCDAccountNumber = "D000000001";
        Account::nextUniqueAccountNumber = "U000000001";
    }
    inFile.close();
    cout << endl;
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
AccountType::AccountType(string acctTypeName, double monFee, double servFee, double interestR, double minBalance, bool isCert)
{
    if(acctTypeName == "blank")
    {
        monthlyFee = 0.0;
        serviceFee = 0.0;
        interestRate = 0.0;
        minimumBalance = 0.0;
        isCertificate = false;
        accountTypeName = "";
    }
    else
    {
        ifstream inFile;
        //attempt to open account type
        inFile.open("AccountData/"+acctTypeName+".txt");
        if(inFile)
        {
            EncryptionBox::positionInFile = 0;
            string line = "";
            getline(inFile,line);
            line = line.substr(0, line.rfind("\r"));
            if(EncryptionBox::decrypt(line) == "True")
                isCertificate = true;
            else
                isCertificate = false;
            getline(inFile,line);
            line = line.substr(0, line.rfind("\r"));
            monthlyFee = stod(EncryptionBox::decrypt(line));
            getline(inFile,line);
            line = line.substr(0, line.rfind("\r"));
            serviceFee = stod(EncryptionBox::decrypt(line));
            getline(inFile,line);
            line = line.substr(0, line.rfind("\r"));
            interestRate = stod(EncryptionBox::decrypt(line));
            getline(inFile,line);
            line = line.substr(0, line.rfind("\r"));
            minimumBalance = stod(EncryptionBox::decrypt(line));
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
            isCertificate = isCert;
            // save account type to file
            EncryptionBox::positionInFile = 0;
            ofstream outFile("AccountData/"+acctTypeName+".txt", ofstream::trunc);
            if(isCert)
                outFile << EncryptionBox::encrypt("True") << endl;
            else
                outFile << EncryptionBox::encrypt("False") << endl;
            outFile << EncryptionBox::encrypt(getDisplayNum(monFee)) << endl;
            outFile << EncryptionBox::encrypt(getDisplayNum(servFee)) << endl;
            outFile << EncryptionBox::encrypt(getDisplayNum(interestR)) << endl;
            outFile << EncryptionBox::encrypt(getDisplayNum(minBalance)) << endl;
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

void AccountType::setIsCertificate(bool setting)
{
    isCertificate = setting;
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

bool AccountType::getIsCertificate()
{
    return isCertificate;
}

/**********************************************************
/ roundNum rounds a floating point number to
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
    if(acctTypeName == "Basic Checking")
    {
        accountNumber = nextCheckingAccountNumber;
        nextCheckingAccountNumber = incrementAcctNum(nextCheckingAccountNumber);
    }
    else if(acctTypeName == "Entry Saving")
    {
        accountNumber = nextSavingsAccountNumber;
        nextSavingsAccountNumber = incrementAcctNum(nextSavingsAccountNumber);
    }
    else if(acctTypeName == "Certificate of Deposit")
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
    if(getIsCertificate()) // if the account type is a certificate of deposit
    {
        if(mDate >= openDate + 31556926) // set the minimum maturity date to one year
            maturityDate = mDate;
        else
            maturityDate = openDate + 31556926;
    }
    else
        maturityDate = 0;
    

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
    if(acctNum != "")
    {
        buildFromFile(acctNum);
        interestCalc();
    }
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
    if(!status)
    {
        if(time(0) < getMaturityDate())
            accountBalance -= getPenaltyFee();
    }
}

void Account::setAccountClosedBy(string name)
{
    accountClosedBy = name;
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

string Account::getAccountTableInfo()
{
    string openOrNot = (openStatus == true) ? "Open: " : "Closed: ";
    return openOrNot+getAccountTypeName()+" $"+getDisplayNum(accountBalance)+" "+accountHolderFirstName+" "+accountHolderLastName+" "+accountHolderPhoneNumber+" "+accountHolderAddress+" "+accountHolderUserID;
}

string Account::getOpenDate()
{
    return convertTimeToString(openDate);
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

string Account::getAccountClosedBy()
{
    return accountClosedBy;
}

string Account::getCloseDate()
{
    return convertTimeToString(closeDate); //returns readable form of time_t
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
    return "Amount added";
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
    // get current time
    time_t currTime;
    time(&currTime);
    if(!getRestrictedStatus() && currTime > maturityDate)
    {
        double tempBalance = accountBalance - roundNum(amount, 2);
        if(tempBalance < getMinimumBalance())
        {
            return "Insufficient Funds";
        }
        else if(tempBalance < 0 && tempBalance > getMinimumBalance())
        {
            accountBalance = tempBalance - getPenaltyFee();
            saveTransaction("Withdrawl",-1 * amount);
            saveTransaction("Overdraft",-1 * getPenaltyFee());
            return "Amount Withdrawn, Overdraft Penalty";
        }
        else
        {
            accountBalance = tempBalance;
            saveTransaction("Withdrawl",amount);
            return "Amount Withdrawn";
        }
    }
    else
        return "Account Restricted";
    return "Something else happened";
}

void Account::doServiceCharge()
{
    accountBalance -= getServiceFee();
}

/**********************************************************
/ interestCalc (private) calculates the accululated interest
/ since the account was last saved
/
/ parameters:
/   none
/
/ returns:
/   void
*//////////////////////////////////////////////////////////
void Account::interestCalc()
{
    //get number of days since last interest calculation
    time_t currTime;
    time(&currTime);
    time_t timePassed = currTime - lastInterestCalculation;
    int days = timePassed / 86400; //86400 seconds in a day, and the int type of day will round like a floor function
    if(accountBalance > 0.0)
    {
        accountBalance += accountBalance * getInterestRate()/100 * days/365;
    }
    if(days)
        time(&lastInterestCalculation); //save the new time interest was calculated if a day or more has passed
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
    time_t startDate = convertDateToTimeT(beginning);
    time_t endDate = convertDateToTimeT(ending);
    time_t tempDate;

    if(endDate < startDate || endDate == 0 || startDate == 0)
    {
        cout << "Invalid Dates" << endl << endl << endl;
        return;
    }

    string line;
    ifstream inFile;
    inFile.open("AccountData/"+accountNumber+"History.txt");
    while(getline(inFile,line))
    {
        EncryptionBox::positionInFile = 0;
        line = line.substr(0, line.rfind("\r"));
        line = EncryptionBox::decrypt(line);
        tempDate = stoi(line.substr(0,line.find(" ")));
        if(tempDate > startDate && tempDate < endDate)
            cout << convertTimeToString(tempDate) + " " + line.substr(line.find(" ")+1,line.length()) << endl;
    }
    cout << endl << endl;
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
time_t Account::convertDateToTimeT(string date)
{
    int month;
    int day;
    int year;
    int hour;
    int minute;
    int second;
    // parse out time info from string
    if(isValidNumber(date.substr(0,2)))
        month = stoi(date.substr(0,2));
    else
        return 0;
    if(isValidNumber(date.substr(3,2)))
        day = stoi(date.substr(3,2));
    else
        return 0;
    if(isValidNumber(date.substr(6,4)))
        year = stoi(date.substr(6,4));
    else
        return 0;
    if(isValidNumber(date.substr(11,2)))
        hour = stoi(date.substr(11,2));
    else
        return 0;
    if(isValidNumber(date.substr(14,2)))
        minute = stoi(date.substr(14,2));
    else
        return 0;
    if(isValidNumber(date.substr(17,2)))
        second = stoi(date.substr(17,2));
    else
        return 0;

    // build struct tm
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    if(year >= 1900)
        timeinfo->tm_year = year - 1900;
    else
        return 0;
    if(month >= 1 && month <= 12)
        timeinfo->tm_mon = month - 1;
    else
        return 0;
    if(day >= 0 && day <= 31 && !(day > 30 && (month == 4 || month == 6 || month == 9 || month == 11)) && !(day > 29 && month == 2))
        timeinfo->tm_mday = day;
    else
        return 0;
    if(hour >= 0 && hour < 24)
        timeinfo->tm_hour = hour;
    else
        return 0;
    if(minute >= 0 && minute < 60)
        timeinfo->tm_min = minute;
    else
        return 0;
    if(second >= 0 && second < 60)
        timeinfo->tm_sec = second;
    else
        return 0;
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
    outFile << EncryptionBox::encrypt(to_string(lastInterestCalculation)) << endl;
    
    // Store accountType data (must use getters since inherited atributes are private)
    if(getIsCertificate())
        outFile << EncryptionBox::encrypt("True") << endl;
    else
        outFile << EncryptionBox::encrypt("False") << endl;
    outFile << EncryptionBox::encrypt(getDisplayNum(getMonthlyFee())) << endl;
    outFile << EncryptionBox::encrypt(getDisplayNum(getServiceFee())) << endl;
    outFile << EncryptionBox::encrypt(getDisplayNum(getInterestRate())) << endl;
    outFile << EncryptionBox::encrypt(getDisplayNum(getMinimumBalance())) << endl;
    outFile << EncryptionBox::encrypt(getAccountTypeName()) << endl;

    //acct closed by name
    outFile << EncryptionBox::encrypt(accountClosedBy) << endl;

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
        accountNumber = EncryptionBox::decrypt(line.substr(0,line.rfind("\r")));
        getline(inFile, line);
        accountHolderUserID = EncryptionBox::decrypt(line.substr(0,line.rfind("\r")));
        getline(inFile, line);
        accountHolderFirstName = EncryptionBox::decrypt(line.substr(0,line.rfind("\r")));
        getline(inFile, line);
        accountHolderLastName = EncryptionBox::decrypt(line.substr(0,line.rfind("\r")));
        getline(inFile, line);
        accountHolderPhoneNumber = EncryptionBox::decrypt(line.substr(0,line.rfind("\r")));
        getline(inFile, line);
        accountHolderAddress = EncryptionBox::decrypt(line.substr(0,line.rfind("\r")));
        getline(inFile, line);
        openDate = stoi(EncryptionBox::decrypt(line.substr(0,line.rfind("\r"))));
        getline(inFile, line);
        closeDate = stoi(EncryptionBox::decrypt(line.substr(0,line.rfind("\r"))));
        getline(inFile, line);
        line = EncryptionBox::decrypt(line.substr(0,line.rfind("\r")));
        maturityDate = stoi(line);
        getline(inFile, line);
        accountBalance = stod(EncryptionBox::decrypt(line.substr(0,line.rfind("\r"))));
        getline(inFile, line);
        if(EncryptionBox::decrypt(line.substr(0,line.rfind("\r"))) == "True")
            restrictedStatus = true;
        else
            restrictedStatus = false;
        getline(inFile, line);
        if(EncryptionBox::decrypt(line.substr(0,line.rfind("\r"))) == "True")
            openStatus = true;
        else
            openStatus = false;
        
        getline(inFile, line);
        lastInterestCalculation = stoi(EncryptionBox::decrypt(line.substr(0,line.rfind("\r"))));

        // defind accountType attributes (must use setters since attributes are inaccessible)
        getline(inFile, line);
        if(EncryptionBox::decrypt(line.substr(0,line.rfind("\r"))) == "True")
            setIsCertificate(true);
        else
            setIsCertificate(false);
        getline(inFile, line);
        setMonthlyFee(stod(EncryptionBox::decrypt(line.substr(0,line.rfind("\r")))));
        getline(inFile, line);
        setServiceFee(stod(EncryptionBox::decrypt(line.substr(0,line.rfind("\r")))));
        getline(inFile, line);
        setInterestRate(stod(EncryptionBox::decrypt(line.substr(0,line.rfind("\r")))));
        getline(inFile, line);
        setMinimumBalance(stod(EncryptionBox::decrypt(line.substr(0,line.rfind("\r")))));
        getline(inFile, line);
        setAccountTypeName(EncryptionBox::decrypt(line.substr(0,line.find("\r"))));

        //account closed by
        getline(inFile, line);
        accountClosedBy = EncryptionBox::decrypt(line.substr(0,line.find("\r")));
    }
    else
    {
        cout << "File not found" << endl;
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
    EncryptionBox::positionInFile = 0;
    ofstream outFile;
    outFile.open("AccountData/nextAccountNumbers.txt", ofstream::trunc);
    outFile << EncryptionBox::encrypt(nextCheckingAccountNumber) << endl;
    outFile << EncryptionBox::encrypt(nextSavingsAccountNumber) << endl;
    outFile << EncryptionBox::encrypt(nextCDAccountNumber) << endl;
    outFile << EncryptionBox::encrypt(nextUniqueAccountNumber) << endl;
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
    EncryptionBox::positionInFile = 0;
    ofstream outFile;
    outFile.open("AccountData/"+accountNumber+"History.txt", ofstream::app);
    string transactionInfo = to_string(date) + " " + getDisplayNum(amount) + " " + type;
    outFile <<  EncryptionBox::encrypt(transactionInfo) << endl;
    outFile.close();
}

bool Account::isValidNumber(string inputNum)
{
    string validNumbers = "0123456789.";
    bool match = false; 
    int count = 0;   

    for (int n = 0; n < inputNum.length(); n++)
    {
        if (validNumbers.find( inputNum[n] ) == string::npos)
        {
            return false;
        }
        else if (inputNum[n] == validNumbers[10]) //if it's a decimal
        {
            count++; //keeps track of how many decimals there are
        }   
    }   
    bool returnValue = (count <= 1) ? true : false; //can only be <=1 decimals in a valid number
    return returnValue;
}