#include "ampblaslib.h"

ampblasStatus Ampblaslibrary :: ampblas_cgemm(const enum AMPBLAS_ORDER order,
                                              const enum AMPBLAS_TRANS typeA,
                                              const enum AMPBLAS_TRANS typeB,
                                              const int M, const int N,
                                              const int K,
                                              const Complex<float> *alpha,
                                              const Complex<float> *A,
                                              const int lda,
                                              const Complex<float> *B,
                                              const int ldb,
                                              const Complex<float> *beta,
                                              Complex<float> *C,
                                              const int ldc)
{
    cout << "\nInside ampblas_cgemm function\n"<<endl;
    return AMPBLAS_SUCCESS;
}

