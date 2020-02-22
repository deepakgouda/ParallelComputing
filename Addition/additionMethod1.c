// mpicc mpi_send_receive.c
// mpiexec -n 4 a.out
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start, stop;

    int *arr;
    int n = atoi(argv[1]), p = world_size, count = n / p, tag = 0;

    start = MPI_Wtime();
    if(rank == 0)
    {
        // Allocating and initializing array
        arr = (int *)malloc(n*sizeof(int));
        for (int i = 0; i < n; i++)
        {
            arr[i] = i + 1;
        }
        
        for (int i = 1; i < p; i++)
        {
            MPI_Ssend(
                arr+i*count,   // array + offset
                count,         // number of values
                MPI_INT,       // type
                i,             // destination
                tag, 
                MPI_COMM_WORLD);
        }
        int sum = 0;

        for (int i = 0; i < count; i++)
        {
            sum+=arr[i];
        }

        // Receive values from processes
        int recv;
        for (int i = 1; i < p; i++)
        {
            MPI_Recv(
                &recv,          // receive at this address
                1,              // num of bytes to receive
                MPI_INT,
                i,              // receive from
                tag,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
            sum+=recv;
        }
        // printf("Sum = %d\n", sum);
    }
    else
    {
        int source = 0;
        arr = (int *)malloc(count*sizeof(int));
        MPI_Recv(
            arr,                    // receive at this address
            count,                  // num of bytes to receive
            MPI_INT, 
            source, 
            tag, 
            MPI_COMM_WORLD, 
            MPI_STATUS_IGNORE);

        // Sum n numbers
        int sum  = 0;
        for (int i = 0; i < count; i++)
        {
            sum+=arr[i];
        }

        // Send data back to parent process
        MPI_Ssend(
            &sum,             // value
            1,                // number of values
            MPI_INT,          // type
            source,           // back to source
            tag,
            MPI_COMM_WORLD);
    }
    MPI_Finalize();
    stop = MPI_Wtime();

    if(rank == 0)
    {
        printf("%1.5f\n", (stop-start));
    }
}
