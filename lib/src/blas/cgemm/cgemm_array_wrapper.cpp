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

#include "./cgemm_array_kernels.h"

hcblasStatus cgemm_alpha0_col(hc::accelerator_view accl_view,
                              hc::short_vector::float_2 *A, __int64_t aOffset,
                              hc::short_vector::float_2 *B, __int64_t bOffset,
                              hc::short_vector::float_2 *C, __int64_t cOffset,
                              int M, int N, int K, int lda, int ldb, int ldc,
                              hc::short_vector::float_2 alpha,
                              hc::short_vector::float_2 beta) {
#define THREADS 16
#define TILE_DIM 16
  hc::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1),
                       (M + (THREADS - 1)) & ~(THREADS - 1));
  hc::tiled_extent<2> t_ext = grdExt.tile(THREADS, THREADS);
  hc::parallel_for_each(accl_view, t_ext, [=](hc::tiled_index<2> tidx)[[hc]] {
    int Row = tidx.tile[0] * TILE_DIM + tidx.local[0];
    int Col = tidx.tile[1] * TILE_DIM + tidx.local[1];
    float CReal = 0.0;
    float CImg = 0.0;
    if (Row < N && Col < M) {
      CReal = C[cOffset + (tidx.global[0] * ldc) + tidx.global[1]].x;
      CImg = C[cOffset + (tidx.global[0] * ldc) + tidx.global[1]].y;
      CReal = (hc::fast_math::isnan(CReal) ||
               hc::fast_math::isinf(static_cast<float>(CReal)))
                  ? 0
                  : CReal;
      CImg = (hc::fast_math::isnan(CImg) ||
              hc::fast_math::isinf(static_cast<float>(CImg)))
                 ? 0
                 : CImg;
      if (!alpha.x && !alpha.y) {
        if (!beta.x && !beta.y) {
          C[cOffset + (tidx.global[0] * ldc) + tidx.global[1]].x = 0.0;
          C[cOffset + (tidx.global[0] * ldc) + tidx.global[1]].y = 0.0;
        } else {
          C[cOffset + (tidx.global[0] * ldc) + tidx.global[1]].x =
              ((CReal * beta.x) - (CImg * beta.y));
          C[cOffset + (tidx.global[0] * ldc) + tidx.global[1]].y =
              ((CReal * beta.y) + (CImg * beta.x));
        }
      }
    }
  }) _WAIT1;
#undef THREADS
#undef TILE_DIM
  return HCBLAS_SUCCEEDS;
}

hcblasStatus cgemm_alpha0_colbatch(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A[],
    __int64_t aOffset, __int64_t A_batchOffset, hc::short_vector::float_2 *B[],
    __int64_t bOffset, __int64_t B_batchOffset, hc::short_vector::float_2 *C[],
    __int64_t cOffset, __int64_t C_batchOffset, int M, int N, int K, int lda,
    int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta, int batchSize) {
#define THREADS 16
#define TILE_DIM 16
  hc::extent<3> grdExt(batchSize, (N + (THREADS - 1)) & ~(THREADS - 1),
                       (M + (THREADS - 1)) & ~(THREADS - 1));
  hc::tiled_extent<3> t_ext = grdExt.tile(1, THREADS, THREADS);
  hc::parallel_for_each(accl_view, t_ext, [=](hc::tiled_index<3> tidx)[[hc]] {
    int elt = tidx.tile[0];
    int Row = tidx.tile[1] * TILE_DIM + tidx.local[1];
    int Col = tidx.tile[2] * TILE_DIM + tidx.local[2];
    float CReal = 0.0;
    float CImg = 0.0;
    if (Row < N && Col < M) {
      CReal = C[elt][cOffset + (tidx.global[1] * ldc) + tidx.global[2]].x;
      CImg = C[elt][cOffset + (tidx.global[1] * ldc) + tidx.global[2]].y;
      CReal = (hc::fast_math::isnan(CReal) ||
               hc::fast_math::isinf(static_cast<float>(CReal)))
                  ? 0
                  : CReal;
      CImg = (hc::fast_math::isnan(CImg) ||
              hc::fast_math::isinf(static_cast<float>(CImg)))
                 ? 0
                 : CImg;
      if (!alpha.x && !alpha.y) {
        if (!beta.x && !beta.y) {
          C[elt][cOffset + (tidx.global[1] * ldc) + tidx.global[2]].x = 0.0;
          C[elt][cOffset + (tidx.global[1] * ldc) + tidx.global[2]].y = 0.0;
        } else {
          C[elt][cOffset + (tidx.global[1] * ldc) + tidx.global[2]].x =
              ((CReal * beta.x) - (CImg * beta.y));
          C[elt][cOffset + (tidx.global[1] * ldc) + tidx.global[2]].y =
              ((CReal * beta.y) + (CImg * beta.x));
        }
      }
    }
  }) _WAIT1;
#undef THREADS
#undef TILE_DIM
  return HCBLAS_SUCCEEDS;
}

