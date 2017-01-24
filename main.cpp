#include <iostream>
using namespace std;

#include "bankServer.h"
#include "bankAccount.h"
#include "transaction.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */

#include "readWriteFunct.h"

#define HASHTABLESIZE 100000
int main(int argc, char *argv[])
{
    int workThreads = -1 , queueSize = -1, port = -1, hashTSize = HASHTABLESIZE;
    if (argc != 7)
    {
        cout<<"Please give all attributes -p <port> -s <thread pool size> -q <queue size>"<<endl;
        exit(1);
    }

    for(int i=1; i < argc; i++) //diavasma parametrwn apo grammi entolis me morfi -p <port> -s <thread pool size> -q <queue size>
    {
        if(i+1 != argc)
        {
            if(strcmp(argv[i], "-p") == 0)
            {
                port = atoi(argv[i+1]);
            }
            else if(strcmp(argv[i], "-s") == 0)
            {
                workThreads = atoi(argv[i+1]);
            }
            else if(strcmp(argv[i], "-q") == 0)
            {
                queueSize = atoi(argv[i+1]);
            }
        }
    }

    if(port == -1 || workThreads == -1 || queueSize == -1)
    {
        cout<<"Please give all attributes -p <port> -s <thread pool size> -q <queue size>"<<endl;
       	exit(1);
    }

    BankServer* server = new BankServer(hashTSize, queueSize);

    server->masterThread(port, workThreads); //trexw ton server

    delete server;
}

