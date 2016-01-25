#include "hcblas.h"
#include "gtest/gtest.h"
#include "hc_am.hpp"
#include "cblas.h"

TEST(hcblaswrapper_sasum, func_return_correct_sasum) {
  hcblasStatus_t status;
  hcblasHandle_t *handle =  hcblasCreate();
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float result;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float* devX = hc::am_alloc(sizeof(float) * lenx, acc[handle->deviceId], 0);
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
  hcblasDestroy(handle);
  status = hcblasSasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED); 

  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_sasumBatched, func_return_correct_sasumBatched) {
  hcblasStatus_t status;
  hcblasHandle_t *handle = hcblasCreate();
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float result;
  int batchSize = 128;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float* devX = hc::am_alloc(sizeof(float) * lenx * batchSize, acc[handle->deviceId], 0);
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
  hcblasDestroy(handle);
  status = hcblasSasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED); 

  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_dasum, func_return_correct_dasum) {
  hcblasStatus_t status;
  hcblasHandle_t *handle = hcblasCreate();
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double result;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double*)calloc(lenx, sizeof(double));//host input
  double* devX = hc::am_alloc(sizeof(double) * lenx, acc[handle->deviceId], 0);
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
  hcblasDestroy(handle);
  status = hcblasDasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_dasumBatched, func_return_correct_dasumBatched) {
  hcblasStatus_t status;
  hcblasHandle_t *handle = hcblasCreate();
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double result;
  int batchSize = 128;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double*)calloc(lenx * batchSize, sizeof(double));//host input
  double* devX = hc::am_alloc(sizeof(double) * lenx * batchSize, acc[handle->deviceId], 0);
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
  hcblasDestroy(handle);
  status = hcblasDasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
}

TEST(hcblaswrapper_sscal, func_return_correct_sscal) {
  hcblasStatus_t status;
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float alpha = 1;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float*)calloc(lenx, sizeof(float));
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float* devX = hc::am_alloc(sizeof(float) * lenx, acc[handle->deviceId], 0);
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
  hcblasDestroy(handle);
  status = hcblasSscal(handle, n, &alpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED); 

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_sscalBatched, func_return_correct_sscalBatched) {
  hcblasStatus_t status;
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float alpha = 1;
  int batchSize = 128;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float*)calloc(lenx * batchSize , sizeof(float));
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float* devX = hc::am_alloc(sizeof(float) * lenx * batchSize, acc[handle->deviceId], 0);
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
  hcblasDestroy(handle);
  status = hcblasSscalBatched(handle, n, &alpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED); 

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_dscal, func_return_correct_dscal) {
  hcblasStatus_t status;
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double alpha = 1;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double*)calloc(lenx, sizeof(double));
  double *X = (double*)calloc(lenx, sizeof(double));//host input
  double* devX = hc::am_alloc(sizeof(double) * lenx, acc[handle->deviceId], 0);
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
  hcblasDestroy(handle);
  status = hcblasDscal(handle, n, &alpha, devX, incx);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED); 

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_dscalBatched, func_return_correct_dscalBatched) {
  hcblasStatus_t status;
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double alpha = 1;
  int batchSize = 128;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double*)calloc(lenx * batchSize , sizeof(double));
  double *X = (double*)calloc(lenx * batchSize, sizeof(double));//host input
  double* devX = hc::am_alloc(sizeof(double) * lenx * batchSize, acc[handle->deviceId], 0);
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
  hcblasDestroy(handle);
  status = hcblasDscalBatched(handle, n, &alpha, devX, incx, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED); 

  free(X);
  free(Xcblas);
  hc::am_free(devX);
}

