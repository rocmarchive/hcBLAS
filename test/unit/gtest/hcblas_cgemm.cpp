#include "hcblaslib.h"
#include <cstdlib>
#include "gtest/gtest.h"
#include "hc_am.hpp"
#include "cblas.h"
#include "hc_short_vector.hpp"

using namespace hc::short_vector;

TEST(hcblas_cgemm, return_correct_cgemm_Implementation_type_1) {
    Hcblaslibrary hc; 
    int M = 189, N = 9, K = 19;
    long lda, ldb, ldc;
    int incX = 1, incY = 1;
    long aOffset = 0;
    long bOffset = 0;
    long cOffset = 0;
    hcblasOrder hcOrder;
    hcblasTranspose typeA, typeB;
    hcblasStatus status;
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view()); 
    
// Implementation type I - Inputs and Outputs are HCC device pointers */

    float_2 cAlpha, cBeta;
    cAlpha.x = 1;
    cAlpha.y = 1;
    cBeta.x = 1;
    cBeta.y = 1;
    float_2 *A = (float_2*) calloc(M * K, sizeof(float_2));
    float_2 *B = (float_2*) calloc(K * N, sizeof(float_2));
    float_2 *C = (float_2*) calloc(M * N, sizeof(float_2));
    float_2* devA = hc::am_alloc(sizeof(float_2) * M * K, acc[1], 0);
    float_2* devB = hc::am_alloc(sizeof(float_2) * K * N, acc[1], 0);
    float_2* devC = hc::am_alloc(sizeof(float_2) * M * N, acc[1], 0);
    for(int i = 0; i < M * K; i++) {
                A[i].x = rand() % 10;
                A[i].y = rand() % 20;
    }
    for(int i = 0; i < K * N;i++) {
                B[i].x = rand() % 15;
                B[i].y = rand() % 25;
    }
    for(int i = 0; i < M * N;i++) {
                C[i].x = rand() % 18;
                C[i].y = rand() % 28;
                C[i] = rand() % 25;
    }
    hc::am_copy(devA, A, M * K * sizeof(float_2));
    hc::am_copy(devB, B, K * N * sizeof(float_2));
    hc::am_copy(devC, C, M * N * sizeof(float_2));
// NoTransA and NoTransB */           
    typeA = NoTrans;
    typeB = NoTrans;
    // Column major */
    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major */
    lda = K; ldb = N ; ldc = N;      
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
// NoTransA TransB */  
    typeA = NoTrans;
    typeB = Trans;
    // Column major */
    lda = M; ldb = N ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major */ 
    lda = K; ldb = K ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
// TransA NoTransB */
    typeA = Trans;
    typeB = NoTrans;
    // Column major */
    lda = K; ldb = K ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major */ 
    lda = M; ldb = N ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);  
    
// TransA TransB */
    typeA = Trans;
    typeB = Trans;
    // Column major */
    lda = K; ldb = N ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major */ 
    lda = M; ldb = K ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);  
    
    typeA = NoTrans;
    typeB = NoTrans;
    lda = M; ldb = K ; ldc = M;
    hcOrder = ColMajor;
    float_2 *devA1 = NULL;
    float_2 *devB1 = NULL;
    float_2 *devC1 = NULL;
    /* A, B, C device pointers are not allocated properly */
    status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, K, cAlpha, devA1, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_INVALID);
    status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB1, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_INVALID);
    status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC1, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_INVALID);
    // M is 0 */ 
    status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, 0, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_INVALID);
    // N is 0 */
    status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, 0, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_INVALID);
    // K is 0 */
    status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, 0, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_INVALID);
    free(A);
    free(B);
    free(C);
    hc::am_free(devA);
    hc::am_free(devB);
    hc::am_free(devC);
}


TEST(hcblas_cgemm, func_correct_cgemm_Implementation_type_1) {
    Hcblaslibrary hc; 
    int M = 189, N = 9, K = 19;
    long lda, ldb, ldc;
    int incX = 1, incY = 1;
    long aOffset = 0;
    long bOffset = 0;
    long cOffset = 0;
    hcblasOrder hcOrder;
    hcblasTranspose typeA, typeB;
    hcblasStatus status;
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view()); 
    enum CBLAS_TRANSPOSE Transa, Transb;
    float alpha[2], beta[2];
