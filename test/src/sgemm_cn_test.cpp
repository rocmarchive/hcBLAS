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
#include <cblas.h>
#include <cstdlib>
#include <hc_am.hpp>
#include <iostream>

unsigned int global_seed = 100;

int main(int argc, char* argv[]) {
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
  __int64_t lda = (atoi(argv[6]));
  __int64_t ldb = (atoi(argv[7]));
  __int64_t ldc = (atoi(argv[8]));
  float alpha = (atoi(argv[9]));
  float beta = (atoi(argv[10]));
  int incX = 1;
  int incY = 1;
  __int64_t aOffset = (atoi(argv[11]));
  __int64_t bOffset = (atoi(argv[12]));
  __int64_t cOffset = (atoi(argv[13]));
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
    } else {
      typeA = Trans;
    }
    if (isTransB == 0) {
      typeB = NoTrans;
    } else {
      typeB = Trans;
    }
  } else {
    std::cout << "Invalid Transpose type specified" << std::endl;
    return -1;
  }

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

  float* C_cblas = (float*)calloc(M * N + cOffset, sizeof(float));
  float* A = (float*)calloc(M * K + aOffset, sizeof(float));
  float* B = (float*)calloc(K * N + bOffset, sizeof(float));
  float* C = (float*)calloc(M * N + cOffset, sizeof(float));
  float* devA = hc::am_alloc(sizeof(float) * (M * K + aOffset), acc[1], 0);
  float* devB = hc::am_alloc(sizeof(float) * (K * N + bOffset), acc[1], 0);
  float* devC = hc::am_alloc(sizeof(float) * (M * N + cOffset), acc[1], 0);
  for (int i = 0; i < M * K; i++) {
    A[i + aOffset] = rand_r(&global_seed) % 100;
  }
  for (int i = 0; i < K * N; i++) {
    B[i + bOffset] = rand_r(&global_seed) % 15;
  }
#ifdef PROFILE
  for (int iter = 0; iter < 10; iter++) {
#endif
    for (int i = 0; i < M * N; i++) {
      C[i + cOffset] = rand_r(&global_seed) % 25;
      C_cblas[i + cOffset] = C[i + cOffset];
    }
    accl_view.copy(A, devA, (M * K + aOffset) * sizeof(float));
    accl_view.copy(B, devB, (K * N + bOffset) * sizeof(float));
    accl_view.copy(C, devC, (M * N + cOffset) * sizeof(float));
    status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, M, N, K, alpha,
                             devA, lda, devB, ldb, beta, devC, ldc, aOffset,
                             bOffset, cOffset);
    accl_view.copy(devC, C, (M * N + cOffset) * sizeof(float));
    cblas_sgemm(order, Transa, Transb, M, N, K, alpha, A + aOffset, lda,
                B + bOffset, ldb, beta, C_cblas + cOffset, ldc);
    for (int i = 0; i < M * N; i++) {
      if (C_cblas[i + cOffset] != (C[i + cOffset])) {
        ispassed = 0;
        std::cout << " HCSGEMM[" << i << "] = " << C[i + cOffset]
                  << " doesnot match with CBLASSGEMM[" << i
                  << "] =" << C_cblas[i + cOffset] << std::endl;
        break;
      } else {
        continue;
      }
    }
    if (!ispassed) std::cout << "TEST FAILED" << std::endl;
    if (status) std::cout << "TEST FAILED" << status << std::endl;
#ifdef PROFILE
  }
#endif
  free(A);
  free(B);
  free(C);
  hc::am_free(devA);
  hc::am_free(devB);
  hc::am_free(devC);
  return 0;
}