TEST(hcblaswrapper_scopy, func_return_correct_scopy) {
  hcblasStatus_t status;
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float alpha = 1;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float *Y = (float*)calloc(leny, sizeof(float));
  float *Ycblas = (float*)calloc(leny, sizeof(float));
  float* devX = hc::am_alloc(sizeof(float) * lenx, acc[handle->deviceId], 0);
  float* devY = hc::am_alloc(sizeof(float) * leny, acc[handle->deviceId], 0);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  hc::am_copy(devX, X, lenx * sizeof(float));
  hc::am_copy(devY, Y, leny * sizeof(float));
  status = hcblasScopy(handle, n, devX, incx, devY, incy);
  hc::am_copy(Y, devY, leny * sizeof(float));
  cblas_scopy( n, X, incx, Ycblas, incy );
  for(int i = 0; i < leny; i++){
        EXPECT_EQ(Y[i], Ycblas[i]);
  }

  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(handle);
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
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float alpha = 1;
  int batchSize = 32; 
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float *Y = (float*)calloc(leny * batchSize, sizeof(float));
  float *Ycblas = (float*)calloc(leny *  batchSize, sizeof(float));
  float* devX = hc::am_alloc(sizeof(float) * lenx *  batchSize, acc[handle->deviceId], 0);
  float* devY = hc::am_alloc(sizeof(float) * leny *  batchSize, acc[handle->deviceId], 0);
  for(int i = 0; i < lenx *  batchSize; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny *  batchSize;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  hc::am_copy(devX, X, lenx *  batchSize * sizeof(float));
  hc::am_copy(devY, Y, leny *  batchSize * sizeof(float));
  status = hcblasScopyBatched(handle, n, devX, incx, devY, incy, batchSize);
  hc::am_copy(Y, devY, leny * batchSize * sizeof(float));
  for(int i = 0; i < batchSize; i++)
      cblas_scopy( n, X + i * n, incx, Ycblas + i * n, incy );
  for(int i = 0; i < leny * batchSize; i++){
        EXPECT_EQ(Y[i], Ycblas[i]);
  }

  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(handle);
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
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  double alpha = 1;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double*)calloc(lenx, sizeof(double));//host input
  double *Y = (double*)calloc(leny, sizeof(double));
  double *Ycblas = (double*)calloc(leny, sizeof(double));
  double* devX = hc::am_alloc(sizeof(double) * lenx, acc[handle->deviceId], 0);
  double* devY = hc::am_alloc(sizeof(double) * leny, acc[handle->deviceId], 0);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  hc::am_copy(devX, X, lenx * sizeof(double));
  hc::am_copy(devY, Y, leny * sizeof(double));
  status = hcblasDcopy(handle, n, devX, incx, devY, incy);
  hc::am_copy(Y, devY, leny * sizeof(double));
  cblas_dcopy( n, X, incx, Ycblas, incy );
  for(int i = 0; i < leny; i++){
        EXPECT_EQ(Y[i], Ycblas[i]);
  }

  // HCBLAS_STATUS_NOT_INITIALIZED
   hcblasDestroy(handle);
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
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  double alpha = 1;
  int batchSize = 32;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double*)calloc(lenx * batchSize, sizeof(double));//host input
  double *Y = (double*)calloc(leny * batchSize, sizeof(double));
  double *Ycblas = (double*)calloc(leny *  batchSize, sizeof(double));
  double* devX = hc::am_alloc(sizeof(double) * lenx *  batchSize, acc[handle->deviceId], 0);
  double* devY = hc::am_alloc(sizeof(double) * leny *  batchSize, acc[handle->deviceId], 0);
  for(int i = 0; i < lenx *  batchSize; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny *  batchSize;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  hc::am_copy(devX, X, lenx *  batchSize * sizeof(double));
  hc::am_copy(devY, Y, leny *  batchSize * sizeof(double));
  status = hcblasDcopyBatched(handle, n, devX, incx, devY, incy, batchSize);
  hc::am_copy(Y, devY, leny * batchSize * sizeof(double));
  for(int i = 0; i < batchSize; i++)
      cblas_dcopy( n, X + i * n, incx, Ycblas + i * n, incy );
  for(int i = 0; i < leny * batchSize; i++){
        EXPECT_EQ(Y[i], Ycblas[i]);
  }

  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(handle);
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
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float result;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float *Y = (float*)calloc(leny, sizeof(float));
  float* devX = hc::am_alloc(sizeof(float) * lenx, acc[handle->deviceId], 0);
  float* devY = hc::am_alloc(sizeof(float) * leny, acc[handle->deviceId], 0);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
  }
  hc::am_copy(devX, X, lenx * sizeof(float));
  hc::am_copy(devY, Y, leny * sizeof(float));
  status = hcblasSdot(handle, n, devX, incx, devY, incy, &result);
  float  dotcblas = 0.0;
  dotcblas = cblas_sdot( n, X, incx, Y, incy);
  EXPECT_EQ(result, dotcblas);

  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(handle);
  status = hcblasSdot(handle, n, devX, incx, devY, incy, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_sdotBatched, func_return_correct_sdotBatched) {
  hcblasStatus_t status;
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float result;
  int batchSize = 32;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float *Y = (float*)calloc(leny * batchSize, sizeof(float));
  float* devX = hc::am_alloc(sizeof(float) * lenx * batchSize, acc[handle->deviceId], 0);
  float* devY = hc::am_alloc(sizeof(float) * leny * batchSize, acc[handle->deviceId], 0);
  float *dotcblastemp =(float*)calloc(batchSize, sizeof(float));
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny * batchSize;i++){
            Y[i] =  rand() % 15;
  }
  hc::am_copy(devX, X, lenx * batchSize * sizeof(float));
  hc::am_copy(devY, Y, leny * batchSize * sizeof(float));
  status = hcblasSdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  float  dotcblas = 0.0;
  for(int i = 0; i < batchSize; i++){
                dotcblastemp[i] = cblas_sdot( n, X + i * n, incx, Y + i * n, incy);
                dotcblas += dotcblastemp[i];
  }
  EXPECT_EQ(result, dotcblas);

  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(handle);
  status = hcblasSdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_ddot, func_return_correct_ddot) {
  hcblasStatus_t status;
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  double result;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double*)calloc(lenx, sizeof(double));//host input
  double *Y = (double*)calloc(leny, sizeof(double));
  double* devX = hc::am_alloc(sizeof(double) * lenx, acc[handle->deviceId], 0);
  double* devY = hc::am_alloc(sizeof(double) * leny, acc[handle->deviceId], 0);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
  }
  hc::am_copy(devX, X, lenx * sizeof(double));
  hc::am_copy(devY, Y, leny * sizeof(double));
  status = hcblasDdot(handle, n, devX, incx, devY, incy, &result);
  double  dotcblas = 0.0;
  dotcblas = cblas_ddot( n, X, incx, Y, incy);
  EXPECT_EQ(result, dotcblas);

  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(handle);
  status = hcblasDdot(handle, n, devX, incx, devY, incy, &result);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}

