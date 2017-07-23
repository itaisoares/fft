#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define true 1
#define false 0
typedef int bool;

typedef struct no
{
    double re;
    double im;
} complex;
typedef complex Complex;

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
        sinal[i] = sin(pi2n * 200 * i) + 0.33 * sin(pi2n * 300 * i) + 0.2 * sin(pi2n * 500 * i);
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

void plotaSinal(double *sinal, int N, char *titulo)
{
    int i;
    char setTitulo[100];
    FILE *gnuplot = popen("gnuplot", "w");
    strcpy(setTitulo, "set title '");
    strcat(setTitulo, titulo);
    strcat(setTitulo, "'\n");
    fprintf(gnuplot, setTitulo);
    fprintf(gnuplot, "plot '-' with lines\n");
    for (i = 0; i < N; i++)
        fprintf(gnuplot, "%d %g\n", i, sinal[i]);
    fprintf(gnuplot, "e\n");
    fflush(gnuplot);
}

double *allocate_vector(int N, bool initialize)
{
    double *vector;
    int i;

    vector = (double *)malloc(N * sizeof(double));
    if (vector == NULL)
    {
        printf("Error alocating vector");
        exit(-1);
    }

    if (initialize == true)
    {
        for (i = 0; i < N; i++)
        {
            vector[i] = 0;
        }
    }
    return vector;
}

double *pol_to_cart(Complex *X, int N)
{
    int i;
    double *vector = allocate_vector(N, false);
    for (i = 0; i < N; i++)
    {
        vector[i] = sqrt(X[i].re * X[i].re + X[i].im * X[i].im);
    }

    return vector;
}

/************* NAO USADO POR ENQUANTO ***************/

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
