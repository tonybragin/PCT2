#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

double func(double x)
{
    return sin(x + 2) / (0.4 + cos(x));
}

const double a = -1.0;
const double b = 1.0;
const double eps = 1E-6;
const int n0 = 1000000;

//Параллельный алгоритм интегрирования методом средних прямоугольников
int main(int argc, char **argv)
{
    int commsize, rank;
    double t;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    t = MPI_Wtime();

    int n = n0, k;
    double sq[2], delta = 1;
    
    for (k = 0; delta > eps; n *= 2, k ^= 1) {
        int points_per_proc = n / commsize;
        int lb = rank * points_per_proc;
        int ub = (rank == commsize - 1) ? (n - 1) : (lb + points_per_proc - 1);
        
        double h = (b - a) / n;
        double s = 0.0;
        
        for (int i = lb; i <= ub; i++)
            s += func(a + h * (i + 0.5));
        
        MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        sq[k] *= h;
        if (n > n0)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
    }
    
    t = MPI_Wtime() - t;
    double gt;
    MPI_Reduce(&t, &gt, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("Result Pi: %.12f; n %d\n time: %.6f sec.\n", sq[k] * sq[k], n / 2, gt);
    }
    
    MPI_Finalize();
    return 0;
}
