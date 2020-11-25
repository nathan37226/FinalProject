#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Data Structures/AccountTable.h"
#include "Data Structures/AVL.h"
using namespace std;

int main()
{
    //phoneTable.saveInfo("Tables/PhoneTable.txt");
    //nameTable.saveInfo("Tables/NameTable.txt");
    //acctTable.saveInfo("Tables/AccountTable.txt");

    AVLTree<string> nameTree, phoneTree;
    nameTree.buildTree("Tables/NameTable - Copy.txt");
    phoneTree.buildTree("Tables/PhoneTable - Copy.txt");
    nameTree.display();
    phoneTree.display();
    nameTree.saveInfo("Tables/NameTable.txt");
    phoneTree.saveInfo("Tables/PhoneTable.txt");

    
    return 0;
}
