#include "ampblaslib.h"
#include <amp.h>

using namespace concurrency;
#define BLOCK_SIZE 16 

void axpy_AMP(long n, float alpha,
              Concurrency::array_view<float> &X, long xOffset, long incx,
              Concurrency::array_view<float> &Y, long yOffset, long incy)
{
    long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    Concurrency::extent<1> compute_domain(size);

    Concurrency::parallel_for_each(compute_domain.tile<BLOCK_SIZE>(),[=] 
                                   (Concurrency::tiled_index<BLOCK_SIZE> tidx)
                                   restrict(amp) {
        if (tidx.global[0] < n)
            Y[yOffset + tidx.global[0]] += X[xOffset + tidx.global[0]] * alpha;
    });
}

ampblasStatus Ampblaslibrary :: ampblas_saxpy(const int N, const float *alpha,
                                              float *X, const int incX,
                                              float *Y, const int incY,
                                              long xOffset, long yOffset)
{
    if (alpha == NULL || X == NULL || Y == NULL || N <= 0 ) {
        return AMPBLAS_INVALID;
    }

    int lenX = 1 + (N - 1) * abs(incX);
    int lenY = 1 + (N - 1) * abs(incY);
   
    array_view<float> xView(lenX, X);
    array_view<float> yView(lenY, Y);

    axpy_AMP(N, *alpha, xView, xOffset, incX, yView, yOffset, incY);
    yView.synchronize();

    return AMPBLAS_SUCCESS;
}
