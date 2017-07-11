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

unsigned int global_seed = 100;

TEST(hcblas_dgemm, return_correct_dgemm_Implementation_type_1) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int M = 189, N = 9, K = 19;
  double alpha = 1, beta = 1;
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

  double *A = (double *)calloc(M * K, sizeof(double));
  double *B = (double *)calloc(K * N, sizeof(double));
  double *C = (double *)calloc(M * N, sizeof(double));
  double *devA = hc::am_alloc(sizeof(double) * M * K, acc, 0);
  double *devB = hc::am_alloc(sizeof(double) * K * N, acc, 0);
  double *devC = hc::am_alloc(sizeof(double) * M * N, acc, 0);
  for (int i = 0; i < M * K; i++) {
    A[i] = rand_r(&global_seed) % 100;
  }
  for (int i = 0; i < K * N; i++) {
    B[i] = rand_r(&global_seed) % 15;
  }
  for (int i = 0; i < M * N; i++) {
    C[i] = rand_r(&global_seed) % 25;
  }
  accl_view.copy(A, devA, M * K * sizeof(double));
  accl_view.copy(B, devB, K * N * sizeof(double));
  accl_view.copy(C, devC, M * N * sizeof(double));
  // NoTransA and NoTransB */
  typeA = NoTrans;
  typeB = NoTrans;
  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major */
  lda = K;
  ldb = N;
  ldc = N;
  status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // NoTransA TransB */
  typeA = NoTrans;
  typeB = Trans;
  // Column major */
  lda = M;
  ldb = N;
  ldc = M;
  status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major */
  lda = K;
  ldb = K;
  ldc = N;
  status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // TransA NoTransB */
  typeA = Trans;
  typeB = NoTrans;
  // Column major */
  lda = K;
  ldb = K;
  ldc = M;
  status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major */
  lda = M;
  ldb = N;
  ldc = N;
  status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // TransA TransB */
  typeA = Trans;
  typeB = Trans;
  // Column major */
  lda = K;
  ldb = N;
  ldc = M;
  status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  // Row Major */
  lda = M;
  ldb = K;
  ldc = N;
  status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  typeA = NoTrans;
  typeB = NoTrans;
  lda = M;
  ldb = K;
  ldc = M;
  hcOrder = ColMajor;
  double *devA1 = NULL;
  double *devB1 = NULL;
  double *devC1 = NULL;
  /* A, B, C device pointers are not allocated properly */
  status = hc.hcblas_dgemm(accl_view, hcOrder, typeA, typeB, M, N, K, alpha,
                           devA1, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_INVALID);
  status = hc.hcblas_dgemm(accl_view, hcOrder, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB1, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_INVALID);
  status = hc.hcblas_dgemm(accl_view, hcOrder, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC1, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // M is 0 */
  status = hc.hcblas_dgemm(accl_view, hcOrder, typeA, typeB, 0, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // N is 0 */
  status = hc.hcblas_dgemm(accl_view, hcOrder, typeA, typeB, M, 0, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // K is 0 */
  status = hc.hcblas_dgemm(accl_view, hcOrder, typeA, typeB, M, N, 0, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_INVALID);

  // add an explicit wait to sync on host
  accl_view.wait();
  free(A);
  free(B);
  free(C);
  hc::am_free(devA);
  hc::am_free(devB);
  hc::am_free(devC);
}

TEST(hcblas_dgemm, func_correct_dgemm_Implementation_type_1) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int M = 189, N = 9, K = 19;
  double alpha = 1, beta = 1;
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
  // Implementation type I - Inputs and Outputs are HCC device pointers */

  double *A = (double *)calloc(M * K, sizeof(double));
  double *B = (double *)calloc(K * N, sizeof(double));
  double *C = (double *)calloc(M * N, sizeof(double));
  double *C_hcblas = (double *)calloc(M * N, sizeof(double));
  double *C_cblas = (double *)calloc(M * N, sizeof(double));
  double *devA = hc::am_alloc(sizeof(double) * M * K, acc, 0);
  double *devB = hc::am_alloc(sizeof(double) * K * N, acc, 0);
  double *devC = hc::am_alloc(sizeof(double) * M * N, acc, 0);
  for (int i = 0; i < M * K; i++) {
    A[i] = rand_r(&global_seed) % 100;
  }
  for (int i = 0; i < K * N; i++) {
    B[i] = rand_r(&global_seed) % 15;
  }
  for (int i = 0; i < M * N; i++) {
    C[i] = rand_r(&global_seed) % 25;
    C_cblas[i] = C[i];
  }
  accl_view.copy(A, devA, M * K * sizeof(double));
  accl_view.copy(B, devB, K * N * sizeof(double));
  accl_view.copy(C, devC, M * N * sizeof(double));
  // NoTransA and NoTransB */
  typeA = NoTrans;
  typeB = NoTrans;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasColMajor, Transa, Transb, M, N, K, alpha, A, lda, B, ldb,
              beta, C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);

  // Row Major */
  lda = K;
  ldb = N;
  ldc = N;
  status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasRowMajor, Transa, Transb, M, N, K, alpha, A, lda, B, ldb,
              beta, C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);

  // NoTransA TransB */
  typeA = NoTrans;
  typeB = Trans;
  Transa = CblasNoTrans;
  Transb = CblasTrans;

  // Column major */
  lda = M;
  ldb = N;
  ldc = M;
  status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasColMajor, Transa, Transb, M, N, K, alpha, A, lda, B, ldb,
              beta, C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);

  /* alpha and beta are zeroes */
  /* alpha = 0*/
  lda = M;
  ldb = N;
  ldc = M;
  status =
      hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, 0, devA, lda,
                      devB, ldb, beta, devC, ldc, aOffset, bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasColMajor, Transa, Transb, M, N, K, 0, A, lda, B, ldb, beta,
              C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);
  /* alpha = 0, beta = 0*/
  lda = M;
  ldb = N;
  ldc = M;
  status =
      hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, 0, devA, lda,
                      devB, ldb, 0, devC, ldc, aOffset, bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasColMajor, Transa, Transb, M, N, K, 0, A, lda, B, ldb, 0,
              C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);

  // Row Major */
  lda = K;
  ldb = K;
  ldc = N;
  status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasRowMajor, Transa, Transb, M, N, K, alpha, A, lda, B, ldb,
              beta, C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);

  /* alpha and beta are zeroes */
  /* alpha = 0*/
  lda = K;
  ldb = K;
  ldc = N;
  status =
      hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, 0, devA, lda,
                      devB, ldb, beta, devC, ldc, aOffset, bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasRowMajor, Transa, Transb, M, N, K, 0, A, lda, B, ldb, beta,
              C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);
  /* alpha = 0, beta = 0*/
  lda = K;
  ldb = K;
  ldc = N;
  status =
      hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, 0, devA, lda,
                      devB, ldb, 0, devC, ldc, aOffset, bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasRowMajor, Transa, Transb, M, N, K, 0, A, lda, B, ldb, 0,
              C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);

  // TransA NoTransB */
  typeA = Trans;
  typeB = NoTrans;
  Transa = CblasTrans;
  Transb = CblasNoTrans;

  // Column major */
  lda = K;
  ldb = K;
  ldc = M;
  status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasColMajor, Transa, Transb, M, N, K, alpha, A, lda, B, ldb,
              beta, C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);

  // Row Major */
  lda = M;
  ldb = N;
  ldc = N;
  status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasRowMajor, Transa, Transb, M, N, K, alpha, A, lda, B, ldb,
              beta, C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);

  // TransA TransB */
  typeA = Trans;
  typeB = Trans;
  Transa = CblasTrans;
  Transb = CblasTrans;

  // Column major */
  lda = K;
  ldb = N;
  ldc = M;
  status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasColMajor, Transa, Transb, M, N, K, alpha, A, lda, B, ldb,
              beta, C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);

  // Row Major */
  lda = M;
  ldb = K;
  ldc = N;
  status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
                           devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                           bOffset, cOffset);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  accl_view.copy(devC, C_hcblas, M * N * sizeof(double));
  cblas_dgemm(CblasRowMajor, Transa, Transb, M, N, K, alpha, A, lda, B, ldb,
              beta, C_cblas, ldc);
  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);
  free(A);
  free(B);
  free(C);
  free(C_cblas);
  free(C_hcblas);
  hc::am_free(devA);
  hc::am_free(devB);
  hc::am_free(devC);
}

