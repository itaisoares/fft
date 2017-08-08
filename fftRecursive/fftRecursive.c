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

Complex **splitComplex(Complex *signal, int N)
{
    Complex **signalOddEven; /*, *signalEven;*/
    int i;

    signalOddEven = (Complex **)malloc(2 * sizeof(Complex *));
    if (signalOddEven == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    signalOddEven[EVEN] = (Complex *)malloc(N / 2 * sizeof(Complex));
    signalOddEven[ODD] = (Complex *)malloc(N / 2 * sizeof(Complex));
    if (signalOddEven[0] == NULL || signalOddEven[1] == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    for (i = 0; i < N - 1; i += 2)
    {
        signalOddEven[EVEN][i / 2].re = signal[i].re;
        signalOddEven[EVEN][i / 2].im = signal[i].im;
        signalOddEven[ODD][i / 2].re = signal[i + 1].re;
        signalOddEven[ODD][i / 2].im = signal[i + 1].im;
    }

    return signalOddEven;
}

void freeVectors(double **signal)
{
    free(signal[0]);
    free(signal[1]);
    free(signal);
}

void freeComplexVectors(Complex **signal)
{
    free(signal[0]);
    free(signal[1]);
    free(signal);
}

Complex *fftCore(double *signal, int N)
{
    double **splittedSignal;
    int k;
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
    for (k = 0; k < N; k++)
    {
        X[k].re = XEven[k % (N / 2)].re + XOdd[k % (N / 2)].re * cos(pi2n * k) + XOdd[k % (N / 2)].im * sin(pi2n * k);
        X[k].im = XEven[k % (N / 2)].im + XOdd[k % (N / 2)].im * cos(pi2n * k) - XOdd[k % (N / 2)].re * sin(pi2n * k);
    }
    /* verificar erro */
    // for (k = 0; k < N / 2; k++)
    // {
    //     X[k].re = XEven[k].re + cos(pi2n * k) * XOdd[k].re + sin(pi2n * k) * XOdd[k].im;
    //     X[k].im = XEven[k].im + cos(pi2n * k) * XOdd[k].im - sin(pi2n * k) * XOdd[k].re;
    // }
    // for (k = N / 2; k < N - 1; k++)
    // {
    //     int n2 = N / 2;
    //     X[k].re = XEven[k % n2].re - (cos(pi2n * (k % n2)) * XOdd[k % n2].re + sin(pi2n * (k % n2)) * XOdd[k % n2].im);
    //     X[k].im = XEven[k % n2].im - (cos(pi2n * (k % n2)) * XOdd[k % n2].im - sin(pi2n * (k % n2)) * XOdd[k % n2].re);
    // }

    XEven = XOdd = NULL;
    freeVectors(splittedSignal);
    return X;
}

Complex *ifftCore(Complex *X, int N)
{
    Complex **splittedBins;
    int k;
    Complex *signal, *signalEven, *signalOdd;
    /*double pi2n = (M_PI * 2) / N;*/
    double pi2n = (8 * atan(1)) / N;
    signal = allocate_bin_vector(N, true);

    if (N == 2)
    {
        signal[0].re = X[0].re - X[1].re;
        signal[0].im = X[0].im - X[1].im;
        signal[1].re = X[0].re + X[1].re;
        signal[1].im = X[0].im + X[1].im;
        return X;
    }
    else
    {
        splittedBins = splitComplex(signal, N);
        signalEven = ifftCore(splittedBins[EVEN], N / 2);
        signalOdd = ifftCore(splittedBins[ODD], N / 2);
    }
    /*X_k = XEven_k + e^{-i2pik/N} * XOdd_k */
    /*X_k = XEven_{k-N/2} - e^{-i2pi(k-N/2)/N} * XOdd_{k-N/2} */
    for (k = 0; k < N; k++)
    {
        signal[k].re = signalEven[k % (N / 2)].re - (signalOdd[k % (N / 2)].re * cos(pi2n * k) + signalOdd[k % (N / 2)].im * sin(pi2n * k));
        signal[k].im = signalEven[k % (N / 2)].im - (signalOdd[k % (N / 2)].im * cos(pi2n * k) - signalOdd[k % (N / 2)].re * sin(pi2n * k));
    }
    /* verificar erro */
    // for (k = 0; k < N / 2; k++)
    // {
    //     X[k].re = XEven[k].re + cos(pi2n * k) * XOdd[k].re + sin(pi2n * k) * XOdd[k].im;
    //     X[k].im = XEven[k].im + cos(pi2n * k) * XOdd[k].im - sin(pi2n * k) * XOdd[k].re;
    // }
    // for (k = N / 2; k < N - 1; k++)
    // {
    //     int n2 = N / 2;
    //     X[k].re = XEven[k % n2].re - (cos(pi2n * (k % n2)) * XOdd[k % n2].re + sin(pi2n * (k % n2)) * XOdd[k % n2].im);
    //     X[k].im = XEven[k % n2].im - (cos(pi2n * (k % n2)) * XOdd[k % n2].im - sin(pi2n * (k % n2)) * XOdd[k % n2].re);
    // }

    signalEven = signalOdd = NULL;
    freeComplexVectors(splittedBins);
    return X;
}

Complex *fftRecursive(double *signal, int N)
{
    int i;
    Complex *X;

    X = fftCore(signal, N);
    for (i = 0; i < N; i++)
    {
        X[i].im /= N;
        X[i].re /= N;
    }
    return X;
}

double *ifftRecursive(Complex *X, int N)
{
    Complex *signalComplex;
    double *signalReal;
    int i;

    signalComplex = ifftCore(X, N);

    signalReal = allocate_vector(N, false);

    for (i = 0; i < N; i++)
    {
        signalReal[i] = signalComplex[i].re;
    }

    return signalReal;
}
