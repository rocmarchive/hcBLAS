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
#include <hc_am.hpp>

unsigned int global_seed = 100;

#ifdef HGEMM_UNIT_TESTING

void cblas_hgemm(int, int, int, half *, half *, half *, half, half);

void cblas_hgemm(int M, int N, int K, half *A, half *B, half *C_cblas,
                 half alpha, half beta) {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      half temp = 0;
      for (int k = 0; k < K; k++) {
        temp += alpha * A[k * M + i] * B[j * K + k];
      }
      C_cblas[j * M + i] = temp + beta * C_cblas[j * M + i];
    }
  }
}

#endif

TEST(hcblaswrapper_sasum, func_return_correct_sasum) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 23;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  float result;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float *)calloc(lenx, sizeof(float));  // host input
  float *devX = hc::am_alloc(sizeof(float) * lenx, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }

  status = hcblasSetVector(handle, lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  handle->currentAcclView.copy(X, devX, lenx * sizeof(float));
  status = hcblasSasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  float asumcblas = 0.0;
  asumcblas = cblas_sasum(n, X, incx);
  EXPECT_EQ(result, asumcblas);

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);
  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_sasumBatched, func_return_correct_sasumBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 23;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  float result;
  int batchSize = 128;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float *)calloc(lenx * batchSize, sizeof(float));  // host input
  float *devX =
      hc::am_alloc(sizeof(float) * lenx * batchSize, handle->currentAccl, 0);
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(float), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  float asumcblas = 0.0;
  float *asumcblastemp = (float *)calloc(batchSize, sizeof(float));
  for (int i = 0; i < batchSize; i++) {
    asumcblastemp[i] = cblas_sasum(n, X + i * n, incx);
    asumcblas += asumcblastemp[i];
  }
  EXPECT_EQ(result, asumcblas);

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_dasum, func_return_correct_dasum) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 23;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  double result;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double *)calloc(lenx, sizeof(double));  // host input
  double *devX = hc::am_alloc(sizeof(double) * lenx, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  status = hcblasSetVector(handle, lenx, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasDasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  double asumcblas = 0.0;
  asumcblas = cblas_dasum(n, X, incx);
  EXPECT_EQ(result, asumcblas);

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_dasumBatched, func_return_correct_dasumBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 23;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  double result;
  int batchSize = 128;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double *)calloc(lenx * batchSize, sizeof(double));  // host input
  double *devX =
      hc::am_alloc(sizeof(double) * lenx * batchSize, handle->currentAccl, 0);
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(double), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasDasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  double asumcblas = 0.0;
  double *asumcblastemp = (double *)calloc(batchSize, sizeof(double));
  for (int i = 0; i < batchSize; i++) {
    asumcblastemp[i] = cblas_dasum(n, X + i * n, incx);
    asumcblas += asumcblastemp[i];
  }
  EXPECT_EQ(result, asumcblas);

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_sscal, func_return_correct_sscal) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  float alpha = 1;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float *)calloc(lenx, sizeof(float));
  float *X = (float *)calloc(lenx, sizeof(float));  // host input
  float *devX = hc::am_alloc(sizeof(float) * lenx, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
    Xcblas[i] = X[i];
  }
  status = hcblasSetVector(handle, lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSscal(handle, n, &alpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasGetVector(handle, lenx, sizeof(float), devX, incx, X, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  cblas_sscal(n, alpha, Xcblas, incx);
  for (int i = 0; i < lenx; i++) {
    EXPECT_EQ(X[i], Xcblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSscal(handle, n, &alpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_sscalBatched, func_return_correct_sscalBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  float alpha = 1;
  int batchSize = 128;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float *)calloc(lenx * batchSize, sizeof(float));
  float *X = (float *)calloc(lenx * batchSize, sizeof(float));  // host input
  float *devX =
      hc::am_alloc(sizeof(float) * lenx * batchSize, handle->currentAccl, 0);
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
    Xcblas[i] = X[i];
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(float), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSscalBatched(handle, n, &alpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasGetVector(handle, lenx * batchSize, sizeof(float), devX, incx,
                           X, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for (int i = 0; i < batchSize; i++)
    cblas_sscal(n, alpha, Xcblas + i * n, incx);
  for (int i = 0; i < lenx * batchSize; i++) {
    EXPECT_EQ(X[i], Xcblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSscalBatched(handle, n, &alpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_dscal, func_return_correct_dscal) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  double alpha = 1;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double *)calloc(lenx, sizeof(double));
  double *X = (double *)calloc(lenx, sizeof(double));  // host input
  double *devX = hc::am_alloc(sizeof(double) * lenx, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
    Xcblas[i] = X[i];
  }
  status = hcblasSetVector(handle, lenx, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasDscal(handle, n, &alpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasGetVector(handle, lenx, sizeof(double), devX, incx, X, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  cblas_dscal(n, alpha, Xcblas, incx);
  for (int i = 0; i < lenx; i++) {
    EXPECT_EQ(X[i], Xcblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDscal(handle, n, &alpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_dscalBatched, func_return_correct_dscalBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  double alpha = 1;
  int batchSize = 128;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double *)calloc(lenx * batchSize, sizeof(double));
  double *X = (double *)calloc(lenx * batchSize, sizeof(double));  // host input
  double *devX =
      hc::am_alloc(sizeof(double) * lenx * batchSize, handle->currentAccl, 0);
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
    Xcblas[i] = X[i];
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(double), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasDscalBatched(handle, n, &alpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasGetVector(handle, lenx * batchSize, sizeof(double), devX, incx,
                           X, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for (int i = 0; i < batchSize; i++)
    cblas_dscal(n, alpha, Xcblas + i * n, incx);
  for (int i = 0; i < lenx * batchSize; i++) {
    EXPECT_EQ(X[i], Xcblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDscalBatched(handle, n, &alpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_cscal, func_return_correct_cscal) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  float alpha[2];
  hcComplex cAlpha;

  cAlpha.x = 1;
  cAlpha.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float *)calloc(lenx * 2, sizeof(float));
  hcComplex *X = (hcComplex *)calloc(lenx, sizeof(hcComplex));  // host input
  hcComplex *devX =
      hc::am_alloc(sizeof(hcComplex) * lenx, handle->currentAccl, 0);
  int k = 0;
  for (int i = 0; i < lenx; i++) {
    X[i].x = rand_r(&global_seed) % 10;
    X[i].y = rand_r(&global_seed) % 20;
    Xcblas[k++] = X[i].x;
    Xcblas[k++] = X[i].y;
  }
  status =
      hcblasSetVector(handle, lenx, sizeof(hcComplex), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasCscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status =
      hcblasGetVector(handle, lenx, sizeof(hcComplex), devX, incx, X, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  cblas_cscal(n, alpha, Xcblas, incx);
  for (int i = 0, k = 0; i < lenx && k < lenx * 2; i++, k = k + 2) {
    EXPECT_EQ(X[i].x, Xcblas[k]);
    EXPECT_EQ(X[i].y, Xcblas[k + 1]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasCscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_cscalBatched, func_return_correct_cscalBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  float alpha[2];
  int batchSize = 128;
  hcComplex cAlpha;

  cAlpha.x = 1;
  cAlpha.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float *)calloc(lenx * batchSize * 2, sizeof(float));
  hcComplex *X =
      (hcComplex *)calloc(lenx * batchSize, sizeof(hcComplex));  // host input
  hcComplex *devX = hc::am_alloc(sizeof(hcComplex) * lenx * batchSize,
                                 handle->currentAccl, 0);
  int k = 0;
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i].x = rand_r(&global_seed) % 10;
    X[i].y = rand_r(&global_seed) % 20;
    Xcblas[k++] = X[i].x;
    Xcblas[k++] = X[i].y;
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(hcComplex), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasCscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasGetVector(handle, lenx * batchSize, sizeof(hcComplex), devX,
                           incx, X, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for (int i = 0; i < batchSize; i++)
    cblas_cscal(n, alpha, Xcblas + i * n * 2, incx);
  for (int i = 0, k = 0; i < lenx * batchSize && k < lenx * batchSize * 2;
       i++, k = k + 2) {
    EXPECT_EQ(X[i].x, Xcblas[k]);
    EXPECT_EQ(X[i].y, Xcblas[k + 1]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasCscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_zscal, func_return_correct_zscal) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  double alpha[2];
  hcDoubleComplex cAlpha;

  cAlpha.x = 1;
  cAlpha.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double *)calloc(lenx * 2, sizeof(double));
  hcDoubleComplex *X =
      (hcDoubleComplex *)calloc(lenx, sizeof(hcDoubleComplex));  // host input
  hcDoubleComplex *devX =
      hc::am_alloc(sizeof(hcDoubleComplex) * lenx, handle->currentAccl, 0);
  int k = 0;
  for (int i = 0; i < lenx; i++) {
    X[i].x = rand_r(&global_seed) % 10;
    X[i].y = rand_r(&global_seed) % 20;
    Xcblas[k++] = X[i].x;
    Xcblas[k++] = X[i].y;
  }
  status = hcblasSetVector(handle, lenx, sizeof(hcDoubleComplex), X, incx, devX,
                           incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasZscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasGetVector(handle, lenx, sizeof(hcDoubleComplex), devX, incx, X,
                           incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  cblas_zscal(n, alpha, Xcblas, incx);
  for (int i = 0, k = 0; i < lenx && k < lenx * 2; i++, k = k + 2) {
    EXPECT_EQ(X[i].x, Xcblas[k]);
    EXPECT_EQ(X[i].y, Xcblas[k + 1]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasZscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_zscalBatched, func_return_correct_zscalBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  double alpha[2];
  int batchSize = 128;
  hcDoubleComplex cAlpha;

  cAlpha.x = 1;
  cAlpha.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double *)calloc(lenx * batchSize * 2, sizeof(double));
  hcDoubleComplex *X = (hcDoubleComplex *)calloc(
      lenx * batchSize, sizeof(hcDoubleComplex));  // host input
  hcDoubleComplex *devX = hc::am_alloc(
      sizeof(hcDoubleComplex) * lenx * batchSize, handle->currentAccl, 0);
  int k = 0;
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i].x = rand_r(&global_seed) % 10;
    X[i].y = rand_r(&global_seed) % 20;
    Xcblas[k++] = X[i].x;
    Xcblas[k++] = X[i].y;
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(hcDoubleComplex), X,
                           incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasZscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasGetVector(handle, lenx * batchSize, sizeof(hcDoubleComplex),
                           devX, incx, X, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for (int i = 0; i < batchSize; i++)
    cblas_zscal(n, alpha, Xcblas + i * n * 2, incx);
  for (int i = 0, k = 0; i < lenx * batchSize && k < lenx * batchSize * 2;
       i++, k = k + 2) {
    EXPECT_EQ(X[i].x, Xcblas[k]);
    EXPECT_EQ(X[i].y, Xcblas[k + 1]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasZscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_csscal, func_return_correct_csscal) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  float alpha;
  float cAlpha;

  cAlpha = 1;
  alpha = cAlpha;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float *)calloc(lenx * 2, sizeof(float));
  hcComplex *X = (hcComplex *)calloc(lenx, sizeof(hcComplex));  // host input
  hcComplex *devX =
      hc::am_alloc(sizeof(hcComplex) * lenx, handle->currentAccl, 0);
  int k = 0;
  for (int i = 0; i < lenx; i++) {
    X[i].x = rand_r(&global_seed) % 10;
    X[i].y = rand_r(&global_seed) % 20;
    Xcblas[k++] = X[i].x;
    Xcblas[k++] = X[i].y;
  }
  status =
      hcblasSetVector(handle, lenx, sizeof(hcComplex), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasCsscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status =
      hcblasGetVector(handle, lenx, sizeof(hcComplex), devX, incx, X, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  cblas_csscal(n, alpha, Xcblas, incx);
  for (int i = 0, k = 0; i < lenx && k < lenx * 2; i++, k = k + 2) {
    EXPECT_EQ(X[i].x, Xcblas[k]);
    EXPECT_EQ(X[i].y, Xcblas[k + 1]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasCsscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_csscalBatched, func_return_correct_csscalBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  float alpha;
  int batchSize = 128;
  float cAlpha;

  cAlpha = 1;
  alpha = cAlpha;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float *)calloc(lenx * batchSize * 2, sizeof(float));
  hcComplex *X =
      (hcComplex *)calloc(lenx * batchSize, sizeof(hcComplex));  // host input
  hcComplex *devX = hc::am_alloc(sizeof(hcComplex) * lenx * batchSize,
                                 handle->currentAccl, 0);
  int k = 0;
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i].x = rand_r(&global_seed) % 10;
    X[i].y = rand_r(&global_seed) % 20;
    Xcblas[k++] = X[i].x;
    Xcblas[k++] = X[i].y;
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(hcComplex), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasCsscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasGetVector(handle, lenx * batchSize, sizeof(hcComplex), devX,
                           incx, X, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for (int i = 0; i < batchSize; i++)
    cblas_csscal(n, alpha, Xcblas + i * n * 2, incx);
  for (int i = 0, k = 0; i < lenx * batchSize && k < lenx * batchSize * 2;
       i++, k = k + 2) {
    EXPECT_EQ(X[i].x, Xcblas[k]);
    EXPECT_EQ(X[i].y, Xcblas[k + 1]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasCsscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_zdscal, func_return_correct_zdscal) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  double alpha;
  double cAlpha;

  cAlpha = 1;
  alpha = cAlpha;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double *)calloc(lenx * 2, sizeof(double));
  hcDoubleComplex *X =
      (hcDoubleComplex *)calloc(lenx, sizeof(hcDoubleComplex));  // host input
  hcDoubleComplex *devX =
      hc::am_alloc(sizeof(hcDoubleComplex) * lenx, handle->currentAccl, 0);
  int k = 0;
  for (int i = 0; i < lenx; i++) {
    X[i].x = rand_r(&global_seed) % 10;
    X[i].y = rand_r(&global_seed) % 20;
    Xcblas[k++] = X[i].x;
    Xcblas[k++] = X[i].y;
  }
  status = hcblasSetVector(handle, lenx, sizeof(hcDoubleComplex), X, incx, devX,
                           incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasZdscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasGetVector(handle, lenx, sizeof(hcDoubleComplex), devX, incx, X,
                           incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  cblas_zdscal(n, alpha, Xcblas, incx);
  for (int i = 0, k = 0; i < lenx && k < lenx * 2; i++, k = k + 2) {
    EXPECT_EQ(X[i].x, Xcblas[k]);
    EXPECT_EQ(X[i].y, Xcblas[k + 1]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasZdscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_zdscalBatched, func_return_correct_zdscalBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  double alpha;
  int batchSize = 128;
  double cAlpha;

  cAlpha = 1;
  alpha = cAlpha;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double *)calloc(lenx * batchSize * 2, sizeof(double));
  hcDoubleComplex *X = (hcDoubleComplex *)calloc(
      lenx * batchSize, sizeof(hcDoubleComplex));  // host input
  hcDoubleComplex *devX = hc::am_alloc(
      sizeof(hcDoubleComplex) * lenx * batchSize, handle->currentAccl, 0);
  int k = 0;
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i].x = rand_r(&global_seed) % 10;
    X[i].y = rand_r(&global_seed) % 20;
    Xcblas[k++] = X[i].x;
    Xcblas[k++] = X[i].y;
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(hcDoubleComplex), X,
                           incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasZdscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasGetVector(handle, lenx * batchSize, sizeof(hcDoubleComplex),
                           devX, incx, X, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for (int i = 0; i < batchSize; i++)
    cblas_zdscal(n, alpha, Xcblas + i * n * 2, incx);
  for (int i = 0, k = 0; i < lenx * batchSize && k < lenx * batchSize * 2;
       i++, k = k + 2) {
    EXPECT_EQ(X[i].x, Xcblas[k]);
    EXPECT_EQ(X[i].y, Xcblas[k + 1]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasZdscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_scopy, func_return_correct_scopy) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  float alpha = 1;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float *)calloc(lenx, sizeof(float));  // host input
  float *Y = (float *)calloc(leny, sizeof(float));
  float *Ycblas = (float *)calloc(leny, sizeof(float));
  float *devX = hc::am_alloc(sizeof(float) * lenx, handle->currentAccl, 0);
  float *devY = hc::am_alloc(sizeof(float) * leny, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny; i++) {
    Y[i] = rand_r(&global_seed) % 15;
    Ycblas[i] = Y[i];
  }
  status = hcblasSetVector(handle, lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasScopy(handle, n, devX, incx, devY, incy);
  handle->currentAcclView.copy(devY, Y, leny * sizeof(float));
  status = hcblasGetVector(handle, leny, sizeof(float), devY, incy, Y, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  cblas_scopy(n, X, incx, Ycblas, incy);
  for (int i = 0; i < leny; i++) {
    EXPECT_EQ(Y[i], Ycblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasScopy(handle, n, devX, incx, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Ycblas);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_scopyBatched, func_return_correct_scopyBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  float alpha = 1;
  int batchSize = 32;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float *)calloc(lenx * batchSize, sizeof(float));  // host input
  float *Y = (float *)calloc(leny * batchSize, sizeof(float));
  float *Ycblas = (float *)calloc(leny * batchSize, sizeof(float));
  float *devX =
      hc::am_alloc(sizeof(float) * lenx * batchSize, handle->currentAccl, 0);
  float *devY =
      hc::am_alloc(sizeof(float) * leny * batchSize, handle->currentAccl, 0);
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny * batchSize; i++) {
    Y[i] = rand_r(&global_seed) % 15;
    Ycblas[i] = Y[i];
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(float), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny * batchSize, sizeof(float), Y, incy,
                           devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasScopyBatched(handle, n, devX, incx, devY, incy, batchSize);
  status = hcblasGetVector(handle, leny * batchSize, sizeof(float), devY, incy,
                           Y, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  handle->currentAcclView.copy(devY, Y, leny * batchSize * sizeof(float));
  for (int i = 0; i < batchSize; i++)
    cblas_scopy(n, X + i * n, incx, Ycblas + i * n, incy);
  for (int i = 0; i < leny * batchSize; i++) {
    EXPECT_EQ(Y[i], Ycblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasScopyBatched(handle, n, devX, incx, devY, incy, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Ycblas);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_dcopy, func_return_correct_dcopy) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  double alpha = 1;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double *)calloc(lenx, sizeof(double));  // host input
  double *Y = (double *)calloc(leny, sizeof(double));
  double *Ycblas = (double *)calloc(leny, sizeof(double));
  double *devX = hc::am_alloc(sizeof(double) * lenx, handle->currentAccl, 0);
  double *devY = hc::am_alloc(sizeof(double) * leny, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny; i++) {
    Y[i] = rand_r(&global_seed) % 15;
    Ycblas[i] = Y[i];
  }
  status = hcblasSetVector(handle, lenx, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny, sizeof(double), Y, incy, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasDcopy(handle, n, devX, incx, devY, incy);
  status = hcblasGetVector(handle, leny, sizeof(double), devY, incy, Y, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  handle->currentAcclView.copy(devY, Y, leny * sizeof(double));
  cblas_dcopy(n, X, incx, Ycblas, incy);
  for (int i = 0; i < leny; i++) {
    EXPECT_EQ(Y[i], Ycblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDcopy(handle, n, devX, incx, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Ycblas);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_dcopyBatched, func_return_correct_dcopyBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  double alpha = 1;
  int batchSize = 32;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double *)calloc(lenx * batchSize, sizeof(double));  // host input
  double *Y = (double *)calloc(leny * batchSize, sizeof(double));
  double *Ycblas = (double *)calloc(leny * batchSize, sizeof(double));
  double *devX =
      hc::am_alloc(sizeof(double) * lenx * batchSize, handle->currentAccl, 0);
  double *devY =
      hc::am_alloc(sizeof(double) * leny * batchSize, handle->currentAccl, 0);
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny * batchSize; i++) {
    Y[i] = rand_r(&global_seed) % 15;
    Ycblas[i] = Y[i];
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(double), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny * batchSize, sizeof(double), Y, incy,
                           devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasDcopyBatched(handle, n, devX, incx, devY, incy, batchSize);
  handle->currentAcclView.copy(devY, Y, leny * batchSize * sizeof(double));
  status = hcblasGetVector(handle, leny * batchSize, sizeof(double), devY, incy,
                           Y, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for (int i = 0; i < batchSize; i++)
    cblas_dcopy(n, X + i * n, incx, Ycblas + i * n, incy);
  for (int i = 0; i < leny * batchSize; i++) {
    EXPECT_EQ(Y[i], Ycblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDcopyBatched(handle, n, devX, incx, devY, incy, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Ycblas);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_sdot, func_return_correct_sdot) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  float result;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float *)calloc(lenx, sizeof(float));  // host input
  float *Y = (float *)calloc(leny, sizeof(float));
  float *devX = hc::am_alloc(sizeof(float) * lenx, handle->currentAccl, 0);
  float *devY = hc::am_alloc(sizeof(float) * leny, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny; i++) {
    Y[i] = rand_r(&global_seed) % 15;
  }
  status = hcblasSetVector(handle, lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSdot(handle, n, devX, incx, devY, incy, &result);
  float dotcblas = 0.0;
  dotcblas = cblas_sdot(n, X, incx, Y, incy);
  EXPECT_EQ(result, dotcblas);

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSdot(handle, n, devX, incx, devY, incy, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_sdotBatched, func_return_correct_sdotBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  float result;
  int batchSize = 32;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float *)calloc(lenx * batchSize, sizeof(float));  // host input
  float *Y = (float *)calloc(leny * batchSize, sizeof(float));
  float *devX =
      hc::am_alloc(sizeof(float) * lenx * batchSize, handle->currentAccl, 0);
  float *devY =
      hc::am_alloc(sizeof(float) * leny * batchSize, handle->currentAccl, 0);
  float *dotcblastemp = (float *)calloc(batchSize, sizeof(float));
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny * batchSize; i++) {
    Y[i] = rand_r(&global_seed) % 15;
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(float), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny * batchSize, sizeof(float), Y, incy,
                           devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status =
      hcblasSdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  float dotcblas = 0.0;
  for (int i = 0; i < batchSize; i++) {
    dotcblastemp[i] = cblas_sdot(n, X + i * n, incx, Y + i * n, incy);
    dotcblas += dotcblastemp[i];
  }
  EXPECT_EQ(result, dotcblas);

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status =
      hcblasSdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_ddot, func_return_correct_ddot) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  double result;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double *)calloc(lenx, sizeof(double));  // host input
  double *Y = (double *)calloc(leny, sizeof(double));
  double *devX = hc::am_alloc(sizeof(double) * lenx, handle->currentAccl, 0);
  double *devY = hc::am_alloc(sizeof(double) * leny, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny; i++) {
    Y[i] = rand_r(&global_seed) % 15;
  }
  status = hcblasSetVector(handle, lenx, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny, sizeof(double), Y, incy, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasDdot(handle, n, devX, incx, devY, incy, &result);
  double dotcblas = 0.0;
  dotcblas = cblas_ddot(n, X, incx, Y, incy);
  EXPECT_EQ(result, dotcblas);

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDdot(handle, n, devX, incx, devY, incy, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_ddotBatched, func_return_correct_ddotBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  double result;
  int batchSize = 32;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double *)calloc(lenx * batchSize, sizeof(double));  // host input
  double *Y = (double *)calloc(leny * batchSize, sizeof(double));
  double *devX =
      hc::am_alloc(sizeof(double) * lenx * batchSize, handle->currentAccl, 0);
  double *devY =
      hc::am_alloc(sizeof(double) * leny * batchSize, handle->currentAccl, 0);
  double *dotcblastemp = (double *)calloc(batchSize, sizeof(double));
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny * batchSize; i++) {
    Y[i] = rand_r(&global_seed) % 15;
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(double), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny * batchSize, sizeof(double), Y, incy,
                           devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status =
      hcblasDdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  double dotcblas = 0.0;
  for (int i = 0; i < batchSize; i++) {
    dotcblastemp[i] = cblas_ddot(n, X + i * n, incx, Y + i * n, incy);
    dotcblas += dotcblastemp[i];
  }
  EXPECT_EQ(result, dotcblas);

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status =
      hcblasDdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_saxpy, func_return_correct_saxpy) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  float alpha = 1;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float *)calloc(lenx, sizeof(float));  // host input
  float *Y = (float *)calloc(leny, sizeof(float));
  float *Ycblas = (float *)calloc(leny, sizeof(float));
  float *devX = hc::am_alloc(sizeof(float) * lenx, handle->currentAccl, 0);
  float *devY = hc::am_alloc(sizeof(float) * leny, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny; i++) {
    Y[i] = rand_r(&global_seed) % 15;
    Ycblas[i] = Y[i];
  }
  status = hcblasSetVector(handle, lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSaxpy(handle, n, &alpha, devX, incx, devY, incy);
  status = hcblasGetVector(handle, leny, sizeof(float), devY, 1, Y, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  cblas_saxpy(n, alpha, X, incx, Ycblas, incy);
  for (int i = 0; i < leny; i++) {
    EXPECT_EQ(Y[i], Ycblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSaxpy(handle, n, &alpha, devX, incx, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Ycblas);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_saxpyBatched, func_return_correct_saxpyBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int n = 123;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  float alpha = 1;
  int batchSize = 32;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float *)calloc(lenx * batchSize, sizeof(float));  // host input
  float *Y = (float *)calloc(leny * batchSize, sizeof(float));
  float *Ycblas = (float *)calloc(leny * batchSize, sizeof(float));
  float *devX =
      hc::am_alloc(sizeof(float) * lenx * batchSize, handle->currentAccl, 0);
  float *devY =
      hc::am_alloc(sizeof(float) * leny * batchSize, handle->currentAccl, 0);
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny * batchSize; i++) {
    Y[i] = rand_r(&global_seed) % 15;
    Ycblas[i] = Y[i];
  }
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(float), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny * batchSize, sizeof(float), Y, incy,
                           devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status =
      hcblasSaxpyBatched(handle, n, &alpha, devX, incx, devY, incy, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status =
      hcblasGetVector(handle, leny * batchSize, sizeof(float), devY, 1, Y, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for (int i = 0; i < batchSize; i++)
    cblas_saxpy(n, alpha, X + i * n, incx, Ycblas + i * n, incy);
  for (int i = 0; i < leny * batchSize; i++) {
    EXPECT_EQ(Y[i], Ycblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status =
      hcblasSaxpyBatched(handle, n, &alpha, devX, incx, devY, incy, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  free(Ycblas);
  hc::am_free(devY);
}

TEST(hcblaswrapper_sger, func_return_correct_sger) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int m = 123;
  int n = 78;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (m - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  float alpha = 1;
  __int64_t lda;
  lda = (handle->Order) ? m : n;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Acblas = (float *)calloc(lenx * leny, sizeof(float));
  float *X = (float *)calloc(lenx, sizeof(float));  // host input
  float *Y = (float *)calloc(leny, sizeof(float));
  float *A = (float *)calloc(lenx * leny, sizeof(float));
  float *devA =
      hc::am_alloc(sizeof(float) * lenx * leny, handle->currentAccl, 0);
  float *devX = hc::am_alloc(sizeof(float) * lenx, handle->currentAccl, 0);
  float *devY = hc::am_alloc(sizeof(float) * leny, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny; i++) {
    Y[i] = rand_r(&global_seed) % 15;
  }
  for (int i = 0; i < lenx * leny; i++) {
    A[i] = rand_r(&global_seed) % 25;
    Acblas[i] = A[i];
  }
  status =
      hcblasSetVector(handle, lenx * leny, sizeof(float), A, incx, devA, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSger(handle, m, n, &alpha, devX, incx, devY, incy, devA, lda);
  status = hcblasGetVector(handle, lenx * leny, sizeof(float), devA, 1, A, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  cblas_sger(order, m, n, alpha, X, incx, Y, incy, Acblas, lda);
  for (int i = 0; i < lenx * leny; i++) {
    EXPECT_EQ(A[i], Acblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSger(handle, m, n, &alpha, devX, incx, devY, incy, devA, lda);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Acblas);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_dger, func_return_correct_dger) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int m = 123;
  int n = 78;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (m - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  double alpha = 1;
  __int64_t lda;
  lda = (handle->Order) ? m : n;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Acblas = (double *)calloc(lenx * leny, sizeof(double));
  double *X = (double *)calloc(lenx, sizeof(double));  // host input
  double *Y = (double *)calloc(leny, sizeof(double));
  double *A = (double *)calloc(lenx * leny, sizeof(double));
  double *devA =
      hc::am_alloc(sizeof(double) * lenx * leny, handle->currentAccl, 0);
  double *devX = hc::am_alloc(sizeof(double) * lenx, handle->currentAccl, 0);
  double *devY = hc::am_alloc(sizeof(double) * leny, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny; i++) {
    Y[i] = rand_r(&global_seed) % 15;
  }
  for (int i = 0; i < lenx * leny; i++) {
    A[i] = rand_r(&global_seed) % 25;
    Acblas[i] = A[i];
  }
  status =
      hcblasSetVector(handle, lenx * leny, sizeof(double), A, incx, devA, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, lenx, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny, sizeof(double), Y, incy, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasDger(handle, m, n, &alpha, devX, incx, devY, incy, devA, lda);
  status = hcblasGetVector(handle, lenx * leny, sizeof(double), devA, 1, A, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  cblas_dger(order, m, n, alpha, X, incx, Y, incy, Acblas, lda);
  for (int i = 0; i < lenx * leny; i++) {
    EXPECT_EQ(A[i], Acblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDger(handle, m, n, &alpha, devX, incx, devY, incy, devA, lda);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Acblas);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_sgerBatched, func_return_correct_sgerBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int m = 123;
  int n = 67;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (m - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  float alpha = 1;
  int batchSize = 32;
  __int64_t lda;
  lda = (handle->Order) ? m : n;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float *)calloc(lenx * batchSize, sizeof(float));  // host input
  float *Y = (float *)calloc(leny * batchSize, sizeof(float));
  float *Acblas = (float *)calloc(leny * lenx * batchSize, sizeof(float));
  float *A = (float *)calloc(lenx * leny * batchSize, sizeof(float));
  float *devA = hc::am_alloc(sizeof(float) * lenx * leny * batchSize,
                             handle->currentAccl, 0);
  float *devX =
      hc::am_alloc(sizeof(float) * lenx * batchSize, handle->currentAccl, 0);
  float *devY =
      hc::am_alloc(sizeof(float) * leny * batchSize, handle->currentAccl, 0);
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny * batchSize; i++) {
    Y[i] = rand_r(&global_seed) % 15;
  }
  for (int i = 0; i < lenx * leny * batchSize; i++) {
    A[i] = rand_r(&global_seed) % 25;
    Acblas[i] = A[i];
  }
  status = hcblasSetVector(handle, lenx * leny * batchSize, sizeof(float), A,
                           incx, devA, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(float), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny * batchSize, sizeof(float), Y, incy,
                           devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSgerBatched(handle, m, n, &alpha, devX, incx, devY, incy, devA,
                             lda, batchSize);
  status = hcblasGetVector(handle, lenx * leny * batchSize, sizeof(float), devA,
                           1, A, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for (int i = 0; i < batchSize; i++)
    cblas_sger(order, m, n, alpha, X + i * m, incx, Y + i * n, incy,
               Acblas + i * m * n, lda);
  for (int i = 0; i < lenx * leny * batchSize; i++) {
    EXPECT_EQ(A[i], Acblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSgerBatched(handle, m, n, &alpha, devX, incx, devY, incy, devA,
                             lda, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  free(Acblas);
  hc::am_free(devY);
  free(A);
  hc::am_free(devA);
}

TEST(hcblaswrapper_dgerBatched, func_return_correct_dgerBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int m = 123;
  int n = 67;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (m - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  double alpha = 1;
  int batchSize = 32;
  __int64_t lda;
  lda = (handle->Order) ? m : n;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double *)calloc(lenx * batchSize, sizeof(double));  // host input
  double *Y = (double *)calloc(leny * batchSize, sizeof(double));
  double *Acblas = (double *)calloc(leny * lenx * batchSize, sizeof(double));
  double *A = (double *)calloc(lenx * leny * batchSize, sizeof(double));
  double *devA = hc::am_alloc(sizeof(double) * lenx * leny * batchSize,
                              handle->currentAccl, 0);
  double *devX =
      hc::am_alloc(sizeof(double) * lenx * batchSize, handle->currentAccl, 0);
  double *devY =
      hc::am_alloc(sizeof(double) * leny * batchSize, handle->currentAccl, 0);
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny * batchSize; i++) {
    Y[i] = rand_r(&global_seed) % 15;
  }
  for (int i = 0; i < lenx * leny * batchSize; i++) {
    A[i] = rand_r(&global_seed) % 25;
    Acblas[i] = A[i];
  }
  status = hcblasSetVector(handle, lenx * leny * batchSize, sizeof(double), A,
                           incx, devA, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, lenx * batchSize, sizeof(double), X, incx,
                           devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny * batchSize, sizeof(double), Y, incy,
                           devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasDgerBatched(handle, m, n, &alpha, devX, incx, devY, incy, devA,
                             lda, batchSize);
  status = hcblasGetVector(handle, lenx * leny * batchSize, sizeof(double),
                           devA, 1, A, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for (int i = 0; i < batchSize; i++)
    cblas_dger(order, m, n, alpha, X + i * m, incx, Y + i * n, incy,
               Acblas + i * m * n, lda);
  for (int i = 0; i < lenx * leny * batchSize; i++) {
    EXPECT_EQ(A[i], Acblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDgerBatched(handle, m, n, &alpha, devX, incx, devY, incy, devA,
                             lda, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  free(Acblas);
  hc::am_free(devY);
  free(A);
  hc::am_free(devA);
}

TEST(hcblaswrapper_sgemv, func_return_correct_sgemv) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int m = 123;
  int n = 78;
  int incx = 1;
  int incy = 1;
  __int64_t lenx;
  __int64_t leny;
  float alpha = 1;
  float beta = 1;
  __int64_t lda;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;
  int row, col;
  row = n;
  col = m;
  lda = m;
  hcblasOperation_t trans = HCBLAS_OP_N;
  CBLAS_TRANSPOSE transa;
  transa = (trans == HCBLAS_OP_N) ? CblasNoTrans : CblasTrans;
  lenx = 1 + (row - 1) * abs(incx);
  leny = 1 + (col - 1) * abs(incy);

  // NoTransA
  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Ycblas = (float *)calloc(leny, sizeof(float));
  float *X = (float *)calloc(lenx, sizeof(float));  // host input
  float *Y = (float *)calloc(leny, sizeof(float));
  float *A = (float *)calloc(lenx * leny, sizeof(float));
  float *devA =
      hc::am_alloc(sizeof(float) * lenx * leny, handle->currentAccl, 0);
  float *devX = hc::am_alloc(sizeof(float) * lenx, handle->currentAccl, 0);
  float *devY = hc::am_alloc(sizeof(float) * leny, handle->currentAccl, 0);
  for (int i = 0; i < lenx; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny; i++) {
    Y[i] = rand_r(&global_seed) % 15;
    Ycblas[i] = Y[i];
  }
  for (int i = 0; i < lenx * leny; i++) {
    A[i] = rand_r(&global_seed) % 25;
  }
  status = hcblasSetVector(handle, lenx * leny, sizeof(float), A, 1, devA, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, lenx, sizeof(float), X, 1, devX, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetVector(handle, leny, sizeof(float), Y, 1, devY, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  status = hcblasSgemv(handle, trans, m, n, &alpha, devA, lda, devX, incx,
                       &beta, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  status = hcblasGetVector(handle, leny, sizeof(float), devY, 1, Y, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  lda = (handle->Order) ? m : n;
  cblas_sgemv(order, transa, m, n, alpha, A, lda, X, incx, beta, Ycblas, incy);
  for (int i = 0; i < leny; i++) {
    EXPECT_EQ(Y[i], Ycblas[i]);
  }
  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSgemv(handle, trans, m, n, &alpha, devA, lda, devX, incx,
                       &beta, devY, incy);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Ycblas);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
  free(A);
  hc::am_free(devA);
}

TEST(hcblaswrapper_sgemvBatched, func_return_correct_sgemvBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int m = 123;
  int n = 67;
  int incx = 1;
  int incy = 1;
  __int64_t lenx;
  __int64_t leny;
  float alpha = 1;
  float beta = 1;
  int batchSize = 32;
  __int64_t lda;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;
  int row, col;
  row = n;
  col = m;
  lda = m;
  hcblasOperation_t trans = HCBLAS_OP_N;
  CBLAS_TRANSPOSE transa;
  transa = (trans == HCBLAS_OP_N) ? CblasNoTrans : CblasTrans;
  lenx = 1 + (row - 1) * abs(incx);
  leny = 1 + (col - 1) * abs(incy);

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float *)calloc(lenx * batchSize, sizeof(float));  // host input
  float *Y = (float *)calloc(leny * batchSize, sizeof(float));
  float *Ycblas = (float *)calloc(leny * batchSize, sizeof(float));
  float *A = (float *)calloc(lenx * leny * batchSize, sizeof(float));
  float *devA = hc::am_alloc(sizeof(float) * lenx * leny * batchSize,
                             handle->currentAccl, 0);
  float *devX =
      hc::am_alloc(sizeof(float) * lenx * batchSize, handle->currentAccl, 0);
  float *devY =
      hc::am_alloc(sizeof(float) * leny * batchSize, handle->currentAccl, 0);
  for (int i = 0; i < lenx * batchSize; i++) {
    X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny * batchSize; i++) {
    Y[i] = rand_r(&global_seed) % 15;
    Ycblas[i] = Y[i];
  }
  for (int i = 0; i < lenx * leny * batchSize; i++) {
    A[i] = rand_r(&global_seed) % 25;
  }

  status = hcblasSetVector(handle, lenx * leny * batchSize, sizeof(float), A, 1,
                           devA, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status =
      hcblasSetVector(handle, lenx * batchSize, sizeof(float), X, 1, devX, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status =
      hcblasSetVector(handle, leny * batchSize, sizeof(float), Y, 1, devY, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  status = hcblasSgemvBatched(handle, trans, m, n, &alpha, devA, lda, devX,
                              incx, &beta, devY, incy, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  status =
      hcblasGetVector(handle, leny * batchSize, sizeof(float), devY, 1, Y, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  lda = (handle->Order) ? m : n;
  for (int i = 0; i < batchSize; i++)
    cblas_sgemv(order, transa, m, n, alpha, A + i * m * n, lda, X + i * row,
                incx, beta, Ycblas + i * col, incy);
  for (int i = 0; i < leny * batchSize; i++) {
    EXPECT_EQ(Y[i], Ycblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSgemvBatched(handle, trans, m, n, &alpha, devA, lda, devX,
                              incx, &beta, devY, incy, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  free(Ycblas);
  hc::am_free(devY);
  free(A);
  hc::am_free(devA);
}

TEST(hcblaswrapper_sgemm, func_return_correct_sgemm) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API

  status = hcblasCreate(&handle, &av);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  float alpha = 2;
  float beta = 2;
  __int64_t lda;
  __int64_t ldb;
  __int64_t ldc;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;
  hcblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  float *A = (float *)calloc(M * K, sizeof(float));
  float *B = (float *)calloc(K * N, sizeof(float));
  float *C = (float *)calloc(M * N, sizeof(float));
  float *C_hcblas = (float *)calloc(M * N, sizeof(float));
  float *C_cblas = (float *)calloc(M * N, sizeof(float));
  float *devA = hc::am_alloc(sizeof(float) * M * K, handle->currentAccl, 0);
  float *devB = hc::am_alloc(sizeof(float) * K * N, handle->currentAccl, 0);
  float *devC = hc::am_alloc(sizeof(float) * M * N, handle->currentAccl, 0);
  for (int i = 0; i < M * K; i++) {
    A[i] = 1;  // rand_r(&global_seed)%100;
  }
  for (int i = 0; i < K * N; i++) {
    B[i] = 1;  // rand_r(&global_seed) % 15;
  }
  for (int i = 0; i < M * N; i++) {
    C[i] = 1;  // rand_r(&global_seed) % 25;
    C_cblas[i] = C[i];
  }

  // float* devC = hc::am_alloc(sizeof(float) * M * N, handle->currentAccl, 0);
  status = hcblasSetMatrix(handle, M, K, sizeof(float), A, 1, devA, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetMatrix(handle, K, N, sizeof(float), B, 1, devB, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetMatrix(handle, M, N, sizeof(float), C, 1, devC, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  // NoTransA and NoTransB */
  typeA = HCBLAS_OP_N;
  typeB = HCBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status = hcblasSgemm(handle, typeA, typeB, M, N, K, &alpha, devA, lda, devB,
                       ldb, &beta, devC, ldc);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  status = hcblasGetMatrix(handle, M, N, sizeof(float), devC, 1, C_hcblas, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  cblas_sgemm(order, Transa, Transb, M, N, K, alpha, A, lda, B, ldb, beta,
              C_cblas, ldc);
  for (int i = 0; i < M * N; i++) {
    EXPECT_EQ(C_hcblas[i], C_cblas[i]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSgemm(handle, typeA, typeB, M, N, K, &alpha, devA, lda, devB,
                       ldb, &beta, devC, ldc);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(A);
  free(B);
  free(C);
  hc::am_free(devA);
  hc::am_free(devB);
  hc::am_free(devC);
  free(C_cblas);
  free(C_hcblas);
}

TEST(hcblaswrapper_sgemmBatched, func_return_correct_sgemmBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  hc::accelerator_view accl_view = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  float alpha = 1;
  float beta = 1;
  __int64_t lda;
  __int64_t ldb;
  __int64_t ldc;
  int batchSize = 32;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;
  hcblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  float *A[batchSize];
  float *B[batchSize];
  float *C[batchSize];
  float *C_hcblas[batchSize];
  float *C_cblas[batchSize];
  // Device pointers stored in host memory
  float *devA[batchSize], *devB[batchSize], *devC[batchSize];
  // Create device double pointer to store device pointers in device memory
  float **d_Aarray = hc::am_alloc(batchSize * sizeof(float *), default_acc, 0);
  float **d_Barray = hc::am_alloc(batchSize * sizeof(float *), default_acc, 0);
  float **d_Carray = hc::am_alloc(batchSize * sizeof(float *), default_acc, 0);
  const size_t aSize = sizeof(float) * M * K;
  const size_t bSize = sizeof(float) * K * N;
  const size_t cSize = sizeof(float) * M * N;

  // Host and Device Array allocation
  for (int i = 0; i < batchSize; i++) {
    A[i] = (float *)malloc(aSize);
    B[i] = (float *)malloc(bSize);
    C[i] = (float *)malloc(cSize);
    C_hcblas[i] = (float *)malloc(cSize);
    C_cblas[i] = (float *)malloc(cSize);
    devA[i] = hc::am_alloc(aSize, default_acc, 0);
    devB[i] = hc::am_alloc(bSize, default_acc, 0);
    devC[i] = hc::am_alloc(cSize, default_acc, 0);
  }

  // Populate the inputs
  for (int b = 0; b < batchSize; b++) {
    // Populate each subscript of array
    for (int i = 0; i < M * K; i++) {
      A[b][i] = rand_r(&global_seed) % 100;
    }
    status = hcblasSetMatrix(handle, M, K, sizeof(float), A[b], 1, devA[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
    for (int i = 0; i < K * N; i++) {
      B[b][i] = rand_r(&global_seed) % 15;
    }
    status = hcblasSetMatrix(handle, K, N, sizeof(float), B[b], 1, devB[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
    for (int i = 0; i < M * N; i++) {
      C[b][i] = rand_r(&global_seed) % 25;
      C_cblas[b][i] = C[b][i];
    }
    status = hcblasSetMatrix(handle, M, N, sizeof(float), C[b], 1, devC[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  }

  // NoTransA and NoTransB */
  typeA = HCBLAS_OP_N;
  typeB = HCBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Copyinng device pointers stored in host memory to device memory
  accl_view.copy(devA, d_Aarray, batchSize * sizeof(float *));
  accl_view.copy(devB, d_Barray, batchSize * sizeof(float *));
  accl_view.copy(devC, d_Carray, batchSize * sizeof(float *));

  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status =
      hcblasSgemmBatched(handle, typeA, typeB, M, N, K, &alpha, d_Aarray, lda,
                         d_Barray, ldb, &beta, d_Carray, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  // Get the results
  for (int b = 0; b < batchSize; b++) {
    status = hcblasGetMatrix(handle, M, N, sizeof(float), devC[b], 1,
                             C_hcblas[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  }

  for (int b = 0; b < batchSize; b++)
    cblas_sgemm(order, Transa, Transb, M, N, K, alpha, A[b], lda, B[b], ldb,
                beta, C_cblas[b], ldc);

  for (int b = 0; b < batchSize; b++) {
    for (int i = 0; i < M * N; i++) {
      EXPECT_EQ(C_hcblas[b][i], C_cblas[b][i]);
    }
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status =
      hcblasSgemmBatched(handle, typeA, typeB, M, N, K, &alpha, d_Aarray, lda,
                         d_Barray, ldb, &beta, d_Carray, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);
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
}

TEST(hcblaswrapper_dgemmBatched, func_return_correct_dgemmBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  hc::accelerator_view accl_view = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  double alpha = 1;
  double beta = 1;
  __int64_t lda;
  __int64_t ldb;
  __int64_t ldc;
  int batchSize = 32;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;
  hcblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  double *A[batchSize];
  double *B[batchSize];
  double *C[batchSize];
  double *C_hcblas[batchSize];
  double *C_cblas[batchSize];
  // Device pointers stored in host memory
  double *devA[batchSize], *devB[batchSize], *devC[batchSize];
  // Create device double pointer to store device pointers in device memory
  double **d_Aarray =
      hc::am_alloc(batchSize * sizeof(double *), default_acc, 0);
  double **d_Barray =
      hc::am_alloc(batchSize * sizeof(double *), default_acc, 0);
  double **d_Carray =
      hc::am_alloc(batchSize * sizeof(double *), default_acc, 0);
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
    devA[i] = hc::am_alloc(aSize, default_acc, 0);
    devB[i] = hc::am_alloc(bSize, default_acc, 0);
    devC[i] = hc::am_alloc(cSize, default_acc, 0);
  }

  // Populate the inputs
  for (int b = 0; b < batchSize; b++) {
    // Populate each subscript of array
    for (int i = 0; i < M * K; i++) {
      A[b][i] = rand_r(&global_seed) % 100;
    }
    status = hcblasSetMatrix(handle, M, K, sizeof(double), A[b], 1, devA[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
    for (int i = 0; i < K * N; i++) {
      B[b][i] = rand_r(&global_seed) % 15;
    }
    status = hcblasSetMatrix(handle, K, N, sizeof(double), B[b], 1, devB[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
    for (int i = 0; i < M * N; i++) {
      C[b][i] = rand_r(&global_seed) % 25;
      C_cblas[b][i] = C[b][i];
    }
    status = hcblasSetMatrix(handle, M, N, sizeof(double), C[b], 1, devC[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  }

  // NoTransA and NoTransB */
  typeA = HCBLAS_OP_N;
  typeB = HCBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Copyinng device pointers stored in host memory to device memory
  accl_view.copy(devA, d_Aarray, batchSize * sizeof(double *));
  accl_view.copy(devB, d_Barray, batchSize * sizeof(double *));
  accl_view.copy(devC, d_Carray, batchSize * sizeof(double *));

  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status =
      hcblasDgemmBatched(handle, typeA, typeB, M, N, K, &alpha, d_Aarray, lda,
                         d_Barray, ldb, &beta, d_Carray, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  // Get the results
  for (int b = 0; b < batchSize; b++) {
    status = hcblasGetMatrix(handle, M, N, sizeof(double), devC[b], 1,
                             C_hcblas[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  }

  for (int b = 0; b < batchSize; b++)
    cblas_dgemm(order, Transa, Transb, M, N, K, alpha, A[b], lda, B[b], ldb,
                beta, C_cblas[b], ldc);

  for (int b = 0; b < batchSize; b++) {
    for (int i = 0; i < M * N; i++) {
      EXPECT_EQ(C_hcblas[b][i], C_cblas[b][i]);
    }
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status =
      hcblasDgemmBatched(handle, typeA, typeB, M, N, K, &alpha, d_Aarray, lda,
                         d_Barray, ldb, &beta, d_Carray, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);
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
}

TEST(hcblaswrapper_cgemm, func_return_correct_cgemm) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  __int64_t lda;
  __int64_t ldb;
  __int64_t ldc;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;
  hcblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  float alpha[2], beta[2];
  hcComplex cAlpha, cBeta;
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  hcComplex *A = (hcComplex *)calloc(M * K, sizeof(hcComplex));
  hcComplex *B = (hcComplex *)calloc(K * N, sizeof(hcComplex));
  hcComplex *C = (hcComplex *)calloc(M * N, sizeof(hcComplex));
  hcComplex *devA =
      hc::am_alloc(sizeof(hcComplex) * M * K, handle->currentAccl, 0);
  hcComplex *devB =
      hc::am_alloc(sizeof(hcComplex) * K * N, handle->currentAccl, 0);
  hcComplex *devC =
      hc::am_alloc(sizeof(hcComplex) * M * N, handle->currentAccl, 0);
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

  status = hcblasSetMatrix(handle, M, K, sizeof(hcComplex), A, 1, devA, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetMatrix(handle, K, N, sizeof(hcComplex), B, 1, devB, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetMatrix(handle, M, N, sizeof(hcComplex), C, 1, devC, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  // NoTransA and NoTransB */
  typeA = HCBLAS_OP_N;
  typeB = HCBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status = hcblasCgemm(handle, typeA, typeB, M, N, K, &cAlpha, devA, lda, devB,
                       ldb, &cBeta, devC, ldc);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  status = hcblasGetMatrix(handle, M, N, sizeof(hcComplex), devC, 1, C, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  cblas_cgemm(order, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb,
              &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasCgemm(handle, typeA, typeB, M, N, K, &cAlpha, devA, lda, devB,
                       ldb, &cBeta, devC, ldc);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(A);
  free(B);
  free(C);
  hc::am_free(devA);
  hc::am_free(devB);
  hc::am_free(devC);
  free(ablas);
  free(bblas);
  free(cblas);
}

TEST(hcblaswrapper_cgemmBatched, func_return_correct_cgemmBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  __int64_t lda;
  __int64_t ldb;
  __int64_t ldc;
  int batchSize = 64;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;
  hcblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  float alpha[2], beta[2];
  hcComplex cAlpha, cBeta;
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;

  hcComplex *devA[batchSize], *devB[batchSize], *devC[batchSize];

  hcComplex *A[batchSize];
  hcComplex *B[batchSize];
  hcComplex *C[batchSize];
  hcComplex **d_Aarray =
      hc::am_alloc(sizeof(hcComplex *) * batchSize, default_acc, 0);
  hcComplex **d_Barray =
      hc::am_alloc(sizeof(hcComplex *) * batchSize, default_acc, 0);
  hcComplex **d_Carray =
      hc::am_alloc(sizeof(hcComplex *) * batchSize, default_acc, 0);
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
    int k = 0;
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
    k = 0;
    for (int i = 0; i < M * N; i++) {
      C[b][i].x = rand_r(&global_seed) % 18;
      C[b][i].y = rand_r(&global_seed) % 28;
      cblas[b][k++] = C[b][i].x;
      cblas[b][k++] = C[b][i].y;
    }
  }

  for (int b = 0; b < batchSize; b++) {
    status =
        hcblasSetMatrix(handle, M, K, sizeof(hcComplex), A[b], 1, devA[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
    status =
        hcblasSetMatrix(handle, K, N, sizeof(hcComplex), B[b], 1, devB[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
    status =
        hcblasSetMatrix(handle, M, N, sizeof(hcComplex), C[b], 1, devC[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  }

  // NoTransA and NoTransB */
  typeA = HCBLAS_OP_N;
  typeB = HCBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Copyinng device pointers stored in host memory to device memory
  av.copy(devA, d_Aarray, batchSize * sizeof(hcComplex *));
  av.copy(devB, d_Barray, batchSize * sizeof(hcComplex *));
  av.copy(devC, d_Carray, batchSize * sizeof(hcComplex *));

  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status =
      hcblasCgemmBatched(handle, typeA, typeB, M, N, K, &cAlpha, d_Aarray, lda,
                         d_Barray, ldb, &cBeta, d_Carray, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  for (int b = 0; b < batchSize; b++) {
    status =
        hcblasGetMatrix(handle, M, N, sizeof(hcComplex), devC[b], 1, C[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  }

  for (int b = 0; b < batchSize; b++) {
    cblas_cgemm(order, Transa, Transb, M, N, K, &alpha, ablas[b], lda, bblas[b],
                ldb, &beta, cblas[b], ldc);
  }

  for (int b = 0; b < batchSize; b++) {
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(C[b][i].x, cblas[b][k]);
      EXPECT_EQ(C[b][i].y, cblas[b][k + 1]);
    }
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status =
      hcblasCgemmBatched(handle, typeA, typeB, M, N, K, &cAlpha, d_Aarray, lda,
                         d_Barray, ldb, &cBeta, d_Carray, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

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

#ifdef HGEMM_UNIT_TESTING

TEST(hcblaswrapper_hgemm, func_return_correct_hgemm) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);

  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  half alpha = 1;
  half beta = 1;
  __int64_t lda;
  __int64_t ldb;
  __int64_t ldc;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;
  hcblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  half *A = (half *)calloc(M * K, sizeof(half));
  half *B = (half *)calloc(K * N, sizeof(half));
  half *C = (half *)calloc(M * N, sizeof(half));
  half *C_hcblas = (half *)calloc(M * N, sizeof(half));
  half *C_cblas = (half *)calloc(M * N, sizeof(half));
  half *devA = hc::am_alloc(sizeof(half) * M * K, handle->currentAccl, 0);
  half *devB = hc::am_alloc(sizeof(half) * K * N, handle->currentAccl, 0);
  half *devC = hc::am_alloc(sizeof(half) * M * N, handle->currentAccl, 0);

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

  status = hcblasSetMatrix(handle, M, K, sizeof(half), A, 1, devA, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetMatrix(handle, K, N, sizeof(half), B, 1, devB, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status = hcblasSetMatrix(handle, M, N, sizeof(half), C, 1, devC, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  // NoTransA and NoTransB */
  typeA = HCBLAS_OP_N;
  typeB = HCBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status = hcblasHgemm(handle, typeA, typeB, M, N, K, &alpha, devA, lda, devB,
                       ldb, &beta, devC, ldc);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  status = hcblasGetMatrix(handle, M, N, sizeof(half), devC, 1, C_hcblas, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  cblas_hgemm(M, N, K, A, B, C_cblas, alpha, beta);

  for (int i = 0; i < M * N; i++) EXPECT_EQ(C_hcblas[i], C_cblas[i]);

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasHgemm(handle, typeA, typeB, M, N, K, &alpha, devA, lda, devB,
                       ldb, &beta, devC, ldc);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(A);
  free(B);
  free(C);
  hc::am_free(devA);
  hc::am_free(devB);
  hc::am_free(devC);
  free(C_cblas);
  free(C_hcblas);
}

TEST(hcblaswrapper_zgemm, func_return_correct_zgemm) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  __int64_t lda;
  __int64_t ldb;
  __int64_t ldc;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;
  hcblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  double alpha[2], beta[2];
  hcDoubleComplex cAlpha, cBeta;
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;
  hcDoubleComplex *A =
      (hcDoubleComplex *)calloc(M * K, sizeof(hcDoubleComplex));
  hcDoubleComplex *B =
      (hcDoubleComplex *)calloc(K * N, sizeof(hcDoubleComplex));
  hcDoubleComplex *C =
      (hcDoubleComplex *)calloc(M * N, sizeof(hcDoubleComplex));
  hcDoubleComplex *devA =
      hc::am_alloc(sizeof(hcDoubleComplex) * M * K, handle->currentAccl, 0);
  hcDoubleComplex *devB =
      hc::am_alloc(sizeof(hcDoubleComplex) * K * N, handle->currentAccl, 0);
  hcDoubleComplex *devC =
      hc::am_alloc(sizeof(hcDoubleComplex) * M * N, handle->currentAccl, 0);
  double *ablas = (double *)malloc(sizeof(double) * M * K * 2);
  double *bblas = (double *)malloc(sizeof(double) * K * N * 2);
  double *cblas = (double *)malloc(sizeof(double) * M * N * 2);
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

  status =
      hcblasSetMatrix(handle, M, K, sizeof(hcDoubleComplex), A, 1, devA, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status =
      hcblasSetMatrix(handle, K, N, sizeof(hcDoubleComplex), B, 1, devB, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  status =
      hcblasSetMatrix(handle, M, N, sizeof(hcDoubleComplex), C, 1, devC, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  // NoTransA and NoTransB */
  typeA = HCBLAS_OP_N;
  typeB = HCBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status = hcblasZgemm(handle, typeA, typeB, M, N, K, &cAlpha, devA, lda, devB,
                       ldb, &cBeta, devC, ldc);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  status =
      hcblasGetMatrix(handle, M, N, sizeof(hcDoubleComplex), devC, 1, C, 1);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  cblas_zgemm(order, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb,
              &beta, cblas, ldc);
  for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
    EXPECT_EQ(C[i].x, cblas[k]);
    EXPECT_EQ(C[i].y, cblas[k + 1]);
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasZgemm(handle, typeA, typeB, M, N, K, &cAlpha, devA, lda, devB,
                       ldb, &cBeta, devC, ldc);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(A);
  free(B);
  free(C);
  hc::am_free(devA);
  hc::am_free(devB);
  hc::am_free(devC);
  free(ablas);
  free(bblas);
  free(cblas);
}

TEST(hcblaswrapper_zgemmBatched, func_return_correct_zgemmBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  __int64_t lda;
  __int64_t ldb;
  __int64_t ldc;
  int batchSize = 64;
  CBLAS_ORDER order;
  order = (handle->Order) ? CblasColMajor : CblasRowMajor;
  hcblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  double alpha[2], beta[2];
  hcDoubleComplex cAlpha, cBeta;
  cAlpha.x = 1;
  cAlpha.y = 1;
  cBeta.x = 1;
  cBeta.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;
  beta[0] = cBeta.x;
  beta[1] = cBeta.y;

  hcDoubleComplex *devA[batchSize], *devB[batchSize], *devC[batchSize];

  hcDoubleComplex *A[batchSize];
  hcDoubleComplex *B[batchSize];
  hcDoubleComplex *C[batchSize];
  hcDoubleComplex **d_Aarray =
      hc::am_alloc(sizeof(hcDoubleComplex *) * batchSize, default_acc, 0);
  hcDoubleComplex **d_Barray =
      hc::am_alloc(sizeof(hcDoubleComplex *) * batchSize, default_acc, 0);
  hcDoubleComplex **d_Carray =
      hc::am_alloc(sizeof(hcDoubleComplex *) * batchSize, default_acc, 0);
  double *ablas[batchSize];
  double *bblas[batchSize];
  double *cblas[batchSize];

  for (int b = 0; b < batchSize; b++) {
    A[b] = (hcDoubleComplex *)calloc(M * K, sizeof(hcDoubleComplex));
    B[b] = (hcDoubleComplex *)calloc(K * N, sizeof(hcDoubleComplex));
    C[b] = (hcDoubleComplex *)calloc(M * N, sizeof(hcDoubleComplex));
    devA[b] = hc::am_alloc(sizeof(hcDoubleComplex) * M * K, default_acc, 0);
    devB[b] = hc::am_alloc(sizeof(hcDoubleComplex) * K * N, default_acc, 0);
    devC[b] = hc::am_alloc(sizeof(hcDoubleComplex) * M * N, default_acc, 0);
    ablas[b] = (double *)malloc(sizeof(double) * M * K * 2);
    bblas[b] = (double *)malloc(sizeof(double) * K * N * 2);
    cblas[b] = (double *)malloc(sizeof(double) * M * N * 2);
  }
  for (int b = 0; b < batchSize; b++) {
    int k = 0;
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
    k = 0;
    for (int i = 0; i < M * N; i++) {
      C[b][i].x = rand_r(&global_seed) % 18;
      C[b][i].y = rand_r(&global_seed) % 28;
      cblas[b][k++] = C[b][i].x;
      cblas[b][k++] = C[b][i].y;
    }
  }

  for (int b = 0; b < batchSize; b++) {
    status = hcblasSetMatrix(handle, M, K, sizeof(hcDoubleComplex), A[b], 1,
                             devA[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
    status = hcblasSetMatrix(handle, K, N, sizeof(hcDoubleComplex), B[b], 1,
                             devB[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
    status = hcblasSetMatrix(handle, M, N, sizeof(hcDoubleComplex), C[b], 1,
                             devC[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  }

  // NoTransA and NoTransB */
  typeA = HCBLAS_OP_N;
  typeB = HCBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Copyinng device pointers stored in host memory to device memory
  av.copy(devA, d_Aarray, batchSize * sizeof(hcDoubleComplex *));
  av.copy(devB, d_Barray, batchSize * sizeof(hcDoubleComplex *));
  av.copy(devC, d_Carray, batchSize * sizeof(hcDoubleComplex *));

  // Column major */
  lda = M;
  ldb = K;
  ldc = M;
  status =
      hcblasZgemmBatched(handle, typeA, typeB, M, N, K, &cAlpha, d_Aarray, lda,
                         d_Barray, ldb, &cBeta, d_Carray, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  for (int b = 0; b < batchSize; b++) {
    status = hcblasGetMatrix(handle, M, N, sizeof(hcDoubleComplex), devC[b], 1,
                             C[b], 1);
    EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  }

  for (int b = 0; b < batchSize; b++) {
    cblas_zgemm(order, Transa, Transb, M, N, K, &alpha, ablas[b], lda, bblas[b],
                ldb, &beta, cblas[b], ldc);
  }

  for (int b = 0; b < batchSize; b++) {
    for (int i = 0, k = 0; ((i < M * N) && (k < M * N * 2)); i++, k = k + 2) {
      EXPECT_EQ(C[b][i].x, cblas[b][k]);
      EXPECT_EQ(C[b][i].y, cblas[b][k + 1]);
    }
  }

  handle->currentAcclView.wait();
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status =
      hcblasZgemmBatched(handle, typeA, typeB, M, N, K, &cAlpha, d_Aarray, lda,
                         d_Barray, ldb, &cBeta, d_Carray, ldc, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

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
#endif