hcblasStatus cgemm_alpha0_row(hc::accelerator_view accl_view,
                              hc::short_vector::float_2 *A, __int64_t aOffset,
                              hc::short_vector::float_2 *B, __int64_t bOffset,
                              hc::short_vector::float_2 *C, __int64_t cOffset,
                              int M, int N, int K, int lda, int ldb, int ldc,
                              hc::short_vector::float_2 alpha,
                              hc::short_vector::float_2 beta) {
#define THREADS 16
#define TILE_DIM 16
  hc::extent<2> grdExt((M + (THREADS - 1)) & ~(THREADS - 1),
                       (N + (THREADS - 1)) & ~(THREADS - 1));
  hc::tiled_extent<2> t_ext = grdExt.tile(THREADS, THREADS);
  hc::parallel_for_each(accl_view, t_ext, [=](hc::tiled_index<2> tidx)[[hc]] {
    int Row = tidx.tile[1] * TILE_DIM + tidx.local[1];
    int Col = tidx.tile[0] * TILE_DIM + tidx.local[0];
    float CReal = 0.0;
    float CImg = 0.0;
    if (Row < N && Col < M) {
      CReal = C[cOffset + tidx.global[1] + (tidx.global[0] * ldc)].x;
      CImg = C[cOffset + tidx.global[1] + (tidx.global[0] * ldc)].y;
      CReal = (hc::fast_math::isnan(CReal) ||
               hc::fast_math::isinf(static_cast<float>(CReal)))
                  ? 0
                  : CReal;
      CImg = (hc::fast_math::isnan(CImg) ||
              hc::fast_math::isinf(static_cast<float>(CImg)))
                 ? 0
                 : CImg;
      if (!alpha.x && !alpha.y) {
        if (!beta.x && !beta.y) {
          C[cOffset + tidx.global[1] + (tidx.global[0] * ldc)].x = 0.0;
          C[cOffset + tidx.global[1] + (tidx.global[0] * ldc)].y = 0.0;
        } else {
          C[cOffset + tidx.global[1] + (tidx.global[0] * ldc)].x =
              ((CReal * beta.x) - (CImg * beta.y));
          C[cOffset + tidx.global[1] + (tidx.global[0] * ldc)].y =
              ((CReal * beta.y) + (CImg * beta.x));
        }
      }
    }
  }) _WAIT1;
#undef THREADS
#undef TILE_DIM
  return HCBLAS_SUCCEEDS;
}

