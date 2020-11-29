#ifndef ALLDATASTRUCTURES_H
#define ALLDATASTRUCTURES_H

//place Accounts.h/.cpp up here since AccountTable depends upon it!

#include "AVL.h"
#include "EncryptionBox.h"
#include "AccountTable.h"

//allows for easy passing of all tables, best to do by reference since they can get large!
struct tableSet
{
    AVLTree<string> firstNameTable, lastNameTable, phoneNumTable, addressTable, userTable;
    AccountTable accountTable;
};

//Place all user objs here!

#endif