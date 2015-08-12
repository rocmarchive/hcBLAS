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

void sscal_AMP(Concurrency::accelerator_view &accl_view,
               long n, float alpha,
               Concurrency::array_view<float> &X, long incx, long xOffset,
               long X_batchOffset, int batchSize)
{
    long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    Concurrency::extent<2> compute_domain(batchSize, size);
    Concurrency::parallel_for_each(accl_view, compute_domain.tile<1, BLOCK_SIZE>(),[=] (Concurrency::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp)
    {
      int elt = tidx.tile[0];
      if(tidx.global[1] < n)
        X[xOffset + X_batchOffset * elt + tidx.global[1]] = X[xOffset + X_batchOffset * elt + tidx.global[1]] * alpha;
    });
}

// SSCAL call Type I - SSCAL Inputs and Outputs are host float pointers 
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

// SSCAL Call Type II: Inputs and outputs are C++ AMP float array_View containers
ampblasStatus Ampblaslibrary :: ampblas_sscal(Concurrency::accelerator_view &accl_view,
                                              const int N, const float &alpha,
                                              Concurrency::array_view<float> &X, const int incX,
                                              const long xOffset)
{
    /*Check the conditions*/
    if (  N <= 0 ){
        return AMPBLAS_INVALID;
    }
    if ( alpha == 0){
        return AMPBLAS_SUCCESS;
    }
    sscal_AMP(accl_view, N, alpha, X, incX, xOffset);

    return AMPBLAS_SUCCESS;

}

// SSCAL TYpe III - Overloaded function with arguments related to batch processing 
ampblasStatus Ampblaslibrary :: ampblas_sscal(Concurrency::accelerator_view &accl_view,
                                                const int N,const float &alpha,
                                                Concurrency::array_view<float> &X, const int incX,
                                                const long xOffset, const long X_batchOffset, const int batchSize)
{
    /*Check the conditions*/
    if (  N <= 0 ){
        return AMPBLAS_INVALID;
    }
    if ( alpha == 0){
        return AMPBLAS_SUCCESS;
    }
    sscal_AMP(accl_view, N, alpha, X, incX, xOffset, X_batchOffset, batchSize);

    return AMPBLAS_SUCCESS;

}

