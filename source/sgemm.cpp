#include "ampblaslib.h"
#include <amp.h>
#include <amp_math.h>
using namespace Concurrency;

#define REGISTER 0
#define STEP 0
#define SUBMICROTILE 0
#define LOOPUNROLL 0
#define LOOPUNROLL_SWPREFETCH 1

#if SUBMICROTILE
#define NOTRANSAB 0
#define NOTRANSA 1
#define NOTRANSB 0
#define TRANSAB 0
#endif

#define THREADS    16
#define GEMM_BLOCK 256
#define TILE_DIM  16

#define TILE_SZ_A 32
#define TILE_SZ_B 16
#define TILE_SZ_RATIO (TILE_SZ_A/TILE_SZ_B)
#define TILESIZE 16
#define STEPSIZE 128 
#define MICROTILESIZE 2

#define  M1x1(offset)			\
            rA[0][0] = lA[offA + 0];	\
            rB[0][0] = lB[offB + 0];	\
            offA += offset;			\
            offB += offset;			\
            rC[0][0]=rA[0][0] *rB[0][0] + rC[0][0]; \	

#define  MS1x1(offsetA, offsetB)			\
            for(int iter = 0; iter < STEPSIZE/TILESIZE; ++iter) \
            {\
              rA[0][iter] = lA[offA + (TILESIZE * TILESIZE) * iter];	\
              rB[0][iter] = lB[offB + (TILESIZE * TILESIZE) * iter];	\
              rC[0][0] +=rA[0][iter] *rB[0][iter]; \
            }\
            offA += offsetA;			\
            offB += offsetB;			\

#define  MTS                                                                \
           for(int iter = 0; iter < MICROTILESIZE ; iter++)                 \
           {                                                                \
             rA[0][iter] = lA[offA + (iter * TILESIZE)];                    \
             rB[0][iter] = lB[offB + (iter * TILESIZE)];                    \
           }                                                                \
           for(int rowIndex = 0; rowIndex < MICROTILESIZE ; rowIndex++)     \
           {                                                                \
           for(int colIndex = 0; colIndex < MICROTILESIZE ; colIndex++)     \
           {                                                                \
           rC[rowIndex][colIndex] = rA[0][rowIndex] * rB[0][colIndex] +     \
                                    rC[rowIndex][colIndex];                 \
           }                                                                \
           }                                                                \
           offA += (MICROTILESIZE * TILESIZE);                              \
           offB += (MICROTILESIZE * TILESIZE);                              \

#if LOOPUNROLL_SWPREFETCH
static void gemm_NoTransAB_loopunroll_swprefetch(Concurrency::array_view<float, 1> &A, long aOffset,
                                                 Concurrency::array_view<float, 1> &B, long bOffset,
                                                 Concurrency::array_view<float, 1> &C, long cOffset,
                                                 int M, int N, int K, int lda, int ldb, int ldc,
                                                 float alpha, float beta)
{
  Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1), (M + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);
  Concurrency::array_view<float,2> Cmat = C.view_as<2>(Concurrency::extent<2>(N, M));
  Concurrency::array_view<float,2> Amat = A.view_as<2>(Concurrency::extent<2>(K, M));
  Concurrency::array_view<float,2> Bmat = B.view_as<2>(Concurrency::extent<2>(N, K));

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float CValue = 0;
    tile_static float As[2][TILE_DIM][TILE_DIM];
    tile_static float Bs[2][TILE_DIM][TILE_DIM];

    int kSize = ((K + (TILE_DIM - 1)) & ~(TILE_DIM - 1)) / TILE_DIM;
    int kNext = 0, kmod = 0;

    if (tidx.local[1] < K && tidx.global[0] < N)
      Bs[0][tidx.local[0]][tidx.local[1]] = Bmat[tidx.global[0]][bOffset + tidx.local[1]];
    else
      Bs[0][tidx.local[0]][tidx.local[1]] = 0.0;
    if (tidx.local[0] < K && tidx.global[1] < M)
      As[0][tidx.local[1]][tidx.local[0]] = Amat[aOffset + tidx.local[0]][tidx.global[1]];
    else
      As[0][tidx.local[1]][tidx.local[0]] = 0.0;

    for (int k = 0; k < kSize; k++)
    {
      kNext = k + 1;
      kmod = k & 1;

      tidx.barrier.wait();

      if (kNext * TILE_DIM + tidx.local[1] < K && tidx.global[0] < N)
        Bs[kNext & 1][tidx.local[0]][tidx.local[1]] = Bmat[tidx.global[0]][bOffset + kNext * TILE_DIM + tidx.local[1]];
      else
        Bs[kNext & 1][tidx.local[0]][tidx.local[1]] = 0.0;
      if (kNext * TILE_DIM + tidx.local[0] < K && tidx.global[1] < M)
        As[kNext & 1][tidx.local[1]][tidx.local[0]] = Amat[aOffset + kNext * TILE_DIM + tidx.local[0]][tidx.global[1]];
      else
        As[kNext & 1][tidx.local[1]][tidx.local[0]] = 0.0;

      // Unrolled Matrix Mul operation
      CValue += Bs[kmod][tidx.local[0]][0] * As[kmod][tidx.local[1]][0] +
                Bs[kmod][tidx.local[0]][1] * As[kmod][tidx.local[1]][1] +
                Bs[kmod][tidx.local[0]][2] * As[kmod][tidx.local[1]][2] +
                Bs[kmod][tidx.local[0]][3] * As[kmod][tidx.local[1]][3] +
                Bs[kmod][tidx.local[0]][4] * As[kmod][tidx.local[1]][4] +
                Bs[kmod][tidx.local[0]][5] * As[kmod][tidx.local[1]][5] +
                Bs[kmod][tidx.local[0]][6] * As[kmod][tidx.local[1]][6] +
                Bs[kmod][tidx.local[0]][7] * As[kmod][tidx.local[1]][7] +
                Bs[kmod][tidx.local[0]][8] * As[kmod][tidx.local[1]][8] +
                Bs[kmod][tidx.local[0]][9] * As[kmod][tidx.local[1]][9] +
                Bs[kmod][tidx.local[0]][10] * As[kmod][tidx.local[1]][10] +
                Bs[kmod][tidx.local[0]][11] * As[kmod][tidx.local[1]][11] +
                Bs[kmod][tidx.local[0]][12] * As[kmod][tidx.local[1]][12] +
                Bs[kmod][tidx.local[0]][13] * As[kmod][tidx.local[1]][13] +
                Bs[kmod][tidx.local[0]][14] * As[kmod][tidx.local[1]][14] +
                Bs[kmod][tidx.local[0]][15] * As[kmod][tidx.local[1]][15];
   }
   if (tidx.global[0] < N && tidx.global[1] < M)
   {
     Cmat[tidx.global[0]][cOffset + tidx.global[1]] *= beta;
     Cmat[tidx.global[0]][cOffset + tidx.global[1]] += CValue * alpha;
   }
 });
}

