#include <iostream>
#include <deque>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX 10

using namespace std;

typedef struct Block
{
	unsigned int n;
	public:
	Block(int n)
	{
		this -> n = n;
	}
}Block;

double start, timeout, stop;

class Queue
{
	deque <Block> pipe;
	public:
	int lock = 1;
	int totalOps = 0;
	int maxSize = 5;

	void push(Block b)
	{
		// Wait if the queue is full
		while (pipe.size() == maxSize);

		pipe.push_back(b);
		
		printf("%02.7f : Block pushed %d\n", omp_get_wtime()-start, b.n);
	}
	
	int pop()
	{
		int value;

		// Wait if the queue is empty
		while(pipe.empty());

		Block b = pipe.front();
		value = b.n;
		pipe.pop_front();

		totalOps += b.n;

		printf("%02.7f : Block popped %d\n", omp_get_wtime()-start, value);
		return value;
	}
};

Queue *q = new Queue;

void *producer(void *v)
{
	unsigned int producerTimeout = 5 * 100000;

	while ((omp_get_wtime() - start) < timeout)
	{
		q -> push(Block((rand() % MAX) + 1));
		usleep(producerTimeout);
	}
	return nullptr;
}

void *consumer(void *v)
{
	int value;
	unsigned int consumerTimeout;

	while ((omp_get_wtime() - start) < timeout)
	{
		while(q -> lock != 1);
		q -> lock = 0;
		value = q -> pop();
		consumerTimeout = value * 100000;
		usleep(consumerTimeout);
		q -> lock = 1;
	}
	return nullptr;
}

int main(int argc, char *argv[])
{
	int p = 4;
	srand(7);
	timeout = 5;
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

	stop = omp_get_wtime();

	printf("Total operations = %d\n", q -> totalOps);
	printf("Total time = %lf seconds\n", stop-start);
	printf("Throughput = %lf\n", (double)(q -> totalOps)/(stop-start));
	return 0;
}