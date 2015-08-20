#include "ampblaslib.h"
#include <amp.h>

using namespace concurrency;

void ger_AMP(Concurrency::accelerator_view &accl_view,
	     long m, long n, float alpha,
             Concurrency::array<float> &x, long xOffset, long incx,
             Concurrency::array<float> &y, long yOffset, long incy,
             Concurrency::array<float> &a, long aOffset, long lda)
{
  long M = (m + 15) & ~15;
  long N = (n + 15) & ~15;
  Concurrency::extent<2> compute_domain(M, N);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<16, 16>(),[=, &x, &y, &a] (Concurrency::tiled_index<16, 16> tidx) restrict(amp)
  {
    int i = tidx.global[0];
    int j = tidx.global[1];
    if(i < m && j < n)
      a[aOffset + j*m + i] += x[xOffset + i] * y[yOffset + j] * alpha;
  });
}

void ger_AMP(Concurrency::accelerator_view &accl_view,
             long m, long n, float alpha,
             Concurrency::array<float> &x, 
             long xOffset, long X_batchOffset, long incx,
             Concurrency::array<float> &y, 
             long yOffset, long Y_batchOffset, long incy,
             Concurrency::array<float> &a, 
             long aOffset, long A_batchOffset, long lda, int batchSize)
{
  long M = (m + 15) & ~15;
  long N = (n + 15) & ~15;
  Concurrency::extent<3> compute_domain(batchSize, M, N);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<1, 16, 16>(),[=, &x, &y, &a] (Concurrency::tiled_index<1, 16, 16> tidx) restrict(amp)
  {  
    int elt = tidx.tile[0];
    int i = tidx.global[1];
    int j = tidx.global[2];
    if(i < m && j < n)
      a[aOffset + A_batchOffset * elt + j*m + i] += x[xOffset + X_batchOffset * elt + i] * y[yOffset + Y_batchOffset * elt + j] * alpha;
  });
}

void ger_AMP_rMajor(Concurrency::accelerator_view &accl_view,
             long m, long n, float alpha,
             Concurrency::array<float> &x, long xOffset, long incx,
             Concurrency::array<float> &y, long yOffset, long incy,
             Concurrency::array<float> &a, long aOffset, long lda)
{
  long M = (m + 15) & ~15;
  long N = (n + 15) & ~15;
  Concurrency::extent<2> compute_domain(N, M);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<16, 16>(),[=, &x, &y, &a] (Concurrency::tiled_index<16, 16> tidx) restrict(amp)
  {
    int i = tidx.global[1];
    int j = tidx.global[0];
    if(i < m && j < n)
      a[aOffset + j + i * n] += x[xOffset + i] * y[yOffset + j] * alpha;
  });
}

void ger_AMP_rMajor(Concurrency::accelerator_view &accl_view,
             long m, long n, float alpha,
             Concurrency::array<float> &x,
             long xOffset, long X_batchOffset, long incx,
             Concurrency::array<float> &y,
             long yOffset, long Y_batchOffset, long incy,
             Concurrency::array<float> &a,
             long aOffset, long A_batchOffset, long lda, int batchSize)
{
  long M = (m + 15) & ~15;
  long N = (n + 15) & ~15;
  Concurrency::extent<3> compute_domain(batchSize, N, M);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<1, 16, 16>(),[=, &x, &y, &a] (Concurrency::tiled_index<1, 16, 16> tidx) restrict(amp)
  {
    int elt = tidx.tile[0];
    int i = tidx.global[2];
    int j = tidx.global[1];
    if(i < m && j < n)
      a[aOffset + A_batchOffset * elt + j + i * n] += x[xOffset + X_batchOffset * elt + i] * y[yOffset + Y_batchOffset * elt + j] * alpha;
  });
}

ampblasStatus Ampblaslibrary :: ampblas_sger(const enum AMPBLAS_ORDER order, const int M, const int N,
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

    Concurrency::array<float> xView(lenX, X);
    Concurrency::array<float> yView(lenY, Y);
    Concurrency::array<float> aMat(M*N, A);
    std::vector<float> HostX(lenX);
    std::vector<float> HostY(lenY);
    std::vector<float> HostA(M * N);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    for( int i = 0; i < lenX; i++)
	HostX[i] = X[i];
    for( int i = 0; i < lenY; i++)
        HostY[i] = Y[i];    
    for( int i = 0; i < M * N; i++)
	HostA[i] = A[i];
    Concurrency::copy(begin(HostX), end(HostX), xView);
    Concurrency::copy(begin(HostY), end(HostY), yView);
    Concurrency::copy(begin(HostA), end(HostA), aMat);
    if(order)
    	ger_AMP(accl_view, M, N, *alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, M);
    else
        ger_AMP_rMajor(accl_view, M, N, *alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, M);
    Concurrency::copy(aMat, begin(HostA));
    for( int i = 0; i < M * N; i++)
        A[i] = HostA[i];
    return AMPBLAS_SUCCESS;
}


ampblasStatus Ampblaslibrary ::ampblas_sger(Concurrency::accelerator_view &accl_view, const enum AMPBLAS_ORDER order,
                                            const int M, const int N, const float &alpha,
                                            Concurrency::array<float> &X, const long xOffset, const int incX,
                                            Concurrency::array<float> &Y, const long yOffset, const int incY,
                                            Concurrency::array<float> &A, const long aOffset, const int lda)
{ 
    /*Check the conditions*/
    if (N <= 0 || M <= 0 || incX == 0 || incY == 0) {
        return AMPBLAS_INVALID;
    }

    if(alpha == 0)
	return AMPBLAS_SUCCESS;

    if(order)
	ger_AMP(accl_view, M, N, alpha, X, xOffset, incX, Y, yOffset, incY, A, aOffset, M);
    else
        ger_AMP_rMajor(accl_view, M, N, alpha, X, xOffset, incX, Y, yOffset, incY, A, aOffset, M);
    return AMPBLAS_SUCCESS;
}

ampblasStatus Ampblaslibrary :: ampblas_sger(Concurrency::accelerator_view &accl_view, const enum AMPBLAS_ORDER order,
                                             const int M, const int N, const float &alpha,
                               		     Concurrency::array<float> &X,
                                             const long xOffset, const long X_batchOffset, const int incX,
                                             Concurrency::array<float> &Y,
                                             const long yOffset, const long Y_batchOffset, const int incY,
                                             Concurrency::array<float> &A,
                                             const long aOffset, const long A_batchOffset, const int lda, int batchSize)

{
    /*Check the conditions*/
    if (N <= 0 || M <= 0 || incX == 0 || incY == 0) {
        return AMPBLAS_INVALID;
    }

    if(alpha == 0)
        return AMPBLAS_SUCCESS;
   
    if(order)
        ger_AMP(accl_view, M, N, alpha, X, xOffset, X_batchOffset, incX, Y, yOffset, Y_batchOffset, incY, A, aOffset, A_batchOffset, M, batchSize);
    else
        ger_AMP_rMajor(accl_view, M, N, alpha, X, xOffset, X_batchOffset, incX, Y, yOffset, Y_batchOffset, incY, A, aOffset, A_batchOffset, M, batchSize);

    return AMPBLAS_SUCCESS;
}




