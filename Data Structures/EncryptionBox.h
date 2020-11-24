/*
This header contains the declaration for the EncryptionBox object.
This object will never truly need to be initialized as all members are static.
This object's purpose is to encrypt/decrypt and hash user provided info.
As a part of hashing, this box will also convert decimals to hexadecimals.

The encryption scheme revolves around an xor cipher. This was chose because 
it makes plain text indecipherable as well as being fast to perform.
The hashing algorithm is FNV-1a, 64-bit. This algorithm was chosen due to
it being fast as well as relatively unique. When the hash is put
into a table, there will be some collisions, but never a lot
in one spot. So, using seperate chaining inside an array makes this algorithm
work well.
As a side note, I used this hashing algorithm on a text file of 159,288 unique
fake user names. At most 9 mapped to the same index, using a table size of 125%,
with a load of 0.551.
Since iterating through so few items is cheap (much better than a BST), I settled on this
algorithm.

Xor cipher, performed on each character in the text:
    text xor key = encrpyted text
    encrypted text xor key = text
*/
#ifndef ENCRYPTIONBOX_H
#define ENCRYPTIONBOX_H
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

class EncryptionBox
{
private:
    static string salt; //used to further secure info in hashing
    static string key; //used for encryption and decryption

protected:
    static unsigned long long FNV1a(string input); //outputs hash value

public:
    EncryptionBox(){}

    static string encrypt(string text);
    static string decrypt(string encryptedText);
    static string hash(string input); //will output hexadecimal string
    static string toHex(unsigned long long decInput);
    static unsigned int getIndex(string input, unsigned int tableSize);
};

string EncryptionBox::salt = "8%a4/.B&cT"; //initalizing salt since static
string EncryptionBox::key = "-.4#mV&Qmy{B~KY"; //just some random chars

#include "EncryptionBox.cpp"
#endif
