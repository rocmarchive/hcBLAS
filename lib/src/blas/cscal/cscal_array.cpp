#include "hcblaslib.h"
#include <hc.hpp>
#include "hc_math.hpp"
using namespace hc::fast_math;

using namespace hc;
#define BLOCK_SIZE 8

void cscal_HC(hc::accelerator_view accl_view,
              long n, float_2 alpha,
              float_2 *X, long incx, long xOffset) {
  long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  hc::extent<1> compute_domain(size);
  hc::parallel_for_each(accl_view, compute_domain.tile(BLOCK_SIZE), [ = ] (hc::tiled_index<1> tidx) [[hc]] {
    float XReal = 0.0;
    float XImg = 0.0;
    if(tidx.global[0] < n) {
      long X_index = xOffset + tidx.global[0];
      XReal = X[X_index].x;
      XImg = X[X_index].y;
      XReal = (isnan(XReal) || isinf(XReal)) ? 0 : XReal;
      XImg = (isnan(XImg) || isinf(XImg)) ? 0 : XImg;
    if (alpha == 0) {
      X[X_index].x = 0.0;
      X[X_index].y = 0.0;
    }
    else {
      X[X_index].x = (XReal * alpha.x) - (XImg * alpha.y);
      X[X_index].y = (XReal * alpha.x) + (XImg * alpha.y);
    }
    }
  })_WAIT1;
}

void cscal_HC(hc::accelerator_view accl_view,
              long n, float_2 alpha,
              float_2 *X, long incx, long xOffset,
              long X_batchOffset, int batchSize) {
  long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  hc::extent<2> compute_domain(batchSize, size);
  hc::parallel_for_each(accl_view, compute_domain.tile(1, BLOCK_SIZE), [ = ] (hc::tiled_index<2> tidx) [[hc]] {
    int elt = tidx.tile[0];
    float XReal = 0.0;
    float XImg = 0.0;
    if(tidx.global[1] < n) {
      long X_index = xOffset + X_batchOffset * elt + tidx.global[1];
      XReal = X[X_index].x;
      XImg = X[X_index].y;
      XReal = (isnan(XReal) || isinf(XReal)) ? 0 : XReal;
      XImg = (isnan(XImg) || isinf(XImg)) ? 0 : XImg;
    if (alpha == 0) {
      X[X_index].x = 0.0;
      X[X_index].y = 0.0;
    }
    else {
      X[X_index].x = (XReal * alpha.x) - (XImg * alpha.y);
      X[X_index].y = (XReal * alpha.x) + (XImg * alpha.y);
    }
    }
  })_WAIT1;
}

// SSCAL Call Type I: Inputs and outputs are HCC device pointers
hcblasStatus Hcblaslibrary :: hcblas_cscal(hc::accelerator_view accl_view,
				           const int N, const float_2 &alpha,
				           float_2 *X, const int incX,
				           const long xOffset) {
  /*Check the conditions*/
  if ( X == NULL || N <= 0 || incX <= 0 ) {
    return HCBLAS_INVALID;
  }
  cscal_HC(accl_view, N, alpha, X, incX, xOffset);
  return HCBLAS_SUCCEEDS;
}

// SSCAL Type II - Overloaded function with arguments related to batch processing
hcblasStatus Hcblaslibrary :: hcblas_cscal(hc::accelerator_view accl_view,
				           const int N, const float_2 &alpha,
				           float_2 *X, const int incX,
				           const long xOffset, const long X_batchOffset, const int batchSize) {
  /*Check the conditions*/
  if ( X == NULL || N <= 0 || incX <= 0 ) {
    return HCBLAS_INVALID;
  }
  cscal_HC(accl_view, N, alpha, X, incX, xOffset, X_batchOffset, batchSize);
  return HCBLAS_SUCCEEDS;
}

