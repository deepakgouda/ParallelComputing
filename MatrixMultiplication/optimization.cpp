/**
Code to check optimizations in matrix multiplication.

Optimization 1 : Interchange inner loops, so that cache
loads the entire row in each interation.

Optimization 2 : Block multiplication, with block size 
depending on cache size.

Optimization 3 : Recursive method.
*/

#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<fstream>
#include<cstring>
#include<time.h>
#include<math.h>

#include<chrono>

#define debug(x) cerr<<#x<<" = "<<x<<endl;
#define MAX 1000

using namespace std;
using namespace std::chrono;

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

void matmul(int **a, int **b, int **c, int row1, 
			int col1, int row2, int col2, int n)
{
	static int i = 0, j = 0, k = 0;

	if (i >= row1)
		return;

	if (j < col2)
	{
		if (k < col1)
		{
			c[i][j] += a[i][k] * b[k][j];
			k++;

			matmul(a, b, c, row1, col1, row2, col2, 
					n);
		}

		k = 0;
		j++;
		matmul(a, b, c, row1, col1, row2, col2, n);
	}

	j = 0;
	i++;
	matmul(a, b, c, row1, col1, row2, col2, n);
}

// int ** matmul(int **a, int **b, int r1, int c1, int r2, int c2, int n)
// {
// 	int **c = new int *[n];
// 	for (int i = 0; i < n; ++i)
// 	{
// 		c[i] = new int[n];
// 	}
// 	memset(c, n);
// 	matmul_util(r1, c1, a, r2, cc2, b, c);
// 	return c;
// }

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false); 
    srand(time(0)); 
	int s;
	// clock_t start, stop;
	// long double duration;
	ofstream file1, file2, file3, file4;
	file1.open("dat1.txt");
	file2.open("dat2.txt");
	file3.open("dat3.txt");
	file4.open("dat4.txt");

	for (int n=1; n <= 2048; n*=2)
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

		auto start = high_resolution_clock::now();
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				for (int k = 0; k < n; ++k)
					c[i][j]+=a[i][k]*b[k][j];

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start).count();
		file1<<setprecision(10)<<duration<<endl;
		// display(c, n);

		// Optimization 1

		memset(c, n);
		start = high_resolution_clock::now();;
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
		stop = high_resolution_clock::now();;
		duration = duration_cast<microseconds>(stop - start).count();
		file2<<setprecision(10)<<duration<<endl;
		// display(c, n);

		// Optimization 2
		// $ lscpu | grep cache
		// L1d cache:           32K
		// L1i cache:           32K
		// L2 cache:            256K
		// L3 cache:            6144K

		memset(c, n);
		s = 64;
		start = high_resolution_clock::now();;
		for (int i = 0; i < n; i+=s)
			for (int j = 0; j < n; j+=s)
				for (int k = 0; k < n; k+=s)
					for (int i1 = 0; i1 < min(s, n); ++i1)
						for (int j1 = 0; j1 < min(s, n); ++j1)
							for (int k1 = 0; k1 < min(s, n); ++k1)
								c[i+i1][j+j1]+=a[i+i1][k+k1]*b[k+k1][j+j1];
		stop = high_resolution_clock::now();;
		duration = duration_cast<microseconds>(stop - start).count();
		file3<<setprecision(10)<<duration<<endl;
		// display(c, n);

		// Optimization 3

		memset(c, n);
		start = high_resolution_clock::now();;
		matmul(a, b, c, n, n, n, n, n);
		stop = high_resolution_clock::now();;
		duration = duration_cast<microseconds>(stop - start).count();
		file4<<setprecision(10)<<duration<<endl;
		// display(c, n);
	}
	file1.close();
	file2.close();
	file3.close();
	file4.close();
	return 0;
}