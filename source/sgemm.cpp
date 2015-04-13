#include "ampblaslib.h"
#include <amp.h>
#include <amp_math.h>
using namespace Concurrency;

#define THREADS    16
#define GEMM_BLOCK 256
#define TILE_DIM  16

#define TILE_SZ_A 64
#define TILE_SZ_B 16
#define TILE_SZ_RATIO (TILE_SZ_A/TILE_SZ_B)
#define TILESIZE 16
#define STEPSIZE 128 
#define MICROTILESIZE 1

#define  M1x1(offset)			\
            rA[0][0] = lA[offA + 0];	\
            rB[0][0] = lB[offB + 0];	\
            offA += offset;			\
            offB += offset;			\
            rC[0][0]=rA[0][0] *rB[0][0] + rC[0][0]; \	

#define  MS1x1(offset)			\
            for(int iter = 0; iter < STEPSIZE/TILESIZE; ++iter) \
            {\
              rA[0][iter] = lA[offA + (TILESIZE * TILESIZE) * iter];	\
              rB[0][iter] = lB[offB + (TILESIZE * TILESIZE) * iter];	\
              rC[0][0] +=rA[0][iter] *rB[0][iter]; \
            }\
            offA += offset;			\
            offB += offset;			\

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

static void gemm_NoTransAB_batch(Concurrency::array_view<float, 1> &A, long aOffset,
                                 Concurrency::array_view<float, 1> &B, long bOffset,
                                 Concurrency::array_view<float, 1> &C, long cOffset,
                                 int M, int N, int K, int lda, int ldb, int ldc,
                                 float alpha, float beta)
{
  Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
    float rC[1][1];
    float rA[1][STEPSIZE/TILESIZE];
    float rB[1][STEPSIZE/TILESIZE];
    tile_static float lA[TILESIZE * MICROTILESIZE * STEPSIZE];//8*8+8
    tile_static float lB[TILESIZE * MICROTILESIZE * STEPSIZE];
    rC[0][0] = 0;
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = TILESIZE * idy + idx;
    int idxT = idt % TILESIZE;
    int idyT = idt / TILESIZE;
    int block_k = ((K + (STEPSIZE - 1)) & ~(STEPSIZE - 1)) >> shiftFactor;
    int i = 0;
    do
    {
      tidx.barrier.wait();

      // Load Sections of A and B into respective shared memory slots
      for (int sec =0; sec < STEPSIZE/TILESIZE; ++sec)
      {
        // Load Section 'sec' from global memory B onto shared lB
        if(gidy*TILESIZE+idxT  < N && (idyT + i * STEPSIZE + (TILESIZE * sec)) < K) 
          lB[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = B[bOffset + (gidy*TILESIZE+ idxT) * ldb + idyT + i * STEPSIZE + (TILESIZE * sec)];
        else
          lB[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = 0;

        // Load Section 'sec' from global memory A onto shared lA
        if(gidx * TILESIZE + idxT < M && (i * STEPSIZE + idyT + (TILESIZE * sec)) < K)
           lA[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = A[aOffset  + gidx*TILESIZE+ idxT + idyT*lda + i * (lda << shiftFactor) + (TILESIZE * sec) * lda];
        else
           lA[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = 0;
      }
      tidx.barrier.wait();

      int offA = idx * TILESIZE;
      int offB = idy * TILESIZE;
      int offset = 1;

      for (int iter=0; iter < TILESIZE; ++iter)
      {
         MS1x1(offset);
      }

      i++;
    } while (--block_k > 0);


    tidx.barrier.wait();
    if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N)
        C[cOffset + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc] = alpha * rC[0][0] + beta * C[cOffset + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc];
  });

}

static void gemm_NoTransB(Concurrency::array_view<float, 1> &A, long aOffset,
                          Concurrency::array_view<float, 1> &B, long bOffset,
                          Concurrency::array_view<float, 1> &C, long cOffset,
                          int M, int N, int K, int lda, int ldb, int ldc,
                          float alpha, float beta,
                          Concurrency::array_view<float,1> &temp_buf)
{
    Concurrency::extent<2> grdExt(((N - 1) / TILE_SZ_A + 1) * TILE_SZ_A, (M - 1) / TILE_SZ_B + 1);
    Concurrency::tiled_extent <TILE_SZ_A, 1> t_ext(grdExt);

    Concurrency::parallel_for_each(t_ext,
                                   [=] (Concurrency::tiled_index<TILE_SZ_A,1> tidx)
                                   restrict(amp) {

    // Shared memory for tiling input B array
    tile_static float B_s [TILE_SZ_RATIO][TILE_SZ_B];

    // Macros for accessing flattened matrices
    #define A1(row,col) A[(row) * K + (col)]
    #define B1(row,col) B[(row) + (col) * K]
    #define C1(row,col) C[(row) + (col) * M]

    // Index variables
    const unsigned int row = tidx.global[0];
    const unsigned int col = tidx.tile[1] * TILE_SZ_B;

    // Privatization of output variables
    float c_reg[TILE_SZ_B];

    // Initialize output values
    c_reg[tidx.local[0] & (TILE_SZ_B-1)] = 0;

    // Loop over the input tiles
    for(unsigned int tileIdx = 0; tileIdx < (K - 1)/TILE_SZ_RATIO + 1; ++tileIdx) {
        // Load the tile of B into shared memory
        const unsigned int i = tidx.local[0]/TILE_SZ_B;
        const unsigned int j = tidx.local[0]%TILE_SZ_B;

        if (tileIdx*TILE_SZ_RATIO + i < K && col + j < N) {
            B_s[i][j] = B1(tileIdx*TILE_SZ_RATIO + i, col + j);
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
                a_reg = A1(row, tileIdx*TILE_SZ_RATIO + idx);
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
            (C1(row, col + outIdx) *= beta) += (c_reg[outIdx] *= alpha);
        }
    }
});
}

static void gemm_NoTransB_batch(Concurrency::array_view<float, 1> &A, long aOffset,
                                Concurrency::array_view<float, 1> &B, long bOffset,
                                Concurrency::array_view<float, 1> &C, long cOffset,
                                int M, int N, int K, int lda, int ldb, int ldc,
                                float alpha, float beta)
{
  Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    int shiftFactor = Concurrency::fast_math::log2(TILESIZE);
    float rC[1][1];
    float rA[1][1];
    float rB[1][1];
    tile_static float lA[TILESIZE * MICROTILESIZE * TILESIZE];//8*8+8
    tile_static float lB[TILESIZE * MICROTILESIZE * TILESIZE];
    rC[0][0] = 0;
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = TILESIZE*idy + idx;
    int idxT = idt % TILESIZE;
    int idyT = idt / TILESIZE;
    int block_k =( (K + (TILESIZE - 1)) & ~(TILESIZE - 1)) >> shiftFactor;

    int i = 0;
    do
    {
      //barrier(CLK_LOCAL_MEM_FENCE);
      tidx.barrier.wait();
      if(gidy*TILESIZE+idxT < N && i*TILESIZE+idyT < K)
      {
        lB[idyT+idxT*TILESIZE] = B[bOffset + (gidy*TILESIZE+ idxT)*ldb + idyT + i * TILESIZE];
      }
      else
        lB[idyT+idxT*TILESIZE] = 0;
      if(gidx*TILESIZE+idxT < M && i*TILESIZE+idyT < K)
      {
        lA[idyT+idxT*TILESIZE] = A[aOffset  + (gidx*TILESIZE+ idxT)*lda + idyT + i * TILESIZE];
      }
      else
      {
        lA[idyT+idxT*TILESIZE] = 0;
      }
      tidx.barrier.wait();

      //barrier(CLK_LOCAL_MEM_FENCE);
      int offA = idx*TILESIZE;
      int offB = idy*TILESIZE;
      int offset = 1;

      for(int iter = 0; iter < TILESIZE; ++iter)
      {
            M1x1(offset);
      }
        

      i++;
    } while (--block_k > 0);

    tidx.barrier.wait();
    if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N)
        C[cOffset + (gidx*TILESIZE +idx) + (gidy*TILESIZE + idy)*ldc] = alpha*rC[0][0] + beta*C[cOffset + (gidx*TILESIZE+idx) + (gidy*TILESIZE + idy)*ldc];

  });

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

    c_reg[tidx.local[0] & (TILE_SZ_B-1)] = 0;

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

