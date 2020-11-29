#ifndef ALLDATASTRUCTURES_H
#define ALLDATASTRUCTURES_H

#include "AVL.h"
#include "EncryptionBox.h"
#include "AccountTable.h"

//allows for easy passing of all tables, best to do by reference since they can get large!
struct tableSet
{
    AVLTree<string> firstNameTable, lastNameTable, phoneNumTable, addressTable, userTable;
    AccountTable accountTable;
};

//add all user and acct objs here!

#endif