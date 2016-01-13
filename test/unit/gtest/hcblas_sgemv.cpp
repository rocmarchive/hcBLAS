#include "hcblas.h"
#include <cstdlib> 
#include <amp_short_vectors.h>
#include <unistd.h>
#include "gtest/gtest.h"

TEST(hcblas_sgemv, func_correct_sgemv_Implementation_type_1) {
    Hcblaslibrary hc;
    int M = 19;
    int N = 19;
    int row, col;
    float alpha = 1;
    float beta = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    long lenx,  leny;
    hcblasStatus status;
    hcblasTranspose typeA;
    
/* Implementation type I - Inputs and Outputs are host float pointers */
/* TransA */
    hcblasOrder hcOrder = ColMajor;
    row = M; col = N; lda = N; typeA = Trans;
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    float *xSgemv = (float*)calloc( lenx , sizeof(float));
    float *ySgemv = (float*)calloc( leny , sizeof(float));
    float *ASgemv = (float *)calloc( lenx * leny , sizeof(float));
    float *x1 = NULL;
    float *y1 = NULL;
    float *A1 = NULL;
    for(int i = 0;i < lenx;i++) {
            xSgemv[i] = rand() % 10;
    }
    for(int i = 0;i< lenx * leny;i++) {
            ASgemv[i] = rand() % 25;
    }
    for(int i = 0;i < leny;i++) {
            ySgemv[i] = rand() % 15;
    }
    /* Proper call with column major */
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /*Proper call with row major */
    status =  hc.hcblas_sgemv(RowMajor, typeA, M, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* x is NULL */
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, ASgemv, aOffset, lda, x1, xOffset, incX, &beta, ySgemv, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* Y is NULL */
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, y1, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* A is NULL */
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, A1, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* alpha is NULL */
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, NULL, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* beta is NULL */
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, NULL, ySgemv, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* M is 0 */
    status =  hc.hcblas_sgemv(hcOrder, typeA, 0, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* N is 0 */
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, 0, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incx is 0 */
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, 0, &beta, ySgemv, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incy is 0 */
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, 0);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* alpha and beta is 0 */
    alpha = 0; beta = 0;
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* alpha is 0 and beta is 1*/
    beta = 1;
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    
/* NoTransA */
    hcOrder = ColMajor;
    row = N; col = M; lda = M; typeA = NoTrans;
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    float *xSgemv1 = (float*)calloc( lenx , sizeof(float));
    float *ySgemv1 = (float*)calloc( leny , sizeof(float));
    float *ASgemv1 = (float *)calloc( lenx * leny , sizeof(float));
    for(int i = 0;i < lenx;i++) {
            xSgemv1[i] = rand() % 10;
    }
    for(int i = 0;i< lenx * leny;i++) {
            ASgemv1[i] = rand() % 25;
    }
    for(int i = 0;i < leny;i++) {
            ySgemv1[i] = rand() % 15;
    }
    /* Proper call with column major */
    status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, ASgemv1, aOffset, lda, xSgemv1, xOffset, incX, &beta, ySgemv1, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /*Proper call with row major */
    status =  hc.hcblas_sgemv(RowMajor, typeA, M, N, &alpha, ASgemv1, aOffset, lda, xSgemv1, xOffset, incX, &beta, ySgemv1, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
}

TEST(hcblas_sgemv, func_correct_sgemv_Implementation_type_2) {
    Hcblaslibrary hc;
    int M = 19;
    int N = 19;
    int row, col;
    float alpha = 1;
    float beta = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    long lenx,  leny;
    hcblasStatus status;
    hcblasTranspose typeA;
    hcblasOrder hcOrder = ColMajor;
    row = M; col = N; lda = N; typeA = Trans;
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    float *xSgemv = (float*)calloc( lenx , sizeof(float));
    float *ySgemv = (float*)calloc( leny , sizeof(float));
    float *ASgemv = (float *)calloc( lenx * leny , sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type II - Inputs and Outputs are HC++ float array_view containers */

    hc::array_view<float> xView(lenx, xSgemv);
    hc::array_view<float> yView(leny, ySgemv);
    hc::array_view<float> aMat(M * N, ASgemv);
    for(int i = 0; i < lenx; i++) {
           xView[i] = rand() % 10;
    }
    for(int i = 0;i< lenx * leny;i++) {
           aMat[i] = rand() % 25;
    }
    for(int i = 0;i < leny;i++) {
           yView[i] = rand() % 15;
    }
    /* Proper call with column major */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /*Proper call with row major */
    status = hc.hcblas_sgemv(accl_view, RowMajor, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* M is 0 */
    status =  hc.hcblas_sgemv(accl_view, hcOrder, typeA, 0, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* N is 0 */
    status =hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, 0, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incx is 0 */
    status =hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, 0, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incy is 0 */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, 0);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* alpha and beta is 0 */
    alpha = 0; beta = 0;
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* alpha is 0 and beta is 1*/
    beta = 1;
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    
/* NoTransA */
    hcOrder = ColMajor;
    row = N; col = M; lda = M; typeA = NoTrans;
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    float *xSgemv1 = (float*)calloc( lenx , sizeof(float));
    float *ySgemv1 = (float*)calloc( leny , sizeof(float));
    float *ASgemv1 = (float *)calloc( lenx * leny , sizeof(float));
    hc::array_view<float> xView1(lenx, xSgemv1);
    hc::array_view<float> yView1(leny, ySgemv1);
    hc::array_view<float> aMat1(M * N, ASgemv1);
    for(int i = 0; i < lenx; i++) {
           xView1[i] = rand() % 10;
    }
    for(int i = 0;i< lenx * leny;i++) {
           aMat1[i] = rand() % 25;
    }
    for(int i = 0;i < leny;i++) {
           yView1[i] = rand() % 15;
    }
    /* Proper call with column major */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat1, aOffset, lda, xView1, xOffset, incX, beta, yView1, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /*Proper call with row major */
    status = hc.hcblas_sgemv(accl_view, RowMajor, typeA, M, N, alpha, aMat1, aOffset, lda, xView1, xOffset, incX, beta, yView1, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);

    }

TEST(hcblas_sgemv, func_correct_sgemv_Implementation_type_3) {
    Hcblaslibrary hc;
    int M = 19;
    int N = 19;
    int isTransA = 1;
    int row, col;
    float alpha = 1;
    float beta = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    int batchSize = 64;
    long lenx,  leny;
    hcblasStatus status;
    hcblasTranspose typeA;
    hcblasOrder hcOrder = ColMajor;
    row = M; col = N; lda = N; typeA = Trans;
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    long X_batchOffset = row;
    long Y_batchOffset = col;
    long A_batchOffset = row * col;
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type III - Inputs and Outputs are HC++ float array_view containers with batch processing */

   float *xSgemvbatch = (float*)calloc( lenx * batchSize, sizeof(float));
   float *ySgemvbatch = (float*)calloc( leny * batchSize, sizeof(float));
   float *ASgemvbatch = (float *)calloc( row * col * batchSize, sizeof(float));
   hc::array_view<float> xbatchView(lenx * batchSize, xSgemvbatch);
   hc::array_view<float> ybatchView(leny * batchSize, ySgemvbatch);
   hc::array_view<float> abatchMat(M * N * batchSize, ASgemvbatch);
   for(int i = 0; i < lenx * batchSize; i++) {
            xbatchView[i] = rand() % 10;
   }
   for(int i = 0; i< lenx * leny * batchSize; i++){
            abatchMat[i] = rand() % 25;
   }
   for(int i = 0; i < leny * batchSize; i++) {
            ybatchView[i] = rand() % 15;
   }
 /* Proper call with column major */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /*Proper call with row major */
    status = hc.hcblas_sgemv(accl_view, RowMajor, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* M is 0 */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, 0, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* N is 0 */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, 0, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incx is 0 */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, 0, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incy is 0 */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, 0, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* alpha and beta is 0 */
    alpha = 0; beta = 0;
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* alpha is 0 and beta is 1*/
    beta = 1;
    status =  hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);  
    
/* NoTransA */
   hcOrder = ColMajor;
   row = N; col = M; lda = M; typeA = NoTrans;
   lenx = 1 + (row - 1) * abs(incX);
   leny = 1 + (col - 1) * abs(incY);
   float *xSgemvbatch1 = (float*)calloc( lenx * batchSize, sizeof(float));
   float *ySgemvbatch1 = (float*)calloc( leny * batchSize, sizeof(float));
   float *ASgemvbatch1 = (float *)calloc( row * col * batchSize, sizeof(float));
   hc::array_view<float> xbatchView1(lenx * batchSize, xSgemvbatch1);
   hc::array_view<float> ybatchView1(leny * batchSize, ySgemvbatch1);
   hc::array_view<float> abatchMat1(M * N * batchSize, ASgemvbatch1);
   for(int i = 0; i < lenx * batchSize; i++) {
            xbatchView1[i] = rand() % 10;
   }
   for(int i = 0; i< lenx * leny * batchSize; i++){
            abatchMat1[i] = rand() % 25;
   }
   for(int i = 0; i < leny * batchSize; i++) {
            ybatchView1[i] = rand() % 15;
   }
   /* Proper call with column major */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat1, aOffset, A_batchOffset, lda, xbatchView1, xOffset, X_batchOffset, incX, beta, ybatchView1, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /*Proper call with row major */
    status = hc.hcblas_sgemv(accl_view, RowMajor, typeA, M, N, alpha, abatchMat1, aOffset, A_batchOffset, lda, xbatchView1, xOffset, X_batchOffset, incX, beta, ybatchView1, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
}

TEST(hcblas_sgemv, func_correct_sgemv_Implementation_type_4) {
    Hcblaslibrary hc;
    int M = 19;
    int N = 19;
    int isTransA = 1;
    int row, col;
    float alpha = 1;
    float beta = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    int batchSize = 128;
    long lenx,  leny;
    hcblasStatus status;
    hcblasTranspose typeA;
    hcblasOrder hcOrder = ColMajor;
    row = M; col = N; lda = N; typeA = Trans;
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    float *xSgemv = (float*)calloc( lenx , sizeof(float));
    float *ySgemv = (float*)calloc( leny , sizeof(float));
    float *ASgemv = (float *)calloc( lenx * leny , sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type IV - Inputs and Outputs are HC++ float array containers */

    hc::array<float> xView(lenx, xSgemv);
    hc::array<float> yView(leny, ySgemv);
    hc::array<float> aMat(lenx * leny, ASgemv);
    std::vector<float> HostX(lenx);
    std::vector<float> HostY(leny);
    std::vector<float> HostA(lenx * leny);
    for(int i = 0; i < lenx; i++) {
           HostX[i] = rand() % 10;
    }
    for(int i = 0; i< lenx * leny; i++) {
           HostA[i] = rand() % 25;
    }
    for(int i = 0; i < leny; i++) {
           HostY[i] = rand() % 15;
    }
    hc::copy(begin(HostX), end(HostX), xView);
    hc::copy(begin(HostY), end(HostY), yView);
    hc::copy(begin(HostA), end(HostA), aMat);
    /* Proper call with column major */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /*Proper call with row major */
    status = hc.hcblas_sgemv(accl_view, RowMajor, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* M is 0 */
    status =  hc.hcblas_sgemv(accl_view, hcOrder, typeA, 0, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* N is 0 */
    status =hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, 0, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incx is 0 */
    status =hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, 0, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incy is 0 */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, 0);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* alpha and beta is 0 */
    alpha = 0; beta = 0;
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* alpha is 0 and beta is 1*/
    beta = 1;
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    
/* NoTransA */
    hcOrder = ColMajor;
    row = N; col = M; lda = M; typeA = NoTrans;
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    float *xSgemv1 = (float*)calloc( lenx , sizeof(float));
    float *ySgemv1 = (float*)calloc( leny , sizeof(float));
    float *ASgemv1 = (float *)calloc( lenx * leny , sizeof(float));
    hc::array<float> xView1(lenx, xSgemv1);
    hc::array<float> yView1(leny, ySgemv1);
    hc::array<float> aMat1(lenx * leny, ASgemv1);
    std::vector<float> HostX1(lenx);
    std::vector<float> HostY1(leny);
    std::vector<float> HostA1(lenx * leny);
    for(int i = 0; i < lenx; i++) {
           HostX1[i] = rand() % 10;
    }
    for(int i = 0; i< lenx * leny; i++) {
           HostA1[i] = rand() % 25;
    }
    for(int i = 0; i < leny; i++) {
           HostY1[i] = rand() % 15;
    }
    hc::copy(begin(HostX1), end(HostX1), xView1);
    hc::copy(begin(HostY1), end(HostY1), yView1);
    hc::copy(begin(HostA1), end(HostA1), aMat1);
    /* Proper call with column major */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat1, aOffset, lda, xView1, xOffset, incX, beta, yView1, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /*Proper call with row major */
    status = hc.hcblas_sgemv(accl_view, RowMajor, typeA, M, N, alpha, aMat1, aOffset, lda, xView1, xOffset, incX, beta, yView1, yOffset, incY);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    hc::copy(yView, begin(HostY));
}

TEST(hcblas_sgemv, func_correct_sgemv_Implementation_type_5) {
    Hcblaslibrary hc;
    int M = 19;
    int N = 19;
    int isTransA = 1;
    int row, col;
    float alpha = 1;
    float beta = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    int batchSize = 32;
    long lenx,  leny;
    hcblasStatus status;
    hcblasTranspose typeA;
    hcblasOrder hcOrder = ColMajor;
    row = M; col = N; lda = N; typeA = Trans;
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    long X_batchOffset = row;
    long Y_batchOffset = col;
    long A_batchOffset = row * col;
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type V - Inputs and Outputs are HC++ float array containers with batch processing */

    float *xSgemvbatch = (float*)calloc( lenx * batchSize, sizeof(float));
    float *ySgemvbatch = (float*)calloc( leny * batchSize, sizeof(float));
    float *ASgemvbatch = (float *)calloc( lenx * leny * batchSize, sizeof(float));
    float *ycblasbatch = (float *)calloc( leny * batchSize, sizeof(float));
    hc::array<float> xbatchView(lenx * batchSize, xSgemvbatch);
    hc::array<float> ybatchView(leny * batchSize, ySgemvbatch);
    hc::array<float> abatchMat(lenx * leny * batchSize, ASgemvbatch);
    std::vector<float> HostX_batch(lenx * batchSize);
    std::vector<float> HostY_batch(leny * batchSize);
    std::vector<float> HostA_batch(lenx * leny * batchSize);
    for(int i = 0; i < lenx * batchSize; i++) {
            HostX_batch[i] = rand() % 10;
    }
    for(int i = 0; i< lenx * leny * batchSize; i++) {
            HostA_batch[i] = rand() % 25;
    }
    for(int i = 0; i < leny * batchSize; i++) {
            HostY_batch[i] = rand() % 15;
    }
    hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
    hc::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
    hc::copy(begin(HostA_batch), end(HostA_batch), abatchMat);
    /* Proper call with column major */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /*Proper call with row major */
    status = hc.hcblas_sgemv(accl_view, RowMajor, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* M is 0 */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, 0, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* N is 0 */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, 0, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incx is 0 */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, 0, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incy is 0 */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, 0, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* alpha and beta is 0 */
    alpha = 0; beta = 0;
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* alpha is 0 and beta is 1*/
    beta = 1;
    status =  hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);  
    
/* NoTransA */
    hcOrder = ColMajor;
    row = N; col = M; lda = M; typeA = NoTrans;
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    float *xSgemvbatch1 = (float*)calloc( lenx * batchSize, sizeof(float));
    float *ySgemvbatch1 = (float*)calloc( leny * batchSize, sizeof(float));
    float *ASgemvbatch1 = (float *)calloc( row * col * batchSize, sizeof(float));
    hc::array_view<float> xbatchView1(lenx * batchSize, xSgemvbatch1);
    hc::array_view<float> ybatchView1(leny * batchSize, ySgemvbatch1);
    hc::array_view<float> abatchMat1(M * N * batchSize, ASgemvbatch1);
    std::vector<float> HostX_batch1(lenx * batchSize);
    std::vector<float> HostY_batch1(leny * batchSize);
    std::vector<float> HostA_batch1(lenx * leny * batchSize);
    for(int i = 0; i < lenx * batchSize; i++) {
            HostX_batch1[i] = rand() % 10;
    }
    for(int i = 0; i< lenx * leny * batchSize; i++) {
            HostA_batch1[i] = rand() % 25;
    }
    for(int i = 0; i < leny * batchSize; i++) {
            HostY_batch1[i] = rand() % 15;
    }
    hc::copy(begin(HostX_batch1), end(HostX_batch1), xbatchView1);
    hc::copy(begin(HostY_batch1), end(HostY_batch1), ybatchView1);
    hc::copy(begin(HostA_batch1), end(HostA_batch1), abatchMat1);
   /* Proper call with column major */
    status = hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat1, aOffset, A_batchOffset, lda, xbatchView1, xOffset, X_batchOffset, incX, beta, ybatchView1, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /*Proper call with row major */
    status = hc.hcblas_sgemv(accl_view, RowMajor, typeA, M, N, alpha, abatchMat1, aOffset, A_batchOffset, lda, xbatchView1, xOffset, X_batchOffset, incX, beta, ybatchView1, yOffset, Y_batchOffset, incY, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
}