static void gemm_TransAB_batch(Concurrency::array_view<float, 1> &A, long aOffset,
                               Concurrency::array_view<float, 1> &B, long bOffset,
                               Concurrency::array_view<float, 1> &C, long cOffset,
                               int M, int N, int K, int lda, int ldb, int ldc,
                               float alpha, float beta)
{
  Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    int shiftFactor = Concurrency::fast_math::log2(TILESIZE);
    float rC[1][1];
    float rA[1][1];
    float rB[1][1];
    tile_static float lA[TILESIZE * MICROTILESIZE * TILESIZE];
    tile_static float lB[TILESIZE * MICROTILESIZE * TILESIZE];
    rC[0][0] = 0;
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = TILESIZE*idy + idx;
    int idxT = idt % TILESIZE;
    int idyT = idt / TILESIZE;
    int block_k = ((K + (TILESIZE - 1)) & ~(TILESIZE - 1)) >> shiftFactor;
    int i = 0;
    do
    {
      tidx.barrier.wait();
      if(gidy*TILESIZE+idxT < N && i*TILESIZE+idyT < K)
      {
        lB[idyT*TILESIZE+idxT] = B[bOffset + gidy*TILESIZE+ idxT + idyT*ldb + i * (ldb << shiftFactor)];
      }
      else
        lB[idyT*TILESIZE+idxT] = 0;

      if(gidx*TILESIZE+idxT < M && i*TILESIZE+idyT < K)
      {
        lA[idyT*TILESIZE+idxT] = A[aOffset  + (gidx*TILESIZE+ idxT)*lda + idyT + i * TILESIZE];
      }
      else
      {
        lA[idyT*TILESIZE+idxT] = 0;
      }
      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;

      for(int iter = 0; iter < TILESIZE; iter++)
      {
         M1x1(TILESIZE);
      }

      i++;
    } while (--block_k > 0);


    tidx.barrier.wait();
    if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N)
        C[cOffset + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc] = alpha * rC[0][0] + beta * C[cOffset + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc];
  });
}

