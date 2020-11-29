#ifndef ALLDATASTRUCTURES_H
#define ALLDATASTRUCTURES_H

#include "Users/Account/Account.h"

#include "DataTables/AVL.h"
#include "DataTables/EncryptionBox.h"
#include "DataTables/AccountTable.h"

//allows for easy passing of all tables, best to do by reference since they can get large!
struct tableSet
{
    AVLTree<string> firstNameTable, lastNameTable, phoneNumTable, addressTable, userTable;
    AccountTable accountTable;
};

#include "Users/Tools.h"
#include "Users/User.h"
#include "Users/Admin.h"
#include "Users/Official.h"
#include "Users/Client.h"
#include "Users/DataHandler.h"

#endif