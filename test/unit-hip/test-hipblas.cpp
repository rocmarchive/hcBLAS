#include "hip/hip_runtime.h"
#include "hipblas.h"
#include "gtest/gtest.h"
#include "cblas.h"


void cblas_hgemm( int, int, int, __half* , __half* , __half* , __half, __half );

void cblas_hgemm( int M, int N, int K, __half* A, __half* B, __half* C_cblas, __half alpha , __half beta)
{
  for( int i = 0 ; i < M ; i++)
  {
     for( int j = 0 ; j < N ; j++)
     {
         for( int k = 0 ; k < K ; k++)
         {
            C_cblas[j * M + i] = (  alpha * A[k * M + i] * B[j * K + k] ) + beta * C_cblas[j * M + i]  ;//+ ( C_cblas[j * M + i] ); //column-major
         }
     }
   }
}

TEST(hipblaswrapper_sasum, func_return_correct_sasum) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float* result;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float* devX =NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }

  status = hipblasSetVector(lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  status = hipblasSasum(handle, n, devX, incx, result);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  float asumcblas = 0.0;
  asumcblas = cblas_sasum( n, X, incx);
  EXPECT_EQ(*result, asumcblas);

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasSasum(handle, n, devX, incx, result);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED); 
#endif 

  free(X);
  hipFree(devX);
}

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_sasumBatched, func_return_correct_sasumBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float result;
  int batchSize = 128;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float* devX =NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx * batchSize);
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
  }
  status = hipblasSetVector(lenx*batchSize, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  float asumcblas = 0.0;
  float *asumcblastemp = (float*)calloc(batchSize, sizeof(float));
  for(int i = 0; i < batchSize; i++) {
                asumcblastemp[i] = cblas_sasum( n, X + i * n, incx);
                asumcblas += asumcblastemp[i];
  }
  EXPECT_EQ(result, asumcblas);

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasSasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED); 

  free(X);
  hipFree(devX);
}
#endif

