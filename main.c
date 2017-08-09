#include "commons/commons.h"
#include "dftManual/dftmanual.h"
#include "fftRecursive/fftRecursive.h"
#include "fftw3/fftw3Impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#define BASE_TAMANHO_SINAL 32

void printTimes(double *times, int signalQty, int nTestRounds, char *title)
{
    int i, signalSize;

    printf("%s", title);
    for (i = 0; i < signalQty; i++)
    {
        signalSize = (int)BASE_TAMANHO_SINAL * pow(2, i);
        printf("%5d - %.5fs\n", signalSize, (double)times[i] / nTestRounds);
    }
}

Complex **runDFTTests(double **signals, int *signalSizes, int signalQty, int nTestRounds)
{
    int i, j;
    clock_t start, end;
    Complex **X;
    double total;
    double *sinalReconstruido, *times;

    X = (Complex **)malloc(signalQty * sizeof(Complex *));
    if (X == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }

    times = allocate_vector(signalQty, true);

    for (j = 0; j < nTestRounds; j++)
    {
        for (i = 0; i < signalQty; i++)
        {
            start = clock();
            X[i] = dft(signals[i], signalSizes[i]);
            end = clock();
            free(X[i]);
            total = ((double)(end - start)) / CLOCKS_PER_SEC;
            times[i] += total;
        }
    }
    printTimes(times, signalQty, nTestRounds, "DFT Times:\n");
    free(times);

    return X;
}

Complex **runFFTTests(double **signals, int *signalSizes, int signalQty, int nTestRounds)
{
    int i, j;
    clock_t start, end;
    Complex **X;
    double total;
    double *sinalReconstruido, *times, *fftPlot, *idft2;

    X = (Complex **)malloc(signalQty * sizeof(Complex *));
    if (X == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    times = allocate_vector(signalQty, true);
    for (j = 0; j < nTestRounds; j++)
    {
        for (i = 0; i < signalQty; i++)
        {
            start = clock();
            X[i] = fftRecursive(signals[i], signalSizes[i]);
            end = clock();
            /*sinalReconstruido = ifftRecursive(X[i], signalSizes[i]);
            plotaSinal(signals[i], signalSizes[i], "sinal");
            plotaSinal(sinalReconstruido, signalSizes[i], "reconstruido");
            pause();*/
            free(X[i]);
            total = ((double)(end - start)) / CLOCKS_PER_SEC;
            times[i] += total;
        }
    }

    printTimes(times, signalQty, nTestRounds, "FFT Recursive Times:\n");
    free(times);

    return X;
}

Complex **runFFTW3Tests(double **signals, int *signalSizes, int signalQty, int nTestRounds)
{
    int i, j;
    clock_t start, end;
    Complex **X;
    double total;
    double *sinalReconstruido, *times, *fftPlot, *idft2;

    X = (Complex **)malloc(signalQty * sizeof(Complex *));
    if (X == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    times = allocate_vector(signalQty, true);
    for (j = 0; j < nTestRounds; j++)
    {
        for (i = 0; i < signalQty; i++)
        {
            start = clock();
            X[i] = fftw3(signals[i], signalSizes[i]);
            end = clock();
            sinalReconstruido = ifftw3(X[i], signalSizes[i]);
            /*plotaSinal(signals[i], signalSizes[i], "sinal");
            plotaSinal(sinalReconstruido, signalSizes[i], "reconstruido");
            pause();
            free(X[i]);*/
            total = ((double)(end - start)) / CLOCKS_PER_SEC;
            times[i] += total;
        }
    }

    printTimes(times, signalQty, nTestRounds, "FFT Recursive Times:\n");
    free(times);

    return X;
}

double **create_signals(int *signalSizes, int sizeQty)
{
    int i;
    double **signals;

    signals = (double **)malloc(sizeQty * sizeof(double *));
    if (signals == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    for (i = 0; i < sizeQty; i++)
    {
        signals[i] = create_signal(signalSizes[i]);
    }

    return signals;
}

int *create_signal_sizes(int sizeQty)
{
    int *signalSizes;
    int i;

    signalSizes = (int *)malloc(sizeQty * sizeof(int));
    if (signalSizes == NULL)
    {
        printf("memory error\n");
        exit(-1);
    }
    for (i = 0; i < sizeQty; i++)
    {
        signalSizes[i] = BASE_TAMANHO_SINAL * pow(2, i);
    }

    return signalSizes;
}

void freeMemory(double **signal, int signalQty)
{
    int i;

    for (i = 0; i < signalQty; i++)
    {
        free(signal[i]);
    }
    free(signal);
}

int main(int argc, char *argv[])
{
    double **signals;
    double *sinalReconstruido, *dftCart;
    Complex **X;
    int signalQty = 5;
    int nTestRounds = 1;
    int *signalSizes;

    if (argc == 3)
    {
        signalQty = atoi(argv[1]);
        nTestRounds = atoi(argv[2]);
        printf("%d signals and %d test times\n", signalQty, nTestRounds);
    }
    else
    {
        printf("Use: ./main #signals #tests. Using default numbers: %d signals and %d test times\n", signalQty, nTestRounds);
    }

    signalSizes = create_signal_sizes(signalQty);

    signals = create_signals(signalSizes, signalQty);

    X = runDFTTests(signals, signalSizes, signalQty, nTestRounds);

    X = runFFTTests(signals, signalSizes, signalQty, nTestRounds);

    X = runFFTW3Tests(signals, signalSizes, signalQty, nTestRounds);

    freeMemory(signals, signalQty);

    pause();

    return 0;
}
