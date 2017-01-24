#ifndef Included_BankServer_H
#define Included_BankServer_H

#include <iostream>
#include "hashtable.h"
#include "bankAccount.h"
#include "queue.h"
#include <queue>
#include "List.h"

void* worker(void* ptr);

extern pthread_cond_t cond_nonempty;
extern pthread_cond_t cond_nonfull;

class BankServer
{
    private:
        HashTable* bankAccounts;
        Queue* socketQueue;
        //threads
    public:
        BankServer(int numberBuckets, int queueSize);
        ~BankServer();

        bool addAccount(bankAccount* bAccount, long delay = 0);
        bool addTransfer(std::string fromAccount, std::string toAccount, double amount, long delay = 0, bool lock = true);
        bool multi_addTransfer(std::string from, List<std::string>* to, double amount, long delay = 0);
        //multi transfer
        bool printBalance(std::string accountName, double& amount, bool lock = true);
        bool multiPrintBalance(List<std::string>* que, int fd);
        //multi print

        bool parseCommand(std::string& line, int fd);

        void masterThread(int port, int numberOfThreads);


};

struct threadData //gia na steilw ta dedomena sto thread function
{
    Queue* socketsQueue;
    BankServer* server;
};
#endif
