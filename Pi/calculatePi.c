/**
 * gcc -fopenmp calculatePi.c && ./a.out
**/
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

int main()
{
	int i, num_threads, max_threads = 8;
	long int num_steps, max_steps = 10000000;
	double x, pi, step, sum;

	double start, stop;
	printf("Threads\tValue\t\tError\t\t\tTime taken");
	num_steps = max_steps;
	for (num_threads = 1; num_threads < max_threads; num_threads++)
	{
		sum = 0.0;
		start = omp_get_wtime();
		omp_set_num_threads(num_threads);
		step = 1.0/(double) num_steps;
		#pragma omp parallel for reduction(+: sum)
		for(i = 0; i < num_steps; i++)
		{
			x = (i+0.5)*step;
			sum += 4.0/(1.0+x*x);
		}
		pi = step * sum;
		stop = omp_get_wtime();
		printf("\n%d \t", num_threads);
		printf("%1.10lf \t", pi);
		printf("%1.10lf e-3 \t", fabs(pi-PI)/PI*1000);
		printf("%1.7lf seconds\t", stop-start);
	}
	
	return 0;
}