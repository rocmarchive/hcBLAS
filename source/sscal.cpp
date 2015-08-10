#include "ampblaslib.h"
#include <amp.h>

using namespace concurrency;
#define BLOCK_SIZE 8 

void sscal_AMP(Concurrency::accelerator_view &accl_view,
               long n, float alpha,
               Concurrency::array_view<float> &X, long incx, long xOffset)
{
    long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    Concurrency::extent<1> compute_domain(size);
    Concurrency::parallel_for_each(accl_view, compute_domain.tile<BLOCK_SIZE>(),[=] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp)
    {
      if(tidx.global[0] < n)
        X[xOffset + tidx.global[0]] = X[xOffset + tidx.global[0]] * alpha;
    });
  }

ampblasStatus Ampblaslibrary :: ampblas_sscal(const int N, const float *alpha,
                                              float *X, const int incX, const long xOffset)
{

    if (alpha == NULL || X == NULL || N <= 0 ) {
        return AMPBLAS_INVALID;
    }

    int lenX = 1 + (N - 1) * abs(incX);
    array_view<float> xView(lenX, X);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    sscal_AMP(accl_view, N, *alpha, xView, incX, xOffset);

    return AMPBLAS_SUCCESS;

}
