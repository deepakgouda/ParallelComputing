/**
 * Compile instructions : 
 * $ mpicc blockSplit.c -lm
 * $ mpiexec -o blockSplit -n <num_processes> blockSplit.c
 * 
 * References : http://www.hpcc.unn.ru/mskurs/ENG/DOC/pp07.pdf
 */

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
    int **arr = (int **)malloc(m * sizeof(int));
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

    int m = 8, n = 4, m1, n1, tag = 0;
    m1 = (int)(sqrt(world_size));
    n1 = (int)(sqrt(world_size));

    int **A = allocate2D(m1, n1);
    initialize2D(A, m1, n1, 1);

    int *x = allocate1D(n1);
    initialize1D(x, n1, 1);

    int *b = allocate1D(m1);
    initialize1D(b, m1, 0);

    // Split MPI_COMM_WORLD into m/sqrt(p) number of row
    // communicators and after computation of each matrix 
    // block vector multiplication reduce the vector along
    // row communicators to get the sum
    int color = rank/n, key = rank;
    MPI_Comm row_comm;
    MPI_Comm_split(
            MPI_COMM_WORLD,     // Communicator to split
            color,              // Group number
            key,                // Rank in group
            &row_comm);         // New communicator

    int row_rank, row_size;
    MPI_Comm_rank(row_comm, &row_rank);
    MPI_Comm_size(row_comm, &row_size);

    for (int i = 0; i < m1; i++)
    {
        for (int j = 0; j < n1; j++)
        {
            b[i] += A[i][j] * x[j];
        }
    }

    int *all_sum = allocate1D(m1);

    // Reducing the sum across row communicator
    MPI_Allreduce(
                b,       // send
                all_sum, // recv
                m1,      // count
                MPI_INT,
                MPI_SUM, // operation
                row_comm);

    printf("Row rank : %d\n", row_rank);
    for (int i = 0; i < m1; i++)
    {
        printf("%d : %d\n", i, all_sum[i]);
    }

    MPI_Comm_free(&row_comm);

    // Finalize the MPI environment.
    MPI_Finalize();
}