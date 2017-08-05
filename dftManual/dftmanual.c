#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <unistd.h>
#include "./../commons/commons.h"
/**
Algoritmo se baseou no livro Introduction to computer Music (F. Richard Moore)
na página 66-68. Os valores das funções são previamente calculados neste caso.
**/

/**
cria os valores das funcoes basicas
da forma exp(-i*2*pi*n/N), cada coluna é um uma funcao k
**/
Complex **create_exponential_basis(int N)
{
    int i, n, k;
    /*double pi2n = (M_PI * 2) / N;*/
    double pi2n = (8 * atan(1)) / N;

    /* alocando matriz */
    Complex **matriz = (Complex **)malloc(N * sizeof(Complex *));
    if (matriz == NULL)
    {
        printf("ERRO! nao ha' memoria suficiente\n");
        exit(-1);
    }
    for (i = 0; i < N; i++)
    {
        matriz[i] = (Complex *)malloc(N * sizeof(Complex));
        if (matriz[i] == NULL)
        {
            printf("ERRO! nao ha'memoria suficiente\n");
            exit(-1);
        }
    }

    /*calculando os valores */
    for (i = 0; i < N; i++)
    {
        matriz[0][i].re = matriz[i][0].re = 1;
        matriz[i][0].im = matriz[0][i].im = 0;
    }
    for (k = 1; k < N; k++)
    {
        for (n = 1; n < N; n++)
        {
            matriz[n][k].re = cos(pi2n * k * n);
            matriz[n][k].im = sin(pi2n * k * n);
        }
    }
    return matriz;
}

void deallocate_exponential_basis(Complex **basis, int N)
{
    int i;

    for (i = 0; i < N; i++)
    {
        free(basis[i]);
    }
    free(basis);
}
/* each row is a slice with binSize fourier constant */
Complex **allocate_bin_matrix(int binSize, int slices, bool initialize)
{
    Complex **matrix;
    int i, j;

    matrix = (Complex **)malloc(slices * sizeof(Complex *));
    if (matrix == NULL)
    {
        printf("Error alocating bin matrix");
        exit(-1);
    }
    for (i = 0; i < slices; i++)
    {
        matrix[i] = (Complex *)malloc(binSize * sizeof(Complex));
        if (matrix[i] == NULL)
        {
            printf("Error alocating bin matrix");
            exit(-1);
        }
    }

    if (initialize == true)
    {
        for (i = 0; i < slices; i++)
        {
            for (j = 0; j < binSize; j++)
            {
                matrix[i][j].im = matrix[i][j].re = 0;
            }
        }
    }
    return matrix;
}

Complex *dftCore(Complex *X, double *vectorSignal, Complex **exponentialBasis, int N)
{
    int k, n;

    for (k = 0; k < N; k++)
    {
        X[k].re = X[k].im = 0;
        for (n = 0; n < N; n++)
        {
            X[k].re += vectorSignal[n] * exponentialBasis[n][k].re;
            X[k].im -= vectorSignal[n] * exponentialBasis[n][k].im;
        }
        X[k].re /= N;
        X[k].im /= N;
    }

    return X;
}

/* windoSize <= vectorSize, hopSize [0, 0.99] */
/* return a Complex matrix(x,y) where each y column has
the x dimensional X fourier coefficients*/
Complex **dftWindowed(double *vectorSignal, int vectorSize, int windowSize, float hopSize)
{
    Complex **exponentialBasis, **X;
    int i, steps, begin;
    double *subVector;

    /* validations / avoid errors */
    if (hopSize > 1.0)
    {
        hopSize = 0.9;
    }
    if (windowSize > vectorSize)
    {
        windowSize = vectorSize;
    }

    exponentialBasis = create_exponential_basis(windowSize);

    /*steps: 1 + (vectorSize - windowSize) / (1 - hopSize) * windowSize */
    steps = 1 + (int)((vectorSize - windowSize) / (int)((1 - hopSize) * windowSize));

    X = allocate_bin_matrix(windowSize, steps, false);
    for (i = 0; i < steps; i++)
    {
        begin = i * windowSize;
        subVector = &vectorSignal[begin];
        X[i] = dftCore(X[i], subVector, exponentialBasis, windowSize);
    }
    return X;
}

Complex *dft(double *vetor, int N)
{
    Complex *X;

    Complex **exponentialBasis = create_exponential_basis(N);

    X = allocate_bin_vector(N, true);

    X = dftCore(X, vetor, exponentialBasis, N);

    deallocate_exponential_basis(exponentialBasis, N);

    return X;
}

double *idft(Complex *X, int N)
{
    int i, n, k;
    Complex **matrizExponenciais = create_exponential_basis(N);
    double *sinal;

    sinal = (double *)malloc(N * sizeof(double));
    if (sinal == NULL)
    {
        printf("ERRO! nao ha'memoria suficiente\n");
        exit(-1);
    }
    for (i = 0; i < N; i++)
    {
        sinal[i] = 0;
    }

    for (k = 0; k < N; k++)
    {
        for (n = 0; n < N; n++)
        {
            sinal[k] += X[n].re * matrizExponenciais[n][k].re - X[n].im * matrizExponenciais[n][k].im;
        }
    }

    return sinal;
}
