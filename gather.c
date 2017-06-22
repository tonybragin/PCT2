#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int rank, commsize, N = 1;
    double t;
    char *msg;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    
    msg = malloc( N*sizeof(char) );

    t = MPI_Wtime();
    
    if (rank > 0) {
        MPI_Send(msg, N, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < commsize; i++) {
            MPI_Recv(msg, N, MPI_CHAR, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    
    t = MPI_Wtime() - t;
    
    if (rank == 0) printf("%dB time %.6f sec.\n", N, t);

    free(msg);
    
    MPI_Finalize();
    return 0;
}
