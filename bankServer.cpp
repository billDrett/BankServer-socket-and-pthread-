#include "bankServer.h"
#include "bankAccount.h"
#include "hashtable.h"
#include "readWriteFunct.h"
#include "queue.h"

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */

pthread_cond_t cond_nonempty;
pthread_cond_t cond_nonfull;

using namespace std;

BankServer::BankServer(int numberBuckets, int queueSize)
{
    bankAccounts = new HashTable(numberBuckets);
    socketQueue = new Queue(queueSize);
}

BankServer::~BankServer()
{
    delete bankAccounts;
    delete socketQueue;
}

bool BankServer::addAccount(bankAccount* bAccount, long delay)
{
    bool result;

    bankAccounts->lockHashtable();
    usleep(delay); //koimathe msec
    result = bankAccounts->insertNode(bAccount);
    bankAccounts->unlockHashtable();

    return result;
}

bool BankServer::addTransfer(std::string fromAccount, std::string toAccount, double amount, long delay, bool lock)
{
    bankAccount* frAccount;
    bankAccount* tAccount;


    if(lock == true) //an prepei na kleidwthei to kleidwnei
    {
        bankAccounts->lockHashtable();
        usleep(delay);
    }

    frAccount = bankAccounts->searchNode(fromAccount); //psaxnw ta dyo stoxeia
    tAccount = bankAccounts->searchNode(toAccount);

    if(frAccount == NULL || tAccount == NULL)
    {
        if(lock == true) bankAccounts->unlockHashtable();
        return false;
    }
    if(frAccount->sendAmount(tAccount->get_name(), amount) == false)
    {
        if(lock == true) bankAccounts->unlockHashtable();
        return false;
    }

    tAccount->receiveAmount(frAccount->get_name(), amount); //kai metaferw ta xrhmata
    if(lock == true) bankAccounts->unlockHashtable(); //ama to kleidwsa prepei na to kseklidwsw
    return true;
}

bool BankServer::multi_addTransfer(std::string from, List<std::string>* to, double amount, long delay)
{
    string* tmp;
    bool success = true;

    bankAccounts->lockHashtable(); //kleidwno to hashtable
    usleep(delay);

    for(Node<std::string>* i = to->get_begin(); i != NULL; i = i->get_next())
    {
        tmp = i->get_data();
        if(this->addTransfer(from, *tmp, amount, 0, false) == false) //ama apotyxei esto kai mia prepei na epistrafoun ta proigoumena posa
        {
            success = false;
            for(Node<std::string>* j = to->get_begin(); j != i; j = j->get_next()) // prepei na epanaferw oso xrhmata esteila mexri tora
            {
                tmp = j->get_data();
                this->addTransfer(*tmp, from, amount, 0, false);
            }
            break;
        }
    }

    bankAccounts->unlockHashtable(); //kseklidwma
    return success;
}


bool BankServer::printBalance(std::string accountName, double& amount, bool lock)
{
    bankAccount* bAccount;

    if(lock == true) bankAccounts->lockHashtable();
    bAccount = bankAccounts->searchNode(accountName);

    if(bAccount == NULL) //an dn yparxei vgainw kai epistrefw apotyxia
    {
        if(lock == true) bankAccounts->unlockHashtable();
        return false;
    }
    amount = bAccount->get_amount();

    if(lock == true) bankAccounts->unlockHashtable();
    return true;
}

