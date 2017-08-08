#ifndef FFTW3RECURSIVE_H_
#define FFTW3RECURSIVE_H_
#include "./../commons/commons.h"

Complex *fftw3(double *signal, int N);

double *ifftw3(Complex *X, int N);

#endif
