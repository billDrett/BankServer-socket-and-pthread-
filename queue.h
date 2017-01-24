#ifndef Included_Queue_H
#define Included_Queue_H

#include <pthread.h>
#include <unistd.h>

extern pthread_cond_t cond_nonempty;
extern pthread_cond_t cond_nonfull;

class Queue
{//cyrcle queue thread safe
    private:
        int* queueArray;
        const int maxSize;
        int start, end, count;

        pthread_mutex_t mtx;

    public:
        Queue(int size);
        ~Queue();

        int getMaxSize();
        int getCount();

        void push(int item);
        int pop();
};

#endif
