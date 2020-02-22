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

    int n = atoi(argv[1]), count = n / world_size, recv, sum = 0;
    int *arr;

    double start, stop;

    start = MPI_Wtime();
    // Distribute data
    if (rank == 0)
    {

        arr = (int *)malloc(n * sizeof(int));

        for (int i = 0; i < n; i++)
        {
            arr[i] = i+1;
        }

        for (int i = 1; i < world_size; i++)
        {
            MPI_Ssend((arr + i * count), // data buffer
                        count,           // num of bytes
                        MPI_INT, 
                        i,               // destination
                        0,               // tag
                        MPI_COMM_WORLD);
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
        MPI_Recv(arr, 
                count, 
                MPI_INT, 
                0,                      // destination
                0, 
                MPI_COMM_WORLD, 
                MPI_STATUS_IGNORE);

        for (int i = 0; i < count; i++)
        {
            sum += arr[i];
        }
    }

    // Collection by sending to parent

    int x = 2;
    while (x <= world_size)
    {
        if (rank % x)
        {
            int dest = rank - x / 2;
            MPI_Ssend(&sum, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            break;
        }
        else
        {
            int src = rank + x / 2;
            MPI_Recv(&recv, 1, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // printf("Process %d received data %d from process %d\n", rank, recv, src);
            sum += recv;
        }
        x *= 2;
    }

    if (rank == 0)
    {
        // printf("Full Sum: %d\n", sum);
    }

    // Finalize the MPI environment.
    MPI_Finalize();
    stop = MPI_Wtime();

    if (rank == 0)
    {
        printf("%1.5f\n", (stop - start));
    }
}