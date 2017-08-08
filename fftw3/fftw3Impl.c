#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
// #include <math.h>
// #include <stddef.h>
// #include <unistd.h>
#include "./../commons/commons.h"
#define EVEN 0
#define ODD 1

fftw_complex *double_to_fftw_complex(double *signal, int N)
{
    int i;
    fftw_complex *signalFftw = (fftw_complex *)fftw_malloc(N * sizeof(fftw_complex));

    for (i = 0; i < N; i++)
    {
        signalFftw[i][0] = signal[i];
        signalFftw[i][1] = 0;
    }
    return signalFftw;
}

Complex *fftw_complex_to_complex(fftw_complex *signalFfftw, int N)
{
    int i;
    Complex *signal = (Complex *)malloc(N * sizeof(Complex));

    for (i = 0; i < N; i++)
    {
        signal[i].re = signalFfftw[i][0];
        signal[i].im = signalFfftw[i][1];
    }
    return signal;
}

fftw_complex *complex_to_fftw_complex(Complex *X, int N)
{
    int i;
    fftw_complex *signalFftw = (fftw_complex *)fftw_malloc(N * sizeof(fftw_complex));

    for (i = 0; i < N; i++)
    {
        signalFftw[i][0] = X[i].re;
        signalFftw[i][1] = X[i].im;
    }
    return signalFftw;
}

double *fftw_complex_to_double(fftw_complex *singalFfftw, int N)
{
    int i;
    double *signal = (double *)malloc(N * sizeof(double));

    for (i = 0; i < N; i++)
    {
        signal[i] = singalFfftw[i][0];
    }
    return signal;
}

Complex *fftw3(double *signal, int N)
{
    fftw_complex *signalIn, *signalOut;
    fftw_plan p;
    Complex *X;

    signalIn = double_to_fftw_complex(signal, N);
    signalOut = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, signalIn, signalOut, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(p);

    X = fftw_complex_to_complex(signalOut, N);

    fftw_destroy_plan(p);

    fftw_free(signalIn);
    fftw_free(signalOut);

    return X;
}

double *ifftw3(Complex *X, int N)
{
    fftw_complex *signalIn, *signalOut;
    fftw_plan p;
    double *signal;

    signalIn = complex_to_fftw_complex(X, N);
    signalOut = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, signalIn, signalOut, FFTW_BACKWARD, FFTW_ESTIMATE);

    fftw_execute(p);

    signal = fftw_complex_to_double(signalOut, N);

    fftw_destroy_plan(p);

    fftw_free(signalIn);
    fftw_free(signalOut);

    return signal;
}
