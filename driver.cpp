#include <iostream>
#include <string>
#include "Data Structures/AllDataStructures.h" //include all classes into this header! ensure all classes are in that folder, too
using namespace std;

bool isValidOption(string input, int upperBound);
int getUserOption(int upperBound);

int main()
{
    string loginScreen = "Welcome to BearBank!\n";
    string loginPrompt = "[1] Forgot User ID / Password?\n[2] Open An Account\n[3] Login To An Existing Account";
    cout << loginScreen << endl << loginPrompt << endl << "Option: ";
    int option = getUserOption(3);

    switch (option)
    {
        case 1:
        {
            break;
        }
        case 2:
        {
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
