#include "ampblaslib.h"

ampblasStatus ampblas_sgemv(const enum AMPBLAS_ORDER order, const enum AMPBLAS_TRANS type, const int M, const int N,
                              const float *alpha, const float *A, const int lda, const float *X, const int incX,
                              const float *beta, float *Y, const int incY)
{
    cout << "\nInside ampblas_sgemv function\n"<<endl;
    return AMPBLAS_SUCCESS;
}
