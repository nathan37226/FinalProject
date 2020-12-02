#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include "Account.cpp"
using namespace std;



int main()
{
    accountInit();
    cout << endl;
    //Account a("checking","dom14","Dominic","Zucchini","417-551-2422","1234 N Street Place Springfield, MO, 65805",0,0.0);
    Account a("C000000001");
    a.deposit(200.001);
    a.withdraw(100.5);
    cout << a.getAccountHolderUserID() << endl;
    cout << Account::getDisplayNum(a.getAccountBalance()) << endl;
    a.displayHistory("12/01/2020 00:00:00", "12/02/2020 00:00:00");
    cout << a.saveToFile() << endl;
    cout << a.saveToFile() << endl;
    accountExit();


    return 0;
}