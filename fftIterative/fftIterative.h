#ifndef FFTITERATIVE_H_
#define FFTITERATIVE_H_
#include "./../commons/commons.h"

Complex *fftIterative(double *signal, int N);

double *ifftIterative(Complex *X, int N);

#endif
