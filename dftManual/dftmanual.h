#ifndef DFTMANUAL_H_
#define DFTMANUAL_H_
#include "./../commons/commons.h"

Complex *dft(double *sinal, int N);
Complex **dftWindowed(double *vectorSignal, int vectorSize, int windowSize, float hopSize);
double *idft(Complex *X, int N);
/*double *cria_sinal(int N);
void imprime_resultados2(double *sinal, Complex *X, double *sinalReconstruido, int N);
void plotaSinal(double *sinal, int N, char *title);*/

#endif
