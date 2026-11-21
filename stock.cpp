#include "stock.h"
#include <fstream>
#include <iomanip>
#include <cctype>
#include <windows.h>

using namespace std;

// Default Constructor.
stock::stock()
{
    symbol = "";
    companyName = "";
    price = 0.0;
    availableQuantity = 0;
}

// Parameterized Constructor.
stock::stock(string sym, string name, double stockPrice, int quantity)
{
    symbol = sym;
    companyName = name;
    price = stockPrice;
    availableQuantity = quantity;
}


// Display Individual Stock Details.
void stock::displayStock()
{
    cout << "\nStock Symbol: " << symbol << endl;
    cout << "Company Name: " << companyName << endl;
    cout << "Stock Price: Rs. " << fixed << setprecision(2) << price << endl;
    cout << "Available Quantity: " << availableQuantity << endl;
}


// Load Stock Details From File.
bool stockMarket::loadStocks()
{
    ifstream file("stocks.txt");

    if(!file)
    {
        cout << "\nUnable To Open Stock File.\n";
        return false;
    }

    stocks.clear();

    string symbol;
    string companyName;
    double price;
    int quantity;

    // Reading One Stock Record At A Time.
    while(file >> symbol >> companyName >> price >> quantity)
    {
        stock s(symbol, companyName, price, quantity);

        stocks.push_back(s);
    }

    file.close();

    return true;
}


// Display All Available Stocks.
void stockMarket::displayStocks()
{
    if(stocks.empty())
    {
        cout << "\nNo Stocks Available In The Market.\n";
        return;
    }

    cout << "\n========== STOCK MARKET ==========\n";

    for(int i = 0; i < stocks.size(); i++)
    {
        cout << "\nStock " << i + 1 << endl;
        stocks[i].displayStock();
    }
}

// Search For A Stock Using Its Symbol.
int stockMarket::findStockIndex(string stockSymbol)
{
    for(int i = 0; i < stocks.size(); i++)
    {
        if(stocks[i].symbol == stockSymbol)
        {
            return i;
        }
    }

    return -1;
}

// Search And Display A Stock Using Its Symbol.
void stockMarket::searchStock()
{
    string stockSymbol;

    cout << "\nEnter Stock Symbol: ";
    cin >> stockSymbol;

    // Convert The Entered Symbol To Uppercase.
    for(int i = 0; i < stockSymbol.length(); i++)
    {
        stockSymbol[i] = toupper((unsigned char)stockSymbol[i]);
    }

    int index = findStockIndex(stockSymbol);

    if(index != -1)
    {
        cout << "\nStock Found Successfully!\n";

        stocks[index].displayStock();
    }
    else
    {
        cout << "\nStock Not Found In The Market.\n";
        cout << "Please Enter A Valid Stock Symbol.\n";
    }
}

// Save Updated Stock Details To File.
bool stockMarket::saveStocks()
{
    ofstream file("stocks_temp.txt");

    if(!file)
    {
        cout << "\nUnable To Save Market Data.\n";
        return false;
    }

    for(int i = 0; i < stocks.size(); i++)
    {
        file << stocks[i].symbol << " "
             << stocks[i].companyName << " "
             << fixed << setprecision(2) << stocks[i].price << " "
             << stocks[i].availableQuantity << endl;
    }

    file.close();

    if(!file.good())
    {
        remove("stocks_temp.txt");
        cout << "\nUnable To Write Market Data.\n";
        return false;
    }

    if(!MoveFileExA("stocks_temp.txt", "stocks.txt",
                    MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
    {
        cout << "\nUnable To Update Market Data File.\n";
        return false;
    }

    return true;
}
