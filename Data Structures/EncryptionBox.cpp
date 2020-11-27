/*
Implementation of the EncryptionBox object
*/

string EncryptionBox::encrypt(string text) 
{
    string encryptedText = "";

    for (int i = 0; i < text.length(); i++)
    {
        int asciiValue = text[i] + offsetKey[i % offsetKey.size()];
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
        int asciiValue = text[i] - offsetKey[i % offsetKey.size()];
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
