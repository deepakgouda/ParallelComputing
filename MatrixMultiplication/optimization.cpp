/**
Code to check optimizations in matrix multiplication.

Optimization 1 : Interchange inner loops, so that cache
loads the entire row in each interation.

Optimization 2 : Block multiplication, with block size 
depending on cache size.

Optimization 3 : Recursive method.
*/

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <time.h>
#include <math.h>

#include <chrono>

#define debug(x) cerr << #x << " = " << x << endl;
#define MAX 5

using namespace std;
using namespace std::chrono;

void display(int *a[], int n)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
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

void matmul_divide(int **a, int **b, int **c, int i1, int j1,
				   int i2, int j2, int row, int col, int n)
{
	if (n <= 32)
	{
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				for (int k = 0; k < n; ++k)
					c[row + i][col + j] += a[i1 + i][j1 + k] * b[i2 + k][j2 + j];
		return;
	}

	matmul_divide(a, b, c, i1, j1, i2, j2, row, col, n / 2);				 // A*A1
	matmul_divide(a, b, c, i1, j1, i2, j2 + n / 2, row, col + n / 2, n / 2); //A*B1

	matmul_divide(a, b, c, i1, j1 + n / 2, i2 + n / 2, j2, row, col, n / 2);				 // A*C1
	matmul_divide(a, b, c, i1, j1 + n / 2, i2 + n / 2, j2 + n / 2, row, col + n / 2, n / 2); // B*D1

	matmul_divide(a, b, c, i1 + n / 2, j1, i2, j2, row + n / 2, col, n / 2);				 // C*A1
	matmul_divide(a, b, c, i1 + n / 2, j1, i2, j2 + n / 2, row + n / 2, col + n / 2, n / 2); // C*B1

	matmul_divide(a, b, c, i1 + n / 2, j1 + n / 2, i2 + n / 2, j2, row + n / 2, col, n / 2);				 // D*C1
	matmul_divide(a, b, c, i1 + n / 2, j1 + n / 2, i2 + n / 2, j2 + n / 2, row + n / 2, col + n / 2, n / 2); // D*D1
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

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);
	srand(time(0));
	int s;
	ofstream file0, file1, file2, file3, file4;
	file0.open("dat0.txt", ios_base::app);
	file1.open("dat1.txt", ios_base::app);
	file2.open("dat2.txt", ios_base::app);
	file3.open("dat3.txt", ios_base::app);
	file4.open("dat4.txt", ios_base::app);

	for (int n = 1; n <= 4096; n *= 2)
	{
		int **a = new int *[n];
		int **b = new int *[n];
		int **c = new int *[n];
		for (int i = 0; i < n; ++i)
			a[i] = new int[n];
		for (int i = 0; i < n; ++i)
			b[i] = new int[n];
		for (int i = 0; i < n; ++i)
			c[i] = new int[n];

		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
			{
				a[i][j] = rand() % MAX;
				b[i][j] = rand() % MAX;
			}
		memset(c, n);

		// Theoretical estimates
		// CPU MHz : 2100.212 
		// CPU max MHz : 3800.0000 
		// CPU min MHz : 800.0000
		long double totalOps = 2*n*n*n;
		long double time = totalOps / (800 * 1e6);
		file0 << time << endl;

		auto start = high_resolution_clock::now();
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				for (int k = 0; k < n; ++k)
					c[i][j] += a[i][k] * b[k][j];

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start).count();
		// display(c, n);
		file1 << duration << endl;

		// Optimization 1

		memset(c, n);
		start = high_resolution_clock::now();
		for (int i = 0; i < n; ++i)
			for (int k = 0; k < n; ++k)
				for (int j = 0; j < n; ++j)
					c[i][j] += a[i][k] * b[k][j];
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start).count();
		// display(c, n);
		file2 << duration << endl;		

		// Optimization 2
		// $ lscpu | grep cache
		// L1d cache:           32K
		// L1i cache:           32K
		// L2 cache:            256K
		// L3 cache:            6144K

		memset(c, n);
		s = 64;
		start = high_resolution_clock::now();
		for (int i = 0; i < n; i += s)
			for (int j = 0; j < n; j += s)
				for (int k = 0; k < n; k += s)
					for (int i1 = 0; i1 < min(s, n); ++i1)
						for (int j1 = 0; j1 < min(s, n); ++j1)
							for (int k1 = 0; k1 < min(s, n); ++k1)
								c[i + i1][j + j1] += a[i + i1][k + k1] * b[k + k1][j + j1];
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start).count();
		// display(c, n);
		file3 << duration << endl;

		// Optimization 3 : Recursion

		memset(c, n);
		start = high_resolution_clock::now();
		matmul_divide(a, b, c, 0, 0, 0, 0, 0, 0, n);
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start).count();
		// display(c, n);
		file4 << duration << endl;
	}
	file1.close();
	file2.close();
	file3.close();
	file4.close();
	return 0;
}