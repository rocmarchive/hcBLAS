#include "sgemm_array_kernels.h"

// Sgemm Wrapper routine that invokes the appropriate kernel routines depending on the input dimension M N and K
hcblasStatus gemm_HC(Concurrency::accelerator_view &accl_view,
                     const int order, char TransA, char TransB,
                     const int M, const int N, const int K,
                     const float alpha, Concurrency::array<float> &A_mat,
                     long aOffset, long lda,
                     Concurrency::array<float> &B_mat,
                     long bOffset, long ldb, const float beta,
                     Concurrency::array<float> &C_mat,
                     long cOffset, long ldc,
                     long A_batchOffset = 0, long B_batchOffset = 0, long C_batchOffset = 0, int batchSize = 0) {
  hcblasStatus status = HCBLAS_SUCCESS;
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


// Sgemm Call Type 1: Inputs and Outputs are host float pointers
hcblasStatus Hcblaslibrary :: hcblas_sgemm(hcblasOrder order,
    					   hcblasTranspose typeA,
				           hcblasTranspose typeB,
				           const int M, const int N,
				           const int K, const float* alpha,
				           float* A, const long lda,
				           float* B, const long ldb,
				           const float* beta, float* C,
				           const long ldc, const long aOffset,
				           const long bOffset,
				           const long cOffset) {
  // Quick return if possible
  if (!M || !N || !K) {
    return HCBLAS_INVALID;
  }
  Concurrency::array<float> A_mat(K * M, A);
  Concurrency::array<float> B_mat(N * K, B);
  Concurrency::array<float> C_mat(M * N, C);
  std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  std::vector<float> HostA(M * K);
  std::vector<float> HostB(K * N);
  std::vector<float> HostC(M * N);

  for(int i = 0; i < M * K; i++) {
    HostA[i] = A[i];
  }

  for(int i = 0; i < K * N; i++) {
    HostB[i] = B[i];
  }

  for(int i = 0; i < M * N; i++)  {
    HostC[i] = C[i];
  }
 
  int i, j;
  float temp;
  hcblasStatus status = HCBLAS_SUCCESS;

  // For alpha = 0
  if (*alpha == 0) {
    if (*beta == 0) {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          HostC[cOffset + i + j * ldc] = 0;
        }
      }
    } else {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          temp = HostC[cOffset + i + j * ldc];
          HostC[cOffset + i + j * ldc] = temp * (*beta);
        }
      }
    }
  for(int i = 0; i < M * N; i++)  {
    C[i] = HostC[i];
  }

    return status;
  }
  Concurrency::copy(begin(HostA), end(HostA), A_mat);
  Concurrency::copy(begin(HostB), end(HostB), B_mat);
  Concurrency::copy(begin(HostC), end(HostC), C_mat);
  status = gemm_HC(accl_view, order, typeA, typeB, M, N, K, *alpha,
                                A_mat, aOffset, lda, B_mat, bOffset, ldb,
                                *beta, C_mat, cOffset, ldc);
  Concurrency::copy(C_mat, begin(HostC));

  for(int i = 0; i < M * N; i++)  {
    C[i] = HostC[i];
  }

  return status;
}


// Sgemm Call Type II: Inputs and outputs are C++ HC float array_View containers
hcblasStatus  Hcblaslibrary :: hcblas_sgemm(Concurrency::accelerator_view &accl_view,
					    hcblasOrder order,
					    hcblasTranspose typeA,
					    hcblasTranspose typeB, const int M,
					    const int N, const int K, const float &alpha,
					    Concurrency::array<float> &A, const long lda,
					    Concurrency::array<float> &B, const long ldb,
					    const float &beta,
					    Concurrency::array<float> &C, const long ldc,
					    const long aOffset, const long bOffset, const long cOffset) {
  int i, j;
  float temp;
  hcblasStatus status = HCBLAS_SUCCESS;

  // Quick return if possible
  if (!M || !N || !K) {
    return HCBLAS_INVALID;
  }

  // For alpha = 0
  if (alpha == 0) {
    std::vector<float> HostC(M * N);
    Concurrency::copy(C, begin(HostC));
    if (beta == 0) {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          HostC[cOffset + i + j * ldc] = 0;
        }
      }
    } else {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          temp = HostC[cOffset + i + j * ldc];
          HostC[cOffset + i + j * ldc] = temp * (beta);
        }
      }
    }
    Concurrency::copy(begin(HostC), end(HostC), C);
    return status;
  }

  status = gemm_HC(accl_view, order, typeA, typeB, M, N, K, alpha, A,
                                aOffset, lda, B, bOffset, ldb, beta, C,
                                cOffset, ldc);
  return status;
}

/* SGEMM- Overloaded function with arguments related to batch processing */
hcblasStatus Hcblaslibrary :: hcblas_sgemm(Concurrency::accelerator_view &accl_view,
					   hcblasOrder order,
					   hcblasTranspose typeA,
					   hcblasTranspose typeB, const int M,
					   const int N, const int K, const float &alpha,
					   Concurrency::array<float> &A, const long lda, const long A_batchOffset,
					   Concurrency::array<float> &B, const long ldb, const long B_batchOffset,
					   const float &beta,
					   Concurrency::array<float> &C, const long ldc, const long C_batchOffset,
					   const long aOffset, const long bOffset, const long cOffset, const int batchSize) {
  int i, j, k;
  float temp;
  hcblasStatus status = HCBLAS_SUCCESS;

  // Quick return if possible
  if (!M || !N || !K) {
    return HCBLAS_INVALID;
  }

  // For alpha = 0
  if (alpha == 0) {
    std::vector<float> HostC(M * N * batchSize);
    Concurrency::copy(C, begin(HostC));
    if (beta == 0) {
     for ( k = 0; k < batchSize; ++k) {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          HostC[cOffset + C_batchOffset * k + i + j * ldc] = 0;
        }
      }
     }
    } else {
     for (k = 0; k < batchSize; ++k) {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          temp = HostC[cOffset + C_batchOffset * k + i + j * ldc];
          HostC[cOffset + C_batchOffset * k + i + j * ldc] = temp * (beta);
        }
      }
     }
    }
    Concurrency::copy(begin(HostC), end(HostC), C);
    return status;
  }

  status = gemm_HC(accl_view, order, typeA, typeB, M, N, K, alpha, A, aOffset, lda, B,
          bOffset, ldb, beta, C, cOffset, ldc, A_batchOffset, B_batchOffset, C_batchOffset, batchSize);
  return status;
}

