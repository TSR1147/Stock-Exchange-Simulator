#include <iostream>
#include "user.h"
#include "password_hash.h"
#include <conio.h>
#include <fstream>
#include <cctype>
#include <sstream>

using namespace std;

string toLowerCase(string str)
{
    for(int i = 0; i < str.length(); i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }

    return str;
}

// Convert Salt And Password Hash Into Hexadecimal Format.
string bytesToHex(unsigned char data[], int size)
{
    string hex = "";
    char digits[] = "0123456789ABCDEF";

    for(int i = 0; i < size; i++)
    {
        hex += digits[(data[i] >> 4) & 0x0F];
        hex += digits[data[i] & 0x0F];
    }

    return hex;
}

// Convert Hexadecimal Format Back Into Bytes.
int hexValue(char ch)
{
    if(ch >= '0' && ch <= '9')
        return ch - '0';

    if(ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;

    if(ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;

    return -1;
}

bool hexToBytes(string hex, unsigned char data[], int size)
{
    if(hex.length() != size * 2)
        return false;

    for(int i = 0; i < size; i++)
    {
        int first = hexValue(hex[i * 2]);
        int second = hexValue(hex[i * 2 + 1]);

        if(first == -1 || second == -1)
            return false;

        data[i] = (first << 4) | second;
    }

    return true;
}

user::user()
{
    username = "";
    password = "";
    balance = 1000000;
}

//Function To Create A New ACcount.
void user::createAccount()
{
    cout << "\nEnter Username: ";
    cin >> username;

    username = toLowerCase(username); // Convert username to lowercase for consistency

    if(userExists(username))
    {
        cout << "\nUsername Already Exists.\n";
        cout << "Please Try Again With A Different Username.\n";
        return;
    }

    cout << "Enter Password: ";

    //Clear the input buffer before reading password.
    password = "";
    char ch;
    while((ch = _getch()) !=13)
    {
        if((int)ch == 8)
        {

            //Remove The LAst Character From Password If Backspace Is Pressed.
            if(!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            //Adding The Character To Password And Displaying Asterisk.
            password += ch;
            cout << '*';
        }
    }
    cout << endl;

    if(password.empty())
    {
        cout << "\nPassword Cannot Be Empty.\n";
        return;
    }

    if(password.length() > 128)
    {
        cout << "\nPassword Cannot Exceed 128 Characters.\n";
        return;
    }

    //Save User Details INto File.
    if(saveToFile())
    {
        cout << "\nAccount Created Successfully!\n";
        cout << "Starting Balance: " << balance << endl;
        cout << "User Details Saved Succesfully!.\n";
    }
    else
    {
        cout << "\nAccount Creation Failed!\n";
    }
}


bool user::saveToFile()
{
    // Generate salt and password hash.
    unsigned char salt[SALT_SIZE];
    unsigned char passwordHash[HASH_SIZE];

    if(!generateSalt(salt, SALT_SIZE))
        return false;

    if(!hashPassword(password, salt, SALT_SIZE, passwordHash, HASH_SIZE))
        return false;

    // Convert salt and hash into hexadecimal format.
    string saltHex = bytesToHex(salt, SALT_SIZE);
    string hashHex = bytesToHex(passwordHash, HASH_SIZE);

    // Check whether the existing file ends with a newline.
    bool needsNewLine = false;

    ifstream checkFile("users.txt", ios::binary);

    if(checkFile)
    {
        checkFile.seekg(0, ios::end);

        streamoff fileSize = checkFile.tellg();

        if(fileSize > 0)
        {
            checkFile.seekg(-1, ios::end);

            char lastChar;
            checkFile.get(lastChar);

            if(lastChar != '\n')
                needsNewLine = true;
        }

        checkFile.close();
    }

    // Open users.txt in append mode.
    ofstream file("users.txt", ios::app);

    if(!file)
        return false;

    // Add a newline if the previous record has no newline.
    if(needsNewLine)
        file << endl;

    // Save username, password hashing method, salt, hash, and balance.
    file << username << " PBKDF2 " << saltHex << " "
         << hashHex << " " << balance << endl;

    if(!file.good())
    {
        file.close();
        return false;
    }

    file.close();

    return true;
}

// Checking whether username already exists in users.txt
bool user::userExists(string uname)
{
    ifstream file("users.txt");

    string line;
    string fileUsername;

    //Reading One USer Record At A Time
    while(getline(file, line))
    {
        istringstream lineStream(line);

        if(!(lineStream >> fileUsername))
            continue;

        //If Username Matches, Return True
        if(toLowerCase(fileUsername) == toLowerCase(uname))
        {
            file.close();
            return true;
        }
    }

    file.close();

    //Username Not Found
    return false;
}

//Login Into An Existing Account
bool user::login()
{
    ifstream file("users.txt");

    if(!file)
    {
        cout << "\nUnable To Open users.txt!\n";
        return false;
    }

    string loginUsername;
    string loginPassword;

    string line;
    string fileUsername;
    string secondField;

    cout << "\nEnter Username: ";
    cin >> loginUsername;

    //convert username to lowercase for consistency
    loginUsername = toLowerCase(loginUsername);

    cout << "Enter Password: ";

    char ch;
    loginPassword = "";

    while((ch = _getch()) != 13)
    {
        if((int)ch == 8)
        {
            //Remove The Last Character From Password If Backspace Is Pressed.
            if(!loginPassword.empty())
            {
                loginPassword.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            //Adding The Character To Password And Displaying Asterisk.
            loginPassword += ch;
            cout << '*';
        }
    }

    cout << endl;

    //Reading One User Record At A Time.
    while(getline(file, line))
    {
        istringstream lineStream(line);

        if(!(lineStream >> fileUsername >> secondField))
            continue;

        if(toLowerCase(fileUsername) != loginUsername)
            continue;

        double fileBalance;

        //Check Whether The Account Uses Password Hashing.
        if(secondField == "PBKDF2")
        {
            string saltHex;
            string hashHex;

            if(!(lineStream >> saltHex >> hashHex >> fileBalance))
                continue;

            unsigned char salt[SALT_SIZE];
            unsigned char storedHash[HASH_SIZE];

            //Convert Stored Salt And Hash Back Into Bytes.
            if(!hexToBytes(saltHex, salt, SALT_SIZE))
                continue;

            if(!hexToBytes(hashHex, storedHash, HASH_SIZE))
                continue;

            //Verify The Entered Password.
            if(verifyPassword(loginPassword, salt, SALT_SIZE,
                              storedHash, HASH_SIZE))
            {
                balance = fileBalance;

                cout << "\nLogin Successful!\n";
                cout << "Welcome, " << loginUsername << "!\n";

                file.close();
                return true;
            }
        }
        else
        {
            //Support Accounts Created Using The Old Plaintext Format.
            string filePassword = secondField;

            if(!(lineStream >> fileBalance))
                continue;

            if(filePassword == loginPassword)
            {
                balance = fileBalance;

                cout << "\nLogin Successful!\n";
                cout << "Welcome, " << loginUsername << "!\n";

                file.close();
                return true;
            }
        }
    }

    file.close();

    cout << "\nInvalid Username or Password. Please Try Again.\n";
    return false;
}
