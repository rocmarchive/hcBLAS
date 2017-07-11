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

int main(int argc, char *argv[]) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  hcblasCreate(&handle, &av);
  if (argc < 7) {
    std::cout << "No sufficient commandline arguments specified"
              << "argc :" << argc << std::endl;
    return -1;
  }
  int k;
  int M = atoi(argv[1]);
  int N = atoi(argv[2]);
  int K = atoi(argv[3]);
  int incX = 1;
  int incY = 1;
  __int64_t lda;
  __int64_t ldb;
  __int64_t ldc;
  hcblasOrder hcOrder = ColMajor;
  int isTransA = (atoi(argv[4]));
  int isTransB = (atoi(argv[5]));
  int Imple_type = (atoi(argv[6]));
  hcComplex cAlpha, cBeta;
  float alpha[2], beta[2];
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  int batchSize = 128;
  hcblasOperation_t typeA, typeB;
  if ((isTransA == 0 || isTransA == 1) && (isTransB == 0 || isTransB == 1)) {
    if (isTransA == 0) {
      typeA = HCBLAS_OP_N;
      lda = (hcOrder) ? M : K;
    } else {
      typeA = HCBLAS_OP_T;
      lda = (hcOrder) ? K : M;
    }
    if (isTransB == 0) {
      typeB = HCBLAS_OP_N;
      ldb = (hcOrder) ? K : N;
    } else {
      typeB = HCBLAS_OP_T;
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
  Transa = (typeA == HCBLAS_OP_N) ? CblasNoTrans : CblasTrans;
  Transb = (typeB == HCBLAS_OP_N) ? CblasNoTrans : CblasTrans;
  if (M > 3000 && N > 3000) {
    batchSize = 25;
  }
  if (M > 9000 && N > 9000) {
    batchSize = 1;
  }

  /* Implementation type I - Inputs and Outputs are HCC float array containers
   */

  if (Imple_type == 1) { /* MULTIPLE GPU CALL */
    float *ablas = (float *)calloc(M * K * 2, sizeof(float));
    float *bblas = (float *)calloc(K * N * 2, sizeof(float));
    float *cblas = (float *)calloc(M * N * 2, sizeof(float));
    hcComplex *A = (hcComplex *)calloc(M * K, sizeof(hcComplex));
    hcComplex *B = (hcComplex *)calloc(K * N, sizeof(hcComplex));
    hcComplex *C = (hcComplex *)calloc(M * N, sizeof(hcComplex));
    hcComplex *devA = hc::am_alloc(sizeof(hcComplex) * M * K, default_acc, 0);
    hcComplex *devB = hc::am_alloc(sizeof(hcComplex) * K * N, default_acc, 0);
    hcComplex *devC = hc::am_alloc(sizeof(hcComplex) * M * N, default_acc, 0);

    k = 0;
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
#ifdef PROFILE
    for (int iter = 0; iter < 10; iter++) {
#endif
      k = 0;
      for (int i = 0; i < M * N; i++) {
        C[i].x = rand_r(&global_seed) % 18;
        C[i].y = rand_r(&global_seed) % 28;
        cblas[k++] = C[i].x;
        cblas[k++] = C[i].y;
      }

      hcblasSetMatrix(handle, M, K, sizeof(hcComplex), A, 1, devA, 1);
      hcblasSetMatrix(handle, K, N, sizeof(hcComplex), B, 1, devB, 1);
      hcblasSetMatrix(handle, M, N, sizeof(hcComplex), C, 1, devC, 1);
      status = hcblasCgemm(handle, typeA, typeB, M, N, K, &cAlpha, devA, lda,
                           devB, ldb, &cBeta, devC, ldc);
      hcblasGetMatrix(handle, M, N, sizeof(hcComplex), devC, 1, C, 1);
      cblas_cgemm(order, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas,
                  ldb, &beta, cblas, ldc);
      for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
        if (cblas[k] != (C[i].x) || cblas[k + 1] != (C[i].y)) {
          ispassed = 0;
          std::cout << " HCSGEMM[" << i << "].x = " << C[i].x
                    << " doesnot match with CBLASSGEMM[" << i
                    << "] =" << cblas[k];
          std::cout << " HCSGEMM[" << i << "].y = " << C[i].y
                    << " doesnot match with CBLASSGEMM[" << i
                    << "] =" << cblas[k + 1] << std::endl;
          break;
        } else {
          continue;
        }
      }
      if (!ispassed) std::cout << "TEST FAILED" << std::endl;
      if (status) std::cout << "TEST FAILED" << std::endl;
#ifdef PROFILE
    }
#endif
    free(A);
    free(B);
    free(C);
    free(ablas);
    free(bblas);
    free(cblas);
    hc::am_free(devA);
    hc::am_free(devB);
    hc::am_free(devC);
  }

  /* Implementation type II - Inputs and Outputs are HCC float array containers
     with batch processing */

  else {
    hcComplex *devA[batchSize], *devB[batchSize], *devC[batchSize];

    hcComplex *A[batchSize];
    hcComplex *B[batchSize];
    hcComplex *C[batchSize];
    hcComplex **d_Aarray =
        hc::am_alloc(sizeof(hcComplex) * batchSize, default_acc, 0);
    hcComplex **d_Barray =
        hc::am_alloc(sizeof(hcComplex) * batchSize, default_acc, 0);
    hcComplex **d_Carray =
        hc::am_alloc(sizeof(hcComplex) * batchSize, default_acc, 0);
    float *ablas[batchSize];
    float *bblas[batchSize];
    float *cblas[batchSize];

    for (int b = 0; b < batchSize; b++) {
      A[b] = (hcComplex *)calloc(M * K, sizeof(hcComplex));
      B[b] = (hcComplex *)calloc(K * N, sizeof(hcComplex));
      C[b] = (hcComplex *)calloc(M * N, sizeof(hcComplex));
      devA[b] = hc::am_alloc(sizeof(hcComplex) * M * K, default_acc, 0);
      devB[b] = hc::am_alloc(sizeof(hcComplex) * K * N, default_acc, 0);
      devC[b] = hc::am_alloc(sizeof(hcComplex) * M * N, default_acc, 0);
      ablas[b] = (float *)malloc(sizeof(float) * M * K * 2);
      bblas[b] = (float *)malloc(sizeof(float) * K * N * 2);
      cblas[b] = (float *)malloc(sizeof(float) * M * N * 2);
    }
    for (int b = 0; b < batchSize; b++) {
      k = 0;
      for (int i = 0; i < M * K; i++) {
        A[b][i].x = rand_r(&global_seed) % 10;
        A[b][i].y = rand_r(&global_seed) % 20;
        ablas[b][k++] = A[b][i].x;
        ablas[b][k++] = A[b][i].y;
      }
      k = 0;
      for (int i = 0; i < K * N; i++) {
        B[b][i].x = rand_r(&global_seed) % 15;
        B[b][i].y = rand_r(&global_seed) % 25;
        bblas[b][k++] = B[b][i].x;
        bblas[b][k++] = B[b][i].y;
      }
    }
#ifdef PROFILE
    for (int iter = 0; iter < 10; iter++) {
#endif
      for (int b = 0; b < batchSize; b++) {
        k = 0;
        for (int i = 0; i < M * N; i++) {
          C[b][i].x = rand_r(&global_seed) % 18;
          C[b][i].y = rand_r(&global_seed) % 28;
          cblas[b][k++] = C[b][i].x;
          cblas[b][k++] = C[b][i].y;
        }
      }
      for (int b = 0; b < batchSize; b++) {
        hcblasSetMatrix(handle, M, K, sizeof(hcComplex), A[b], 1, devA[b], 1);
        hcblasSetMatrix(handle, K, N, sizeof(hcComplex), B[b], 1, devB[b], 1);
        hcblasSetMatrix(handle, M, N, sizeof(hcComplex), C[b], 1, devC[b], 1);
      }

      av.copy(devA, d_Aarray, batchSize * sizeof(hcComplex *));
      av.copy(devB, d_Barray, batchSize * sizeof(hcComplex *));
      av.copy(devC, d_Carray, batchSize * sizeof(hcComplex *));

      status = hcblasCgemmBatched(handle, typeA, typeB, M, N, K, &cAlpha,
                                  d_Aarray, lda, d_Barray, ldb, &cBeta,
                                  d_Carray, ldc, batchSize);
      for (int b = 0; b < batchSize; b++) {
        hcblasGetMatrix(handle, M, N, sizeof(hcComplex), devC[b], 1, C[b], 1);
      }

      for (int i = 0; i < batchSize; i++)
        cblas_cgemm(order, Transa, Transb, M, N, K, alpha, ablas[i], lda,
                    bblas[i], ldb, beta, cblas[i], ldc);

      for (int b = 0; b < batchSize; b++) {
        for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2));
             i++, k = k + 2) {
          if (C[b][i].x != cblas[b][k] || C[b][i].y != cblas[b][k + 1]) {
            ispassed = 0;
            std::cout << " HCCGEMM[" << b << "][" << i << "].x = " << C[b][i].x
                      << " doesnot match with CBLASCGEMM[" << b << "][" << k
                      << "] =" << cblas[b][k];
            std::cout << " HCCGEMM[" << b << "][" << i << "].y = " << C[b][i].y
                      << " doesnot match with CBLASCGEMM[" << b << "]["
                      << (k + 1) << "] =" << cblas[b][k + 1] << std::endl;
            break;
          } else {
            continue;
          }
        }
      }
      if (!ispassed) std::cout << "TEST FAILED" << std::endl;
      if (status) std::cout << "TEST FAILED" << std::endl;
#ifdef PROFILE
    }
#endif

    for (int b = 0; b < batchSize; b++) {
      hc::am_free(devA[b]);
      hc::am_free(devB[b]);
      hc::am_free(devC[b]);
      free(A[b]);
      free(B[b]);
      free(C[b]);
      free(ablas[b]);
      free(bblas[b]);
      free(cblas[b]);
    }
    hc::am_free(d_Aarray);
    hc::am_free(d_Barray);
    hc::am_free(d_Carray);
  }

  hcblasDestroy(&handle);

  return 0;
}
