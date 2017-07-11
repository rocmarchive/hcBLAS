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

#include "include/hcblas.h"
#include "include/hcblaslib.h"
#include "gtest/gtest.h"
#include <cblas.h>
#include <cstdlib>
#include <hc_am.hpp>
#include <hc_short_vector.hpp>

unsigned int global_seed = 100;

TEST(hcblas_cgemm, return_correct_cgemm_Implementation_type_1) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int M = 189, N = 9, K = 19;
  __int64_t lda, ldb, ldc;
  int incX = 1, incY = 1;
  __int64_t aOffset = 0;
  __int64_t bOffset = 0;
  __int64_t cOffset = 0;
  hcblasOrder hcOrder;
  hcblasTranspose typeA, typeB;
  hcblasStatus status;
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;

  // Implementation type I - Inputs and Outputs are HCC device pointers */

  hc::short_vector::float_2 cAlpha, cBeta;
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  hc::short_vector::float_2 *A = (hc::short_vector::float_2 *)calloc(
      M * K, sizeof(hc::short_vector::float_2));
  hc::short_vector::float_2 *B = (hc::short_vector::float_2 *)calloc(
      K * N, sizeof(hc::short_vector::float_2));
  hc::short_vector::float_2 *C = (hc::short_vector::float_2 *)calloc(
      M * N, sizeof(hc::short_vector::float_2));
  hc::short_vector::float_2 *devA =
      hc::am_alloc(sizeof(hc::short_vector::float_2) * M * K, acc, 0);
  hc::short_vector::float_2 *devB =
      hc::am_alloc(sizeof(hc::short_vector::float_2) * K * N, acc, 0);
  hc::short_vector::float_2 *devC =
      hc::am_alloc(sizeof(hc::short_vector::float_2) * M * N, acc, 0);
  for (int i = 0; i < M * K; i++) {
    A[i].x = rand_r(&global_seed) % 10;
    A[i].y = rand_r(&global_seed) % 20;
  }
  for (int i = 0; i < K * N; i++) {
    B[i].x = rand_r(&global_seed) % 15;
    B[i].y = rand_r(&global_seed) % 25;
  }
  for (int i = 0; i < M * N; i++) {
    C[i].x = rand_r(&global_seed) % 18;
    C[i].y = rand_r(&global_seed) % 28;
    C[i] = rand_r(&global_seed) % 25;
  }
  accl_view.copy(A, devA, M * K * sizeof(hc::short_vector::float_2));
  accl_view.copy(B, devB, K * N * sizeof(hc::short_vector::float_2));
  accl_view.copy(C, devC, M * N * sizeof(hc::short_vector::float_2));
  // NoTransA and NoTransB */
  typeA = NoTrans;
  typeB = NoTrans;
  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major */
  lda = K;
  ldb = N;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // NoTransA TransB */
  typeA = NoTrans;
  typeB = Trans;
  // Column major */
  lda = M;
  ldb = N;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major */
  lda = K;
  ldb = K;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // TransA NoTransB */
  typeA = Trans;
  typeB = NoTrans;
  // Column major */
  lda = K;
  ldb = K;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major */
  lda = M;
  ldb = N;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // TransA TransB */
  typeA = Trans;
  typeB = Trans;
  // Column major */
  lda = K;
  ldb = N;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major */
  lda = M;
  ldb = K;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  typeA = NoTrans;
  typeB = NoTrans;
  lda = M;
  ldb = K;
  ldc = M;
  hcOrder = ColMajor;
  hc::short_vector::float_2 *devA1 = NULL;
  hc::short_vector::float_2 *devB1 = NULL;
  hc::short_vector::float_2 *devC1 = NULL;
  /* A, B, C device pointers are not allocated properly */
  status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, K, cAlpha,
                           devA1, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_INVALID);
  status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB1, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_INVALID);
  status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC1,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // M is 0 */
  status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, 0, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // N is 0 */
  status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, 0, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // K is 0 */
  status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, 0, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_INVALID);
  free(A);
  free(B);
  free(C);
  hc::am_free(devA);
  hc::am_free(devB);
  hc::am_free(devC);
}

