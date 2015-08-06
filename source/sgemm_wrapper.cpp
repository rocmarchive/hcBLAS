#include "sgemm_kernels.h"

// Sgemm Wrapper routine that invokes the appropriate kernel routines depending on the input dimension M N and K
ampblasStatus gemm_AMP(Concurrency::accelerator_view &accl_view,
                       const int order, char TransA, char TransB, 
                       const int M, const int N, const int K,
                       const float alpha, Concurrency::array_view<float> &A_mat, 
                       long aOffset, long lda,
                       Concurrency::array_view<float> &B_mat, 
                       long bOffset, long ldb, const float beta,
                       Concurrency::array_view<float> &C_mat, 
                       long cOffset, long ldc,
                       Concurrency::array_view<float> &temp_buf, long A_batchOffset = 0, long B_batchOffset = 0, long C_batchOffset=0, int batchSize =0)
{
  int i, j;
  ampblasStatus status = AMPBLAS_SUCCESS;
  // Quick return if possible
  if (!M || !N || ((alpha == 0 || !K) && beta == 1))
    return status;
  // For alpha = 0
  if (alpha == 0)
  {
    if (beta == 0)
    {
      for (j = 0; j < N; ++j)
        for (i = 0; i < M; ++i)
          C_mat[i + j * ldc] = 0;
    }
    else
    {
      for (j = 0; j < N; ++j)
        for (i = 0; i < M; ++i)
          C_mat[i + j * ldc] *= beta;
    }
    return status;
  }
  // Start the operations
   
  if (order)
  {
    if(batchSize > 0)
    {
      if (TransB == 'n')
      {
        if (TransA == 'n')
          status = gemm_NoTransAB(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
        else
          status = gemm_NoTransB(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
      }
      else if (TransA == 'n')
        status = gemm_NoTransA(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
      else
        status = gemm_TransAB(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);

    }
    else
    {
      if (TransB == 'n')
      {
         if (TransA == 'n')
           status = gemm_NoTransAB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
         else
           status = gemm_NoTransB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      }
      else if (TransA == 'n')
        status = gemm_NoTransA(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      else
        status = gemm_TransAB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
    }
  }
  else
  {
   if(batchSize > 0)
   {
     if (TransB == 'n')
      {
        if (TransA == 'n')
          status = gemm_NoTransAB_rMajor(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
        else
          status = gemm_NoTransB_rMajor(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
      }
      else if (TransA == 'n')
        status = gemm_NoTransA_rMajor(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
      else
        status = gemm_TransAB_rMajor(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);

    }
    else
    {
      if (TransB == 'n')
      {                   
         if (TransA == 'n')
           status = gemm_NoTransAB_rMajor(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
         else
           status = gemm_NoTransB_rMajor(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      }
      else if (TransA == 'n')
        status = gemm_NoTransA_rMajor(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      else
        status = gemm_TransAB_rMajor(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
    }
  }
  return status;
}


// Sgemm Call Type 1: Inputs and Outputs are host float pointers
ampblasStatus Ampblaslibrary :: ampblas_sgemm(const enum AMPBLAS_ORDER order,
 					      const enum AMPBLAS_TRANS typeA,
                                              const enum AMPBLAS_TRANS typeB,
                                              const int M, const int N,
                                              const int K, const float *alpha,
                                              float *A, const long lda,
                                              float *B, const long ldb,
                                              const float *beta, float *C,
                                              const long ldc, const long aOffset,
                                              const long bOffset,
                                              const long cOffset)
{
    Concurrency::array_view<float> A_mat(K * M, A);
    Concurrency::array_view<float> B_mat(N * K, B);
    Concurrency::array_view<float> C_mat(M * N, C);
    Concurrency::array_view<float> *temp_buf = NULL;
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

 
    ampblasStatus status = gemm_AMP(accl_view, order, typeA, typeB, M, N, K, *alpha,
                                    A_mat, aOffset, lda, B_mat, bOffset, ldb,
                                    *beta, C_mat, cOffset, ldc, *temp_buf);

    return status;
}


// Sgemm Call Type II: Inputs and outputs are C++ AMP float array_View containers
ampblasStatus  Ampblaslibrary :: ampblas_sgemm(Concurrency::accelerator_view &accl_view,
 					       const enum AMPBLAS_ORDER order,
                                 	       const enum AMPBLAS_TRANS typeA,
                                               const enum AMPBLAS_TRANS typeB, const int M,
                                               const int N, const int K, const float &alpha,
                                               Concurrency::array_view<float> &A, const long lda,
                                               Concurrency::array_view<float> &B, const long ldb,
                                               const float &beta,
                                               Concurrency::array_view<float> &C, const long ldc,
                                               const long aOffset, const long bOffset, const long cOffset)

{
    Concurrency::array_view<float> *temp_buf = NULL;
    ampblasStatus status = gemm_AMP(accl_view, order, typeA, typeB, M, N, K, alpha, A,
                                    aOffset, lda, B, bOffset, ldb, beta, C,
                                    cOffset, ldc, *temp_buf);

    return status;
}

/* SGEMM- Overloaded function with arguments related to batch processing */
ampblasStatus Ampblaslibrary :: ampblas_sgemm(Concurrency::accelerator_view &accl_view,
                                              const enum AMPBLAS_ORDER order,
                                              const enum AMPBLAS_TRANS typeA,
                                              const enum AMPBLAS_TRANS typeB, const int M,
                                              const int N, const int K, const float &alpha,
                                              Concurrency::array_view<float> &A, const long lda, const long A_batchOffset,
                                              Concurrency::array_view<float> &B, const long ldb, const long B_batchOffset,
                                              const float &beta,
                                              Concurrency::array_view<float> &C, const long ldc, const long C_batchOffset,
                                              const long aOffset, const long bOffset, const long cOffset, const int batchSize)

{
    Concurrency::array_view<float> *temp_buf = NULL;
    gemm_AMP(accl_view, order, typeA, typeB, M, N, K, alpha, A, aOffset, lda, B,
             bOffset, ldb, beta, C, cOffset, ldc, *temp_buf, A_batchOffset, B_batchOffset, C_batchOffset, batchSize);

    return AMPBLAS_SUCCESS;
}