static void gemm_NoTransA_loopunroll_swprefetch(Concurrency::array_view<float, 1> &A, long aOffset,
                                                Concurrency::array_view<float, 1> &B, long bOffset,
                                                Concurrency::array_view<float, 1> &C, long cOffset,
                                                int M, int N, int K, int lda, int ldb, int ldc,
                                                float alpha, float beta)
{
  Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1), (M + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);
  Concurrency::array_view<float,2> Cmat = C.view_as<2>(Concurrency::extent<2>(N, M));
  Concurrency::array_view<float,2> Amat = A.view_as<2>(Concurrency::extent<2>(K, M));
  Concurrency::array_view<float,2> Bmat = B.view_as<2>(Concurrency::extent<2>(K, N));

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float CValue = 0;
    int tileId = tidx.tile[0] * TILE_DIM + tidx.local[1];
    tile_static float As[2][TILE_DIM][TILE_DIM];
    tile_static float Bs[2][TILE_DIM][TILE_DIM];

    int kNext, kmod = 0;
    int kSize = ((K + (TILE_DIM - 1)) & ~(TILE_DIM - 1)) / TILE_DIM;

    if (tidx.local[0] < K && tileId < N)
      Bs[0][tidx.local[1]][tidx.local[0]] = Bmat[tidx.local[0]][tileId];
    else
      Bs[0][tidx.local[1]][tidx.local[0]] = 0.0;

    if (tidx.global[1] < M && (tidx.local[0]) < K)
      As[0][tidx.local[1]][tidx.local[0]] = Amat[tidx.local[0]] [tidx.global[1]];
    else
      As[0][tidx.local[1]][tidx.local[0]] = 0.0;

    for (int k = 0; k < kSize; k++)
    {
      kNext = k + 1;
      kmod = k & 1;

      tidx.barrier.wait();
      if (kNext * TILE_DIM + tidx.local[0] < K && tileId < N)
        Bs[kNext & 1][tidx.local[1]][tidx.local[0]] = Bmat[kNext * TILE_DIM + tidx.local[0]][tileId];
      else
        Bs[kNext & 1][tidx.local[1]][tidx.local[0]] = 0;

      if (tidx.global[1] < M && (kNext * TILE_DIM + tidx.local[0]) < K)
        As[kNext & 1][tidx.local[1]][tidx.local[0]] = Amat[(kNext * TILE_DIM + tidx.local[0])] [tidx.global[1]];
      else
        As[kNext & 1][tidx.local[1]][tidx.local[0]] = 0.0;

      // Unrolled Matrix Mul operation
      CValue += Bs[kmod][tidx.local[0]][0] * As[kmod][tidx.local[1]][0] +
                Bs[kmod][tidx.local[0]][1] * As[kmod][tidx.local[1]][1] +
                Bs[kmod][tidx.local[0]][2] * As[kmod][tidx.local[1]][2] +
                Bs[kmod][tidx.local[0]][3] * As[kmod][tidx.local[1]][3] +
                Bs[kmod][tidx.local[0]][4] * As[kmod][tidx.local[1]][4] +
                Bs[kmod][tidx.local[0]][5] * As[kmod][tidx.local[1]][5] +
                Bs[kmod][tidx.local[0]][6] * As[kmod][tidx.local[1]][6] +
                Bs[kmod][tidx.local[0]][7] * As[kmod][tidx.local[1]][7] +
                Bs[kmod][tidx.local[0]][8] * As[kmod][tidx.local[1]][8] +
                Bs[kmod][tidx.local[0]][9] * As[kmod][tidx.local[1]][9] +
                Bs[kmod][tidx.local[0]][10] * As[kmod][tidx.local[1]][10] +
                Bs[kmod][tidx.local[0]][11] * As[kmod][tidx.local[1]][11] +
                Bs[kmod][tidx.local[0]][12] * As[kmod][tidx.local[1]][12] +
                Bs[kmod][tidx.local[0]][13] * As[kmod][tidx.local[1]][13] +
                Bs[kmod][tidx.local[0]][14] * As[kmod][tidx.local[1]][14] +
                Bs[kmod][tidx.local[0]][15] * As[kmod][tidx.local[1]][15];

   }
   if (tidx.global[0] < N && tidx.global[1] < M)
   {
     Cmat[tidx.global[0]][tidx.global[1]] *= beta;
     Cmat[tidx.global[0]][tidx.global[1]] += CValue * alpha;
   }
 });
}

