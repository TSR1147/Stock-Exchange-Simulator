#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "user.h"
#include "stock.h"
#include "portfolio.h"

// Blueprint For Buying And Selling Stocks.
class transaction
{
    public:
        // Buy Stocks For The Logged-In User.
        void buyStock(user& loggedInUser, stockMarket& market, portfolio& userPortfolio);

        // Sell Stocks Owned By The Logged-In User.
        void sellStock(user& loggedInUser, stockMarket& market, portfolio& userPortfolio);
};

#endif
