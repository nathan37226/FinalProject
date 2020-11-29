/*
This is a header file for a dictionary ADT, named AccountTable. This dictionary will be an array-based 
implementation. Each index of the array will implement seperate chainging through a linked list. A given 
key will hash to a specific index, or really linked list, that contains both the key and info related to 
the key. Further, all keys will be strings in this dictionary, and all hashing will be done by the 
EncryptionBox.
As the array has a linked list in each index, an object needed to be created to be inserted into the list.
This is why the 'accountEntry' object was created. Further, in AccountTable.cpp, the << operator was
overloaded so that the accountEntry object could be directly displayed by the linkedlist.
The keys for this will be accountNums, and the info string will be the general info about the account,
which includes account holder name, phone num, address, etc.
*/
#ifndef ACCOUNTTABLE_H
#define ACCOUNTTABLE_H
#include <iostream>
#include <string>
#include <fstream>
#include "EncryptionBox.h"
#include "LinkedList.h"
using namespace std;

struct accountEntry
{
    string key;
    string info; //this is where info associated with the key would be stored

    accountEntry();
    accountEntry(string keyValue);
    accountEntry(string keyValue, string infoValue);
    bool operator == (const accountEntry &obj);
    bool operator != (const accountEntry &obj); //so that can be inserted and removed from linked list, compares based on key value
};

class AccountTable
{
private:
    LinkedList<accountEntry> *dict;

    //Used for statistics of list and hashing
    unsigned int capacity;
    unsigned int usedIndicies;
    unsigned int totalKeys;
    double loadFactor; //will be determined based off of totalKeys / capacity
    float loadThreshold;
    int maxCollisions;

protected:
    void updateLoad();
    void resize(); //increases size to keep load factor below threshold

public:
    AccountTable();
    ~AccountTable();
    void insert(accountEntry newValue);
    void display() const;
    bool isEmpty() const;
    string search(string key) const; //returns all info of value, if found
    void remove(string key);
    void updateInfo(string key, string newInfo);
    bool doesExist(string key) const; //if just needing to check if a value is present
    void getStatistics() const;
    void saveInfo(string filename) const;
    void buildTable(string filename);
    void refreshInfo(); //goes through each acct in array/linked lists and builds them again - where interest will get computed!
};

#include "AccountTable.cpp"
#endif