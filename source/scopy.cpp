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

void scopy_AMP(Concurrency::accelerator_view &accl_view, long n,
               Concurrency::array_view<float> &X, long incx, long xOffset,
               Concurrency::array_view<float> &Y, long incy, long yOffset,
               long X_batchOffset, long Y_batchOffset, int batchSize)
{
    long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    Concurrency::extent<2> compute_domain(batchSize, size);
    Concurrency::parallel_for_each(accl_view, compute_domain.tile<1, BLOCK_SIZE>(),[=] (Concurrency::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp)
    {
      int elt = tidx.tile[0];      
      if(tidx.global[1] < n)
        Y[yOffset + Y_batchOffset * elt + tidx.global[1]] = X[xOffset + X_batchOffset * elt + tidx.global[1]];
    });
}

// SCOPY call Type I - SSCAL Inputs and Outputs are host float pointers 
ampblasStatus Ampblaslibrary :: ampblas_scopy(const int N, float *X, const int incX, const long xOffset, 
                                              float *Y, const int incY, const long yOffset)
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

// SCOPY Call Type II: Inputs and outputs are C++ AMP float array_View containers
ampblasStatus Ampblaslibrary :: ampblas_scopy(Concurrency::accelerator_view &accl_view, const int N, 
                                              Concurrency::array_view<float> &X, const int incX, const long xOffset, 
                                              Concurrency::array_view<float> &Y, const int incY, const long yOffset)
{
    /*Check the conditions*/
    if (  N <= 0 ){
        return AMPBLAS_INVALID;
    }
    scopy_AMP(accl_view, N, X, incX, xOffset, Y, incY, yOffset);

    return AMPBLAS_SUCCESS;

}

// SCOPY TYpe III - Overloaded function with arguments related to batch processing 
ampblasStatus Ampblaslibrary :: ampblas_scopy(Concurrency::accelerator_view &accl_view, const int N,
                                              Concurrency::array_view<float> &X, const int incX, const long xOffset,
                                              Concurrency::array_view<float> &Y, const int incY, const long yOffset,
                                              const long X_batchOffset, const long Y_batchOffset, const int batchSize)
{
    /*Check the conditions*/
    if (  N <= 0 ){
        return AMPBLAS_INVALID;
    }
    scopy_AMP(accl_view, N, X, incX, xOffset, Y, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);

    return AMPBLAS_SUCCESS;

}

