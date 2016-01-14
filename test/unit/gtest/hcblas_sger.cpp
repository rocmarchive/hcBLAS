#include "hcblaslib.h"
#include <cstdlib> 
#include "gtest/gtest.h"

TEST(hcblas_sger, func_correct_sger_Implementation_type_1) {
    Hcblaslibrary hc;
    int M = 19;
    int N = 19;
    float alpha = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    long lenx,  leny; 
    hcblasStatus status;
    lenx =  1 + (M-1) * abs(incX);
    leny =  1 + (N-1) * abs(incY);
    float *xSger = (float*)calloc( lenx , sizeof(float));
    float *ySger = (float*)calloc( leny , sizeof(float));
    float *ASger = (float *)calloc( lenx * leny , sizeof(float));
    float *x1 = NULL;
    float *y1 = NULL;
    float *A1 = NULL;
/* Implementation type I - Inputs and Outputs are host float pointers */
    hcblasOrder hcOrder = ColMajor;
    lda = (hcOrder)? M : N;
    for(int i = 0; i < lenx; i++) {
                xSger[i] = rand() % 10;
    }
    for(int i = 0; i < leny; i++) {
                ySger[i] = rand() % 15;
    }
    for(int i = 0; i< lenx * leny; i++) {
                ASger[i] = rand() % 25;
    }

    /* Proper call column major*/
    status = hc.hcblas_sger(hcOrder, M , N , &alpha, xSger, xOffset, incX, ySger, yOffset, incY, ASger, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   
    hcOrder = RowMajor;
    lda = (hcOrder)? M : N;
    /* Proper call row major*/
    status = hc.hcblas_sger(hcOrder, M , N , &alpha, xSger, xOffset, incX, ySger, yOffset, incY, ASger, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    /* x is NULL */
    status = hc.hcblas_sger(hcOrder, M , N , &alpha, x1, xOffset, incX, ySger, yOffset, incY, ASger, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* y is NULL */
    status = hc.hcblas_sger(hcOrder, M , N , &alpha, xSger, xOffset, incX, y1, yOffset, incY, ASger, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* A is NULL */
    status = hc.hcblas_sger(hcOrder, M , N , &alpha, xSger, xOffset, incX, ySger, yOffset, incY, A1, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* alpha is NULL */
    status = hc.hcblas_sger(hcOrder, M , N , NULL, xSger, xOffset, incX, ySger, yOffset, incY, ASger, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* alpha is 0 */
    alpha = 0;
    status = hc.hcblas_sger(hcOrder, M , N , &alpha, xSger, xOffset, incX, ySger, yOffset, incY, ASger, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* M is 0 */
    status = hc.hcblas_sger(hcOrder, 0 , N , &alpha, xSger, xOffset, incX, ySger, yOffset, incY, ASger, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* N is 0 */
    status = hc.hcblas_sger(hcOrder, M , 0 , &alpha, xSger, xOffset, incX, ySger, yOffset, incY, ASger, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incx is 0 */
    status = hc.hcblas_sger(hcOrder, M , N , &alpha, xSger, xOffset, 0 , ySger, yOffset, incY, ASger, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incy is 0 */
    status = hc.hcblas_sger(hcOrder, M , N , &alpha, xSger, xOffset, incX, ySger, yOffset, 0 , ASger, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_sger, func_correct_sger_Implementation_type_2) {
    Hcblaslibrary hc;
    int M = 19;
    int N = 19;
    float alpha = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    long lenx,  leny;
    hcblasStatus status;
    hcblasOrder hcOrder = ColMajor;
    lda = (hcOrder)? M : N;
    lenx =  1 + (M-1) * abs(incX);
    leny =  1 + (N-1) * abs(incY);
    float *xSger = (float*)calloc( lenx , sizeof(float));
    float *ySger = (float*)calloc( leny , sizeof(float));
    float *ASger = (float *)calloc( lenx * leny , sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type II - Inputs and Outputs are HC++ float array_view containers */
    array_view<float> xView(lenx, xSger);
    array_view<float> yView(leny, ySger);
    array_view<float> aMat( M * N, ASger);
    for(int i = 0; i < lenx; i++) {
                xView[i] = rand() % 10;
    }
    for(int i = 0; i < leny; i++) {
                yView[i] = rand() % 15;
    }
    for(int i = 0; i< lenx * leny; i++) {
                aMat[i] = rand() % 25;
    }
    /* Proper call with column major */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    hcOrder = RowMajor;
    lda = (hcOrder)? M : N;
    /* Proper call with row major */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* alpha is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , 0 , xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* M is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, 0 , N , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* N is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , 0 , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incx is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, 0, yView, yOffset, incY, aMat, aOffset, lda);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incy is 0*/
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, incX, yView, yOffset, 0, aMat, aOffset, lda);
    EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_sger, func_correct_sger_Implementation_type_3) {
    Hcblaslibrary hc;
    int M = 19;
    int N = 19;
    float alpha = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    long X_batchOffset = M;
    long Y_batchOffset = N;
    long A_batchOffset = M * N;
    int batchSize = 128;
    long lenx,  leny;
    hcblasStatus status;
    hcblasOrder hcOrder = ColMajor;
    lda = (hcOrder)? M : N;
    lenx =  1 + (M-1) * abs(incX);
    leny =  1 + (N-1) * abs(incY);
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type III - Inputs and Outputs are HC++ float array_view containers with batch processing */
    float *xSgerbatch = (float*)calloc( lenx * batchSize, sizeof(float));
    float *ySgerbatch = (float*)calloc( leny * batchSize, sizeof(float));
    float *ASgerbatch = (float *)calloc( lenx * leny * batchSize, sizeof(float));
    array_view<float> xbatchView(lenx * batchSize, xSgerbatch);
    array_view<float> ybatchView(leny * batchSize, ySgerbatch);
    array_view<float> abatchMat( lenx * leny * batchSize, ASgerbatch);
    for(int i = 0;i < lenx * batchSize;i++) {
               xbatchView[i] = rand() % 10;
    }
    for(int i = 0;i < leny * batchSize;i++) {
               ybatchView[i] = rand() % 15;
    }
    for(int i = 0;i< lenx * leny * batchSize;i++) {
               abatchMat[i] = rand() % 25;
    }

    /* Proper call with column major */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    hcOrder = RowMajor;
    lda = (hcOrder)? M : N;
    /* Proper call with Row major */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* alpha is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , 0, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* M is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, 0 , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* N is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , 0 , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incx is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, 0, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incy is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, 0, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_sger, func_correct_sger_Implementation_type_4) {
    Hcblaslibrary hc;
    int M = 19;
    int N = 19;
    float alpha = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    long lenx,  leny;
    hcblasStatus status;
    hcblasOrder hcOrder = ColMajor;
    lda = (hcOrder)? M : N;
    lenx =  1 + (M-1) * abs(incX);
    leny =  1 + (N-1) * abs(incY);
    float *xSger = (float*)calloc( lenx , sizeof(float));
    float *ySger = (float*)calloc( leny , sizeof(float));
    float *ASger = (float *)calloc( lenx * leny , sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type IV - Inputs and Outputs are HC++ float array containers */
    std::vector<float> HostX(lenx);
    std::vector<float> HostY(leny);
    std::vector<float> HostA(lenx * leny);
    hc::array<float> xView(lenx, xSger);
    hc::array<float> yView(leny, ySger);
    hc::array<float> aMat( lenx * leny, ASger);
    for(int i = 0; i < lenx; i++) {
                HostX[i] = rand() % 10;
    }
    for(int i = 0; i < leny; i++) {
                HostY[i] = rand() % 15;
    }
    for(int i = 0; i< lenx * leny; i++) {
                HostA[i] = rand() % 25;
    }
    hc::copy(begin(HostX), end(HostX), xView);
    hc::copy(begin(HostY), end(HostY), yView);
    hc::copy(begin(HostA), end(HostA), aMat);
    /* Proper call with column major */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    hcOrder = RowMajor;
    lda = (hcOrder)? M : N;
    /* Proper call with row major */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* alpha is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , 0, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* M is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, 0 , N , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* N is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , 0 , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incx is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, 0 , yView, yOffset, incY, aMat, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incy is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, incX, yView, yOffset, 0 , aMat, aOffset, lda );
    EXPECT_EQ(status, HCBLAS_INVALID);
    hc::copy(aMat, begin(HostA));
}

TEST(hcblas_sger, func_correct_sger_Implementation_type_5) {
    Hcblaslibrary hc;
    int M = 19;
    int N = 19;
    float alpha = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    long X_batchOffset = M;
    long Y_batchOffset = N;
    long A_batchOffset = M * N;
    int batchSize = 128;
    long lenx,  leny;
    hcblasStatus status;
    hcblasOrder hcOrder = ColMajor;
    lda = (hcOrder)? M : N;
    lenx =  1 + (M-1) * abs(incX);
    leny =  1 + (N-1) * abs(incY);
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type V - Inputs and Outputs are HC++ float array containers with batch processing */

    float *xSgerbatch = (float*)calloc( lenx * batchSize, sizeof(float));
    float *ySgerbatch = (float*)calloc( leny * batchSize, sizeof(float));
    float *ASgerbatch = (float *)calloc( lenx * leny * batchSize, sizeof(float));
    std::vector<float> HostX_batch(lenx * batchSize);
    std::vector<float> HostY_batch(leny * batchSize);
    std::vector<float> HostA_batch(lenx * leny * batchSize);
    hc::array<float> xbatchView(lenx * batchSize, xSgerbatch);
    hc::array<float> ybatchView(leny * batchSize, ySgerbatch);
    hc::array<float> abatchMat( lenx * leny * batchSize, ASgerbatch);
    for(int i = 0; i < lenx * batchSize; i++){
                HostX_batch[i] = rand() % 10;
    }
    for(int i = 0; i < leny * batchSize; i++){
                HostY_batch[i] = rand() % 15;
    }
    for(int i = 0; i< lenx * leny * batchSize; i++){
                HostA_batch[i] = rand() % 25;
    }
    hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
    hc::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
    hc::copy(begin(HostA_batch), end(HostA_batch), abatchMat);
    /* Proper call with column major */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hcOrder = RowMajor;
    lda = (hcOrder)? M : N;
    /* Proper call with row major */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* alpha is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , 0, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    /* M is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, 0 , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* N is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , 0 , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incx is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, 0, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incy is 0 */
    status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, 0, abatchMat, aOffset, A_batchOffset, lda, batchSize );
    EXPECT_EQ(status, HCBLAS_INVALID);
    hc::copy(abatchMat, begin(HostA_batch));
}
