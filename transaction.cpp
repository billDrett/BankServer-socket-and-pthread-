#include "transaction.h"
#include <cstdlib>

Transaction::Transaction(std::string* name, double amount)
{
    accountName = name;
    transactionAmount = amount;
}

Transaction::~Transaction()
{
}

std::string Transaction::get_account()
{
    return *accountName;
}

double Transaction::get_amount()
{
    return transactionAmount;
}

void Transaction::set_amount(double amount)
{
    transactionAmount = amount;
}
