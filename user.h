#ifndef USER_H
#define USER_H

#include<string>
using namespace std;
//The Blueprint For User Class
class user
{
    public:
        //User Details
        string username;
        string password;
        double balance;
    //Default Constructor
        user();

        void createAccount();
};
#endif