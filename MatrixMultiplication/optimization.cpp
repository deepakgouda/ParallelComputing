/**
Code to check optimizations in matrix multiplication.

Optimization 1 : Interchange inner loops, so that cache
loads the entire row in each interation.

Optimization 2 : Block multiplication, with block size 
depending on cache size.

Optimization 3 : Recursive method.
*/

#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<cstring>
#include<time.h>
#include<math.h>
#define debug(x) cerr<<#x<<" = "<<x<<endl;
#define MAX 1000

using namespace std;

void display(int *a[], int n)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout<<a[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void memset(int **a, int n)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			a[i][j] = 0;
		}
	}
}

// int ** matmul(int **a, int **b, int r1, int c1, int r2, int c2, int n)
// {
// }

int main(int argc, char const *argv[])
{
    srand(time(0)); 
	int s;
	clock_t start, stop;
	double duration;
	ofstream file1, file2, file3, file4;
	file1.open("dat1.txt");
	file2.open("dat2.txt");
	file3.open("dat3.txt");
	file4.open("dat4.txt");

	for (int n=1; n <= 512; n*=2)
	{
		int **a = new int*[n];
		int **b = new int*[n];
		int **c = new int*[n];
		for (int i = 0; i < n; ++i)
			a[i] = new int[n];
		for (int i = 0; i < n; ++i)
			b[i] = new int[n];
		for (int i = 0; i < n; ++i)
			c[i] = new int[n];

		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
			{
				a[i][j] = rand()%MAX;
				b[i][j] = rand()%MAX;
			}
		memset(c, n);

		start = clock();
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				for (int k = 0; k < n; ++k)
					c[i][j]+=a[i][k]*b[k][j];

		stop = clock();
		duration = ((double)(stop-start))/CLOCKS_PER_SEC; 
		file1<<duration<<endl;
		// display(c, n);

		// Optimization 1

		memset(c, n);
		start = clock();
		for (int i = 0; i < n; ++i)
		{
			for (int k = 0; k < n; ++k)
			{
				for (int j = 0; j < n; ++j)
				{
					c[i][j]+=a[i][k]*b[k][j];
				}
			}
		}
		stop = clock();
		duration = ((double)(stop-start))/CLOCKS_PER_SEC; 
		file2<<duration<<endl;
		// display(c, n);

		// Optimization 2
		memset(c, n);
		// $ lscpu | grep cache
		// L1d cache:           32K
		// L1i cache:           32K
		// L2 cache:            256K
		// L3 cache:            6144K

		s = 64;
		start = clock();
		for (int i = 0; i < n; i+=s)
			for (int j = 0; j < n; j+=s)
				for (int k = 0; k < n; k+=s)
					for (int i1 = 0; i1 < min(s, n); ++i1)
						for (int j1 = 0; j1 < min(s, n); ++j1)
							for (int k1 = 0; k1 < min(s, n); ++k1)
								c[i+i1][j+j1]+=a[i+i1][k+k1]*b[k+k1][j+j1];
		stop = clock();
		duration = ((double)(stop-start))/CLOCKS_PER_SEC; 
		file3<<duration<<endl;
		// display(c, n);

		// Optimization 3

		// memset(c, n);
		// start = clock();
		// c = matmul(a, b, 0, 0, 0, 0, n);
		// stop = clock();
		// duration = ((double)(stop-start))/CLOCKS_PER_SEC;
		// file4<<duration<<endl;
		// display(c, n);
	}
	file1.close();
	file2.close();
	file3.close();
	// file4.close();
	return 0;
}