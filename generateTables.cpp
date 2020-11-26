#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Data Structures/AccountTable.h"
#include "Data Structures/AVL.h"
using namespace std;

int main()
{
    AVLTree<string> tree;
    tree.insertWithItem("1", "2");
    tree.display();
}

/*
int main()
{
    AVLTree<string> nameTable, phoneTable;
    AccountTable accountTable;

    accountTable.buildTable("Tables/AccountTable.txt");
    nameTable.buildTree("Tables/NameTable.txt");
    phoneTable.buildTree("Tables/PhoneTable.txt");

    cout << endl << "Start of Account Table:" << endl;
    accountTable.display();
    cout << endl << "Start of Name Table:" << endl;
    nameTable.display();
    cout << endl << "Start of Phone Table:" << endl;
    phoneTable.display();
    
    return 0;
}
*/