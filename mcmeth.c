#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

double getrand()
{
    return (double)rand() / RAND_MAX;
}

double func(double x, double y)
{
    return x / pow(y, 2);
}

const int n = 10000000;

//Интегрирование методом Монте-Карло
int main(int argc, char **argv)
{
    int rank, commsize;
    double t;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    
    srand(rank);
    int in = 0;
    double s = 0;
    
    t = MPI_Wtime();

    for (int i = rank; i < n; i += commsize) {
        double x = getrand();
        double y = getrand() * 5;
        if (y > 2 && y < 5) {
            in++;
            s += func(x, y);
        }
    }
    
    int gin = 0;
    MPI_Reduce(&in, &gin, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double gsum = 0.0;
    MPI_Reduce(&s, &gsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    t = MPI_Wtime() - t;
    
    double gt;
    MPI_Reduce(&t, &gt, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double v =  gin / n;
        double res = v * gsum / gin;
        printf("Result: %.12f, n: %d, time: %.6f sec.\n", res, n, gt);
    }
    
    MPI_Finalize();
    return 0;
}
