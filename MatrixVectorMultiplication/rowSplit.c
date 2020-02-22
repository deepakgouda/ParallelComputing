#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void display(int rank, int *arr, int n)
{
    printf("Process %d :\n", rank);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int **allocate2D(int m, int n)
{
    int *data = (int *)malloc(m * n * sizeof(int));
    int **arr = (int **)malloc(m * sizeof(int));
    for (int i = 0; i < m; i++)
    {
        arr[i] = &(data[i * n]);
    }
    return arr;
}

void initialize2D(int **arr, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            arr[i][j] = 1;
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

    int m = 8, n = 8;
    int *all_x = NULL;

    all_x = allocate1D(n);
    initialize1D(all_x, n, 0);

    int **A = allocate2D(m/world_size, n);
    initialize2D(A, m/world_size, n);

    int *x = allocate1D(n/world_size);
    initialize1D(x, n/world_size, 1);

    // Scatter data x to all processes
    MPI_Scatter(x,                  // Send data
                n / world_size,     // Send count
                MPI_INT, 
                all_x,              // Recv data
                n / world_size,     // Recv count
                MPI_INT, 
                0,                  // root
                MPI_COMM_WORLD);

    // Set up barrier for data scatter to be complete
    // MPI_Barrier(MPI_COMM_WORLD);

    // Gather data from all processes and store in all_x
    MPI_Allgather(x,                // Gather from
                  n/world_size,     // Count to gather
                  MPI_INT, 
                  all_x,            // Store in
                  n/world_size,     // Store count
                  MPI_INT, 
                  MPI_COMM_WORLD);

    int *b = allocate1D(m / world_size);
    initialize1D(b, m / world_size, 0);
    
    for (int i = 0; i < m / world_size; i++)
    {
        for (int j = 0; j < n; j++)
        {
            b[i] += A[i][j] * all_x[j];
        }
    }
    
    printf("Process %d : \n", rank);
    for (int i = 0; i < m/world_size; i++)
    {
        printf("%d : %d\n", i, b[i]);
    }
    
    // Finalize the MPI environment.
    MPI_Finalize();
}