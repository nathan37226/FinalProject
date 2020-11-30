/*
This file runs the Bear Bank program!
Tables Needed:
    AccountTable:
        accountTable -- AcctNum -> info about acct
    AVLTree<string>:
        firstNameTable -- First name -> acct nums with that first name
        lastNameTable -- Last name -> acct nums with that last name
        phoneNumTable -- Phone num -> acct nums with that phone num
        addressTable -- Address -> acct nums with that address
        userTable -- User ID -> {hashed password, userType, accts...}, if accts are applicable to the user type

Default User Accounts:
    Admin: admin - password1
    Official: official - password1
    Client: house - password1
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
    */
    initialSetup(); //not finished--need to do interest on accounts
    

    //Put inside a while loop at the end!
    string loginScreen = "Welcome to BearBank!\n";
    string loginPrompt = "[1] Login To An Existing Account\n[2] Forgot User ID / Password\n[3] Enroll in Bear Bank";
    cout << loginScreen << endl << loginPrompt << endl << "Option: ";
    int initialOption = getUserOption(3);
    cout << endl;

    switch (initialOption)
    {
        case 1: //login to acct
        {
            string userID = "", password = "", userType = "";
            cout << "Username: ";
            getline(cin, userID);
            cout << "Password: ";
            getline(cin, password);
            cout << endl;

            userType = DataHandler::isValidLogin(userID, password); //returns the type if successful, else "false"
            if (userType == "false")
            {
                cout << "Invalid User ID or Password." << endl;
            }
            else
            {
                if (userType == "client")
                {
                    clientLogin(userID);
                }    
                else if (userType == "official")
                {
                    officialLogin(userID);
                }
                else
                {
                    adminLogin(userID);
                }
            }
            break;
        }
        case 2: //reset pw or get userID
        {
            cout << "Connecting you with the Automated Bear Bank Administrator... Connected." << endl;
            cout << "Do you need help logging into your account?" << endl << endl;
            cout << "[1] Yes, I need help logging in\n[2] No, I was mistaken" << endl << "Option: ";
            int wantsHelp = getUserOption(2);
            cout << endl;

            if (wantsHelp == 1)
            {
                userLoginReset(); //needs finishing work!
            }
            else
            {
                cout << "If you ever do need any help, please contact Bear Bank anytime!" << endl;
            }

            break;
        }
        case 3: //new user enroll
        {
            cout << "Connecting you to the Automated Enrollment System... Connected." << endl;
            cout << "Before an account is created for you, do you wish to proceed enrolling into Bear Bank?" << endl;
            cout << "[1] Yes, I wish to create an account\n[2] No, I want to cancel enrolling" << endl;
            cout << "Option: ";
            int wantsToEnroll = getUserOption(2);
            cout << endl;

            if (wantsToEnroll == 1)
            {
                //getting initial user info
                string firstName = "", lastName = "", phoneNum = "", address = "", userName = "", password = "";
                cout << "Your personal information must be gathered to create an account." << endl;
                cout << "Enter your first name: ";
                getline(cin, firstName);
                cout << "Enter your last name: ";
                getline(cin, lastName);
                cout << "Enter your phone number: ";
                getline(cin, phoneNum);
                cout << "Enter your address: ";
                getline(cin, address);
                cout << endl;

                //creating online acct
                bool isAvailableID = false;
                cout << "Now, an online account will be created for you." << endl;
                cout << "Enter your desired user ID: ";
                getline(cin, userName); //check for non duplicates!!!
                isAvailableID = isValidUserID(userName);

                while (!isAvailableID) //validation check on userName
                {
                    cout << "That user ID is not available." << endl;
                    cout << "Enter a new user ID: ";
                    getline(cin, userName);
                    isAvailableID = isValidUserID(userName);
                }
                cout << "Enter your new password: ";
                getline(cin, password);
                cout << endl;

                //now to create the Client obj
                Client user(firstName + " " + lastName, userName, EncryptionBox::hash(password), "client");
                user.setRecentActivity("Just Enrolled!");
                user.saveUser(); //creates a .txt file record

                //create Automated Official to register enrolled user inside tables
                Official official;
                official.buildUser("UserData/official.txt");
                official.addNewClientUser(user);
                official.setRecentActivity("Enrolled new user: " + userName);
                official.saveUser();

                //create initial savings account for new client!
                //don't forget to update all tables with new acct!!
                //save accts in 'AccountData' folder!
            }
            break;
        }
    }
    
    savingTables(); //saves all stored info accumulated or changed throughout program's runtime
    return 0;
}

