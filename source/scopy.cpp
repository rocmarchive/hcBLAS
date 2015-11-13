#include "hcblas.h"
#include <amp.h>
#include "amp_math.h"
using namespace concurrency::fast_math;

using namespace concurrency;
#define BLOCK_SIZE 8

void scopy_HC(Concurrency::accelerator_view &accl_view, long n,
              Concurrency::array<float> &X, long incx, long xOffset,
              Concurrency::array<float> &Y, long incy, long yOffset) {
  long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  Concurrency::extent<1> compute_domain(size);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<BLOCK_SIZE>(), [ =, &X, &Y] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp) {
    if(tidx.global[0] < n) {
      long Y_index = yOffset + tidx.global[0];
      Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
      Y[Y_index] = X[xOffset + tidx.global[0]];
    }
  });
}

void scopy_HC(Concurrency::accelerator_view &accl_view, long n,
              Concurrency::array<float> &X, long incx, long xOffset,
              Concurrency::array<float> &Y, long incy, long yOffset,
              long X_batchOffset, long Y_batchOffset, int batchSize) {
  long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  Concurrency::extent<2> compute_domain(batchSize, size);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<1, BLOCK_SIZE>(), [ =, &X, &Y] (Concurrency::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp) {
    int elt = tidx.tile[0];

    if(tidx.global[1] < n) {
      long Y_index = yOffset + Y_batchOffset * elt + tidx.global[1];
      Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
      Y[Y_index] = X[xOffset + X_batchOffset * elt + tidx.global[1]];
    }
  });
}

// SCOPY call Type I - SSCAL Inputs and Outputs are host float pointers
hcblasStatus Hcblaslibrary :: hcblas_scopy(const int N, float* X, const int incX, const long xOffset,
   					   float* Y, const int incY, const long yOffset) {
  if (Y == NULL || X == NULL || N <= 0 || incX <= 0 || incY <= 0 ) {
    return HCBLAS_INVALID;
  }

  int lenX = 1 + (N - 1) * abs(incX);
  int lenY = 1 + (N - 1) * abs(incY);
  Concurrency::array<float> xView(lenX, X);
  Concurrency::array<float> yView(lenY, Y);
  std::vector<float> HostX(lenX);
  std::vector<float> HostY(lenY);

  for( int i = 0; i < lenX; i++) {
    HostX[i] = X[i];
  }

  for( int i = 0; i < lenY; i++) {
    HostY[i] = Y[i];
  }

  Concurrency::copy(begin(HostX), end(HostX), xView);
  Concurrency::copy(begin(HostY), end(HostY), yView);
  std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  scopy_HC(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
  Concurrency::copy(yView, begin(HostY));

  for(int i = 0 ; i < lenY; i++) {
    Y[i] = HostY[i];
  }

  return HCBLAS_SUCCESS;
}

// SCOPY Call Type II: Inputs and outputs are C++ HC float array containers
hcblasStatus Hcblaslibrary :: hcblas_scopy(Concurrency::accelerator_view &accl_view, const int N,
				           Concurrency::array<float> &X, const int incX, const long xOffset,
				           Concurrency::array<float> &Y, const int incY, const long yOffset) {
  /*Check the conditions*/
  if (  N <= 0 || incX <= 0 || incY <= 0 ) {
    return HCBLAS_INVALID;
  }

  scopy_HC(accl_view, N, X, incX, xOffset, Y, incY, yOffset);
  return HCBLAS_SUCCESS;
}

// SCOPY TYpe III - Overloaded function with arguments related to batch processing
hcblasStatus Hcblaslibrary :: hcblas_scopy(Concurrency::accelerator_view &accl_view, const int N,
				           Concurrency::array<float> &X, const int incX, const long xOffset,
				           Concurrency::array<float> &Y, const int incY, const long yOffset,
				           const long X_batchOffset, const long Y_batchOffset, const int batchSize) {
  /*Check the conditions*/
  if (  N <= 0 || incX <= 0 || incY <= 0 ) {
    return HCBLAS_INVALID;
  }

  scopy_HC(accl_view, N, X, incX, xOffset, Y, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
  return HCBLAS_SUCCESS;
}

