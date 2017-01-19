#include "hip/hip_runtime.h"
#include "gtest/gtest.h"
#include "hipblas.h"

TEST(hipblasCreateTest, return_Check_hipblasCreate) {
 // Case I: Input to the API is null handle
 hipblasHandle_t handle = NULL;
 // Passing a Null handle to the API
 hipblasStatus_t status = hipblasCreate(&handle); 
 // Assert if the handle is still NULL after allocation
 EXPECT_TRUE(handle != NULL);
 // If allocation succeeds we must expect a success status
 if (handle != NULL)
   EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS); 
 else
   EXPECT_EQ(status, HIPBLAS_STATUS_ALLOC_FAILED);
}

TEST(hipblasDestroyTest, return_Check_hipblasDestroy) {
 hipblasHandle_t handle = NULL;
 // Passing a Null handle to the API
 hipblasStatus_t status = hipblasCreate(&handle);
 //hipblasDestroy
 status = hipblasDestroy(handle);
 EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
 // Destory again
#ifdef __HIP_PLATFORM_HCC__
 status = hipblasDestroy(handle);
 EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif
}


TEST(hipblasSetVectorTest, return_Check_hipblasSetVector) {
 int n = 10;
 int incx = 1, incy = 1;
 float *x1 = (float*) calloc(n, sizeof(float));
 double *x2 = (double*) calloc(n, sizeof(double));
 hipblasStatus_t status;
 hipblasHandle_t handle = NULL;
 status= hipblasCreate(&handle);
 float *y1 = NULL;
 double *y2 = NULL;
 hipError_t err = hipMalloc(&y1, n*sizeof(float));
 err = hipMalloc(&y2, n*sizeof(double));

 // HIPBLAS_STATUS_SUCCESS
 // float type memory transfer from host to device
 status = hipblasSetVector(n, sizeof(float), x1 , incx, y1, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
 // double type memory transfer from host to device
 status = hipblasSetVector(n, sizeof(double), x2 , incx, y2, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
 
 // HIPBLAS_STATUS_INVALID_VALUE 
 // incx is 0
 status = hipblasSetVector(n, sizeof(float), x1 , 0, y1, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);
 // incy is 0
 status = hipblasSetVector(n, sizeof(float), x1 , incx, y1, 0);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);
 // elemSize is 0
 status = hipblasSetVector(n, 0, x1 , incx, y1, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);

 // HIPBLAS_STATUS_MAPPING_ERROR
 status = hipblasSetVector(n, sizeof(double), x1 , incx, y1, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_MAPPING_ERROR);
 
 hipblasDestroy(handle);

 // HIPBLAS_STATUS_NOT_INITIALIZED  
#ifdef __HIP_PLATFORM_HCC__
 status = hipblasSetVector(n, sizeof(float), x1 , incx, y1, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif
 free(x1);
 free(x2);
 hipFree(y1);
 hipFree(y2);
}


TEST(hipblasGetVectorTest, return_Check_hipblasGetVector) {
 int n = 10;
 int incx = 1, incy = 1;
 float *y1 = (float*) calloc(n, sizeof(float));
 double *y2 = (double*) calloc(n, sizeof(double));
 hipblasStatus_t status;
 hipblasHandle_t handle = NULL;
 status= hipblasCreate(&handle);
 float *x1 = NULL;
 double *x2 = NULL;
 hipError_t err = hipMalloc(&x1, n * sizeof(float));
 err = hipMalloc(&x2, n * sizeof(double));

 // HIPBLAS_STATUS_SUCCESS
 // float type memory transfer from device to host
 status = hipblasGetVector(n, sizeof(float), x1 , incx, y1, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
 // double type memory transfer from device to host
 status = hipblasGetVector(n, sizeof(double), x2 , incx, y2, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

 // HIPBLAS_STATUS_INVALID_VALUE
 // incx is 0
 status = hipblasGetVector(n, sizeof(float), x1 , 0, y1, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);
 // incy is 0
 status = hipblasGetVector(n, sizeof(float), x1 , incx, y1, 0);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);
 // elemSize is 0
 status = hipblasGetVector(n, 0, x1 , incx, y1, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);

  // HIPBLAS_STATUS_MAPPING_ERROR
 status = hipblasGetVector(n, sizeof(double), x1 , incx, y1, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_MAPPING_ERROR);

 hipblasDestroy(handle);

 // HIPBLAS_STATUS_NOT_INITIALIZED
#ifdef __HIP_PLATFORM_HCC__
 status = hipblasGetVector(n, sizeof(float), x1 , incx, y1, incy);
 EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

 free(y1);
 free(y2);
 hipFree(x1);
 hipFree(x2);
}


TEST(hipblasSetMatrixTest, return_Check_hipblasSetMatrix) {
 int rows = 10;
 int cols = 10;
 int lda = 10, ldb = 10;
 float *x1 = (float*) calloc(rows * cols, sizeof(float));
 double *x2 = (double*) calloc(rows * cols, sizeof(double));
 hipblasStatus_t status;
 hipblasHandle_t handle = NULL;
 status= hipblasCreate(&handle);
 float *y1 = NULL;
 double *y2 = NULL;
 hipError_t err = hipMalloc(&y1, rows * cols * sizeof(float));
 err = hipMalloc(&y2, rows * cols * sizeof(double));
 
 // HIPBLAS_STATUS_SUCCESS
 // float type memory transfer from host to device
 status = hipblasSetMatrix(rows, cols, sizeof(float), x1 , lda, y1, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
 // double type memory transfer from host to device
 status = hipblasSetMatrix(rows, cols, sizeof(double), x2 , lda, y2, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

 // HIPBLAS_STATUS_INVALID_VALUE 
 // lda is 0
 status = hipblasSetMatrix(rows, cols, sizeof(float), x1 , 0, y1, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);
 // ldb is 0
 status = hipblasSetMatrix(rows, cols, sizeof(float), x1 , lda, y1, 0);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);
 // elemSize is 0
 status = hipblasSetMatrix(rows, cols, 0, x1 , lda, y1, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);

 // HIPBLAS_STATUS_MAPPING_ERROR
 status = hipblasSetMatrix(rows, cols, sizeof(double), x1 , lda, y1, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_MAPPING_ERROR);
 
 hipblasDestroy(handle);

#ifdef __HIP_PLATFORM_HCC__
 // HIPBLAS_STATUS_NOT_INITIALIZED  
 status = hipblasSetMatrix(rows, cols, sizeof(float), x1 , lda, y1, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

 free(x1);
 free(x2);
 hipFree(y1);
 hipFree(y2);
}

TEST(hipblasGetMatrixTest, return_Check_hipblasGetMatrix) {
 int rows = 10;
 int cols = 10;
 int lda = 10, ldb = 10;
 float *y1 = (float*) calloc(cols * rows, sizeof(float));
 double *y2 = (double*) calloc(cols * rows, sizeof(double));
 hipblasStatus_t status;
 hipblasHandle_t handle = NULL;
 status= hipblasCreate(&handle);
 float *x1 = NULL;
 double *x2 = NULL;
 hipError_t err = hipMalloc(&x1, rows * cols * sizeof(float));
 err = hipMalloc(&x2, rows * cols * sizeof(double));

 // HIPBLAS_STATUS_SUCCESS
 // float type memory transfer from device to host
 status = hipblasGetMatrix(rows, cols, sizeof(float), x1 , lda, y1, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);
 // double type memory transfer from device to host
 status = hipblasGetMatrix(rows, cols, sizeof(double), x2 , lda, y2, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_SUCCESS);

 // HIPBLAS_STATUS_INVALID_VALUE
 // lda is 0
 status = hipblasGetMatrix(rows, cols, sizeof(float), x1 , 0, y1, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);
 // ldb is 0
 status = hipblasGetMatrix(rows, cols, sizeof(float), x1 , lda, y1, 0);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);
 // elemSize is 0
 status = hipblasGetMatrix(rows, cols, 0, x1 , lda, y1, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_INVALID_VALUE);

 // HIPBLAS_STATUS_MAPPING_ERROR
 status = hipblasGetMatrix(rows, cols, sizeof(double), x1 , lda, y1, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_MAPPING_ERROR);

 hipblasDestroy(handle);

 // HIPBLAS_STATUS_NOT_INITIALIZED
#ifdef __HIP_PLATFORM_HCC__
 status = hipblasGetMatrix(rows, cols, sizeof(float), x1 , lda, y1, ldb);
 EXPECT_EQ(status, HIPBLAS_STATUS_NOT_INITIALIZED);
#endif

 free(y1);
 free(y2);
 hipFree(x1);
 hipFree(x2);
}
