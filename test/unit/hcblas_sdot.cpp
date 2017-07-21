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
#include "./test_constants.h"
#include "include/hcblaslib.h"
#include "gtest/gtest.h"
#include <cblas.h>
#include <cstdlib>
#include <hc_am.hpp>

unsigned int global_seed = 100;

// code to check input given n size N
void func_check_sdot_with_input(__int64_t N) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int incX = 1;
  int incY = 1;
  __int64_t yOffset = 0;
  __int64_t xOffset = 0;
  float dothcblas;
  hcblasStatus status;
  float dotcblas = 0.0;
  __int64_t lenx = 1 + (N - 1) * abs(incX);
  __int64_t leny = 1 + (N - 1) * abs(incY);
  float* X = (float*)calloc(lenx, sizeof(float));
  float* Y = (float*)calloc(leny, sizeof(float));
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  /*Implementation type I - Inputs and Outputs are HCC float array containers */
  float* devX = hc::am_alloc(sizeof(float) * lenx, acc, 0);
  float* devY = hc::am_alloc(sizeof(float) * leny, acc, 0);

  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }

  for (int i = 0; i < leny; i++) {
    Y[i] = rand_r(&global_seed) % 15;
  }

  accl_view.copy(X, devX, lenx * sizeof(float));
  accl_view.copy(Y, devY, leny * sizeof(float));
  /* Proper call */
  status = hc.hcblas_sdot(accl_view, N, devX, incX, xOffset, devY, incY,
                          yOffset, dothcblas);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  dotcblas = cblas_sdot(N, X, incX, Y, incY);
  EXPECT_EQ(dothcblas, dotcblas);
  free(X);
  free(Y);
  hc::am_free(devX);
  hc::am_free(devY);
}

TEST(hcblas_sdot, return_correct_sdot_Implementation_type_1) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  __int64_t N = 189;
  int incX = 1;
  int incY = 1;
  __int64_t yOffset = 0;
  __int64_t xOffset = 0;
  float dothcblas;
  hcblasStatus status;
  __int64_t lenx = 1 + (N - 1) * abs(incX);
  __int64_t leny = 1 + (N - 1) * abs(incY);
  float* X = (float*)calloc(lenx, sizeof(float));
  float* Y = (float*)calloc(leny, sizeof(float));
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  /* Implementation type I - Inputs and Outputs are HCC float array containers
   */
  float* devX = hc::am_alloc(sizeof(float) * lenx, acc, 0);
  float* devY = hc::am_alloc(sizeof(float) * leny, acc, 0);

  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }

  for (int i = 0; i < leny; i++) {
    Y[i] = rand_r(&global_seed) % 15;
  }

  accl_view.copy(X, devX, lenx * sizeof(float));
  accl_view.copy(Y, devY, leny * sizeof(float));
  /* Proper call */
  status = hc.hcblas_sdot(accl_view, N, devX, incX, xOffset, devY, incY,
                          yOffset, dothcblas);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  /* X and Y are not properly allocated */
  float* devX1 = NULL;
  float* devY1 = NULL;
  status = hc.hcblas_sdot(accl_view, N, devX1, incX, xOffset, devY, incY,
                          yOffset, dothcblas);
  EXPECT_EQ(status, HCBLAS_INVALID);
  status = hc.hcblas_sdot(accl_view, N, devX, incX, xOffset, devY1, incY,
                          yOffset, dothcblas);
  EXPECT_EQ(status, HCBLAS_INVALID);
  /* N is 0 */
  N = 0;
  status = hc.hcblas_sdot(accl_view, N, devX, incX, xOffset, devY, incY,
                          yOffset, dothcblas);
  EXPECT_EQ(status, HCBLAS_INVALID);
  /* incX is 0 */
  incX = 0;
  status = hc.hcblas_sdot(accl_view, N, devX, incX, xOffset, devY, incY,
                          yOffset, dothcblas);
  EXPECT_EQ(status, HCBLAS_INVALID);
  /* incY is 0 */
  incX = 1;
  incY = 0;
  status = hc.hcblas_sdot(accl_view, N, devX, incX, xOffset, devY, incY,
                          yOffset, dothcblas);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // Mandatory wait after kernel invocations when no copy to host happens
  accl_view.wait();
  free(X);
  free(Y);
  hc::am_free(devX);
  hc::am_free(devY);
}

