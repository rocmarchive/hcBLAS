#include "hcblas.h"
#include <amp.h>
#include "amp_math.h"
using namespace concurrency::fast_math;
using namespace concurrency;

void ger_HC(Concurrency::accelerator_view &accl_view,
            long m, long n, float alpha,
            Concurrency::array_view<float> &x, long xOffset, long incx,
            Concurrency::array_view<float> &y, long yOffset, long incy,
            Concurrency::array_view<float> &a, long aOffset, long lda) {
  long M = (m + 15) & ~15;
  long N = (n + 15) & ~15;
  Concurrency::extent<2> compute_domain(M, N);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<16, 16>(), [ = ] (Concurrency::tiled_index<16, 16> tidx) restrict(amp) {
    int i = tidx.global[0];
    int j = tidx.global[1];

    if(i < m && j < n) {
      long a_index = aOffset + j * lda + i;
      a[a_index] = (isnan(a[a_index]) || isinf(a[a_index])) ? 0 : a[a_index];
      a[a_index] += x[xOffset + i] * y[yOffset + j] * alpha;
    }
  });
}

void ger_HC(Concurrency::accelerator_view &accl_view,
            long m, long n, float alpha,
            Concurrency::array_view<float> &x,
            long xOffset, long X_batchOffset, long incx,
            Concurrency::array_view<float> &y,
            long yOffset, long Y_batchOffset, long incy,
            Concurrency::array_view<float> &a,
            long aOffset, long A_batchOffset, long lda, int batchSize) {
  long M = (m + 15) & ~15;
  long N = (n + 15) & ~15;
  Concurrency::extent<3> compute_domain(batchSize, M, N);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<1, 16, 16>(), [ = ] (Concurrency::tiled_index<1, 16, 16> tidx) restrict(amp) {
    int elt = tidx.tile[0];
    int i = tidx.global[1];
    int j = tidx.global[2];

    if(i < m && j < n) {
      long a_index = aOffset + A_batchOffset * elt + j * lda + i;
      a[a_index] = (isnan(a[a_index]) || isinf(a[a_index])) ? 0 : a[a_index];
      a[a_index] += x[xOffset + X_batchOffset * elt + i] * y[yOffset + Y_batchOffset * elt + j] * alpha;
    }
  });
}

void ger_HC_rMajor(Concurrency::accelerator_view &accl_view,
                   long m, long n, float alpha,
                   Concurrency::array_view<float> &x, long xOffset, long incx,
                   Concurrency::array_view<float> &y, long yOffset, long incy,
                   Concurrency::array_view<float> &a, long aOffset, long lda) {
  long M = (m + 15) & ~15;
  long N = (n + 15) & ~15;
  Concurrency::extent<2> compute_domain(N, M);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<16, 16>(), [ = ] (Concurrency::tiled_index<16, 16> tidx) restrict(amp) {
    int i = tidx.global[1];
    int j = tidx.global[0];

    if(i < m && j < n) {
      long a_index = aOffset + j + i * lda;
      a[a_index] = (isnan(a[a_index]) || isinf(a[a_index])) ? 0 : a[a_index];
      a[a_index] += x[xOffset + i] * y[yOffset + j] * alpha;
    }
  });
}

void ger_HC_rMajor(Concurrency::accelerator_view &accl_view,
                   long m, long n, float alpha,
                   Concurrency::array_view<float> &x,
                   long xOffset, long X_batchOffset, long incx,
                   Concurrency::array_view<float> &y,
                   long yOffset, long Y_batchOffset, long incy,
                   Concurrency::array_view<float> &a,
                   long aOffset, long A_batchOffset, long lda, int batchSize) {
  long M = (m + 15) & ~15;
  long N = (n + 15) & ~15;
  Concurrency::extent<3> compute_domain(batchSize, N, M);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<1, 16, 16>(), [ = ] (Concurrency::tiled_index<1, 16, 16> tidx) restrict(amp) {
    int elt = tidx.tile[0];
    int i = tidx.global[2];
    int j = tidx.global[1];

    if(i < m && j < n) {
      long a_index = aOffset + A_batchOffset * elt + j + i * lda;
      a[a_index] = (isnan(a[a_index]) || isinf(a[a_index])) ? 0 : a[a_index];
      a[a_index] += x[xOffset + X_batchOffset * elt + i] * y[yOffset + Y_batchOffset * elt + j] * alpha;
    }
  });
}

hcblasStatus Hcblaslibrary ::hcblas_sger(Concurrency::accelerator_view &accl_view, hcblasOrder order,
                                         const int M, const int N, const float &alpha,
                                         Concurrency::array_view<float> &X, const long xOffset, const int incX,
                                         Concurrency::array_view<float> &Y, const long yOffset, const int incY,
                                         Concurrency::array_view<float> &A, const long aOffset, const int lda) {
  /*Check the conditions*/
  if (N <= 0 || M <= 0 || incX == 0 || incY == 0) {
    return HCBLAS_INVALID;
  }

  if(alpha == 0) {
    return HCBLAS_SUCCESS;
  }

  if(order) {
    ger_HC(accl_view, M, N, alpha, X, xOffset, incX, Y, yOffset, incY, A, aOffset, M);
  } else {
    ger_HC_rMajor(accl_view, M, N, alpha, X, xOffset, incX, Y, yOffset, incY, A, aOffset, N);
  }

  return HCBLAS_SUCCESS;
}

hcblasStatus Hcblaslibrary :: hcblas_sger(Concurrency::accelerator_view &accl_view, hcblasOrder order,
                                          const int M, const int N, const float &alpha,
                                          Concurrency::array_view<float> &X,
                                          const long xOffset, const long X_batchOffset, const int incX,
                                          Concurrency::array_view<float> &Y,
                                          const long yOffset, const long Y_batchOffset, const int incY,
                                          Concurrency::array_view<float> &A,
                                          const long aOffset, const long A_batchOffset, const int lda, int batchSize) {
  /*Check the conditions*/
  if (N <= 0 || M <= 0 || incX == 0 || incY == 0) {
    return HCBLAS_INVALID;
  }

  if(alpha == 0) {
    return HCBLAS_SUCCESS;
  }

  if(order) {
    ger_HC(accl_view, M, N, alpha, X, xOffset, X_batchOffset, incX, Y, yOffset, Y_batchOffset, incY, A, aOffset, A_batchOffset, M, batchSize);
  } else {
    ger_HC_rMajor(accl_view, M, N, alpha, X, xOffset, X_batchOffset, incX, Y, yOffset, Y_batchOffset, incY, A, aOffset, A_batchOffset, N, batchSize);
  }

  return HCBLAS_SUCCESS;
}
