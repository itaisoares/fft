#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <unistd.h>
#include "./../commons/commons.h"
#define EVEN 0
#define ODD 1

double **split(double *signal, int N)
{
    double **signalOddEven; /*, *signalEven;*/
    int i;

    signalOddEven = (double **)malloc(2 * sizeof(double *));
    if (signalOddEven == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    signalOddEven[EVEN] = (double *)malloc(N / 2 * sizeof(double));
    signalOddEven[ODD] = (double *)malloc(N / 2 * sizeof(double));
    if (signalOddEven[0] == NULL || signalOddEven[1] == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    for (i = 0; i < N - 1; i += 2)
    {
        signalOddEven[EVEN][i] = signal[i];
        signalOddEven[ODD][i + 1] = signal[i + 1];
    }

    return signalOddEven;
}

Complex *fftCore(double *signal, int N)
{
    double **splittedSignal;
    int k;
    Complex *X, *XEven, *XOdd;
    /*double pi2n = (M_PI * 2) / N;*/
    double pi2n = (8 * atan(1)) / N;
    X = allocate_bin_vector(N, true);

    if (N == 1)
    {
        X[0].re = signal[0];
        X[0].im = 0;
        return X;
    }
    else
    {
        splittedSignal = split(signal, N);
        XEven = fftCore(splittedSignal[EVEN], N / 2);
        XOdd = fftCore(splittedSignal[ODD], N / 2);
    }
    for (k = 0; k < N / 2; k++)
    {
        /*X_k = XEven_k + e^{-i2pik/N} * XOdd_k */
        X[k].re = XEven[k].re + cos(pi2n * k) * XOdd[k].re + sin(pi2n * k) * XOdd[k].im;
        X[k].im = XEven[k].im + cos(pi2n * k) * XOdd[k].im - sin(pi2n * k) * XOdd[k].re;
    }
    for (k = N / 2; k < N - 1; k++)
    {
        int n2 = N / 2;
        /*X_k = XEven_{k-N/2} - e^{-i2pi(k-N/2)/N} * XOdd_{k-N/2} */
        X[k].re = XEven[k - n2].re - cos(pi2n * (k - n2)) * XOdd[k - n2].re - sin(pi2n * (k - n2)) * XOdd[k - n2].im;
        X[k].im = XEven[k - n2].im - cos(pi2n * (k - n2)) * XOdd[k - n2].im + sin(pi2n * (k - n2)) * XOdd[k - n2].re;
    }

    return X;
}

Complex *fftRecursive(double *signal, int N)
{
    return fftCore(signal, N);
}
