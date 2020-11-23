/*
*/
#include "InfoTable.h"

//this allows for 'cout << entry' and 'outFile << entry' to work correctly
ostream& operator << (ostream& os, const infoEntry obj)
{
    os << obj.key;
    return os;
}

bool infoEntry::operator == (const infoEntry &obj)
{
    bool status = (key == obj.key) ? true : false;
    return status;
}

bool infoEntry::operator != (const infoEntry &obj)
{
    bool status = (key != obj.key) ? true : false;
    return status;
}

infoEntry::infoEntry()
{
    key = "";
    info = "";
}

infoEntry::infoEntry(string Key)
{
    key = Key;
    info = {};
}

infoEntry::infoEntry(string Key, vector<string> Info)
{
    key = Key;
    info = Info;
}

void InfoTable::resize()
{
    LinkedList<infoEntry> *tempDict = new LinkedList<infoEntry>[capacity * 2];
    unsigned int tempUsedIndicies = 0;
    int tempMaxCollisions = 0;

    for (int i = 0; i < capacity; i++) //for index in dictionary array
    {
        while (!dict[i].isEmpty()) //if index is not empty, then need to rehash each entry into tempDict
        {
            infoEntry value;
            dict[i].peekFirst(value); //value takes on first element in linked list's value
            dict[i].delFirst();
            
            unsigned int index = box.getIndex(value.key, capacity * 2); //finding where value needs to go in new dict

            if (tempDict[index].isEmpty())
            {
                tempUsedIndicies++;
            }
            tempDict[index].append(value); //adding value to new dict
            if (tempDict[index].size() > tempMaxCollisions)
            {
                tempMaxCollisions = tempDict[index].size();
            }
        }
    }

    //getting everything up to date
    capacity *= 2;
    delete [] dict;
    dict = tempDict; //reassigning the protected member
    tempDict = nullptr;
    usedIndicies = tempUsedIndicies;
    maxCollisions = tempMaxCollisions;
    updateLoad();
}

void InfoTable::insert(infoEntry newEntry)
{
    unsigned int index = box.getIndex(newValue.key, capacity); //hashing the entry's key % dict size to find where it belongs

    if (dict[index].isEmpty())
    {
        usedIndicies++;
    }

    dict[index].append(newValue); //adding value to that index's linked list
    totalKeys++;
    updateLoad();

    if (dict[index].size() > maxCollisions)
    {
        maxCollisions = dict[index].size();
    }

    if (loadFactor > loadThreshhold)
    {
        resize();
    }
}

void InfoTable::display() const
{
    ;
}