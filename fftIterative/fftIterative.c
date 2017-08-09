#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <unistd.h>
#include "./../commons/commons.h"
#define EVEN 0
#define ODD 1

double *reverse(double *signal, int N)
{
}
Complex *fftIterative(double *signal, int N)
{
    int i, j = 1;

    for (i = 1; i < N; i += 2)
    {
        if (j > i)
        {
            swap(data[j - 1], data[i - 1]);
            swap(data[j], data[i]);
        }
        m = nn;
        while (m >= 2 && j > m)
        {
            j -= m;
            m >>= 1;
        }
        j += m;
    };
}