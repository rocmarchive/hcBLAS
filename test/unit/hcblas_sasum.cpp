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

TEST(hcblas_sasum, return_correct_sasum_Implementation_type_1) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int N = 119;
  int incX = 1;
  __int64_t xOffset = 0;
  float asumhcblas;
  hcblasStatus status;
  __int64_t lenx = 1 + (N - 1) * abs(incX);
  float *X = (float *)calloc(lenx, sizeof(float));
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  /* Implementation type I - Inputs and Outputs are HCC device pointers */
  float *devX = hc::am_alloc(sizeof(float) * lenx, acc, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  accl_view.copy(X, devX, lenx * sizeof(float));
  /* Proper call */
  status = hc.hcblas_sasum(accl_view, N, devX, incX, xOffset, &asumhcblas);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  /* X not properly allocated */
  float *devX1 = NULL;
  status = hc.hcblas_sasum(accl_view, N, devX1, incX, xOffset, &asumhcblas);
  EXPECT_EQ(status, HCBLAS_INVALID);
  /* N is 0 */
  N = 0;
  status = hc.hcblas_sasum(accl_view, N, devX, incX, xOffset, &asumhcblas);
  EXPECT_EQ(status, HCBLAS_INVALID);
  /* incX is 0 */
  incX = 0;
  status = hc.hcblas_sasum(accl_view, N, devX, incX, xOffset, &asumhcblas);
  EXPECT_EQ(status, HCBLAS_INVALID);
  free(X);
  hc::am_free(devX);
}

TEST(hcblas_sasum, func_correct_sasum_Implementation_type_1) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int N = 119;
  int incX = 1;
  __int64_t xOffset = 0;
  float asumhcblas;
  float asumcblas = 0.0;
  hcblasStatus status;
  __int64_t lenx = 1 + (N - 1) * abs(incX);
  float *X = (float *)calloc(lenx, sizeof(float));
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  /* Implementation type I - Inputs and Outputs are HCC device pointers */
  float *devX = hc::am_alloc(sizeof(float) * lenx, acc, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  accl_view.copy(X, devX, lenx * sizeof(float));
  /* Proper call */
  status = hc.hcblas_sasum(accl_view, N, devX, incX, xOffset, &asumhcblas);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  asumcblas = cblas_sasum(N, X, incX);
  EXPECT_EQ(asumhcblas, asumcblas);
  // Mandatory wait after kernel invocations when no copy to host happens
  accl_view.wait();
  free(X);
  hc::am_free(devX);
}

TEST(hcblas_sasum, return_correct_sasum_Implementation_type_2) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int N = 119;
  int incX = 1;
  int batchSize = 128;
  __int64_t xOffset = 0;
  float asumhcblas;
  hcblasStatus status;
  __int64_t X_batchOffset = N;
  __int64_t lenx = 1 + (N - 1) * abs(incX);
  float *Xbatch = (float *)calloc(lenx * batchSize, sizeof(float));
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  float *devXbatch = hc::am_alloc(sizeof(float) * lenx * batchSize, acc, 0);
  /* Implementation type II - Inputs and Outputs are HCC device pointers with
   * batch processing */
  for (int i = 0; i < lenx * batchSize; i++) {
    Xbatch[i] = rand_r(&global_seed) % 10;
  }
  accl_view.copy(Xbatch, devXbatch, lenx * batchSize * sizeof(float));
  /* Proper call */
  status = hc.hcblas_sasum(accl_view, N, devXbatch, incX, xOffset, &asumhcblas,
                           X_batchOffset, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  /* X is not properly allocated */
  float *devX1 = NULL;
  status = hc.hcblas_sasum(accl_view, N, devX1, incX, xOffset, &asumhcblas,
                           X_batchOffset, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  /* N is 0 */
  N = 0;
  status = hc.hcblas_sasum(accl_view, N, devXbatch, incX, xOffset, &asumhcblas,
                           X_batchOffset, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  /* incX is 0 */
  incX = 0;
  status = hc.hcblas_sasum(accl_view, N, devXbatch, incX, xOffset, &asumhcblas,
                           X_batchOffset, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // Mandatory wait after kernel invocations when no copy to host happens
  accl_view.wait();
  free(Xbatch);
  hc::am_free(devXbatch);
}

TEST(hcblas_sasum, func_correct_sasum_Implementation_type_2) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int N = 119;
  int incX = 1;
  int batchSize = 128;
  __int64_t xOffset = 0;
  float asumhcblas;
  float asumcblas = 0.0;
  float *asumcblastemp = (float *)calloc(batchSize, sizeof(float));
  hcblasStatus status;
  __int64_t X_batchOffset = N;
  __int64_t lenx = 1 + (N - 1) * abs(incX);
  float *Xbatch = (float *)calloc(lenx * batchSize, sizeof(float));
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  float *devXbatch = hc::am_alloc(sizeof(float) * lenx * batchSize, acc, 0);
  /* Implementation type II - Inputs and Outputs are HCC float array containers
   * with batch processing */
  for (int i = 0; i < lenx * batchSize; i++) {
    Xbatch[i] = rand_r(&global_seed) % 10;
  }
  accl_view.copy(Xbatch, devXbatch, lenx * batchSize * sizeof(float));
  /* Proper call */
  status = hc.hcblas_sasum(accl_view, N, devXbatch, incX, xOffset, &asumhcblas,
                           X_batchOffset, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  for (int i = 0; i < batchSize; i++) {
    asumcblastemp[i] = cblas_sasum(N, Xbatch + i * N, incX);
    asumcblas += asumcblastemp[i];
  }
  EXPECT_EQ(asumhcblas, asumcblas);
  free(Xbatch);
  hc::am_free(devXbatch);
}