// Implementation type I - Inputs and Outputs are HCC device pointers */
    float_2 cAlpha, cBeta;
    cAlpha.x = 1;
    cAlpha.y = 1;
    cBeta.x = 1;
    cBeta.y = 1;
    alpha[0] = cAlpha.x;
    alpha[1] = cAlpha.y;
    beta[0] = cBeta.x;
    beta[1] = cBeta.y;
    float_2 *A = (float_2*) calloc(M * K, sizeof(float_2));
    float_2 *B = (float_2*) calloc(K * N, sizeof(float_2));
    float_2 *C = (float_2*) calloc(M * N, sizeof(float_2));
    float_2* devA = hc::am_alloc(sizeof(float_2) * M * K, acc[1], 0);
    float_2* devB = hc::am_alloc(sizeof(float_2) * K * N, acc[1], 0);
    float_2* devC = hc::am_alloc(sizeof(float_2) * M * N, acc[1], 0);
    float* ablas = (float *)malloc(sizeof(float )* M * K * 2);
    float* bblas = (float *)malloc(sizeof(float )* K * N * 2);
    float* cblas = (float *)malloc(sizeof(float )* M * N * 2);
    int k = 0;
    for(int i = 0; i < M * K; i++) {
                A[i].x = rand() % 10;
                A[i].y = rand() % 20;
                ablas[k++] = A[i].x;
                ablas[k++] = A[i].y;
    }
    k = 0;
    for(int i = 0; i < K * N;i++) {
                B[i].x = rand() % 15;
                B[i].y = rand() % 25;
                bblas[k++] = B[i].x;
                bblas[k++] = B[i].y;
    }
    k = 0;
    for(int i = 0; i < M * N;i++) {
                C[i].x = rand() % 18;
                C[i].y = rand() % 28;
                cblas[k++] = C[i].x;
                cblas[k++] = C[i].y;
    }
    hc::am_copy(devA, A, M * K * sizeof(float_2));
    hc::am_copy(devB, B, K * N * sizeof(float_2));
    hc::am_copy(devC, C, M * N * sizeof(float_2));

// NoTransA and NoTransB */           
    typeA = NoTrans;
    typeB = NoTrans;
    Transa = CblasNoTrans;
    Transb = CblasNoTrans;

    // Column major */
    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0, k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    }

    // Row Major */
    lda = K; ldb = N ; ldc = N;     
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    } 
    
// NoTransA TransB */  
    typeA = NoTrans;
    typeB = Trans;
    Transa = CblasNoTrans;
    Transb = CblasTrans;

    // Column major */
    lda = M; ldb = N ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    }
    
    /* alpha and beta are zeroes */
    /* alpha = 0*/
    cAlpha.x = 0;
    cAlpha.y = 0;
    cBeta.x = 1;
    cBeta.y = 1;
    alpha[0] = cAlpha.x;
    alpha[1] = cAlpha.y;
    beta[0] = cBeta.x;
    beta[1] = cBeta.y;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    }
    /* alpha = 0, beta = 0*/
    cAlpha.x = 0;
    cAlpha.y = 0;
    cBeta.x = 0;
    cBeta.y = 0;
    alpha[0] = cAlpha.x;
    alpha[1] = cAlpha.y;
    beta[0] = cBeta.x;
    beta[1] = cBeta.y;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    }

    // Row Major */
    cAlpha.x = 1;
    cAlpha.y = 1;
    cBeta.x = 1;
    cBeta.y = 1;
    alpha[0] = cAlpha.x;
    alpha[1] = cAlpha.y;
    beta[0] = cBeta.x;
    beta[1] = cBeta.y; 
    lda = K; ldb = K ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    }

    /* alpha and beta are zeroes */
    /* alpha = 0*/
    cAlpha.x = 0;
    cAlpha.y = 0;
    cBeta.x = 1;
    cBeta.y = 1;
    alpha[0] = cAlpha.x;
    alpha[1] = cAlpha.y;
    beta[0] = cBeta.x;
    beta[1] = cBeta.y;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    }

    /* alpha = 0, beta = 0*/
    cAlpha.x = 0;
    cAlpha.y = 0;
    cBeta.x = 0;
    cBeta.y = 0;
    alpha[0] = cAlpha.x;
    alpha[1] = cAlpha.y;
    beta[0] = cBeta.x;
    beta[1] = cBeta.y;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    }

// TransA NoTransB */
    typeA = Trans;
    typeB = NoTrans;
    Transa = CblasTrans;
    Transb = CblasNoTrans;
    cAlpha.x = 1;
    cAlpha.y = 1;
    cBeta.x = 1;
    cBeta.y = 1;
    alpha[0] = cAlpha.x;
    alpha[1] = cAlpha.y;
    beta[0] = cBeta.x;
    beta[1] = cBeta.y;

    // Column major */
    lda = K; ldb = K ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    }

    // Row Major */ 
    lda = M; ldb = N ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    }

