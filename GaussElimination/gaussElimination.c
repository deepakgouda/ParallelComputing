#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void display1D(int rank, int *arr, int n)
{
	printf("Process %d :\n", rank);
	for (int i = 0; i < n; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void display2D(int rank, int **arr, int m, int n)
{
	printf("Process %d :\n", rank);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}
}

int **allocate2D(int m, int n)
{
	int *data = (int *)malloc(m * n * sizeof(int));
	int **arr = (int **)malloc(m * sizeof(int *));
	for (int i = 0; i < m; i++)
	{
		arr[i] = &(data[i * n]);
	}
	return arr;
}

void initialize2D(int **arr, int m, int n, int val)
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			arr[i][j] = val;
		}
	}
}

int *allocate1D(int n)
{
	int *arr = (int *)malloc(n * sizeof(int));
	return arr;
}

void initialize1D(int *arr, int n, int val)
{
	for (int i = 0; i < n; i++)
	{
		arr[i] = val;
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

	int m = 20, n = 20, p = world_size, local_row = -1, count = m/p;

	int **a = allocate2D(m, n);
	initialize2D(a, count, n, 1);

	for (int row = 0; row < m; row++)
	{
		if (row%p == 0)
		{
			local_row = (local_row+1)%count;
		}

		if(row%p == rank)
		{
			for (int j = row; j < n; j++)
			{
				a[local_row][j] /= a[local_row][0];
			}

			for (int i = 0; i < p; i++)
			{
				if (i != rank)
				{
					MPI_Ssend(
						a[local_row],
						n,
						MPI_INT,
						i,
						17,
						MPI_COMM_WORLD);
				}
				
			}
		}
		else
		{
			int *root = allocate1D(n);
			initialize1D(root, n, 0);
			MPI_Recv(
				root,
				n,
				MPI_INT,
				row%p,
				17,
				MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);

			int multiplier = a[local_row][0] / root[0];
			for (int j = row; j < n; j++)
			{
				a[local_row][j] = multiplier * root[j];
			}
		}
	}
	
	// Finalize the MPI environment.
	MPI_Finalize();

	// for (int i = 0; i < p; i++)
	// {
	// 	display2D(rank, a, count, n);
	// }
}