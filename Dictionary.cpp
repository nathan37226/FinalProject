/*
Implementation file for the Dictionary object
*/
#include "Dictionary.h"

//this allows for 'cout << entry' and 'outFile << entry' to work correctly
ostream& operator << (ostream& os, const entry obj)
{
    os << obj.key;
    return os;
}

bool entry::operator == (const entry &obj)
{
    bool status = (key == obj.key) ? true : false;
    return status;
}

bool entry::operator != (const entry &obj)
{
    bool status = (key != obj.key) ? true : false;
    return status;
}

entry::entry()
{
    key = "";
    info = "";
}

entry::entry(string Key)
{
    key = Key;
    info = "blah";
}

entry::entry(string Key, string Info)
{
    key = Key;
    info = Info;
}

//Start of Dict Implementations

Dictionary::Dictionary()
{
    capacity = 8; //just some default value
    usedIndicies = 0;
    totalKeys = 0;
    loadFactor = 0.0;
    loadThreshhold = .75; //arbitrary number
    maxCollisions = 0;
    dict = new LinkedList<entry>[capacity];
}

Dictionary::~Dictionary()
{
    delete [] dict; //linked list has a valid destructor, so only need to delete the dict containing them
}

void Dictionary::updateLoad()
{
    double keys = totalKeys;
    loadFactor = keys / capacity; //cannot divide an int by an int and retain the decimals, so key is a substitute used
}

bool Dictionary::isEmpty() const
{
    bool status = (totalKeys == 0) ? true : false;
    return status;
}

void Dictionary::insert(entry newValue)
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

string Dictionary::search(string key) const
{
    unsigned int index = box.getIndex(key, capacity);
    entry value(key);
    bool didFind = dict[index].search(value);
    if (!didFind)
    {
        cout << "The key: " << key << " is not present in the dictionary" << endl;
        return "false";
    }
    else
    {
        return value.info;
    }
}

void Dictionary::remove(string key)
{
    if (isEmpty())
    {
        cout << "There are no values to remove" << endl;
    }
    else
    {
        unsigned int index = box.getIndex(key, capacity); //hashing the entry's key % dict size to find where it belongs
        entry toRemove(key);

        int currentSize = dict[index].size();
        dict[index].remove(toRemove);
        if (currentSize != dict[index].size())
        {
            totalKeys--;
        }
    }
}

void Dictionary::resize()
{
    LinkedList<entry> *tempDict = new LinkedList<entry>[capacity * 2];
    unsigned int tempUsedIndicies = 0;
    int tempMaxCollisions = 0;

    for (int i = 0; i < capacity; i++)
    {
        while (!dict[i].isEmpty())
        {
            entry value;
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
    dict = tempDict;
    tempDict = nullptr;
    usedIndicies = tempUsedIndicies;
    maxCollisions = tempMaxCollisions;
    updateLoad();
}

void Dictionary::display() const
{
    for (int i = 0; i < capacity; i++)
    {
        if (!dict[i].isEmpty())
        {
            cout << "Index: " << i << " Size: " << dict[i].size() << endl;
            dict[i].displayList();
        }
    }
}

void Dictionary::getStatistics() const
{
    unsigned int avaliableKeys = (loadThreshhold * capacity) - totalKeys;
    double indicies = usedIndicies;
    double usagePercent = (indicies / capacity) * 100;
    cout << endl;
    cout << "Total capacity: " << capacity << endl;
    cout << "Total keys stored: " << totalKeys << endl;
    cout << "Total indicies used: " << usedIndicies << endl;
    cout << "Load factor: " << loadFactor << endl;
    cout << "Load threshold: " << loadThreshhold << endl;
    cout << "Avaliable entries before resizing: " << avaliableKeys << endl;
    cout << "Max Collisions in a single index: " << maxCollisions << endl;
    cout << "Array index usage: " << usagePercent << "%" << endl;
    cout << "Hashing algorithm: FNL-1a 64-bit" << endl;
}

void Dictionary::writeInfo(string filename) const
{
    ofstream outFile(filename);
    entry storedValue;
    if (outFile)
    {
        for (int i = 0; i < capacity; i++)
        {
            while (!dict[i].isEmpty())
            {
                dict[i].peekFirst(storedValue);
                dict[i].delFirst();
                outFile << storedValue.key << "---" << storedValue.info << endl;
            }
        }
    }
    else
    {
        cout << "The directory needing to be accessed does not exist:" << endl << filename << endl;
    }
    outFile.close();
}