TEST(hcblas_sdot, return_correct_sdot_Implementation_type_2) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  __int64_t N = 189;
  int incX = 1;
  int incY = 1;
  __int64_t yOffset = 0;
  int batchSize = 128;
  __int64_t xOffset = 0;
  float dothcblas;
  hcblasStatus status;
  __int64_t X_batchOffset = N;
  __int64_t Y_batchOffset = N;
  __int64_t lenx = 1 + (N - 1) * abs(incX);
  __int64_t leny = 1 + (N - 1) * abs(incY);
  float* Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
  float* Ybatch = (float*)calloc(leny * batchSize, sizeof(float));
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  float* devXbatch = hc::am_alloc(sizeof(float) * lenx * batchSize, acc, 0);
  float* devYbatch = hc::am_alloc(sizeof(float) * leny * batchSize, acc, 0);

  /* Implementation type II - Inputs and Outputs are HCC float array containers
   * with batch processing */
  for (int i = 0; i < lenx * batchSize; i++) {
    Xbatch[i] = rand_r(&global_seed) % 10;
  }

  for (int i = 0; i < leny * batchSize; i++) {
    Ybatch[i] = rand_r(&global_seed) % 15;
  }

  accl_view.copy(Xbatch, devXbatch, lenx * batchSize * sizeof(float));
  accl_view.copy(Ybatch, devYbatch, leny * batchSize * sizeof(float));
  /* Proper call */
  status = hc.hcblas_sdot(accl_view, N, devXbatch, incX, xOffset, devYbatch,
                          incY, yOffset, dothcblas, X_batchOffset,
                          Y_batchOffset, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);
  /* X and Y are not properly allocated */
  float* devX1 = NULL;
  float* devY1 = NULL;
  status = hc.hcblas_sdot(accl_view, N, devX1, incX, xOffset, devYbatch, incY,
                          yOffset, dothcblas, X_batchOffset, Y_batchOffset,
                          batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  status = hc.hcblas_sdot(accl_view, N, devXbatch, incX, xOffset, devY1, incY,
                          yOffset, dothcblas, X_batchOffset, Y_batchOffset,
                          batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  /* N is 0 */
  N = 0;
  status = hc.hcblas_sdot(accl_view, N, devXbatch, incX, xOffset, devYbatch,
                          incY, yOffset, dothcblas, X_batchOffset,
                          Y_batchOffset, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  /* incX is 0 */
  incX = 0;
  status = hc.hcblas_sdot(accl_view, N, devXbatch, incX, xOffset, devYbatch,
                          incY, yOffset, dothcblas, X_batchOffset,
                          Y_batchOffset, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  /* incY is 0 */
  incX = 1;
  incY = 0;
  status = hc.hcblas_sdot(accl_view, N, devXbatch, incX, xOffset, devYbatch,
                          incY, yOffset, dothcblas, X_batchOffset,
                          Y_batchOffset, batchSize);
  EXPECT_EQ(status, HCBLAS_INVALID);
  // Mandatory wait after kernel invocations when no copy to host happens
  accl_view.wait();
  free(Xbatch);
  free(Ybatch);
  hc::am_free(devXbatch);
  hc::am_free(devYbatch);
}
// SDOT functionality check for different sizes
// vvlarge test
TEST(hcblas_sdot, func_correct_sdot_vvlargeN_Implementation_type_1) {
  __int64_t input = gen_vvlarge();
  func_check_sdot_with_input(input);
}

// vlarge test
TEST(hcblas_sdot, func_correct_sdot_vlargeN_Implementation_type_1) {
  __int64_t input = gen_vlarge();
  func_check_sdot_with_input(input);
}

// large test
TEST(hcblas_sdot, func_correct_sdot_largeN_Implementation_type_1) {
  __int64_t input = gen_large();
  func_check_sdot_with_input(input);
}

// REGULAR test
TEST(hcblas_sdot, func_correct_sdot_regularN_Implementation_type_1) {
  __int64_t input = gen_regular();
  func_check_sdot_with_input(input);
}

// SMALL test
TEST(hcblas_sdot, func_correct_sdot_smallN_Implementation_type_1) {
  __int64_t input = gen_small();
  func_check_sdot_with_input(input);
}

// VSMALL test
TEST(hcblas_sdot, func_correct_sdot_vsmallN_Implementation_type_1) {
  __int64_t input = gen_vsmall();
  func_check_sdot_with_input(input);
}

// VV_SMALL test
TEST(hcblas_sdot, func_correct_sdot_vvsmallN_Implementation_type_1) {
  __int64_t input = gen_vsmall();
  func_check_sdot_with_input(input);
}

// Func to check batch sdot gven inut size
void func_check_sdot_batch_with_input(__int64_t N) {
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  int incX = 1;
  int incY = 1;
  __int64_t yOffset = 0;
  int batchSize = 128;
  __int64_t xOffset = 0;
  float dothcblas;
  float dotcblas = 0.0;
  float* dotcblastemp = (float*)calloc(batchSize, sizeof(float));
  hcblasStatus status;
  __int64_t X_batchOffset = N;
  __int64_t Y_batchOffset = N;
  __int64_t lenx = 1 + (N - 1) * abs(incX);
  __int64_t leny = 1 + (N - 1) * abs(incY);
  float* Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
  float* Ybatch = (float*)calloc(leny * batchSize, sizeof(float));
  hc::accelerator_view accl_view = hc.currentAcclView;
  hc::accelerator acc = hc.currentAccl;
  float* devXbatch = hc::am_alloc(sizeof(float) * lenx * batchSize, acc, 0);
  float* devYbatch = hc::am_alloc(sizeof(float) * leny * batchSize, acc, 0);

  /* Implementation type II - Inputs and Outputs are HCC float array containers
   * with batch processing */
  for (int i = 0; i < lenx * batchSize; i++) {
    Xbatch[i] = rand_r(&global_seed) % 10;
  }

  for (int i = 0; i < leny * batchSize; i++) {
    Ybatch[i] = rand_r(&global_seed) % 15;
  }

  accl_view.copy(Xbatch, devXbatch, lenx * batchSize * sizeof(float));
  accl_view.copy(Ybatch, devYbatch, leny * batchSize * sizeof(float));
  /* Proper call */
  status = hc.hcblas_sdot(accl_view, N, devXbatch, incX, xOffset, devYbatch,
                          incY, yOffset, dothcblas, X_batchOffset,
                          Y_batchOffset, batchSize);
  EXPECT_EQ(status, HCBLAS_SUCCEEDS);

  for (int i = 0; i < batchSize; i++) {
    dotcblastemp[i] = cblas_sdot(N, Xbatch + i * N, incX, Ybatch + i * N, incY);
    dotcblas += dotcblastemp[i];
  }

  EXPECT_EQ(dothcblas, dotcblas);
  free(Xbatch);
  free(Ybatch);
  hc::am_free(devXbatch);
  hc::am_free(devYbatch);
}

// SDOT batch functionality check for different sizes

// SMALL test
TEST(hcblas_sdot, func_correct_sdot_batch_smallN_Implementation_type_2) {
  __int64_t input = gen_small();
  func_check_sdot_batch_with_input(input);
}

// VSMALL test
TEST(hcblas_sdot, func_correct_sdot_batch_vsmallN_Implementation_type_2) {
  __int64_t input = gen_vsmall();
  func_check_sdot_batch_with_input(input);
}

// VV_SMALL test
TEST(hcblas_sdot, func_correct_sdot_batch_vvsmallN_Implementation_type_2) {
  __int64_t input = gen_vsmall();
  func_check_sdot_batch_with_input(input);
}

