#include "bankAccount.h"
#include "transaction.h"
#include "List.h"
#include <cstdlib>

#include <iostream>
using namespace std;

bankAccount::bankAccount(std::string Name, double startAmount)
{
    name = new string(Name);
    amount = startAmount;

    receivedTransactions = new List<Transaction>();
}

bankAccount::~bankAccount()
{
    delete name;

    Transaction* tmp = receivedTransactions->deleteFirstNode();
    while(tmp != NULL) //diagrafei tin receivedTransactions
    {
        delete tmp;
        tmp = receivedTransactions->deleteFirstNode();
    }

    delete receivedTransactions;
}

std::string* bankAccount::get_name()
{
    return name;
}

double bankAccount::get_amount()
{
    return amount;
}

Node<Transaction>* bankAccount::get_receivedTrs()
{
    return receivedTransactions->get_begin();
}

bool bankAccount::sendAmount(std::string* toAccount, int sAmount)
{
    if(this->amount < sAmount) return false;

    amount -= sAmount;
    return true;
}

void bankAccount::receiveAmount(std::string* fromAccount, int rAmount)
{
    amount += rAmount;
    Node<Transaction>* tmp = receivedTransactions->get_begin();
    while(tmp != NULL) //psaxnei an yparxei idi
    {
        if(tmp->get_data()->get_account() == (*fromAccount)) //an yparxei prosthetei to varos sto yparxon
        {
            tmp->get_data()->set_amount(tmp->get_data()->get_amount()+ rAmount);
            return;
        }
        tmp = tmp->get_next();
    }

    Transaction* trans = new Transaction(fromAccount, rAmount);
    receivedTransactions->insertEnd(trans);
}
