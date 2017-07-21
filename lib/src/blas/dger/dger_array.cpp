/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "include/hcblaslib.h"
#include <hc.hpp>
#include <hc_math.hpp>

void ger_HC(hc::accelerator_view accl_view, __int64_t m, __int64_t n,
            double alpha, const double *x, __int64_t xOffset, __int64_t incx,
            const double *y, __int64_t yOffset, __int64_t incy, double *a,
            __int64_t aOffset, __int64_t lda) {
  __int64_t M = (m + 15) & ~15;
  __int64_t N = (n + 15) & ~15;
  hc::extent<2> compute_domain(M, N);
  hc::parallel_for_each(accl_view, compute_domain.tile(16, 16), [=
  ](hc::tiled_index<2> tidx)[[hc]] {
    int i = tidx.global[0];
    int j = tidx.global[1];

    if (i < m && j < n) {
      __int64_t a_index = aOffset + j * lda + i;
      a[a_index] = (hc::fast_math::isnan(static_cast<float>(a[a_index])) ||
                    hc::fast_math::isinf(static_cast<float>(a[a_index])))
                       ? 0
                       : a[a_index];
      a[a_index] += x[xOffset + i] * y[yOffset + j] * alpha;
    }
  }) ;
}

void ger_HC(hc::accelerator_view accl_view, __int64_t m, __int64_t n,
            double alpha, const double *x, __int64_t xOffset,
            __int64_t X_batchOffset, __int64_t incx, const double *y,
            __int64_t yOffset, __int64_t Y_batchOffset, __int64_t incy,
            double *a, __int64_t aOffset, __int64_t A_batchOffset,
            __int64_t lda, int batchSize) {
  __int64_t M = (m + 15) & ~15;
  __int64_t N = (n + 15) & ~15;
  hc::extent<3> compute_domain(batchSize, M, N);
  hc::parallel_for_each(accl_view, compute_domain.tile(1, 16, 16), [=
  ](hc::tiled_index<3> tidx)[[hc]] {
    int elt = tidx.tile[0];
    int i = tidx.global[1];
    int j = tidx.global[2];

    if (i < m && j < n) {
      __int64_t a_index = aOffset + A_batchOffset * elt + j * lda + i;
      a[a_index] = (hc::fast_math::isnan(static_cast<float>(a[a_index])) ||
                    hc::fast_math::isinf(static_cast<float>(a[a_index])))
                       ? 0
                       : a[a_index];
      a[a_index] += x[xOffset + X_batchOffset * elt + i] *
                    y[yOffset + Y_batchOffset * elt + j] * alpha;
    }
  }) ;
}

void ger_HC_rMajor(hc::accelerator_view accl_view, __int64_t m, __int64_t n,
                   double alpha, const double *x, __int64_t xOffset,
                   __int64_t incx, const double *y, __int64_t yOffset,
                   __int64_t incy, double *a, __int64_t aOffset,
                   __int64_t lda) {
  __int64_t M = (m + 15) & ~15;
  __int64_t N = (n + 15) & ~15;
  hc::extent<2> compute_domain(N, M);
  hc::parallel_for_each(accl_view, compute_domain.tile(16, 16), [=
  ](hc::tiled_index<2> tidx)[[hc]] {
    int i = tidx.global[1];
    int j = tidx.global[0];

    if (i < m && j < n) {
      __int64_t a_index = aOffset + j + i * lda;
      a[a_index] = (hc::fast_math::isnan(static_cast<float>(a[a_index])) ||
                    hc::fast_math::isinf(static_cast<float>(a[a_index])))
                       ? 0
                       : a[a_index];
      a[a_index] += x[xOffset + i] * y[yOffset + j] * alpha;
    }
  }) ;
}

void ger_HC_rMajor(hc::accelerator_view accl_view, __int64_t m, __int64_t n,
                   double alpha, const double *x, __int64_t xOffset,
                   __int64_t X_batchOffset, __int64_t incx, const double *y,
                   __int64_t yOffset, __int64_t Y_batchOffset, __int64_t incy,
                   double *a, __int64_t aOffset, __int64_t A_batchOffset,
                   __int64_t lda, int batchSize) {
  __int64_t M = (m + 15) & ~15;
  __int64_t N = (n + 15) & ~15;
  hc::extent<3> compute_domain(batchSize, N, M);
  hc::parallel_for_each(accl_view, compute_domain.tile(1, 16, 16), [=
  ](hc::tiled_index<3> tidx)[[hc]] {
    int elt = tidx.tile[0];
    int i = tidx.global[2];
    int j = tidx.global[1];

    if (i < m && j < n) {
      __int64_t a_index = aOffset + A_batchOffset * elt + j + i * lda;
      a[a_index] = (hc::fast_math::isnan(static_cast<float>(a[a_index])) ||
                    hc::fast_math::isinf(static_cast<float>(a[a_index])))
                       ? 0
                       : a[a_index];
      a[a_index] += x[xOffset + X_batchOffset * elt + i] *
                    y[yOffset + Y_batchOffset * elt + j] * alpha;
    }
  }) ;
}

/* DGER - Type I : Inputs and outputs are double array containers */
hcblasStatus Hcblaslibrary::hcblas_dger(
    hc::accelerator_view accl_view, hcblasOrder order, const int M, const int N,
    const double &alpha, const double *X, const __int64_t xOffset,
    const int incX, const double *Y, const __int64_t yOffset, const int incY,
    double *A, const __int64_t aOffset, const int lda) {
  /*Check the conditions*/
  if (X == NULL || Y == NULL || A == NULL || N <= 0 || M <= 0 || incX == 0 ||
      incY == 0) {
    return HCBLAS_INVALID;
  }

  if (alpha == 0) {
    return HCBLAS_SUCCEEDS;
  }

  if (order) {
    ger_HC(accl_view, M, N, alpha, X, xOffset, incX, Y, yOffset, incY, A,
           aOffset, M);
  } else {
    ger_HC_rMajor(accl_view, M, N, alpha, X, xOffset, incX, Y, yOffset, incY, A,
                  aOffset, N);
  }

  return HCBLAS_SUCCEEDS;
}

/* DGER - Type II : Inputs and outputs are double array containers with batch
 * processing */
hcblasStatus Hcblaslibrary::hcblas_dger(
    hc::accelerator_view accl_view, hcblasOrder order, const int M, const int N,
    const double &alpha, const double *X, const __int64_t xOffset,
    const __int64_t X_batchOffset, const int incX, const double *Y,
    const __int64_t yOffset, const __int64_t Y_batchOffset, const int incY,
    double *A, const __int64_t aOffset, const __int64_t A_batchOffset,
    const int lda, int batchSize)

{
  /*Check the conditions*/
  if (X == NULL || Y == NULL || A == NULL || N <= 0 || M <= 0 || incX == 0 ||
      incY == 0) {
    return HCBLAS_INVALID;
  }

  if (alpha == 0) {
    return HCBLAS_SUCCEEDS;
  }

  if (order) {
    ger_HC(accl_view, M, N, alpha, X, xOffset, X_batchOffset, incX, Y, yOffset,
           Y_batchOffset, incY, A, aOffset, A_batchOffset, M, batchSize);
  } else {
    ger_HC_rMajor(accl_view, M, N, alpha, X, xOffset, X_batchOffset, incX, Y,
                  yOffset, Y_batchOffset, incY, A, aOffset, A_batchOffset, N,
                  batchSize);
  }

  return HCBLAS_SUCCEEDS;
}

