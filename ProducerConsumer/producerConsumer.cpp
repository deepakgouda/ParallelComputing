#include <iostream>
#include <deque>
#include <omp.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

typedef struct Block
{
	int n;
	public:
	Block(int n)
	{
		this -> n = n;
	}
}Block;

double start, timeout;
int maxOps;

class Queue
{
	deque <Block> pipe;
	public:
	int lock = 1;
	int opsLock = 1;
	int numOps = 0;
	unsigned int microseconds = 500000;

	void push(Block b)
	{
		pipe.push_back(b);
		usleep(microseconds);
		
		while(opsLock != 1);
		opsLock = 0;
		printf("%02d : Block pushed\n", numOps);
		numOps++;
		opsLock = 1;
	}
	
	void pop()
	{
		pipe.pop_front();
		usleep(microseconds);

		while (opsLock != 1);
		opsLock = 0;
		printf("%02d : Block popped\n", numOps);
		numOps++;
		opsLock = 1;
	}
};

Queue *q = new Queue;

void *producer(void *v)
{
	while(q -> numOps < maxOps)
	{
		q -> push(Block(1));
	}
	return nullptr;
}

void *consumer(void *v)
{
	while (q -> numOps < maxOps)
	{
		while(q -> lock != 1);
		q -> lock = 0;
		q -> pop();
		q -> lock = 1;
	}
	return nullptr;
}

int main(int argc, char *argv[])
{
	int p = 4;
	timeout = 0.0004;
	maxOps = 1;
	start = omp_get_wtime();

	pthread_t threads[p];

	for (int i = 0; i < p; i++)
	{
		if(i%2)
			pthread_create(&threads[i], NULL, consumer, (void *)NULL);
		else
			pthread_create(&threads[i], NULL, producer, (void *)NULL);
	}

	for (int i = 0; i < p; i++)
	{
		pthread_join(threads[i], NULL);
	}
	return 0;
}