static void gemm_NoTransB_loopunroll_swprefetch(Concurrency::array_view<float, 1> &A, long aOffset,
                                                Concurrency::array_view<float, 1> &B, long bOffset,
                                                Concurrency::array_view<float, 1> &C, long cOffset,
                                                int M, int N, int K, int lda, int ldb, int ldc,
                                                float alpha, float beta,
                                                Concurrency::array_view<float,1> &temp_buf)
{
  Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1), (M + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);
  Concurrency::array_view<float,2> Cmat = C.view_as<2>(Concurrency::extent<2>(N, M));
  Concurrency::array_view<float,2> Amat = A.view_as<2>(Concurrency::extent<2>(M, K));
  Concurrency::array_view<float,2> Bmat = B.view_as<2>(Concurrency::extent<2>(N, K));

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float CValue = 0;
    tile_static float As[2][TILE_DIM][TILE_DIM];
    tile_static float Bs[2][TILE_DIM][TILE_DIM];

    int tileId = tidx.tile[1] * TILE_DIM + tidx.local[0];
    int kNext, kmod = 0;
    int kSize = ((K + (TILE_DIM - 1)) & ~(TILE_DIM - 1)) / TILE_DIM;

    if (tidx.local[1] < K && tidx.global[0] < N)
      Bs[0][tidx.local[0]][tidx.local[1]] = Bmat[tidx.global[0]][bOffset + tidx.local[1]];
    else
      Bs[0][tidx.local[0]][tidx.local[1]] = 0.0;
    if (tidx.local[1] < K && tileId < M)
      As[0][tidx.local[0]][tidx.local[1]] = Amat[tileId] [aOffset + tidx.local[1]];
    else
      As[0][tidx.local[0]][tidx.local[1]] = 0.0;

    for (int k = 0; k < kSize; k++)
    {
     kNext = k + 1;
     kmod = k & 1;

     tidx.barrier.wait();
     if (kNext * TILE_DIM + tidx.local[1] < K && tidx.global[0] < N)
        Bs[kNext & 1][tidx.local[0]][tidx.local[1]] = Bmat[tidx.global[0]][bOffset + kNext * TILE_DIM + tidx.local[1]];
      else
        Bs[kNext & 1][tidx.local[0]][tidx.local[1]] = 0.0;
      if (kNext * TILE_DIM + tidx.local[1] < K && tileId < M)
        As[kNext & 1][tidx.local[0]][tidx.local[1]] = Amat[tileId] [aOffset + kNext * TILE_DIM+ tidx.local[1]];
      else
        As[kNext & 1][tidx.local[0]][tidx.local[1]] = 0.0;

      // Unrolled Matrix Mul operation
      CValue += Bs[kmod][tidx.local[0]][0] * As[kmod][tidx.local[1]][0] +
                Bs[kmod][tidx.local[0]][1] * As[kmod][tidx.local[1]][1] +
                Bs[kmod][tidx.local[0]][2] * As[kmod][tidx.local[1]][2] +
                Bs[kmod][tidx.local[0]][3] * As[kmod][tidx.local[1]][3] +
                Bs[kmod][tidx.local[0]][4] * As[kmod][tidx.local[1]][4] +
                Bs[kmod][tidx.local[0]][5] * As[kmod][tidx.local[1]][5] +
                Bs[kmod][tidx.local[0]][6] * As[kmod][tidx.local[1]][6] +
                Bs[kmod][tidx.local[0]][7] * As[kmod][tidx.local[1]][7] +
                Bs[kmod][tidx.local[0]][8] * As[kmod][tidx.local[1]][8] +
                Bs[kmod][tidx.local[0]][9] * As[kmod][tidx.local[1]][9] +
                Bs[kmod][tidx.local[0]][10] * As[kmod][tidx.local[1]][10] +
                Bs[kmod][tidx.local[0]][11] * As[kmod][tidx.local[1]][11] +
                Bs[kmod][tidx.local[0]][12] * As[kmod][tidx.local[1]][12] +
                Bs[kmod][tidx.local[0]][13] * As[kmod][tidx.local[1]][13] +
                Bs[kmod][tidx.local[0]][14] * As[kmod][tidx.local[1]][14] +
                Bs[kmod][tidx.local[0]][15] * As[kmod][tidx.local[1]][15];
   }
   if (tidx.global[0] < N && tidx.global[1] < M)
   {
     Cmat[tidx.global[0]][cOffset + tidx.global[1]] *= beta;
     Cmat[tidx.global[0]][cOffset + tidx.global[1]] += CValue * alpha;
   }
 });
}

