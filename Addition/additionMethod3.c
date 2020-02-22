#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

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

    int n = atoi(argv[1]);
    int count = n / world_size, recv, sum = 0;
    int *arr;

    double start, stop;
    // Distribute data
    start = MPI_Wtime();
    if (rank == 0)
    {
        arr = (int *)malloc(n * sizeof(int));

        for (int i = 0; i < n; i++)
        {
            arr[i] = i;
        }

        for (int i = 1; i < world_size; i++)
        {
            MPI_Ssend((arr + i * count), count, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        sum = 0;
        for (int i = 0; i < count; i++)
        {
            sum += arr[i];
        }
    }
    else
    {
        sum = 0;
        arr = (int *)malloc(count * sizeof(int));
        MPI_Recv(arr, count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < count; i++)
        {
            sum += arr[i];
        }
    }

    // Addition with MPI_Reduce

    int fullSum = 0;
    MPI_Reduce(&sum, &fullSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        // printf("Final sum: %d\n", fullSum);
    }

        // Finalize the MPI environment.
    MPI_Finalize();
    stop = MPI_Wtime();

    if (rank == 0)
    {
        printf("%1.5f\n", (stop - start));
    }
}