hcblasStatus cgemm_alpha0_rowbatch(
    hc::accelerator_view accl_view, hc::short_vector::float_2 *A[],
    __int64_t aOffset, __int64_t A_batchOffset, hc::short_vector::float_2 *B[],
    __int64_t bOffset, __int64_t B_batchOffset, hc::short_vector::float_2 *C[],
    __int64_t cOffset, __int64_t C_batchOffset, int M, int N, int K, int lda,
    int ldb, int ldc, hc::short_vector::float_2 alpha,
    hc::short_vector::float_2 beta, int batchSize) {
#define THREADS 16
#define TILE_DIM 16
  hc::extent<3> grdExt(batchSize, (M + (THREADS - 1)) & ~(THREADS - 1),
                       (N + (THREADS - 1)) & ~(THREADS - 1));
  hc::tiled_extent<3> t_ext = grdExt.tile(1, THREADS, THREADS);
  hc::parallel_for_each(accl_view, t_ext, [=](hc::tiled_index<3> & tidx)[[hc]] {
    int elt = tidx.tile[0];
    int Row = tidx.tile[2] * TILE_DIM + tidx.local[2];
    int Col = tidx.tile[1] * TILE_DIM + tidx.local[1];
    float CReal = 0.0;
    float CImg = 0.0;
    if (Row < N && Col < M) {
      CReal = C[elt][cOffset + tidx.global[2] + (tidx.global[1] * ldc)].x;
      CImg = C[elt][cOffset + tidx.global[2] + (tidx.global[1] * ldc)].y;
      CReal = (hc::fast_math::isnan(CReal) ||
               hc::fast_math::isinf(static_cast<float>(CReal)))
                  ? 0
                  : CReal;
      CImg = (hc::fast_math::isnan(CImg) ||
              hc::fast_math::isinf(static_cast<float>(CImg)))
                 ? 0
                 : CImg;
      if (!alpha.x && !alpha.y) {
        if (!beta.x && !beta.y) {
          C[elt][cOffset + tidx.global[2] + (tidx.global[1] * ldc)].x = 0.0;
          C[elt][cOffset + tidx.global[2] + (tidx.global[1] * ldc)].y = 0.0;
        } else {
          C[elt][cOffset + tidx.global[2] + (tidx.global[1] * ldc)].x =
              ((CReal * beta.x) - (CImg * beta.y));
          C[elt][cOffset + tidx.global[2] + (tidx.global[1] * ldc)].y =
              ((CReal * beta.y) + (CImg * beta.x));
        }
      }
    }
  }) _WAIT1;
#undef THREADS
#undef TILE_DIM
  return HCBLAS_SUCCEEDS;
}

