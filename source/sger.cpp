#include "ampblaslib.h"

ampblasStatus Ampblaslibrary :: ampblas_sger(const enum AMPBLAS_ORDER order, const int M, const int N,
                              const float alpha, const float *X, const int incX,
                              const float *Y, const int incY, float *A, const int lda)
{
    cout << "\nInside ampblas_sger function\n"<<endl;
    return AMPBLAS_SUCCESS;
}
