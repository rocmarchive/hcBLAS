#include "hcblaslib.h"
#include <cstdlib> 
#include "gtest/gtest.h"

TEST(hcblas_dcopy, func_correct_dcopy_Implementation_type_1) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    int incY = 1;
    long yOffset = 0;
    long xOffset = 0;
    hcblasStatus status; 
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    double *X = (double*)calloc(lenx, sizeof(double));
    double *Y = (double*)calloc(leny, sizeof(double));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
/* Implementation type I - Inputs and Outputs are HCC double array containers */
   hc::array<double> xView(lenx, X);
   std::vector<double> HostX(lenx);
   hc::array<double> yView(leny, Y);
   std::vector<double> HostY(leny);
   for(int i = 0; i < lenx; i++){
            HostX[i] = rand() % 10;
   }
   for(int i = 0; i < leny; i++){
            HostY[i] =  rand() % 15;
   }
   hc::copy(begin(HostX), end(HostX), xView);
   hc::copy(begin(HostY), end(HostY), yView);
   /* Proper call */
   status = hc.hcblas_dcopy(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* N is 0 */
   N = 0;
   status = hc.hcblas_dcopy(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status = hc.hcblas_dcopy(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
   EXPECT_EQ(status, HCBLAS_INVALID); 
   /* incY is 0 */
   incX = 1; incY = 0;
   status = hc.hcblas_dcopy(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
   EXPECT_EQ(status, HCBLAS_INVALID); 
}

TEST(hcblas_dcopy, func_correct_dcopy_Implementation_type_2) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    int incY = 1;
    long yOffset = 0;
    int batchSize = 128;
    long xOffset = 0;
    hcblasStatus status;
    long X_batchOffset = N; 
    long Y_batchOffset = N;
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
    double *Ybatch = (double*)calloc(leny * batchSize, sizeof(double));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    
/* Implementation type II - Inputs and Outputs are HCC double array containers with batch processing */
   hc::array<double> xbatchView(lenx * batchSize, Xbatch);
   std::vector<double> HostX_batch(lenx * batchSize);
   hc::array<double> ybatchView(leny * batchSize, Ybatch);
   std::vector<double> HostY_batch(leny * batchSize); 
   for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
   }
   for(int i = 0;i < leny * batchSize;i++){
            HostY_batch[i] =  rand() % 15;
   }
   hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
   hc::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
   /* Proper call */
   status= hc.hcblas_dcopy(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* N is 0 */
   N = 0;
   status= hc.hcblas_dcopy(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status= hc.hcblas_dcopy(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incY is 0 */
   incX = 1; incY = 0;
   status= hc.hcblas_dcopy(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
}
