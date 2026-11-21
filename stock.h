#ifndef STOCK_H
#define STOCK_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// The Blueprint For Stock Class.
class stock
{
    public:

        // Stock Details.
        string symbol;
        string companyName;
        double price;
        int availableQuantity;

        // Default Constructor.
        stock();

        // Parameterized Constructor.
        stock(string sym, string name, double stockPrice, int quantity);

        // Display Individual Stock Details.
        void displayStock();
};

// The Blueprint For Stock Market Class.
class stockMarket
{
    public:

        // Stores All Available Stocks.
        vector<stock> stocks;

        // Load Stock Details From File.
        bool loadStocks();

        // Save Updated Stock Quantities To File.
        bool saveStocks();

        // Display All Available Stocks.
        void displayStocks();

        // Search For A Stock Using Its Symbol.
        int findStockIndex(string stockSymbol);

        // Search And Display A Stock Using Its Symbol.
        void searchStock();
};

#endif
