/**
 * g++ fft.cpp && ./a.out
 **/

#include <iostream>
#include <complex>
#include <cmath>
#include <stdlib.h>
#include <iomanip>

#define MAX 10
#define PI 3.14159265358979323846

#define debug(x) cerr << #x << " = " << x << endl;

using namespace std;

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
		std::cout << coefficients[i] << ", ";
	}
	std::cout << endl;
}

// Compute fft of the coefficient vector recursively
complex<double> *fft(complex<double> *coefficient, int n, bool inverse=false)
{
	complex<double> *result = new complex<double>[n]();
	if(n == 1)
	{
		result[0] = coefficient[0];
		return result;
	}
	
	// Obtain even and odd coefficients
	complex<double> *even = new complex<double>[n/2]();
	complex<double> *odd = new complex<double>[n/2]();

	for (int i = 0; i < n/2; i++)
	{
		even[i] = coefficient[2 * i];
		odd[i] = coefficient[2 * i + 1];
	}
	
	// If we need to compute Inverse FFT, the flag calculates the complex conjugate
	double flag = (inverse)?-1:1;
	complex<double> w = 1, wi = exp(flag*(double)2*1i*PI/(double)n);

	// Recursively compute even and odd fft
	complex<double> *res0 = fft(even, n/2, inverse);
	complex<double> *res1 = fft(odd, n/2, inverse);

	for (int i = 0; i < n/2; i++)
	{
		result[i] = (res0[i] + w*res1[i]);
		result[n/2 + i] = (res0[i] - w*res1[i]);
		w*=wi;
	}
	return result;
}

int main(int argc, char *argv[])
{
	int n = 16;

	complex<double> *coefficient1 = new complex<double>[2*n]();
	complex<double> *coefficient2 = new complex<double>[2*n]();

	srand(7);
	cout << fixed << setprecision(0);
	
	initialize(coefficient1, n);
	initialize(coefficient2, n);

	complex<double> *coeff1FFT = new complex<double>[2*n]();
	complex<double> *coeff2FFT = new complex<double>[2*n]();
	complex<double> *resultFFT = new complex<double>[2*n]();

	// Evaluate polynomial1 and polynomial2 at 2*n points using FFT
	coeff1FFT = fft(coefficient1, 2 * n);
	coeff2FFT = fft(coefficient2, 2 * n);

	// Elementwise multiplication of polynomials
	for (int i = 0; i < 2*n; i++)
	{
		resultFFT[i] = coeff1FFT[i] * coeff2FFT[i];
	}

	complex<double> *result = new complex<double>[n]();
	result = fft(resultFFT, 2*n, true);
	
	display(coefficient1, n);
	display(coefficient2, n);

	for (int i = 0; i < 2*n; i++)
	{
		result[i]/=(2*n);
	}

	display(result, 2*n-1);
	return 0;
}
