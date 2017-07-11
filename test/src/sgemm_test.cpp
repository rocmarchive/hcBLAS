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

#include "cblas.h"
#include "include/hcblas.h"
#include "include/hcblaslib.h"
#include <cstdlib>
#include <hc_am.hpp>
#include <iostream>

unsigned int global_seed = 100;

int main(int argc, char *argv[]) {
  /* HCBLAS Implementation */
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  if (argc < 7) {
    std::cout << "No sufficient commandline arguments specified"
              << "argc :" << argc << std::endl;
    return -1;
  }
  int M = atoi(argv[1]);
  int N = atoi(argv[2]);
  int K = atoi(argv[3]);
  int isTransA = (atoi(argv[4]));
  int isTransB = (atoi(argv[5]));
  int Imple_type = (atoi(argv[6]));
  float alpha = 1;
  float beta = 1;
  __int64_t lda;
  __int64_t ldb;
  __int64_t ldc;
  int incX = 1;
  int incY = 1;
  __int64_t aOffset = 0;
  __int64_t bOffset = 0;
  __int64_t cOffset = 0;
  __int64_t A_batchOffset = 0;
  __int64_t B_batchOffset = 0;
  __int64_t C_batchOffset = M * N;
  int batchSize = 128;
  hcblasOrder hcOrder = ColMajor;
  hcblasTranspose typeA, typeB;
  hcblasStatus status;
  if ((isTransA == 0 || isTransA == 1) && (isTransB == 0 || isTransB == 1)) {
    if (isTransA == 0) {
      typeA = NoTrans;
      lda = (hcOrder) ? M : K;
    } else {
      typeA = Trans;
      lda = (hcOrder) ? K : M;
    }
    if (isTransB == 0) {
      typeB = NoTrans;
      ldb = (hcOrder) ? K : N;
    } else {
      typeB = Trans;
      ldb = (hcOrder) ? N : K;
    }
  } else {
    std::cout << "Invalid Transpose type specified" << std::endl;
    return -1;
  }
  ldc = (hcOrder) ? M : N;

  /* CBLAS implementation */
  bool ispassed = 1;
  CBLAS_ORDER order;
  CBLAS_TRANSPOSE Transa, Transb;
  order = CblasColMajor;
  Transa = (typeA == NoTrans) ? CblasNoTrans : CblasTrans;
  Transb = (typeB == NoTrans) ? CblasNoTrans : CblasTrans;
  if (M > 3000 && N > 3000) {
    batchSize = 25;
  }
  if (M > 9000 && N > 9000) {
    batchSize = 1;
  }
  std::vector<hc::accelerator> acc = hc::accelerator::get_all();
  hc::accelerator_view accl_view = (acc[1].get_default_view());

  /* Implementation type I - Inputs and Outputs are HCC float array containers
   */

  if (Imple_type == 1) { /* MULTIPLE GPU CALL */
    float *C_cblas = (float *)calloc(M * N, sizeof(float));
    float *A = (float *)calloc(M * K, sizeof(float));
    float *B = (float *)calloc(K * N, sizeof(float));
    float *C = (float *)calloc(M * N, sizeof(float));
    float *devA = hc::am_alloc(sizeof(float) * M * K, acc[1], 0);
    float *devB = hc::am_alloc(sizeof(float) * K * N, acc[1], 0);
    float *devC = hc::am_alloc(sizeof(float) * M * N, acc[1], 0);
    for (int i = 0; i < M * K; i++) {
      A[i] = rand() % 100;
    }
    for (int i = 0; i < K * N; i++) {
      B[i] = rand() % 15;
    }
#ifdef PROFILE
    for (int iter = 0; iter < 10; iter++) {
#endif
      for (int i = 0; i < M * N; i++) {
        C[i] = rand() % 25;
        C_cblas[i] = C[i];
      }
      accl_view.copy(A, devA, M * K * sizeof(float));
      accl_view.copy(B, devB, K * N * sizeof(float));
      accl_view.copy(C, devC, M * N * sizeof(float));
      status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, M, N, K, alpha,
                               devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                               bOffset, cOffset);
      accl_view.copy(devC, C, M * N * sizeof(float));
      cblas_sgemm(order, Transa, Transb, M, N, K, alpha, A, lda, B, ldb, beta,
                  C_cblas, ldc);
      for (int i = 0; i < M * N; i++) {
        if (C_cblas[i] != (C[i])) {
          ispassed = 0;
          std::cout << " HCSGEMM[" << i << "] = " << C[i]
                    << " doesnot match with CBLASSGEMM[" << i
                    << "] =" << C_cblas[i] << std::endl;
          break;
        } else
          continue;
      }
      if (!ispassed) std::cout << "TEST FAILED" << std::endl;
      if (status) std::cout << "TEST FAILED" << std::endl;
#ifdef PROFILE
    }
#endif
    free(A);
    free(B);
    free(C);
    hc::am_free(devA);
    hc::am_free(devB);
    hc::am_free(devC);
  }

  /* Implementation type II - Inputs and Outputs are HCC float array containers
     with batch processing */

  else {
    float *Abatch = (float *)calloc(M * K, sizeof(float));
    float *Bbatch = (float *)calloc(K * N, sizeof(float));
    float *Cbatch = (float *)calloc(M * N * batchSize, sizeof(float));
    float *CCblasbatch = (float *)calloc(M * N * batchSize, sizeof(float));
    float *devAbatch = hc::am_alloc(sizeof(float) * M * K, acc[1], 0);
    float *devBbatch = hc::am_alloc(sizeof(float) * K * N, acc[1], 0);
    float *devCbatch =
        hc::am_alloc(sizeof(float) * M * N * batchSize, acc[1], 0);

    for (int i = 0; i < M * K; i++) {
      Abatch[i] = rand() % 100;
    }
    for (int i = 0; i < K * N; i++) {
      Bbatch[i] = rand() % 15;
    }
#ifdef PROFILE
    for (int iter = 0; iter < 10; iter++) {
#endif
      for (int i = 0; i < M * N * batchSize; i++) {
        Cbatch[i] = rand() % 25;
        CCblasbatch[i] = Cbatch[i];
      }
      accl_view.copy(Abatch, devAbatch, M * K * sizeof(float));
      accl_view.copy(Bbatch, devBbatch, K * N * sizeof(float));
      accl_view.copy(Cbatch, devCbatch, M * N * batchSize * sizeof(float));
      // status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, M, N, K,
      // alpha, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset,
      // beta, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset,
      // batchSize);
      accl_view.copy(devCbatch, Cbatch, M * N * batchSize * sizeof(float));
      for (int i = 0; i < batchSize; i++)
        cblas_sgemm(order, Transa, Transb, M, N, K, alpha, Abatch, lda, Bbatch,
                    ldb, beta, CCblasbatch + i * M * N, ldc);

      for (int i = 0; i < M * N * batchSize; i++) {
        if (Cbatch[i] != (CCblasbatch[i])) {
          ispassed = 0;
          std::cout << " HCSGEMM[" << i << "] = " << Cbatch[i]
                    << " doesnot match with CBLASSGEMM[" << i
                    << "] =" << CCblasbatch[i] << std::endl;
          break;
        } else
          continue;
      }
      if (!ispassed) std::cout << "TEST FAILED" << std::endl;
      if (status) std::cout << "TEST FAILED" << std::endl;
#ifdef PROFILE
    }
#endif
    free(Abatch);
    free(Bbatch);
    free(Cbatch);
    hc::am_free(devAbatch);
    hc::am_free(devBbatch);
    hc::am_free(devCbatch);
  }
  return 0;
}

