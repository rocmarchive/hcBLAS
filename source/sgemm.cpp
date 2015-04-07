#include "ampblaslib.h"
#include <amp.h>

using namespace Concurrency;

#define THREADS    16
#define GEMM_BLOCK 256
#define TILE_DIM  16

#define TILE_SZ_A 64
#define TILE_SZ_B 32
#define TILE_SZ_RATIO (TILE_SZ_A/TILE_SZ_B)

static void gemm_NoTransAB(Concurrency::array_view<float, 1> &A, long aOffset,
                           Concurrency::array_view<float, 1> &B, long bOffset,
                           Concurrency::array_view<float, 1> &C, long cOffset,
                           int M, int N, int K, int lda, int ldb, int ldc,
                           float alpha, float beta)
{
  Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1),(M + (THREADS-1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float CValue = 0;
    int Row = tidx.tile[0] * TILE_DIM + tidx.local[0];
    int Col = tidx.tile[1] * TILE_DIM + tidx.local[1];
    tile_static float As[TILE_DIM][TILE_DIM];
    tile_static float Bs[TILE_DIM][TILE_DIM];

    for (int k = 0; k < (TILE_DIM + K - 1) / TILE_DIM; k++)
    {
      // Read Matrix B from global to shared tile
      if (k * TILE_DIM + tidx.local[1] < K && Row < N)
        Bs[tidx.local[0]][tidx.local[1]] = B[bOffset + Row * K + k * TILE_DIM + tidx.local[1]];
      else
        Bs[tidx.local[0]][tidx.local[1]] = 0.0;

      // Read Matrix A from global to shared tile
      if (k*TILE_DIM + tidx.local[0] < K && Col < M)
        As[tidx.local[0]][tidx.local[1]] = A[aOffset + (k * TILE_DIM + tidx.local[0]) * M + Col];
      else
        As[tidx.local[0]][tidx.local[1]] = 0.0;

      tidx.barrier.wait();

      for (int n = 0; n < TILE_DIM; ++n)
        CValue += Bs[tidx.local[0]][n] * As[n][tidx.local[1]] * alpha;

      tidx.barrier.wait();
    }

    if (Row < N && Col < M)
    {
      C[cOffset + (tidx.global[0] * M) + tidx.global[1]] *= beta;
      C[cOffset + (tidx.global[0] * M) + tidx.global[1]] += CValue;
    }

  });
}

