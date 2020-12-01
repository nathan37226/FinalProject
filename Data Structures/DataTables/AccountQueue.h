/*
Nathan Obert CSC 232
*/
#ifndef ACCOUNTQUEUE_H
#define ACCOUNTQUEUE_H
#include <iostream>
#include "LinkedList.h"
#include "EncryptionBox.h"
#include <fstream>
using namespace std;

class AccountQueue
{
private:
    LinkedList<string> list; //will be destoryed with implicit destructor just fine since ~LinkedList is defined, which means no destructor needs to be delcared
public:
    AccountQueue(int capacity = -1);

    bool peekFirst(string &item) const;
    void enqueue(string item);
    bool dequeue(string &item);
    void clear();
    bool isEmpty() const;
    bool isFull() const;
    void displayQueue() const;
    void saveQueue() const;
    void buildQueue();
};

AccountQueue::AccountQueue(int capacity)
{
    capacity = (capacity < 1) ? -1 : capacity;
    list = LinkedList<string>(capacity);
}

bool AccountQueue::peekFirst(string &item) const
{
    if (!isEmpty())
    {
        list.peekFirst(item); //item takes head node's value
        return true;
    }
    return false;
}

void AccountQueue::enqueue(string item)
{
    list.append(item); //append sends item to back of list
}

bool AccountQueue::dequeue(string &item)
{
    if (!isEmpty())
    {
        list.peekFirst(item); //item takes head's value
        list.delFirst(); //deleteing head node
        return true;
    }
    return false;
}

void AccountQueue::clear()
{
    list.clear();
}

bool AccountQueue::isEmpty() const
{
    return list.isEmpty();
}

bool AccountQueue::isFull() const
{
    return list.isFull();
}

void AccountQueue::displayQueue() const
{
    list.displayList();
}

void AccountQueue::saveQueue() const
{
    EncryptionBox::positionInFile = 0;
    LinkedList<string> deepCopyList = list;
    string entry = "";
    ofstream outFile("AccountData/RequestedAccounts.txt"); //where to write info

    if (outFile)
    {
        while (!deepCopyList.isEmpty())
        {
            deepCopyList.peekFirst(entry);
            deepCopyList.delFirst();
            outFile << EncryptionBox::encrypt(entry) << endl;
        }
    }
    else
    {
        cout << "Could not save to file at: \nAccountData/RequestedAccounts.txt" << endl;
    }
    outFile.close();
}

void AccountQueue::buildQueue()
{
    list.clear();
    string entry = "";
    ifstream inFile;
    inFile.open("AccountData/RequestedAccounts.txt");
    EncryptionBox::positionInFile = 0;

    if (inFile)
    {
        while (getline(inFile, entry))
        {
            entry = entry.substr(0, entry.rfind("\r")); //removing carriage return
            entry = EncryptionBox::decrypt(entry); //making ledgible
            list.append(entry); //puts to end of list, thus preserving queue order
        }
    }
    else
    {
        //cout << "Could not save to file at: \nAccountData/RequestedAccounts.txt" << endl;
    }
}

#endif