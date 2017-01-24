#include "hashtable.h"
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include "bankAccount.h"
#include <errno.h>

#include <pthread.h>
#include <unistd.h>

using namespace std;
HashTable::HashTable(int NBuckets)
{
    if(NBuckets <= 0)
    {
        perror("size under 0");
        exit(1);
    }

    nBuckets = NBuckets;
    totalSize = 0;
    pthread_mutex_init (&mtx, NULL); //arxikopoioume to mutex

    buckets = new Bucket*[nBuckets]; //kataskeuazoume ena pinaka apo buckets
    for(int i = 0; i < nBuckets; i++)
    {
        buckets[i] = new Bucket();
    }
}

HashTable::~HashTable()
{
    bankAccount* tmp;
    for(int i = 0; i < nBuckets; i++)
    {
        tmp = buckets[i]->bucketList->deleteFirstNode();
        while(tmp != NULL)
        {
            delete tmp;
            tmp = buckets[i]->bucketList->deleteFirstNode();//diagrafoume ta stoixeia tou bucket
        }
        delete buckets[i]; //kai meta to bucket
    }
    delete []buckets;

    pthread_mutex_destroy(&mtx);
}

int HashTable::hashFunction(std::string name)
{//djb2 hashfunction
    const char *str = name.c_str();
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % nBuckets;
}

int HashTable::get_size()
{
    return totalSize;
}

int HashTable::get_nBuckets()
{
    return nBuckets;
}

Node<bankAccount>* HashTable::get_bucket(std::string name)
{ //epistregei thn lista pou antistixei se auto to bucket
    return buckets[hashFunction(name)]->bucketList->get_begin();
}

void HashTable::lockHashtable() //kleidwnei to hashtable
{
    pthread_mutex_lock(&this->mtx);
}

void HashTable::unlockHashtable() //ksekleidwnei to hashtable
{
    pthread_mutex_unlock(&this->mtx);
}

void HashTable::lockBucketOf(std::string& name) //kleidwnei to bucket
{
    pthread_mutex_lock(&(buckets[hashFunction(name)]->mtx));
}

void HashTable::unlockBucketOf(std::string& name) //ksekleidwnei to bucket
{
    pthread_mutex_lock(&(buckets[hashFunction(name)]->mtx));
}


bankAccount* HashTable::searchNode(std::string name) //psaxnei ena stoixeio tou hashtable
{
    Node<bankAccount>* tmp = buckets[hashFunction(name)]->bucketList->get_begin();

    while(tmp != NULL)
    {
        if(*(tmp->get_data()->get_name()) == name)
        {
            return tmp->get_data();
        }
        tmp = tmp->get_next();
    }
    return NULL;
}
/*                                              */

bool HashTable::insertNode(bankAccount* bAccount) //eisagei ena stoixeio sto hashtable
{
    if(searchNode(*(bAccount->get_name())) != NULL) return false; //yparxei idi

    totalSize++;
    buckets[hashFunction(*(bAccount->get_name()))]->bucketList->insertEnd(bAccount);
    return true;
}

/*//dn xreaizetai
bool HashTable::deleteNode(std::string name) //dn xreaizetai
{
    Node<bankAccount>* tmp = buckets[hashFunction(name)]->get_begin();

    while(tmp != NULL)
    {
        if(tmp->get_data()->get_name() == name)
        {
            //diagrafei apo opoio bankaccount mporei na exei kanei synallagi me auto

            totalSize--;
            delete buckets[hashFunction(Id)]->deleteNode(tmp);
            return true;
        }
        tmp = tmp->get_next();
    }

    return false;
}*/
