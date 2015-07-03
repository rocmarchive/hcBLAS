#include "ampblaslib.h"
#include <amp.h>

using namespace concurrency;

void ger_AMP(Concurrency::accelerator_view &accl_view,
	     long m, long n, float alpha,
             Concurrency::array_view<float> &x, long xOffset, long incx,
             Concurrency::array_view<float> &y, long yOffset, long incy,
             Concurrency::array_view<float> &a, long aOffset, long lda)
{
  long M = (m + 15) & ~15;
  long N = (n + 15) & ~15;
  Concurrency::extent<2> compute_domain(M, N);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<16, 16>(),[=] (Concurrency::tiled_index<16, 16> tidx) restrict(amp)
  {
    int i = tidx.global[0];
    int j = tidx.global[1];
    if(i < m && j < n)
      a[aOffset + j*m + i] += x[xOffset + i] * y[yOffset + j] * alpha;
  });
}

ampblasStatus Ampblaslibrary :: ampblas_sger(const int M, const int N,
                                             const float *alpha, float *X,
                                             const long xOffset, const int incX,
                                             float *Y, const long yOffset,
                                             const int incY, float *A,
                                             const long aOffset, const int lda)
{
   
    if (alpha == NULL || X == NULL || Y == NULL || N <= 0 || M <= 0 || A == NULL || incX == 0 || incY == 0) {
        return AMPBLAS_INVALID;
    }

    long lenX = 1 + (M - 1) * abs(incX);
    long lenY = 1 + (N - 1) * abs(incY);

    array_view<float> xView(lenX, X);
    array_view<float> yView(lenY, Y);
    array_view<float> aMat(M*N, A);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

    
    ger_AMP(accl_view, M, N, *alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, M);
    aMat.synchronize();

    /* Debug purpose */
    /*for (int i = 0 ; i < M * N ;i++ )
      cout<< "A[" << i << "]" << A[i] << "\n" << endl; 
    */

    return AMPBLAS_SUCCESS;
}


ampblasStatus Ampblaslibrary ::ampblas_sger(Concurrency::accelerator_view &accl_view,
                                            const int M, const int N, const float &alpha,
                                            Concurrency::array_view<float> &X, const long xOffset, const int incX,
                                            Concurrency::array_view<float> &Y, const long yOffset, const int incY,
                                            Concurrency::array_view<float> &A, const long aOffset, const int lda)
{ 
    /*Check the conditions*/
    if (N <= 0 || M <= 0 || incX == 0 || incY == 0) {
        return AMPBLAS_INVALID;
    }

    if(alpha == 0)
	return AMPBLAS_SUCCESS;

    ger_AMP(accl_view, M, N, alpha, X, xOffset, incX, Y, yOffset, incY, A, aOffset, M);
    A.synchronize();
    return AMPBLAS_SUCCESS;
}




