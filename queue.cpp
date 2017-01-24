#include "queue.h"

#include <pthread.h>
#include <unistd.h>

#include <iostream>

using namespace std;
Queue::Queue(int size): maxSize(size)
{
    queueArray = new int[size];
    start = 0;
    end = -1;
    count = 0;

    pthread_mutex_init (&mtx, NULL);
}

Queue::~Queue()
{
    delete queueArray;
    pthread_mutex_destroy(&mtx);
}

int Queue::getMaxSize() //epistrefei to megisto megethos ths ouras
{
    return maxSize;
}

int Queue::getCount() //epistrefei to megethos ths wras auth th stigmi
{
    return count;
}

void Queue::push(int item) //prosthetei ena int sto telos ths ouras
{
    pthread_mutex_lock(&mtx);
	while (count >= maxSize) //ama einai gemati perimenw signal
	{
		pthread_cond_wait(&cond_nonfull, &mtx);
    }
	end = (end + 1) % maxSize; //epomeno stoixeio ouras
	queueArray[end] = item;
	count++;
	pthread_mutex_unlock(&mtx);
}

int Queue::pop()
{
    int data = 0;

	pthread_mutex_lock(&mtx);
	while (count <= 0) //ama einai adeia perimenw signal
	{
		pthread_cond_wait(&cond_nonempty, &mtx);
    }
	data = queueArray[start];
	start = (start + 1) % maxSize;
	count--;
	pthread_mutex_unlock(&mtx);

	return data;
}
