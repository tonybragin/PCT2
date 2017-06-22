#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int commsize, rank, countr = 0;
    double t;
    char *msg;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    MPI_Request reqs[commsize];
    MPI_Status stats[commsize];
    
    msg = malloc( 1024*sizeof(char) );
        
    t = MPI_Wtime();
    
    for (int i = 0; i < commsize; i++) {
        if (i != rank) {
            MPI_Isend(msg, 1024, MPI_CHAR, i, 1, MPI_COMM_WORLD, &reqs[countr++]);
        }
    }
    for (int i = 0; i < commsize; i++) {
        if (i != rank) {
            MPI_Recv(msg, 1024, MPI_CHAR, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    
    MPI_Waitall(countr, reqs, stats);
    
    t = MPI_Wtime() - t;
    
    printf("time %.6f sec.\n", t);
    
    MPI_Finalize();
    return 0;
}
