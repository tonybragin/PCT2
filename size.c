#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float func(float a, float b, float c, int proc)
{
    b = proc*2*a;
    float d = powf(b, 2) + a*c;
    
    return (-b + powf(d, 0.5)) / 2*a;
}

int main()
{
    float a = 4, b = 2*a, c = 8*1E9;
    
    for (int i = 1; i <= 8; i++) {
        printf("N%d = %.2f\n", i, func(a, b, c, i));
    }
    
    printf("\n18 = %.2f", func(a, b, c*18, 18));
}