// TransA TransB */
    typeA = Trans;
    typeB = Trans;
    Transa = CblasTrans;
    Transb = CblasTrans;

    // Column major */
    lda = K; ldb = N ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    }

    
    // Row Major */ 
    lda = M; ldb = K ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devA, aOffset, lda, devB, bOffset, ldb, cBeta, devC, cOffset, ldc);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(C, devC, M * N * sizeof(float_2));
    cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, &alpha, ablas, lda, bblas, ldb, &beta, cblas, ldc );
    for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2) {
            EXPECT_EQ(C[i].x, cblas[k]);
            EXPECT_EQ(C[i].y, cblas[k+1]);
    }

    free(A);
    free(B);
    free(C);
    free(cblas);
    free(ablas);
    free(bblas);
    hc::am_free(devA);
    hc::am_free(devB);
    hc::am_free(devC);
}


TEST(hcblas_cgemm, return_correct_cgemm_Implementation_type_2) {
    Hcblaslibrary hc; 
    int M = 189, N = 9, K = 19;
    long lda, ldb, ldc;
    int incX = 1, incY = 1;
    long aOffset = 0;
    long bOffset = 0;
    long cOffset = 0;
    long A_batchOffset = 0;
    long B_batchOffset = 0;
    long C_batchOffset = M * N;
    int batchSize = 64;
    hcblasOrder hcOrder;
    hcblasTranspose typeA, typeB;
    hcblasStatus status;
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view()); 
    float_2 cAlpha, cBeta;
    cAlpha.x = 1;
    cAlpha.y = 1;
    cBeta.x = 1;
    cBeta.y = 1;
 
   // Implementation type II - Inputs and Outputs are HCC float array containers with batch processing 
        
   float_2 *Abatch = (float_2*) calloc(M * K, sizeof(float_2));
   float_2 *Bbatch = (float_2*) calloc(K * N, sizeof(float_2));
   float_2 *Cbatch = (float_2*) calloc(M * N * batchSize, sizeof(float_2));          
   float_2* devAbatch = hc::am_alloc(sizeof(float_2) * M * K, acc[1], 0);
   float_2* devBbatch = hc::am_alloc(sizeof(float_2) * K * N, acc[1], 0);
   float_2* devCbatch = hc::am_alloc(sizeof(float_2) * M * N * batchSize, acc[1], 0);

   for(int i = 0; i < M * K; i++) {
             Abatch[i].x = rand() % 10;
             Abatch[i].y = rand() % 20;
   }
   for(int i = 0; i < K * N;i++) {
             Bbatch[i].x = rand() % 15;
             Bbatch[i].y = rand() % 25;
   }
   for(int i = 0; i < M * N * batchSize;i++) {
             Cbatch[i].x = rand() % 18;
             Cbatch[i].y = rand() % 28;
   } 

   hc::am_copy(devAbatch, Abatch, M * K * sizeof(float));
   hc::am_copy(devBbatch, Bbatch, K * N * sizeof(float));
   hc::am_copy(devCbatch, Cbatch, M * N * batchSize * sizeof(float));

   // NoTransA and NoTransB            
    typeA = NoTrans;
    typeB = NoTrans;
    // Column major 
    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major 
    lda = K; ldb = N ; ldc = N;   
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);   
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    // NoTransA TransB   
    typeA = NoTrans;
    typeB = Trans;
    // Column major 
    lda = M; ldb = N ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major  
    lda = K; ldb = K ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    // TransA NoTransB 
    typeA = Trans;
    typeB = NoTrans;
    // Column major
    lda = K; ldb = K ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major 
    lda = M; ldb = N ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);  

    // TransA TransB 
    typeA = Trans;
    typeB = Trans;
    // Column major 
    lda = K; ldb = N ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major 
    lda = M; ldb = K ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    typeA = NoTrans;
    typeB = NoTrans;
    lda = M; ldb = K ; ldc = M;
    float_2 *devA1 = NULL;
    float_2 *devB1 = NULL;
    float_2 *devC1 = NULL;
    /* A, B, C device pointers are not allocated properly */
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devA1, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID); 
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devB1, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devC1, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    // M is 0
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, 0, N, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    // N is 0
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, 0, K, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    // K is 0
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, 0, cAlpha, devAbatch, aOffset, A_batchOffset, lda, devBbatch, bOffset, B_batchOffset, ldb, cBeta, devCbatch, cOffset, C_batchOffset, ldc, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);            
    free(Abatch);
    free(Bbatch);
    free(Cbatch);
    hc::am_free(devAbatch);
    hc::am_free(devBbatch);
    hc::am_free(devCbatch);
}   
 
