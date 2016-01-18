#include "hcblaslib.h"
#include <cstdlib> 
#include "gtest/gtest.h"

TEST(hcblas_sdot, func_correct_sdot_Implementation_type_1) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    int incY = 1;
    long yOffset = 0;
    long xOffset = 0;
    float dothcblas;
    hcblasStatus status; 
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Y = (float*)calloc(leny, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
/* Implementation type I - Inputs and Outputs are HCC float array containers */
   hc::array<float> xView(lenx, X);
   std::vector<float> HostX(lenx);
   hc::array<float> yView(leny, Y);
   std::vector<float> HostY(leny);
   for(int i = 0; i < lenx; i++){
            HostX[i] = rand() % 10;
   }
   for(int i = 0; i < leny; i++){
            HostY[i] =  rand() % 15;
   }
   hc::copy(begin(HostX), end(HostX), xView);
   hc::copy(begin(HostY), end(HostY), yView);
   /* Proper call */
   status = hc.hcblas_sdot(accl_view, N, xView, incX, xOffset, yView, incY, yOffset, dothcblas);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* N is 0 */
   N = 0;
   status = hc.hcblas_sdot(accl_view, N, xView, incX, xOffset, yView, incY, yOffset, dothcblas);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status = hc.hcblas_sdot(accl_view, N, xView, incX, xOffset, yView, incY, yOffset, dothcblas);
   EXPECT_EQ(status, HCBLAS_INVALID); 
   /* incY is 0 */
   incX = 1; incY = 0;
   status = hc.hcblas_sdot(accl_view, N, xView, incX, xOffset, yView, incY, yOffset, dothcblas);
   EXPECT_EQ(status, HCBLAS_INVALID); 
}

TEST(hcblas_sdot, func_correct_sdot_Implementation_type_2) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    int incY = 1;
    long yOffset = 0;
    int batchSize = 128;
    long xOffset = 0;
    float dothcblas;
    hcblasStatus status;
    long X_batchOffset = N; 
    long Y_batchOffset = N;
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    float *Ybatch = (float*)calloc(leny * batchSize, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    
/* Implementation type II - Inputs and Outputs are HCC float array containers with batch processing */
   hc::array<float> xbatchView(lenx * batchSize, Xbatch);
   std::vector<float> HostX_batch(lenx * batchSize); 
   hc::array<float> ybatchView(leny * batchSize, Ybatch);
   std::vector<float> HostY_batch(leny * batchSize);
   for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
   }
   for(int i = 0;i < leny * batchSize;i++){
            HostY_batch[i] =  rand() % 15;
   }
   hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
   hc::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
   /* Proper call */
   status= hc.hcblas_sdot(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, dothcblas, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* N is 0 */
   N = 0;
   status= hc.hcblas_sdot(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, dothcblas, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status= hc.hcblas_sdot(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, dothcblas, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incY is 0 */
   incX = 1; incY = 0;
   status= hc.hcblas_sdot(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, dothcblas, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
}
