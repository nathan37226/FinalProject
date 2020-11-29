/*
This file runs the Bear Bank program!
*/
#include <iostream>
#include <string>
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
    tableSet allTables; //still need to build the tables!
    initialBankSetup();
    

    //Put inside a while loop at the end!
    string loginScreen = "Welcome to BearBank!\n";
    string loginPrompt = "[1] Login To An Existing Account\n[2] Forgot User ID / Password\n[3] Enroll in Bear Bank";
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

            vector<string> userInfo = allTables.userTable.returnMappedItems(username);
            if (userInfo.size() == 0)
            {
                cout << "Invalid User ID Or Password!" << endl;
                break;
            }

            else
            {            
                savedHashPassword = userInfo[0];
                userType = userInfo[1];
                hashedInputPassword = EncryptionBox::hash(password);

                if (savedHashPassword == hashedInputPassword)
                {
                    if (userType == "Member")
                    {
                        memberLogin(allTables);
                    }    
                    else if (userType == "Official")
                    {
                        officialLogin(allTables);
                    }
                    else
                    {
                        adminLogin(allTables);
                    }
                }
            }
            
            break;
        }
        case 2:
        {
            cout << "Connecting you with the Automated Bear Bank Administrator... Connected." << endl;
            cout << "Do you need help logging into your online account?" << endl << endl;
            cout << "[1] Yes, I need help logging in\n[2] No, I was mistaken" << endl << "Option: ";
            int wantsHelp = getUserOption(2);
            cout << endl;
            if (wantsHelp == 1)
            {
                userLoginReset(allTables); //needs finishing work!
            }
            else
            {
                cout << "If you ever do need any help, please contact Bear Bank anytime!" << endl;
            }

            break;
        }
        case 3:
        {
            cout << "Connecting you to the Automated Enrollment System... Connected." << endl;
            /*
            Create new user
            Add single basic checking account for new user
            */
            break;
        }
    }
    
    return 0;
}

