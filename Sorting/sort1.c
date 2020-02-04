#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    *a = *a+*b;
    *b = *a-*b;
    *a = *a-*b;
}

void display(int *arr, int n)
{
    printf("Sorted array: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
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

    int n = 8, source, dest;
    int *arr;

    // Distribute data
    if (rank == 0)
    {

        arr = (int *)malloc(n * sizeof(int));

        for (int i = 0; i < n; i++)
        {
            arr[i] = n - i;
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = i%2; j < n-(i%2); j+=2)
            {
                dest = j/2;
                if(dest)
                {
                    int *sorted_arr;
                    // printf("Sent data to %d\n", dest);
                    MPI_Ssend((arr + j), 2, MPI_INT, dest, 0, MPI_COMM_WORLD);
                    // printf("Received data from %d\n", dest);
                    sorted_arr = (int *)malloc(2*sizeof(int));
                    MPI_Recv(sorted_arr, 2, MPI_INT, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    arr[j] = sorted_arr[0];
                    arr[j+1] = sorted_arr[1];
                }
                else
                {
                    if(arr[j] > arr[j+1])
                        swap(&arr[j], &arr[j+1]);
                }
            }
            // printf("%d : ",i);
            // display(arr, n);
        }
    }
    else
    {
        for (int i = 0; i < (rank==(world_size-1)?(n/2):n); i++)
        {
            arr = (int *)malloc(2 * sizeof(int));
            source = 0;
            MPI_Recv(arr, 2, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (arr[0] > arr[1])
                swap(&arr[0], &arr[1]);
            MPI_Ssend(arr, 2, MPI_INT, source, 0, MPI_COMM_WORLD);
        }
    }

    if(!rank)
    {
        display(arr, n);
    }
    // printf("Process %d done\n", rank);

    // Finalize the MPI environment.
    MPI_Finalize();
}