/*TEST(hcblas_dgemm, return_correct_dgemm_Implementation_type_2) {
   hc::accelerator accl;
   Hcblaslibrary hc(&av);
    int M = 189, N = 9, K = 19;
    double alpha = 1, beta = 1;
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
    accelerator_view accl_view = hc.currentAcclView;
    accelerator acc = hc.currentAccl;

   // Implementation type II - Inputs and Outputs are HCC double array
containers with batch processing

   double *Abatch = (double*) calloc(M * K, sizeof(double));
   double *Bbatch = (double*) calloc(K * N, sizeof(double));
   double *Cbatch = (double*) calloc(M * N * batchSize, sizeof(double));
   double* devAbatch = hc::am_alloc(sizeof(double) * M * K, acc, 0);
   double* devBbatch = hc::am_alloc(sizeof(double) * K * N, acc, 0);
   double* devCbatch = hc::am_alloc(sizeof(double) * M * N * batchSize, acc, 0);

   for(int i = 0; i < M * K; i++) {
                Abatch[i] = rand_r(&global_seed)%100;
   }
   for(int i = 0; i < K * N;i++) {
                Bbatch[i] = rand_r(&global_seed) % 15;
   }
   for(int i = 0; i < M * N * batchSize;i++) {
                Cbatch[i] = rand_r(&global_seed) % 25;
   }

   accl_view.copy(Abatch, devAbatch, M * K * sizeof(double));
   accl_view.copy(Bbatch, devBbatch, K * N * sizeof(double));
   accl_view.copy(Cbatch, devCbatch, M * N * batchSize * sizeof(double));

   // NoTransA and NoTransB
    typeA = NoTrans;
    typeB = NoTrans;
    // Column major
    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    // Row Major
    lda = K; ldb = N ; ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    // NoTransA TransB
    typeA = NoTrans;
    typeB = Trans;
    // Column major
    lda = M; ldb = N ; ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    // Row Major
    lda = K; ldb = K ; ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    // TransA NoTransB
    typeA = Trans;
    typeB = NoTrans;
    // Column major
    lda = K; ldb = K ; ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    // Row Major
    lda = M; ldb = N ; ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    // TransA TransB
    typeA = Trans;
    typeB = Trans;
    // Column major
    lda = K; ldb = N ; ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    // Row Major
    lda = M; ldb = K ; ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    typeA = NoTrans;
    typeB = NoTrans;
    lda = M; ldb = K ; ldc = M;
    double *devA1 = NULL;
    double *devB1 = NULL;
    double *devC1 = NULL;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
devA1, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch, ldc,
C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devB1, ldb, B_batchOffset, beta, devCbatch, ldc,
C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devC1, ldc,
C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    // M is 0
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, 0, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    // N is 0
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, 0, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    // K is 0
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, 0, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    free(Abatch);
    free(Bbatch);
    free(Cbatch);
    hc::am_free(devAbatch);
    hc::am_free(devBbatch);
    hc::am_free(devCbatch);
}  */

