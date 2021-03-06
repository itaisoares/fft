#ifndef FFTRECURSIVE_H_
#define FFTRECURSIVE_H_
#include "./../commons/commons.h"

Complex *fftRecursive(double *signal, int N);

double *ifftRecursive(Complex *X, int N);

#endif
