#include "ampblaslib.h"

ampblasStatus Ampblaslibrary::ampblas_saxpy(const int N,
                              const float *alpha, const float *X, const int incX,
                              const float *Y, const int incY)
{
    cout << "\nInside ampblas_saxpy function\n"<<endl;
    return AMPBLAS_SUCCESS;
}

