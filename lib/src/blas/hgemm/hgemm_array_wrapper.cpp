/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "./hgemm_array_kernels.h"

// Sgemm Wrapper routine that invokes the appropriate kernel routines depending
// on the input dimension M N and K
hcblasStatus gemm_HC(hc::accelerator_view accl_view, const int order,
                     char TransA, char TransB, const int M, const int N,
                     const int K, const hc::half alpha, hc::half *A_mat,
                     __int64_t aOffset, __int64_t lda, hc::half *B_mat,
                     __int64_t bOffset, __int64_t ldb, const hc::half beta,
                     hc::half *C_mat, __int64_t cOffset, __int64_t ldc) {
  hcblasStatus status = HCBLAS_SUCCEEDS;
  // Start the operations

  if (order) {
    if (TransB == 'n') {
      if (TransA == 'n') {
        status =
            gemm_NoTransAB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat,
                           cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      } else {
        status = gemm_NoTransB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat,
                               cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      }
    } else if (TransA == 'n') {
      status = gemm_NoTransA(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat,
                             cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
    } else {
      status = gemm_TransAB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat,
                            cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
    }
  } else {
    if (TransB == 'n') {
      if (TransA == 'n') {
        status = gemm_NoTransAB_rMajor(accl_view, A_mat, aOffset, B_mat,
                                       bOffset, C_mat, cOffset, M, N, K, lda,
                                       ldb, ldc, alpha, beta);
      } else {
        status = gemm_NoTransB_rMajor(accl_view, A_mat, aOffset, B_mat, bOffset,
                                      C_mat, cOffset, M, N, K, lda, ldb, ldc,
                                      alpha, beta);
      }
    } else if (TransA == 'n') {
      status =
          gemm_NoTransA_rMajor(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat,
                               cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
    } else {
      status =
          gemm_TransAB_rMajor(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat,
                              cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
    }
  }
  return status;
}
hcblasStatus gemm_HC(hc::accelerator_view accl_view, const int order,
                     char TransA, char TransB, const int M, const int N,
                     const int K, const hc::half alpha, hc::half *A_mat[],
                     __int64_t aOffset, __int64_t lda, hc::half *B_mat[],
                     __int64_t bOffset, __int64_t ldb, const hc::half beta,
                     hc::half *C_mat[], __int64_t cOffset, __int64_t ldc,
                     __int64_t A_batchOffset = 0, __int64_t B_batchOffset = 0,
                     __int64_t C_batchOffset = 0, int batchSize = 0) {
  hcblasStatus status = HCBLAS_SUCCEEDS;
  // Start the operations

  if (order) {
    if (batchSize >= 0) {
      if (TransB == 'n') {
        if (TransA == 'n') {
          status = gemm_NoTransAB(accl_view, A_mat, aOffset, A_batchOffset,
                                  B_mat, bOffset, B_batchOffset, C_mat, cOffset,
                                  C_batchOffset, M, N, K, lda, ldb, ldc, alpha,
                                  beta, batchSize);
        } else {
          status = gemm_NoTransB(accl_view, A_mat, aOffset, A_batchOffset,
                                 B_mat, bOffset, B_batchOffset, C_mat, cOffset,
                                 C_batchOffset, M, N, K, lda, ldb, ldc, alpha,
                                 beta, batchSize);
        }
      } else if (TransA == 'n') {
        status =
            gemm_NoTransA(accl_view, A_mat, aOffset, A_batchOffset, B_mat,
                          bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset,
                          M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
      } else {
        status =
            gemm_TransAB(accl_view, A_mat, aOffset, A_batchOffset, B_mat,
                         bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset,
                         M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
      }
    }
  } else {
    /*    if(batchSize > 0) {
          if (TransB == 'n') {
            if (TransA == 'n') {
              status = gemm_NoTransAB_rMajor(accl_view, A_mat, aOffset,
       A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset,
       C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
            } else {
              status = gemm_NoTransB_rMajor(accl_view, A_mat, aOffset,
       A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset,
       C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
            }
          } else if (TransA == 'n') {
            status = gemm_NoTransA_rMajor(accl_view, A_mat, aOffset,
       A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset,
       C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
          } else {
            status = gemm_TransAB_rMajor(accl_view, A_mat, aOffset,
       A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset,
       C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
          }
        } else {
          if (TransB == 'n') {
            if (TransA == 'n') {
              status = gemm_NoTransAB_rMajor(accl_view, A_mat, aOffset, B_mat,
       bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
            } else {
              status = gemm_NoTransB_rMajor(accl_view, A_mat, aOffset, B_mat,
       bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
            }
          } else if (TransA == 'n') {
            status = gemm_NoTransA_rMajor(accl_view, A_mat, aOffset, B_mat,
       bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
          } else {
            status = gemm_TransAB_rMajor(accl_view, A_mat, aOffset, B_mat,
       bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
          }
        }*/
  }

  return status;
}
// Type 1 -  alpha = 0 Kernel

hcblasStatus gemm_alpha0_col(hc::accelerator_view accl_view, hc::half *A,
                             __int64_t aOffset, hc::half *B, __int64_t bOffset,
                             hc::half *C, __int64_t cOffset, int M, int N,
                             int K, int lda, int ldb, int ldc, hc::half alpha,
                             hc::half beta) {
#define GEMM_BLOCK 256
  hc::extent<2> grdExt(N, M * GEMM_BLOCK);
  hc::tiled_extent<2> t_ext = grdExt.tile(1, GEMM_BLOCK);
  hc::parallel_for_each(accl_view, t_ext, [=](hc::tiled_index<2> tidx)[[hc]] {
    int threadIdx = tidx.local[1];
    int Row = tidx.tile[0];
    int Col = tidx.tile[1];
    if (threadIdx == 0 && Col < M && Row < N) {
      __int64_t C_index = cOffset + Row * ldc + Col;
      C[C_index] = (hisnan(C[C_index]) || hisinf(C[C_index])) ? 0 : C[C_index];
      if (alpha == 0) {
        if (beta == 0) {
          C[C_index] = 0.0;
        } else {
          C[C_index] *= beta;
        }
      }
    }
  }) ;
#undef GEMM_BLOCK
  return HCBLAS_SUCCEEDS;
}

// Type 2 - alpha = 0 kernel

hcblasStatus gemm_alpha0_col_batch(hc::accelerator_view accl_view, hc::half *A,
                                   __int64_t aOffset, __int64_t A_batchOffset,
                                   hc::half *B, __int64_t bOffset,
                                   __int64_t B_batchOffset, hc::half *C,
                                   __int64_t cOffset, __int64_t C_batchOffset,
                                   int M, int N, int K, int lda, int ldb,
                                   int ldc, hc::half alpha, hc::half beta,
                                   int batchSize) {
#define GEMM_BLOCK 256
  hc::extent<3> grdExt(batchSize, N, M * GEMM_BLOCK);
  hc::tiled_extent<3> t_ext = grdExt.tile(1, 1, GEMM_BLOCK);
  hc::parallel_for_each(accl_view, t_ext, [=](hc::tiled_index<3> tidx)[[hc]] {
    int elt = tidx.tile[0];
    int threadIdx = tidx.local[2];
    int Row = tidx.tile[1];
    int Col = tidx.tile[2];
    if (threadIdx == 0 && Col < M && Row < N) {
      __int64_t C_index = cOffset + C_batchOffset * elt + Row * ldc + Col;
      C[C_index] = (hisnan(C[C_index]) || hisinf(C[C_index])) ? 0 : C[C_index];
      if (alpha == 0) {
        if (beta == 0) {
          C[C_index] = 0.0;
        } else {
          C[C_index] *= beta;
        }
      }
    }
  }) ;
#undef GEMM_BLOCK
  return HCBLAS_SUCCEEDS;
}

// Type 1 -  alpha = 0 Kernel

hcblasStatus gemm_alpha0_row(hc::accelerator_view accl_view, hc::half *A,
                             __int64_t aOffset, hc::half *B, __int64_t bOffset,
                             hc::half *C, __int64_t cOffset, int M, int N,
                             int K, int lda, int ldb, int ldc, hc::half alpha,
                             hc::half beta) {
#define GEMM_BLOCK 256
  hc::extent<2> grdExt(N, M * GEMM_BLOCK);
  hc::tiled_extent<2> t_ext = grdExt.tile(1, GEMM_BLOCK);
  hc::parallel_for_each(accl_view, t_ext, [=](hc::tiled_index<2> tidx)[[hc]] {
    int threadIdx = tidx.local[1];
    int Row = tidx.tile[0];
    int Col = tidx.tile[1];
    if (threadIdx == 0 && Col < M && Row < N) {
      __int64_t C_index = cOffset + Row + Col * ldc;
      C[C_index] = (hisnan(C[C_index]) || hisinf(C[C_index])) ? 0 : C[C_index];
      if (alpha == 0) {
        if (beta == 0) {
          C[C_index] = 0.0;
        } else {
          C[C_index] *= beta;
        }
      }
    }
  }) ;
#undef GEMM_BLOCK
  return HCBLAS_SUCCEEDS;
}

// Type 2 - alpha = 0 kernel

hcblasStatus gemm_alpha0_row_batch(hc::accelerator_view accl_view, hc::half *A,
                                   __int64_t aOffset, __int64_t A_batchOffset,
                                   hc::half *B, __int64_t bOffset,
                                   __int64_t B_batchOffset, hc::half *C,
                                   __int64_t cOffset, __int64_t C_batchOffset,
                                   int M, int N, int K, int lda, int ldb,
                                   int ldc, hc::half alpha, hc::half beta,
                                   int batchSize) {
#define GEMM_BLOCK 256
  hc::extent<3> grdExt(batchSize, N, M * GEMM_BLOCK);
  hc::tiled_extent<3> t_ext = grdExt.tile(1, 1, GEMM_BLOCK);
  hc::parallel_for_each(accl_view, t_ext, [=](hc::tiled_index<3> tidx)[[hc]] {
    int elt = tidx.tile[0];
    int threadIdx = tidx.local[2];
    int Row = tidx.tile[1];
    int Col = tidx.tile[2];
    if (threadIdx == 0 && Col < M && Row < N) {
      __int64_t C_index = cOffset + C_batchOffset * elt + Row + Col * ldc;
      C[C_index] = (hisnan(C[C_index]) || hisinf(C[C_index])) ? 0 : C[C_index];
      if (alpha == 0) {
        if (beta == 0) {
          C[C_index] = 0.0;
        } else {
          C[C_index] *= beta;
        }
      }
    }
  }) ;
#undef GEMM_BLOCK
  return HCBLAS_SUCCEEDS;
}

// Hgemm Call Type I: Inputs and outputs are HCC device pointers
hcblasStatus Hcblaslibrary::hcblas_hgemm(
    hc::accelerator_view accl_view, hcblasOrder order, hcblasTranspose typeA,
    hcblasTranspose typeB, const int M, const int N, const int K,
    const hc::half &alpha, hc::half *A, const __int64_t lda, hc::half *B,
    const __int64_t ldb, const hc::half &beta, hc::half *C, const __int64_t ldc,
    const __int64_t aOffset, const __int64_t bOffset, const __int64_t cOffset) {
  int i, j;
  hc::half temp;
  hcblasStatus status = HCBLAS_SUCCEEDS;

  // Quick return if possible
  if (A == NULL || B == NULL || C == NULL || !M || !N || !K) {
    return HCBLAS_INVALID;
  }

  // For alpha = 0
  if (alpha == 0) {
    if (order) {
      status = gemm_alpha0_col(accl_view, A, aOffset, B, bOffset, C, cOffset, M,
                               N, K, lda, ldb, ldc, alpha, beta);
    } else {
      status = gemm_alpha0_row(accl_view, A, aOffset, B, bOffset, C, cOffset, M,
                               N, K, lda, ldb, ldc, alpha, beta);
    }
    return status;
  }
  status = gemm_HC(accl_view, order, typeA, typeB, M, N, K, alpha, A, aOffset,
                   lda, B, bOffset, ldb, beta, C, cOffset, ldc);
  return status;
}

/* HGEMM- Overloaded function with arguments related to batch processing */
/*hcblasStatus Hcblaslibrary :: hcblas_hgemm(hc::accelerator_view accl_view,
                                           hcblasOrder order,
                                           hcblasTranspose typeA,
                                           hcblasTranspose typeB, const int M,
                                           const int N, const int K, const
  hc::half
  &alpha,
                                           hc::half *A[], const __int64_t lda,
  const
  __int64_t
  A_batchOffset,
                                           hc::half *B[], const __int64_t ldb,
  const
  __int64_t
  B_batchOffset,
                                           const hc::half &beta,
                                           hc::half *C[], const __int64_t ldc,
  const
  __int64_t
  C_batchOffset,
                                           const __int64_t aOffset, const
  __int64_t
  bOffset, const __int64_t cOffset, const int batchSize) {
  int i, j, k;
  hc::half temp;
  hcblasStatus status = HCBLAS_SUCCEEDS;

  // Quick return if possible
  if (A == NULL || B == NULL || C == NULL || !M || !N || !K) {
    return HCBLAS_INVALID;
  }
*/

/*  // For alpha = 0
  if (alpha == 0) {
    if (order) {
        status = gemm_alpha0_col_batch(accl_view, A, aOffset, A_batchOffset, B,
  bOffset, B_batchOffset, C, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc,
  alpha, beta, batchSize);
    }
    else {
        status = gemm_alpha0_row_batch(accl_view, A, aOffset, A_batchOffset, B,
  bOffset, B_batchOffset, C, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc,
  alpha, beta, batchSize);
    }
    return status;
  }*/

/* status = gemm_HC(accl_view, order, typeA, typeB, M, N, K, alpha, A, aOffset,
lda, B,
         bOffset, ldb, beta, C, cOffset, ldc, A_batchOffset, B_batchOffset,
C_batchOffset, batchSize);
 return status;
}
*/
