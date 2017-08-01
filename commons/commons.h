#ifndef COMMONS_H_
#define COMMONS_H_
#define true 1
#define false 0
typedef int bool;

typedef struct no
{
    double re;
    double im;
} complex;
typedef complex Complex;

double *create_signal(int N);

void imprime_resultados(double *sinal, Complex **X, int N);

void imprime_resultados2(double *sinal, Complex *X, double *sinal2, int N);

void plotaSinal(double *sinal, int N, char *titulo);

double *pol_to_cart(Complex *X, int N);

Complex *allocate_bin_vector(int N, bool initialize);

#endif
