#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int commsize, rank, N = 1;
    double t;
    char *inbuf, *outbuf, *temp;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    inbuf = malloc( N*sizeof(char) );
    outbuf = malloc( N*sizeof(char) );
    
    t = MPI_Wtime();
    
    for (int i = 1; i < commsize; i++) {
        MPI_Sendrecv(outbuf, N, MPI_CHAR, (rank + 1) % commsize, 1, inbuf, NELEMS(inbuf), MPI_CHAR, (rank - 1 + commsize) % commsize, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        temp = outbuf;
        outbuf = inbuf;
        inbuf = temp;
    }
    
    t = MPI_Wtime() - t;

    if (rank == 0) printf("%dB time %.6f sec.\n", N, t);

    free(inbuf);
    free(outbuf);

    MPI_Finalize();
    return 0;
}