/*TEST(hcblas_dgemm, func_correct_dgemm_Implementation_type_2) {
   hc::accelerator accl;
   Hcblaslibrary hc(&av);
    int M = 189, N = 9, K = 19;
    double alpha = 1, beta = 1;
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
   // Implementation type II - Inputs and Outputs are HCC double array
containers with batch processing

   double *Abatch = (double*) calloc(M * K, sizeof(double));
   double *Bbatch = (double*) calloc(K * N, sizeof(double));
   double *Cbatch = (double*) calloc(M * N * batchSize, sizeof(double));
   double *CCblasbatch = (double*) calloc(M * N * batchSize, sizeof(double));
   double *Chcblasbatch = (double*) calloc(M * N * batchSize, sizeof(double));
   double* devAbatch = hc::am_alloc(sizeof(double) * M * K, acc, 0);
   double* devBbatch = hc::am_alloc(sizeof(double) * K * N, acc, 0);
   double* devCbatch = hc::am_alloc(sizeof(double) * M * N * batchSize, acc, 0);

   for(int i = 0; i < M * K; i++) {
                Abatch[i] = rand_r(&global_seed)%100;
   }
   for(int i = 0; i < K * N;i++) {
                Bbatch[i] = rand_r(&global_seed) % 15;
   }
   for(int i = 0; i < M * N * batchSize;i++) {
                Cbatch[i] = rand_r(&global_seed) % 25;
                CCblasbatch[i] = Cbatch[i];
   }

   accl_view.copy(Abatch, devAbatch, M * K * sizeof(double));
   accl_view.copy(Bbatch, devBbatch, K * N * sizeof(double));
   accl_view.copy(Cbatch, devCbatch, M * N * batchSize * sizeof(double));

   // NoTransA and NoTransB
    typeA = NoTrans;
    typeB = NoTrans;
    Transa = CblasNoTrans;
    Transb = CblasNoTrans;

    // Column major
    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasColMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, 0,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasColMajor, Transa, Transb, M, N, K, 0, Abatch, lda,
Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, 0,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, 0, devCbatch, ldc,
C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasColMajor, Transa, Transb, M, N, K, 0, Abatch, lda,
Bbatch, ldb, 0, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    // Row Major
    lda = K; ldb = N ; ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    lda = K; ldb = N ; ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, 0,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, 0, Abatch, lda,
Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    lda = K; ldb = N ; ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, 0,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, 0, devCbatch, ldc,
C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, 0, Abatch, lda,
Bbatch, ldb, 0, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    // NoTransA TransB
    typeA = NoTrans;
    typeB = Trans;
    Transa = CblasNoTrans;
    Transb = CblasTrans;

    // Column major
    lda = M; ldb = N ; ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasColMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    // Row Major
    lda = K; ldb = K ; ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    // TransA NoTransB
    typeA = Trans;
    typeB = NoTrans;
    Transa = CblasTrans;
    Transb = CblasNoTrans;

    // Column major
    lda = K; ldb = K ; ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasColMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    // Row Major
    lda = M; ldb = N ; ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    // TransA TransB
    typeA = Trans;
    typeB = Trans;
    Transa = CblasTrans;
    Transb = CblasTrans;

    // Column major
    lda = K; ldb = N ; ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasColMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    // Row Major
    lda = M; ldb = K ; ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch,
ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));
    for(int i = 0; i < batchSize; i++)
        cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    free(Abatch);
    free(Bbatch);
    free(Cbatch);
    free(CCblasbatch);
    free(Chcblasbatch);
    hc::am_free(devAbatch);
    hc::am_free(devBbatch);
    hc::am_free(devCbatch);

}*/

