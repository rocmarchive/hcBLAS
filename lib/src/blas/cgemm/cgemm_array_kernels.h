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

#ifndef LIB_SRC_BLAS_CGEMM_CGEMM_ARRAY_KERNELS_H_
#define LIB_SRC_BLAS_CGEMM_CGEMM_ARRAY_KERNELS_H_

#include "include/hcblaslib.h"
#include <assert.h>
#include <hc.hpp>
#include <hc_math.hpp>
#include <hc_short_vector.hpp>
#include <iostream>
#include <vector>

#define TILE_SZ_A 64
#define TILE_SZ_B 16
#define TILE_SZ_RATIO (TILE_SZ_A / TILE_SZ_B)
#define TOTMICROTILEPROD (TILESIZE * TILESIZE * MICROTILESIZE)
#define MICROTILEPROD (TILESIZE * MICROTILESIZE)
#define BANKMICROTILESIZE (TILESIZE * MICROTILESIZE + 1)

#define M1x1(offset)                                            \
  rAreal[0][0] = lAreal[offA + 0];                              \
  rBreal[0][0] = lBreal[offB + 0];                              \
  rAimg[0][0] = lAimg[offA + 0];                                \
  rBimg[0][0] = lBimg[offB + 0];                                \
  offA += offset;                                               \
  offB += offset;                                               \
  rCreal[0][0] = rCreal[0][0] + (rAreal[0][0] * rBreal[0][0]) - \
                 (rAimg[0][0] * rBimg[0][0]);                   \
  rCimg[0][0] = rCimg[0][0] + (rAreal[0][0] * rBimg[0][0]) +    \
                (rAimg[0][0] * rBreal[0][0]);

#define MS1x1(offset)                                                   \
  for (int iter = 0; iter < STEPSIZE / TILESIZE; ++iter) {              \
    rAreal[0][iter] = lAreal[offA + (TILESIZE * TILESIZE) * iter];      \
    rBreal[0][iter] = lBreal[offB + (TILESIZE * TILESIZE) * iter];      \
    rAimg[0][iter] = lAimg[offA + (TILESIZE * TILESIZE) * iter];        \
    rBimg[0][iter] = lBimg[offB + (TILESIZE * TILESIZE) * iter];        \
    rCreal[0][0] = rCreal[0][0] + (rAreal[0][iter] * rBreal[0][iter]) - \
                   (rAimg[0][iter] * rBimg[0][iter]);                   \
    rCimg[0][0] = rCimg[0][0] + (rAreal[0][iter] * rBimg[0][iter]) +    \
                  (rAimg[0][iter] * rBreal[0][iter]);                   \
  }                                                                     \
  offA += offset;                                                       \
  offB += offset;

#define MTS                                                                    \
  for (int iter = 0; iter < MICROTILESIZE; iter++) {                           \
    rAreal[0][iter] = lAreal[offA + (iter * TILESIZE)];                        \
    rBreal[0][iter] = lBreal[offB + (iter * TILESIZE)];                        \
    rAimg[0][iter] = lAimg[offA + (iter * TILESIZE)];                          \
    rBimg[0][iter] = lBimg[offB + (iter * TILESIZE)];                          \
  }                                                                            \
  for (int rowIndex = 0; rowIndex < MICROTILESIZE; rowIndex++) {               \
    for (int colIndex = 0; colIndex < MICROTILESIZE; colIndex++) {             \
      rCreal[rowIndex][colIndex] =                                             \
          (rAreal[0][rowIndex] * rBreal[0][colIndex] -                         \
           rAimg[0][rowIndex] * rBimg[0][colIndex]) +                          \
          rCreal[rowIndex][colIndex];                                          \
      rCimg[rowIndex][colIndex] = (rAreal[0][rowIndex] * rBimg[0][colIndex] +  \
                                   rAimg[0][rowIndex] * rBreal[0][colIndex]) + \
                                  rCimg[rowIndex][colIndex];                   \
    }                                                                          \
  }                                                                            \
  offA += BANKMICROTILESIZE;                                                   \
  offB += BANKMICROTILESIZE;

/*
*  CGEMM kernels - column major Order
*/
hcblasStatus cgemm_NoTransAB(hc::accelerator_view accl_view,
                             hc::short_vector::float_2 *A, __int64_t aOffset,
                             hc::short_vector::float_2 *B, __int64_t bOffset,
                             hc::short_vector::float_2 *C, __int64_t cOffset,
                             int M, int N, int K, int lda, int ldb, int ldc,
                             hc::short_vector::float_2 alpha,
                             hc::short_vector::float_2 beta);

hcblasStatus cgemm_NoTransA(hc::accelerator_view accl_view,
                            hc::short_vector::float_2 *A, __int64_t aOffset,
                            hc::short_vector::float_2 *B, __int64_t bOffset,
                            hc::short_vector::float_2 *C, __int64_t cOffset,
                            int M, int N, int K, int lda, int ldb, int ldc,
                            hc::short_vector::float_2 alpha,
                            hc::short_vector::float_2 beta);

hcblasStatus cgemm_NoTransB(hc::accelerator_view accl_view,
                            hc::short_vector::float_2 *A, __int64_t aOffset,
                            hc::short_vector::float_2 *B, __int64_t bOffset,
                            hc::short_vector::float_2 *C, __int64_t cOffset,
                            int M, int N, int K, int lda, int ldb, int ldc,
                            hc::short_vector::float_2 alpha,
                            hc::short_vector::float_2 beta);

