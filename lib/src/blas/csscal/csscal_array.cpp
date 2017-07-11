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

#define BLOCK_SIZE 8

void csscal_HC(hc::accelerator_view accl_view, __int64_t n, float alpha,
               hc::short_vector::float_2 *X, __int64_t incx,
               __int64_t xOffset) {
  __int64_t size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  hc::extent<1> compute_domain(size);
  hc::parallel_for_each(accl_view, compute_domain.tile(BLOCK_SIZE),
                        [=](hc::tiled_index<1> tidx)[[hc]] {
                          float XReal = 0.0;
                          float XImg = 0.0;
                          if (tidx.global[0] < n) {
                            __int64_t X_index = xOffset + tidx.global[0];
                            XReal = X[X_index].x;
                            XImg = X[X_index].y;
                            XReal = (isnan(XReal) || isinf(XReal)) ? 0 : XReal;
                            XImg = (isnan(XImg) || isinf(XImg)) ? 0 : XImg;
                            if (alpha == 0) {
                              X[X_index].x = 0.0;
                              X[X_index].y = 0.0;
                            } else {
                              X[X_index].x = XReal * alpha;
                              X[X_index].y = XImg * alpha;
                            }
                          }
                        }) _WAIT1;
}

void csscal_HC(hc::accelerator_view accl_view, __int64_t n, float alpha,
               hc::short_vector::float_2 *X, __int64_t incx, __int64_t xOffset,
               __int64_t X_batchOffset, int batchSize) {
  __int64_t size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  hc::extent<2> compute_domain(batchSize, size);
  hc::parallel_for_each(accl_view, compute_domain.tile(1, BLOCK_SIZE), [=
  ](hc::tiled_index<2> tidx)[[hc]] {
    int elt = tidx.tile[0];
    float XReal = 0.0;
    float XImg = 0.0;
    if (tidx.global[1] < n) {
      __int64_t X_index = xOffset + X_batchOffset * elt + tidx.global[1];
      XReal = X[X_index].x;
      XImg = X[X_index].y;
      XReal = (isnan(XReal) || isinf(XReal)) ? 0 : XReal;
      XImg = (isnan(XImg) || isinf(XImg)) ? 0 : XImg;
      if (alpha == 0) {
        X[X_index].x = 0.0;
        X[X_index].y = 0.0;
      } else {
        X[X_index].x = XReal * alpha;
        X[X_index].y = XImg * alpha;
      }
    }
  }) _WAIT1;
}

// SSCAL Call Type I: Inputs and outputs are HCC device pointers
hcblasStatus Hcblaslibrary::hcblas_csscal(hc::accelerator_view accl_view,
                                          const int N, const float &alpha,
                                          hc::short_vector::float_2 *X,
                                          const int incX,
                                          const __int64_t xOffset) {
  /*Check the conditions*/
  if (X == NULL || N <= 0 || incX <= 0) {
    return HCBLAS_INVALID;
  }
  csscal_HC(accl_view, N, alpha, X, incX, xOffset);
  return HCBLAS_SUCCEEDS;
}

// SSCAL Type II - Overloaded function with arguments related to batch
// processing
hcblasStatus Hcblaslibrary::hcblas_csscal(
    hc::accelerator_view accl_view, const int N, const float &alpha,
    hc::short_vector::float_2 *X, const int incX, const __int64_t xOffset,
    const __int64_t X_batchOffset, const int batchSize) {
  /*Check the conditions*/
  if (X == NULL || N <= 0 || incX <= 0) {
    return HCBLAS_INVALID;
  }
  csscal_HC(accl_view, N, alpha, X, incX, xOffset, X_batchOffset, batchSize);
  return HCBLAS_SUCCEEDS;
}

