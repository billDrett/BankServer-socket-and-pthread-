#ifndef Included_Transaction_H
#define Included_Transaction_H

#include <iostream>

class Transaction
{
    private:
        std::string* accountName;
        double transactionAmount;

    public:
        Transaction(std::string* name, double amount);
        ~Transaction();

        std::string get_account();
        double get_amount();

        void set_amount(double amount);
};
#endif