hcblasStatus cgemm_TransAB(hc::accelerator_view accl_view,
                           hc::short_vector::float_2 *A, __int64_t aOffset,
                           hc::short_vector::float_2 *B, __int64_t bOffset,
                           hc::short_vector::float_2 *C, __int64_t cOffset,
                           int M, int N, int K, int lda, int ldb, int ldc,
                           hc::short_vector::float_2 alpha,
                           hc::short_vector::float_2 beta);

/*
* CGEMM Kernels for Batch processing in column major order
*/

hcblasStatus cgemm_NoTransAB(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A[],
    __int64_t aOffset, __int64_t A_batchOffset, hc::short_vector::float_2 *B[],
    __int64_t bOffset, __int64_t B_batchOffset, hc::short_vector::float_2 *C[],
    __int64_t cOffset, __int64_t C_batchOffset, int M, int N, int K, int lda,
    int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta, int batchSize);

hcblasStatus cgemm_NoTransA(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A[],
    __int64_t aOffset, __int64_t A_batchOffset, hc::short_vector::float_2 *B[],
    __int64_t bOffset, __int64_t B_batchOffset, hc::short_vector::float_2 *C[],
    __int64_t cOffset, __int64_t C_batchOffset, int M, int N, int K, int lda,
    int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta, int batchSize);

hcblasStatus cgemm_NoTransB(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A[],
    __int64_t aOffset, __int64_t A_batchOffset, hc::short_vector::float_2 *B[],
    __int64_t bOffset, __int64_t B_batchOffset, hc::short_vector::float_2 *C[],
    __int64_t cOffset, __int64_t C_batchOffset, int M, int N, int K, int lda,
    int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta, int batchSize);

hcblasStatus cgemm_TransAB(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A[],
    __int64_t aOffset, __int64_t A_batchOffset, hc::short_vector::float_2 *B[],
    __int64_t bOffset, __int64_t B_batchOffset, hc::short_vector::float_2 *C[],
    __int64_t cOffset, __int64_t C_batchOffset, int M, int N, int K, int lda,
    int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta, int batchSize);

/*
* CGEMM Kernels - Row major order
*/

hcblasStatus cgemm_NoTransAB_rMajor(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A,
    __int64_t aOffset, hc::short_vector::float_2 *B, __int64_t bOffset,
    hc::short_vector::float_2 *C, __int64_t cOffset, int M, int N, int K,
    int lda, int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta);

hcblasStatus cgemm_NoTransA_rMajor(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A,
    __int64_t aOffset, hc::short_vector::float_2 *B, __int64_t bOffset,
    hc::short_vector::float_2 *C, __int64_t cOffset, int M, int N, int K,
    int lda, int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta);

hcblasStatus cgemm_NoTransB_rMajor(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A,
    __int64_t aOffset, hc::short_vector::float_2 *B, __int64_t bOffset,
    hc::short_vector::float_2 *C, __int64_t cOffset, int M, int N, int K,
    int lda, int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta);

hcblasStatus cgemm_TransAB_rMajor(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A,
    __int64_t aOffset, hc::short_vector::float_2 *B, __int64_t bOffset,
    hc::short_vector::float_2 *C, __int64_t cOffset, int M, int N, int K,
    int lda, int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta);

/*
* CGEMM Kernels for Batch-processing in Row major order
*/

hcblasStatus cgemm_NoTransAB_rMajor(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A[],
    __int64_t aOffset, __int64_t A_batchOffset, hc::short_vector::float_2 *B[],
    __int64_t bOffset, __int64_t B_batchOffset, hc::short_vector::float_2 *C[],
    __int64_t cOffset, __int64_t C_batchOffset, int M, int N, int K, int lda,
    int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta, int batchSize);

hcblasStatus cgemm_NoTransA_rMajor(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A[],
    __int64_t aOffset, __int64_t A_batchOffset, hc::short_vector::float_2 *B[],
    __int64_t bOffset, __int64_t B_batchOffset, hc::short_vector::float_2 *C[],
    __int64_t cOffset, __int64_t C_batchOffset, int M, int N, int K, int lda,
    int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta, int batchSize);

hcblasStatus cgemm_NoTransB_rMajor(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A[],
    __int64_t aOffset, __int64_t A_batchOffset, hc::short_vector::float_2 *B[],
    __int64_t bOffset, __int64_t B_batchOffset, hc::short_vector::float_2 *C[],
    __int64_t cOffset, __int64_t C_batchOffset, int M, int N, int K, int lda,
    int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta, int batchSize);

hcblasStatus cgemm_TransAB_rMajor(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A[],
    __int64_t aOffset, __int64_t A_batchOffset, hc::short_vector::float_2 *B[],
    __int64_t bOffset, __int64_t B_batchOffset, hc::short_vector::float_2 *C[],
    __int64_t cOffset, __int64_t C_batchOffset, int M, int N, int K, int lda,
    int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta, int batchSize);

#endif  // LIB_SRC_BLAS_CGEMM_CGEMM_ARRAY_KERNELS_H_
