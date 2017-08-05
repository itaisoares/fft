#include "commons/commons.h"
#include "dftManual/dftmanual.h"
#include "fftRecursive/fftRecursive.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

Complex **runDFTTests(double **signals, int *signalSizes, int signalQty)
{
    int i;
    clock_t start, end;
    Complex **X;
    double total;
    double *sinalReconstruido;

    X = (Complex **)malloc(signalQty * sizeof(Complex));
    if (X == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    printf("DFT Times:\n");
    for (i = 0; i < signalQty; i++)
    {
        start = clock();
        X[i] = dft(signals[i], signalSizes[i]);
        end = clock();
        free(X[i]);
        total = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("%5d - %.3fs\n", signalSizes[i], total);
    }

    return X;
}

Complex **runFFTTests(double **signals, int *signalSizes, int signalQty)
{
    int i;
    clock_t start, end;
    Complex **X;
    double total;
    double *sinalReconstruido;

    X = (Complex **)malloc(signalQty * sizeof(Complex));
    if (X == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    printf("FFT Recursive Times:\n");
    for (i = 0; i < signalQty; i++)
    {
        start = clock();
        X[i] = fftRecursive(signals[i], signalSizes[i]);
        end = clock();
        free(X[i]);
        total = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("%5d - %.3fs\n", signalSizes[i], total);
    }

    return X;
}

/*not tested*/
Complex **runDFTWindowedTests(double **signals, int *signalSizes, int signalQty)
{
    int i;
    clock_t start, end;
    Complex **X;
    double total;

    X = (Complex **)malloc(signalQty * sizeof(Complex));
    if (X == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    printf("DFT Times:\n");
    for (i = 0; i < signalQty; i++)
    {
        start = clock();
        X[i] = fftRecursive(signals[i], signalSizes[i]);
        end = clock();
        total = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("%5d - %.3fs\n", signalSizes[i], total);
    }

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
    int base = 16;
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
        signalSizes[i] = base * pow(2, i);
    }

    return signalSizes;
}
int main(int argc, char *argv[])
{
    double **signals;
    double *sinalReconstruido, *dftCart;
    Complex **X;
    int signalQty = 2;
    int *signalSizes;
    int TEMP = 64;

    double *signal;
    Complex *X2;

    if (argc == 2)
    {
        signalQty = atoi(argv[1]);
        TEMP = atoi(argv[2]);
    }

    signalSizes = create_signal_sizes(signalQty);

    signals = create_signals(signalSizes, signalQty);

    X = runDFTTests(signals, signalSizes, signalQty);

    X = runFFTTests(signals, signalSizes, signalQty);

    // signal = create_signal(TEMP);
    // X2 = fftRecursive(signal, TEMP);

    // sinalReconstruido = idft(X2, TEMP);
    /*imprime_resultados2(sinal, X, sinalReconstruido, N);*/
    // plotaSinal(signal, TEMP, "sinal original");
    // dftCart = pol_to_cart(X2, TEMP);
    // plotaSinal(dftCart, TEMP, "DFT");
    // plotaSinal(sinalReconstruido, TEMP, "sinal reconstruido");

    /*printf("Criando sinal de N %d\n", N);
    sinal = cria_sinal(N);
    //Complex **X = dft(sinal, N, N, 0);
    imprime_resultados(sinal, X, N);*/

    /*start = clock();
    X = dft(sinal, N);
    end = clock();
    total = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("DFT time %.3f\n", total);*/

    /*sinalReconstruido = idft(X[TEMP], signalSizes[TEMP]);
    
    plotaSinal(signals[TEMP], signalSizes[TEMP], "sinal original");
    dftCart = pol_to_cart(X[TEMP], signalSizes[TEMP]);
    plotaSinal(dftCart, signalSizes[TEMP], "DFT");
    plotaSinal(sinalReconstruido, signalSizes[TEMP], "sinal reconstruido");*/
    pause();

    return 0;
}
