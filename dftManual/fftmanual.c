#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
Complex **cria_matriz_exponenciais(int N)
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
    for (k = 0; k < N; k++)
    {
        for (n = 0; n < N; n++)
        {
            matriz[n][k].re = cos(pi2n * k * n);
            matriz[n][k].im = sin(pi2n * k * n);
        }
    }
    return matriz;
}
/* windoSize <= vectorSize, hopSize [0, 0.99] */
/* return a Complex matrix(x,y) where each y column has
the x dimensional X fourier coefficients*/
Complex **dft(double *vector, int vectorSize, int windowSize, float hopSize)
{
    /* validations and avoid errors */
    if (hopSize > 1.0)
    {
        hopSize = 0.9;
    }
    if (windowSize > vectorSize)
    {
        windowSize = vectorSize;
    }

    Complex **matrizExponenciais = cria_matriz_exponenciais(windowSize);

    /*steps: 1 + (vectorSize - windowSize) / (1 - hopSize) * windowSize */
    int steps = 1 + (int)((vectorSize - windowSize) / (int)((1 - hopSize) * windowSize));

    int i;
    for (i = 0; i < steps; i++)
    {
        int begin = i * windowSize;
        int end = begin + windowSize;
        /*memcpy*/
    }
    return matrizExponenciais;
}

Complex *dftMain(double *vetor, int N)
{
    int i, k, n;
    Complex *X;

    Complex **matrizExponenciais = cria_matriz_exponenciais(N);

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
    Complex **matrizExponenciais = cria_matriz_exponenciais(N);
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
    printf("aqui");
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

void imprime_resultados(double *sinal, Complex *X, double *sinalReconstruido, int N)
{
    int i;
    for (i = 0; i < N; i++)
    {
        printf("[%2d] x1 = %6.3f, X = (%6.3f, %6.3f), x2 = %6.3f\n", i, sinal[i], X[i].re, X[i].im, sinalReconstruido[i]);
    }
}
int main()
{
    int i;
    int *vetor = (int *)malloc(TAMANHO * sizeof(int));
    for(i = 0; i < TAMANHO; i ++){
        vetor[i] = i;
    }
    int *copia;
    memccpy(vetor, copia, TAMANHO);
    // double *sinal = cria_sinal(TAMANHO);
    // Complex *X = dftMain(sinal, TAMANHO);
    // double *sinalReconstruido = idft(X, TAMANHO);
    // imprime_resultados(sinal, X, sinalReconstruido, TAMANHO);
    // double **matriz = cria_matriz(3, 4);
    // preenche_matriz(matriz, 3, 4);
    /*imprime_matriz(matriz, 3, 4);*/
    return 0;
}