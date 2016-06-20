#include "hcblaslib.h"
#include <cstdlib> 
#include "gtest/gtest.h"
#include "hc_am.hpp"
#include "cblas.h"

TEST(hcblas_dasum, return_correct_dasum_Implementation_type_1) {
   Hcblaslibrary hc;
   int N = 119;
   int incX = 1;
   long xOffset = 0;
   double asumhcblas;
   hcblasStatus status; 
   long lenx = 1 + (N-1) * abs(incX);
   double *X = (double*)calloc(lenx, sizeof(double));
   std::vector<hc::accelerator>acc = hc::accelerator::get_all();
   accelerator_view accl_view = (acc[1].get_default_view());
/* Implementation type I - Inputs and Outputs are HCC device pointers */
   double* devX = hc::am_alloc(sizeof(double) * lenx, acc[1], 0);
   for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
   }
   hc::am_copy(devX, X, lenx * sizeof(double));
   /* Proper call */
   status = hc.hcblas_dasum(accl_view, N, devX, incX, xOffset, &asumhcblas);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* X not properly allocated */
   double *devX1 = NULL;
   status = hc.hcblas_dasum(accl_view, N, devX1, incX, xOffset, &asumhcblas);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* N is 0 */
   N = 0;
   status = hc.hcblas_dasum(accl_view, N, devX, incX, xOffset, &asumhcblas);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status = hc.hcblas_dasum(accl_view, N, devX, incX, xOffset, &asumhcblas);
   EXPECT_EQ(status, HCBLAS_INVALID);  
   free(X);
   hc::am_free(devX);
}

TEST(hcblas_dasum, func_correct_dasum_Implementation_type_1) {
   Hcblaslibrary hc;
   int N = 119;
   int incX = 1;
   long xOffset = 0;
   double asumhcblas;
   double asumcblas = 0.0;
   hcblasStatus status;
   long lenx = 1 + (N-1) * abs(incX);
   double *X = (double*)calloc(lenx, sizeof(double));
   std::vector<hc::accelerator>acc = hc::accelerator::get_all();
   accelerator_view accl_view = (acc[1].get_default_view());
/* Implementation type I - Inputs and Outputs are HCC device pointers */
   double* devX = hc::am_alloc(sizeof(double) * lenx, acc[1], 0);
   for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
   }
   hc::am_copy(devX, X, lenx * sizeof(double));
   /* Proper call */
   status = hc.hcblas_dasum(accl_view, N, devX, incX, xOffset, &asumhcblas);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   asumcblas = cblas_dasum( N, X, incX);
   EXPECT_EQ(asumhcblas, asumcblas);
   free(X);
   hc::am_free(devX);
}

TEST(hcblas_dasum, return_correct_dasum_Implementation_type_2) {
   Hcblaslibrary hc;
   int N = 119;
   int incX = 1;
   int batchSize = 128;
   long xOffset = 0;
   double asumhcblas;
   hcblasStatus status;
   long X_batchOffset = N; 
   long lenx = 1 + (N-1) * abs(incX);
   double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
   std::vector<hc::accelerator>acc = hc::accelerator::get_all();
   accelerator_view accl_view = (acc[1].get_default_view());
   double* devXbatch = hc::am_alloc(sizeof(double) * lenx * batchSize, acc[1], 0); 
/* Implementation type II - Inputs and Outputs are HCC device pointers with batch processing */
   for(int i = 0;i < lenx * batchSize;i++){
            Xbatch[i] = rand() % 10;
   }
   hc::am_copy(devXbatch, Xbatch, lenx * batchSize * sizeof(double));
   /* Proper call */
   status= hc.hcblas_dasum(accl_view, N, devXbatch, incX, xOffset, &asumhcblas, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* X is not properly allocated */
   double *devX1 = NULL;
   status= hc.hcblas_dasum(accl_view, N, devX1, incX, xOffset, &asumhcblas, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* N is 0 */
   N = 0;
   status= hc.hcblas_dasum(accl_view, N, devXbatch, incX, xOffset, &asumhcblas, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status= hc.hcblas_dasum(accl_view, N, devXbatch, incX, xOffset, &asumhcblas, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   free(Xbatch);
   hc::am_free(devXbatch);
}

TEST(hcblas_dasum, func_correct_dasum_Implementation_type_2) {
   Hcblaslibrary hc;
   int N = 119;
   int incX = 1;
   int batchSize = 128;
   long xOffset = 0;
   double asumhcblas;
   double asumcblas = 0.0;
   double *asumcblastemp = (double*)calloc(batchSize, sizeof(double));
   hcblasStatus status;
   long X_batchOffset = N;
   long lenx = 1 + (N-1) * abs(incX);
   double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
   std::vector<hc::accelerator>acc = hc::accelerator::get_all();
   accelerator_view accl_view = (acc[1].get_default_view());
   double* devXbatch = hc::am_alloc(sizeof(double) * lenx * batchSize, acc[1], 0);
/* Implementation type II - Inputs and Outputs are HCC double array containers with batch processing */
   for(int i = 0;i < lenx * batchSize;i++){
            Xbatch[i] = rand() % 10;
   }
   hc::am_copy(devXbatch, Xbatch, lenx * batchSize * sizeof(double));
   /* Proper call */
   status= hc.hcblas_dasum(accl_view, N, devXbatch, incX, xOffset, &asumhcblas, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   for(int i = 0; i < batchSize; i++) {
                asumcblastemp[i] = cblas_dasum( N, Xbatch + i * N, incX);
                asumcblas += asumcblastemp[i];
   }
   EXPECT_EQ(asumhcblas, asumcblas);
   free(Xbatch);
   hc::am_free(devXbatch);
} 

