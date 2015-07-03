#include "ampblaslib.h"
#include <amp.h>

using namespace concurrency;
#define BLOCK_SIZE 8 

void axpy_AMP(Concurrency::accelerator_view &accl_view, 
	      long n, float alpha,
              Concurrency::array_view<float> &X, long xOffset, long incx,
              Concurrency::array_view<float> &Y, long yOffset, long incy)
{
  if(n <= 102400)
  {
    long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    Concurrency::extent<1> compute_domain(size);
    Concurrency::parallel_for_each(accl_view, compute_domain.tile<BLOCK_SIZE>(),[=] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp)
    {
      if(tidx.global[0] < n)
        Y[yOffset + tidx.global[0]] += X[xOffset + tidx.global[0]] * alpha;
    });
  }
  else
  {
    int step_sz;
    if (n <= 409600)
      step_sz = 3;
    else if (n <= 921600)
      step_sz = 5;
    else if ( n <= 1939526)
      step_sz = 7;
    else
      step_sz = 15;

    long size = (n/step_sz + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    long nBlocks = size/BLOCK_SIZE;
    Concurrency::extent<1> compute_domain(size);
    Concurrency::parallel_for_each(compute_domain.tile<BLOCK_SIZE>(),[=] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp)
    {
      if(tidx.tile[0] != nBlocks -1)
      {
        for(int iter = 0; iter < step_sz; iter++)
        {
          Y[yOffset + tidx.tile[0] * 256 * step_sz + tidx.local[0] + iter*256] += X[xOffset + tidx.tile[0] * 256 * step_sz + tidx.local[0] + iter*256] * alpha;
        }
      }
      else
      {
        int n_iter = ((n - ((nBlocks - 1) * 256 * step_sz)) / 256) + 1;
        for(int iter = 0; iter < n_iter; iter++)
        {
          if (((nBlocks - 1) * 256 * step_sz + iter * 256 + tidx.local[0]) < n)
            Y[yOffset + tidx.tile[0] * 256 * step_sz + tidx.local[0] + iter*256] += X[xOffset + tidx.tile[0] * 256 * step_sz + tidx.local[0] + iter*256] * alpha;
        }
      }
    });
  } 
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
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    axpy_AMP(accl_view, N, *alpha, xView, xOffset, incX, yView, yOffset, incY);
    yView.synchronize();

    return AMPBLAS_SUCCESS;
}


 ampblasStatus Ampblaslibrary :: ampblas_saxpy(Concurrency::accelerator_view &accl_view,
                                               const int N,const float &alpha,
                                               Concurrency::array_view<float> &X, const int incX,
                                               Concurrency::array_view<float> &Y, const int incY,
                                               long xOffset, long yOffset)

{
    /*Check the conditions*/
    if (  N <= 0 ){
        return AMPBLAS_INVALID;
    }

    if ( alpha == 0){
        return AMPBLAS_SUCCESS;
    }
    axpy_AMP(accl_view, N, alpha, X, xOffset, incX, Y, yOffset, incY);
    Y.synchronize();

    return AMPBLAS_SUCCESS;

}
