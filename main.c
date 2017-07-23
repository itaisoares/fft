#include "commons/commons.h"
#include "dftManual/dftmanual.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
    double *sinal, *sinalReconstruido, *dftCart;
    Complex *X;
    int N = 128;
    clock_t start, end;
    double total;

    if (argc == 2)
    {
        N = atoi(argv[1]);
    }
    printf("Criando sinal de N %d\n", N);
    sinal = cria_sinal(N);
    /*Complex **X = dft(sinal, N, N, 0);
    imprime_resultados(sinal, X, N);*/

    start = clock();
    X = dftMain2(sinal, N);
    end = clock();
    total = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("DFT time %.3f\n", total);

    sinalReconstruido = idft(X, N);
    /*imprime_resultados2(sinal, X, sinalReconstruido, N);*/
    plotaSinal(sinal, N, "sinal original");
    dftCart = pol_to_cart(X, N);
    plotaSinal(dftCart, N, "DFT");
    plotaSinal(sinalReconstruido, N, "sinal reconstruido");
    pause();
    /*double **matriz = cria_matriz(3, 4);
    preenche_matriz(matriz, 3, 4);
    imprime_matriz(matriz, 3, 4);*/
    return 0;
}
