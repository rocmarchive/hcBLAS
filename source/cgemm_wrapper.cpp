#include "cgemm_kernels.h"

// CGEMM Wrapper routine that invokes the appropriate kernel routines depending on the input dimension M N and K
// CGEMM Call Type 1: Inputs and Outputs are host float pointers
hcblasStatus Hcblaslibrary:: hcblas_cgemm(hcblasOrder order, hcblasTranspose typeA,
				          hcblasTranspose typeB,
				          const int M, const int N,
				          const int K, const hcComplex* alpha,
				          const hcComplex* A, long aOffset,
				          long lda, const hcComplex* B,
				          long bOffset, long ldb,
				          const hcComplex* beta,
				          hcComplex* C, long cOffset,
				          long ldc) {
  Concurrency::array<float_2, 1> Acmplx(M * K * 2);
  Concurrency::array<float_2, 1> Bcmplx(N * K * 2);
  Concurrency::array<float_2, 1> Ccmplx(M * N * 2);
  std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  float_2 Calpha(alpha->real, alpha->img);
  float_2 Cbeta(beta->real, beta->img);
  std::vector<float_2> HostA(M * K * 2);
  std::vector<float_2> HostB(K * N * 2);
  std::vector<float_2> HostC(M * N * 2);

  for ( int i = 0 ; i <  M * K; i++) {
    HostA[i].x = A[i].real;
    HostA[i].y = A[i].img;
  }

  for ( int i = 0 ; i <  K * N; i++) {
    HostB[i].x = B[i].real;
    HostB[i].y = B[i].img;
  }

  for ( int i = 0 ; i <  M * N; i++) {
    HostC[i].x = C[i].real;
    HostC[i].y = C[i].img;
  }

  Concurrency::copy(begin(HostA), end(HostA), Acmplx);
  Concurrency::copy(begin(HostB), end(HostB), Bcmplx);
  Concurrency::copy(begin(HostC), end(HostC), Ccmplx);
  hcblasStatus status;

  // Start the operations
  if(order) {
    if (typeB == NoTrans) {
      if (typeA == NoTrans) {
        status = cgemm_NoTransAB(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
      } else {
        status = cgemm_NoTransB(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
      }
    } else if (typeA == NoTrans) {
      status = cgemm_NoTransA(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    } else {
      status = cgemm_TransAB(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    }
  } else {
    if (typeB == NoTrans) {
      if (typeA == NoTrans) {
        status = cgemm_NoTransAB_rMajor(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
      } else {
        status = cgemm_NoTransB_rMajor(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
      }
    } else if (typeA == NoTrans) {
      status = cgemm_NoTransA_rMajor(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    } else {
      status = cgemm_TransAB_rMajor(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    }
  }

  Concurrency::copy(Ccmplx, begin(HostC));

  for ( int i = 0 ; i <  M * N; i++) {
    C[i].real = HostC[i].x;
    C[i].img = HostC[i].y;
  }

  return status;
}

// CGEMM Call Type II: Inputs and outputs are C++ HC float array containers
hcblasStatus Hcblaslibrary :: hcblas_cgemm(Concurrency::accelerator_view &accl_view,
				           hcblasOrder order, hcblasTranspose typeA,
					   hcblasTranspose typeB, const int M,
					   const int N, const int K,
					   const Concurrency::graphics::float_2 &Calpha,
					   Concurrency::array<float_2> &Acmplx, long aOffset, long lda,
					   Concurrency::array<float_2> &Bcmplx, long bOffset, long ldb,
					   const Concurrency::graphics::float_2 &Cbeta,
					   Concurrency::array<float_2> &Ccmplx, long cOffset, long ldc) {
  int i, j;
  hcblasStatus status = HCBLAS_SUCCESS;

  // Quick return if possible
  if (!M || !N || (((Calpha.x  == 0 && Calpha.y == 0) || !K) && (Cbeta.x == 1 && Cbeta.y == 1))) {
    return HCBLAS_INVALID;
  }

  // For alpha = 0
  if (!Calpha.x  && !Calpha.y) {
    if (!Cbeta.x && !Cbeta.y) {
      for (j = 0; j < N; ++j)
        for (i = 0; i < M; ++i) {
          Ccmplx[i + j * ldc].x = 0;
        }

      Ccmplx[i + j * ldc].y = 0;
    } else {
      for (j = 0; j < N; ++j)
        for (i = 0; i < M; ++i) {
          Ccmplx[i + j * ldc].x *= Cbeta.x;
        }

      Ccmplx[i + j * ldc].y *= Cbeta.y;
    }

    return status;
  }

  if(order) {
    if (typeB == NoTrans) {
      if (typeA == NoTrans) {
        status = cgemm_NoTransAB(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
      } else {
        status = cgemm_NoTransB(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
      }
    } else if (typeA == NoTrans) {
      status = cgemm_NoTransA(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    } else {
      status = cgemm_TransAB(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    }
  } else {
    if (typeB == NoTrans) {
      if (typeA == NoTrans) {
        status = cgemm_NoTransAB_rMajor(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
      } else {
        status = cgemm_NoTransB_rMajor(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
      }
    } else if (typeA == NoTrans) {
      status = cgemm_NoTransA_rMajor(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    } else {
      status = cgemm_TransAB_rMajor(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    }
  }

  return status;
}

/* CGEMM Call Type III - Overloaded function with arguments related to batch processing */
hcblasStatus Hcblaslibrary :: hcblas_cgemm(Concurrency::accelerator_view &accl_view,
					   hcblasOrder order, hcblasTranspose typeA,
					   hcblasTranspose typeB, const int M,
					   const int N, const int K,
					   const Concurrency::graphics::float_2 &Calpha,
					   Concurrency::array<float_2> &Acmplx,
					   const long aOffset, const long A_batchOffset, const long lda,
					   Concurrency::array<float_2> &Bcmplx,
					   const long bOffset, const long B_batchOffset, const long ldb,
					   const Concurrency::graphics::float_2 &Cbeta,
					   Concurrency::array<float_2> &Ccmplx,
					   const long cOffset, const long C_batchOffset, const long ldc, const int batchSize) {
  int i, j;
  hcblasStatus status = HCBLAS_SUCCESS;

  // Quick return if possible
  if (!M || !N || (((Calpha.x  == 0 && Calpha.y == 0) || !K) && (Cbeta.x == 1 && Cbeta.y == 1))) {
    return HCBLAS_INVALID;
  }

  // For alpha = 0
  if (!Calpha.x  && !Calpha.y) {
    if (!Cbeta.x && !Cbeta.y) {
      for (j = 0; j < N; ++j)
        for (i = 0; i < M; ++i) {
          Ccmplx[i + j * ldc].x = 0;
        }

      Ccmplx[i + j * ldc].y = 0;
    } else {
      for (j = 0; j < N; ++j)
        for (i = 0; i < M; ++i) {
          Ccmplx[i + j * ldc].x *= Cbeta.x;
        }

      Ccmplx[i + j * ldc].y *= Cbeta.y;
    }

    return status;
  }

  if(order) {
    if (typeB == NoTrans) {
      if (typeA == NoTrans) {
        status = cgemm_NoTransAB(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
      } else {
        status = cgemm_NoTransB(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
      }
    } else if (typeA == NoTrans) {
      status = cgemm_NoTransA(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
    } else {
      status = cgemm_TransAB(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
    }
  } else {
    if (typeB == NoTrans) {
      if (typeA == NoTrans) {
        status = cgemm_NoTransAB_rMajor(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
      } else {
        status = cgemm_NoTransB_rMajor(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
      }
    } else if (typeA == NoTrans) {
      status = cgemm_NoTransA_rMajor(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
    } else {
      status = cgemm_TransAB_rMajor(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
    }
  }

  return status;
}


