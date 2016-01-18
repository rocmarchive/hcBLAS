#include "hcblaslib.h"
#include <cstdlib> 
#include "gtest/gtest.h"

TEST(hcblas_sasum, func_correct_sasum_Implementation_type_1) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    long xOffset = 0;
    float asumhcblas;
    hcblasStatus status; 
    long lenx = 1 + (N-1) * abs(incX);
    float *X = (float*)calloc(lenx, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
/* Implementation type I - Inputs and Outputs are HCC float array containers */
   hc::array<float> xView(lenx, X);
   std::vector<float> HostX(lenx);
   for(int i = 0; i < lenx; i++){
            HostX[i] = rand() % 10;
   }
   hc::copy(begin(HostX), end(HostX), xView);
   /* Proper call */
   status = hc.hcblas_sasum(accl_view, N, xView, incX, xOffset, asumhcblas);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* N is 0 */
   N = 0;
   status = hc.hcblas_sasum(accl_view, N, xView, incX, xOffset, asumhcblas);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status = hc.hcblas_sasum(accl_view, N, xView, incX, xOffset, asumhcblas);
   EXPECT_EQ(status, HCBLAS_INVALID);  
}

TEST(hcblas_sasum, func_correct_sasum_Implementation_type_2) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    int batchSize = 128;
    long xOffset = 0;
    float asumhcblas;
    hcblasStatus status;
    long X_batchOffset = N; 
    long lenx = 1 + (N-1) * abs(incX);
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    
/* Implementation type II - Inputs and Outputs are HCC float array containers with batch processing */
   hc::array<float> xbatchView(lenx * batchSize, Xbatch);
   std::vector<float> HostX_batch(lenx * batchSize); 
   for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
   }
   hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
   /* Proper call */
   status= hc.hcblas_sasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* N is 0 */
   N = 0;
   status= hc.hcblas_sasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status= hc.hcblas_sasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
}
