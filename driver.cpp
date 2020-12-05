/*
Nathan Obert M03134502
This file runs the Bear Bank program!
Tables:
    AccountTable:
        accountTable -- AcctNum -> info about acct
    AVLTree<string>:
        firstNameTable -- First name -> acct nums with that first name
        lastNameTable -- Last name -> acct nums with that last name
        phoneNumTable -- Phone num -> acct nums with that phone num
        addressTable -- Address -> acct nums with that address
        userTable -- User ID -> {hashed password, userType, accts...}, if accts are applicable to the user type

Default User Accounts:              These are hard coded to work each time the program is launched
    Admin: admin - password1
    Official: official - password1
*/
#include "Data Structures/AllDataStructures.h" //include all classes into this header! ensure all classes are in that folder, too
#include "driverOS.cpp" //all functs used to run driver are placed in here to reduce clutter
#include "driverClientOS.cpp"
#include "driverOfficialOS.cpp"
#include "driverAdminOS.cpp"
using namespace std;

int main()
{
    initialSetup(); //not finished--need to do interest on accounts

    string loginScreen = "Welcome to BearBank!\n";
    bool wantsToCloseBearBank = false;
    cout << loginScreen << endl;

    while (!wantsToCloseBearBank)
    {
        string loginPrompt = "[1] Login To An Existing Account\n[2] Forgot User ID / Password\n[3] Enroll in Bear Bank\n[4] Close Bear Bank";
        cout << loginPrompt << endl << "Option: ";
        int initialOption = getUserOption(4);
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
                    cout << "Invalid User ID or Password." << endl << endl;
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
                    string firstName = "", lastName = "", phoneNum = "", address = "", clientID = "", password = "";
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
                    getline(cin, clientID); //check for non duplicates!!!
                    isAvailableID = DataHandler::isAvaliableUserID(clientID);

                    while (!isAvailableID) //validation check on clientID
                    {
                        cout << "That user ID is not available." << endl;
                        cout << "Enter a new user ID: ";
                        getline(cin, clientID);
                        isAvailableID = DataHandler::isAvaliableUserID(clientID);
                    }
                    cout << "Enter your new password: ";
                    getline(cin, password);
                    cout << endl;

                    //now to create the Client obj
                    Client user(firstName + " " + lastName, clientID, EncryptionBox::hash(password), "client", address, phoneNum);
                    user.setRecentActivity("Just Enrolled!");
                    user.saveUser(); //creates a .txt file record

                    //create Automated Official to register enrolled user inside tables
                    Official official;
                    official.buildUser("UserData/official.txt");
                    official.addNewClientUser(user);
                    official.setRecentActivity("Enrolled new user: " + clientID);
                    official.saveUser();

                    //create initial savings account for new client!
                    Account newAccount("Basic Checking", clientID, firstName, lastName, phoneNum, address);
                    double newBal = newAccount.getMinimumBalance();
                    if (newBal > 0)
                    {
                        newAccount.deposit(newBal);
                    }
                    official.addAccountToClient(user, newAccount);
                    newAccount.saveToFile();
                    cout << "A Basic Checking Account has been Automatically Created for You." << endl;
                    cout << "Happy Banking!" << endl << endl;
                }
                break;
            }
            case 4: //close
            {
                wantsToCloseBearBank = true;
                break;
            }
        }
    }
    
    savingBank(); //saves all stored info accumulated or changed throughout program's runtime
    return 0;
}