TEST(hcblas_cgemm, func_correct_cgemm_Implementation_type_1) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int M = 189, N = 9, K = 19;
  __int64_t lda, ldb, ldc;
  int incX = 1, incY = 1;
  __int64_t aOffset = 0;
  __int64_t bOffset = 0;
  __int64_t cOffset = 0;
  hcblasOrder hcOrder;
  hcblasTranspose typeA, typeB;
  hcblasStatus status;
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  CBLAS_TRANSPOSE Transa, Transb;
  float alpha[2], beta[2];
  // Implementation type I - Inputs and Outputs are HCC device pointers */
  hc::short_vector::float_2 cAlpha, cBeta;
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  hc::short_vector::float_2 *A = (hc::short_vector::float_2 *)calloc(
      M * K, sizeof(hc::short_vector::float_2));
  hc::short_vector::float_2 *B = (hc::short_vector::float_2 *)calloc(
      K * N, sizeof(hc::short_vector::float_2));
  hc::short_vector::float_2 *C = (hc::short_vector::float_2 *)calloc(
      M * N, sizeof(hc::short_vector::float_2));
  hc::short_vector::float_2 *devA =
      hc::am_alloc(sizeof(hc::short_vector::float_2) * M * K, acc, 0);
  hc::short_vector::float_2 *devB =
      hc::am_alloc(sizeof(hc::short_vector::float_2) * K * N, acc, 0);
  hc::short_vector::float_2 *devC =
      hc::am_alloc(sizeof(hc::short_vector::float_2) * M * N, acc, 0);
  float *ablas = (float *)malloc(sizeof(float) * M * K * 2);
  float *bblas = (float *)malloc(sizeof(float) * K * N * 2);
  float *cblas = (float *)malloc(sizeof(float) * M * N * 2);
  int k = 0;
  for (int i = 0; i < M * K; i++) {
    A[i].x = rand_r(&global_seed) % 10;
    A[i].y = rand_r(&global_seed) % 20;
    ablas[k++] = A[i].x;
    ablas[k++] = A[i].y;
  }
  k = 0;
  for (int i = 0; i < K * N; i++) {
    B[i].x = rand_r(&global_seed) % 15;
    B[i].y = rand_r(&global_seed) % 25;
    bblas[k++] = B[i].x;
    bblas[k++] = B[i].y;
  }
  k = 0;
  for (int i = 0; i < M * N; i++) {
    C[i].x = rand_r(&global_seed) % 18;
    C[i].y = rand_r(&global_seed) % 28;
    cblas[k++] = C[i].x;
    cblas[k++] = C[i].y;
  }
  accl_view.copy(A, devA, M * K * sizeof(hc::short_vector::float_2));
  accl_view.copy(B, devB, K * N * sizeof(hc::short_vector::float_2));
  accl_view.copy(C, devC, M * N * sizeof(hc::short_vector::float_2));

  // NoTransA and NoTransB */
  typeA = NoTrans;
  typeB = NoTrans;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  // Row Major */
  lda = K;
  ldb = N;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  // NoTransA TransB */
  typeA = NoTrans;
  typeB = Trans;
  Transa = CblasNoTrans;
  Transb = CblasTrans;

  // Column major */
  lda = M;
  ldb = N;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  /* alpha and beta are zeroes */
  /* alpha = 0*/
  cAlpha.x = 0;
  cAlpha.y = 0;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }
  /* alpha = 0, beta = 0*/
  cAlpha.x = 0;
  cAlpha.y = 0;
  cBeta.x = 0;
  cBeta.y = 0;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  // Row Major */
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  lda = K;
  ldb = K;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  /* alpha and beta are zeroes */
  /* alpha = 0*/
  cAlpha.x = 0;
  cAlpha.y = 0;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  /* alpha = 0, beta = 0*/
  cAlpha.x = 0;
  cAlpha.y = 0;
  cBeta.x = 0;
  cBeta.y = 0;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  // TransA NoTransB */
  typeA = Trans;
  typeB = NoTrans;
  Transa = CblasTrans;
  Transb = CblasNoTrans;
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;

  // Column major */
  lda = K;
  ldb = K;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  // Row Major */
  lda = M;
  ldb = N;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  // TransA TransB */
  typeA = Trans;
  typeB = Trans;
  Transa = CblasTrans;
  Transb = CblasTrans;

  // Column major */
  lda = K;
  ldb = N;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  // Row Major */
  lda = M;
  ldb = K;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC,
                           cOffset, ldc);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C, M * N * sizeof(hc::short_vector::float_2));
  cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
              ldb, &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  free(A);
  free(B);
  free(C);
  free(cblas);
  free(ablas);
  free(bblas);
  hc::am_free(devA);
  hc::am_free(devB);
  hc::am_free(devC);
}

