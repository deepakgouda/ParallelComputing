/**
 * g++ -pthread naive.cpp && ./a.out
 **/
#include<iostream>
#include<stdlib.h>
#include <pthread.h>

#define MAX 10
#define debug(x) cerr<<#x<<" = "<<x<<endl;
using namespace std;

// Block holds `n/p` length of coeff1 and coeff2 and stores
// the result in array result
typedef struct Block
{
	int l1, l2, blockID;
	int *coefficient1, *coefficient2, *result;
}Block;

// Initializes array with random values
void initialize(int *coefficients, int n)
{
	for (int i = 0; i < n; i++)
	{
		coefficients[i] = rand()%MAX;
	}
}

// Displays array
void display(int *coefficients, int n)
{
	for (int i = 0; i < n; i++)
	{
		cout<<coefficients[i]<<" ";
	}
	cout<<endl;
}

// Performs the coefficient multiplication in the block
void *polyMultiplication(void * block_h)
{
	// Typecast void * to Block *
	Block *block = (Block *)block_h;
	for (int i = 0; i < block->l1; i++)
	{
		for (int j = 0; j < block->l2; j++)
		{
			block->result[i+j] += block->coefficient1[i] * block->coefficient2[j];
		}
	}
	pthread_exit(NULL);
}

// Create array of blocks
Block *blocks = new Block[10];

int main(int argc, char *argv[])
{
	int n = 10, numThreads = 5, status;
	int *coefficient1 = new int[n]();
	int *coefficient2 = new int[n]();
	int *final_result = new int[n + n - 1]();

	srand(7);
	initialize(coefficient1, n);
	initialize(coefficient2, n);

	cout<<"Polynomial 1 : ";
	display(coefficient1, n);
	cout<<"Polynomial 2 : ";
	display(coefficient2, n);

	pthread_t threads[numThreads];

	for (int i = 0; i < numThreads; i++)
	{
		// Initialize blocks
		blocks[i].l1 = n / numThreads;
		blocks[i].l2 = n;
		blocks[i].blockID = i;
		blocks[i].coefficient1 = &coefficient1[i*n/numThreads];
		blocks[i].coefficient2 = coefficient2;
		blocks[i].result = new int[n+(n/numThreads)-1]();

		// Create threads and call polyMultiplication subroutine
		// Variables passed to the function subroutine cannot be shared. So, we create 
		// an array of blocks and send the addr of each block
		status = pthread_create(&threads[i], NULL, polyMultiplication, (void *)&blocks[i]);
		if(status)
		{
			cout << "ERROR; return code from pthread_create() is " << status << endl;
			exit(-1);
		}
	}

	// Add up the results from each thread to obtain final result
	for (int i = 0; i < numThreads; i++)
	{
		for (int j = 0; j < n + n/numThreads - 1; j++)
		{
			final_result[i*n/numThreads + j] += blocks[i].result[j];
		}
	}

	display(final_result, n+n-1);

	pthread_exit(NULL);
	return 0;
}