static void gemm_TransAB_loopunroll_swprefetch(Concurrency::array_view<float, 1> &A, long aOffset,
                                               Concurrency::array_view<float, 1> &B, long bOffset,
                                               Concurrency::array_view<float, 1> &C, long cOffset,
                                               int M, int N, int K, int lda, int ldb, int ldc,
                                               float alpha, float beta)
{
  Concurrency::extent<2> grdExt((M + (THREADS - 1)) & ~(THREADS - 1), (N + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);
  Concurrency::array_view<float,2> Cmat = C.view_as<2>(Concurrency::extent<2>(N, M));
  Concurrency::array_view<float,2> Amat = A.view_as<2>(Concurrency::extent<2>(M, K));
  Concurrency::array_view<float,2> Bmat = B.view_as<2>(Concurrency::extent<2>(K, N));

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float CValue = 0;
    tile_static float As[2][TILE_DIM][TILE_DIM];
    tile_static float Bs[2][TILE_DIM][TILE_DIM];
    int kSize = ((K + (TILE_DIM - 1)) & ~(TILE_DIM - 1));
    int kNext = 0, kmod = 0;

    if (tidx.local[0] < K && tidx.global[1] < N)
     Bs[0][tidx.local[1]][tidx.local[0]] = Bmat[bOffset + tidx.local[0]][tidx.global[1]];
    else
     Bs[0][tidx.local[1]][tidx.local[0]] = 0.0;
    if (tidx.global[0] < M && tidx.local[1] < K)
     As[0][tidx.local[0]][tidx.local[1]] = Amat[tidx.global[0]][tidx.local[1]];
    else
     As[0][tidx.local[0]][tidx.local[1]] = 0.0;

    for (int k = 0; k < kSize ; k++)
    {
      kNext = k + 1;
      kmod = k & 1;

      if (kNext * TILE_DIM + tidx.local[0] < K && tidx.global[1] < N)
        Bs[kNext & 1][tidx.local[1]][tidx.local[0]] = Bmat[bOffset + kNext * TILE_DIM + tidx.local[0]][tidx.global[1]];
      else
        Bs[kNext & 1][tidx.local[1]][tidx.local[0]] = 0.0;
      if (tidx.global[0] < M && (kNext * TILE_DIM + tidx.local[1]) < K)
        As[kNext & 1][tidx.local[0]][tidx.local[1]] = Amat[tidx.global[0]][(kNext * TILE_DIM + tidx.local[1])];
      else
        As[kNext & 1][tidx.local[0]][tidx.local[1]] = 0.0;

      tidx.barrier.wait();
      // Unrolled Matrix Mul operation
      CValue += Bs[kmod][tidx.local[1]][0] * As[kmod][tidx.local[0]][0] +
                Bs[kmod][tidx.local[1]][1] * As[kmod][tidx.local[0]][1] +
                Bs[kmod][tidx.local[1]][2] * As[kmod][tidx.local[0]][2] +
                Bs[kmod][tidx.local[1]][3] * As[kmod][tidx.local[0]][3] +
                Bs[kmod][tidx.local[1]][4] * As[kmod][tidx.local[0]][4] +
                Bs[kmod][tidx.local[1]][5] * As[kmod][tidx.local[0]][5] +
                Bs[kmod][tidx.local[1]][6] * As[kmod][tidx.local[0]][6] +
                Bs[kmod][tidx.local[1]][7] * As[kmod][tidx.local[0]][7] +
                Bs[kmod][tidx.local[1]][8] * As[kmod][tidx.local[0]][8] +
                Bs[kmod][tidx.local[1]][9] * As[kmod][tidx.local[0]][9] +
                Bs[kmod][tidx.local[1]][10] * As[kmod][tidx.local[0]][10] +
                Bs[kmod][tidx.local[1]][11] * As[kmod][tidx.local[0]][11] +
                Bs[kmod][tidx.local[1]][12] * As[kmod][tidx.local[0]][12] +
                Bs[kmod][tidx.local[1]][13] * As[kmod][tidx.local[0]][13] +
                Bs[kmod][tidx.local[1]][14] * As[kmod][tidx.local[0]][14] +
                Bs[kmod][tidx.local[1]][15] * As[kmod][tidx.local[0]][15];

   tidx.barrier.wait();
   }
   if (tidx.global[0] < M && tidx.global[1] < N)
   {
    Cmat[cOffset + tidx.global[1]][tidx.global[0]] *= beta;
    Cmat[cOffset + tidx.global[1]][tidx.global[0]] += CValue * alpha;
   }
 });
}

#endif
#if LOOPUNROLL
static void gemm_NoTransAB_loopunroll(Concurrency::array_view<float, 1> &A, long aOffset,
                                      Concurrency::array_view<float, 1> &B, long bOffset,
                                      Concurrency::array_view<float, 1> &C, long cOffset,
                                      int M, int N, int K, int lda, int ldb, int ldc,
                                      float alpha, float beta)
{
  Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1), (M + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);
  Concurrency::array_view<float,2> Cmat = C.view_as<2>(Concurrency::extent<2>(N, M));
  Concurrency::array_view<float,2> Amat = A.view_as<2>(Concurrency::extent<2>(K, M));
  Concurrency::array_view<float,2> Bmat = B.view_as<2>(Concurrency::extent<2>(N, K));

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
      if (k + tidx.local[0] < K && Col < M)
        As[tidx.local[1]][tidx.local[0]] = Amat[aOffset + k + tidx.local[0]][Col];
      else
        As[tidx.local[1]][tidx.local[0]] = 0.0;

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