static void gemm_NoTransA_batch(Concurrency::array_view<float, 1> &A, long aOffset,
                                Concurrency::array_view<float, 1> &B, long bOffset,
                                Concurrency::array_view<float, 1> &C, long cOffset,
                                int M, int N, int K, int lda, int ldb, int ldc,
                                float alpha, float beta)
{
  Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);
 
  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  { 
    int shiftFactor = Concurrency::fast_math::log2(TILESIZE);
    float rC[1][1];
    float rA[1][1];
    float rB[1][1];
    tile_static float lA[TILESIZE * MICROTILESIZE * TILESIZE];//8*8+8
    tile_static float lB[TILESIZE * MICROTILESIZE * TILESIZE];
    rC[0][0] = 0;
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = TILESIZE * idy + idx;
    int idxT = idt % TILESIZE;
    int idyT = idt / TILESIZE;
    int block_k = ((K + (TILESIZE - 1)) & ~(TILESIZE - 1)) >> shiftFactor;
    int i = 0;
    do
    {
      //barrier(CLK_LOCAL_MEM_FENCE);
      tidx.barrier.wait();
      if(gidy*TILESIZE+idxT < N && i*TILESIZE+idyT < K)
      {
        lB[idyT*TILESIZE+idxT] = B[bOffset + gidy*TILESIZE+ idxT + idyT*ldb + i * (ldb << shiftFactor)];
      }
      else
        lB[idyT*TILESIZE+idxT] = 0;
      if(gidx*TILESIZE+idxT < M && i*TILESIZE+idyT < K)
      {
        lA[idyT*TILESIZE+idxT] = A[aOffset  + gidx*TILESIZE+ idxT + idyT*lda + i * (lda << shiftFactor)];
      }
      else
      {
        lA[idyT*TILESIZE+idxT] = 0;
      }
      tidx.barrier.wait();

      //barrier(CLK_LOCAL_MEM_FENCE);
      int offA = idx;
      int offB = idy;
   
      for (int iter =0; iter < TILESIZE; ++iter)
      {
        M1x1(TILESIZE);
      }

      i++;
    } while (--block_k > 0);


    tidx.barrier.wait();
    if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N)
        C[cOffset + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc] = alpha * rC[0][0] + beta * C[cOffset + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc];

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
      gemm_NoTransAB_batch(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
    else
      gemm_NoTransB_batch(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else if (TransA == 'n') 
    gemm_NoTransA_batch(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  else
    gemm_TransAB_batch(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);

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
    /*for (int i = 0; i < M * N; i++)
        cout<<" C_mat["<<i<<"] "<<C_mat[i]<<endl;*/

    return AMPBLAS_SUCCESS;
}
