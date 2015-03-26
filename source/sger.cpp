#include "ampblaslib.h"
#include <amp.h>

using namespace concurrency;

void ger_AMP(long m, long n, float alpha,
             Concurrency::array_view<float> &x, long xOffset, long incx,
             Concurrency::array_view<float> &y, long yOffset, long incy,
             Concurrency::array_view<float> &a, long aOffset, long lda)
{
  long M = (m + 15) & ~15;
  long N = (n + 15) & ~15;
  Concurrency::extent<2> compute_domain(M, N);
  Concurrency::parallel_for_each(compute_domain.tile<16, 16>(),[=] (Concurrency::tiled_index<16, 16> tidx) restrict(amp)
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
    long lenX = 1 + (N - 1) * abs(incX);
    long lenY = 1 + (M - 1) * abs(incY);

    array_view<float> xView(lenX, X);
    array_view<float> yView(lenY, Y);
    array_view<float> aMat(M*N, A);
    
    ger_AMP(M, N, *alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, M);
    aMat.synchronize();

    /* Debug purpose */
    /*for (int i = 0 ; i < M * N ;i++ )
      cout<< "A[" << i << "]" << A[i] << "\n" << endl; 
    */

    return AMPBLAS_SUCCESS;
}
