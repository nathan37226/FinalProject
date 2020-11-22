/*
This is a header file for a dictionary ADT. This dictionary will be an array-based implementation.
Each index of the array will implement seperate chainging through a linked list. A given key will
hash to a specific index, or really linked list, that contains both the key and info related to the key.
Further, all keys will be strings in this dictionary, and all hashing will be done by the EncryptionBox
*/
#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <iostream>
#include <string>
#include "EncryptionBox.h"
#include "LinkedList.h"
using namespace std;

struct entry
{
    string key;
    string info; //this is where info associated with the key would be stored

    entry();
    entry(string Key);
    entry(string Key, string Info);
    bool operator == (const entry &obj);
    bool operator != (const entry &obj); //so that can be inserted and removed from linked list, compares based on key value
};

class Dictionary
{
private:
    LinkedList<entry> *dict;
    EncryptionBox box; //performs all hashing and encrypting

    //Used for statistics of list and hashing
    unsigned int capacity;
    unsigned int usedIndicies;
    unsigned int totalKeys;
    double loadFactor; //will be determined based off of totalKeys / capacity
    float loadThreshhold;
    int maxCollisions;

protected:
    void updateLoad();
    void resize(); //increases size to keep load factor below threshold

public:
    Dictionary();
    ~Dictionary();
    void insert(entry newValue);
    void display() const;
    bool isEmpty() const;
    string search(string key) const; //returns info of value, if found
    void remove(string key);
    void getStatistics() const;
};

#include "Dictionary.cpp"
#endif