TEST(hcblas_dgemm, func_correct_dgemm_Implementation_type_2) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int M = 189, N = 9, K = 19;
  double alpha = 1, beta = 1;
  __int64_t lda, ldb, ldc;
  int incX = 1, incY = 1;
  __int64_t aOffset = 0;
  __int64_t bOffset = 0;
  __int64_t cOffset = 0;
  __int64_t A_batchOffset = 0;
  __int64_t B_batchOffset = 0;
  __int64_t C_batchOffset = 0;
  int batchSize = 64;
  hcblasOrder hcOrder;
  hcblasTranspose typeA, typeB;
  hcblasStatus status;
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  CBLAS_TRANSPOSE Transa, Transb;
  // Implementation type II - Inputs and Outputs are HCC double array containers
  // with batch processing
  double *A[batchSize];
  double *B[batchSize];
  double *C[batchSize];
  double *C_hcblas[batchSize];
  double *C_cblas[batchSize];
  // Device pointers stored in host memory
  double *devA[batchSize], *devB[batchSize], *devC[batchSize];
  // Create device double pointer to store device pointers in device memory
  double **d_Aarray = hc::am_alloc(batchSize * sizeof(double *), acc, 0);
  double **d_Barray = hc::am_alloc(batchSize * sizeof(double *), acc, 0);
  double **d_Carray = hc::am_alloc(batchSize * sizeof(double *), acc, 0);
  const size_t aSize = sizeof(double) * M * K;
  const size_t bSize = sizeof(double) * K * N;
  const size_t cSize = sizeof(double) * M * N;

  // Host and Device Array allocation
  for (int i = 0; i < batchSize; i++) {
    A[i] = (double *)malloc(aSize);
    B[i] = (double *)malloc(bSize);
    C[i] = (double *)malloc(cSize);
    C_hcblas[i] = (double *)malloc(cSize);
    C_cblas[i] = (double *)malloc(cSize);
    devA[i] = hc::am_alloc(aSize, acc, 0);
    devB[i] = hc::am_alloc(aSize, acc, 0);
    devC[i] = hc::am_alloc(aSize, acc, 0);
  }
  // Populate the inputs
  for (int b = 0; b < batchSize; b++) {
    // Populate each subscript of array
    for (int i = 0; i < M * K; i++) {
      A[b][i] = rand_r(&global_seed) % 100;
    }
    accl_view.copy(A[b], devA[b], sizeof(double) * M * K);
    for (int i = 0; i < K * N; i++) {
      B[b][i] = rand_r(&global_seed) % 15;
    }
    accl_view.copy(B[b], devB[b], sizeof(double) * K * N);
    for (int i = 0; i < M * N; i++) {
      C[b][i] = 4;
      C_cblas[b][i] = C[b][i];
    }
    accl_view.copy(C[b], devC[b], sizeof(double) * M * N);
  }

  // NoTransA and NoTransB */
  typeA = NoTrans;
  typeB = NoTrans;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Copyinng device pointers stored in host memory to device memory
  accl_view.copy(devA, d_Aarray, batchSize * sizeof(double *));
  accl_view.copy(devB, d_Barray, batchSize * sizeof(double *));
  accl_view.copy(devC, d_Carray, batchSize * sizeof(double *));

  // Column major
  lda = M;
  ldb = K;
  ldc = M;
  status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
                           d_Aarray, lda, A_batchOffset, d_Barray, ldb,
                           B_batchOffset, beta, d_Carray, ldc, C_batchOffset,
                           aOffset, bOffset, cOffset, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  // Get the results
  for (int b = 0; b < batchSize; b++) {
    accl_view.copy(devC[b], C_hcblas[b], M * N * sizeof(double));
  }

  for (int b = 0; b < batchSize; b++)
    cblas_dgemm(CblasColMajor, Transa, Transb, M, N, K, alpha, A[b], lda, B[b],
                ldb, beta, C_cblas[b], ldc);

  for (int b = 0; b < batchSize; b++) {
    for (int i = 0; i < M * N; i++) {
      EXPECT_EQ(C_hcblas[b][i], C_cblas[b][i]);
    }
  }
  // Free up resources
  for (int b = 0; b < batchSize; b++) {
    hc::am_free(devA[b]);
    hc::am_free(devB[b]);
    hc::am_free(devC[b]);
    free(A[b]);
    free(B[b]);
    free(C[b]);
    free(C_cblas[b]);
    free(C_hcblas[b]);
  }
  hc::am_free(d_Aarray);
  hc::am_free(d_Barray);
  hc::am_free(d_Carray);

  /*  // alpha = 0
    lda = M;
    ldb = K ;
    ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, 0,
    devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta,
    devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));

    for(int i = 0; i < batchSize; i++) {
      cblas_dgemm( CblasColMajor, Transa, Transb, M, N, K, 0, Abatch, lda,
    Bbatch, ldb, beta, CCblasbatch  + i * M * N , ldc );
    }

    for(int i = 0 ; i < M * N * batchSize; i++) {
      EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    }

    // alpha = 0, beta = 0
    lda = M;
    ldb = K ;
    ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, 0,
    devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, 0, devCbatch,
    ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));

    for(int i = 0; i < batchSize; i++) {
      cblas_dgemm( CblasColMajor, Transa, Transb, M, N, K, 0, Abatch, lda,
    Bbatch, ldb, 0, CCblasbatch  + i * M * N , ldc );
    }

    for(int i = 0 ; i < M * N * batchSize; i++) {
      EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    }

    // Row Major
    lda = K;
    ldb = N ;
    ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
    devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta,
    devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));

    for(int i = 0; i < batchSize; i++) {
      cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
    Bbatch, ldb, beta, CCblasbatch  + i * M * N , ldc );
    }

    for(int i = 0 ; i < M * N * batchSize; i++) {
      EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    }

    // alpha = 0
    lda = K;
    ldb = N ;
    ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, 0,
    devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta,
    devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch,  M * N * batchSize *
    sizeof(double));

    for(int i = 0; i < batchSize; i++) {
      cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, 0, Abatch, lda,
    Bbatch, ldb, beta, CCblasbatch  + i * M * N , ldc );
    }

    for(int i = 0 ; i < M * N * batchSize; i++) {
      EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    }

    // alpha = 0, beta = 0
    lda = K;
    ldb = N ;
    ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, 0,
    devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, 0, devCbatch,
    ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));

    for(int i = 0; i < batchSize; i++) {
      cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, 0, Abatch, lda,
    Bbatch, ldb, 0, CCblasbatch  + i * M * N , ldc );
    }

    for(int i = 0 ; i < M * N * batchSize; i++) {
      EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    }

    // NoTransA TransB
    typeA = NoTrans;
    typeB = Trans;
    Transa = CblasNoTrans;
    Transb = CblasTrans;
    // Column major
    lda = M;
    ldb = N ;
    ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
    devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta,
    devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));

    for(int i = 0; i < batchSize; i++) {
      cblas_dgemm( CblasColMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
    Bbatch, ldb, beta, CCblasbatch  + i * M * N , ldc );
    }

    for(int i = 0 ; i < M * N * batchSize; i++) {
      EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    }

    // Row Major
    lda = K;
    ldb = K ;
    ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
    devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta,
    devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));

    for(int i = 0; i < batchSize; i++) {
      cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
    Bbatch, ldb, beta, CCblasbatch  + i * M * N , ldc );
    }

    for(int i = 0 ; i < M * N * batchSize; i++) {
      EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    }

    // TransA NoTransB
    typeA = Trans;
    typeB = NoTrans;
    Transa = CblasTrans;
    Transb = CblasNoTrans;
    // Column major
    lda = K;
    ldb = K ;
    ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
    devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta,
    devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch,  M * N * batchSize *
    sizeof(double));

    for(int i = 0; i < batchSize; i++) {
      cblas_dgemm( CblasColMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
    Bbatch, ldb, beta, CCblasbatch  + i * M * N , ldc );
    }

    for(int i = 0 ; i < M * N * batchSize; i++) {
      EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    }

    // Row Major
    lda = M;
    ldb = N ;
    ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
    devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta,
    devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));

    for(int i = 0; i < batchSize; i++) {
      cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
    Bbatch, ldb, beta, CCblasbatch  + i * M * N , ldc );
    }

    for(int i = 0 ; i < M * N * batchSize; i++) {
      EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    }

    // TransA TransB
    typeA = Trans;
    typeB = Trans;
    Transa = CblasTrans;
    Transb = CblasTrans;
    // Column major
    lda = K;
    ldb = N ;
    ldc = M;
    status = hc.hcblas_dgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha,
    devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta,
    devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));

    for(int i = 0; i < batchSize; i++) {
      cblas_dgemm( CblasColMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
    Bbatch, ldb, beta, CCblasbatch  + i * M * N , ldc );
    }

    for(int i = 0 ; i < M * N * batchSize; i++) {
      EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    }

    // Row Major
    lda = M;
    ldb = K ;
    ldc = N;
    status = hc.hcblas_dgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha,
    devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta,
    devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    accl_view.copy(devCbatch, Chcblasbatch, M * N * batchSize * sizeof(double));

    for(int i = 0; i < batchSize; i++) {
      cblas_dgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda,
    Bbatch, ldb, beta, CCblasbatch  + i * M * N , ldc );
    }

    for(int i = 0 ; i < M * N * batchSize; i++) {
      EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    }

    free(Abatch);
    free(Bbatch);
    free(Cbatch);
    free(CCblasbatch);
    free(Chcblasbatch);
    hc::am_free(devAbatch);
    hc::am_free(devBbatch);
    hc::am_free(devCbatch);*/
}

