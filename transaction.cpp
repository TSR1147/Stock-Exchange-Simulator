#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits>
#include "transaction.h"

using namespace std;

// Buy Stocks For The Logged-In User.
void transaction::buyStock(user& loggedInUser, stockMarket& market, portfolio& userPortfolio)
{
    string symbol;
    int quantity;

    cout << "\nEnter Stock Symbol To Buy: ";
    cin >> symbol;

    for(int i = 0; i < symbol.length(); i++)
    {
        symbol[i] = toupper((unsigned char)symbol[i]);
    }

    int index = market.findStockIndex(symbol);

    if(index == -1)
    {
        cout << "\nStock Not Found In The Market.\n";
        return;
    }

    cout << "Enter Quantity: ";
    if(!(cin >> quantity))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nInvalid Quantity. Please Enter A Number.\n";
        return;
    }

    if(quantity <= 0)
    {
        cout << "\nQuantity Must Be Greater Than Zero.\n";
        return;
    }

    if(quantity > market.stocks[index].availableQuantity)
    {
        cout << "\nNot Enough Shares Available In The Market.\n";
        return;
    }

    double totalCost = market.stocks[index].price * quantity;

    if(totalCost > loggedInUser.balance)
    {
        cout << "\nInsufficient Account Balance.\n";
        return;
    }

    // Keep Copies So The In-Memory Values Can Be Restored If Saving Fails.
    double oldBalance = loggedInUser.balance;
    int oldMarketQuantity = market.stocks[index].availableQuantity;
    vector<holding> oldHoldings = userPortfolio.holdings;

    loggedInUser.balance -= totalCost;
    market.stocks[index].availableQuantity -= quantity;
    userPortfolio.addHolding(symbol, market.stocks[index].companyName,
                             quantity, market.stocks[index].price);

    // Save All Three Updated Records.
    if(!userPortfolio.savePortfolio(loggedInUser.username) ||
       !market.saveStocks() || !loggedInUser.updateBalance())
    {
        loggedInUser.balance = oldBalance;
        market.stocks[index].availableQuantity = oldMarketQuantity;
        userPortfolio.holdings = oldHoldings;
        cout << "\nTrade Could Not Be Fully Saved. Please Check Your Data Files Before Trading Again.\n";
        return;
    }

    cout << "\n========== BUY ORDER SUCCESSFUL ==========\n";
    cout << "Stock: " << symbol << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Price Per Share: Rs. " << fixed << setprecision(2)
         << market.stocks[index].price << endl;
    cout << "Total Amount: Rs. " << totalCost << endl;
    cout << "Remaining Balance: Rs. " << loggedInUser.balance << endl;
}


// Sell Stocks Owned By The Logged-In User.
void transaction::sellStock(user& loggedInUser, stockMarket& market, portfolio& userPortfolio)
{
    string symbol;
    int quantity;

    cout << "\nEnter Stock Symbol To Sell: ";
    cin >> symbol;

    for(int i = 0; i < symbol.length(); i++)
    {
        symbol[i] = toupper((unsigned char)symbol[i]);
    }

    int marketIndex = market.findStockIndex(symbol);

    if(marketIndex == -1)
    {
        cout << "\nStock Not Found In The Market.\n";
        return;
    }

    cout << "Enter Quantity: ";
    if(!(cin >> quantity))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nInvalid Quantity. Please Enter A Number.\n";
        return;
    }

    if(quantity <= 0)
    {
        cout << "\nQuantity Must Be Greater Than Zero.\n";
        return;
    }

    int holdingIndex = -1;

    for(int i = 0; i < userPortfolio.holdings.size(); i++)
    {
        if(userPortfolio.holdings[i].symbol == symbol)
        {
            holdingIndex = i;
            break;
        }
    }

    if(holdingIndex == -1 || quantity > userPortfolio.holdings[holdingIndex].quantity)
    {
        cout << "\nYou Do Not Own Enough Shares To Complete This Sale.\n";
        return;
    }

    double saleAmount = market.stocks[marketIndex].price * quantity;

    double oldBalance = loggedInUser.balance;
    int oldMarketQuantity = market.stocks[marketIndex].availableQuantity;
    vector<holding> oldHoldings = userPortfolio.holdings;

    if(!userPortfolio.removeHolding(symbol, quantity))
    {
        cout << "\nUnable To Update Your Portfolio.\n";
        return;
    }

    loggedInUser.balance += saleAmount;
    market.stocks[marketIndex].availableQuantity += quantity;

    if(!userPortfolio.savePortfolio(loggedInUser.username) ||
       !market.saveStocks() || !loggedInUser.updateBalance())
    {
        loggedInUser.balance = oldBalance;
        market.stocks[marketIndex].availableQuantity = oldMarketQuantity;
        userPortfolio.holdings = oldHoldings;
        cout << "\nTrade Could Not Be Fully Saved. Please Check Your Data Files Before Trading Again.\n";
        return;
    }

    cout << "\n========== SELL ORDER SUCCESSFUL ==========\n";
    cout << "Stock: " << symbol << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Price Per Share: Rs. " << fixed << setprecision(2)
         << market.stocks[marketIndex].price << endl;
    cout << "Sale Amount: Rs. " << saleAmount << endl;
    cout << "Updated Balance: Rs. " << loggedInUser.balance << endl;
}