TEST(hcblas_cgemm, return_correct_cgemm_Implementation_type_2) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int M = 189, N = 9, K = 19;
  __int64_t lda, ldb, ldc;
  int incX = 1, incY = 1;
  __int64_t aOffset = 0;
  __int64_t bOffset = 0;
  __int64_t cOffset = 0;
  __int64_t A_batchOffset = 0;
  __int64_t B_batchOffset = 0;
  __int64_t C_batchOffset = M * N;
  int batchSize = 64;
  hcblasOrder hcOrder;
  hcblasTranspose typeA, typeB;
  hcblasStatus status;
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  hc::short_vector::float_2 cAlpha, cBeta;
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;

  // Implementation type II - Inputs and Outputs are HCC device pointers with
  // batch processing
  hc::short_vector::float_2 *Abatch[batchSize];
  hc::short_vector::float_2 *Bbatch[batchSize];
  hc::short_vector::float_2 *Cbatch[batchSize];

  hc::short_vector::float_2 *devA[batchSize], *devB[batchSize],
      *devC[batchSize];

  hc::short_vector::float_2 **d_Aarray =
      hc::am_alloc(sizeof(hc::short_vector::float_2 *) * batchSize, acc, 0);
  hc::short_vector::float_2 **d_Barray =
      hc::am_alloc(sizeof(hc::short_vector::float_2 *) * batchSize, acc, 0);
  hc::short_vector::float_2 **d_Carray =
      hc::am_alloc(sizeof(hc::short_vector::float_2 *) * batchSize, acc, 0);

  for (int b = 0; b < batchSize; b++) {
    Abatch[b] = (hc::short_vector::float_2 *)calloc(
        M * K, sizeof(hc::short_vector::float_2));
    Bbatch[b] = (hc::short_vector::float_2 *)calloc(
        K * N, sizeof(hc::short_vector::float_2));
    Cbatch[b] = (hc::short_vector::float_2 *)calloc(
        M * N, sizeof(hc::short_vector::float_2));
    devA[b] = (hc::short_vector::float_2 *)hc::am_alloc(
        sizeof(hc::short_vector::float_2) * M * K, acc, 0);
    devB[b] = (hc::short_vector::float_2 *)hc::am_alloc(
        sizeof(hc::short_vector::float_2) * K * N, acc, 0);
    devC[b] = (hc::short_vector::float_2 *)hc::am_alloc(
        sizeof(hc::short_vector::float_2) * M * N, acc, 0);
  }

  for (int b = 0; b < batchSize; b++) {
    for (int i = 0; i < M * K; i++) {
      Abatch[b][i].x = rand_r(&global_seed) % 10;
      Abatch[b][i].y = rand_r(&global_seed) % 20;
    }
    for (int i = 0; i < K * N; i++) {
      Bbatch[b][i].x = rand_r(&global_seed) % 15;
      Bbatch[b][i].y = rand_r(&global_seed) % 25;
    }
    for (int i = 0; i < M * N; i++) {
      Cbatch[b][i].x = rand_r(&global_seed) % 18;
      Cbatch[b][i].y = rand_r(&global_seed) % 28;
    }
  }

  for (int b = 0; b < batchSize; b++) {
    accl_view.copy(Abatch[b], devA[b],
                   M * K * sizeof(hc::short_vector::float_2));
    accl_view.copy(Bbatch[b], devB[b],
                   K * N * sizeof(hc::short_vector::float_2));
    accl_view.copy(Cbatch[b], devC[b],
                   M * N * sizeof(hc::short_vector::float_2));
  }

  // Copyinng device pointers stored in host memory to device memory
  accl_view.copy(devA, d_Aarray,
                 batchSize * sizeof(hc::short_vector::float_2 *));
  accl_view.copy(devB, d_Barray,
                 batchSize * sizeof(hc::short_vector::float_2 *));
  accl_view.copy(devC, d_Carray,
                 batchSize * sizeof(hc::short_vector::float_2 *));

  // NoTransA and NoTransB
  typeA = NoTrans;
  typeB = NoTrans;
  // Column major
  lda = M;
  ldb = K;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major
  lda = K;
  ldb = N;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // NoTransA TransB
  typeA = NoTrans;
  typeB = Trans;
  // Column major
  lda = M;
  ldb = N;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major
  lda = K;
  ldb = K;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // TransA NoTransB
  typeA = Trans;
  typeB = NoTrans;
  // Column major
  lda = K;
  ldb = K;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major
  lda = M;
  ldb = N;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // TransA TransB
  typeA = Trans;
  typeB = Trans;
  // Column major
  lda = K;
  ldb = N;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major
  lda = M;
  ldb = K;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  typeA = NoTrans;
  typeB = NoTrans;
  lda = M;
  ldb = K;
  ldc = M;
  hc::short_vector::float_2 **devA1 = NULL;
  hc::short_vector::float_2 **devB1 = NULL;
  hc::short_vector::float_2 **devC1 = NULL;
  /* A, B, C device pointers are not allocated properly */
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           devA1, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, devB1,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, devC1, cOffset,
                           C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // M is 0
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, 0, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // N is 0
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, 0, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // K is 0
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, 0, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);

  for (int b = 0; b < batchSize; b++) {
    free(Abatch[b]);
    free(Bbatch[b]);
    free(Cbatch[b]);
    hc::am_free(devA[b]);
    hc::am_free(devB[b]);
    hc::am_free(devC[b]);
  }
  hc::am_free(d_Aarray);
  hc::am_free(d_Barray);
  hc::am_free(d_Carray);
}