static void gemm_NoTransB(Concurrency::array_view<float, 1> &A, long aOffset,
                          Concurrency::array_view<float, 1> &B, long bOffset,
                          Concurrency::array_view<float, 1> &C, long cOffset,
                          int M, int N, int K, int lda, int ldb, int ldc,
                          float alpha, float beta,
                          Concurrency::array_view<float,1> &temp_buf)
{
  if (K > N && K > M)
  {
    Concurrency::extent<2> grdExt(N, M * GEMM_BLOCK);
    Concurrency::tiled_extent<1, GEMM_BLOCK> t_ext(grdExt);

    Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<1, GEMM_BLOCK> tidx) restrict(amp)
    {
      int threadIdx = tidx.local[1];
      int blockIdx = tidx.tile[1];
      int Row = tidx.tile[0];
      int Col = blockIdx;

      tile_static float sh[GEMM_BLOCK];
      sh[threadIdx] = 0;

      for (int tileId = 0; tileId < ((K + GEMM_BLOCK - 1) & ~(GEMM_BLOCK - 1)) / GEMM_BLOCK; tileId++)
      {
        if (tileId * GEMM_BLOCK + threadIdx < K && Col < M && Row < N)
          sh[threadIdx] += A[aOffset + Col * K + tileId * GEMM_BLOCK + threadIdx] * B[bOffset + Row * K + tileId * GEMM_BLOCK + threadIdx];
      }
      tidx.barrier.wait();

      for (int stride = GEMM_BLOCK / 2; stride >= 1; stride /= 2)
      {
        if (threadIdx < stride)
          sh[threadIdx] += sh[threadIdx + stride];
        tidx.barrier.wait();
      }

      if(threadIdx == 0 && Col < M && Row < N)
      {
        C[cOffset + Row * M + Col] *= beta;
        C[cOffset + Row * M + Col] += sh[0] * alpha;
      }
    });
  }
  else
  {
    Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1), (M + (THREADS - 1)) & ~(THREADS - 1));
    Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);
    Concurrency::array_view<float,2> Cmat = C.view_as<2>(Concurrency::extent<2>(N, M));
    Cmat.discard_data();
    Concurrency::array_view<float,2> Amat = A.view_as<2>(Concurrency::extent<2>(M, K));
    Amat.discard_data();
    Concurrency::array_view<float,2> Bmat = B.view_as<2>(Concurrency::extent<2>(N, K));
    Bmat.discard_data();

    Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
    {
      float CValue = 0;
      int Row = tidx.global[0];
      int Col = tidx.global[1];
      tile_static float As[TILE_DIM][TILE_DIM];
      tile_static float Bs[TILE_DIM][TILE_DIM];

      for (int k = 0; k < ((K + (TILE_DIM - 1)) & ~(TILE_DIM - 1)) ; k += TILE_DIM)
      {
        if (k + tidx.local[1] < K && Row < N)
          Bs[tidx.local[0]][tidx.local[1]] = Bmat[Row][bOffset + k + tidx.local[1]];
        else
          Bs[tidx.local[0]][tidx.local[1]] = 0.0;

        if (k + tidx.local[1] < K && (tidx.tile[1] * TILE_DIM + tidx.local[0]) < M)
          As[tidx.local[0]][tidx.local[1]] = Amat[(tidx.tile[1] * TILE_DIM + tidx.local[0])] [aOffset + k + tidx.local[1]];
        else
          As[tidx.local[0]][tidx.local[1]] = 0.0;

        tidx.barrier.wait();

        // Unrolled Matrix Mul operation
        CValue += Bs[tidx.local[0]][0] * As[tidx.local[1]][0] +
                  Bs[tidx.local[0]][1] * As[tidx.local[1]][1] +
                  Bs[tidx.local[0]][2] * As[tidx.local[1]][2] +
                  Bs[tidx.local[0]][3] * As[tidx.local[1]][3] +
                  Bs[tidx.local[0]][4] * As[tidx.local[1]][4] +
                  Bs[tidx.local[0]][5] * As[tidx.local[1]][5] +
                  Bs[tidx.local[0]][6] * As[tidx.local[1]][6] +
                  Bs[tidx.local[0]][7] * As[tidx.local[1]][7] +
                  Bs[tidx.local[0]][8] * As[tidx.local[1]][8] +
                  Bs[tidx.local[0]][9] * As[tidx.local[1]][9] +
                  Bs[tidx.local[0]][10] * As[tidx.local[1]][10] +
                  Bs[tidx.local[0]][11] * As[tidx.local[1]][11] +
                  Bs[tidx.local[0]][12] * As[tidx.local[1]][12] +
                  Bs[tidx.local[0]][13] * As[tidx.local[1]][13] +
                  Bs[tidx.local[0]][14] * As[tidx.local[1]][14] +
                  Bs[tidx.local[0]][15] * As[tidx.local[1]][15];

        tidx.barrier.wait();
      }
      if (Row < N && Col < M)
      {
        Cmat[Row][cOffset + Col] *= beta;
        Cmat[Row][cOffset + Col] += CValue * alpha;
      }
    });
  }
}