static void gemm_NoTransA_loopunroll(Concurrency::array_view<float, 1> &A, long aOffset,
                                     Concurrency::array_view<float, 1> &B, long bOffset,
                                     Concurrency::array_view<float, 1> &C, long cOffset,
                                     int M, int N, int K, int lda, int ldb, int ldc,
                                     float alpha, float beta)
{
  Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1), (M + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);
  Concurrency::array_view<float,2> Cmat = C.view_as<2>(Concurrency::extent<2>(N, M));
  Concurrency::array_view<float,2> Amat = A.view_as<2>(Concurrency::extent<2>(K, M));
  Concurrency::array_view<float,2> Bmat = B.view_as<2>(Concurrency::extent<2>(K, N));

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float CValue = 0;
    int Row = tidx.global[0];
    int Col = tidx.global[1];
    tile_static float As[TILE_DIM][TILE_DIM];
    tile_static float Bs[TILE_DIM][TILE_DIM];
    for (int k = 0; k < ((K + (TILE_DIM - 1)) & ~(TILE_DIM - 1)) ; k += TILE_DIM)
    {
      if (k + tidx.local[0] < K && tidx.tile[0] * TILE_DIM + tidx.local[1] < N)
        Bs[tidx.local[1]][tidx.local[0]] = Bmat[bOffset + k + tidx.local[0]][tidx.tile[0] * TILE_DIM + tidx.local[1]];
      else
        Bs[tidx.local[1]][tidx.local[0]] = 0.0;
      if (Col < M && (k + tidx.local[0]) < K)
        As[tidx.local[1]][tidx.local[0]] = Amat[(k + tidx.local[0])] [Col];
      else
        As[tidx.local[1]][tidx.local[0]] = 0.0;

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

static void gemm_NoTransB_loopunroll(Concurrency::array_view<float, 1> &A, long aOffset,
                                     Concurrency::array_view<float, 1> &B, long bOffset,
                                     Concurrency::array_view<float, 1> &C, long cOffset,
                                     int M, int N, int K, int lda, int ldb, int ldc,
                                     float alpha, float beta,
                                     Concurrency::array_view<float,1> &temp_buf)
{
  Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1), (M + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);
  Concurrency::array_view<float,2> Cmat = C.view_as<2>(Concurrency::extent<2>(N, M));
  Concurrency::array_view<float,2> Amat = A.view_as<2>(Concurrency::extent<2>(M, K));
  Concurrency::array_view<float,2> Bmat = B.view_as<2>(Concurrency::extent<2>(N, K));

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

static void gemm_TransAB_loopunroll(Concurrency::array_view<float, 1> &A, long aOffset,
                                    Concurrency::array_view<float, 1> &B, long bOffset,
                                    Concurrency::array_view<float, 1> &C, long cOffset,
                                    int M, int N, int K, int lda, int ldb, int ldc,
                                    float alpha, float beta)
{
  Concurrency::extent<2> grdExt((M + (THREADS - 1)) & ~(THREADS - 1), (N + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);
  Concurrency::array_view<float,2> Cmat = C.view_as<2>(Concurrency::extent<2>(N, M));
  Concurrency::array_view<float,2> Amat = A.view_as<2>(Concurrency::extent<2>(M, K));
  Concurrency::array_view<float,2> Bmat = B.view_as<2>(Concurrency::extent<2>(K, N));

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float CValue = 0;
    int Row = tidx.global[0];
    int Col = tidx.global[1];
    tile_static float As[TILE_DIM][TILE_DIM];
    tile_static float Bs[TILE_DIM][TILE_DIM];
    for (int k = 0; k < ((K + (TILE_DIM - 1)) & ~(TILE_DIM - 1)) ; k += TILE_DIM)
    {
      if (k + tidx.local[0] < K && Col < N)
        Bs[tidx.local[1]][tidx.local[0]] = Bmat[bOffset + k + tidx.local[0]][Col];
      else
        Bs[tidx.local[1]][tidx.local[0]] = 0.0;
      if (Row < M && (k + tidx.local[1]) < K)
        As[tidx.local[0]][tidx.local[1]] = Amat[Row][(k + tidx.local[1])];
      else
        As[tidx.local[0]][tidx.local[1]] = 0.0;

      tidx.barrier.wait();
      // Unrolled Matrix Mul operation
      CValue += Bs[tidx.local[1]][0] * As[tidx.local[0]][0] +
                Bs[tidx.local[1]][1] * As[tidx.local[0]][1] +
                Bs[tidx.local[1]][2] * As[tidx.local[0]][2] +
                Bs[tidx.local[1]][3] * As[tidx.local[0]][3] +
                Bs[tidx.local[1]][4] * As[tidx.local[0]][4] +
                Bs[tidx.local[1]][5] * As[tidx.local[0]][5] +
                Bs[tidx.local[1]][6] * As[tidx.local[0]][6] +
                Bs[tidx.local[1]][7] * As[tidx.local[0]][7] +
                Bs[tidx.local[1]][8] * As[tidx.local[0]][8] +
                Bs[tidx.local[1]][9] * As[tidx.local[0]][9] +
                Bs[tidx.local[1]][10] * As[tidx.local[0]][10] +
                Bs[tidx.local[1]][11] * As[tidx.local[0]][11] +
                Bs[tidx.local[1]][12] * As[tidx.local[0]][12] +
                Bs[tidx.local[1]][13] * As[tidx.local[0]][13] +
                Bs[tidx.local[1]][14] * As[tidx.local[0]][14] +
                Bs[tidx.local[1]][15] * As[tidx.local[0]][15];

   tidx.barrier.wait();
   }
   if (Row < M && Col < N)
   {
    Cmat[cOffset + Col][Row] *= beta;
    Cmat[cOffset + Col][Row] += CValue * alpha;
   }
 });
}

#endif

#if REGISTER

