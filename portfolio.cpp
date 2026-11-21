#include "portfolio.h"
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <windows.h>

using namespace std;

// Default Constructor.
holding::holding()
{
    symbol = "";
    companyName = "";
    quantity = 0;
    purchasePrice = 0.0;
}

// Parameterized Constructor.
holding::holding(string sym, string name, int qty, double price)
{
    symbol = sym;
    companyName = name;
    quantity = qty;
    purchasePrice = price;
}


// Load User Portfolio From File.
bool portfolio::loadPortfolio(string username)
{
    ifstream file("portfolios.txt");

    holdings.clear();

    // If The File Does Not Exist, Start With An Empty Portfolio.
    if(!file)
    {
        return true;
    }

    string storedUsername;
    string symbol;
    string companyName;
    int quantity;
    double purchasePrice;

    // Read Portfolio Records One By One.
    while(file >> storedUsername >> symbol >> companyName >> quantity >> purchasePrice)
    {
        if(storedUsername == username)
        {
            holding h(symbol, companyName, quantity, purchasePrice);

            holdings.push_back(h);
        }
    }

    file.close();

    return true;
}


// Save User Portfolio To File.
bool portfolio::savePortfolio(string username)
{
    ifstream inputFile("portfolios.txt");
    ofstream tempFile("portfolios_temp.txt");

    if(!tempFile)
    {
        cout << "\nUnable To Save Portfolio.\n";
        return false;
    }

    string storedUsername;
    string symbol;
    string companyName;
    int quantity;
    double purchasePrice;

    // Preserve Other Users' Portfolio Records.
    while(inputFile >> storedUsername >> symbol >> companyName >> quantity >> purchasePrice)
    {
        if(storedUsername != username)
        {
            tempFile << storedUsername << " "
                     << symbol << " "
                     << companyName << " "
                     << quantity << " "
                     << purchasePrice << endl;
        }
    }

    inputFile.close();

    // Save The Current User's Portfolio.
    for(int i = 0; i < holdings.size(); i++)
    {
        tempFile << username << " "
                 << holdings[i].symbol << " "
                 << holdings[i].companyName << " "
                 << holdings[i].quantity << " "
                 << fixed << setprecision(2)
                 << holdings[i].purchasePrice << endl;
    }

    tempFile.close();

    if(!tempFile.good())
    {
        remove("portfolios_temp.txt");
        cout << "\nUnable To Write Portfolio Data.\n";
        return false;
    }

    // Replace The Old File Only After The New File Is Ready.
    if(!MoveFileExA("portfolios_temp.txt", "portfolios.txt",
                    MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
    {
        cout << "\nUnable To Update Portfolio File.\n";
        return false;
    }

    return true;
}


// Add A Stock To The Portfolio.
void portfolio::addHolding(string symbol, string companyName, int quantity, double purchasePrice)
{
    // Check Whether The Stock Already Exists In The Portfolio.
    for(int i = 0; i < holdings.size(); i++)
    {
        if(holdings[i].symbol == symbol)
        {
            double totalCost = (holdings[i].quantity * holdings[i].purchasePrice)
                               + (quantity * purchasePrice);

            holdings[i].quantity += quantity;

            // Calculate The New Average Purchase Price.
            holdings[i].purchasePrice = totalCost / holdings[i].quantity;

            return;
        }
    }

    // Add A New Stock If It Does Not Already Exist.
    holding h(symbol, companyName, quantity, purchasePrice);

    holdings.push_back(h);
}


// Display User Portfolio.
void portfolio::displayPortfolio(stockMarket& market)
{
    if(holdings.empty())
    {
        cout << "\nYour Portfolio Is Empty.\n";
        return;
    }

    double totalInvestment = 0.0;
    double totalCurrentValue = 0.0;

    cout << "\n================ YOUR PORTFOLIO ================\n";

    cout << left << setw(12) << "Symbol"
         << setw(18) << "Company"
         << right << setw(10) << "Quantity"
         << setw(15) << "Buy Price"
         << setw(15) << "Market Price" << endl;

    cout << "--------------------------------------------------------------------------\n";

    for(int i = 0; i < holdings.size(); i++)
    {
        double investment = holdings[i].quantity * holdings[i].purchasePrice;

        totalInvestment += investment;

        int index = market.findStockIndex(holdings[i].symbol);

        cout << left << setw(12) << holdings[i].symbol
             << setw(18) << holdings[i].companyName
             << right << setw(10) << holdings[i].quantity
             << setw(15) << fixed << setprecision(2)
             << holdings[i].purchasePrice;

        if(index != -1)
        {
            double currentPrice = market.stocks[index].price;

            double currentValue = holdings[i].quantity * currentPrice;

            totalCurrentValue += currentValue;

            cout << setw(15) << currentPrice << endl;
        }
        else
        {
            cout << setw(15) << "N/A" << endl;

            cout << "Stock " << holdings[i].symbol
                 << " is not currently available in the market.\n";
        }
    }

    cout << "\nTotal Investment: Rs. "
         << fixed << setprecision(2) << totalInvestment << endl;

    cout << "Current Market Value (Available Stocks): Rs. "
         << totalCurrentValue << endl;

    cout << "Profit / Loss (Available Stocks): Rs. "
         << totalCurrentValue - totalInvestment << endl;
}


// Remove Shares From An Existing Holding.
bool portfolio::removeHolding(string symbol, int quantity)
{
    for(int i = 0; i < holdings.size(); i++)
    {
        if(holdings[i].symbol == symbol)
        {
            if(quantity <= 0 || quantity > holdings[i].quantity)
                return false;

            holdings[i].quantity -= quantity;

            // Remove The Holding If No Shares Are Left.
            if(holdings[i].quantity == 0)
            {
                holdings.erase(holdings.begin() + i);
            }

            return true;
        }
    }

    return false;
}