TEST(hcblaswrapper_ddotBatched, func_return_correct_ddotBatched) {
  hcblasStatus_t status;
  hcblasHandle_t *handle = hcblasCreate();
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  double result;
  int batchSize = 32;
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *X = (double*)calloc(lenx * batchSize, sizeof(double));//host input
  double *Y = (double*)calloc(leny * batchSize, sizeof(double));
  double* devX = hc::am_alloc(sizeof(double) * lenx * batchSize, acc[handle->deviceId], 0);
  double* devY = hc::am_alloc(sizeof(double) * leny * batchSize, acc[handle->deviceId], 0);
  double *dotcblastemp =(double*)calloc(batchSize, sizeof(double));
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny * batchSize;i++){
            Y[i] =  rand() % 15;
  }
  hc::am_copy(devX, X, lenx * batchSize * sizeof(double));
  hc::am_copy(devY, Y, leny * batchSize * sizeof(double));
  status = hcblasDdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  double  dotcblas = 0.0;
  for(int i = 0; i < batchSize; i++){
                dotcblastemp[i] = cblas_ddot( n, X + i * n, incx, Y + i * n, incy);
                dotcblas += dotcblastemp[i];
  }
  EXPECT_EQ(result, dotcblas);

  // HCBLAS_STATUS_NOT_INITIALIZED
  hcblasDestroy(handle);
  status = hcblasDdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hc::am_free(devX);
  free(Y);
  hc::am_free(devY);
}


