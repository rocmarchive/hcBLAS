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
#include <hc_short_vector.hpp>
#include <iostream>
#include <unistd.h>

unsigned int global_seed = 100;

int main(int argc, char **argv) {
  /*  HCBLAS Implementation */
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  if (argc < 5) {
    std::cout << "No sufficient commandline arguments specified"
              << "argc :" << argc << std::endl;
    return -1;
  }
  int M = atoi(argv[1]);
  int N = atoi(argv[2]);
  int isTransA = (atoi(argv[3]));
  int Imple_type = (atoi(argv[4]));
  int row, col;
  bool ispassed = 1;
  float alpha = 1;
  float beta = 1;
  __int64_t lda;
  int incX = 1;
  int incY = 1;
  __int64_t xOffset = 0;
  __int64_t yOffset = 0;
  __int64_t aOffset = 0;
  int batchSize = 128;
  __int64_t lenx, leny;
  hcblasStatus status;
  hcblasTranspose typeA;
  hcblasOrder hcOrder = ColMajor;
  if (isTransA == 0) {
    row = N;
    col = M;
    lda = M;
    typeA = NoTrans;
  } else {
    row = M;
    col = N;
    lda = N;
    typeA = Trans;
  }
  /* CBLAS Implementation */
  CBLAS_ORDER order;
  CBLAS_TRANSPOSE transa;
  order = CblasColMajor;
  transa = (typeA == NoTrans) ? CblasNoTrans : CblasTrans;
  lenx = 1 + (row - 1) * abs(incX);
  leny = 1 + (col - 1) * abs(incY);
  __int64_t X_batchOffset = row;
  __int64_t Y_batchOffset = col;
  __int64_t A_batchOffset = row * col;
  std::vector<hc::accelerator> acc = hc::accelerator::get_all();
  hc::accelerator_view accl_view = (acc[1].get_default_view());

  /* Implementation type I - Inputs and Outputs are HCC float array containers
   */

  if (Imple_type == 1) {
    float *x = (float *)calloc(lenx, sizeof(float));
    float *y = (float *)calloc(leny, sizeof(float));
    float *A = (float *)calloc(lenx * leny, sizeof(float));
    float *ycblas = (float *)calloc(leny, sizeof(float));
    float *devA = hc::am_alloc(sizeof(float) * lenx * leny, acc[1], 0);
    float *devX = hc::am_alloc(sizeof(float) * lenx, acc[1], 0);
    float *devY = hc::am_alloc(sizeof(float) * leny, acc[1], 0);
    for (int i = 0; i < lenx; i++) {
      x[i] = rand_r(&global_seed) % 10;
    }
    for (int i = 0; i < lenx * leny; i++) {
      A[i] = rand_r(&global_seed) % 25;
    }
#ifdef PROFILE
    for (int iter = 0; iter < 10; iter++) {
#endif
      for (int i = 0; i < leny; i++) {
        y[i] = rand_r(&global_seed) % 15;
        ycblas[i] = y[i];
      }
      accl_view.copy(A, devA, lenx * leny * sizeof(float));
      accl_view.copy(x, devX, lenx * sizeof(float));
      accl_view.copy(y, devY, leny * sizeof(float));
      status =
          hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, devA, aOffset,
                          lda, devX, xOffset, incX, beta, devY, yOffset, incY);
      accl_view.copy(devY, y, leny * sizeof(float));
      lda = (hcOrder) ? M : N;
      cblas_sgemv(order, transa, M, N, alpha, A, lda, x, incX, beta, ycblas,
                  incY);
      for (int i = 0; i < leny; i++) {
        if (y[i] != ycblas[i]) {
          ispassed = 0;
          std::cout << " HCSGEMV[" << i << "] " << y[i]
                    << " does not match with CBLASSGEMV[" << i << "] "
                    << ycblas[i] << std::endl;
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
    free(x);
    free(y);
    free(A);
    free(ycblas);
    hc::am_free(devA);
    hc::am_free(devX);
    hc::am_free(devY);
  }

  /* Implementation type II - Inputs and Outputs are HCC float array containers
     with batch processing */

  else {
    float *xbatch = (float *)calloc(lenx * batchSize, sizeof(float));
    float *ybatch = (float *)calloc(leny * batchSize, sizeof(float));
    float *Abatch = (float *)calloc(lenx * leny * batchSize, sizeof(float));
    float *ycblasbatch = (float *)calloc(leny * batchSize, sizeof(float));
    float *devXbatch =
        hc::am_alloc(sizeof(float) * lenx * batchSize, acc[1], 0);
    float *devYbatch =
        hc::am_alloc(sizeof(float) * leny * batchSize, acc[1], 0);
    float *devAbatch =
        hc::am_alloc(sizeof(float) * lenx * leny * batchSize, acc[1], 0);
    for (int i = 0; i < lenx * batchSize; i++) {
      xbatch[i] = rand_r(&global_seed) % 10;
    }
    for (int i = 0; i < lenx * leny * batchSize; i++) {
      Abatch[i] = rand_r(&global_seed) % 25;
    }
#ifdef PROFILE
    for (int iter = 0; iter < 10; iter++) {
#endif
      for (int i = 0; i < leny * batchSize; i++) {
        ybatch[i] = rand_r(&global_seed) % 15;
        ycblasbatch[i] = ybatch[i];
      }
      accl_view.copy(xbatch, devXbatch, lenx * batchSize * sizeof(float));
      accl_view.copy(ybatch, devYbatch, leny * batchSize * sizeof(float));
      accl_view.copy(Abatch, devAbatch,
                     lenx * leny * batchSize * sizeof(float));
      status = hc.hcblas_sgemv(
          accl_view, hcOrder, typeA, M, N, alpha, devAbatch, aOffset,
          A_batchOffset, lda, devXbatch, xOffset, X_batchOffset, incX, beta,
          devYbatch, yOffset, Y_batchOffset, incY, batchSize);
      accl_view.copy(devYbatch, ybatch, leny * batchSize * sizeof(float));
      lda = (hcOrder) ? M : N;
      for (int i = 0; i < batchSize; i++)
        cblas_sgemv(order, transa, M, N, alpha, Abatch + i * M * N, lda,
                    xbatch + i * row, incX, beta, ycblasbatch + i * col, incY);
      for (int i = 0; i < leny * batchSize; i++) {
        if (ybatch[i] != ycblasbatch[i]) {
          ispassed = 0;
          std::cout << " HCSGEMV[" << i << "] " << ybatch[i]
                    << " does not match with CBLASSGEMV[" << i << "] "
                    << ycblasbatch[i] << std::endl;
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
    free(xbatch);
    free(ybatch);
    free(Abatch);
    free(ycblasbatch);
    hc::am_free(devAbatch);
    hc::am_free(devXbatch);
    hc::am_free(devYbatch);
  }
  return 0;
}
