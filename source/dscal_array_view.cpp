#include "hcblas.h"
#include <amp.h>
#include "amp_math.h"
using namespace concurrency::fast_math;
using namespace concurrency;
#define BLOCK_SIZE 8

void dscal_HC(Concurrency::accelerator_view &accl_view,
              long n, double alpha,
              Concurrency::array_view<double> &X, long incx, long xOffset) {
  long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  Concurrency::extent<1> compute_domain(size);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<BLOCK_SIZE>(), [ = ] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp) {
    if(tidx.global[0] < n) {
      long X_index = xOffset + tidx.global[0];
      X[X_index] = (isnan(X[X_index]) || isinf(X[X_index])) ? 0 : X[X_index];
      X[X_index] = X[X_index] * alpha;
    }
  });
}

void dscal_HC(Concurrency::accelerator_view &accl_view,
              long n, double alpha,
              Concurrency::array_view<double> &X, long incx, long xOffset,
              long X_batchOffset, int batchSize) {
  long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
  Concurrency::extent<2> compute_domain(batchSize, size);
  Concurrency::parallel_for_each(accl_view, compute_domain.tile<1, BLOCK_SIZE>(), [ = ] (Concurrency::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp) {
    int elt = tidx.tile[0];

    if(tidx.global[1] < n) {
      long X_index = xOffset + X_batchOffset * elt + tidx.global[1];
      X[X_index] = (isnan(X[X_index]) || isinf(X[X_index])) ? 0 : X[X_index];
      X[X_index] = X[X_index] * alpha;
    }
  });
}

// DSCAL Call Type II: Inputs and outputs are C++ HC float array_View containers
hcblasStatus Hcblaslibrary :: hcblas_dscal(Concurrency::accelerator_view &accl_view,
				           const int N, const double &alpha,
   					   Concurrency::array_view<double> &X, const int incX,
   					   const long xOffset) {
  /*Check the conditions*/
  if (  N <= 0 || incX <= 0 ) {
    return HCBLAS_INVALID;
  }

  int lenX = 1 + (N - 1) * abs(incX);
   if ( alpha == 0 ) {
    for (int i = 0; i < lenX; i++) {
      X[xOffset + i] = 0.0;
    }
    return HCBLAS_SUCCESS;
   }

  dscal_HC(accl_view, N, alpha, X, incX, xOffset);
  return HCBLAS_SUCCESS;
}

// DSCAL TYpe III - Overloaded function with arguments related to batch processing
hcblasStatus Hcblaslibrary :: hcblas_dscal(Concurrency::accelerator_view &accl_view,
   					   const int N, const double &alpha,
  					   Concurrency::array_view<double> &X, const int incX,
   					   const long xOffset, const long X_batchOffset, const int batchSize) {
  /*Check the conditions*/
  if (  N <= 0 || incX <= 0 ) {
    return HCBLAS_INVALID;
  }

  int lenX = 1 + (N - 1) * abs(incX);
  if ( alpha == 0 ) {
   for (int j = 0; j < batchSize; ++j) {
    for (int i = 0; i < lenX; i++) {
        X[xOffset + X_batchOffset * j + i] = 0.0;
    }
   }
   return HCBLAS_SUCCESS;
  }

  dscal_HC(accl_view, N, alpha, X, incX, xOffset, X_batchOffset, batchSize);
  return HCBLAS_SUCCESS;
}