static void gemm_NoTransAB(Concurrency::array_view<float, 1> &A, long aOffset,
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
    #define B(row,col) B[(row) + (col) * K]
    #define C(row,col) C[(row) + (col) * M]

    // Index variables
    const unsigned int row = tidx.global[0];
    const unsigned int col = tidx.tile[1] * TILE_SZ_B;

    // Privatization of output variables
    float c_reg[TILE_SZ_B] = {(float)0};

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
    float c_reg[TILE_SZ_B] = {(float)0};

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

static void gemm_NoTransB(Concurrency::array_view<float, 1> &A, long aOffset,
                          Concurrency::array_view<float, 1> &B, long bOffset,
                          Concurrency::array_view<float, 1> &C, long cOffset,
                          int M, int N, int K, int lda, int ldb, int ldc,
                          float alpha, float beta,
                          Concurrency::array_view<float,1> &temp_buf)
{
    Concurrency::extent<2> grdExt(((M - 1) / TILE_SZ_A + 1) * TILE_SZ_A, (N - 1) / TILE_SZ_B + 1);
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
    float c_reg[TILE_SZ_B] = {(float)0};

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

static void gemm_TransAB(Concurrency::array_view<float, 1> &A, long aOffset,
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
    #define A1(row,col) A[(row) * K + (col)]
    #define B1(row,col) B[(row) * N + (col)]
    #define C1(row,col) C[(row) + (col) * M]

    // Index variables
    const unsigned int row = tidx.global[0];
    const unsigned int col = tidx.tile[1] * TILE_SZ_B;

    // Privatization of output variables
    float c_reg[TILE_SZ_B] = {(float)0};

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
#endif

#if STEP
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
         MS1x1(offset, offset);
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
    int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
    float rC[1][1] = {(float)0};
    float rA[1][STEPSIZE / TILESIZE];
    float rB[1][STEPSIZE / TILESIZE];
    tile_static float lA[TILESIZE * MICROTILESIZE * STEPSIZE];
    tile_static float lB[TILESIZE * MICROTILESIZE * STEPSIZE];
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
      for(int sec = 0; sec < STEPSIZE / TILESIZE; ++sec)
      {
        if(gidy * TILESIZE + idxT < N && i * STEPSIZE + idyT + (sec * TILESIZE) < K)
        {
          lB[(idyT + (sec * TILESIZE)) * TILESIZE + idxT] = B[bOffset + gidy * TILESIZE + idxT + (idyT + (sec * TILESIZE)) * ldb + i * (ldb << shiftFactor)];
        }
        else
        {
          lB[(idyT + (sec * TILESIZE )) * TILESIZE + idxT] = 0;
	}

        if(gidx * TILESIZE + idxT < M && i * STEPSIZE + idyT + (sec * TILESIZE) < K)
        {
          lA[(idyT + (sec * TILESIZE)) * TILESIZE + idxT] = A[aOffset + gidx * TILESIZE + idxT + (idyT + (sec * TILESIZE)) * lda + i * (lda << shiftFactor)];
        }
        else
        {
          lA[(idyT + (sec * TILESIZE)) * TILESIZE + idxT] = 0;
        }
      }
      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;
      int offset = TILESIZE;

      for (int iter=0; iter < TILESIZE; ++iter)
      {
        MS1x1(offset, offset);
      }

      i++;
    } while (--block_k > 0);

    tidx.barrier.wait();
    if(gidx * TILESIZE + idx < M && gidy * TILESIZE + idy < N)
      C[cOffset + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc] =  alpha * rC[0][0] + beta * C[cOffset + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc];
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
    int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
    float rC[1][1] = {(float)0};
    float rA[1][STEPSIZE / TILESIZE];
    float rB[1][STEPSIZE / TILESIZE];
    tile_static float lA[TILESIZE * MICROTILESIZE * STEPSIZE];
    tile_static float lB[TILESIZE * MICROTILESIZE * STEPSIZE];
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = TILESIZE * idy + idx;
    int idxT = idt % TILESIZE;
    int idyT = idt / TILESIZE;
    int block_k =((K + (STEPSIZE - 1)) & ~(STEPSIZE - 1)) >> shiftFactor;

    int i = 0;
    do
    {
      tidx.barrier.wait();
      for(int sec = 0; sec < STEPSIZE / TILESIZE; ++sec)
      {
        if(gidy * TILESIZE + idxT < N && i * STEPSIZE + idyT + (sec * TILESIZE) < K)
        {
          lB[(sec * TILESIZE * TILESIZE) + idyT + idxT * TILESIZE] = B[bOffset + (gidy * TILESIZE + idxT) * ldb + idyT + i * STEPSIZE + (sec * TILESIZE)];
        }
        else
        {
          lB[(sec * TILESIZE * TILESIZE ) + idyT + idxT * TILESIZE] = 0;
	}

        if(gidx * TILESIZE + idxT < M && i * STEPSIZE + idyT + (sec * TILESIZE ) < K)
        {
          lA[(sec * TILESIZE * TILESIZE) + idyT + idxT * TILESIZE] = A[aOffset + (gidx * TILESIZE + idxT) * lda + idyT + i * STEPSIZE + (sec * TILESIZE)];
        }
        else
        {
          lA[(sec * TILESIZE * TILESIZE ) + idyT + idxT * TILESIZE] = 0;
        }
      }

    tidx.barrier.wait();

    int offA = idx * TILESIZE;
    int offB = idy * TILESIZE;
    int offset = 1;

    for (int iter=0; iter < TILESIZE; ++iter)
    {
      MS1x1(offset, offset);
    }

    i++;
    }while (--block_k > 0);

    tidx.barrier.wait();
    if(gidx * TILESIZE + idx < M && gidy * TILESIZE + idy < N)
        C[cOffset + (gidx * TILESIZE + idx) + (gidy * TILESIZE + idy) * ldc] = alpha * rC[0][0] + beta * C[cOffset + (gidx * TILESIZE + idx) + (gidy * TILESIZE + idy) * ldc];
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
    int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
    float rC[1][1] = {(float)0};
    float rA[1][STEPSIZE/TILESIZE];
    float rB[1][STEPSIZE/TILESIZE];
    tile_static float lA[TILESIZE * MICROTILESIZE * STEPSIZE];
    tile_static float lB[TILESIZE * MICROTILESIZE * STEPSIZE];
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
      for(int sec = 0; sec < STEPSIZE / TILESIZE; sec++ ) {

        if(gidy * TILESIZE + idxT < N && i * STEPSIZE + idyT +(sec * TILESIZE) < K)
        {
          lB[((idxT + sec * TILESIZE) * TILESIZE) + idyT] = B[bOffset + gidy * TILESIZE + idxT + ((idyT + (sec * TILESIZE)) *ldb) + i * (ldb << shiftFactor)];
        }
        else
        {
          lB[((idxT + sec * TILESIZE) * TILESIZE) + idyT] = 0;
        }

        if(gidx * TILESIZE + idxT < M && i * STEPSIZE + idyT + (sec * TILESIZE) < K)
        {
          lA[(sec * TILESIZE * TILESIZE) + idyT + idxT * TILESIZE] = A[aOffset  + (gidx * TILESIZE + idxT) * lda + idyT + i * STEPSIZE + (sec * TILESIZE)];
        }
        else
        {
          lA[(sec * TILESIZE * TILESIZE ) + idyT + idxT * TILESIZE] = 0;
        }
      }
      tidx.barrier.wait();

      int offA = idx * TILESIZE;
      int offB = idy * TILESIZE;
      int offset = 1;
      for (int iter = 0; iter < TILESIZE; ++iter)
      {
        MS1x1(offset, offset);
      }
      i++;
    } while (--block_k > 0);


    tidx.barrier.wait();
    if(gidx * TILESIZE + idx < M && gidy * TILESIZE + idy < N)
        C[cOffset + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc] = alpha * rC[0][0] + beta * C[cOffset + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc];
  });
}
#endif

