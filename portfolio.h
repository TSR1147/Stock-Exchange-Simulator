#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <iostream>
#include <string>
#include <vector>
#include "stock.h"

using namespace std;

// Blueprint For Individual Stock Holding.
class holding
{
    public:

        string symbol;
        string companyName;
        int quantity;
        double purchasePrice;

        // Default Constructor.
        holding();

        // Parameterized Constructor.
        holding(string sym, string name, int qty, double price);
};


// Blueprint For User Portfolio.
class portfolio
{
    public:

        // Stores All Stocks Owned By The User.
        vector<holding> holdings;

        // Load User Portfolio From File.
        bool loadPortfolio(string username);

        // Save User Portfolio To File.
        bool savePortfolio(string username);

        // Display User Portfolio.
        void displayPortfolio(stockMarket& market);

        // Add A Stock To The Portfolio.
        void addHolding(string symbol, string companyName, int quantity, double purchasePrice);

        // Remove Shares From An Existing Holding.
        bool removeHolding(string symbol, int quantity);
};

#endif