#if 0 
TEST(hcblas_cgemm, func_correct_cgemm_Implementation_type_2) {
    Hcblaslibrary hc; 
    int M = 189, N = 9, K = 19;
    float alpha = 1, beta = 1;
    long lda, ldb, ldc;
    int incX = 1, incY = 1;
    long aOffset = 0;
    long bOffset = 0;
    long cOffset = 0;
    long A_batchOffset = 0;
    long B_batchOffset = 0;
    long C_batchOffset = M * N;
    int batchSize = 64;
    hcblasOrder hcOrder;
    hcblasTranspose typeA, typeB;
    hcblasStatus status;
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view()); 
    enum CBLAS_TRANSPOSE Transa, Transb;
   // Implementation type II - Inputs and Outputs are HCC float array containers with batch processing 
        
   float *Abatch = (float*) calloc(M * K, sizeof(float));
   float *Bbatch = (float*) calloc(K * N, sizeof(float));
   float *Cbatch = (float*) calloc(M * N * batchSize, sizeof(float));          
   float *CCblasbatch = (float*) calloc(M * N * batchSize, sizeof(float));
   float *Chcblasbatch = (float*) calloc(M * N * batchSize, sizeof(float));
   float* devAbatch = hc::am_alloc(sizeof(float) * M * K, acc[1], 0);
   float* devBbatch = hc::am_alloc(sizeof(float) * K * N, acc[1], 0);
   float* devCbatch = hc::am_alloc(sizeof(float) * M * N * batchSize, acc[1], 0);

   for(int i = 0; i < M * K; i++) {
                Abatch[i] = rand()%100;
   }
   for(int i = 0; i < K * N;i++) {
                Bbatch[i] = rand() % 15;
   }
   for(int i = 0; i < M * N * batchSize;i++) {
                Cbatch[i] = rand() % 25;
                CCblasbatch[i] = Cbatch[i];
   } 

   hc::am_copy(devAbatch, Abatch, M * K * sizeof(float));
   hc::am_copy(devBbatch, Bbatch, K * N * sizeof(float));
   hc::am_copy(devCbatch, Cbatch, M * N * batchSize * sizeof(float));

   // NoTransA and NoTransB            
    typeA = NoTrans;
    typeB = NoTrans;
    Transa = CblasNoTrans;
    Transb = CblasNoTrans;

    // Column major 
    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, alpha, Abatch, lda, Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++) 
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    /* alpha = 0 */
    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, 0, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, 0, Abatch, lda, Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++) 
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    /* alpha = 0, beta = 0*/
    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, 0, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, 0, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, 0, Abatch, lda, Bbatch, ldb, 0, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++) 
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    
    // Row Major 
    lda = K; ldb = N ; ldc = N;      
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda, Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++) 
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    
    /* alpha = 0 */
    lda = K; ldb = N ; ldc = N;      
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, 0, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, 0, Abatch, lda, Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++) 
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    /* alpha = 0, beta = 0 */
    lda = K; ldb = N ; ldc = N;      
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, 0, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, 0, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, 0, Abatch, lda, Bbatch, ldb, 0, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    // NoTransA TransB   
    typeA = NoTrans;
    typeB = Trans;
    Transa = CblasNoTrans;
    Transb = CblasTrans;

    // Column major 
    lda = M; ldb = N ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, alpha, Abatch, lda, Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++) 
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    
    // Row Major  
    lda = K; ldb = K ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda, Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    // TransA NoTransB 
    typeA = Trans;
    typeB = NoTrans;
    Transa = CblasTrans;
    Transb = CblasNoTrans;

    // Column major
    lda = K; ldb = K ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, alpha, Abatch, lda, Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++) 
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    
    // Row Major 
    lda = M; ldb = N ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);  
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda, Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++) 
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);

    // TransA TransB 
    typeA = Trans;
    typeB = Trans;
    Transa = CblasTrans;
    Transb = CblasTrans;

    // Column major 
    lda = K; ldb = N ; ldc = M;
    status = hc.hcblas_cgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasColMajor, Transa, Transb, M, N, K, alpha, Abatch, lda, Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++)
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    
    // Row Major 
    lda = M; ldb = K ; ldc = N;
    status = hc.hcblas_cgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, devAbatch, lda, A_batchOffset, devBbatch, ldb, B_batchOffset, beta, devCbatch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    hc::am_copy(Chcblasbatch, devCbatch,  M * N * batchSize * sizeof(float));
    for(int i = 0; i < batchSize; i++)
        cblas_cgemm( CblasRowMajor, Transa, Transb, M, N, K, alpha, Abatch, lda, Bbatch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
    for(int i = 0 ; i < M * N * batchSize; i++) 
        EXPECT_EQ(Chcblasbatch[i], CCblasbatch[i]);
    free(Abatch);
    free(Bbatch);
    free(Cbatch);
    free(CCblasbatch);
    free(Chcblasbatch);
    hc::am_free(devAbatch);
    hc::am_free(devBbatch);
    hc::am_free(devCbatch);

}
#endif
