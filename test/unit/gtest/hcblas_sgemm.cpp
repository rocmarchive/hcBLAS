#include "hcblaslib.h"
#include <cstdlib>
#include "gtest/gtest.h"

TEST(hcblas_sgemm, func_correct_sgemm_Implementation_type_1) {
    Hcblaslibrary hc; 
    int M = 19, N = 19, K = 19;
    float alpha = 1, beta = 1;
    long lda, ldb, ldc;
    int incX = 1, incY = 1;
    long aOffset = 0;
    long bOffset = 0;
    long cOffset = 0;
    hcblasOrder hcOrder;
    hcblasTranspose typeA, typeB;
    hcblasStatus status;
    float *Asgemm = (float*) calloc(M * K, sizeof(float));
    float *Bsgemm = (float*) calloc(K * N, sizeof(float));
    float *Csgemm = (float*) calloc(M * N, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view()); 
    
// Implementation type I - Inputs and Outputs are HCC float array containers */

    hc::array<float, 1> A_mat(K * M, Asgemm);
    hc::array<float, 1> B_mat(N * K, Bsgemm);
    hc::array<float, 1> C_mat(M * N, Csgemm);
    std::vector<float> HostA(M * K);
    std::vector<float> HostB(K * N);
    std::vector<float> HostC(M * N);
    for(int i = 0; i < M * K; i++) {
                HostA[i] = rand()%100;
    }
    for(int i = 0; i < K * N;i++) {
                HostB[i] = rand() % 15;
    }
    for(int i = 0; i < M * N;i++) {
            HostC[i] = rand() % 25;
    }
    hc::copy(begin(HostA), end(HostA), A_mat);
    hc::copy(begin(HostB), end(HostB), B_mat);
    hc::copy(begin(HostC), end(HostC), C_mat);

// NoTransA and NoTransB */           
    typeA = NoTrans;
    typeB = NoTrans;
    // Column major */
    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major */
    lda = K; ldb = N ; ldc = N;      
    status = hc.hcblas_sgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
// NoTransA TransB */  
    typeA = NoTrans;
    typeB = Trans;
    // Column major */
    lda = M; ldb = N ; ldc = M;
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major */ 
    lda = K; ldb = K ; ldc = N;
    status = hc.hcblas_sgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
// TransA NoTransB */
    typeA = Trans;
    typeB = NoTrans;
    // Column major */
    lda = K; ldb = K ; ldc = M;
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major */ 
    lda = M; ldb = N ; ldc = N;
    status = hc.hcblas_sgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);  
    
// TransA TransB */
    typeA = Trans;
    typeB = Trans;
    // Column major */
    lda = K; ldb = N ; ldc = M;
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major */ 
    lda = M; ldb = K ; ldc = N;
    status = hc.hcblas_sgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset); 
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);  
    
    typeA = NoTrans;
    typeB = NoTrans;
    lda = M; ldb = K ; ldc = M;
    hcOrder = ColMajor;
    // alpha is 0 */
    status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, M, N, K, 0, A_mat, lda, B_mat, ldb, 0, C_mat, ldc, aOffset, bOffset, cOffset); 
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, M, N, K, 0, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset); 
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    // M is 0 */ 
    status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, 0, N, K, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
    // N is 0 */
    status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, M, 0, K, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
    // K is 0 */
    status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, M, N, 0, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_sgemm, func_correct_sgemm_Implementation_type_2) {
    Hcblaslibrary hc; 
    int M = 19, N = 19, K = 19;
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
    
   // Implementation type II - Inputs and Outputs are HCC float array containers with batch processing 
        
   float *Asgemm_batch = (float*) calloc(M * K, sizeof(float));
   float *Bsgemm_batch = (float*) calloc(K * N, sizeof(float));
   float *Csgemm_batch = (float*) calloc(M * N * batchSize, sizeof(float));
   hc::array<float> A_batch(K * M, Asgemm_batch);
   hc::array<float> B_batch(N * K, Bsgemm_batch);
   hc::array<float> C_batch(M * N * batchSize, Csgemm_batch);
   std::vector<float> HostA_batch(M * K);
   std::vector<float> HostB_batch(K * N);
   std::vector<float> HostC_batch(M * N * batchSize);
   for(int i = 0; i < M * K; i++) {
                HostA_batch[i] = rand()%100;
   }
   for(int i = 0; i < K * N;i++) {
                HostB_batch[i] = rand() % 15;
   }
   for(int i = 0; i < M * N * batchSize;i++) {
                HostC_batch[i] = rand() % 25;
   } 

   hc::copy(HostA_batch.begin(), HostA_batch.end(), A_batch);
   hc::copy(HostB_batch.begin(), HostB_batch.end(), B_batch);
   hc::copy(HostC_batch.begin(), HostC_batch.end(), C_batch);
   // NoTransA and NoTransB            
    typeA = NoTrans;
    typeB = NoTrans;
    // Column major 
    lda = M; ldb = K ; ldc = M;
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major 
    lda = K; ldb = N ; ldc = N;      
    status = hc.hcblas_sgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    // NoTransA TransB   
    typeA = NoTrans;
    typeB = Trans;
    // Column major 
    lda = M; ldb = N ; ldc = M;
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major  
    lda = K; ldb = K ; ldc = N;
    status = hc.hcblas_sgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    // TransA NoTransB 
    typeA = Trans;
    typeB = NoTrans;
    // Column major
    lda = K; ldb = K ; ldc = M;
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major 
    lda = M; ldb = N ; ldc = N;
    status = hc.hcblas_sgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);  

    // TransA TransB 
    typeA = Trans;
    typeB = Trans;
    // Column major 
    lda = K; ldb = N ; ldc = M;
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    
    // Row Major 
    lda = M; ldb = K ; ldc = N;
    status = hc.hcblas_sgemm(accl_view, RowMajor, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);

    typeA = NoTrans;
    typeB = NoTrans;
    lda = M; ldb = K ; ldc = M;

    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, N, K, 0, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, 0, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, N, K, 0, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCEEDS);
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, 0, N, K, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, 0, K, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    status = hc.hcblas_sgemm(accl_view, ColMajor, typeA, typeB, M, N, 0, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);            
}   
   
  

