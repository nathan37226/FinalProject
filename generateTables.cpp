#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Data Structures/AccountTable.h"
#include "Data Structures/AVL.h"
using namespace std;

int main()
{
    /*vector<string> nameList = {}; //can have multiple accts with same name
    vector<string> phoneNumList = {}; //can have multiple accts with same phone num
    vector<string> acctList = {};

    for (int i = 0; i < 100; i++)
    {
        string acctNum = "C000" + to_string(i);
        acctList.push_back(acctNum);
    }
    for (int i = 0; i < 50; i++)
    {
        string name = "Name" + to_string(i);
        nameList.push_back(name);
    }

    for (int i = 1000; phoneNumList.size() < 50; i += 2)
    {
        string num = "417-555-" + to_string(i);
        phoneNumList.push_back(num);
    }

    AccountTable acctTable;
    AVLTree<string> phoneTable, nameTable;

    for (int i = 0; i < nameList.size(); i++)
    {
        accountEntry firEntry(acctList[i], nameList[i] + " " + phoneNumList[i]);
        accountEntry secEntry(acctList[acctList.size() - i - 1], nameList[i] + " " + phoneNumList[i]);
        acctTable.insert(firEntry);
        acctTable.insert(secEntry);
        nameTable.insertWithItem(nameList[i], acctList[i]);
        nameTable.insertWithItem(nameList[i], acctList[acctList.size() - i - 1]);
        phoneTable.insertWithItem(phoneNumList[i], acctList[i]);
        phoneTable.insertWithItem(phoneNumList[i], acctList[acctList.size() - i - 1]);
    }

    acctTable.saveInfo("Tables/AccountTable.txt");
    nameTable.saveInfo("Tables/NameTable.txt");
    phoneTable.saveInfo("Tables/PhoneTable.txt");*/

    AccountTable newTable;
    newTable.buildTable("Tables/AccountTable.txt");
    
    newTable.updateInfo("C00091", "Here is some new info");
    newTable.display();
    

    return 0;
}
