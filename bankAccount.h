#ifndef Included_bankAccount_H
#define Included_bankAccount_H

#include "List.h"
#include <iostream>

class Transaction;

class bankAccount
{
    private:
        std::string* name;
        double amount;
        List<Transaction>* receivedTransactions;

    public:
        bankAccount(std::string Name, double startAmount);
        ~bankAccount();

        std::string* get_name();
        double get_amount();
        Node<Transaction>* get_receivedTrs();

        bool sendAmount(std::string* toAccount, int sAmount);
        void receiveAmount(std::string* fromAccount, int rAmount);
};

#endif

