/*
This file runs the Bear Bank program!
*/
#include <iostream>
#include <string>
#include <iomanip>
#include <stdio.h>
#include "Data Structures/AllDataStructures.h" //include all classes into this header! ensure all classes are in that folder, too
#include "driverOS.cpp" //all functs used to run driver are placed in here to reduce clutter
using namespace std;

int main()
{
    /*
    Perform intial start-up of all related tables and bank accounts!
    Tables Needed:
        AccountTable:
            accountTable -- AcctNum -> info about acct
        AVLTree<string>:
            firstNameTable -- First name -> acct nums with that first name
            lastNameTable -- Last name -> acct nums with that last name
            phoneNumTable -- Phone num -> acct nums with that phone num
            addressTable -- Address -> acct nums with that address
            userTable -- User ID -> {hashed password, userType}
    */
    AVLTree<string> firstNameTable, lastNameTable, phoneNumTable, addressTable, userTable;
    AccountTable accountTable;
    

    //Put entire structure of main inside a while loop at the end!
    string loginScreen = "Welcome to BearBank!\n";
    string loginPrompt = "[1] Login To An Existing Account\n[2] Forgot User ID / Password\n[3] Open An Account";
    cout << loginScreen << endl << loginPrompt << endl << "Option: ";
    int initialOption = getUserOption(3);
    cout << endl;

    switch (initialOption)
    {
        case 1:
        {
            string username = "", password = "", hashedInputPassword = "", savedHashPassword = "", userType = "";
            cout << "Username: ";
            getline(cin, username);
            cout << "Password: ";
            getline(cin, password);

            vector<string> userInfo = userTable.returnMappedItems(username);
            hashedInputPassword = EncryptionBox::hash(password);
            savedHashPassword = userInfo[0];
            userType = userInfo[1];

            if (savedHashPassword == hashedInputPassword)
            {
                if (userType == "Member")
                {
                    memberLogin();
                }    
                else if (userType == "Official")
                {
                    officialLogin();
                }
                else
                {
                    adminLogin();
                }
            }

            else
            {
                cout << "Invalid User ID Or Password!" << endl;
            }
            
            break;
        }
        case 2:
        {
            cout << "Connecting you with a Bear Bank Representative... Connected." << endl;
            cout << "So, I'm hearing that you're having trouble logging into your account. Is this true?" << endl << endl;
            cout << "[1] Yes, I need help logging in\n[2] No, I was mistaken" << endl << "Option: ";
            int wantsHelp = getUserOption(2);
            cout << endl;
            if (wantsHelp == 1)
            {
                userLoginReset(); //needs finishing work!
            }
            else
            {
                cout << "Well, that's good news. If you ever do need any help, please contact us anytime!" << endl;
            }

            break;
        }
        case 3:
        {
            break;
        }
    }
    
    return 0;
}

