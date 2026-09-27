#include <iostream>
#include "user.h"
#include <conio.h>
#include <fstream>
#include <cctype>
using namespace std;

string toLowerCase(string str)
{
    for(int i = 0; i < str.length(); i++)
    {
        str[i] = tolower(str[i]);
    }

    return str;
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

    cout << "\nAccount Created Successfully!\n";
    cout << "Starting Balance: " << balance << endl;

    //Save User Details INto File.
            saveToFile();
}

void user::saveToFile()
{
    // Open users.txt in append mode
    // New users will be added at the end of the file
    //append mode is used to ensure that existing user data is not overwritten
    ofstream file("users.txt", ios::app);

    //Save Username, Password, and Balance to the file
    file << username << " " << password << " " << balance << endl;

    file.close();

    cout << "User Details Saved Succesfully!.\n";
}

// Checking whether username already exists in users.txt
bool user::userExists(string uname)
{
    ifstream file("users.txt");
    
    string fileUsername;
    string filePassword;
    double fileBalance;

    //Reading One USer Record At A Time
    while(file >> fileUsername >> filePassword >> fileBalance)
    {
        //If Username Matches, Return True
        if(toLowerCase(fileUsername) == toLowerCase(uname))
        {
            return true;
        }
    }

    file.close();

    //Username Not Found
    return false;
}