#include <iostream>
#include "user.h"
#include <conio.h>
using namespace std;
user::user()
{
    username = "";
    password = "";
    balance = 1000000;
}

void user::createAccount()
{
    cout << "\nEnter Username: ";
    cin >> username;

    cout << "Enter Password: ";
    password = "";
    char ch;
    while((ch = _getch()) !=13)
    {
        if((int)ch == 8)
        {
            if(!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;

    cout << "\nAccount Created Successfully!\n";
    cout << "Starting Balance: " << balance << endl;
}