bool BankServer::multiPrintBalance(List<std::string>* que, int fd)
{
    string printString = "";
    string* tmp;
    bool success = true;
    double amount;
    char amountString[24];

    bankAccounts->lockHashtable();
    for(Node<std::string>* i = que->get_begin(); i != NULL; i = i->get_next())
    {
        tmp = i->get_data();
        if(this->printBalance(*tmp, amount, false) == false) //an apotyxei esto kai ena prepei na steilw mnm apotyxias
        {
            success = false;
            break;
        }

        sprintf(amountString, "%lf", amount);
        printString += *tmp + "/" + amountString; //prosthetw to name kai amount sto string ektypwshs
        if(i->get_next() != NULL) printString +=":"; //sto teleutaio dn prepei na mpei to ":"

    }
    bankAccounts->unlockHashtable();

    if(success == true)
    {
        printString.insert(0, "Success. Multi-Balance (");
        printString.append(")");
    }
    else //an apetyxe prepei na steilw mnm sfalmatos
    {
        printString.clear();
        printString.insert(0, "Error. Multi-Balance (");
        for(Node<std::string>* i = que->get_begin(); i != NULL; i = i->get_next()) //vazw ola ta names
        {
            printString += *(i->get_data());
            if(i->get_next() != NULL) printString +=":";
        }
        printString.append(")");
    }

    write_data(fd, printString.c_str());
    return success;
}

void BankServer::masterThread(int port, int numberOfThreads) //arxiko thread
{
    int sock, newsock;
    struct sockaddr_in server, client;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr *serverptr=(struct sockaddr *)&server;
    struct sockaddr *clientptr=(struct sockaddr *)&client;
    struct hostent *rem;

    pthread_t cons, prod;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(2);
    }

    server.sin_family = AF_INET;       /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);      /* The given port */
    /* Bind socket to address */
    if (bind(sock, serverptr, sizeof(server)) < 0)
    {
        perror("bind");
        exit(2);
    }

    /* Listen for connections */
    if (listen(sock, 5) < 0)
    {
        perror("listen");
        exit(2);
    }
    printf("Listening for connections to port %d\n", port);

    //kataskeuei threads
  	struct threadData* data = (struct threadData*) malloc(sizeof(struct threadData));

    data->socketsQueue = this->socketQueue;
    data->server = this;
    for(int i=0; i< numberOfThreads; i++)
    {
        pthread_create(&cons, NULL, worker, (void*)data); //kathe thread ttha parei ta idia data giati douleuei ston idio server kai sto ido queue
    }

    while (1) //stamataei mono me control c
    {
        /* accept connection */
    	if ((newsock = accept(sock, clientptr, &clientlen)) < 0)
    	{
            perror("accept");
            exit(2);
    	}
    	printf("Accepted connection\n");


        socketQueue->push(newsock); //to prosthetw sthn oura
        pthread_cond_signal(&cond_nonempty); //kai eidopoiw oti h oura dn einai kenh, akrivos ena kai opoiodipote thread thelw na ksypnisei gia auto dn xreiazetai broadcast

    }

    delete data;
}

void* worker(void* ptr) //thread function
{
    struct threadData* data = (struct threadData*)(ptr);
    char buf[1024];
    cout<<"thread created"<<endl;
    string buff;
    int i, fd;
    while(1)
    {
        fd = data->socketsQueue->pop(); //afairw ena socket apo thn oura
        pthread_cond_signal(&cond_nonfull); //eidopoiw oti dn einai gemati
        printf("pop %d thread %ld\n", fd, pthread_self());
        while((i = read_data(fd, buf)) > 0) //diavaszw mexri na kleisei h syndesh
        {
            buff = buf;
            if(data->server->parseCommand(buff, fd) == false) //stelnw apanthsh 'h mnm sfalmatos
            {
                write_data(fd, "Error. Unknown command");
            }
        }

        printf("Closing connection.\n");
        close(fd);	  /* Close socket */
    }

}


