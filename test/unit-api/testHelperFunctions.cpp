#include "hcblas.h"
#include "gtest/gtest.h"
#include <hc_am.hpp>

TEST(hcblasCreateTest, return_Check_hcblasCreate) {
 // Proper call
 hcblasHandle_t handle;
 // Call hcblasCreate
 hcblasStatus_t status = hcblasCreate(&handle); 
 // Check the status
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
}

TEST(hcblasSetVectorTest, return_Check_hcblasSetVector) {
 int n = 10;
 int incx = 1, incy = 1;
 float *x1 = (float*) calloc(n, sizeof(float));
 double *x2 = (double*) calloc(n, sizeof(double));
 hcblasStatus_t status;
 hcblasHandle_t handle;
 // Call hcblasCreate
 status = hcblasCreate(&handle);
 std::vector<hc::accelerator>accs = hc::accelerator::get_all();
 float *y1 = (float*)am_alloc(n, accs[1], 0);
 double *y2 = (double*)am_alloc(n, accs[1], 0);
 // HCBLAS_STATUS_SUCCESS
 // float type memory transfer from host to device
 status = hcblasSetVector(n, sizeof(x1), x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
 // double type memory transfer from host to device
 status = hcblasSetVector(n, sizeof(x2), x2 , incx, y2, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
 
 // HCBLAS_STATUS_INVALID_VALUE 
 // incx is 0
 status = hcblasSetVector(n, sizeof(x1), x1 , 0, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // incy is 0
 status = hcblasSetVector(n, sizeof(x1), x1 , incx, y1, 0);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // elemSize is 0
 status = hcblasSetVector(n, 0, x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);

/* 
// HCBLAS_STATUS_MAPPING_ERROR
 float *y3 = (float*)am_alloc(n, accs[0], 0);//device not found
 status = hcblasSetVector(n, sizeof(x1), x1 , incx, y3, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_MAPPING_ERROR);

 // HCBLAS_STATUS_NOT_INITIALIZED  
 hcblasDestroy(handle);
 status = hcblasSetVector(n, sizeof(x1), x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);
*/
}

TEST(hcblasGetVectorTest, return_Check_hcblasGetVector) {
 int n = 10;
 int incx = 1, incy = 1;
 float *y1 = (float*) calloc(n, sizeof(float));
 double *y2 = (double*) calloc(n, sizeof(double));
 hcblasStatus_t status;
 hcblasHandle_t handle;
 // Call hcblasCreate
 status = hcblasCreate(&handle);
 std::vector<hc::accelerator>accs = hc::accelerator::get_all();
 float *x1 = (float*)am_alloc(n, accs[1], 0);
 double *x2 = (double*)am_alloc(n, accs[1], 0);
 // HCBLAS_STATUS_SUCCESS
 // float type memory transfer from host to device
 status = hcblasSetVector(n, sizeof(y1), x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
 // double type memory transfer from host to device
 status = hcblasSetVector(n, sizeof(y2), x2 , incx, y2, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

 // HCBLAS_STATUS_INVALID_VALUE
 // incx is 0
 status = hcblasSetVector(n, sizeof(y1), x1 , 0, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // incy is 0
 status = hcblasSetVector(n, sizeof(y1), x1 , incx, y1, 0);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // elemSize is 0
 status = hcblasSetVector(n, 0, x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
/*
 // HCBLAS_STATUS_MAPPING_ERROR
 float *x3 = (float*)am_alloc(n, accs[0], 0);//device not found
 status = hcblasSetVector(n, sizeof(y1), x3 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_MAPPING_ERROR);

 // HCBLAS_STATUS_NOT_INITIALIZED
 hcblasDestroy(handle);
 status = hcblasSetVector(n, sizeof(y1), x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);
*/
}
