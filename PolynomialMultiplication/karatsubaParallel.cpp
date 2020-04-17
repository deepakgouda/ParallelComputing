/**
 * g++ -pthread -fopenmp karatsuba.cpp && ./a.out
 **/
#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <pthread.h>

#define MAX 10
#define debug(x) cerr << #x << " = " << x << endl;
using namespace std;

// Block holds `n/p` length of coeff1 and coeff2 and stores
// the result in array result
typedef struct Block
{
	int n;
	int *coefficient1, *coefficient2, *result;
} Block;

// Initializes array with random values
void initialize(int *coefficients, int n)
{
	for (int i = 0; i < n; i++)
	{
		coefficients[i] = rand() % MAX;
	}
}

// Displays array
void display(int *coefficients, int n)
{
	for (int i = 0; i < n; i++)
	{
		std::cout << coefficients[i] << " ";
	}
	std::cout << endl;
}

// Performs the coefficient multiplication in the block
void *polyMultiplication(void *block_h)
{
	// Typecast void * to Block *
	Block *block = (Block *)block_h;
	int n = block -> n;

	// If n is less than 20, perform naive computation and return result
	if (n <= 128)
	{
		block->result = new int[2*n-1]();
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				block->result[i+j] += block->coefficient1[i] * block->coefficient2[j];
			}
		}
		return nullptr;
	}

	Block *allBlocks = new Block[3];
	allBlocks[0].n = (n) / 2;
	allBlocks[1].n = (n) / 2;
	allBlocks[2].n = (n) / 2;

	// Calculate LL product
	allBlocks[0].coefficient1 = block->coefficient1;
	allBlocks[0].coefficient2 = block->coefficient2;

	allBlocks[1].coefficient1 = new int[n/2];
	allBlocks[1].coefficient2 = new int[n/2];
	
	// Calculate HL product
	for (int i = 0; i < n/2; i++)
	{
		allBlocks[1].coefficient1[i] = block->coefficient1[i] + block->coefficient1[i+n/2];
		allBlocks[1].coefficient2[i] = block->coefficient2[i] + block->coefficient2[i+n/2];
	}
	
	// Calculate HH product
	allBlocks[2].coefficient1 = &(block->coefficient1[n/2]);
	allBlocks[2].coefficient2 = &(block->coefficient2[n/2]);

	pthread_t threads[3];
	pthread_attr_t attr[3];

	for (int i = 0; i < 3; i++)
	{
		pthread_attr_init(&attr[i]);
		pthread_attr_setdetachstate(&attr[i], PTHREAD_CREATE_JOINABLE);
		pthread_create(&threads[i], &attr[i], polyMultiplication, (void *)&allBlocks[i]);
	}
	
	for (int i = 0; i < 3; i++)
	{
		pthread_join(threads[i], NULL);
	}

	block->result = new int[2*n-1]();

	for (int i = 0; i < n; i++)
	{
		block->result[i] = allBlocks[0].result[i];
		block->result[i+n] = allBlocks[2].result[i];
	}

	for (int i = 0; i < n; i++)
	{
		block->result[i + n / 2] += allBlocks[1].result[i] - allBlocks[0].result[i] - allBlocks[2].result[i];
	}
	return nullptr;
}

int main(int argc, char *argv[])
{
	int n = atoi(argv[1]), status;
	int *coefficient1 = new int[n]();
	int *coefficient2 = new int[n]();
	int *final_result = new int[2*n-1]();
	double start, stop;

	srand(7);
	initialize(coefficient1, n);
	initialize(coefficient2, n);

	// std::cout << "Polynomial 1 : ";
	// display(coefficient1, n);
	// std::cout << "Polynomial 2 : ";
	// display(coefficient2, n);

	Block *mainBlock = new Block();
	mainBlock -> n = n;
	mainBlock -> coefficient1 = coefficient1;
	mainBlock -> coefficient2 = coefficient2;

	start = omp_get_wtime();
	polyMultiplication((void *)mainBlock);
	stop = omp_get_wtime();
	cout << stop - start << endl;
	// display(mainBlock->result, 2*n-1);

	return 0;
}