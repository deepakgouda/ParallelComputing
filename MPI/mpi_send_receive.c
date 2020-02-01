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
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int data = 10;
  int count = 1;
  int destination = (world_rank + 1) % world_size;
  int tag = 0;
  MPI_Ssend(&data, count, MPI_INT, destination, tag, MPI_COMM_WORLD); // Stuck in deadlock
  printf("Processor %d : Sent data %d to processor %d\n", world_rank, data, destination);

  int source = (world_rank==0?world_size-1:world_rank-1) % world_size;
  MPI_Recv(&data, count, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  printf("Processor %d : Received data %d from processor %d\n", world_rank, data, source);

    // MPI_Reduce();
    // MPI_WorldClock()

  MPI_Finalize();
}