// CGEMM Call Type I: Inputs and outputs are C++ HC float array containers
hcblasStatus Hcblaslibrary::hcblas_cgemm(
    hc::accelerator_view accl_view, hcblasOrder order, hcblasTranspose typeA,
    hcblasTranspose typeB, const int M, const int N, const int K,
    const hc::short_vector::float_2 &Calpha, hc::short_vector::float_2 *Acmplx,
    __int64_t aOffset, __int64_t lda, hc::short_vector::float_2 *Bcmplx,
    __int64_t bOffset, __int64_t ldb, const hc::short_vector::float_2 &Cbeta,
    hc::short_vector::float_2 *Ccmplx, __int64_t cOffset, __int64_t ldc) {
  int i, j;
  hcblasStatus status = HCBLAS_SUCCEEDS;
  float tempReal = 0.0, tempImg = 0.0;
  // Quick return if possible
  if (Acmplx == NULL || Bcmplx == NULL || Ccmplx == NULL || !M || !N || !K) {
    return HCBLAS_INVALID;
  }

  if (!Calpha.x && !Calpha.y) {
    if (order)
      status =
          cgemm_alpha0_col(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx,
                           cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    else
      status =
          cgemm_alpha0_row(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx,
                           cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    return status;
  }

  if (order) {
    if (typeB == NoTrans) {
      if (typeA == NoTrans) {
        status =
            cgemm_NoTransAB(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx,
                            cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
      } else {
        status =
            cgemm_NoTransB(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx,
                           cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
      }
    } else if (typeA == NoTrans) {
      status =
          cgemm_NoTransA(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx,
                         cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    } else {
      status =
          cgemm_TransAB(accl_view, Acmplx, aOffset, Bcmplx, bOffset, Ccmplx,
                        cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    }
  } else {
    if (typeB == NoTrans) {
      if (typeA == NoTrans) {
        status = cgemm_NoTransAB_rMajor(accl_view, Acmplx, aOffset, Bcmplx,
                                        bOffset, Ccmplx, cOffset, M, N, K, lda,
                                        ldb, ldc, Calpha, Cbeta);
      } else {
        status = cgemm_NoTransB_rMajor(accl_view, Acmplx, aOffset, Bcmplx,
                                       bOffset, Ccmplx, cOffset, M, N, K, lda,
                                       ldb, ldc, Calpha, Cbeta);
      }
    } else if (typeA == NoTrans) {
      status = cgemm_NoTransA_rMajor(accl_view, Acmplx, aOffset, Bcmplx,
                                     bOffset, Ccmplx, cOffset, M, N, K, lda,
                                     ldb, ldc, Calpha, Cbeta);
    } else {
      status = cgemm_TransAB_rMajor(accl_view, Acmplx, aOffset, Bcmplx, bOffset,
                                    Ccmplx, cOffset, M, N, K, lda, ldb, ldc,
                                    Calpha, Cbeta);
    }
  }

  return status;
}

/* CGEMM Call Type II - Overloaded function with arguments related to batch
 * processing */
hcblasStatus Hcblaslibrary::hcblas_cgemm(
    hc::accelerator_view accl_view, hcblasOrder order, hcblasTranspose typeA,
    hcblasTranspose typeB, const int M, const int N, const int K,
    const hc::short_vector::float_2 &Calpha,
    hc::short_vector::float_2 *Acmplx[], const __int64_t aOffset,
    const __int64_t A_batchOffset, const __int64_t lda,
    hc::short_vector::float_2 *Bcmplx[], const __int64_t bOffset,
    const __int64_t B_batchOffset, const __int64_t ldb,
    const hc::short_vector::float_2 &Cbeta, hc::short_vector::float_2 *Ccmplx[],
    const __int64_t cOffset, const __int64_t C_batchOffset, const __int64_t ldc,
    const int batchSize) {
  int i, j, k;
  hcblasStatus status = HCBLAS_SUCCEEDS;
  float tempReal = 0.0, tempImg = 0.0;
  // Quick return if possible
  if (Acmplx == NULL || Bcmplx == NULL || Ccmplx == NULL || !M || !N || !K) {
    return HCBLAS_INVALID;
  }

  if (!Calpha.x && !Calpha.y) {
    if (order)
      status = cgemm_alpha0_colbatch(accl_view, Acmplx, aOffset, A_batchOffset,
                                     Bcmplx, bOffset, B_batchOffset, Ccmplx,
                                     cOffset, C_batchOffset, M, N, K, lda, ldb,
                                     ldc, Calpha, Cbeta, batchSize);
    else
      status = cgemm_alpha0_rowbatch(accl_view, Acmplx, aOffset, A_batchOffset,
                                     Bcmplx, bOffset, B_batchOffset, Ccmplx,
                                     cOffset, C_batchOffset, M, N, K, lda, ldb,
                                     ldc, Calpha, Cbeta, batchSize);
    return status;
  }

  if (order) {
    if (typeB == NoTrans) {
      if (typeA == NoTrans) {
        status = cgemm_NoTransAB(accl_view, Acmplx, aOffset, A_batchOffset,
                                 Bcmplx, bOffset, B_batchOffset, Ccmplx,
                                 cOffset, C_batchOffset, M, N, K, lda, ldb, ldc,
                                 Calpha, Cbeta, batchSize);
      } else {
        status = cgemm_NoTransB(accl_view, Acmplx, aOffset, A_batchOffset,
                                Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset,
                                C_batchOffset, M, N, K, lda, ldb, ldc, Calpha,
                                Cbeta, batchSize);
      }
    } else if (typeA == NoTrans) {
      status =
          cgemm_NoTransA(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx,
                         bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset,
                         M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
    } else {
      status =
          cgemm_TransAB(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx,
                        bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset,
                        M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
    }
  } else {
    if (typeB == NoTrans) {
      if (typeA == NoTrans) {
        status = cgemm_NoTransAB_rMajor(
            accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset,
            B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb,
            ldc, Calpha, Cbeta, batchSize);
      } else {
        status = cgemm_NoTransB_rMajor(
            accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset,
            B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb,
            ldc, Calpha, Cbeta, batchSize);
      }
    } else if (typeA == NoTrans) {
      status = cgemm_NoTransA_rMajor(accl_view, Acmplx, aOffset, A_batchOffset,
                                     Bcmplx, bOffset, B_batchOffset, Ccmplx,
                                     cOffset, C_batchOffset, M, N, K, lda, ldb,
                                     ldc, Calpha, Cbeta, batchSize);
    } else {
      status = cgemm_TransAB_rMajor(accl_view, Acmplx, aOffset, A_batchOffset,
                                    Bcmplx, bOffset, B_batchOffset, Ccmplx,
                                    cOffset, C_batchOffset, M, N, K, lda, ldb,
                                    ldc, Calpha, Cbeta, batchSize);
    }
  }

  return status;
}

