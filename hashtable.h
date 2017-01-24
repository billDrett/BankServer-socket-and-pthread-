#ifndef Included_HashTable_H
#define Included_HashTable_H

#include "List.h"
#include "Node.h"
//#include "bankAccount.h"
#include <iostream>
#include <pthread.h>

class bankAccount;
class Bucket
{
    public:
        List<bankAccount>* bucketList; //lista apo bankAccount
        pthread_mutex_t mtx; //kai mutex gia kleidwma

        Bucket()
        {
            bucketList = new List<bankAccount>();
            pthread_mutex_init (&mtx, NULL);
        }
        ~Bucket()
        {
            delete bucketList;
            pthread_mutex_destroy(&mtx);
        }
};

class HashTable
{
    private:
        int nBuckets;
        int totalSize;
        Bucket** buckets;
        pthread_mutex_t mtx;

    public:
        HashTable(int NBuckets);
        ~HashTable();

        int get_size();
        int get_nBuckets();

        int hashFunction(std::string name);

        Node<bankAccount>* get_bucket(std::string name);
        void lockHashtable();
        void unlockHashtable();

        void lockBucketOf(std::string& name);
        void unlockBucketOf(std::string& name);

        bool insertNode(bankAccount* bAccount);
        bankAccount* searchNode(std::string name);
        //bool deleteNode(std::string name);

        //void printSizeOfBuckets();
};
#endif

