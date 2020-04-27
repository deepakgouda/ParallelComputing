/**
 * g++ -pthread -fopenmp producerConsumer.cpp && ./a.out <numProducers> <numConsumers> <numQueues> <queueSize>
 */
#include <iostream>
#include <vector>
#include <mutex>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX 10
#define MAXLEN 50

using namespace std;

// Block simulates a task with n simulating the number of 
// instructions to be performed
typedef struct Block
{
	unsigned int n;

public:
	Block(int n)
	{
		this->n = n;
	}
} Block;

double start, timeout, stop;

// A stacked queue implementation which consists of n number of
// queues of length count. Corressponding to each queue segment
// we have two mutexes producerLock and consumerLock.
class Queue
{
	vector<vector<Block>> pipe;
	int n, count;
	mutex producerLocks[MAXLEN];
	mutex consumerLocks[MAXLEN];

public:
	int totalOps = 0;
	Queue(int n, int count)
	{
		this->n = n;
		this->count = count;
		pipe.resize(n);
	}

	void push(Block b)
	{
		int indx = 0;
		while (true)
		{
			// If timeout, stop waiting and return
			if ((omp_get_wtime() - start) >= timeout)
				return;
			indx = (indx + 1) % n;

			// If queue segment is not full, push Block
			if (pipe[indx].size() < count)
			{
				// Try to acquire producerLock. If successful
				// push block and release lock.
				if (producerLocks[indx].try_lock())
				{
					pipe[indx].push_back(b);
					producerLocks[indx].unlock();
					printf("%02.7f : Block pushed %d\n", omp_get_wtime() - start, b.n);
					return;
				}
			}
		}
	}

	int pop()
	{
		int val, indx = 0;
		while (true)
		{
			// If timeout, stop waiting and return
			if ((omp_get_wtime() - start) >= timeout)
				return -1;
			indx = (indx + 1) % n;
			// If queue segment is not empty, pop Block
			if (pipe[indx].size() > 0)
			{
				// Try to acquire consumerLock. If successful
				// pop block and release lock.
				if (consumerLocks[indx].try_lock())
				{
					val = pipe[indx][0].n;
					// Pop first element
					pipe[indx].erase(pipe[indx].begin());
					consumerLocks[indx].unlock();
					// Add total operations performed to calculate
					// throughput
					this->totalOps += val;
					printf("%02.7f : Block popped %d\n", omp_get_wtime() - start, val);
					return val;
				}
			}
		}
	}
};

Queue *q;

void *producer(void *v)
{
	unsigned int producerTimeout = 5 * 10000;
	while ((omp_get_wtime() - start) < timeout)
	{
		// Produce a block and push it to queue
		q->push(Block((rand() % MAX) + 1));
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
		// Pop a block from the queue
		value = q->pop();
		// value < 0 returned when timeout
		if (value < 0)
			break;
		consumerTimeout = value * 10000;
		usleep(consumerTimeout);
	}
	return nullptr;
}

int main(int argc, char *argv[])
{
	int numProducers = atoi(argv[1]), numConsumers = atoi(argv[2]);
	int n = atoi(argv[3]), count = atoi(argv[4]);

	q = new Queue(n, count);
	srand(7);
	// Total time to run the simulation
	timeout = 5;

	start = omp_get_wtime();
	// Create producer and consumer threads
	pthread_t producers[numProducers];
	pthread_t consumers[numConsumers];

	for (int i = 0; i < numProducers; i++)
	{
		pthread_create(&producers[i], NULL, producer, (void *)NULL);
	}
	for (int i = 0; i < numConsumers; i++)
	{
		pthread_create(&consumers[i], NULL, consumer, (void *)NULL);
	}

	for (int i = 0; i < numProducers; i++)
	{
		pthread_join(producers[i], NULL);
	}
	for (int i = 0; i < numConsumers; i++)
	{
		pthread_join(consumers[i], NULL);
	}

	stop = omp_get_wtime();

	printf("Total operations = %d\n", q->totalOps);
	printf("Total time = %lf seconds\n", stop - start);
	printf("Throughput = %lf\n", (double)(q->totalOps) / (stop - start));
	return 0;
}