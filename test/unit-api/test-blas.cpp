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

  status = hcblasSetVector(n, sizeof(X), X , incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  status = hcblasSasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  float asumcblas = 0.0;
  asumcblas = cblas_sasum( n, X, incx);
  EXPECT_EQ(result, asumcblas);
/*
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(handle);
  status = hcblasSasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED); 
*/
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
  status = hcblasSetVector(n, sizeof(X), X , incx, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  status = hcblasDasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

  double asumcblas = 0.0;
  asumcblas = cblas_dasum( n, X, incx);
  EXPECT_EQ(result, asumcblas);
/*
  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(handle);
  status = hcblasDasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);
*/
  free(X);
  hc::am_free(devX);
}