#if SUBMICROTILE
#if NOTRANSAB
static void gemm_NoTransAB_subMicroTile(Concurrency::array_view<float, 1> &A, long aOffset,
                                        Concurrency::array_view<float, 1> &B, long bOffset,
                                        Concurrency::array_view<float, 1> &C, long cOffset,
                                        int M, int N, int K, int lda, int ldb, int ldc,
                                        float alpha, float beta)
{
  Concurrency::extent<2> grdExt(((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    float rC[MICROTILESIZE][MICROTILESIZE] = {(float)0};
    float rA[1][MICROTILESIZE];
    float rB[1][MICROTILESIZE];
    tile_static float lA[TILESIZE * TILESIZE * MICROTILESIZE];
    tile_static float lB[TILESIZE * TILESIZE * MICROTILESIZE];
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = TILESIZE * idy + idx;
    int idxT = idt % TILESIZE;
    int idyT = idt / TILESIZE;
    int block_k = 0;
    do
    {
      tidx.barrier.wait();
      for(int sec = 0; sec < MICROTILESIZE; ++sec)
      {
        if(gidy * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < N && block_k * TILESIZE + idyT < K)
        {
          lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = B[bOffset + (gidy * TILESIZE * MICROTILESIZE + idxT + sec * TILESIZE) * ldb + idyT + block_k * TILESIZE];
        }
        else
        {
          lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
	}

        if(gidx * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < M && block_k * TILESIZE + idyT < K)
        {
          lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = A[aOffset + (gidx * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE) +  idyT * lda + block_k * (lda * TILESIZE)];
        }
        else
        {
          lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
        }
      }
      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;
      for (int iter=0; iter < TILESIZE; ++iter)
      {
        MTS;
      }
      tidx.barrier.wait();
    } while (++block_k < (((K + TILESIZE - 1) & ~(TILESIZE - 1))/TILESIZE));

    int xIndex = gidx * TILESIZE * MICROTILESIZE + idx;
    int yIndex = (gidy * TILESIZE * MICROTILESIZE + idy) * ldc;
    for( int row = 0; row < MICROTILESIZE; row++)
    {
      for( int col = 0; col < MICROTILESIZE ; col++)
      {
      if(xIndex + (TILESIZE * col) < M && (yIndex / ldc) + (TILESIZE * row) < N)
        C[cOffset + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc] = alpha * rC[col][row] + beta * C[cOffset + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc];
      }
    }
 });
}
#endif
#if NOTRANSA
static void gemm_NoTransA_subMicroTile(Concurrency::array_view<float, 1> &A, long aOffset,
                                       Concurrency::array_view<float, 1> &B, long bOffset,
                                       Concurrency::array_view<float, 1> &C, long cOffset,
                                       int M, int N, int K, int lda, int ldb, int ldc,
                                       float alpha, float beta)
{
  Concurrency::extent<2> grdExt(((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    float rC[MICROTILESIZE][MICROTILESIZE] = {(float)0};
    float rA[1][MICROTILESIZE];
    float rB[1][MICROTILESIZE];
    tile_static float lA[TILESIZE * TILESIZE * MICROTILESIZE];
    tile_static float lB[TILESIZE * TILESIZE * MICROTILESIZE];
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = TILESIZE * idy + idx;
    int idxT = idt % TILESIZE;
    int idyT = idt / TILESIZE;
    int block_k = 0;
    do
    {
      tidx.barrier.wait();
      for(int sec = 0; sec < MICROTILESIZE; ++sec)
      {
        if(gidy * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < N && block_k * TILESIZE + idyT < K)
        {
          lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = B[bOffset + (gidy * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE) + idyT * ldb + block_k * (ldb * TILESIZE)];
        }
        else
        {
          lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
	}

        if(gidx * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < M && block_k * TILESIZE + idyT < K)
        {
          lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = A[aOffset + (gidx * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE) +  idyT * lda + block_k * (lda * TILESIZE)];
        }
        else
        {
          lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
        }
      }
      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;
      for (int iter=0; iter < TILESIZE; ++iter)
      {
        MTS;
      }
      tidx.barrier.wait();
    } while (++block_k < (((K + TILESIZE - 1) & ~(TILESIZE - 1))/TILESIZE));

    int xIndex = gidx * TILESIZE * MICROTILESIZE + idx;
    int yIndex = (gidy * TILESIZE * MICROTILESIZE + idy) * ldc;
    for( int row = 0; row < MICROTILESIZE; row++)
    {
      for( int col = 0; col < MICROTILESIZE ; col++)
      {
      if(xIndex + (TILESIZE * col) < M && (yIndex / ldc) + (TILESIZE * row) < N)
        C[cOffset + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc] = alpha * rC[col][row] + beta * C[cOffset + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc];
      }
    }
 });
}
#endif

#if NOTRANSB
static void gemm_NoTransB_subMicroTile(Concurrency::array_view<float, 1> &A, long aOffset,
                                       Concurrency::array_view<float, 1> &B, long bOffset,
                                       Concurrency::array_view<float, 1> &C, long cOffset,
                                       int M, int N, int K, int lda, int ldb, int ldc,
                                       float alpha, float beta)
{
  Concurrency::extent<2> grdExt(((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    float rC[MICROTILESIZE][MICROTILESIZE] = {(float)0};
    float rA[1][MICROTILESIZE];
    float rB[1][MICROTILESIZE];
    tile_static float lA[TILESIZE * TILESIZE * MICROTILESIZE];
    tile_static float lB[TILESIZE * TILESIZE * MICROTILESIZE];
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = TILESIZE * idy + idx;
    int idxT = idt % TILESIZE;
    int idyT = idt / TILESIZE;
    int block_k = 0;
    do
    {
      tidx.barrier.wait();
      for(int sec = 0; sec < MICROTILESIZE; ++sec)
      {
        if(gidy * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < N && block_k * TILESIZE + idyT < K)
        {
          lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = B[bOffset + (gidy * TILESIZE * MICROTILESIZE + idxT + sec * TILESIZE) * ldb + idyT + block_k * TILESIZE];
        }
        else
        {
          lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
	}

        if(gidx * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < M && block_k * TILESIZE + idyT < K)
        {
          lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = A[aOffset + (gidx * TILESIZE * MICROTILESIZE + idxT + sec * TILESIZE) * lda +  idyT + block_k * TILESIZE];
        }
        else
        {
          lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
        }
      }
      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;
      for (int iter=0; iter < TILESIZE; ++iter)
      {
        MTS;
      }
      tidx.barrier.wait();
    } while (++block_k < (((K + TILESIZE - 1) & ~(TILESIZE - 1))/TILESIZE));

    int xIndex = gidx * TILESIZE * MICROTILESIZE + idx;
    int yIndex = (gidy * TILESIZE * MICROTILESIZE + idy) * ldc;
    for( int row = 0; row < MICROTILESIZE; row++)
    {
      for( int col = 0; col < MICROTILESIZE ; col++)
      {
      if(xIndex + (TILESIZE * col) < M && (yIndex / ldc) + (TILESIZE * row) < N)
        C[cOffset + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc] = alpha * rC[col][row] + beta * C[cOffset + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc];
      }
    }
 });
}
#endif

#if TRANSAB
static void gemm_TransAB_subMicroTile(Concurrency::array_view<float, 1> &A, long aOffset,
                                      Concurrency::array_view<float, 1> &B, long bOffset,
                                      Concurrency::array_view<float, 1> &C, long cOffset,
                                      int M, int N, int K, int lda, int ldb, int ldc,
                                      float alpha, float beta)
{
  Concurrency::extent<2> grdExt(((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    float rC[MICROTILESIZE][MICROTILESIZE] = {(float)0};
    float rA[1][MICROTILESIZE];
    float rB[1][MICROTILESIZE];
    tile_static float lA[TILESIZE * TILESIZE * MICROTILESIZE];
    tile_static float lB[TILESIZE * TILESIZE * MICROTILESIZE];
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = TILESIZE * idy + idx;
    int idxT = idt % TILESIZE;
    int idyT = idt / TILESIZE;
    int block_k = 0;
    do
    {
      tidx.barrier.wait();
      for(int sec = 0; sec < MICROTILESIZE; ++sec)
      {
        if(gidy * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < N && block_k * TILESIZE + idyT < K)
        {
          lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = B[bOffset + (gidy * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE) + idyT * ldb + block_k * (ldb * TILESIZE)];
        }
        else
        {
          lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
	}

        if(gidx * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < M && block_k * TILESIZE + idyT < K)
        {
          lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = A[aOffset + (gidx * TILESIZE * MICROTILESIZE + idxT + sec * TILESIZE) * lda +  idyT + block_k * TILESIZE];
        }
        else
        {
          lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
        }
      }
      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;
      for (int iter=0; iter < TILESIZE; ++iter)
      {
        MTS;
      }
      tidx.barrier.wait();
    } while (++block_k < (((K + TILESIZE - 1) & ~(TILESIZE - 1))/TILESIZE));

    int xIndex = gidx * TILESIZE * MICROTILESIZE + idx;
    int yIndex = (gidy * TILESIZE * MICROTILESIZE + idy) * ldc;
    for( int row = 0; row < MICROTILESIZE; row++)
    {
      for( int col = 0; col < MICROTILESIZE ; col++)
      {
      if(xIndex + (TILESIZE * col) < M && (yIndex / ldc) + (TILESIZE * row) < N)
        C[cOffset + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc] = alpha * rC[col][row] + beta * C[cOffset + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc];
      }
    }
 });
}
#endif
#endif

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
#if LOOPUNROLL
  {
    if (TransB == 'n')
    {
      if (TransA == 'n')
        gemm_NoTransAB_loopunroll(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      else
        gemm_NoTransB_loopunroll(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta, temp_buf);
    }
    else if (TransA == 'n')
      gemm_NoTransA_loopunroll(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
    else
      gemm_TransAB_loopunroll(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
#endif

#if REGISTER
  {
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
  }
#endif
#if STEP 
  {
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
  }
#endif
#if SUBMICROTILE
  {
    if (TransB == 'n')
    {
      if (TransA == 'n')
      {
#if NOTRANSAB
       gemm_NoTransAB_subMicroTile(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
#endif
      }
      else
      {
#if NOTRANSB
        gemm_NoTransB_subMicroTile(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
#endif
      }
    }
    else if (TransA == 'n')
    {
#if NOTRANSA
      gemm_NoTransA_subMicroTile(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
#endif
    }
    else
    {
#if TRANSAB
     gemm_TransAB_subMicroTile(A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
#endif
    }
  }
#endif

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
