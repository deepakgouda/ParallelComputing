/**
 * g++ -pthread -fopenmp fft.cpp && ./a.out
 **/

#include <iostream>
#include <complex>
#include <cmath>
#include <stdlib.h>
#include <iomanip>
#include <omp.h>
#include <pthread.h>

#define MAX 10
#define PI 3.14159265358979323846

#define debug(x) cerr << #x << " = " << x << endl;

using namespace std;

// Initializes array with random values
void initialize(int *coefficients, int n)
{
	for (int i = 0; i < n; i++)
	{
		coefficients[i] = rand() % MAX;
	}
}

// Compute fft of the coefficient vector recursively
complex<double> *fft(int *coefficient, int n)
{
	complex<double> *result = new complex<double>[n]();

	if(n == 1)
	{
		result[0] = (double)coefficient[0];
		return result;
	}
	
	// Obtain even and odd coefficients
	int *even = new int[n/2]();
	int *odd = new int[n/2]();

	for (int i = 0; i < n/2; i++)
	{
		even[i] = coefficient[2*i];
		odd[i] = coefficient[2*i + 1];
	}
	
	complex <double> w = 1, wi = exp((double)2*1i*PI/(double)n);
	// Recursively compute even and odd fft
	complex <double> *res0 = fft(even, n/2);
	complex <double> *res1 = fft(odd, n/2);

	for (int i = 0; i < n/2; i++)
	{
		result[i] = res0[i] + w*res1[i];
		result[n/2 + i] = res0[i] - w*res1[i];
		w*=wi;
	}
	return result;
}

// Displays array
void display(int *coefficients, int n)
{
	for (int i = 0; i < n; i++)
	{
		std::cout << coefficients[i] << ", ";
	}
	std::cout << endl;
}

int main(int argc, char *argv[])
{
	int n = 8;

	int *coefficient1 = new int[2*n]();
	int *coefficient2 = new int[2*n]();

	srand(7);
	cout << fixed << setprecision(3);
	
	initialize(coefficient1, n);
	initialize(coefficient2, n);

	complex<double> *coeff1FFT = new complex<double>[2*n]();
	complex<double> *coeff2FFT = new complex<double>[2*n]();
	complex<double> *resultFFT = new complex<double>[2*n]();

	// Evaluate polynomial1 and polynomial2 at 2*n points using FFT
	coeff1FFT = fft(coefficient1, 2 * n);
	coeff2FFT = fft(coefficient2, 2*n);

	// Elementwise multiplication of polynomials
	for (int i = 0; i < 2*n; i++)
	{
		resultFFT[i] = coeff1FFT[i] * coeff2FFT[i];
	}

	// TODO : Inverse FFT to compute coefficients
	// complex<double> *result = new complex<double>[n]();
	
	return 0;
}

// void VanderMond(complex<double> **V, complex<double> **invV, int n)
// {
// 	double val;
// 	for (int j = 0; j < n; j++)
// 	{
// 		for (int k = 0; k < n; k++)
// 		{
// 			val = 2 * j * k * PI / n;
// 			V[j][k] = std::cos(val) + 1i * std::sin(val); // exp(i*2*pi/n*j*k)
// 			invV[j][k] = std::cos(val) - 1i * std::sin(val);
// 		}
// 	}
// }

// complex<double> *V[n];
// complex<double> *invV[n];

// for (int i = 0; i < n; i++)
// {
// 	V[i] = new complex<double>[n]();
// 	invV[i] = new complex<double>[n]();
// }

// VanderMond(V, invV, n);