#include <iostream>
#include <string>
#include "Data Structures/AllDataStructures.h" //include all classes into this header! ensure all classes are in that folder, too
using namespace std;

bool isValidOption(string input, int upperBound);
int getUserOption(int upperBound);
void userLoginReset();

int main()
{
    //Perform intial start-up of all related tables and bank accounts!

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

//validates the user's input is within a range of options; max possible range is 1-9, for now
bool isValidOption(string input, int upperBound)
{
    if (input.length() > 1)
    {
        return false;
    }
    else
    {
        try
        {
            int option = stoi(input); //since only 1 char is ensured, if 'a' is entered, will throw an error to catch block
            bool returnValue = ( (option <= upperBound) && (option >= 1) ) ? true : false;
            return returnValue;
        }

        catch (...)
        {
            return false;
        }
    }   
}

//upperBound is the max option possible, must be 1-9
int getUserOption(int upperBound)
{
    string userInput = "";
    bool isValid = false;
    while (true)
    {
        getline(cin, userInput);
        isValid = isValidOption(userInput, 3);
        if (!isValid)
        {
            cout << "Invalid option! Try again: ";
        }
        else
        {
            break;
        }
    }
    return stoi(userInput);
}

void userLoginReset()
{
    cout << "Is your User ID or Password giving you trouble?" << endl << endl;
    string resetOptions = "[1] The User ID\n[2] The Password";
    cout << resetOptions << endl << "Option: ";
    int userResetOption = getUserOption(2);
    cout << endl;

    switch (userResetOption)
    {
        case 1:
        {
            cout << "In order for me to provide you with your User ID, I must have one of your Account Numbers." << endl;
            cout << "Do you know any of your Account Numbers?" << endl << endl;
            string accountNumPrompt = "[1] Yes I do\n[2] No I don't";
            cout << accountNumPrompt << endl << "Option: ";
            int knowsAccountNums = getUserOption(2);
            cout << endl;

            if (knowsAccountNums == 1)
            {
                string accountNum = "";
                cout << "Awesome! Please enter your Account Number: ";
                getline(cin, accountNum);
                /*
                perform search for User ID based off of accountNum
                if (account is found)
                {
                    tell user the corresponding username
                }
                else
                {
                    say the account number was not found
                    give options to try again one time
                    if (not found second time)
                        stop helping
                    else
                        tell user the corresponding username
                }
                */
            }
            else
            {
                cout <<  "I regret to inform you that you must know an Account Number for me to provide you with your User ID." << endl;
                cout << "Please do visit a branch office for further assistence!" << endl;
            }
            
            break;
        }
        case 2:
        {
            cout << "Okay, to reset your password, I need your User ID." << endl;
            cout << "Please enter your User ID: ";
            string userID = "", newPassword = "";
            getline(cin, userID);
            /*
            Check and see if userID exists!
            if (exists)
            {
                get new password from user
                store hashed password in UserName -> Hashed-Password table
            }
            else
            {
                say that username couldn't be found
                retry getting username one more time
                if (exists)
                {
                    get new password from user
                    store hashed password in UserName -> Hashed-Password table
                }
                else
                {
                    tell to go visit branch location for further assistence
                }
            }
            */           
            break;
        }
    }
}
