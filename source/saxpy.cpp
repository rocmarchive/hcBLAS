#include "hcblas.h"
#include <amp.h>
#include "amp_math.h"
using namespace concurrency::fast_math;
using namespace concurrency;
#define BLOCK_SIZE 256

void axpy_HC(Concurrency::accelerator_view &accl_view,
             long n, float alpha,
             Concurrency::array<float> &X, long xOffset, long incx,
             Concurrency::array<float> &Y, long yOffset, long incy) {
  if(n <= 102400) {
    long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    Concurrency::extent<1> compute_domain(size);
    Concurrency::parallel_for_each(accl_view, compute_domain.tile<BLOCK_SIZE>(), [ =, &X, &Y] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp) {
      if(tidx.global[0] < n) {
        long Y_index = yOffset + tidx.global[0];
        Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
        Y[Y_index] += X[xOffset + tidx.global[0]] * alpha;
      }
    });
  } else {
    int step_sz;

    if (n <= 409600) {
      step_sz = 3;
    } else if (n <= 921600) {
      step_sz = 5;
    } else if ( n <= 1939526) {
      step_sz = 7;
    } else {
      step_sz = 15;
    }

    long size = (n / step_sz + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    long nBlocks = size / BLOCK_SIZE;
    Concurrency::extent<1> compute_domain(size);
    Concurrency::parallel_for_each(accl_view, compute_domain.tile<BLOCK_SIZE>(), [ =, &X, &Y] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp) {
      if(tidx.tile[0] != nBlocks - 1) {
        for(int iter = 0; iter < step_sz; iter++) {
          long Y_index = yOffset + tidx.tile[0] * 256 * step_sz + tidx.local[0] + iter * 256;
          Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
          Y[Y_index] += X[xOffset + tidx.tile[0] * 256 * step_sz + tidx.local[0] + iter * 256] * alpha;
        }
      } else {
        int n_iter = ((n - ((nBlocks - 1) * 256 * step_sz)) / 256) + 1;

        for(int iter = 0; iter < n_iter; iter++) {
          if (((nBlocks - 1) * 256 * step_sz + iter * 256 + tidx.local[0]) < n) {
            long Y_index = yOffset + tidx.tile[0] * 256 * step_sz + tidx.local[0] + iter * 256;
            Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
            Y[Y_index] += X[xOffset + tidx.tile[0] * 256 * step_sz + tidx.local[0] + iter * 256] * alpha;
          }
        }
      }
    });
  }
}

void axpy_HC(Concurrency::accelerator_view &accl_view,
             long n, float alpha,
             Concurrency::array<float> &X, long xOffset, long incx,
             Concurrency::array<float> &Y, long yOffset, long incy,
             long X_batchOffset, long Y_batchOffset, int batchSize) {
  if(n <= 102400) {
    long size = (n + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    Concurrency::extent<2> compute_domain(batchSize, size);
    Concurrency::parallel_for_each(accl_view, compute_domain.tile<1, BLOCK_SIZE>(), [ =, &X, &Y] (Concurrency::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp) {
      int elt = tidx.tile[0];

      if(tidx.global[1] < n) {
        long Y_index = yOffset + Y_batchOffset * elt + tidx.global[1];
        Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
        Y[Y_index] += X[xOffset + X_batchOffset * elt + tidx.global[1]] * alpha;
      }
    });
  } else {
    int step_sz;

    if (n <= 409600) {
      step_sz = 3;
    } else if (n <= 921600) {
      step_sz = 5;
    } else if ( n <= 1939526) {
      step_sz = 7;
    } else {
      step_sz = 15;
    }

    long size = (n / step_sz + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
    long nBlocks = size / BLOCK_SIZE;
    Concurrency::extent<2> compute_domain(batchSize, size);
    Concurrency::parallel_for_each(accl_view, compute_domain.tile<1, BLOCK_SIZE>(), [ =, &X, &Y] (Concurrency::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp) {
      int elt = tidx.tile[0];

      if(tidx.tile[1] != nBlocks - 1) {
        for(int iter = 0; iter < step_sz; iter++) {
          long Y_index = yOffset +  Y_batchOffset * elt + tidx.tile[1] * 256 * step_sz + tidx.local[1] + iter * 256;
          Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
          Y[Y_index] += X[xOffset +  X_batchOffset * elt + tidx.tile[1] * 256 * step_sz + tidx.local[1] + iter * 256] * alpha;
        }
      } else {
        int n_iter = ((n - ((nBlocks - 1) * 256 * step_sz)) / 256) + 1;

        for(int iter = 0; iter < n_iter; iter++) {
          if (((nBlocks - 1) * 256 * step_sz + iter * 256 + tidx.local[1]) < n) {
            long Y_index = yOffset +  Y_batchOffset * elt + tidx.tile[1] * 256 * step_sz + tidx.local[1] + iter * 256;
            Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
            Y[Y_index] += X[xOffset +  X_batchOffset * elt + tidx.tile[1] * 256 * step_sz + tidx.local[1] + iter * 256] * alpha;
          }
        }
      }
    });
  }
}



hcblasStatus Hcblaslibrary :: hcblas_saxpy(const int N, const float* alpha,
                                           float* X, const int incX,
				           float* Y, const int incY,
				           const long xOffset, const long yOffset) {
  if (alpha == NULL || X == NULL || Y == NULL || N <= 0 ) {
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
  axpy_HC(accl_view, N, *alpha, xView, xOffset, incX, yView, yOffset, incY);
  Concurrency::copy(yView, begin(HostY));

  for(int i = 0 ; i < lenY; i++) {
    Y[i] = HostY[i];
  }

  return HCBLAS_SUCCESS;
}


hcblasStatus Hcblaslibrary :: hcblas_saxpy(Concurrency::accelerator_view &accl_view,
				           const int N, const float &alpha,
				           Concurrency::array<float> &X, const int incX,
				           Concurrency::array<float> &Y, const int incY,
				           const long xOffset, const long yOffset)

{
  /*Check the conditions*/
  if (  N <= 0 ) {
    return HCBLAS_INVALID;
  }

  if ( alpha == 0) {
    return HCBLAS_SUCCESS;
  }

  axpy_HC(accl_view, N, alpha, X, xOffset, incX, Y, yOffset, incY);
  return HCBLAS_SUCCESS;
}


hcblasStatus  Hcblaslibrary :: hcblas_saxpy(Concurrency::accelerator_view &accl_view,
					    const int N, const float &alpha,
					    Concurrency::array<float> &X, const int incX, const long X_batchOffset,
					    Concurrency::array<float> &Y, const int incY, const long Y_batchOffset,
					    const long xOffset, const long yOffset, const int batchSize)

{
  /*Check the conditions*/
  if (  N <= 0 ) {
    return HCBLAS_INVALID;
  }

  if ( alpha == 0) {
    return HCBLAS_SUCCESS;
  }

  axpy_HC(accl_view, N, alpha, X, xOffset, incX, Y, yOffset, incY, X_batchOffset, Y_batchOffset, batchSize);
  return HCBLAS_SUCCESS;
}



