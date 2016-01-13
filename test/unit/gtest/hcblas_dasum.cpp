#include "hcblaslib.h"
#include <cstdlib> 
#include "gtest/gtest.h"

TEST(hcblas_dasum, func_correct_dasum_Implementation_type_1) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    long xOffset = 0;
    double asumhcblas;
    hcblasStatus status;
    long lenx = 1 + (N-1) * abs(incX);
    double *X = (double*)calloc(lenx, sizeof(double));
    double *X1 = NULL;
/* Implementation type I - Inputs and Outputs are host double pointers */
    for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
    }
     /* X1 is NULL */
    status = hc.hcblas_dasum(N, X1, incX, xOffset, &asumhcblas);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* Proper call */
    status = hc.hcblas_dasum(N, X, incX, xOffset, &asumhcblas);
    EXPECT_EQ(status, HCBLAS_SUCCESS);  
    /* N is 0 */
    N = 0;
    status = hc.hcblas_dasum(N, X, incX, xOffset, &asumhcblas); 
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incX is 0 */
    incX = 0;
    status = hc.hcblas_dasum(N, X, incX, xOffset, &asumhcblas);
    EXPECT_EQ(status, HCBLAS_INVALID);
    free(X);
}

TEST(hcblas_dasum, func_correct_dasum_Implementation_type_2) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    long xOffset = 0;
    double asumhcblas;
    hcblasStatus status;
    long lenx = 1 + (N-1) * abs(incX);
    double *X = (double*)calloc(lenx, sizeof(double));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type II - Inputs and Outputs are HC++ double array_view containers */
    hc::array_view<double> xView(lenx, X);
    for(int i = 0; i < lenx; i++) {
            xView[i] = rand() % 10;
    }
    /* Proper call */
    status = hc.hcblas_dasum(accl_view, N, xView, incX, xOffset, asumhcblas);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* N is 0 */
    N = 0;
    status = hc.hcblas_dasum(accl_view, N, xView, incX, xOffset, asumhcblas);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incX is 0 */
    incX = 0;
    status = hc.hcblas_dasum(accl_view, N, xView, incX, xOffset, asumhcblas);
    EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_dasum, func_correct_dasum_Implementation_type_3) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    double asumhcblas;
    int batchSize = 128;
    long xOffset = 0;
    hcblasStatus status;
    long X_batchOffset = N; 
    long lenx = 1 + (N-1) * abs(incX);
    double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
/* Implementation type III - Inputs and Outputs are HC++ double array_view containers with batch processing */
    hc::array_view<double> xbatchView(lenx * batchSize, Xbatch);
    for(int i = 0; i < lenx * batchSize; i++){
            xbatchView[i] = rand() % 10;
    }
    /* Proper call */
    status= hc.hcblas_dasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* N is 0 */
    N = 0;
    status= hc.hcblas_dasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incX is 0 */
    incX = 0;
    status= hc.hcblas_dasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_dasum, func_correct_dasum_Implementation_type_4) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    long xOffset = 0;
    double asumhcblas;
    hcblasStatus status; 
    long lenx = 1 + (N-1) * abs(incX);
    double *X = (double*)calloc(lenx, sizeof(double));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
/* Implementation type IV - Inputs and Outputs are HC++ double array containers */
   hc::array<double> xView(lenx, X);
   std::vector<double> HostX(lenx);
   for(int i = 0; i < lenx; i++){
            HostX[i] = rand() % 10;
   }
   hc::copy(begin(HostX), end(HostX), xView);
   /* Proper call */
   status = hc.hcblas_dasum(accl_view, N, xView, incX, xOffset, asumhcblas);
   EXPECT_EQ(status, HCBLAS_SUCCESS);
   /* N is 0 */
   N = 0;
   status = hc.hcblas_dasum(accl_view, N, xView, incX, xOffset, asumhcblas);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status = hc.hcblas_dasum(accl_view, N, xView, incX, xOffset, asumhcblas);
   EXPECT_EQ(status, HCBLAS_INVALID);  
}

TEST(hcblas_dasum, func_correct_dasum_Implementation_type_5) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    int batchSize = 128;
    long xOffset = 0;
    double asumhcblas;
    hcblasStatus status;
    long X_batchOffset = N; 
    long lenx = 1 + (N-1) * abs(incX);
    double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    
/* Implementation type V - Inputs and Outputs are HC++ double array containers with batch processing */
   hc::array<double> xbatchView(lenx * batchSize, Xbatch);
   std::vector<double> HostX_batch(lenx * batchSize); 
   for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
   }
   hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
   /* Proper call */
   status= hc.hcblas_dasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCESS);
   /* N is 0 */
   N = 0;
   status= hc.hcblas_dasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status= hc.hcblas_dasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
}
