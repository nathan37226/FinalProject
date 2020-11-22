#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Dictionary.h"
using namespace std;

int main()
{
    vector<string> nameList = {};
    vector<string> phoneNumList = {};
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

    Dictionary acctTable;
    for (int i = 0; i < nameList.size(); i++)
    {
        string acctInfo = nameList[i] + " " + phoneNumList[i];
        entry acctTableEntry(acctList[i], acctInfo); //creates an entry for the acctTable
        acctTable.insert(acctTableEntry);
    }

    acctTable.display();
    acctTable.getStatistics();
    return 0;
}