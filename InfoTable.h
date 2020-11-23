/*
*/
#ifndef INFOTABLE_H
#define INFOTABLE_H
#include <iostream>
#include <string>
#include <fstream>
#include "EncryptionBox.h"
#include "LinkedList.h"
using namespace std;

struct infoEntry
{
    string key;
    vector<string> info; //multiple things map to the same key

    infoEntry();
    infoEntry(string Key);
    infoEntry(string Key, vector<string> Info);
    bool operator == (const infoEntry &obj);
    bool operator != (const infoEntry &obj); //so that can be inserted and removed from linked list, compares based on key value
};

class InfoTable : public AccountTable
{
protected:
    void resize();
    
public:
    //no constructor or destructor needed since same as AccountTable
    void removeInfo(string key, string info); //if Key->{info1, info2}, then removing info1 makes the entry become Key->{info2}
    void insert(infoEntry newEntry);
    void display() const;
    void writeInfo() const;
};

#endif