#include <mpi.h>
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

    int m = atoi(argv[1]), n = atoi(argv[1]), tag = 0;

    int **A = allocate2D(m, n / world_size);
    initialize2D(A, m, n / world_size, 1);

    int *x = allocate1D(n / world_size);
    initialize1D(x, n / world_size, 1);

    int *b = allocate1D(m);
    initialize1D(b, m, 0);

    double start, stop;

    start = MPI_Wtime();

    for (int i = 0; i < m ; i++)
    {
        for (int j = 0; j < n / world_size; j++)
        {
            b[i] += A[i][j] * x[j];
        }
    }

	int *all_sum = allocate1D(m);

	MPI_Reduce(
            b,     		// send
            all_sum,	// recv
            m, 			// count
            MPI_INT, 
            MPI_SUM, 	// operation
            0,          // root
            MPI_COMM_WORLD);
				
    if(rank == 0)
    {
        for (int i = 0; i < m; i++)
        {
            // printf("%d : %d\n", i, all_sum[i]);
        }
    }
    // Finalize the MPI environment.
    MPI_Finalize();
    stop = MPI_Wtime();
    if (!rank)
    {
        printf("%1.5f\n", (stop - start));
    }
}