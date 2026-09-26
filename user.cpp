#include <iostream>
#include "user.h"
#include <conio.h>
#include <fstream>
using namespace std;
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