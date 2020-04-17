/**
 * g++ -pthread -fopenmp naive.cpp && ./a.out <n> <numThreads>
 **/
#include <iostream>
#include <stdlib.h>
#include <omp.h>

#define MAX 10
#define debug(x) cerr<<#x<<" = "<<x<<endl;
using namespace std;

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

int main(int argc, char *argv[])
{
	int n = atoi(argv[1]);
	double start, stop;
	int *coefficient1 = new int[n]();
	int *coefficient2 = new int[n]();
	int *final_result = new int[n + n - 1]();

	srand(7);
	initialize(coefficient1, n);
	initialize(coefficient2, n);

	// cout<<"Polynomial 1 : ";
	// display(coefficient1, n);
	// cout<<"Polynomial 2 : ";
	// display(coefficient2, n);

	start = omp_get_wtime();

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            final_result[i+j] += coefficient1[i]*coefficient2[j];
        }
    }
	// display(final_result, n+n-1);

	stop = omp_get_wtime();
	cout << stop - start << endl;
	return 0;
}