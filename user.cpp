#include <iostream>
#include "user.h"
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
    cin >> password;

    cout << "\nAccount Created Successfully!\n";
    cout << "Starting Balance: " << balance << endl;
}