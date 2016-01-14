#include "hcblaslib.h"
#include <cstdlib> 
#include "gtest/gtest.h"

TEST(hcblas_sscal, func_correct_sscal_Implementation_type_1) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    long xOffset = 0;
    float alpha = 1;
    hcblasStatus status;
    long lenx = 1 + (N-1) * abs(incX);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *X1 = NULL;
/* Implementation type I - Inputs and Outputs are host float pointers */
    for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
    }
    /* X1 is NULL */
    status = hc.hcblas_sscal(N, &alpha, X1, incX, xOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* alpha is some scalar */
    status = hc.hcblas_sscal(N, &alpha, X, incX, xOffset);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* alpha is 0*/
    alpha = 0;
    status = hc.hcblas_sscal(N, &alpha, X, incX, xOffset);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);  
    /* N is 0 */
    N = 0;
    status = hc.hcblas_sscal(N, &alpha, X, incX, xOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incX is 0 */
    incX = 0;
    status = hc.hcblas_sscal(N, &alpha, X, incX, xOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
    free(X);
}

TEST(hcblas_sscal, func_correct_sscal_Implementation_type_2) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    long xOffset = 0;
    float alpha = 1;
    hcblasStatus status;
    long lenx = 1 + (N-1) * abs(incX);
    float *X = (float*)calloc(lenx, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type II - Inputs and Outputs are HC++ float array_view containers */
    hc::array_view<float> xView(lenx, X);
    for(int i = 0; i < lenx; i++) {
            xView[i] = rand() % 10;
    }
    /* alpha is some scalar */
    status = hc.hcblas_sscal(accl_view, N, alpha, xView, incX, xOffset);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* alpha is 0 */
    alpha = 0; 
    status = hc.hcblas_sscal(accl_view, N, alpha, xView, incX, xOffset);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* N is 0 */
    N = 0;
    status = hc.hcblas_sscal(accl_view, N, alpha, xView, incX, xOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incX is 0 */
    incX = 0;
    status = hc.hcblas_sscal(accl_view, N, alpha, xView, incX, xOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_sscal, func_correct_sscal_Implementation_type_3) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    int batchSize = 128;
    long xOffset = 0;
    float alpha = 1;
    hcblasStatus status;
    long X_batchOffset = N; 
    long lenx = 1 + (N-1) * abs(incX);
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
/* Implementation type III - Inputs and Outputs are HC++ float array_view containers with batch processing */
    hc::array_view<float> xbatchView(lenx * batchSize, Xbatch);
    for(int i = 0; i < lenx * batchSize; i++){
            xbatchView[i] = rand() % 10;
    }
    /* alpha is some scalar */
    status= hc.hcblas_sscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* alpha is 0 */
    alpha = 0;
    status= hc.hcblas_sscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* N is 0 */
    N = 0;
    status= hc.hcblas_sscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incX is 0 */
    incX = 0;
    status= hc.hcblas_sscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_sscal, func_correct_sscal_Implementation_type_4) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    long xOffset = 0;
    float alpha = 1;
    hcblasStatus status; 
    long lenx = 1 + (N-1) * abs(incX);
    float *X = (float*)calloc(lenx, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
/* Implementation type IV - Inputs and Outputs are HC++ float array containers */
   hc::array<float> xView(lenx, X);
   std::vector<float> HostX(lenx);
   for(int i = 0; i < lenx; i++){
            HostX[i] = rand() % 10;
   }
   hc::copy(begin(HostX), end(HostX), xView);
   /* alpha is some scalar */
   status = hc.hcblas_sscal(accl_view, N, alpha, xView, incX, xOffset);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* alpha is 0 */
   alpha = 0;
   status = hc.hcblas_sscal(accl_view, N, alpha, xView, incX, xOffset);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* N is 0 */
   N = 0;
   status = hc.hcblas_sscal(accl_view, N, alpha, xView, incX, xOffset);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status = hc.hcblas_sscal(accl_view, N, alpha, xView, incX, xOffset);
   EXPECT_EQ(status, HCBLAS_INVALID);
   hc::copy(xView, begin(HostX));  
}

TEST(hcblas_sscal, func_correct_sscal_Implementation_type_5) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    int batchSize = 128;
    long xOffset = 0;
    float alpha = 1;
    hcblasStatus status;
    long X_batchOffset = N; 
    long lenx = 1 + (N-1) * abs(incX);
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    
/* Implementation type V - Inputs and Outputs are HC++ float array containers with batch processing */
   hc::array<float> xbatchView(lenx * batchSize, Xbatch);
   std::vector<float> HostX_batch(lenx * batchSize); 
   for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
   }
   hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
   /* alpha is some scalar */
   status= hc.hcblas_sscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* alpha is 0 */
   alpha = 0;  
   status= hc.hcblas_sscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* N is 0 */
   N = 0;
   status= hc.hcblas_sscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status= hc.hcblas_sscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   hc::copy(xbatchView, begin(HostX_batch));  
}
