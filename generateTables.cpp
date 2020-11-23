#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "AccountTable.h"
#include "InfoTable.h"
using namespace std;

int main()
{
    vector<string> nameList = {}; //can have multiple accts with same name
    vector<string> phoneNumList = {}; //can have multiple accts with same phone num
    vector<string> acctList = {};

    for (int i = 0; i < 100; i++)
    {
        string name = "Name" + to_string(i);
        string acctNum = "C000" + to_string(i);
        nameList.push_back(name);
        acctList.push_back(acctNum);
    }
    for (int i = 1000; phoneNumList.size() < 100; i += 2)
    {
        string num = "417-555-" + to_string(i);
        phoneNumList.push_back(num);
    }

    AccountTable acctTable, phoneNumTable, nameTable;
    for (int i = 0; i < nameList.size(); i++)
    {
        string acctInfo = nameList[i] + " " + phoneNumList[i];
        accountEntry acctTableEntry(acctList[i], acctInfo); //creates an entry for the acctTable
        acctTable.insert(acctTableEntry);

        accountEntry nameEntry(nameList[i], acctList[i]);
        nameTable.insert(nameEntry);

        accountEntry phoneNumEntry(phoneNumList[i], acctList[i]);
        phoneNumTable.insert(phoneNumEntry);
    }

    //acctTable.display();
    acctTable.getStatistics();
    string filename = "Tables/AccountTable.txt";
    acctTable.writeInfo(filename);
    nameTable.writeInfo(filename);
    phoneNumTable.writeInfo(filename);

    return 0;
}