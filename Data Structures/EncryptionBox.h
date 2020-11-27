/*
This header contains the declaration for the EncryptionBox object.
This object will never truly need to be initialized as all members are static.
This object's purpose is to encrypt/decrypt and hash user provided info.
As a part of hashing, this box will also convert decimals to hexadecimals.

The encryption scheme revolves around offsetting. This was chosen due to
ciphers like xor sometimes generating newline characters, which hinders
saving to a text file. This cipher is guarenteed to never generate any unwanted
characters, which ensures saving to and reading from a textfile will not have
any issues.

The hashing algorithm is FNV-1a, 64-bit. This algorithm was chosen due to
it being fast as well as relatively unique. When the hash is put
into a table, there will be some collisions, but never a lot
in one spot. So, using seperate chaining inside an array makes this algorithm
work well.
*/
#ifndef ENCRYPTIONBOX_H
#define ENCRYPTIONBOX_H
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
using namespace std;

class EncryptionBox
{
private:
    static string salt; //used to further secure info in hashing
    static vector<int> offsetKey; //used for encryption and decryption

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
vector<int> EncryptionBox::offsetKey = {1,3,1,0,4,2,3,1,4,2,0}; //random numbers to offset the entered text's ascii value
//these are between 0 and 4 because that ensures a character wont map to any ascii values that won't display in a text file, like DEL or some of the seperators

#include "EncryptionBox.cpp"
#endif
