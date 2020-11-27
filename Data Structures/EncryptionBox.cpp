/*
Implementation of the EncryptionBox object

The encryption algorithm is to find the ascii value of a char,
add 133 to it, then offset it by a certain amount, 0 thru 4.
The 133 comes from the ascii table: the valid characters expected
to be encrypted range from 32 to 122. The range of 161 to 255, which
text files support (I checked by printing to one), are valid characters.

32 to 122 - range of 90
161 to 255 - range of 90 + up to 4 from offset vector
At minimum, " " will map to "¡"
At maximum, "z" will map to "ÿ".
*/

string EncryptionBox::encrypt(string text) 
{
    string encryptedText = "";

    for (int i = 0; i < text.length(); i++)
    {
        int asciiValue = text[i] + 129 + offsetKey[positionInFile % offsetKey.size()];
        positionInFile++;
        char newChar = asciiValue;
        encryptedText += newChar;
    }
    return encryptedText;
}

string EncryptionBox::decrypt(string text) 
{
    string decryptedText = "";

    for (int i = 0; i < text.length(); i++)
    {
        int asciiValue = text[i] - 129 - offsetKey[positionInFile % offsetKey.size()];
        positionInFile++;
        char newChar = asciiValue;
        decryptedText += newChar;
    }
    return decryptedText;
}

string EncryptionBox::hash(string input) 
{
    unsigned long long hashedInput = FNV1a(input);
    string hexHashedInput = toHex(hashedInput);
    return hexHashedInput;
}

string EncryptionBox::toHex(unsigned long long decInput) 
{
    stringstream streamObj;
    streamObj << hex << decInput; //storing the hash int converted into hex inside the stream
    string hexString = streamObj.str(); //copying hex value out of stream into an actual string
    return hexString;
}

unsigned int EncryptionBox::getIndex(string input, unsigned int tableSize) 
{
    unsigned long long hashedInput = FNV1a(input);
    return (hashedInput % tableSize); //gives where in the table the hash is stored
}

unsigned long long EncryptionBox::FNV1a(string input) 
{
    input = input + salt; //input gains a suffix of "8%a4/.B&cT", a way of better securing what is being hashed

    unsigned long long FNVoffsetBasis = 14695981039346656037u; //from psuedocode for algormithm, a 64 bit number, 'u' suffix means unsigned to the complier so no warning
    unsigned long long FNVprime = 1099511628211u; //also from psuedocode

    //performing algorithm
    unsigned long long hash = FNVoffsetBasis;
    for (int i = 0; i < input.length(); i++)
    {
        hash = hash ^ int( input[i] ); //^ is xor bitwise operator, int( char ) returns ascii value
        hash *= FNVprime;
    }

    return hash;    
}
