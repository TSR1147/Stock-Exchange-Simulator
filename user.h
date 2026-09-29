#ifndef USER_H
#define USER_H

#include<string>
using namespace std;
//The Blueprint For User Class.
class user
{
    public:
        //User Details.
        string username;
        string password;
        double balance;
    //Default Constructor.
        user();

        // Create a new account.
        void createAccount();
        // Save user details into a file.
        void saveToFile();
        // Check whether username already exists.
        bool userExists(string uname);
        // Login to an existing account.
        bool login();
};
#endif