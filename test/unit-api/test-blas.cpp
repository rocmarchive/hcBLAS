#include "hcblas.h"
#include "gtest/gtest.h"
#include "hc_am.hpp"
#include "cblas.h"

TEST(hcblaswrapper_sasum, func_return_correct_sasum) {
  hcblasStatus_t status;
  hcblasHandle_t handle;
  status = hcblasCreate(&handle);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float result;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float* devX = hc::am_alloc(sizeof(float) * lenx, acc[handle.deviceId], 0);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  hc::am_copy(devX, X, lenx * sizeof(float));
  status = hcblasSasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  float asumcblas = 0.0;
  asumcblas = cblas_sasum( n, X, incx);
  EXPECT_EQ(result, asumcblas);

  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED); 

  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_sasumBatched, func_return_correct_sasumBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle;
  status = hcblasCreate(&handle);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float result;
  int batchSize = 128;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float* devX = hc::am_alloc(sizeof(float) * lenx * batchSize, acc[handle.deviceId], 0);
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
  }
  hc::am_copy(devX, X, lenx * batchSize * sizeof(float));
  status = hcblasSasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  float asumcblas = 0.0;
  float *asumcblastemp = (float*)calloc(batchSize, sizeof(float));
  for(int i = 0; i < batchSize; i++) {
                asumcblastemp[i] = cblas_sasum( n, X + i * n, incx);
                asumcblas += asumcblastemp[i];
  }
  EXPECT_EQ(result, asumcblas);

  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasSasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED); 

  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_dasum, func_return_correct_dasum) {
  hcblasStatus_t status;
  hcblasHandle_t handle;
  status = hcblasCreate(&handle);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double result;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double*)calloc(lenx, sizeof(double));//host input
  double* devX = hc::am_alloc(sizeof(double) * lenx, acc[handle.deviceId], 0);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
   }
  hc::am_copy(devX, X, lenx * sizeof(double));
  status = hcblasDasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  double asumcblas = 0.0;
  asumcblas = cblas_dasum( n, X, incx);
  EXPECT_EQ(result, asumcblas);

  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_dasumBatched, func_return_correct_dasumBatched) {
  hcblasStatus_t status;
  hcblasHandle_t handle;
  status = hcblasCreate(&handle);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double result;
  int batchSize = 128;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double*)calloc(lenx * batchSize, sizeof(double));//host input
  double* devX = hc::am_alloc(sizeof(double) * lenx * batchSize, acc[handle.deviceId], 0);
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
   }
  hc::am_copy(devX, X, lenx * batchSize * sizeof(double));
  status = hcblasDasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  double asumcblas = 0.0;
  double *asumcblastemp = (double*)calloc(batchSize, sizeof(double));
  for(int i = 0; i < batchSize; i++) {
                asumcblastemp[i] = cblas_dasum( n, X + i * n, incx);
                asumcblas += asumcblastemp[i];
  }
  EXPECT_EQ(result, asumcblas);


  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_sscal, func_return_correct_sscal) {
  hcblasStatus_t status;
  hcblasHandle_t handle;
  status = hcblasCreate(&handle);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float alpha = 1;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float*)calloc(lenx, sizeof(float));
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float* devX = hc::am_alloc(sizeof(float) * lenx, acc[handle.deviceId], 0);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
            Xcblas[i] = X[i];
  }
  hc::am_copy(devX, X, lenx * sizeof(float));
  status = hcblasSscal(handle, n, &alpha, devX, incx);
  hc::am_copy(X, devX, lenx * sizeof(float));
  cblas_sscal( n, alpha, Xcblas, incx );
  for(int i = 0; i < lenx ; i++){
        EXPECT_EQ(X[i], Xcblas[i]);
  }

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
  hcblasHandle_t handle;
  status = hcblasCreate(&handle);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float alpha = 1;
  int batchSize = 128;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float*)calloc(lenx * batchSize , sizeof(float));
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float* devX = hc::am_alloc(sizeof(float) * lenx * batchSize, acc[handle.deviceId], 0);
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
            Xcblas[i] =  X[i];
  }
  hc::am_copy(devX, X, lenx * batchSize * sizeof(float));
  status = hcblasSscalBatched(handle, n, &alpha, devX, incx, batchSize);
  hc::am_copy(X, devX, lenx * batchSize * sizeof(float));
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for(int i = 0; i < batchSize; i++)
          cblas_sscal( n, alpha, Xcblas + i * n, incx);
  for(int i =0; i < lenx * batchSize; i ++){
          EXPECT_EQ(X[i], Xcblas[i]);
  }

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
  hcblasHandle_t handle;
  status = hcblasCreate(&handle);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double alpha = 1;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double*)calloc(lenx, sizeof(double));
  double *X = (double*)calloc(lenx, sizeof(double));//host input
  double* devX = hc::am_alloc(sizeof(double) * lenx, acc[handle.deviceId], 0);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
            Xcblas[i] = X[i];
  }
  hc::am_copy(devX, X, lenx * sizeof(double));
  status = hcblasDscal(handle, n, &alpha, devX, incx);
  hc::am_copy(X, devX, lenx * sizeof(double));
  cblas_dscal( n, alpha, Xcblas, incx );
  for(int i = 0; i < lenx ; i++){
        EXPECT_EQ(X[i], Xcblas[i]);
  }

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
  hcblasHandle_t handle;
  status = hcblasCreate(&handle);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double alpha = 1;
  int batchSize = 128;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double*)calloc(lenx * batchSize , sizeof(double));
  double *X = (double*)calloc(lenx * batchSize, sizeof(double));//host input
  double* devX = hc::am_alloc(sizeof(double) * lenx * batchSize, acc[handle.deviceId], 0);
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
            Xcblas[i] =  X[i];
  }
  hc::am_copy(devX, X, lenx * batchSize * sizeof(double));
  status = hcblasDscalBatched(handle, n, &alpha, devX, incx, batchSize);
  hc::am_copy(X, devX, lenx * batchSize * sizeof(double));
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
  for(int i = 0; i < batchSize; i++)
          cblas_dscal( n, alpha, Xcblas + i * n, incx);
  for(int i =0; i < lenx * batchSize; i ++){
          EXPECT_EQ(X[i], Xcblas[i]);
  }

  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(&handle);
  status = hcblasDscalBatched(handle, n, &alpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED); 

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

