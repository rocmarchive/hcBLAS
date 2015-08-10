#include "ampblaslib.h"
#include <amp.h>

using namespace concurrency;
#define BLOCK_SIZE 8 

void scopy_AMP(Concurrency::accelerator_view &accl_view, long n, 
               Concurrency::array_view<float> &X, long incx, long xOffset,
               Concurrency::array_view<float> &Y, long incy, long yOffset)
{
    long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    Concurrency::extent<1> compute_domain(size);
    Concurrency::parallel_for_each(accl_view, compute_domain.tile<BLOCK_SIZE>(),[=] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp)
    {
      if(tidx.global[0] < n)
        Y[yOffset + tidx.global[0]] = X[xOffset + tidx.global[0]];
    });
  }

ampblasStatus Ampblaslibrary :: ampblas_scopy(const int N, float *X, const int incX, const long xOffset, float *Y, const int incY, const long yOffset)
{

    if (Y == NULL || X == NULL || N <= 0 ) {
        return AMPBLAS_INVALID;
    }

    int lenX = 1 + (N - 1) * abs(incX);
    int lenY = 1 + (N - 1) * abs(incY);
    array_view<float> xView(lenX, X);
    array_view<float> yView(lenY, Y);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    scopy_AMP(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
    return AMPBLAS_SUCCESS;

}
