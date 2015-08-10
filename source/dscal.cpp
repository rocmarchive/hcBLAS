#include "ampblaslib.h"
#include <amp.h>

using namespace concurrency;
#define BLOCK_SIZE 8 

void dscal_AMP(Concurrency::accelerator_view &accl_view,
               long n, double alpha,
               Concurrency::array_view<double> &X, long incx, long xOffset)
{
    long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    Concurrency::extent<1> compute_domain(size);
    Concurrency::parallel_for_each(accl_view, compute_domain.tile<BLOCK_SIZE>(),[=] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp)
    {
      if(tidx.global[0] < n)
        X[xOffset + tidx.global[0]] = X[xOffset + tidx.global[0]] * alpha;
    });
  }

ampblasStatus Ampblaslibrary :: ampblas_dscal(const int N, const double *alpha,
                                              double *X, const int incX, const long xOffset)
{

    if (alpha == NULL || X == NULL || N <= 0 ) {
        return AMPBLAS_INVALID;
    }

    int lenX = 1 + (N - 1) * abs(incX);
    array_view<double> xView(lenX, X);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    dscal_AMP(accl_view, N, *alpha, xView, incX, xOffset);

    return AMPBLAS_SUCCESS;

}