bool BankServer::parseCommand(string& line, int fd) //ektelei thn entolh kai stelnei apotelesma
{
    string command, name;
    double amount;
    long delay;

    istringstream iss(line);

    iss>> command;
    if(iss.fail())
    {
        return false;
    }
    if(command == "add_account")
    {
        iss>> amount>>name;
        if(!iss)
        {
            return false;
        }

        iss>>delay;
        if(!iss) //an dn yparxei delay to thetw 0
        {
            delay = 0;
        }

        bankAccount* tmp = new bankAccount(name, amount);
        string answer = "Account creation ";
        if(!this->addAccount(tmp, delay)) //prosthetw to account sto hashtable
        {
            answer.insert(0, "Error. ");
            answer +="failed ";
            delete tmp;
        }
        else
        {
            answer.insert(0, "Success. ");
        }

        char amountString[24];
        sprintf(amountString, "%lf", amount); //metatropi se string
        answer += "(" + name + ":" + string(amountString)+")"; //egrafh mnmatos
        write_data(fd, answer.c_str());

    }
    else if(command == "add_transfer")
    {
        string fromAc, ToAc;
        iss >>amount >> fromAc >> ToAc;
        if(!iss)
        {
            return false;
        }
        iss>>delay;
        if(!iss)
        {
            delay = 0;
        }

        string answer = "Transfer addition ";
        if(!this->addTransfer(fromAc, ToAc, amount, delay))
        {
            answer.insert(0, "Error. ");
            answer +="failed ";
        }
        else
        {
            answer.insert(0, "Success. ");
        }

        char amountString[24];
        char delayString[24];

        sprintf(amountString, "%lf", amount);
        sprintf(delayString, "%ld", delay);

        answer += "(" + fromAc + ":" + ToAc + ":" + string(amountString)+"[:" + string(delayString)+ "]" + ")";
        write_data(fd, answer.c_str());

    }
    else if(command == "add_multi_transfer")
    {
        List<std::string>* accounts = new List<std::string>();
        string dstName;
        string* tmp;

        iss >> amount >> name;

        while(true)
        {
            iss >> dstName;
            if(!iss) break;

            tmp = new string(dstName); //kathe name pou diavazw to prosthetw sthn lista
            accounts->insertEnd(tmp);
        }
        tmp = accounts->get_last()->get_data(); //to teleutaio mporei na einai to delay
        delay = atoi(tmp->c_str());

        if(delay != 0) delete accounts->deleteLastNode(); //an einai to delay to afairw apo thn lista

        string answer = "Multi-Transfer addition ";
        if(this->multi_addTransfer(name, accounts, amount, delay) == true)
        {
            answer.insert(0, "Success. ");
        }
        else
        {
            answer.insert(0, "Error. ");
            answer+=("failed ");
        }

        char amountString[24];
        char delayString[24];

        sprintf(amountString, "%lf", amount);
        sprintf(delayString, "%ld", delay);

        answer += "(" + name + ":" + string(amountString)+"[:" + string(delayString)+ "]" + ")";
        write_data(fd, answer.c_str());

        while((tmp = accounts->deleteFirstNode()) != NULL)//diagrafh domwn pou eftiksa proigoumenos
        {
            delete tmp;
        }
        delete accounts;

    }
    else if(command == "print_balance")
    {
        double tmpAmount;
        iss>> name;
        if(!iss)
        {
            return false;
        }

        string answer = "Balance ";
        answer += "(" + name;

        if(this->printBalance(name, tmpAmount))
        {
            answer.insert(0, "Success. ");
            char amountString[24];
            sprintf(amountString, "%lf", tmpAmount);

            answer += ":" + string(amountString)+")";
        }
        else
        {
            answer.insert(0, "Error. ");
            answer += ")";
        }

        write_data(fd, answer.c_str());

    }
    else if(command == "print_multi_balance")
    {
        double tmpAmount;
        List<std::string>* accounts = new List<std::string>();
        string* tmp;

        while(true)
        {
            iss >> name;
            if(!iss) break;

            tmp = new string(name);
            accounts->insertEnd(tmp);
        }

        this->multiPrintBalance(accounts, fd);

        while((tmp = accounts->deleteFirstNode()) != NULL)
        {
            delete tmp;
        }
        delete accounts;

    }
    else //error
    {
        return false;
    }

    return true;
}
