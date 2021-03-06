/*
Implementation file for the Dictionary object
*/
#include "AccountTable.h"

//this allows for 'cout << entry' and 'outFile << entry' to work correctly
ostream& operator << (ostream& os, const accountEntry obj)
{
    os << obj.key << ": " << obj.info;
    return os;
}

bool accountEntry::operator == (const accountEntry &obj)
{
    bool status = (key == obj.key) ? true : false;
    return status;
}

bool accountEntry::operator != (const accountEntry &obj)
{
    bool status = (key != obj.key) ? true : false;
    return status;
}

accountEntry::accountEntry()
{
    key = "";
    info = "";
}

accountEntry::accountEntry(string keyValue)
{
    key = keyValue;
    info = "blah";
}

accountEntry::accountEntry(string keyValue, string infoValue)
{
    key = keyValue;
    info = infoValue;
}

//Start of Dict Implementations

AccountTable::AccountTable()
{
    capacity = 8; //just some default value
    usedIndicies = 0;
    totalKeys = 0;
    loadFactor = 0.0;
    loadThreshold = .75; //arbitrary number
    maxCollisions = 0;
    dict = new LinkedList<accountEntry>[capacity];
}

AccountTable::~AccountTable()
{
    delete [] dict; //linked list has a valid destructor, so only need to delete the dict containing them
    dict = nullptr;
}

void AccountTable::updateLoad()
{
    double keys = totalKeys;
    loadFactor = keys / capacity; //cannot divide an int by an int and retain the decimals, so key is a substitute used
}

bool AccountTable::isEmpty() const
{
    bool status = (totalKeys == 0) ? true : false;
    return status;
}

void AccountTable::insert(accountEntry newValue)
{
    unsigned int index = EncryptionBox::getIndex(newValue.key, capacity); //hashing the entry's key % dict size to find where it belongs

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

    if (loadFactor > loadThreshold)
    {
        resize();
    }
}

string AccountTable::search(string key) const
{
    unsigned int index = EncryptionBox::getIndex(key, capacity);
    accountEntry value(key);
    bool didFind = dict[index].search(value);
    if (!didFind)
    {
        //cout << "The key: " << key << " is not present in the dictionary" << endl;
        return "false";
    }
    else
    {
        return value.info;
    }
}

void AccountTable::remove(string key)
{
    if (isEmpty())
    {
        cout << "There are no values to remove" << endl;
    }
    else
    {
        unsigned int index = EncryptionBox::getIndex(key, capacity); //hashing the entry's key % dict size to find where it belongs
        accountEntry toRemove(key);

        int currentSize = dict[index].size();
        dict[index].remove(toRemove);
        if (currentSize != dict[index].size())
        {
            totalKeys--;
        }
    }
}

void AccountTable::updateInfo(string key, string newInfo)
{
    unsigned int index = EncryptionBox::getIndex(key, capacity); //hashing the entry's key % dict size to find where it belongs
    accountEntry oldEntry(key), newEntry(key, newInfo);
    dict[index].remove(oldEntry);
    dict[index].append(newEntry);
}

bool AccountTable::doesExist(string key) const
{
    unsigned int index = EncryptionBox::getIndex(key, capacity);
    accountEntry value(key);
    bool isPresent = dict[index].search(value);
    return isPresent;
}

void AccountTable::resize()
{
    LinkedList<accountEntry> *tempDict = new LinkedList<accountEntry>[capacity * 2];
    unsigned int tempUsedIndicies = 0;
    int tempMaxCollisions = 0;

    for (int i = 0; i < capacity; i++) //for each index in array
    {
        while (!dict[i].isEmpty()) //if not empty, need to rehash each entry into tempDict
        {
            accountEntry value;
            dict[i].peekFirst(value); //value takes on first element in linked list's value
            dict[i].delFirst();
            
            unsigned int index = EncryptionBox::getIndex(value.key, capacity * 2); //finding where value needs to go in new dict
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

void AccountTable::display() const
{
    for (int i = 0; i < capacity; i++)
    {
        if (!dict[i].isEmpty())
        {
            cout << "Index: " << i << ". Size: " << dict[i].size() << endl;
            dict[i].displayList();
        }
    }
}

void AccountTable::getStatistics() const
{
    unsigned int avaliableKeys = (loadThreshold * capacity) - totalKeys;
    double indicies = usedIndicies;
    double usagePercent = (indicies / capacity) * 100;
    cout << endl;
    cout << "Total capacity: " << capacity << endl;
    cout << "Total keys stored: " << totalKeys << endl;
    cout << "Total indicies used: " << usedIndicies << endl;
    cout << "Load factor: " << loadFactor << endl;
    cout << "Load threshold: " << loadThreshold << endl;
    cout << "Avaliable entries before resizing: " << avaliableKeys << endl;
    cout << "Max Collisions in a single index: " << maxCollisions - 1 << endl; // -1 since an obj in an entry by itself counts as a collision initially
    cout << "Array index usage: " << usagePercent << "%" << endl;
    cout << "Hashing algorithm: FNL-1a 64-bit" << endl;
}

void AccountTable::saveInfo(string filename) const
{
    EncryptionBox::positionInFile = 0;
    ofstream outFile(filename);
    accountEntry storedValue;
    if (outFile)
    {
        for (int i = 0; i < capacity; i++)
        {
            while (!dict[i].isEmpty())
            {
                dict[i].peekFirst(storedValue);
                dict[i].delFirst();
                string toWrite = storedValue.key + "%#" + storedValue.info;
                outFile << EncryptionBox::encrypt(toWrite) << endl;
            }
        }
    }
    else
    {
        cerr << "The directory needing to be accessed does not exist:" << endl << filename << endl;
        abort();
    }
    outFile.close();
}

void AccountTable::buildTable(string filename)
{
    EncryptionBox::positionInFile = 0;
    ifstream inFile;
    inFile.open(filename);
    if (inFile)
    {
        string text = "";
        while (getline(inFile, text))
        {
            text = text.substr(0, text.rfind("\r")); //removing carriage return from .txt file
            text = EncryptionBox::decrypt(text); //decrypting in order to read from
            string delim = "%#";
            string key = text.substr(0, text.find(delim));
            text = text.substr(text.find(delim) + delim.length(), string::npos);
            accountEntry entry(key, text);
            insert(entry);
        }
    }
    else
    {
        cout << "The file could not be found at:" << endl << filename << endl;
    }
    inFile.close();
}

void AccountTable::refreshInfo()
{
    for (int i = 0; i < capacity; i++)
    {
        if (!dict[i].isEmpty())
        {
            const int size = dict[i].size(); //so that it doesnt change in the for loop
            LinkedList<accountEntry> newList;

            for (int k = 0; k < size; k++) //ensure loop works properly!
            {
                accountEntry oldAcctEntry;
                dict[i].peekFirst(oldAcctEntry); //takes on value
                dict[i].delFirst();

                //adding interest onto the account
                Account acctObj(oldAcctEntry.key); //automatically computes interest
                string acctInfo = acctObj.getAccountTableInfo(); //reset info, changed with interest
                accountEntry newAcctEntry(oldAcctEntry.key, acctInfo);
                newList.append(newAcctEntry);
            }
            
            dict[i] = newList; //will deep copy new values over
        }
    }
}
