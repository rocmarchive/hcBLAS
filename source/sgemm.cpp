#include "ampblaslib.h"

ampblasStatus Ampblaslibrary :: ampblas_sgemm(const enum AMPBLAS_ORDER order,
                                              const enum AMPBLAS_TRANS typeA,
                                              const enum AMPBLAS_TRANS typeB,
                                              const int M, const int N,
                                              const int K, const float *alpha,
                                              const float *A, const int lda,
                                              const float *B, const int ldb,
                                              const float *beta, float *C,
                                              const int ldc)
{
    cout << "\nInside ampblas_sgemm function\n"<<endl;
    return AMPBLAS_SUCCESS;
}
