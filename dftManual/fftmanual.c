#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

/**
Algoritmo se baseou no livro Introduction to computer Music (F. Richard Moore)
na página 66-68. Os valores das funções são previamente calculados neste caso.
**/
typedef struct no
{
    double re;
    double im;
} complex;
typedef complex Complex;

#define TAMANHO 16

/*funcao utilizada para alocar a matriz*/
double **cria_matriz(int linhas, int colunas)
{
    int i;
    double **matriz;

    matriz = (double **)malloc(linhas * sizeof(double *));
    if (matriz == NULL)
    {
        printf("ERRO! nao ha' espaco na memoria\n");
        exit(-1);
    }
    for (i = 0; i < linhas; i++)
    {
        matriz[i] = (double *)malloc(colunas * sizeof(double));
        if (matriz == NULL)
        {
            printf("ERRO! nao ha' memoria suficiente\n");
            exit(-1);
        }
    }
    return (matriz);
}

void preenche_matriz(double **matriz, int linhas, int colunas)
{
    int i, j;
    int contador = 0;

    for (i = 0; i < linhas; i++)
    {
        for (j = 0; j < colunas; j++)
        {
            matriz[i][j] = contador++;
        }
    }
}

void imprime_matriz(double **matriz, int linhas, int colunas)
{
    int i, j;
    printf("\n");
    for (i = 0; i < linhas; i++)
    {
        for (j = 0; j < colunas; j++)
        {
            printf("%3.2f ", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
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
    /* fazer consulta a tabela pra nao calcular o mesmo valor duas vezes*/
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

/* each row is a slice with binSize fourier constant */
Complex **allocate_bin_matrix(int binSize, int slices)
{
    Complex **matrix;
    int i;

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

    // for (i = 0; i < slices; i++)
    // {
    //     for (j = 0; j < binSize; j++)
    //     {
    //         matrix[i][j].im = matrix[i][j].re = 0;
    //     }
    // }

    return matrix;
}

Complex *dftMain(Complex *X, double *vectorSignal, Complex **exponentialBasis, int N)
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
Complex **dft(double *vectorSignal, int vectorSize, int windowSize, float hopSize)
{
    /* validations / avoid errors */
    if (hopSize > 1.0)
    {
        hopSize = 0.9;
    }
    if (windowSize > vectorSize)
    {
        windowSize = vectorSize;
    }

    Complex **exponentialBasis = create_exponential_basis(windowSize);

    /*steps: 1 + (vectorSize - windowSize) / (1 - hopSize) * windowSize */
    int steps = 1 + (int)((vectorSize - windowSize) / (int)((1 - hopSize) * windowSize));

    Complex **X = allocate_bin_matrix(windowSize, steps);
    int i;
    for (i = 0; i < steps; i++)
    {
        int begin = i * windowSize;
        // int end = begin + windowSize;
        double *subVector = &vectorSignal[begin];
        X[i] = dftMain(X[i], subVector, exponentialBasis, windowSize);
    }
    return X;
}

Complex *dftMain2(double *vetor, int N)
{
    int i, k, n;
    Complex *X;

    Complex **matrizExponenciais = create_exponential_basis(N);

    /* alocando o vetor */
    for (i = 0; i < N; i++)
    {
        X = (Complex *)malloc(N * sizeof(Complex));
        if (X == NULL)
        {
            printf("ERRO! nao ha'memoria suficiente\n");
            exit(-1);
        }
    }

    for (k = 0; k < N; k++)
    {
        X[k].re = X[k].im = 0;
        for (n = 0; n < N; n++)
        {
            X[k].re += vetor[n] * matrizExponenciais[n][k].re;
            X[k].im -= vetor[n] * matrizExponenciais[n][k].im;
        }
        X[k].re /= N;
        X[k].im /= N;
    }

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
double *cria_sinal(int N)
{
    int i;
    double pi2n = (M_PI * 2) / N;
    double *sinal = (double *)malloc(N * sizeof(double));
    if (sinal == NULL)
    {
        printf("ERRO! nao ha'memoria suficiente\n");
        exit(-1);
    }

    for (i = 0; i < N; i++)
    {
        sinal[i] = sin(pi2n * 1 * i) + 0.33 * sin(pi2n * 3 * i) + 0.2 * sin(pi2n * 5 * i);
    }
    return sinal;
}

void imprime_resultados(double *sinal, Complex **X, int N)
{
    int i;
    for (i = 0; i < N; i++)
    {
        /*printf("[%2d] x1 = %6.3f, X = (%6.3f, %6.3f), x2 = %6.3f\n", i, sinal[i], X[0][i].re, X[0][i].im, sinalReconstruido[i]);*/
        printf("[%2d] x1 = %6.3f, X = (%6.3f, %6.3f)\n", i, sinal[i], X[0][i].re, X[0][i].im);
    }
}

void imprime_resultados2(double *sinal, Complex *X, double *sinal2, int N)
{
    int i;
    for (i = 0; i < N; i++)
    {
        /*printf("[%2d] x1 = %6.3f, X = (%6.3f, %6.3f), x2 = %6.3f\n", i, sinal[i], X[0][i].re, X[0][i].im, sinalReconstruido[i]);*/
        printf("[%2d] x1 = %6.3f, X = (%6.3f, %6.3f), x2 = %6.3f\n", i, sinal[i], X[i].re, X[i].im, sinal2[i]);
    }
}

int main()
{
    double *sinal = cria_sinal(TAMANHO);
    // Complex **X = dft(sinal, TAMANHO, TAMANHO, 0);
    // imprime_resultados(sinal, X, TAMANHO);

    Complex *X = dftMain2(sinal, TAMANHO);
    double *sinalReconstruido = idft(X, TAMANHO);
    imprime_resultados2(sinal, X, sinalReconstruido, TAMANHO);
    // double **matriz = cria_matriz(3, 4);
    // preenche_matriz(matriz, 3, 4);
    /*imprime_matriz(matriz, 3, 4);*/
    return 0;
}