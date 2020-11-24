/*
This is a header file for a dictionary ADT, named AccountTable. This dictionary will be an array-based 
implementation. Each index of the array will implement seperate chainging through a linked list. A given 
key will hash to a specific index, or really linked list, that contains both the key and info related to 
the key. Further, all keys will be strings in this dictionary, and all hashing will be done by the 
EncryptionBox.
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
    accountEntry(string Key);
    accountEntry(string Key, string Info);
    bool operator == (const accountEntry &obj);
    bool operator != (const accountEntry &obj); //so that can be inserted and removed from linked list, compares based on key value
};

class AccountTable
{
protected:
    LinkedList<accountEntry> *dict;
    EncryptionBox box; //performs all hashing and encrypting

    //Used for statistics of list and hashing
    unsigned int capacity;
    unsigned int usedIndicies;
    unsigned int totalKeys;
    double loadFactor; //will be determined based off of totalKeys / capacity
    float loadThreshhold;
    int maxCollisions;

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
    void getStatistics() const;
    void saveInfo(string filename) const;
    void buildTable(string filename);
    
};

#include "AccountTable.cpp"
#endif