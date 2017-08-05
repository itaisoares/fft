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
        signalOddEven[EVEN][i / 2] = signal[i];
        signalOddEven[ODD][i / 2] = signal[i + 1];
    }

    return signalOddEven;
}

void freeVectors(double **signal)
{
    free(signal[0]);
    free(signal[1]);
    free(signal);
}

Complex *fftCore(double *signal, int N)
{
    double **splittedSignal;
    int k, i;
    Complex *X, *XEven, *XOdd;
    /*double pi2n = (M_PI * 2) / N;*/
    double pi2n = (8 * atan(1)) / N;
    X = allocate_bin_vector(N, true);

    if (N == 2)
    {
        X[0].re = signal[0] + signal[1];
        X[1].re = signal[0] - signal[1];
        return X;
    }
    else
    {
        splittedSignal = split(signal, N);
        XEven = fftCore(splittedSignal[EVEN], N / 2);
        XOdd = fftCore(splittedSignal[ODD], N / 2);
    }
    /*X_k = XEven_k + e^{-i2pik/N} * XOdd_k */
    /*X_k = XEven_{k-N/2} - e^{-i2pi(k-N/2)/N} * XOdd_{k-N/2} */
    for (i = 0; i < N; i++)
    {
        X[i].re = XEven[i % (N / 2)].re + XOdd[i % (N / 2)].re * cos(pi2n * i) + XOdd[i % (N / 2)].im * sin(pi2n * i);
        X[i].im = XEven[i % (N / 2)].im + XOdd[i % (N / 2)].im * cos(pi2n * i) - XOdd[i % (N / 2)].re * sin(pi2n * i);
    }

    XEven = XOdd = NULL;
    freeVectors(splittedSignal);
    return X;
}

Complex *fftRecursive(double *signal, int N)
{
    return fftCore(signal, N);
}