TEST(hcblas_cgemm, func_correct_cgemm_Implementation_type_2) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int M = 189, N = 9, K = 19;
  __int64_t lda, ldb, ldc;
  int incX = 1, incY = 1;
  __int64_t aOffset = 0;
  __int64_t bOffset = 0;
  __int64_t cOffset = 0;
  __int64_t A_batchOffset = 0;
  __int64_t B_batchOffset = 0;
  __int64_t C_batchOffset = M * N;
  int batchSize = 64;
  hcblasOrder hcOrder;
  hcblasTranspose typeA, typeB;
  hcblasStatus status;
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  CBLAS_TRANSPOSE Transa, Transb;
  // Implementation type II - Inputs and Outputs are HCC device pointers with
  // batch processing
  float alpha[2], beta[2];
  hc::short_vector::float_2 cAlpha, cBeta;

  // Implementation type II - Inputs and Outputs are HCC device pointers with
  // batch processing
  hc::short_vector::float_2 *Abatch[batchSize];
  hc::short_vector::float_2 *Bbatch[batchSize];
  hc::short_vector::float_2 *Cbatch[batchSize];
  float *abatch[batchSize];
  float *bbatch[batchSize];
  float *cbatch[batchSize];

  hc::short_vector::float_2 *devA[batchSize], *devB[batchSize],
      *devC[batchSize];

  hc::short_vector::float_2 **d_Aarray =
      hc::am_alloc(sizeof(hc::short_vector::float_2 *) * batchSize, acc, 0);
  hc::short_vector::float_2 **d_Barray =
      hc::am_alloc(sizeof(hc::short_vector::float_2 *) * batchSize, acc, 0);
  hc::short_vector::float_2 **d_Carray =
      hc::am_alloc(sizeof(hc::short_vector::float_2 *) * batchSize, acc, 0);

  for (int b = 0; b < batchSize; b++) {
    Abatch[b] = (hc::short_vector::float_2 *)calloc(
        M * K, sizeof(hc::short_vector::float_2));
    Bbatch[b] = (hc::short_vector::float_2 *)calloc(
        K * N, sizeof(hc::short_vector::float_2));
    Cbatch[b] = (hc::short_vector::float_2 *)calloc(
        M * N, sizeof(hc::short_vector::float_2));
    abatch[b] = (float *)calloc(M * K * 2, sizeof(float));
    bbatch[b] = (float *)calloc(K * N * 2, sizeof(float));
    cbatch[b] = (float *)calloc(M * N * 2, sizeof(float));
    devA[b] = hc::am_alloc(sizeof(hc::short_vector::float_2) * M * K, acc, 0);
    devB[b] = hc::am_alloc(sizeof(hc::short_vector::float_2) * K * N, acc, 0);
    devC[b] = hc::am_alloc(sizeof(hc::short_vector::float_2) * M * N, acc, 0);
  }

  for (int b = 0; b < batchSize; b++) {
    int k = 0;
    for (int i = 0; i < M * K; i++) {
      Abatch[b][i].x = rand_r(&global_seed) % 10;
      Abatch[b][i].y = rand_r(&global_seed) % 20;
      abatch[b][k++] = Abatch[b][i].x;
      abatch[b][k++] = Abatch[b][i].y;
    }
    k = 0;
    for (int i = 0; i < K * N; i++) {
      Bbatch[b][i].x = rand_r(&global_seed) % 15;
      Bbatch[b][i].y = rand_r(&global_seed) % 25;
      bbatch[b][k++] = Bbatch[b][i].x;
      bbatch[b][k++] = Bbatch[b][i].y;
    }
    k = 0;
    for (int i = 0; i < M * N; i++) {
      Cbatch[b][i].x = rand_r(&global_seed) % 18;
      Cbatch[b][i].y = rand_r(&global_seed) % 28;
      cbatch[b][k++] = Cbatch[b][i].x;
      cbatch[b][k++] = Cbatch[b][i].y;
    }
  }

  for (int b = 0; b < batchSize; b++) {
    accl_view.copy(Abatch[b], devA[b],
                   M * K * sizeof(hc::short_vector::float_2));
    accl_view.copy(Bbatch[b], devB[b],
                   K * N * sizeof(hc::short_vector::float_2));
    accl_view.copy(Cbatch[b], devC[b],
                   M * N * sizeof(hc::short_vector::float_2));
  }

  // Copyinng device pointers stored in host memory to device memory
  accl_view.copy(devA, d_Aarray,
                 batchSize * sizeof(hc::short_vector::float_2 *));
  accl_view.copy(devB, d_Barray,
                 batchSize * sizeof(hc::short_vector::float_2 *));
  accl_view.copy(devC, d_Carray,
                 batchSize * sizeof(hc::short_vector::float_2 *));

  // NoTransA and NoTransB
  typeA = NoTrans;
  typeB = NoTrans;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Column major
  lda = M;
  ldb = K;
  ldc = M;
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }

  /* alpha = 0 */
  lda = M;
  ldb = K;
  ldc = M;
  cAlpha.x = 0;
  cAlpha.y = 0;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }
  /* alpha = 0, beta = 0*/
  lda = M;
  ldb = K;
  ldc = M;
  cAlpha.x = 0;
  cAlpha.y = 0;
  cBeta.x = 0;
  cBeta.y = 0;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }

  // Row Major
  lda = K;
  ldb = N;
  ldc = N;
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }

  /* alpha = 0 */
  lda = K;
  ldb = N;
  ldc = N;
  cAlpha.x = 0;
  cAlpha.y = 0;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }

  /* alpha = 0, beta = 0 */
  lda = K;
  ldb = N;
  ldc = N;
  cAlpha.x = 0;
  cAlpha.y = 0;
  cBeta.x = 0;
  cBeta.y = 0;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }

  // NoTransA TransB
  typeA = NoTrans;
  typeB = Trans;
  Transa = CblasNoTrans;
  Transb = CblasTrans;

  // Column major
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  lda = M;
  ldb = N;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }

  // Row Major
  lda = K;
  ldb = K;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }

  // TransA NoTransB
  typeA = Trans;
  typeB = NoTrans;
  Transa = CblasTrans;
  Transb = CblasNoTrans;

  // Column major
  lda = K;
  ldb = K;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }

  // Row Major
  lda = M;
  ldb = N;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }

  // TransA TransB
  typeA = Trans;
  typeB = Trans;
  Transa = CblasTrans;
  Transb = CblasTrans;

  // Column major
  lda = K;
  ldb = N;
  ldc = M;
  status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasColMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }

  // Row Major
  lda = M;
  ldb = K;
  ldc = N;
  status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha,
                           d_Aarray, aOffset, A_batchOffset, lda, d_Barray,
                           bOffset, B_batchOffset, ldb, cBeta, d_Carray,
                           cOffset, C_batchOffset, ldc, batchSize);
  for (int b = 0; b < batchSize; b++)
    accl_view.copy(devC[b], Cbatch[b],
                   M * N * sizeof(hc::short_vector::float_2));
  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(CblasRowMajor, Transa, Transb, M, N, K, &alpha, abatch[b], lda,
                bbatch[b], ldb, &beta, cbatch[b], ldc);
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(Cbatch[b][i].x, cbatch[b][k]);
      EXPECT_EQ(Cbatch[b][i].y, cbatch[b][k + 1]);
    }
  }
  for (int b = 0; b < batchSize; b++) {
    free(Abatch[b]);
    free(Bbatch[b]);
    free(Cbatch[b]);
    free(abatch[b]);
    free(bbatch[b]);
    free(cbatch[b]);
    hc::am_free(devA[b]);
    hc::am_free(devB[b]);
    hc::am_free(devC[b]);
  }
  hc::am_free(d_Aarray);
  hc::am_free(d_Barray);
  hc::am_free(d_Carray);
}