TEST(hipblaswrapper_dasum, func_return_correct_dasum) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double result;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  double *X = (double*)calloc(lenx, sizeof(double));//host input
  double* devX =NULL;
  hipError_t err= hipMalloc(&devX, sizeof(double) * lenx);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
   }
  status = hipblasSetVector(lenx, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasDasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  double asumcblas = 0.0;
  asumcblas = cblas_dasum( n, X, incx);
  EXPECT_EQ(result, asumcblas);

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasDasum(handle, n, devX, incx, &result);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

  free(X);
  hipFree(devX);
}

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_dasumBatched, func_return_correct_dasumBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 23;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double result;
  int batchSize = 128;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  double *X = (double*)calloc(lenx * batchSize, sizeof(double));//host input
  double* devX =NULL;
  hipError_t err= hipMalloc(&devX, sizeof(double) * lenx * batchSize);
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
   }
  status = hipblasSetVector(lenx*batchSize, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasDasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  double asumcblas = 0.0;
  double *asumcblastemp = (double*)calloc(batchSize, sizeof(double));
  for(int i = 0; i < batchSize; i++) {
                asumcblastemp[i] = cblas_dasum( n, X + i * n, incx);
                asumcblas += asumcblastemp[i];
  }
  EXPECT_EQ(result, asumcblas);


  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasDasumBatched(handle, n, devX, incx, &result, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hipFree(devX);
}
#endif

TEST(hipblaswrapper_sscal, func_return_correct_sscal) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float alpha = 1;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *Xcblas = (float*)calloc(lenx, sizeof(float));
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float* devX =NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
            Xcblas[i] = X[i];
  }
  status = hipblasSetVector(lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSscal(handle, n, &alpha, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasGetVector(lenx, sizeof(float), devX, incx, X, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  cblas_sscal( n, alpha, Xcblas, incx );
  for(int i = 0; i < lenx ; i++){
        EXPECT_EQ(X[i], Xcblas[i]);
  }

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasSscal(handle, n, &alpha, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED); 
#endif

  free(X);
  free(Xcblas);
  hipFree(devX);
}

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_sscalBatched, func_return_correct_sscalBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float alpha = 1;
  int batchSize = 128;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *Xcblas = (float*)calloc(lenx * batchSize , sizeof(float));
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float* devX =NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx * batchSize);
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
            Xcblas[i] =  X[i];
  }
  status = hipblasSetVector(lenx*batchSize, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSscalBatched(handle, n, &alpha, devX, incx, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasGetVector(lenx*batchSize, sizeof(float), devX, incx, X, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  for(int i = 0; i < batchSize; i++)
          cblas_sscal( n, alpha, Xcblas + i * n, incx);
  for(int i =0; i < lenx * batchSize; i ++){
          EXPECT_EQ(X[i], Xcblas[i]);
  }

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasSscalBatched(handle, n, &alpha, devX, incx, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hipFree(devX);
}
#endif 

TEST(hipblaswrapper_dscal, func_return_correct_dscal) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double alpha = 1;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  double *Xcblas = (double*)calloc(lenx, sizeof(double));
  double *X = (double*)calloc(lenx, sizeof(double));//host input
  double* devX =NULL;
  hipError_t err= hipMalloc(&devX, sizeof(double) * lenx);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
            Xcblas[i] = X[i];
  }
  status = hipblasSetVector(lenx, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasDscal(handle, n, &alpha, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasGetVector(lenx, sizeof(double), devX, incx, X, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  cblas_dscal( n, alpha, Xcblas, incx );
  for(int i = 0; i < lenx ; i++){
        EXPECT_EQ(X[i], Xcblas[i]);
  }

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasDscal(handle, n, &alpha, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif 

  free(X);
  free(Xcblas);
  hipFree(devX);
}

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_dscalBatched, func_return_correct_dscalBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double alpha = 1;
  int batchSize = 128;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  double *Xcblas = (double*)calloc(lenx * batchSize , sizeof(double));
  double *X = (double*)calloc(lenx * batchSize, sizeof(double));//host input
  double* devX =NULL;
  hipError_t err= hipMalloc(&devX, sizeof(double) * lenx * batchSize);
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
            Xcblas[i] =  X[i];
  }
  status = hipblasSetVector(lenx*batchSize, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasDscalBatched(handle, n, &alpha, devX, incx, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasGetVector(lenx*batchSize, sizeof(double), devX, incx, X, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  for(int i = 0; i < batchSize; i++)
          cblas_dscal( n, alpha, Xcblas + i * n, incx);
  for(int i =0; i < lenx * batchSize; i ++){
          EXPECT_EQ(X[i], Xcblas[i]);
  }

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasDscalBatched(handle, n, &alpha, devX, incx, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hipFree(devX);
}
#endif 

TEST(hipblaswrapper_cscal, func_return_correct_cscal) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  // Passing a Null handle and default accelerator to the API
  status = hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float alpha[2];
  hipComplex cAlpha;

  cAlpha.x = 1;
  cAlpha.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;

  hipComplex *devX = NULL;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float*)calloc(lenx * 2, sizeof(float));
  hipComplex *X = (hipComplex*)calloc(lenx, sizeof(hipComplex));//host input
  hipError_t err = hipMalloc(&devX, sizeof(hipComplex) * lenx);
  int k = 0;
  for(int i = 0; i < lenx; i++){
            X[i].x = rand() % 10;
            X[i].y = rand() % 20;
            Xcblas[k++] = X[i].x;
            Xcblas[k++] = X[i].y;
  }
  status = hipblasSetVector(lenx, sizeof(hipComplex), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasCscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasGetVector(lenx, sizeof(hipComplex), devX, incx, X, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  cblas_cscal( n, alpha, Xcblas, incx );
  for(int i = 0, k = 0; i < lenx && k < lenx * 2 ; i++, k = k + 2){
        EXPECT_EQ(X[i].x, Xcblas[k]);
        EXPECT_EQ(X[i].y, Xcblas[k+1]);
  }

  // HCBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasCscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hipFree(devX);
}

TEST(hipblaswrapper_cscalBatched, func_return_correct_cscalBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  // Passing a Null handle and default accelerator to the API
  status = hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  float alpha[2];
  int batchSize = 128;
  hipComplex cAlpha;

  cAlpha.x = 1;
  cAlpha.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;

  hipComplex *devX = NULL;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  float *Xcblas = (float*)calloc(lenx * batchSize * 2 , sizeof(float));
  hipComplex *X = (hipComplex*)calloc(lenx * batchSize, sizeof(hipComplex));//host input
  hipError_t err = hipMalloc(&devX, sizeof(hipComplex) * lenx * batchSize);
  int k = 0;
  for(int i = 0; i < lenx * batchSize; i++){
            X[i].x = rand() % 10;
            X[i].y = rand() % 20;
            Xcblas[k++] =  X[i].x;
            Xcblas[k++] =  X[i].y;
  }
  status = hipblasSetVector(lenx*batchSize, sizeof(hipComplex), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasCscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasGetVector(lenx*batchSize, sizeof(hipComplex), devX, incx, X, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  for(int i = 0; i < batchSize; i++)
          cblas_cscal( n, alpha, Xcblas + i * n * 2, incx);
  for(int i =0, k = 0; i < lenx * batchSize && k < lenx * batchSize * 2; i++, k = k + 2){
          EXPECT_EQ(X[i].x, Xcblas[k]);
          EXPECT_EQ(X[i].y, Xcblas[k+1]);
  }

  // HCBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasCscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hipFree(devX);
}

TEST(hipblaswrapper_zscal, func_return_correct_zscal) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  // Passing a Null handle and default accelerator to the API
  status = hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double alpha[2];
  hipDoubleComplex cAlpha;

  cAlpha.x = 1;
  cAlpha.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;

  hipDoubleComplex *devX = NULL;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double*)calloc(lenx * 2, sizeof(double));
  hipDoubleComplex *X = (hipDoubleComplex*)calloc(lenx, sizeof(hipDoubleComplex));//host input
  hipError_t err = hipMalloc(&devX, sizeof(hipDoubleComplex) * lenx);
  int k = 0;
  for(int i = 0; i < lenx; i++){
            X[i].x = rand() % 10;
            X[i].y = rand() % 20;
            Xcblas[k++] = X[i].x;
            Xcblas[k++] = X[i].y;
  }
  status = hipblasSetVector(lenx, sizeof(hipDoubleComplex), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasZscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasGetVector(lenx, sizeof(hipDoubleComplex), devX, incx, X, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  cblas_zscal( n, alpha, Xcblas, incx );
  for(int i = 0, k = 0; i < lenx && k < lenx * 2 ; i++, k = k + 2){
        EXPECT_EQ(X[i].x, Xcblas[k]);
        EXPECT_EQ(X[i].y, Xcblas[k+1]);
  }

  // HCBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasZscal(handle, n, &cAlpha, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hipFree(devX);
}

TEST(hipblaswrapper_zscalBatched, func_return_correct_zscalBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  // Passing a Null handle and default accelerator to the API
  status = hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  long lenx = 1 + (n-1) * abs(incx);
  double alpha[2];
  int batchSize = 128;
  hipDoubleComplex cAlpha;

  cAlpha.x = 1;
  cAlpha.y = 1;
  alpha[0] = cAlpha.x;
  alpha[1] = cAlpha.y;

  hipDoubleComplex *devX = NULL;

  // HCBLAS_STATUS_SUCCESS and FUNCTIONALITY CHECK
  double *Xcblas = (double*)calloc(lenx * batchSize * 2 , sizeof(double));
  hipDoubleComplex *X = (hipDoubleComplex*)calloc(lenx * batchSize, sizeof(hipDoubleComplex));//host input
  hipError_t err = hipMalloc(&devX, sizeof(hipDoubleComplex) * lenx * batchSize);
  int k = 0;
  for(int i = 0; i < lenx * batchSize; i++){
            X[i].x = rand() % 10;
            X[i].y = rand() % 20;
            Xcblas[k++] =  X[i].x;
            Xcblas[k++] =  X[i].y;
  }
  status = hipblasSetVector(lenx*batchSize, sizeof(hipDoubleComplex), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasZscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasGetVector(lenx*batchSize, sizeof(hipDoubleComplex), devX, incx, X, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  for(int i = 0; i < batchSize; i++)
          cblas_zscal( n, alpha, Xcblas + i * n * 2, incx);
  for(int i =0, k = 0; i < lenx * batchSize && k < lenx * batchSize * 2; i++, k = k + 2){
          EXPECT_EQ(X[i].x, Xcblas[k]);
          EXPECT_EQ(X[i].y, Xcblas[k+1]);
  }

  // HCBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasZscalBatched(handle, n, &cAlpha, devX, incx, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Xcblas);
  hipFree(devX);
}
TEST(hipblaswrapper_scopy, func_return_correct_scopy) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float alpha = 1;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float *Y = (float*)calloc(leny, sizeof(float));
  float *Ycblas = (float*)calloc(leny, sizeof(float));
  float* devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx);
  err = hipMalloc(&devY, sizeof(float) * leny);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  status = hipblasSetVector(lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasScopy(handle, n, devX, incx, devY, incy);
  status = hipblasGetVector(leny, sizeof(float), devY, incy, Y, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  cblas_scopy( n, X, incx, Ycblas, incy );
  for(int i = 0; i < leny; i++){
        EXPECT_EQ(Y[i], Ycblas[i]);
  }

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasScopy(handle, n, devX, incx, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

  free(X);
  free(Ycblas);
  hipFree(devX);
  free(Y);
  hipFree(devY);
}
 
#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_scopyBatched, func_return_correct_scopyBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float alpha = 1;
  int batchSize = 32; 

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float *Y = (float*)calloc(leny * batchSize, sizeof(float));
  float *Ycblas = (float*)calloc(leny *  batchSize, sizeof(float));
  float* devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx * batchSize);
  err = hipMalloc(&devY, sizeof(float) * leny * batchSize);
  for(int i = 0; i < lenx *  batchSize; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny *  batchSize;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  status = hipblasSetVector(lenx * batchSize, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny * batchSize, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasScopyBatched(handle, n, devX, incx, devY, incy, batchSize);
  status = hipblasGetVector(leny * batchSize, sizeof(float), devY, incy, Y, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  for(int i = 0; i < batchSize; i++)
      cblas_scopy( n, X + i * n, incx, Ycblas + i * n, incy );
  for(int i = 0; i < leny * batchSize; i++){
        EXPECT_EQ(Y[i], Ycblas[i]);
  }

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasScopyBatched(handle, n, devX, incx, devY, incy, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Ycblas);
  hipFree(devX);
  free(Y);
  hipFree(devY);
}
#endif

TEST(hipblaswrapper_dcopy, func_return_correct_dcopy) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  double alpha = 1;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  double *X = (double*)calloc(lenx, sizeof(double));//host input
  double *Y = (double*)calloc(leny, sizeof(double));
  double *Ycblas = (double*)calloc(leny, sizeof(double));
  double* devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(double) * lenx);
  err = hipMalloc(&devY, sizeof(double) * leny);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  status = hipblasSetVector(lenx, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny, sizeof(double), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasDcopy(handle, n, devX, incx, devY, incy);
  status = hipblasGetVector(leny, sizeof(double), devY, incy, Y, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  cblas_dcopy( n, X, incx, Ycblas, incy );
  for(int i = 0; i < leny; i++){
        EXPECT_EQ(Y[i], Ycblas[i]);
  }

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasDcopy(handle, n, devX, incx, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

  free(X);
  free(Ycblas);
  hipFree(devX);
  free(Y);
  hipFree(devY);
}

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_dcopyBatched, func_return_correct_dcopyBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  double alpha = 1;
  int batchSize = 32;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  double *X = (double*)calloc(lenx * batchSize, sizeof(double));//host input
  double *Y = (double*)calloc(leny * batchSize, sizeof(double));
  double *Ycblas = (double*)calloc(leny *  batchSize, sizeof(double));
  double* devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(double) * lenx * batchSize);
  err = hipMalloc(&devY, sizeof(double) * leny * batchSize);
  for(int i = 0; i < lenx *  batchSize; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny *  batchSize;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  status = hipblasSetVector(lenx*batchSize, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny*batchSize, sizeof(double), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasDcopyBatched(handle, n, devX, incx, devY, incy, batchSize);
  status = hipblasGetVector(leny*batchSize, sizeof(double), devY, incy, Y, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  for(int i = 0; i < batchSize; i++)
      cblas_dcopy( n, X + i * n, incx, Ycblas + i * n, incy );
  for(int i = 0; i < leny * batchSize; i++){
        EXPECT_EQ(Y[i], Ycblas[i]);
  }

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasDcopyBatched(handle, n, devX, incx, devY, incy, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  free(Ycblas);
  hipFree(devX);
  free(Y);
  hipFree(devY);
}
#endif

TEST(hipblaswrapper_sdot, func_return_correct_sdot) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float result;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float *Y = (float*)calloc(leny, sizeof(float));
  float* devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx);
  err = hipMalloc(&devY, sizeof(float) * leny);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
  }
  status = hipblasSetVector(lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSdot(handle, n, devX, incx, devY, incy, &result);
  float  dotcblas = 0.0;
  dotcblas = cblas_sdot( n, X, incx, Y, incy);
  EXPECT_EQ(result, dotcblas);

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasSdot(handle, n, devX, incx, devY, incy, &result);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

  free(X);
  hipFree(devX);
  free(Y);
  hipFree(devY);
}

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_sdotBatched, func_return_correct_sdotBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float result;
  int batchSize = 32;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float *Y = (float*)calloc(leny * batchSize, sizeof(float));
  float* devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx * batchSize);
  err = hipMalloc(&devY, sizeof(float) * leny * batchSize);
  float *dotcblastemp =(float*)calloc(batchSize, sizeof(float));
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny * batchSize;i++){
            Y[i] =  rand() % 15;
  }
  status = hipblasSetVector(lenx*batchSize, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny*batchSize, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  float  dotcblas = 0.0;
  for(int i = 0; i < batchSize; i++){
                dotcblastemp[i] = cblas_sdot( n, X + i * n, incx, Y + i * n, incy);
                dotcblas += dotcblastemp[i];
  }
  EXPECT_EQ(result, dotcblas);

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasSdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hipFree(devX);
  free(Y);
  hipFree(devY);
}
#endif

TEST(hipblaswrapper_ddot, func_return_correct_ddot) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status= hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  double result;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  double *X = (double*)calloc(lenx, sizeof(double));//host input
  double *Y = (double*)calloc(leny, sizeof(double));
  double* devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(double) * lenx);
  err = hipMalloc(&devY, sizeof(double) * leny);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
  }
  status = hipblasSetVector(lenx, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny, sizeof(double), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasDdot(handle, n, devX, incx, devY, incy, &result);
  double  dotcblas = 0.0;
  dotcblas = cblas_ddot( n, X, incx, Y, incy);
  EXPECT_EQ(result, dotcblas);

  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  // HIPBLAS_STATUS_NOT_INITIALIZED
  status = hipblasDdot(handle, n, devX, incx, devY, incy, &result);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

  free(X);
  hipFree(devX);
  free(Y);
  hipFree(devY);
}

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_ddotBatched, func_return_correct_ddotBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  double result;
  int batchSize = 32;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  double *X = (double*)calloc(lenx * batchSize, sizeof(double));//host input
  double *Y = (double*)calloc(leny * batchSize, sizeof(double));
  double* devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(double) * lenx * batchSize);
  err = hipMalloc(&devY, sizeof(double) * leny * batchSize);
  double *dotcblastemp =(double*)calloc(batchSize, sizeof(double));
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny * batchSize;i++){
            Y[i] =  rand() % 15;
  }
  status = hipblasSetVector(lenx*batchSize, sizeof(double), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny*batchSize, sizeof(double), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasDdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  double  dotcblas = 0.0;
  for(int i = 0; i < batchSize; i++){
                dotcblastemp[i] = cblas_ddot( n, X + i * n, incx, Y + i * n, incy);
                dotcblas += dotcblastemp[i];
  }
  EXPECT_EQ(result, dotcblas);

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasDdotBatched(handle, n, devX, incx, devY, incy, &result, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hipFree(devX);
  free(Y);
  hipFree(devY);
}
#endif

TEST(hipblaswrapper_saxpy, func_return_correct_saxpy) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float alpha = 1;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float *Y = (float*)calloc(leny, sizeof(float));
  float *Ycblas = (float*)calloc(leny, sizeof(float));
  float* devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx);
  err = hipMalloc(&devY, sizeof(float) * leny);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  status = hipblasSetVector(lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSaxpy(handle, n, &alpha, devX, incx, devY, incy);
  status = hipblasGetVector(leny, sizeof(float), devY, 1, Y, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  cblas_saxpy( n, alpha, X, incx, Ycblas, incy );
  for(int i = 0; i < leny ; i++){
     EXPECT_EQ(Y[i], Ycblas[i]);
  }
  
  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasSaxpy(handle, n, &alpha, devX, incx, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

  free(X);
  free(Ycblas);
  hipFree(devX);
  free(Y);
  hipFree(devY);
}

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_saxpyBatched, func_return_correct_saxpyBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int n = 123;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (n-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float alpha = 1;
  int batchSize = 32;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float *Y = (float*)calloc(leny * batchSize, sizeof(float));
  float *Ycblas = (float*)calloc(leny * batchSize, sizeof(float));
  float* devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx * batchSize);
  err = hipMalloc(&devY, sizeof(float) * leny * batchSize);
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny * batchSize;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  status = hipblasSetVector(lenx * batchSize, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny * batchSize, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSaxpyBatched(handle, n, &alpha, devX, incx, devY, incy, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasGetVector(leny * batchSize, sizeof(float), devY, 1, Y, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  for(int i = 0; i < batchSize; i++)
       cblas_saxpy( n, alpha, X + i * n, incx, Ycblas + i * n, incy );
  for(int i =0; i < leny * batchSize; i++){
      EXPECT_EQ(Y[i], Ycblas[i]);
  }

  hipblasDestroy(handle);
  // HIPBLAS_STATUS_NOT_INITIALIZED
  status = hipblasSaxpyBatched(handle, n, &alpha, devX, incx, devY, incy, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hipFree(devX);
  free(Y);
  free(Ycblas);
  hipFree(devY);
}
#endif

TEST(hipblaswrapper_sger, func_return_correct_sger) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int m = 123;
  int n = 78;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (m-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float alpha = 1;
  long lda;
  lda = m;
  CBLAS_ORDER order;
  order = CblasColMajor;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *Acblas = (float *)calloc( lenx * leny , sizeof(float));
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float *Y = (float*)calloc(leny, sizeof(float));
  float *A = (float *)calloc( lenx * leny , sizeof(float));
  float *devA = NULL, * devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx);
  err = hipMalloc(&devY, sizeof(float) * leny);
  err = hipMalloc(&devA, sizeof(float) * lenx * leny);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
  }
  for(int i = 0;i< lenx * leny ;i++) {
            A[i] = rand() % 25;
            Acblas[i] = A[i];
  }
  status = hipblasSetVector(lenx*leny, sizeof(float), A, incx, devA, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(lenx, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSger(handle, m, n, &alpha, devX, incx, devY, incy, devA, lda);
  status = hipblasGetVector(lenx * leny, sizeof(float), devA, 1, A, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  cblas_sger( order, m, n, alpha, X, incx, Y, incy, Acblas, lda);
  for(int i =0; i < lenx * leny ; i++){
      EXPECT_EQ(A[i], Acblas[i]);
  }
  
  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasSger(handle, m, n, &alpha, devX, incx, devY, incy, devA, lda);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

  free(X);
  free(Acblas);
  hipFree(devX);
  free(Y);
  hipFree(devY);
}

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_sgerBatched, func_return_correct_sgerBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int m = 123;
  int n = 67;
  int incx = 1;
  int incy = 1;
  long lenx = 1 + (m-1) * abs(incx);
  long leny = 1 + (n-1) * abs(incy);
  float alpha = 1;
  int batchSize = 32;
  long lda;
  lda = m;
  CBLAS_ORDER order;
  order = CblasColMajor;

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float *Y = (float*)calloc(leny * batchSize, sizeof(float));
  float *Acblas = (float*)calloc(leny * lenx * batchSize, sizeof(float));
  float *A = (float *)calloc( lenx * leny * batchSize, sizeof(float));
  float *devA = NULL, * devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx * batchSize);
  err = hipMalloc(&devY, sizeof(float) * leny * batchSize);
  err = hipMalloc(&devA, sizeof(float) * lenx * leny * batchSize);
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = 1;
  }
  for(int i = 0;i < leny * batchSize;i++){
            Y[i] =  2;
  }
  for(int i = 0;i< lenx * leny * batchSize;i++) {
            A[i] = 3;
            Acblas[i] = A[i];
  }
  status = hipblasSetVector(lenx*leny*batchSize, sizeof(float), A, incx, devA, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(lenx*batchSize, sizeof(float), X, incx, devX, incx);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny*batchSize, sizeof(float), Y, incy, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSgerBatched(handle, m, n, &alpha, devX, incx, devY, incy, devA, lda, batchSize);
  status = hipblasGetVector(lenx * leny * batchSize, sizeof(float), devA, 1, A, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  for(int i = 0; i < batchSize; i++)
      cblas_sger( order, m, n, alpha, X + i * m, incx, Y + i * n, incy, Acblas + i * m * n, lda);
  for(int i =0; i < lenx * leny * batchSize; i++){
      EXPECT_EQ(A[i], Acblas[i]);
  }

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasSgerBatched(handle, m, n, &alpha, devX, incx, devY, incy, devA, lda, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hipFree(devX);
  free(Y);
  free(Acblas);
  hipFree(devY);
  free(A);
  hipFree(devA);
}
#endif

TEST(hipblaswrapper_sgemv, func_return_correct_sgemv) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int m = 123;
  int n = 78;
  int incx = 1;
  int incy = 1;
  long lenx;
  long leny;
  float alpha = 1;
  float beta = 1;
  long lda;
  CBLAS_ORDER order;
  order = CblasColMajor;
  int row, col;
  row = n; col = m; lda = m; 
  hipblasOperation_t trans = HIPBLAS_OP_N;
  CBLAS_TRANSPOSE transa;
  transa = (trans == HIPBLAS_OP_N)? CblasNoTrans : CblasTrans;
  lenx = 1 + (row - 1) * abs(incx);
  leny = 1 + (col - 1) * abs(incy);

  // NoTransA
  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *Ycblas = (float *)calloc( leny , sizeof(float));
  float *X = (float*)calloc(lenx, sizeof(float));//host input
  float *Y = (float*)calloc(leny, sizeof(float));
  float *A = (float *)calloc( lenx * leny , sizeof(float));
  float *devA = NULL, * devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx);
  err = hipMalloc(&devY, sizeof(float) * leny);
  err = hipMalloc(&devA, sizeof(float) * lenx * leny);
  for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  for(int i = 0;i< lenx * leny ;i++) {
            A[i] = rand() % 25;
  }
  status = hipblasSetVector(lenx * leny, sizeof(float), A, 1, devA, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(lenx, sizeof(float), X, 1, devX, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny, sizeof(float), Y, 1, devY, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  status = hipblasSgemv(handle, trans, m, n, &alpha, devA, lda, devX, incx, &beta, devY, incy); 
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  status = hipblasGetVector(leny, sizeof(float), devY, 1, Y, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  lda = m;
  cblas_sgemv( order, transa, m, n, alpha, A, lda , X, incx, beta, Ycblas, incy );
  for(int i =0; i < leny ; i++){
      EXPECT_EQ(Y[i], Ycblas[i]);
  }

  hipblasDestroy(handle);
  // HIPBLAS_STATUS_NOT_INITIALIZED
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasSgemv(handle, trans, m, n, &alpha, devA, lda, devX, incx, &beta, devY, incy);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

  free(X);
  free(Ycblas);
  hipFree(devX);
  free(Y);
  hipFree(devY);
  free(A);
  hipFree(devA);
}


#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_sgemvBatched, func_return_correct_sgemvBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int m = 123;
  int n = 67;
  int incx = 1;
  int incy = 1;
  long lenx;
  long leny;
  float alpha = 1;
  float beta = 1;
  int batchSize = 32;
  long lda;
  CBLAS_ORDER order;
  order = CblasColMajor;
  int row, col;
  row = n; col = m; lda = m;
  hipblasOperation_t trans = HIPBLAS_OP_N;
  CBLAS_TRANSPOSE transa;
  transa = (trans == HIPBLAS_OP_N)? CblasNoTrans : CblasTrans;
  lenx = 1 + (row - 1) * abs(incx);
  leny = 1 + (col - 1) * abs(incy);

  // HIPBLAS_STATUS_SUCCESS and FUNCTIONALITY __HIP_PLATFORM_HCC__
  float *X = (float*)calloc(lenx * batchSize, sizeof(float));//host input
  float *Y = (float*)calloc(leny * batchSize, sizeof(float));
  float *Ycblas = (float*)calloc(leny * batchSize, sizeof(float));
  float *A = (float *)calloc( lenx * leny * batchSize, sizeof(float));
  float *devA = NULL, * devX = NULL, *devY = NULL;
  hipError_t err= hipMalloc(&devX, sizeof(float) * lenx * batchSize);
  err = hipMalloc(&devY, sizeof(float) * leny * batchSize);
  err = hipMalloc(&devA, sizeof(float) * lenx * leny * batchSize);
  for(int i = 0; i < lenx * batchSize; i++){
            X[i] = rand() % 10;
  }
  for(int i = 0;i < leny * batchSize;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
  }
  for(int i = 0;i< lenx * leny * batchSize;i++) {
            A[i] = rand() % 25;
  }

  status = hipblasSetVector(lenx * leny * batchSize, sizeof(float), A, 1, devA, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(lenx * batchSize, sizeof(float), X, 1, devX, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetVector(leny * batchSize, sizeof(float), Y, 1, devY, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  status = hipblasSgemvBatched(handle, trans, m, n, &alpha, devA, lda, devX, incx, &beta, devY, incy, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  status = hipblasGetVector(leny * batchSize, sizeof(float), devY, 1, Y, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  lda = m;
  for(int i =0 ; i < batchSize; i++)
      cblas_sgemv( order, transa, m, n, alpha, A + i * m * n, lda , X + i * row, incx, beta, Ycblas + i * col, incy );
  for(int i =0; i < leny * batchSize; i++){
      EXPECT_EQ(Y[i], Ycblas[i]);
  }

  // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasSgemvBatched(handle, trans, m, n, &alpha, devA, lda, devX, incx, &beta, devY, incy, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(X);
  hipFree(devX);
  free(Y);
  free(Ycblas);
  hipFree(devY);
  free(A);
  hipFree(devA);
}
#endif

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_sgemm, func_return_correct_sgemm) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  float alpha = 1;
  float beta = 1;
  long lda;
  long ldb;
  long ldc;
  CBLAS_ORDER order;
  order = CblasColMajor;
  hipblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  float *A = (float*) calloc(M * K, sizeof(float));
  float *B = (float*) calloc(K * N, sizeof(float));
  float *C = (float*) calloc(M * N, sizeof(float));
  float *C_hipblas = (float*) calloc(M * N, sizeof(float));
  float *C_cblas = (float*) calloc(M * N, sizeof(float));
  float *devA = NULL, * devB = NULL, *devC = NULL;
  hipError_t err= hipMalloc(&devA, sizeof(float) * M * K);
  err = hipMalloc(&devB, sizeof(float) * K * N);
  err = hipMalloc(&devC, sizeof(float) * M * N);
  for(int i = 0; i < M * K; i++) {
              A[i] = 1;
  }
  for(int i = 0; i < K * N;i++) {
              B[i] = 2;
  }
  for(int i = 0; i < M * N;i++) {
              C[i] = 3;
              C_cblas[i] = C[i];
  }
  status = hipblasSetMatrix(M, K, sizeof(float), A, M, devA, K);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetMatrix(K, N, sizeof(float), B, K, devB, N);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetMatrix(M, N, sizeof(float), C, M, devC, N);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  // NoTransA and NoTransB            
  typeA = HIPBLAS_OP_N;
  typeB = HIPBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

    // Column major 
  lda = M; ldb = K ; ldc = M;
  status = hipblasSgemm(handle, typeA, typeB, M, N, K, &alpha, devA, lda, devB, ldb, &beta, devC, ldc);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  status = hipblasGetMatrix(M, N, sizeof(float), devC, M, C_hipblas, N);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  cblas_sgemm( order, Transa, Transb, M, N, K, alpha, A, lda, B, ldb, beta, C_cblas, ldc);
  for(int i = 0 ; i < M * N ; i++)
    EXPECT_EQ(C_hipblas[i], C_cblas[i]);

   // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasSgemm(handle, typeA, typeB, M, N, K, &alpha, devA, lda, devB, ldb, &beta, devC, ldc);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

  free(A);
  free(B);
  free(C);
  hipFree(devA);
  hipFree(devB);
  hipFree(devC);
  free(C_cblas);
  free(C_hipblas);
}
#endif

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_sgemmBatched, func_return_correct_sgemmBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  float alpha = 1;
  float beta = 1;
  long lda;
  long ldb;
  long ldc;
  CBLAS_ORDER order;
  order = CblasColMajor;
  hipblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  int batchSize = 32;
  float *A[batchSize];
  float *B[batchSize];
  float *C[batchSize];
  float *C_hipblas[batchSize];
  float *C_cblas[batchSize];
  // Device pointers stored in host memory
  float *devA[batchSize], *devB[batchSize], *devC[batchSize];
  // Create device double pointer to store device pointers in device memory
  float **d_Aarray, **d_Barray, **d_Carray;
  hipMalloc((void **)&d_Aarray, batchSize * sizeof(float*));
  hipMalloc((void **)&d_Barray, batchSize * sizeof(float*));
  hipMalloc((void **)&d_Carray, batchSize * sizeof(float*));
  const size_t aSize = sizeof(float) * M * K;
  const size_t bSize = sizeof(float) * K * N;
  const size_t cSize = sizeof(float) * M * N;


  // Host and Device Array allocation
  for(int i =0; i < batchSize; i++) {
    A[i] = (float *) malloc(aSize);
    B[i] = (float *) malloc(bSize);
    C[i] = (float *) malloc(cSize);
    C_hipblas[i] = (float *) malloc(cSize);
    C_cblas[i] = (float *) malloc(cSize);
    hipMalloc((void**)&(devA[i]), aSize);
    hipMalloc((void**)&(devB[i]), bSize);
    hipMalloc((void**)&(devC[i]), cSize);
  }

  // Populate the inputs 
  for (int b = 0; b < batchSize; b++) {
    // Populate each subscript of array
    for(int i = 0; i < M * K; i++) {
      A[b][i] = 1;
    }
    status = hipblasSetMatrix(M, K, sizeof(float), A[b], M, devA[b], K);
    EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
    for(int i = 0; i < K * N; i++) {
      B[b][i] = 2;
    }
    status = hipblasSetMatrix(K, N, sizeof(float), B[b], K, devB[b], N);
    EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
    for(int i = 0; i < M * N;i++) 
    {
      C[b][i] = 3;
      C_cblas[b][i] = C[b][i];
    }
    status = hipblasSetMatrix(M, N, sizeof(float), C[b], M, devC[b], N);
    EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  }

  // NoTransA and NoTransB           
  typeA = HIPBLAS_OP_N;
  typeB = HIPBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Copyinng device pointers stored in host memory to device memory
  hipMemcpy(d_Aarray, devA, batchSize * sizeof(float*), hipMemcpyHostToDevice);
  hipMemcpy(d_Barray, devB, batchSize * sizeof(float*), hipMemcpyHostToDevice);
  hipMemcpy(d_Carray, devC, batchSize * sizeof(float*), hipMemcpyHostToDevice);

    // Column major 
  lda = M; ldb = K ; ldc = M;
  status = hipblasSgemmBatched(handle, typeA, typeB, M, N, K, &alpha, const_cast<const float**>(d_Aarray), lda, const_cast<const float**>(d_Barray), ldb, &beta, d_Carray, ldc, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  // Get the results
  for(int b =0; b < batchSize; b++) {
    status = hipblasGetMatrix(M, N, sizeof(float), devC[b], M, C_hipblas[b], N);
    EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  }

  for(int b = 0; b < batchSize; b++)
         cblas_sgemm( order, Transa, Transb, M, N, K, alpha, A[b], lda, B[b], ldb, beta, C_cblas[b],ldc );
  
  for(int b = 0; b < batchSize; b++) {
    for(int i = 0 ; i < M * N; i++) {
      //EXPECT_EQ(C_hipblas[b][i], C_cblas[b][i]);
    }
  }

  hipblasDestroy(handle);
  // HIPBLAS_STATUS_NOT_INITIALIZED
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasSgemmBatched(handle, typeA, typeB, M, N, K, &alpha, const_cast<const float**>(d_Aarray), lda, const_cast<const float**>(d_Barray), ldb, &beta, d_Carray, ldc, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif
  // Free up resources
  for(int b = 0; b < batchSize; b++) {
    hipFree(devA[b]);
    hipFree(devB[b]);
    hipFree(devC[b]);
    free(A[b]);
    free(B[b]);
    free(C[b]);
    free(C_cblas[b]);
    free(C_hipblas[b]);
  }
  hipFree(d_Aarray);
  hipFree(d_Barray);
  hipFree(d_Carray);
}
#endif

#ifdef __HIP_PLATFORM_HCC__
// DgemmBatched test
TEST(hipblaswrapper_dgemmBatched, func_return_correct_dgemmBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  double alpha = 1;
  double beta = 1;
  long lda;
  long ldb;
  long ldc;
  CBLAS_ORDER order;
  order = CblasColMajor;
  hipblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  int batchSize = 32;
  double *A[batchSize];
  double *B[batchSize];
  double *C[batchSize];
  double *C_hipblas[batchSize];
  double *C_cblas[batchSize];
  // Device pointers stored in host memory
  double *devA[batchSize], *devB[batchSize], *devC[batchSize];
  // Create device double pointer to store device pointers in device memory
  double **d_Aarray, **d_Barray, **d_Carray;
  hipMalloc((void **)&d_Aarray, batchSize * sizeof(double*));
  hipMalloc((void **)&d_Barray, batchSize * sizeof(double*));
  hipMalloc((void **)&d_Carray, batchSize * sizeof(double*));
  const size_t aSize = sizeof(double) * M * K;
  const size_t bSize = sizeof(double) * K * N;
  const size_t cSize = sizeof(double) * M * N;


  // Host and Device Array allocation
  for(int i =0; i < batchSize; i++) {
    A[i] = (double *) malloc(aSize);
    B[i] = (double *) malloc(bSize);
    C[i] = (double *) malloc(cSize);
    C_hipblas[i] = (double *) malloc(cSize);
    C_cblas[i] = (double *) malloc(cSize);
    hipMalloc((void**)&(devA[i]), aSize);
    hipMalloc((void**)&(devB[i]), bSize);
    hipMalloc((void**)&(devC[i]), cSize);
  }

  // Populate the inputs 
  for (int b = 0; b < batchSize; b++) {
    // Populate each subscript of array
    for(int i = 0; i < M * K; i++) {
      A[b][i] = rand() % 100;
    }
    status = hipblasSetMatrix(M, K, sizeof(double), A[b], 1, devA[b], 1);
    EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
    for(int i = 0; i < K * N; i++) {
      B[b][i] = rand() % 15;
    }
    status = hipblasSetMatrix(K, N, sizeof(double), B[b], 1, devB[b], 1);
    EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
    for(int i = 0; i < M * N;i++) 
    {
      C[b][i] = rand() % 25;
      C_cblas[b][i] = C[b][i];
    }
    status = hipblasSetMatrix(M, N, sizeof(double), C[b], 1, devC[b], 1);
    EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  }

  // NoTransA and NoTransB            
  typeA = HIPBLAS_OP_N;
  typeB = HIPBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

  // Copyinng device pointers stored in host memory to device memory
  hipMemcpy(d_Aarray, devA, batchSize * sizeof(double*), hipMemcpyHostToDevice);
  hipMemcpy(d_Barray, devB, batchSize * sizeof(double*), hipMemcpyHostToDevice);
  hipMemcpy(d_Carray, devC, batchSize * sizeof(double*), hipMemcpyHostToDevice);

    // Column major 
  lda = M; ldb = K ; ldc = M;
  status = hipblasDgemmBatched(handle, typeA, typeB, M, N, K, &alpha, const_cast<const double**>(d_Aarray), lda, const_cast<const double**>(d_Barray), ldb, &beta, d_Carray, ldc, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  // Get the results
  for(int b =0; b < batchSize; b++) {
    status = hipblasGetMatrix(M, N, sizeof(double), devC[b], 1, C_hipblas[b], 1);
    EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  }

  for(int b = 0; b < batchSize; b++)
         cblas_dgemm( order, Transa, Transb, M, N, K, alpha, A[b], lda, B[b], ldb, beta, C_cblas[b],ldc );
  
  for(int b = 0; b < batchSize; b++) {
    for(int i = 0 ; i < M * N; i++) {
      EXPECT_EQ(C_hipblas[b][i], C_cblas[b][i]);
    }
  }

  // HIPBLAS_STATUS_NOT_INITIALIZED
  //hipblasDestroy(handle);
  //status = hipblasDgemmBatched(handle, typeA, typeB, M, N, K, &alpha, devA, lda, devB, ldb, &beta, devC, ldc, batchSize);
  //EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
  // Free up resources
  for(int b = 0; b < batchSize; b++) {
    hipFree(devA[b]);
    hipFree(devB[b]);
    hipFree(devC[b]);
    free(A[b]);
    free(B[b]);
    free(C[b]);
    free(C_cblas[b]);
    free(C_hipblas[b]);
  }
  hipFree(d_Aarray);
  hipFree(d_Barray);
  hipFree(d_Carray);
}

TEST(hipblaswrapper_cgemm, func_return_correct_cgemm) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  long lda;
  long ldb;
  long ldc;
  CBLAS_ORDER order;
  order = CblasColMajor;
  hipblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
    float alpha[2], beta[2];
    hipComplex cAlpha, cBeta;
    cAlpha.x = 1;
    cAlpha.y = 1;
    cBeta.x = 1;
    cBeta.y = 1;
    alpha[0] = cAlpha.x;
    alpha[1] = cAlpha.y;
    beta[0] = cBeta.x;
    beta[1] = cBeta.y;
    hipComplex *A = (hipComplex*) calloc(M * K, sizeof(hipComplex));
    hipComplex *B = (hipComplex*) calloc(K * N, sizeof(hipComplex));
    hipComplex *C = (hipComplex*) calloc(M * N, sizeof(hipComplex));
  hipComplex *devA = NULL, * devB = NULL, *devC = NULL;
  hipError_t err= hipMalloc(&devA, sizeof(hipComplex) * M * K);
  err = hipMalloc(&devB, sizeof(hipComplex) * K * N);
  err = hipMalloc(&devC, sizeof(hipComplex) * M * N);
    float* ablas = (float *)malloc(sizeof(float )* M * K * 2);
    float* bblas = (float *)malloc(sizeof(float )* K * N * 2);
    float* cblas = (float *)malloc(sizeof(float )* M * N * 2);
    int k = 0;
    for(int i = 0; i < M * K; i++) {
                A[i].x = rand() % 10;
                A[i].y = rand() % 20;
                ablas[k++] = A[i].x;
                ablas[k++] = A[i].y;
    }
    k = 0;
    for(int i = 0; i < K * N;i++) {
                B[i].x = rand() % 15;
                B[i].y = rand() % 25;
                bblas[k++] = B[i].x;
                bblas[k++] = B[i].y;
    }
    k = 0;
    for(int i = 0; i < M * N;i++) {
                C[i].x = rand() % 18;
                C[i].y = rand() % 28;
                cblas[k++] = C[i].x;
                cblas[k++] = C[i].y;
    }

  status = hipblasSetMatrix(M, K, sizeof(hipComplex), A, 1, devA, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetMatrix(K, N, sizeof(hipComplex), B, 1, devB, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetMatrix(M, N, sizeof(hipComplex), C, 1, devC, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  // NoTransA and NoTransB            
  typeA = HIPBLAS_OP_N;
  typeB = HIPBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

    // Column major 
  lda = M; ldb = K ; ldc = M;
  status = hipblasCgemm(handle, typeA, typeB, M, N, K, &cAlpha, devA, lda, devB, ldb, &cBeta, devC, ldc);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  status = hipblasGetMatrix(M, N, sizeof(hipComplex), devC, 1, C, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  cblas_cgemm( order, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
  for(int i = 0, k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
  }

   // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasCgemm(handle, typeA, typeB, M, N, K, &cAlpha, devA, lda, devB, ldb, &cBeta, devC, ldc);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

  free(A);
  free(B);
  free(C);
  hipFree(devA);
  hipFree(devB);
  hipFree(devC);
  free(ablas);
  free(bblas);
  free(cblas);
}
#endif


#ifdef __HIP_PLATFORM_HCC__
//TODO: Change prototype of CgemmBatched
TEST(hipblaswrapper_cgemmBatched, func_return_correct_cgemmBatched) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  long lda;
  long ldb;
  long ldc;
  int batchSize = 64;
  CBLAS_ORDER order;
  order = CblasColMajor;
  hipblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
    float alpha[2], beta[2];
    hipComplex cAlpha, cBeta;
    cAlpha.x = 1;
    cAlpha.y = 1;
    cBeta.x = 1;
    cBeta.y = 1;
    alpha[0] = cAlpha.x;
    alpha[1] = cAlpha.y;
    beta[0] = cBeta.x;
    beta[1] = cBeta.y;
    hipComplex *A = (hipComplex*) calloc(M * K, sizeof(hipComplex));
    hipComplex *B = (hipComplex*) calloc(K * N, sizeof(hipComplex));
    hipComplex *C = (hipComplex*) calloc(M * N * batchSize, sizeof(hipComplex));
  hipComplex *devA = NULL, * devB = NULL, *devC = NULL;
  hipError_t err= hipMalloc(&devA, sizeof(hipComplex) * M * K);
  err = hipMalloc(&devB, sizeof(hipComplex) * K * N);
  err = hipMalloc(&devC, sizeof(hipComplex) * M * N * batchSize);
    float* ablas = (float *)malloc(sizeof(float )* M * K * 2);
    float* bblas = (float *)malloc(sizeof(float )* K * N * 2);
    float* cblas = (float *)malloc(sizeof(float )* M * N * batchSize * 2);
    int k = 0;
    for(int i = 0; i < M * K; i++) {
                A[i].x = rand() % 10;
                A[i].y = rand() % 20;
                ablas[k++] = A[i].x;
                ablas[k++] = A[i].y;
    }
    k = 0;
    for(int i = 0; i < K * N;i++) {
                B[i].x = rand() % 15;
                B[i].y = rand() % 25;
                bblas[k++] = B[i].x;
                bblas[k++] = B[i].y;
    }
    k = 0;
    for(int i = 0; i < M * N * batchSize;i++) {
                C[i].x = rand() % 18;
                C[i].y = rand() % 28;
                cblas[k++] = C[i].x;
                cblas[k++] = C[i].y;
    }

  status = hipblasSetMatrix(M, K, sizeof(hipComplex), A, 1, devA, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetMatrix(K, N, sizeof(hipComplex), B, 1, devB, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetMatrix(M, N * batchSize, sizeof(hipComplex), C, 1, devC, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  // NoTransA and NoTransB            
  typeA = HIPBLAS_OP_N;
  typeB = HIPBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

    // Column major 
  lda = M; ldb = K ; ldc = M;
  status = hipblasCgemmBatched(handle, typeA, typeB, M, N, K, &cAlpha, devA, lda, devB, ldb, &cBeta, devC, ldc, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  status = hipblasGetMatrix(M, N * batchSize, sizeof(hipComplex), devC, 1, C, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  for(int i = 0; i < batchSize;i++)
         cblas_cgemm( order, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas + i * M * N * 2, ldc );
  for(int i = 0, k = 0; ((i < M * N * batchSize) && ( k < M * N * batchSize * 2)) ; i++, k = k + 2) {
      EXPECT_EQ(C[i].x, cblas[k]);
      EXPECT_EQ(C[i].y, cblas[k+1]);
  }

   // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
  status = hipblasCgemmBatched(handle, typeA, typeB, M, N, K, &cAlpha, devA, lda, devB, ldb, &cBeta, devC, ldc, batchSize);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);

  free(A);
  free(B);
  free(C);
  hipFree(devA);
  hipFree(devB);
  hipFree(devC);
  free(ablas);
  free(bblas);
  free(cblas);
}
#endif

#ifdef __HIP_PLATFORM_HCC__
TEST(hipblaswrapper_hgemm, func_return_correct_hgemm) {
  hipblasStatus_t status;
  hipblasHandle_t handle = NULL;
  status = hipblasCreate(&handle);
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  __half alpha = 1;
  __half beta = 1;
  long lda;
  long ldb;
  long ldc;
  CBLAS_ORDER order;
  order = CblasColMajor;
  hipblasOperation_t typeA, typeB;
  CBLAS_TRANSPOSE Transa, Transb;
  __half *A = (__half*) calloc(M * K, sizeof(__half));
  __half *B = (__half*) calloc(K * N, sizeof(__half));
  __half *C = (__half*) calloc(M * N, sizeof(__half));
  __half *C_hipblas = (__half*) calloc(M * N, sizeof(__half));
  __half *C_cblas = (__half*) calloc(M * N, sizeof(__half));
  __half *devA = NULL, * devB = NULL, *devC = NULL;
  hipError_t err= hipMalloc(&devA, sizeof(__half) * M * K);
  err = hipMalloc(&devB, sizeof(__half) * K * N);
  err = hipMalloc(&devC, sizeof(__half) * M * N);
  for(int i = 0; i < M * K; i++) {
              A[i] = 1;
  }
  for(int i = 0; i < K * N;i++) {
              B[i] = 2;
  }
  for(int i = 0; i < M * N;i++) {
              C[i] = 3;
              C_cblas[i] = C[i];
  }
  status = hipblasSetMatrix(M, K, sizeof(__half), A, 1, devA, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetMatrix(K, N, sizeof(__half), B, 1, devB, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
  status = hipblasSetMatrix(M, N, sizeof(__half), C, 1, devC, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  // NoTransA and NoTransB */           
  typeA = HIPBLAS_OP_N;
  typeB = HIPBLAS_OP_N;
  Transa = CblasNoTrans;
  Transb = CblasNoTrans;

    // Column major */
  lda = M; ldb = K ; ldc = M;
  status = hipblasHgemm(handle, typeA, typeB, M, N, K, &alpha, devA, lda, devB, ldb, &beta, devC, ldc);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  status = hipblasGetMatrix(M, N, sizeof(__half), devC, 1, C_hipblas, 1);
  EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

  cblas_hgemm( M, N, K, A, B, C_cblas,alpha,beta);
  for(int i = 0 ; i < M * N ; i++)
    //EXPECT_EQ(C_hipblas[i], C_cblas[i]);

   // HIPBLAS_STATUS_NOT_INITIALIZED
  hipblasDestroy(handle);
#ifdef __HIP_PLATFORM_HCC__
  status = hipblasHgemm(handle, typeA, typeB, M, N, K, &alpha, devA, lda, devB, ldb, &beta, devC, ldc);
  EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

  free(A);
  free(B);
  free(C);
  hipFree(devA);
  hipFree(devB);
  hipFree(devC);
  free(C_cblas);
  free(C_hipblas);
}
#endif
