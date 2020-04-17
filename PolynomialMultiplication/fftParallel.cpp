/**
 * g++ -pthread -fopenmp fftParallel.cpp && ./a.out
 **/

#include <iostream>
#include <complex>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <omp.h>
#include <pthread.h>

#define MAX 10
#define PI 3.14159265358979323846

#define debug(x) cerr << #x << " = " << x << endl;
#define debugComp(x) printf("%lf + i%lf\n", real(x), imag(x));

using namespace std;

typedef struct Block
{
	complex <double> *coefficient;
	complex <double> *coefficientFFT;
	int n;
	bool inverse = false;
	public:
	Block()
	{}
	
	Block(int n, bool inverse)
	{
		this -> n = n;
		this -> coefficient = new complex<double>[n]();
		this -> inverse = inverse;
	}
}Block;

// Initializes array with random values
void initialize(complex<double> *coefficients, int n)
{
	for (int i = 0; i < n; i++)
	{
		coefficients[i] = rand() % MAX;
	}
}

// Displays array
void display(complex<double> *coefficients, int n)
{
	for (int i = 0; i < n; i++)
	{
		std::cout << coefficients[i] << " ";
	}
	std::cout << endl;
}

// Compute fft of the coefficient vector recursively
void *fft(void *block_h)
{
	Block *block = (Block *)block_h;
	// if(block -> inverse)
	// 	printf("True\n");
	int n = block -> n;
	block -> coefficientFFT = new complex<double>[n]();

	if(n == 1)
	{
		block -> coefficientFFT[0] = block -> coefficient[0];
		return nullptr;
	}
	
	// Obtain even and odd coefficients
	Block *even = new Block(n / 2, block -> inverse);
	Block *odd = new Block(n / 2, block -> inverse);

	for (int i = 0; i < n/2; i++)
	{
		even -> coefficient[i] = block -> coefficient[2 * i];
		odd -> coefficient[i] = block -> coefficient[2 * i + 1];
	}

	// If we need to compute Inverse FFT, the flag calculates the complex conjugate
	double flag = (block -> inverse)?-1:1;
	complex<double> w = 1, theta = 2*PI/n, wi;
	wi = cos(theta) + 1i * sin(theta) * (double)flag;

	pthread_t threads[2];
	for (int i = 0; i < 2; i++)
	{
		if(i)
			pthread_create(&threads[i], NULL, fft, (void *)odd);
		else
			pthread_create(&threads[i], NULL, fft, (void *)even);
	}

	for (int i = 0; i < 2; i++)
	{
		pthread_join(threads[i], NULL);
	}

	for (int i = 0; i < n/2; i++)
	{
		block -> coefficientFFT[i] = (even -> coefficientFFT[i] + w * odd -> coefficientFFT[i]);
		block -> coefficientFFT[n / 2 + i] = (even -> coefficientFFT[i] - w * odd -> coefficientFFT[i]);
		w*=wi;
	}
	return nullptr;
}

int main(int argc, char *argv[])
{
	int n = atoi(argv[1]);
	double start, stop;

	complex<double> *coefficient1 = new complex<double>[2*n]();
	complex<double> *coefficient2 = new complex<double>[2*n]();

	srand(7);
	cout << fixed << setprecision(8);

	// Initialize polynomial coefficients	
	initialize(coefficient1, n);
	initialize(coefficient2, n);

	// Create array of Blocks to store polynomials
	Block *poly = new Block[2];
	
	poly[0].coefficient = coefficient1;
	poly[0].n = 2*n;
	poly[1].coefficient = coefficient2;
	poly[1].n = 2*n;

	complex<double> *resultFFT = new complex<double>[2*n]();

	start = omp_get_wtime();
	// Evaluate polynomial1 and polynomial2 at 2*n points using FFT
	pthread_t threads[2];
	for (int i = 0; i < 2; i++)
	{
		pthread_create(&threads[i], NULL, fft, (void *)&poly[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		pthread_join(threads[i], NULL);
	}

	// display(poly[0].coefficient, n);
	// display(poly[1].coefficient, n);

	// Elementwise multiplication of polynomials
	for (int i = 0; i < 2*n; i++)
	{
		resultFFT[i] = poly[0].coefficientFFT[i] * poly[1].coefficientFFT[i];
	}

	// Perform inverse FFT on the pointwise multiplied values by setting inverse = true
	Block *result = new Block(2*n, true);
	result -> coefficient = resultFFT;
	fft((void *)result);
	
	// Division by N after inverse FFT
	for (int i = 0; i < 2*n; i++)
	{
		result -> coefficientFFT[i]/=(2*n);
	}

	stop = omp_get_wtime();
	cout<<stop - start<<endl;
	// display(result->coefficientFFT, 2 * n - 1);
	return 0;
}
