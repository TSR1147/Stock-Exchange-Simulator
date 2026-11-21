#include<iostream>
#include "user.h"
#include "stock.h"
#include "portfolio.h"
#include "transaction.h"
#include <limits>
#include <iomanip>

using namespace std;

// Menu For Logged-In Users.

void userMenu(user& loggedInUser)
{
    int choice = 0;

    stockMarket market;

    portfolio userPortfolio;

    transaction trade;

    // Load The Logged-In User's Portfolio.
    userPortfolio.loadPortfolio(loggedInUser.username);

    do{
        cout << "\n========================================\n";
        cout << "Welcome, " << loggedInUser.username << endl;
        cout << "Available Balance: Rs. "
     << fixed << setprecision(2)
     << loggedInUser.balance << endl;
        cout << "Stock Exchange User Menu\n";
        cout << "1. View Stock Market\n";
        cout << "2. Search Stock\n";
        cout << "3. Buy Stocks\n";
        cout << "4. Sell Stocks\n";
        cout << "5. View Portfolio\n";
        cout << "6. Logout\n";
        cout << "Enter your choice: ";

        if(!(cin >> choice))
        {
            if(cin.eof())
            {
                cout << "\nInput Ended. Returning To Main Menu.\n";
                return;
            }

            cin.clear();

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nInvalid Input. Please Enter A Number.\n";

            choice = 0;

            continue;
        }

        switch(choice)
        {
            case 1:
            {
                if(market.loadStocks())
                {
                    market.displayStocks();
                }

                break;
            }

            case 2:
            {
                if(market.loadStocks())
                {
                    market.searchStock();
                }

                break;
            }

            case 3:
            {
                if(market.loadStocks())
                {
                    trade.buyStock(loggedInUser, market, userPortfolio);
                }

                break;
            }

            case 4:
            {
                if(market.loadStocks())
                {
                    trade.sellStock(loggedInUser, market, userPortfolio);
                }

                break;
            }

            case 5:
            {
                if(market.loadStocks())
                {
                    userPortfolio.displayPortfolio(market);
                }

                break;
            }

            case 6:
                cout << "\nLogged Out Successfully.\n";
                break;

            default:
                cout << "\nInvalid Choice. Please Try Again.\n";
        }
    }
    while(choice != 6);
}


int main()
{
    int choice;

    do{
        cout << "\n========================================\n";
        cout << "Stock Exchange Simulator\n";
        cout << "1. Create Account\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
            {
                user newUser;
                newUser.createAccount();
                break;
            }

            case 2:
            {
                user existingUser;

                if(existingUser.login())
                {
                    userMenu(existingUser);
                }
                else
                {
                    cout << "\nLogin Failed. Please Try Again.\n";
                }

                break;
            }

            case 3:
                cout << "\nThank You For Using Stock Exchange Simulator.\n";
                break;

            default:
                cout << "\nInvalid Choice. Please Try Again.\n";
        }
    }
    while(choice != 3);

    return 0;
}