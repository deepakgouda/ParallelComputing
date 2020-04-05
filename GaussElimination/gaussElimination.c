#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 10
void display1D(int rank, float *arr, int n)
{
	printf("Process %d :\n", rank);
	for (int i = 0; i < n; i++)
	{
		printf("%.1f ", arr[i]);
	}
	printf("\n");
}

void display2D(int rank, float **arr, int m, int n)
{
	printf("Process %d :\n", rank);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%.1f ", arr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

float **allocate2D(int m, int n)
{
	float *data = (float *)malloc(m * n * sizeof(float));
	float **arr = (float **)malloc(m * sizeof(float *));
	for (int i = 0; i < m; i++)
	{
		arr[i] = &(data[i * n]);
	}
	return arr;
}

float initialize2D(float **arr, int m, int n)
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			// arr[i][j] = 2*((i)*n + (j+1));
			arr[i][j] = rand() % MAX;
		}
	}
}

float *allocate1D(int n)
{
	float *arr = (float *)malloc(n * sizeof(float));
	return arr;
}

void initialize1D(float *arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		arr[i] = rand() % MAX;
	}
}

int main(int argc, char **argv)
{
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(17);
	int m = 10, n = 10, p = world_size, count = m/p;

	float **a = allocate2D(m, n);
	initialize2D(a, m, n);
	display2D(rank, a, m, n);

	for (int row = 0; row < m; row++)
	{
		float *root = allocate1D(n);
		initialize1D(root, n);

		if(row%p == rank)
		{
			for (int j = n-1; j >= row; j--)
			{
				a[row][j] /= a[row][row];
			}

			for (int j = 0; j < n; j++)
			{
				root[j] = a[row][j];
			}
			
			if(row == 2)
			{
				printf("-------------------------\n");
				for (int j = 0; j < n; j++)
				{
					printf("%f ", root[j]);
				}
				printf("\n");
				printf("-------------------------\n");
			}

			for (int i = 0; i < p; i++)
			{
				if (i != rank)
				{					
					MPI_Ssend(
						root,
						n,
						MPI_FLOAT,
						i,
						17,
						MPI_COMM_WORLD);
				}
			}
		}
		else
		{
			MPI_Recv(
				root,
				n,
				MPI_FLOAT,
				row%p,
				17,
				MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
		}

		if(!rank)
		{
			printf("Row : %d\n", row);
			for (int j = 0; j < n; j++)
			{
				printf("%.1f ", root[j]);
			}
			printf("\n");
		}
		
		for (int local_row = row+1; local_row < m; local_row++)
		{
			if(local_row%p != rank)
				continue;
			int multiplier = a[local_row][row] / root[row];
			for (int j = row; j < n; j++)
			{
				a[local_row][j] -= multiplier * root[j];
			}
		}
	}
	
	// Finalize the MPI environment.
	MPI_Finalize();
	
	display2D(rank, a, m, n);
}