static void gemm_NoTransA(Concurrency::array_view<float, 1> &A, long aOffset,
                          Concurrency::array_view<float, 1> &B, long bOffset,
                          Concurrency::array_view<float, 1> &C, long cOffset,
                          int M, int N, int K, int lda, int ldb, int ldc,
                          float alpha, float beta)
{
    Concurrency::extent<2> grdExt(((M - 1) / TILE_SZ_A + 1) * TILE_SZ_A, (N - 1) / TILE_SZ_B + 1);
    Concurrency::tiled_extent <TILE_SZ_A, 1> t_ext(grdExt);

    Concurrency::parallel_for_each(t_ext,
                                   [=] (Concurrency::tiled_index<TILE_SZ_A,1> tidx)
                                   restrict(amp) {

    // Shared memory for tiling input B array
    tile_static float B_s [TILE_SZ_RATIO][TILE_SZ_B];

    // Macros for accessing flattened matrices
    #define A(row,col) A[(row) + (col) * M]
    #define B(row,col) B[(row) * N + (col)]
    #define C(row,col) C[(row) + (col) * M]

    // Index variables
    const unsigned int row = tidx.global[0];
    const unsigned int col = tidx.tile[1] * TILE_SZ_B;

    // Privatization of output variables
    float c_reg[TILE_SZ_B];

    // Initialize output values
    for(unsigned int outIdx = 0; outIdx < TILE_SZ_B; ++outIdx) {
        c_reg[outIdx] = 0;
    }

    // Loop over the input tiles
    for(unsigned int tileIdx = 0; tileIdx < (K - 1)/TILE_SZ_RATIO + 1; ++tileIdx) {
        // Load the tile of B into shared memory
        const unsigned int i = tidx.local[0]/TILE_SZ_B;
        const unsigned int j = tidx.local[0]%TILE_SZ_B;

        if (tileIdx*TILE_SZ_RATIO + i < K && col + j < N) {
            B_s[i][j] = B(tileIdx*TILE_SZ_RATIO + i, col + j);
        }
        else {
            B_s[i][j] = 0;
        }
        tidx.barrier.wait();

        // Loop over elements inside the tile
        for (unsigned int idx = 0; idx < TILE_SZ_RATIO; ++idx) {
            // Load tile of A matrix into register
            float a_reg;
            if(row < M && tileIdx*TILE_SZ_RATIO + idx < K) {
                a_reg = A(row, tileIdx*TILE_SZ_RATIO + idx);
            }
            else {
                a_reg = 0;
            }

            // Loop over and update the output elements assigned to the thread
            for(unsigned int outIdx = 0; outIdx < TILE_SZ_B; ++outIdx) {
                c_reg[outIdx] += a_reg*B_s[idx][outIdx];
            }
        }
        tidx.barrier.wait();
    }
    for (unsigned int outIdx = 0; outIdx < TILE_SZ_B; ++outIdx) {
        if (row < M && col + outIdx < N) {
            (C(row, col + outIdx) *= beta) += (c_reg[outIdx] *= alpha);
        }
    }
});
}

static void gemm_TransAB(Concurrency::array_view<float, 1> &A, long aOffset,
                         Concurrency::array_view<float, 1> &B, long bOffset,
                         Concurrency::array_view<float, 1> &C, long cOffset,
                         int M, int N, int K, long lda, long ldb, long ldc,
                         float alpha, float beta)
{
  Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1), (M + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float temp;
    int j = tidx.global[0];
    int i = tidx.global[1];
    if(i < M && j < N)
    {
      temp = 0;
      for (int l = 0; l < K; ++l)
        temp += A[aOffset + l + i * lda] * B[bOffset + j + l * ldb];

      C[cOffset + i + j * ldc] = alpha * temp + beta * C[cOffset + i + j * ldc];
    }
  });
}

int gemm_AMP(char TransA, char TransB, const int M, const int N, const int K,
             const float alpha, Concurrency::array_view<float> &A_mat, long aOffset, long lda,
             Concurrency::array_view<float> &B_mat, long bOffset, long ldb, const float beta,
             Concurrency::array_view<float> &C_mat, long cOffset, long ldc,
             Concurrency::array_view<float> &temp_buf)
{
  int i, j;

  // Quick return if possible
  if (!M || !N || ((alpha == 0 || !K) && beta == 1))
    return 0;
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
    return 0;
  }
  // Start the operations
  if (TransB == 'n')
  {
    if (TransA == 'n')
      gemm_NoTransAB(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
    else
      gemm_NoTransB(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta, temp_buf);
  }
  else if (TransA == 'n')
    gemm_NoTransA(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  else
    gemm_TransAB(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);

  return 0;
}

ampblasStatus Ampblaslibrary :: ampblas_sgemm(const enum AMPBLAS_TRANS typeA,
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

    gemm_AMP(typeA, typeB, M, N, K, *alpha, A_mat, aOffset, lda, B_mat,
             bOffset, ldb, *beta, C_mat, cOffset, ldc, *temp_buf);

    C_mat.synchronize();

    /* Print Output */
    for (int i = 0; i < M * N; i++)
        cout<<" C_mat["<<i<<"] "<<C_mat[i]<<endl;

    return AMPBLAS_SUCCESS;
}
