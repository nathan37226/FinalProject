#include <iostream>
#include <string>
#include <cmath>
using namespace std;

double roundNum(double value, int precision);
string getDisplayNum(double input);
string incrementAcctNum(string lastActNum);

int main()
{
    string oldAcctNum = "C002999999";
    string newAcctNum = incrementAcctNum(oldAcctNum);
    cout << "Old Account Number: " << oldAcctNum << endl << "New Account Number: " << newAcctNum << endl << endl;

    double num = 105.68532;
    double roundedNum = roundNum(num, 2); //for money, we round to the 2nd decimal
    string formattedNum = getDisplayNum(roundedNum);
    cout << "Original num: " << num << endl << "Rounded num: " << roundedNum << endl << "Num formatted to display: $" << formattedNum << endl;
    return 0;
}

double roundNum(double value, int precision)
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


string getDisplayNum(double input)
{
    string num = to_string(input);
    return num.substr(0, num.length() - 4); //cuts off the last 0000 of the double
}

string incrementAcctNum(string lastAcctNum)
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