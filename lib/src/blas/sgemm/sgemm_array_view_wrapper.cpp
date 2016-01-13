#include "sgemm_array_view_kernels.h"

// Sgemm Wrapper routine that invokes the appropriate kernel routines depending on the input dimension M N and K
hcblasStatus gemm_HC(hc::accelerator_view &accl_view,
                     const int order, char TransA, char TransB,
                     const int M, const int N, const int K,
                     const float alpha, hc::array_view<float> &A_mat,
                     long aOffset, long lda,
                     hc::array_view<float> &B_mat,
                     long bOffset, long ldb, const float beta,
                     hc::array_view<float> &C_mat,
                     long cOffset, long ldc,
                     long A_batchOffset = 0, long B_batchOffset = 0, long C_batchOffset = 0, int batchSize = 0) {
  int i, j, k;
  hcblasStatus status = HCBLAS_SUCCESS;

  // Quick return if possible
  if (!M || !N || !K) {
    return HCBLAS_INVALID;
  }

  // For alpha = 0
  if (alpha == 0) {
    if (beta == 0) {
     for (k = 0; k <= batchSize; ++k) {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          C_mat[cOffset + C_batchOffset * k + i + j * ldc] = 0;
        }
      }
     }
    } else {
     for(k = 0; k <= batchSize; ++k) {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          C_mat[cOffset + C_batchOffset * k + i + j * ldc] *= beta;
        }
      }
     }
    }

    return status;
  }

  // Start the operations

  if (order) {
    if(batchSize > 0) {
      if (TransB == 'n') {
        if (TransA == 'n') {
          status = gemm_NoTransAB(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
        } else {
          status = gemm_NoTransB(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
        }
      } else if (TransA == 'n') {
        status = gemm_NoTransA(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
      } else {
        status = gemm_TransAB(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
      }
    } else {
      if (TransB == 'n') {
        if (TransA == 'n') {
          status = gemm_NoTransAB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
        } else {
          status = gemm_NoTransB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
        }
      } else if (TransA == 'n') {
        status = gemm_NoTransA(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      } else {
        status = gemm_TransAB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      }
    }
  } else {
    if(batchSize > 0) {
      if (TransB == 'n') {
        if (TransA == 'n') {
          status = gemm_NoTransAB_rMajor(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
        } else {
          status = gemm_NoTransB_rMajor(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
        }
      } else if (TransA == 'n') {
        status = gemm_NoTransA_rMajor(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
      } else {
        status = gemm_TransAB_rMajor(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
      }
    } else {
      if (TransB == 'n') {
        if (TransA == 'n') {
          status = gemm_NoTransAB_rMajor(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
        } else {
          status = gemm_NoTransB_rMajor(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
        }
      } else if (TransA == 'n') {
        status = gemm_NoTransA_rMajor(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      } else {
        status = gemm_TransAB_rMajor(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      }
    }
  }

  return status;
}

// Sgemm Call Type II: Inputs and outputs are C++ HC float array_View containers
hcblasStatus  Hcblaslibrary :: hcblas_sgemm(hc::accelerator_view &accl_view,
 					    hcblasOrder order,
					    hcblasTranspose typeA,
					    hcblasTranspose typeB, const int M,
					    const int N, const int K, const float &alpha,
					    hc::array_view<float> &A, const long lda,
					    hc::array_view<float> &B, const long ldb,
					    const float &beta,
					    hc::array_view<float> &C, const long ldc,
					    const long aOffset, const long bOffset, const long cOffset) {
  hcblasStatus status = gemm_HC(accl_view, order, typeA, typeB, M, N, K, alpha, A,
                                aOffset, lda, B, bOffset, ldb, beta, C,
                                cOffset, ldc);
  return status;
}

/* SGEMM- Overloaded function with arguments related to batch processing */
hcblasStatus Hcblaslibrary :: hcblas_sgemm(hc::accelerator_view &accl_view,
					   hcblasOrder order,
 					   hcblasTranspose typeA,
 					   hcblasTranspose typeB, const int M,
 					   const int N, const int K, const float &alpha,
 					   hc::array_view<float> &A, const long lda, const long A_batchOffset,
 					   hc::array_view<float> &B, const long ldb, const long B_batchOffset,
 					   const float &beta,
 					   hc::array_view<float> &C, const long ldc, const long C_batchOffset,
 					   const long aOffset, const long bOffset, const long cOffset, const int batchSize) {
  hcblasStatus status = gemm_HC(accl_view, order, typeA, typeB, M, N, K, alpha, A, aOffset, lda, B,
          bOffset, ldb, beta, C, cOffset, ldc, A_batchOffset, B_batchOffset, C_batchOffset, batchSize);
  return status;
}

