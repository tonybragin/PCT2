#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv)
{
    int rank, commsize;
    double t;
    char *outbuf, *inbuf;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    inbuf = malloc( commsize*sizeof(char) );
    outbuf = malloc( commsize*sizeof(char) );
    
    sprintf(outbuf, "%d", rank);
    
    t = MPI_Wtime();
    
    for (int k = 0; k <= log2(commsize - 1); k++) {
        int x = pow(2, k);
        MPI_Sendrecv(outbuf, sizeof(outbuf), MPI_CHAR, rank ^ x, 1, inbuf, sizeof(inbuf), MPI_CHAR, rank ^ x, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if ((rank ^ x) > rank)
            sprintf(outbuf, "%s%s", outbuf, inbuf);
        else sprintf(outbuf, "%s%s", inbuf, outbuf);
        
    }
    
    t = MPI_Wtime() - t;
    
    double gt;
    MPI_Reduce(&t, &gt, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (rank == 0) printf("msg = %s time %.6f sec.\n", outbuf, gt);
    
    free(inbuf);
    free(outbuf);
    
    MPI_Finalize();
    return 0;
}
