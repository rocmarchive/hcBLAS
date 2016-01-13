#include "cgemm_array_view_kernels.h"

// CGEMM Wrapper routine that invokes the appropriate kernel routines depending on the input dimension M N and K
// CGEMM Call Type II: Inputs and outputs are C++ HC float array_View containers
hcblasStatus Hcblaslibrary :: hcblas_cgemm(hc::accelerator_view &accl_view,
  					   hcblasOrder order, hcblasTranspose typeA,
   					   hcblasTranspose typeB, const int M,
 					   const int N, const int K,
 					   const Concurrency::graphics::float_2 &Calpha,
 					   hc::array_view<float_2> &Acmplx, long aOffset, long lda,
 					   hc::array_view<float_2> &Bcmplx, long bOffset, long ldb,
 					   const Concurrency::graphics::float_2 &Cbeta,
 					   hc::array_view<float_2> &Ccmplx, long cOffset, long ldc) {
  int i, j;
  hcblasStatus status = HCBLAS_SUCCESS;
  float tempReal = 0.0, tempImg = 0.0;
  // Quick return if possible
  if (!M || !N || !K) {
    return HCBLAS_INVALID;
  }

  // For alpha = 0
  if (!Calpha.x  && !Calpha.y) {
    if (!Cbeta.x && !Cbeta.y) {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          Ccmplx[cOffset + i + j * ldc].x = 0;
          Ccmplx[cOffset + i + j * ldc].y = 0;
        }
      }
    }else {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          tempReal = Ccmplx[cOffset + i + j * ldc].x;
          tempImg =  Ccmplx[cOffset + i + j * ldc].y;
          Ccmplx[cOffset + i + j * ldc].x = (tempReal * Cbeta.x - tempImg * Cbeta.y);
          Ccmplx[cOffset + i + j * ldc].y = (tempReal * Cbeta.y + tempImg * Cbeta.x);
        }
      }
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
hcblasStatus Hcblaslibrary :: hcblas_cgemm(hc::accelerator_view &accl_view,
 					   hcblasOrder order, hcblasTranspose typeA,
 					   hcblasTranspose typeB, const int M,
 					   const int N, const int K,
 					   const Concurrency::graphics::float_2 &Calpha,
 					   hc::array_view<float_2> &Acmplx,
 					   const long aOffset, const long A_batchOffset, const long lda,
 					   hc::array_view<float_2> &Bcmplx,
 					   const long bOffset, const long B_batchOffset, const long ldb,
 					   const Concurrency::graphics::float_2 &Cbeta,
 					   hc::array_view<float_2> &Ccmplx,
 					   const long cOffset, const long C_batchOffset, const long ldc, const int batchSize) {
  int i, j, k;
  hcblasStatus status = HCBLAS_SUCCESS;
  float tempReal = 0.0, tempImg = 0.0;
  // Quick return if possible
  if (!M || !N || !K) {
    return HCBLAS_INVALID;
  }

  // For alpha = 0
  if (!Calpha.x  && !Calpha.y) {
    if (!Cbeta.x && !Cbeta.y) {
     for(k = 0; k < batchSize ; ++k) {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          Ccmplx[cOffset + C_batchOffset * k + i + j * ldc].x = 0;
          Ccmplx[cOffset + C_batchOffset * k + i + j * ldc].y = 0;
        }
      }
     }
    }else {
     for(k = 0 ; k < batchSize ; ++k) {
      for (j = 0; j < N; ++j) {
        for (i = 0; i < M; ++i) {
          tempReal = Ccmplx[cOffset + C_batchOffset * k + i + j * ldc].x;
          tempImg =  Ccmplx[cOffset + C_batchOffset * k + i + j * ldc].y;
          Ccmplx[cOffset + C_batchOffset * k + i + j * ldc].x = (tempReal * Cbeta.x - tempImg * Cbeta.y);
          Ccmplx[cOffset + C_batchOffset * k + i + j * ldc].y = (tempReal * Cbeta.y + tempImg * Cbeta.x);
        }
      }
     }
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


