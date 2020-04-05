/**
 * gcc -fopenmp calculatePi.c && ./a.out
**/
#include <stdio.h>
#include <omp.h>

int main()
{
	int i;
	long int num_steps = 100000000;
	double x, pi, step, sum = 0.0;
	step = 1.0/(double) num_steps;

	double start, stop;
	start = omp_get_wtime();
	#pragma omp parallel for reduction(+: sum)
	for(i = 0; i < num_steps; i++)
	{
		x = (i+0.5)*step;
		sum += 4.0/(1.0+x*x);
	}
	pi = step * sum;
	stop = omp_get_wtime();
	printf("Value of PI = %1.10lf\n", pi);
	printf("Time taken = %1.7lf seconds\n", stop-start);
	return 0;
}