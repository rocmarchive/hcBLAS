#include "hcblaslib.h"
#include <hc.hpp>
#include "hc_math.hpp"
using namespace hc::fast_math;

using namespace hc;
#define BLOCK_SIZE 8

void dcopy_HC(hc::accelerator_view &accl_view, long n,
              hc::array<double> &X, long incx, long xOffset,
              hc::array<double> &Y, long incy, long yOffset) {
  long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  hc::extent<1> compute_domain(size);
  hc::parallel_for_each(accl_view, compute_domain.tile(BLOCK_SIZE), [ =, &X, &Y] (hc::tiled_index<1>& tidx) __attribute__((hc, cpu)) {
    if(tidx.global[0] < n) {
      long Y_index = yOffset + tidx.global[0];
      Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
      Y[Y_index] = X[xOffset + tidx.global[0]];
    }
  });
}

void dcopy_HC(hc::accelerator_view &accl_view, long n,
              hc::array<double> &X, long incx, long xOffset,
              hc::array<double> &Y, long incy, long yOffset,
              long X_batchOffset, long Y_batchOffset, int batchSize) {
  long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  hc::extent<2> compute_domain(batchSize, size);
  hc::parallel_for_each(accl_view, compute_domain.tile(1, BLOCK_SIZE), [ =, &X, &Y] (hc::tiled_index<2>& tidx) __attribute__((hc, cpu)) {
    int elt = tidx.tile[0];

    if(tidx.global[1] < n) {
      long Y_index = yOffset + Y_batchOffset * elt + tidx.global[1];
      Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
      Y[Y_index] = X[xOffset + X_batchOffset * elt + tidx.global[1]];
    }
  });
}

// DCOPY call Type I - SSCAL Inputs and Outputs are host float pointers
hcblasStatus Hcblaslibrary :: hcblas_dcopy(const int N, double* X, const int incX, const long xOffset, double* Y, const int incY, const long yOffset) {
  if (Y == NULL || X == NULL || N <= 0 || incX <= 0 || incY <= 0 ) {
    return HCBLAS_INVALID;
  }

  int lenX = 1 + (N - 1) * abs(incX);
  int lenY = 1 + (N - 1) * abs(incY);
  hc::array<double> xView(lenX, X);
  hc::array<double> yView(lenY, Y);
  std::vector<double> HostX(lenX);
  std::vector<double> HostY(lenY);

  for( int i = 0; i < lenX; i++) {
    HostX[i] = X[i];
  }

  for( int i = 0; i < lenY; i++) {
    HostY[i] = Y[i];
  }

  hc::copy(begin(HostX), end(HostX), xView);
  hc::copy(begin(HostY), end(HostY), yView);
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  dcopy_HC(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
  hc::copy(yView, begin(HostY));

  for(int i = 0 ; i < lenY; i++) {
    Y[i] = HostY[i];
  }

  return HCBLAS_SUCCEEDS;
}

// DCOPY Call Type II: Inputs and outputs are C++ HC float array containers
hcblasStatus Hcblaslibrary :: hcblas_dcopy(hc::accelerator_view &accl_view, const int N,
    					   hc::array<double> &X, const int incX, const long xOffset,
				           hc::array<double> &Y, const int incY, const long yOffset) {
  /*Check the conditions*/
  if (  N <= 0 || incX <= 0 || incY <= 0 ) {
    return HCBLAS_INVALID;
  }

  dcopy_HC(accl_view, N, X, incX, xOffset, Y, incY, yOffset);
  return HCBLAS_SUCCEEDS;
}

// DCOPY TYpe III - Overloaded function with arguments related to batch processing
hcblasStatus Hcblaslibrary :: hcblas_dcopy(hc::accelerator_view &accl_view, const int N,
				           hc::array<double> &X, const int incX, const long xOffset,
				           hc::array<double> &Y, const int incY, const long yOffset,
				           const long X_batchOffset, const long Y_batchOffset, const int batchSize) {
  /*Check the conditions*/
  if (  N <= 0 || incX <= 0 || incY <= 0 ) {
    return HCBLAS_INVALID;
  }

  dcopy_HC(accl_view, N, X, incX, xOffset, Y, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
  return HCBLAS_SUCCEEDS;
}

