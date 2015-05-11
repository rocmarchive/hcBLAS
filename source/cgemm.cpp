#include "ampblaslib.h"
#include<assert.h>
#include <vector>
#include <amp.h>
#include <iostream>
#include <amp_short_vectors.h>
#include <amp_math.h>
using namespace Concurrency;
using namespace Concurrency::graphics;

#define REGISTER 0
#define STEP 0
#define SUBMICROTILE 1
#define LOOPUNROLL 0

#if SUBMICROTILE
#define NOTRANSAB 1
#define NOTRANSA 0
#define NOTRANSB 0
#define TRANSAB 0
#endif

#define THREADS    16
#define GEMM_BLOCK 256
#define TILE_DIM  16

#define TILE_SZ_A 64
#define TILE_SZ_B 16
#define TILE_SZ_RATIO (TILE_SZ_A/TILE_SZ_B)
#define TILESIZE 16
#define MICROTILESIZE 2
#define STEPSIZE 128 
#define TOTMICROTILEPROD (TILESIZE*TILESIZE*MICROTILESIZE)
#define MICROTILEPROD (TILESIZE*MICROTILESIZE)
#define BANKMICROTILESIZE (TILESIZE*MICROTILESIZE+1)

#define  M1x1(offset)			\
            rAreal[0][0] = lAreal[offA + 0];	\
            rBreal[0][0] = lBreal[offB + 0];	\
            rAimg[0][0] = lAimg[offA + 0];	\
            rBimg[0][0] = lBimg[offB + 0];	\
            offA += offset;			\
            offB += offset;			\
            rCreal[0][0] = rCreal[0][0] + (rAreal[0][0] *rBreal[0][0]) - (rAimg[0][0] * rBimg[0][0]) ; \	

#define  M1x1img(offset)			\
            rAreal[0][0] = lAreal[offA + 0];	\
            rBreal[0][0] = lBreal[offB + 0];	\
            rAimg[0][0] = lAimg[offA + 0];	\
            rBimg[0][0] = lBimg[offB + 0];	\
            offA += offset;			\
            offB += offset;			\
            rCimg[0][0] = rCimg[0][0] + (rAreal[0][0] *rBimg[0][0]) + (rAimg[0][0] * rBreal[0][0]) ; \	

#define  MS1x1(offset)			\
            for(int iter = 0; iter < STEPSIZE/TILESIZE; ++iter) \
            {\
              rAreal[0][iter] = lAreal[offA + (TILESIZE * TILESIZE) * iter];	\
              rBreal[0][iter] = lBreal[offB + (TILESIZE * TILESIZE) * iter];	\
              rAimg[0][iter] = lAimg[offA + (TILESIZE * TILESIZE) * iter];	\
              rBimg[0][iter] = lBimg[offB + (TILESIZE * TILESIZE) * iter];	\
              rCreal[0][0] = rCreal[0][0] + (rAreal[0][iter] *rBreal[0][iter]) - (rAimg[0][iter] * rBimg[0][iter]) ; \
            }\
            offA += offset;			\
            offB += offset;			\

#define  MS1x1img(offset)			\
            for(int iter = 0; iter < STEPSIZE/TILESIZE; ++iter) \
            {\
              rAreal[0][iter] = lAreal[offA + (TILESIZE * TILESIZE) * iter];	\
              rBreal[0][iter] = lBreal[offB + (TILESIZE * TILESIZE) * iter];	\
              rAimg[0][iter] = lAimg[offA + (TILESIZE * TILESIZE) * iter];	\
              rBimg[0][iter] = lBimg[offB + (TILESIZE * TILESIZE) * iter];	\
              rCimg[0][0] = rCimg[0][0] + (rAreal[0][iter] *rBimg[0][iter]) + (rAimg[0][iter] * rBreal[0][iter]) ; \
            }\
            offA += offset;			\
            offB += offset;			\

#define  MTS                                                                                \
           for(int iter = 0; iter < MICROTILESIZE ; iter++)                                 \
           {                                                                                \
             rAreal[0][iter] = lAreal[offA + (iter * TILESIZE)];                            \
             rBreal[0][iter] = lBreal[offB + (iter * TILESIZE)];                            \
             rAimg[0][iter] = lAimg[offA + (iter * TILESIZE)];                              \
             rBimg[0][iter] = lBimg[offB + (iter * TILESIZE)];                              \
           }                                                                                \
           for(int rowIndex = 0; rowIndex < MICROTILESIZE ; rowIndex++)                     \
           {                                                                                \
           for(int colIndex = 0; colIndex < MICROTILESIZE ; colIndex++)                     \
           {                                                                                \
           rCreal[rowIndex][colIndex] = (rAreal[0][rowIndex] * rBreal[0][colIndex] -        \
	                                 rAimg[0][rowIndex] * rBimg[0][colIndex]) +         \
                                         rCreal[rowIndex][colIndex];                        \
           rCimg[rowIndex][colIndex] = (rAreal[0][rowIndex] * rBimg[0][colIndex] +          \
                                        rAimg[0][rowIndex] * rBreal[0][colIndex]) +         \
                                        rCimg[rowIndex][colIndex];                          \
           }                                                                                \
           }                                                                                \
           offA += BANKMICROTILESIZE;                                                       \
           offB += BANKMICROTILESIZE;                                                       \

#if SUBMICROTILE
#if NOTRANSAB
static void cgemm_NoTransAB_subMicroTile(Concurrency::array_view<float_2, 1> &A, long aOffset,
                                         Concurrency::array_view<float_2, 1> &B, long bOffset,
                                         Concurrency::array_view<float_2, 1> &C, long cOffset,
                                         int M, int N, int K, int lda, int ldb, int ldc,
                                         float_2 alpha, float_2 beta)
{
  Concurrency::extent<2> grdExt((((N + 1) / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), (((M + 1) / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    int shiftTS = Concurrency::fast_math::log2(TILESIZE);
    int shiftMTP = Concurrency::fast_math::log2(MICROTILEPROD);
    float rCreal[MICROTILESIZE][MICROTILESIZE] = {{(float)0}};
    float rCimg[MICROTILESIZE][MICROTILESIZE] = {{(float)0}};
    float rAreal[1][MICROTILESIZE] = {{(float)0}};
    float rAimg[1][MICROTILESIZE] = {{(float)0}};
    float rBreal[1][MICROTILESIZE] = {{(float)0}};
    float rBimg[1][MICROTILESIZE] = {{(float)0}};
    tile_static float lAreal[TOTMICROTILEPROD + TILESIZE];
    tile_static float lAimg[TOTMICROTILEPROD + TILESIZE];
    tile_static float lBreal[TOTMICROTILEPROD + TILESIZE];
    tile_static float lBimg[TOTMICROTILEPROD + TILESIZE];
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt =  ( idy * TILESIZE ) + idx;
    int idxT = idt & (TILESIZE - 1);
    int idyT = idt/ TILESIZE;
    int block_k = 0;
    do
    {
      int colIndex = ( block_k * TILESIZE ) + idyT;
      int lIndex = (idyT * BANKMICROTILESIZE) + idxT;

      tidx.barrier.wait();
      for(int sec = 0; sec < MICROTILESIZE; ++sec)
      {
        int secVal = sec * TILESIZE;
        int BrowIndex =  (gidy * MICROTILEPROD)+ idxT + secVal;
        int ArowIndex = (gidx * MICROTILEPROD) + idxT + secVal;

        if( BrowIndex < N && colIndex < K)
        {
          lBreal[lIndex + secVal] = B[bOffset + BrowIndex * ldb + colIndex].x;
          lBimg[lIndex + secVal] = B[bOffset + BrowIndex * ldb + colIndex].y;
        }
        else
        {
          lBreal[lIndex + secVal] = 0;
          lBimg[lIndex + secVal] = 0;
	}

        if( ArowIndex < M && colIndex < K)
        {
          lAreal[lIndex + secVal] = A[aOffset + ArowIndex +  colIndex * lda].x;
          lAimg[lIndex + secVal] = A[aOffset + ArowIndex +  colIndex * lda].y;
        }
        else
        {
          lAreal[lIndex + secVal] = 0;
          lAimg[lIndex + secVal] = 0;
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


    int xIndex = (gidx << shiftMTP) + idx;
    int yIndex = ((gidy << shiftMTP) + idy) * ldc;
    for( int row = 0; row < MICROTILESIZE; row++)
    {
      for( int col = 0; col < MICROTILESIZE ; col++)
      {
      if(xIndex + (col << shiftTS) < M && (yIndex / ldc) + (row << shiftTS) < N) {
        C[cOffset + (xIndex + (col << shiftTS)) + yIndex + (row << shiftTS) * ldc].x = alpha.x * rCreal[col][row] + beta.x * C[cOffset + (xIndex + (col << shiftTS)) + yIndex + (row << shiftTS) * ldc].x;
        C[cOffset + (xIndex + (col << shiftTS)) + yIndex + (row * TILESIZE) * ldc].y = alpha.y * rCimg[col][row] + beta.y * C[cOffset + (xIndex + (col << shiftTS)) + yIndex + (row << shiftTS) * ldc].y;
      }
      }
    }
 });
}
#endif
#endif


#if LOOPUNROLL
static void cgemm_NoTransAB_loopunroll(Concurrency::array_view<float_2, 1> &A, long aOffset,
                                       Concurrency::array_view<float_2, 1> &B, long bOffset,
                                       Concurrency::array_view<float_2, 1> &C, long cOffset,
                                       int M, int N, int K, int lda, int ldb, int ldc,
                                       float_2 alpha, float_2 beta)
{
  Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1), (M + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float CValue = 0, CValue1 = 0;
    int Row = tidx.global[0];
    int Col = tidx.global[1];
    tile_static float Asreal[TILE_DIM][TILE_DIM];
    tile_static float Asimg[TILE_DIM][TILE_DIM];
    tile_static float Bsreal[TILE_DIM][TILE_DIM];
    tile_static float Bsimg[TILE_DIM][TILE_DIM];

    for (int k = 0; k < ((K + (TILE_DIM - 1)) & ~(TILE_DIM - 1)) ; k += TILE_DIM)
    {
      if (k + tidx.local[1] < K && Row < N) {
        Bsreal[tidx.local[0]][tidx.local[1]] = B[bOffset + Row * K + (k + tidx.local[1])].x;
        Bsimg[tidx.local[0]][tidx.local[1]] = B[bOffset + Row * K + (k + tidx.local[1])].y;
      }
      else {
        Bsreal[tidx.local[0]][tidx.local[1]] = 0.0;
        Bsimg[tidx.local[0]][tidx.local[1]] = 0.0;
      }
      if (k + tidx.local[0] < K && Col < M) {
        Asreal[tidx.local[1]][tidx.local[0]] = A[aOffset + (k + tidx.local[0]) * M + Col].x;
        Asimg[tidx.local[1]][tidx.local[0]] = A[aOffset + (k + tidx.local[0]) * M + Col].y;
      }
      else {
        Asreal[tidx.local[1]][tidx.local[0]] = 0.0;
        Asimg[tidx.local[1]][tidx.local[0]] = 0.0;
     }

      tidx.barrier.wait();
      // Unrolled Matrix Mul operation
      CValue += ((Bsreal[tidx.local[0]][0] * Asreal[tidx.local[1]][0]) - (Bsimg[tidx.local[0]][0] * Asimg[tidx.local[1]][0]) +
                (Bsreal[tidx.local[0]][1] * Asreal[tidx.local[1]][1]) - (Bsimg[tidx.local[0]][1] * Asimg[tidx.local[1]][1]) +
                (Bsreal[tidx.local[0]][2] * Asreal[tidx.local[1]][2]) - (Bsimg[tidx.local[0]][2] * Asimg[tidx.local[1]][2]) +
                (Bsreal[tidx.local[0]][3] * Asreal[tidx.local[1]][3]) - (Bsimg[tidx.local[0]][3] * Asimg[tidx.local[1]][3]) +
                (Bsreal[tidx.local[0]][4] * Asreal[tidx.local[1]][4]) - (Bsimg[tidx.local[0]][4] * Asimg[tidx.local[1]][4]) +
                (Bsreal[tidx.local[0]][5] * Asreal[tidx.local[1]][5]) - (Bsimg[tidx.local[0]][5] * Asimg[tidx.local[1]][5]) +
                (Bsreal[tidx.local[0]][6] * Asreal[tidx.local[1]][6]) - (Bsimg[tidx.local[0]][6] * Asimg[tidx.local[1]][6]) +
                (Bsreal[tidx.local[0]][7] * Asreal[tidx.local[1]][7]) - (Bsimg[tidx.local[0]][7] * Asimg[tidx.local[1]][7]) +
                (Bsreal[tidx.local[0]][8] * Asreal[tidx.local[1]][8]) - (Bsimg[tidx.local[0]][8] * Asimg[tidx.local[1]][8]) +
                (Bsreal[tidx.local[0]][9] * Asreal[tidx.local[1]][9]) - (Bsimg[tidx.local[0]][9] * Asimg[tidx.local[1]][9]) +
                (Bsreal[tidx.local[0]][10] * Asreal[tidx.local[1]][10]) - (Bsimg[tidx.local[0]][10] * Asimg[tidx.local[1]][10]) +
                (Bsreal[tidx.local[0]][11] * Asreal[tidx.local[1]][11]) - (Bsimg[tidx.local[0]][11] * Asimg[tidx.local[1]][11]) +
                (Bsreal[tidx.local[0]][12] * Asreal[tidx.local[1]][12]) - (Bsimg[tidx.local[0]][12] * Asimg[tidx.local[1]][12]) +
                (Bsreal[tidx.local[0]][13] * Asreal[tidx.local[1]][13]) - (Bsimg[tidx.local[0]][13] * Asimg[tidx.local[1]][13]) +
                (Bsreal[tidx.local[0]][14] * Asreal[tidx.local[1]][14]) - (Bsimg[tidx.local[0]][14] * Asimg[tidx.local[1]][14]) +
                (Bsreal[tidx.local[0]][15] * Asreal[tidx.local[1]][15]) - (Bsimg[tidx.local[0]][15] * Asimg[tidx.local[1]][15]));

     CValue1 += ((Bsreal[tidx.local[0]][0] * Asimg[tidx.local[1]][0]) + (Bsimg[tidx.local[0]][0] * Asreal[tidx.local[1]][0]) +
               (Bsreal[tidx.local[0]][1] * Asimg[tidx.local[1]][1]) + (Bsimg[tidx.local[0]][1] * Asreal[tidx.local[1]][1]) +
               (Bsreal[tidx.local[0]][2] * Asimg[tidx.local[1]][2]) + (Bsimg[tidx.local[0]][2] * Asreal[tidx.local[1]][2]) +
               (Bsreal[tidx.local[0]][3] * Asimg[tidx.local[1]][3]) + (Bsimg[tidx.local[0]][3] * Asreal[tidx.local[1]][3]) +
               (Bsreal[tidx.local[0]][4] * Asimg[tidx.local[1]][4]) + (Bsimg[tidx.local[0]][4] * Asreal[tidx.local[1]][4]) +
               (Bsreal[tidx.local[0]][5] * Asimg[tidx.local[1]][5]) + (Bsimg[tidx.local[0]][5] * Asreal[tidx.local[1]][5]) +
               (Bsreal[tidx.local[0]][6] * Asimg[tidx.local[1]][6]) + (Bsimg[tidx.local[0]][6] * Asreal[tidx.local[1]][6]) +
               (Bsreal[tidx.local[0]][7] * Asimg[tidx.local[1]][7]) + (Bsimg[tidx.local[0]][7] * Asreal[tidx.local[1]][7]) +
               (Bsreal[tidx.local[0]][8] * Asimg[tidx.local[1]][8]) + (Bsimg[tidx.local[0]][8] * Asreal[tidx.local[1]][8]) +
               (Bsreal[tidx.local[0]][9] * Asimg[tidx.local[1]][9]) + (Bsimg[tidx.local[0]][9] * Asreal[tidx.local[1]][9]) +
               (Bsreal[tidx.local[0]][10] * Asimg[tidx.local[1]][10]) + (Bsimg[tidx.local[0]][10] * Asreal[tidx.local[1]][10]) +
               (Bsreal[tidx.local[0]][11] * Asimg[tidx.local[1]][11]) + (Bsimg[tidx.local[0]][11] * Asreal[tidx.local[1]][11]) +
               (Bsreal[tidx.local[0]][12] * Asimg[tidx.local[1]][12]) + (Bsimg[tidx.local[0]][12] * Asreal[tidx.local[1]][12]) +
               (Bsreal[tidx.local[0]][13] * Asimg[tidx.local[1]][13]) + (Bsimg[tidx.local[0]][13] * Asreal[tidx.local[1]][13]) +
               (Bsreal[tidx.local[0]][14] * Asimg[tidx.local[1]][14]) + (Bsimg[tidx.local[0]][14] * Asreal[tidx.local[1]][14]) +
               (Bsreal[tidx.local[0]][15] * Asimg[tidx.local[1]][15]) + (Bsimg[tidx.local[0]][15] * Asreal[tidx.local[1]][15])); 

   tidx.barrier.wait();
   }
   if (Row < N && Col < M)
   {
     C[cOffset + (Row * M) + Col].x *= beta.x;
     C[cOffset + (Row * M) + Col].x += CValue * alpha.x;
     C[cOffset + (Row * M) + Col].y *= beta.y;
     C[cOffset + (Row * M) + Col].y += CValue1 * alpha.y;
   }
 });
}

static void cgemm_NoTransA_loopunroll(Concurrency::array_view<float_2, 1> &A, long aOffset,
                                      Concurrency::array_view<float_2, 1> &B, long bOffset,
                                      Concurrency::array_view<float_2, 1> &C, long cOffset,
                                      int M, int N, int K, int lda, int ldb, int ldc,
                                      float_2 alpha, float_2 beta)
{
  Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1), (M + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float CValue = 0, CValue1 = 0;
    int Row = tidx.global[0];
    int Col = tidx.global[1];
    tile_static float Asreal[TILE_DIM][TILE_DIM];
    tile_static float Asimg[TILE_DIM][TILE_DIM];
    tile_static float Bsreal[TILE_DIM][TILE_DIM];
    tile_static float Bsimg[TILE_DIM][TILE_DIM];

    for (int k = 0; k < ((K + (TILE_DIM - 1)) & ~(TILE_DIM - 1)) ; k += TILE_DIM)
    {
      if (k + tidx.local[0] < K && tidx.tile[0] * TILE_DIM + tidx.local[1] < N)
      {
        Bsreal[tidx.local[1]][tidx.local[0]] = B[bOffset + ((k + tidx.local[0]) * N) + (tidx.tile[0] * TILE_DIM + tidx.local[1])].x;
        Bsimg[tidx.local[1]][tidx.local[0]] = B[bOffset + ((k + tidx.local[0]) * N) + (tidx.tile[0] * TILE_DIM + tidx.local[1])].y;
      }
      else
      {
        Bsreal[tidx.local[1]][tidx.local[0]] = 0.0;
        Bsimg[tidx.local[1]][tidx.local[0]] = 0.0;
      }
      if (Col < M && (k + tidx.local[0]) < K)
      {
        Asreal[tidx.local[1]][tidx.local[0]] = A[aOffset + ((k + tidx.local[0]) * M) + Col].x;
        Asimg[tidx.local[1]][tidx.local[0]] = A[aOffset + ((k + tidx.local[0]) * M) + Col].y;
      }
      else
      {
        Asreal[tidx.local[1]][tidx.local[0]] = 0.0;
        Asimg[tidx.local[1]][tidx.local[0]] = 0.0;
      }

      tidx.barrier.wait();
      // Unrolled Matrix Mul operation
      CValue += ((Bsreal[tidx.local[0]][0] * Asreal[tidx.local[1]][0]) - (Bsimg[tidx.local[0]][0] * Asimg[tidx.local[1]][0]) +
                (Bsreal[tidx.local[0]][1] * Asreal[tidx.local[1]][1]) - (Bsimg[tidx.local[0]][1] * Asimg[tidx.local[1]][1]) +
                (Bsreal[tidx.local[0]][2] * Asreal[tidx.local[1]][2]) - (Bsimg[tidx.local[0]][2] * Asimg[tidx.local[1]][2]) +
                (Bsreal[tidx.local[0]][3] * Asreal[tidx.local[1]][3]) - (Bsimg[tidx.local[0]][3] * Asimg[tidx.local[1]][3]) +
                (Bsreal[tidx.local[0]][4] * Asreal[tidx.local[1]][4]) - (Bsimg[tidx.local[0]][4] * Asimg[tidx.local[1]][4]) +
                (Bsreal[tidx.local[0]][5] * Asreal[tidx.local[1]][5]) - (Bsimg[tidx.local[0]][5] * Asimg[tidx.local[1]][5]) +
                (Bsreal[tidx.local[0]][6] * Asreal[tidx.local[1]][6]) - (Bsimg[tidx.local[0]][6] * Asimg[tidx.local[1]][6]) +
                (Bsreal[tidx.local[0]][7] * Asreal[tidx.local[1]][7]) - (Bsimg[tidx.local[0]][7] * Asimg[tidx.local[1]][7]) +
                (Bsreal[tidx.local[0]][8] * Asreal[tidx.local[1]][8]) - (Bsimg[tidx.local[0]][8] * Asimg[tidx.local[1]][8]) +
                (Bsreal[tidx.local[0]][9] * Asreal[tidx.local[1]][9]) - (Bsimg[tidx.local[0]][9] * Asimg[tidx.local[1]][9]) +
                (Bsreal[tidx.local[0]][10] * Asreal[tidx.local[1]][10]) - (Bsimg[tidx.local[0]][10] * Asimg[tidx.local[1]][10]) +
                (Bsreal[tidx.local[0]][11] * Asreal[tidx.local[1]][11]) - (Bsimg[tidx.local[0]][11] * Asimg[tidx.local[1]][11]) +
                (Bsreal[tidx.local[0]][12] * Asreal[tidx.local[1]][12]) - (Bsimg[tidx.local[0]][12] * Asimg[tidx.local[1]][12]) +
                (Bsreal[tidx.local[0]][13] * Asreal[tidx.local[1]][13]) - (Bsimg[tidx.local[0]][13] * Asimg[tidx.local[1]][13]) +
                (Bsreal[tidx.local[0]][14] * Asreal[tidx.local[1]][14]) - (Bsimg[tidx.local[0]][14] * Asimg[tidx.local[1]][14]) +
                (Bsreal[tidx.local[0]][15] * Asreal[tidx.local[1]][15]) - (Bsimg[tidx.local[0]][15] * Asimg[tidx.local[1]][15]));

      CValue1 += ((Bsreal[tidx.local[0]][0] * Asimg[tidx.local[1]][0]) + (Bsimg[tidx.local[0]][0] * Asreal[tidx.local[1]][0]) +
                 (Bsreal[tidx.local[0]][1] * Asimg[tidx.local[1]][1]) + (Bsimg[tidx.local[0]][1] * Asreal[tidx.local[1]][1]) +
                 (Bsreal[tidx.local[0]][2] * Asimg[tidx.local[1]][2]) + (Bsimg[tidx.local[0]][2] * Asreal[tidx.local[1]][2]) +
                 (Bsreal[tidx.local[0]][3] * Asimg[tidx.local[1]][3]) + (Bsimg[tidx.local[0]][3] * Asreal[tidx.local[1]][3]) +
                 (Bsreal[tidx.local[0]][4] * Asimg[tidx.local[1]][4]) + (Bsimg[tidx.local[0]][4] * Asreal[tidx.local[1]][4]) +
                 (Bsreal[tidx.local[0]][5] * Asimg[tidx.local[1]][5]) + (Bsimg[tidx.local[0]][5] * Asreal[tidx.local[1]][5]) +
                 (Bsreal[tidx.local[0]][6] * Asimg[tidx.local[1]][6]) + (Bsimg[tidx.local[0]][6] * Asreal[tidx.local[1]][6]) +
                 (Bsreal[tidx.local[0]][7] * Asimg[tidx.local[1]][7]) + (Bsimg[tidx.local[0]][7] * Asreal[tidx.local[1]][7]) +
                 (Bsreal[tidx.local[0]][8] * Asimg[tidx.local[1]][8]) + (Bsimg[tidx.local[0]][8] * Asreal[tidx.local[1]][8]) +
                 (Bsreal[tidx.local[0]][9] * Asimg[tidx.local[1]][9]) + (Bsimg[tidx.local[0]][9] * Asreal[tidx.local[1]][9]) +
                 (Bsreal[tidx.local[0]][10] * Asimg[tidx.local[1]][10]) + (Bsimg[tidx.local[0]][10] * Asreal[tidx.local[1]][10]) +
                 (Bsreal[tidx.local[0]][11] * Asimg[tidx.local[1]][11]) + (Bsimg[tidx.local[0]][11] * Asreal[tidx.local[1]][11]) +
                 (Bsreal[tidx.local[0]][12] * Asimg[tidx.local[1]][12]) + (Bsimg[tidx.local[0]][12] * Asreal[tidx.local[1]][12]) +
                 (Bsreal[tidx.local[0]][13] * Asimg[tidx.local[1]][13]) + (Bsimg[tidx.local[0]][13] * Asreal[tidx.local[1]][13]) +
                 (Bsreal[tidx.local[0]][14] * Asimg[tidx.local[1]][14]) + (Bsimg[tidx.local[0]][14] * Asreal[tidx.local[1]][14]) +
                 (Bsreal[tidx.local[0]][15] * Asimg[tidx.local[1]][15]) + (Bsimg[tidx.local[0]][15] * Asreal[tidx.local[1]][15])); 

   tidx.barrier.wait();
   }
   if (Row < N && Col < M)
   {
     C[cOffset + (Row * M) + Col].x *= beta.x;
     C[cOffset + (Row * M) + Col].x += CValue * alpha.x;
     C[cOffset + (Row * M) + Col].y *= beta.y;
     C[cOffset + (Row * M) + Col].y += CValue1 * alpha.y;
   }
 });
}

static void cgemm_NoTransB_loopunroll(Concurrency::array_view<float_2, 1> &A, long aOffset,
                                      Concurrency::array_view<float_2, 1> &B, long bOffset,
                                      Concurrency::array_view<float_2, 1> &C, long cOffset,
                                      int M, int N, int K, int lda, int ldb, int ldc,
                                      float_2 alpha, float_2 beta)
{
  Concurrency::extent<2> grdExt((N + (THREADS - 1)) & ~(THREADS - 1), (M + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float CValue = 0, CValue1 = 0;
    int Row = tidx.global[0];
    int Col = tidx.global[1];
    tile_static float Asreal[TILE_DIM][TILE_DIM];
    tile_static float Asimg[TILE_DIM][TILE_DIM];
    tile_static float Bsreal[TILE_DIM][TILE_DIM];
    tile_static float Bsimg[TILE_DIM][TILE_DIM];
    for (int k = 0; k < ((K + (TILE_DIM - 1)) & ~(TILE_DIM - 1)) ; k += TILE_DIM)
    {
      if (k + tidx.local[1] < K && Row < N)
      {
        Bsreal[tidx.local[0]][tidx.local[1]] = B[bOffset + Row * K + k + tidx.local[1]].x;
        Bsimg[tidx.local[0]][tidx.local[1]] = B[bOffset + Row * K + k + tidx.local[1]].y;
	}
      else
      {
        Bsreal[tidx.local[0]][tidx.local[1]] = 0.0;
        Bsimg[tidx.local[0]][tidx.local[1]] = 0.0;
      }
      if (k + tidx.local[1] < K && (tidx.tile[1] * TILE_DIM + tidx.local[0]) < M)
      {
        Asreal[tidx.local[0]][tidx.local[1]] = A[aOffset + ((tidx.tile[1] * TILE_DIM + tidx.local[0]) * K) + k + tidx.local[1]].x;
        Asimg[tidx.local[0]][tidx.local[1]] = A[aOffset + ((tidx.tile[1] * TILE_DIM + tidx.local[0]) * K) + k + tidx.local[1]].y;
      }
      else
      {
        Asreal[tidx.local[0]][tidx.local[1]] = 0.0;
        Asimg[tidx.local[0]][tidx.local[1]] = 0.0;
      }

      tidx.barrier.wait();
      // Unrolled Matrix Mul operation
      CValue += ((Bsreal[tidx.local[0]][0] * Asreal[tidx.local[1]][0]) - (Bsimg[tidx.local[0]][0] * Asimg[tidx.local[1]][0]) +
                (Bsreal[tidx.local[0]][1] * Asreal[tidx.local[1]][1]) - (Bsimg[tidx.local[0]][1] * Asimg[tidx.local[1]][1]) +
                (Bsreal[tidx.local[0]][2] * Asreal[tidx.local[1]][2]) - (Bsimg[tidx.local[0]][2] * Asimg[tidx.local[1]][2]) +
                (Bsreal[tidx.local[0]][3] * Asreal[tidx.local[1]][3]) - (Bsimg[tidx.local[0]][3] * Asimg[tidx.local[1]][3]) +
                (Bsreal[tidx.local[0]][4] * Asreal[tidx.local[1]][4]) - (Bsimg[tidx.local[0]][4] * Asimg[tidx.local[1]][4]) +
                (Bsreal[tidx.local[0]][5] * Asreal[tidx.local[1]][5]) - (Bsimg[tidx.local[0]][5] * Asimg[tidx.local[1]][5]) +
                (Bsreal[tidx.local[0]][6] * Asreal[tidx.local[1]][6]) - (Bsimg[tidx.local[0]][6] * Asimg[tidx.local[1]][6]) +
                (Bsreal[tidx.local[0]][7] * Asreal[tidx.local[1]][7]) - (Bsimg[tidx.local[0]][7] * Asimg[tidx.local[1]][7]) +
                (Bsreal[tidx.local[0]][8] * Asreal[tidx.local[1]][8]) - (Bsimg[tidx.local[0]][8] * Asimg[tidx.local[1]][8]) +
                (Bsreal[tidx.local[0]][9] * Asreal[tidx.local[1]][9]) - (Bsimg[tidx.local[0]][9] * Asimg[tidx.local[1]][9]) +
                (Bsreal[tidx.local[0]][10] * Asreal[tidx.local[1]][10]) - (Bsimg[tidx.local[0]][10] * Asimg[tidx.local[1]][10]) +
                (Bsreal[tidx.local[0]][11] * Asreal[tidx.local[1]][11]) - (Bsimg[tidx.local[0]][11] * Asimg[tidx.local[1]][11]) +
                (Bsreal[tidx.local[0]][12] * Asreal[tidx.local[1]][12]) - (Bsimg[tidx.local[0]][12] * Asimg[tidx.local[1]][12]) +
                (Bsreal[tidx.local[0]][13] * Asreal[tidx.local[1]][13]) - (Bsimg[tidx.local[0]][13] * Asimg[tidx.local[1]][13]) +
                (Bsreal[tidx.local[0]][14] * Asreal[tidx.local[1]][14]) - (Bsimg[tidx.local[0]][14] * Asimg[tidx.local[1]][14]) +
                (Bsreal[tidx.local[0]][15] * Asreal[tidx.local[1]][15]) - (Bsimg[tidx.local[0]][15] * Asimg[tidx.local[1]][15]));

      CValue1 += ((Bsreal[tidx.local[0]][0] * Asimg[tidx.local[1]][0]) + (Bsimg[tidx.local[0]][0] * Asreal[tidx.local[1]][0]) +
                 (Bsreal[tidx.local[0]][1] * Asimg[tidx.local[1]][1]) + (Bsimg[tidx.local[0]][1] * Asreal[tidx.local[1]][1]) +
                 (Bsreal[tidx.local[0]][2] * Asimg[tidx.local[1]][2]) + (Bsimg[tidx.local[0]][2] * Asreal[tidx.local[1]][2]) +
                 (Bsreal[tidx.local[0]][3] * Asimg[tidx.local[1]][3]) + (Bsimg[tidx.local[0]][3] * Asreal[tidx.local[1]][3]) +
                 (Bsreal[tidx.local[0]][4] * Asimg[tidx.local[1]][4]) + (Bsimg[tidx.local[0]][4] * Asreal[tidx.local[1]][4]) +
                 (Bsreal[tidx.local[0]][5] * Asimg[tidx.local[1]][5]) + (Bsimg[tidx.local[0]][5] * Asreal[tidx.local[1]][5]) +
                 (Bsreal[tidx.local[0]][6] * Asimg[tidx.local[1]][6]) + (Bsimg[tidx.local[0]][6] * Asreal[tidx.local[1]][6]) +
                 (Bsreal[tidx.local[0]][7] * Asimg[tidx.local[1]][7]) + (Bsimg[tidx.local[0]][7] * Asreal[tidx.local[1]][7]) +
                 (Bsreal[tidx.local[0]][8] * Asimg[tidx.local[1]][8]) + (Bsimg[tidx.local[0]][8] * Asreal[tidx.local[1]][8]) +
                 (Bsreal[tidx.local[0]][9] * Asimg[tidx.local[1]][9]) + (Bsimg[tidx.local[0]][9] * Asreal[tidx.local[1]][9]) +
                 (Bsreal[tidx.local[0]][10] * Asimg[tidx.local[1]][10]) + (Bsimg[tidx.local[0]][10] * Asreal[tidx.local[1]][10]) +
                 (Bsreal[tidx.local[0]][11] * Asimg[tidx.local[1]][11]) + (Bsimg[tidx.local[0]][11] * Asreal[tidx.local[1]][11]) +
                 (Bsreal[tidx.local[0]][12] * Asimg[tidx.local[1]][12]) + (Bsimg[tidx.local[0]][12] * Asreal[tidx.local[1]][12]) +
                 (Bsreal[tidx.local[0]][13] * Asimg[tidx.local[1]][13]) + (Bsimg[tidx.local[0]][13] * Asreal[tidx.local[1]][13]) +
                 (Bsreal[tidx.local[0]][14] * Asimg[tidx.local[1]][14]) + (Bsimg[tidx.local[0]][14] * Asreal[tidx.local[1]][14]) +
                 (Bsreal[tidx.local[0]][15] * Asimg[tidx.local[1]][15]) + (Bsimg[tidx.local[0]][15] * Asreal[tidx.local[1]][15])); 

   tidx.barrier.wait();
   }
   if (Row < N && Col < M)
   {
     C[cOffset + (Row * M) + Col].x *= beta.x;
     C[cOffset + (Row * M) + Col].x += CValue * alpha.x;
     C[cOffset + (Row * M) + Col].y *= beta.y;
     C[cOffset + (Row * M) + Col].y += CValue1 * alpha.y;
   }
 });
}

static void cgemm_TransAB_loopunroll(Concurrency::array_view<float_2, 1> &A, long aOffset,
                                     Concurrency::array_view<float_2, 1> &B, long bOffset,
                                     Concurrency::array_view<float_2, 1> &C, long cOffset,
                                     int M, int N, int K, int lda, int ldb, int ldc,
                                     float_2 alpha, float_2 beta)
{
  Concurrency::extent<2> grdExt((M + (THREADS - 1)) & ~(THREADS - 1), (N + (THREADS - 1)) & ~(THREADS - 1));
  Concurrency::tiled_extent<THREADS, THREADS> t_ext(grdExt);

  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<THREADS, THREADS> tidx) restrict(amp)
  {
    float CValue = 0, CValue1 = 0;
    int Row = tidx.global[0];
    int Col = tidx.global[1];
    tile_static float Asreal[TILE_DIM][TILE_DIM];
    tile_static float Asimg[TILE_DIM][TILE_DIM];
    tile_static float Bsreal[TILE_DIM][TILE_DIM];
    tile_static float Bsimg[TILE_DIM][TILE_DIM];
    for (int k = 0; k < ((K + (TILE_DIM - 1)) & ~(TILE_DIM - 1)) ; k += TILE_DIM)
    {
      if (k + tidx.local[0] < K && Col < N)
      {
        Bsreal[tidx.local[1]][tidx.local[0]] = B[bOffset + ((k + tidx.local[0]) * N) + Col].x;
        Bsimg[tidx.local[1]][tidx.local[0]] = B[bOffset + ((k + tidx.local[0]) * N) + Col].y;
	}
      else
      {
        Bsreal[tidx.local[1]][tidx.local[0]] = 0.0;
        Bsimg[tidx.local[1]][tidx.local[0]] = 0.0;
	}
      if (Row < M && (k + tidx.local[1]) < K)
      {
        Asreal[tidx.local[0]][tidx.local[1]] = A[aOffset + Row * K + (k + tidx.local[1])].x;
        Asimg[tidx.local[0]][tidx.local[1]] = A[aOffset + Row * K + (k + tidx.local[1])].y;
      }
      else
      {
        Asreal[tidx.local[0]][tidx.local[1]] = 0.0;
        Asimg[tidx.local[0]][tidx.local[1]] = 0.0;
      }

      tidx.barrier.wait();
      // Unrolled Matrix Mul operation
      CValue += ((Bsreal[tidx.local[1]][0] * Asreal[tidx.local[0]][0]) - (Bsimg[tidx.local[1]][0] * Asimg[tidx.local[0]][0]) +
                (Bsreal[tidx.local[1]][1] * Asreal[tidx.local[0]][1]) - (Bsimg[tidx.local[1]][1] * Asimg[tidx.local[0]][1]) +
                (Bsreal[tidx.local[1]][2] * Asreal[tidx.local[0]][2]) - (Bsimg[tidx.local[1]][2] * Asimg[tidx.local[0]][2]) +
                (Bsreal[tidx.local[1]][3] * Asreal[tidx.local[0]][3]) - (Bsimg[tidx.local[1]][3] * Asimg[tidx.local[0]][3]) +
                (Bsreal[tidx.local[1]][4] * Asreal[tidx.local[0]][4]) - (Bsimg[tidx.local[1]][4] * Asimg[tidx.local[0]][4]) +
                (Bsreal[tidx.local[1]][5] * Asreal[tidx.local[0]][5]) - (Bsimg[tidx.local[1]][5] * Asimg[tidx.local[0]][5]) +
                (Bsreal[tidx.local[1]][6] * Asreal[tidx.local[0]][6]) - (Bsimg[tidx.local[1]][6] * Asimg[tidx.local[0]][6]) +
                (Bsreal[tidx.local[1]][7] * Asreal[tidx.local[0]][7]) - (Bsimg[tidx.local[1]][7] * Asimg[tidx.local[0]][7]) +
                (Bsreal[tidx.local[1]][8] * Asreal[tidx.local[0]][8]) - (Bsimg[tidx.local[1]][8] * Asimg[tidx.local[0]][8]) +
                (Bsreal[tidx.local[1]][9] * Asreal[tidx.local[0]][9]) - (Bsimg[tidx.local[1]][9] * Asimg[tidx.local[0]][9]) +
                (Bsreal[tidx.local[1]][10] * Asreal[tidx.local[0]][10]) - (Bsimg[tidx.local[1]][10] * Asimg[tidx.local[0]][10]) +
                (Bsreal[tidx.local[1]][11] * Asreal[tidx.local[0]][11]) - (Bsimg[tidx.local[1]][11] * Asimg[tidx.local[0]][11]) +
                (Bsreal[tidx.local[1]][12] * Asreal[tidx.local[0]][12]) - (Bsimg[tidx.local[1]][12] * Asimg[tidx.local[0]][12]) +
                (Bsreal[tidx.local[1]][13] * Asreal[tidx.local[0]][13]) - (Bsimg[tidx.local[1]][13] * Asimg[tidx.local[0]][13]) +
                (Bsreal[tidx.local[1]][14] * Asreal[tidx.local[0]][14]) - (Bsimg[tidx.local[1]][14] * Asimg[tidx.local[0]][14]) +
                (Bsreal[tidx.local[1]][15] * Asreal[tidx.local[0]][15]) - (Bsimg[tidx.local[1]][15] * Asimg[tidx.local[0]][15]));

      CValue1 += ((Bsreal[tidx.local[1]][0] * Asimg[tidx.local[0]][0]) + (Bsimg[tidx.local[1]][0] * Asreal[tidx.local[0]][0]) +
                 (Bsreal[tidx.local[1]][1] * Asimg[tidx.local[0]][1]) + (Bsimg[tidx.local[1]][1] * Asreal[tidx.local[0]][1]) +
                 (Bsreal[tidx.local[1]][2] * Asimg[tidx.local[0]][2]) + (Bsimg[tidx.local[1]][2] * Asreal[tidx.local[0]][2]) +
                 (Bsreal[tidx.local[1]][3] * Asimg[tidx.local[0]][3]) + (Bsimg[tidx.local[1]][3] * Asreal[tidx.local[0]][3]) +
                 (Bsreal[tidx.local[1]][4] * Asimg[tidx.local[0]][4]) + (Bsimg[tidx.local[1]][4] * Asreal[tidx.local[0]][4]) +
                 (Bsreal[tidx.local[1]][5] * Asimg[tidx.local[0]][5]) + (Bsimg[tidx.local[1]][5] * Asreal[tidx.local[0]][5]) +
                 (Bsreal[tidx.local[1]][6] * Asimg[tidx.local[0]][6]) + (Bsimg[tidx.local[1]][6] * Asreal[tidx.local[0]][6]) +
                 (Bsreal[tidx.local[1]][7] * Asimg[tidx.local[0]][7]) + (Bsimg[tidx.local[1]][7] * Asreal[tidx.local[0]][7]) +
                 (Bsreal[tidx.local[1]][8] * Asimg[tidx.local[0]][8]) + (Bsimg[tidx.local[1]][8] * Asreal[tidx.local[0]][8]) +
                 (Bsreal[tidx.local[1]][9] * Asimg[tidx.local[0]][9]) + (Bsimg[tidx.local[1]][9] * Asreal[tidx.local[0]][9]) +
                 (Bsreal[tidx.local[1]][10] * Asimg[tidx.local[0]][10]) + (Bsimg[tidx.local[1]][10] * Asreal[tidx.local[0]][10]) +
                 (Bsreal[tidx.local[1]][11] * Asimg[tidx.local[0]][11]) + (Bsimg[tidx.local[1]][11] * Asreal[tidx.local[0]][11]) +
                 (Bsreal[tidx.local[1]][12] * Asimg[tidx.local[0]][12]) + (Bsimg[tidx.local[1]][12] * Asreal[tidx.local[0]][12]) +
                 (Bsreal[tidx.local[1]][13] * Asimg[tidx.local[0]][13]) + (Bsimg[tidx.local[1]][13] * Asreal[tidx.local[0]][13]) +
                 (Bsreal[tidx.local[1]][14] * Asimg[tidx.local[0]][14]) + (Bsimg[tidx.local[1]][14] * Asreal[tidx.local[0]][14]) +
                 (Bsreal[tidx.local[1]][15] * Asimg[tidx.local[0]][15]) + (Bsimg[tidx.local[1]][15] * Asreal[tidx.local[0]][15])); 

   tidx.barrier.wait();
   }
   if (Row < M && Col < N)
   {
     C[cOffset + (Col * M) + Row].x *= beta.x;
     C[cOffset + (Col * M) + Row].x += CValue * alpha.x;
     C[cOffset + (Col * M) + Row].y *= beta.y;
     C[cOffset + (Col * M) + Row].y += CValue1 * alpha.y;
   }
 });
}
#endif

#if REGISTER
void cgemm_NoTransAB(int M, int N, int K, float_2 alpha,
             Concurrency::array_view<float_2> &A, long aOffset, long lda,
             Concurrency::array_view<float_2> &B, long bOffset, long ldb,float_2 beta,
             Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(((M - 1) / TILE_SZ_A + 1) * TILE_SZ_A, (N - 1) / TILE_SZ_B + 1);
    Concurrency::tiled_extent <TILE_SZ_A, 1> t_ext(grdExt);
    Concurrency::parallel_for_each(t_ext,
                                   [=] (Concurrency::tiled_index<TILE_SZ_A,1> tidx)
                                   restrict(amp) {

    // Shared memory for tiling input B array
    tile_static float B_s_real [TILE_SZ_RATIO][TILE_SZ_B];
    tile_static float B_s_img [TILE_SZ_RATIO][TILE_SZ_B];

    // Macros for accessing flattened matrices
    #define A_real(row,col) A[(row) + (col) * M].x
    #define B_real(row,col) B[(row) + (col) * K].x
    #define C_real(row,col) C[(row) + (col) * M].x
    #define A_img(row,col) A[(row) + (col) * M].y
    #define B_img(row,col) B[(row) + (col) * K].y
    #define C_img(row,col) C[(row) + (col) * M].y
    
    // Index variables
    const unsigned int row = tidx.global[0];
    const unsigned int col = tidx.tile[1] * TILE_SZ_B;

    // Privatization of output variables
    float c_reg_real[TILE_SZ_B] = {(float)0};
    float c_reg_img[TILE_SZ_B] = {(float)0};
     
    // Loop over the input tiles
    for(unsigned int tileIdx = 0; tileIdx < (K - 1)/TILE_SZ_RATIO + 1; ++tileIdx) {
        // Load the tile of B into shared memory
        const unsigned int i = tidx.local[0]/TILE_SZ_B;
        const unsigned int j = tidx.local[0]%TILE_SZ_B;

        if (tileIdx*TILE_SZ_RATIO + i < K && col + j < N) {
            B_s_real[i][j] = B_real(tileIdx*TILE_SZ_RATIO + i, col + j);
        }
        else {
            B_s_real[i][j] = 0;
        }
        
        if (tileIdx*TILE_SZ_RATIO + i < K && col + j < N) {
            B_s_img[i][j] = B_img(tileIdx*TILE_SZ_RATIO + i, col + j);
        }
        else {
            B_s_img[i][j] = 0;
        }
        
        tidx.barrier.wait();

        // Loop over elements inside the tile
        for (unsigned int idx = 0; idx < TILE_SZ_RATIO; ++idx) {
            // Load tile of A matrix into register
            float a_reg_real, a_reg_img;
            if(row < M && tileIdx*TILE_SZ_RATIO + idx < K) {
                a_reg_real = A_real(row, tileIdx*TILE_SZ_RATIO + idx);
            }
            else {
                a_reg_real = 0;
            }
            
            if(row < M && tileIdx*TILE_SZ_RATIO + idx < K) {
                a_reg_img = A_img(row, tileIdx*TILE_SZ_RATIO + idx);
            }
            else {
                a_reg_img = 0;
            }

            // Loop over and update the output elements assigned to the thread
            for(unsigned int outIdx = 0; outIdx < TILE_SZ_B; ++outIdx) {
                c_reg_real[outIdx] += (a_reg_real*B_s_real[idx][outIdx]) -  (a_reg_img*B_s_img[idx][outIdx]);
                c_reg_img[outIdx] += (a_reg_real*B_s_img[idx][outIdx]) +  (a_reg_img*B_s_real[idx][outIdx]);
            }
        }
        tidx.barrier.wait();
    }
    for (unsigned int outIdx = 0; outIdx < TILE_SZ_B; ++outIdx) {
        if (row < M && col + outIdx < N) {
            (C_real(row, col + outIdx) *= beta.x) += (c_reg_real[outIdx] *= alpha.x);
            (C_img(row, col + outIdx) *= beta.y) += (c_reg_img[outIdx] *= alpha.y);
        }
    }
});
   
}

void cgemm_NoTransA(int M, int N, int K, float_2 alpha,
             Concurrency::array_view<float_2> &A, long aOffset, long lda,
             Concurrency::array_view<float_2> &B, long bOffset, long ldb,float_2 beta,
             Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(((M - 1) / TILE_SZ_A + 1) * TILE_SZ_A, (N - 1) / TILE_SZ_B + 1);
    Concurrency::tiled_extent <TILE_SZ_A, 1> t_ext(grdExt);
    Concurrency::parallel_for_each(t_ext,
                                   [=] (Concurrency::tiled_index<TILE_SZ_A,1> tidx)
                                   restrict(amp) {

    // Shared memory for tiling input B array
    tile_static float B_s_real [TILE_SZ_RATIO][TILE_SZ_B];
    tile_static float B_s_img [TILE_SZ_RATIO][TILE_SZ_B];
    
    #define A1_real(row,col) A[(row) + (col) * M].x
    #define B1_real(row,col) B[(row) * N + (col)].x
    #define C1_real(row,col) C[(row) + (col) * M].x
    #define A1_img(row,col) A[(row) + (col) * M].y
    #define B1_img(row,col) B[(row) * N +(col)].y
    #define C1_img(row,col) C[(row) + (col) * M].y

    // Index variables
    const unsigned int row = tidx.global[0];
    const unsigned int col = tidx.tile[1] * TILE_SZ_B;

    // Privatization of output variables
    float c_reg_real[TILE_SZ_B] = {(float)0};
    float c_reg_img[TILE_SZ_B] = {(float)0};
    
    // Loop over the input tiles
    for(unsigned int tileIdx = 0; tileIdx < (K - 1)/TILE_SZ_RATIO + 1; ++tileIdx) {
        // Load the tile of B into shared memory
        const unsigned int i = tidx.local[0]/TILE_SZ_B;
        const unsigned int j = tidx.local[0]%TILE_SZ_B;

        if (tileIdx*TILE_SZ_RATIO + i < K && col + j < N) {
            B_s_real[i][j] = B1_real(tileIdx*TILE_SZ_RATIO + i, col + j);
        }
        else {
            B_s_real[i][j] = 0;
        }
        
        if (tileIdx*TILE_SZ_RATIO + i < K && col + j < N) {
            B_s_img[i][j] = B1_img(tileIdx*TILE_SZ_RATIO + i, col + j);
        }
        else {
            B_s_img[i][j] = 0;
        }
        
        tidx.barrier.wait();

        // Loop over elements inside the tile
        for (unsigned int idx = 0; idx < TILE_SZ_RATIO; ++idx) {
            // Load tile of A matrix into register
            float a_reg_real, a_reg_img;
            if(row < M && tileIdx*TILE_SZ_RATIO + idx < K) {
                a_reg_real = A1_real(row, tileIdx*TILE_SZ_RATIO + idx);
            }
            else {
                a_reg_real = 0;
            }
            
            if(row < M && tileIdx*TILE_SZ_RATIO + idx < K) {
                a_reg_img = A1_img(row, tileIdx*TILE_SZ_RATIO + idx);
            }
            else {
                a_reg_img = 0;
            }
            // Loop over and update the output elements assigned to the thread
            for(unsigned int outIdx = 0; outIdx < TILE_SZ_B; ++outIdx) {
                c_reg_real[outIdx] += (a_reg_real*B_s_real[idx][outIdx]) -  (a_reg_img*B_s_img[idx][outIdx]);
                c_reg_img[outIdx] += (a_reg_real*B_s_img[idx][outIdx]) +  (a_reg_img*B_s_real[idx][outIdx]);
            }
        }
        tidx.barrier.wait();
    }
    for (unsigned int outIdx = 0; outIdx < TILE_SZ_B; ++outIdx) {
        if (row < M && col + outIdx < N) {
            (C1_real(row, col + outIdx) *= beta.x) += (c_reg_real[outIdx] *= alpha.x);
            (C1_img(row, col + outIdx) *= beta.y) += (c_reg_img[outIdx] *= alpha.y);
        }
    }
});
}

void cgemm_NoTransB(int M, int N, int K, float_2 alpha,
             Concurrency::array_view<float_2> &A, long aOffset, long lda,
             Concurrency::array_view<float_2> &B, long bOffset, long ldb,float_2 beta,
             Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(((M - 1) / TILE_SZ_A + 1) * TILE_SZ_A, (N - 1) / TILE_SZ_B + 1);
    Concurrency::tiled_extent <TILE_SZ_A, 1> t_ext(grdExt);

    Concurrency::parallel_for_each(t_ext,
                                   [=] (Concurrency::tiled_index<TILE_SZ_A,1> tidx)
                                   restrict(amp) {

    // Shared memory for tiling input B array
    tile_static float B_s_real [TILE_SZ_RATIO][TILE_SZ_B];
    tile_static float B_s_img [TILE_SZ_RATIO][TILE_SZ_B];
    
    // Macros for accessing flattened matrices
    #define A2_real(row,col) A[(row) * K + (col)].x
    #define B2_real(row,col) B[(row) + (col) * K].x
    #define C2_real(row,col) C[(row) + (col) * M].x
    #define A2_img(row,col) A[(row) * K + (col)].y
    #define B2_img(row,col) B[(row) + (col) * K].y
    #define C2_img(row,col) C[(row) + (col) * M].y

    // Index variables
   const unsigned int row = tidx.global[0];
    const unsigned int col = tidx.tile[1] * TILE_SZ_B;

    // Privatization of output variables
    float c_reg_real[TILE_SZ_B] = {(float)0};
    float c_reg_img[TILE_SZ_B] = {(float)0};

    // Loop over the input tiles
    for(unsigned int tileIdx = 0; tileIdx < (K - 1)/TILE_SZ_RATIO + 1; ++tileIdx) {
        // Load the tile of B into shared memory
        const unsigned int i = tidx.local[0]/TILE_SZ_B;
        const unsigned int j = tidx.local[0]%TILE_SZ_B;

        if (tileIdx*TILE_SZ_RATIO + i < K && col + j < N) {
            B_s_real[i][j] = B2_real(tileIdx*TILE_SZ_RATIO + i, col + j);
        }
        else {
            B_s_real[i][j] = 0;
        }
        
        if (tileIdx*TILE_SZ_RATIO + i < K && col + j < N) {
            B_s_img[i][j] = B2_img(tileIdx*TILE_SZ_RATIO + i, col + j);
        }
        else {
            B_s_img[i][j] = 0;
        }
        
        tidx.barrier.wait();

        // Loop over elements inside the tile
        for (unsigned int idx = 0; idx < TILE_SZ_RATIO; ++idx) {
            // Load tile of A matrix into register
            float a_reg_real, a_reg_img;
            if(row < M && tileIdx*TILE_SZ_RATIO + idx < K) {
                a_reg_real = A2_real(row, tileIdx*TILE_SZ_RATIO + idx);
            }
            else {
                a_reg_real = 0;
            }
            
            if(row < M && tileIdx*TILE_SZ_RATIO + idx < K) {
                a_reg_img = A2_img(row, tileIdx*TILE_SZ_RATIO + idx);
            }
            else {
                a_reg_img = 0;
            }

            // Loop over and update the output elements assigned to the thread
            for(unsigned int outIdx = 0; outIdx < TILE_SZ_B; ++outIdx) {
                c_reg_real[outIdx] += (a_reg_real*B_s_real[idx][outIdx]) -  (a_reg_img*B_s_img[idx][outIdx]);
                c_reg_img[outIdx] += (a_reg_real*B_s_img[idx][outIdx]) +  (a_reg_img*B_s_real[idx][outIdx]);
            }
        }
        tidx.barrier.wait();
    }
    for (unsigned int outIdx = 0; outIdx < TILE_SZ_B; ++outIdx) {
        if (row < M && col + outIdx < N) {
            (C2_real(row, col + outIdx) *= beta.x) += (c_reg_real[outIdx] *= alpha.x);
            (C2_img(row, col + outIdx) *= beta.y) += (c_reg_img[outIdx] *= alpha.y);
        }
    }
});
}

void cgemm_TransAB(int M, int N, int K, float_2 alpha,
                   Concurrency::array_view<float_2> &A, long aOffset, long lda,
                   Concurrency::array_view<float_2> &B, long bOffset, long ldb,
                   float_2 beta,
                   Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(((M - 1) / TILE_SZ_A + 1) * TILE_SZ_A, (N - 1) / TILE_SZ_B + 1);
    Concurrency::tiled_extent <TILE_SZ_A, 1> t_ext(grdExt);

    Concurrency::parallel_for_each(t_ext,
                                   [=] (Concurrency::tiled_index<TILE_SZ_A,1> tidx)
                                   restrict(amp) {

    // Shared memory for tiling input B array
    tile_static float B_s_real [TILE_SZ_RATIO][TILE_SZ_B];
    tile_static float B_s_img [TILE_SZ_RATIO][TILE_SZ_B];

    // Macros for accessing flattened matrices
    #define A3_real(row,col) A[(row) * K + (col)].x
    #define B3_real(row,col) B[(row) * N + (col)].x
    #define C3_real(row,col) C[(row) + (col) * M].x
    #define A3_img(row,col) A[(row) * K + (col)].y
    #define B3_img(row,col) B[(row) * N + (col)].y
    #define C3_img(row,col) C[(row) + (col) * M].y

    // Index variables
    const unsigned int row = tidx.global[0];
    const unsigned int col = tidx.tile[1] * TILE_SZ_B;

    // Privatization of output variables
    float c_reg_real[TILE_SZ_B] = {(float)0};
    float c_reg_img[TILE_SZ_B] = {(float)0};

    // Loop over the input tiles
    for(unsigned int tileIdx = 0; tileIdx < (K - 1)/TILE_SZ_RATIO + 1; ++tileIdx) {
        // Load the tile of B into shared memory
        const unsigned int i = tidx.local[0]/TILE_SZ_B;
        const unsigned int j = tidx.local[0]%TILE_SZ_B;

        if (tileIdx*TILE_SZ_RATIO + i < K && col + j < N) {
            B_s_real[i][j] = B3_real(tileIdx*TILE_SZ_RATIO + i, col + j);
        }
        else {
            B_s_real[i][j] = 0;
        }
        
        if (tileIdx*TILE_SZ_RATIO + i < K && col + j < N) {
            B_s_img[i][j] = B3_img(tileIdx*TILE_SZ_RATIO + i, col + j);
        }
        else {
            B_s_img[i][j] = 0;
        }
        
        tidx.barrier.wait();

        // Loop over elements inside the tile
        for (unsigned int idx = 0; idx < TILE_SZ_RATIO; ++idx) {
            // Load tile of A matrix into register
            float a_reg_real, a_reg_img;
            if(row < M && tileIdx*TILE_SZ_RATIO + idx < K) {
                a_reg_real = A3_real(row, tileIdx*TILE_SZ_RATIO + idx);
            }
            else {
                a_reg_real = 0;
            }
             
            if(row < M && tileIdx*TILE_SZ_RATIO + idx < K) {
                a_reg_img = A3_img(row, tileIdx*TILE_SZ_RATIO + idx);
            }
            else {
                a_reg_img = 0;
            } 
              
            // Loop over and update the output elements assigned to the thread
            for(unsigned int outIdx = 0; outIdx < TILE_SZ_B; ++outIdx) {
                c_reg_real[outIdx] += (a_reg_real*B_s_real[idx][outIdx]) -  (a_reg_img*B_s_img[idx][outIdx]);
                c_reg_img[outIdx] += (a_reg_real*B_s_img[idx][outIdx]) +  (a_reg_img*B_s_real[idx][outIdx]);
            }
        }
        tidx.barrier.wait();
    }
    for (unsigned int outIdx = 0; outIdx < TILE_SZ_B; ++outIdx) {
        if (row < M && col + outIdx < N) {
            (C3_real(row, col + outIdx) *= beta.x) += (c_reg_real[outIdx] *= alpha.x);
            (C3_img(row, col + outIdx) *= beta.y) += (c_reg_img[outIdx] *= alpha.y);
        }
    }
});
}

#endif


#if STEP
void cgemm_NoTransAB_batch(int M, int N, int K, float_2 alpha,
                           Concurrency::array_view<float_2> &A, long aOffset, long lda,
                           Concurrency::array_view<float_2> &B, long bOffset, long ldb,
                           float_2 beta,
                           Concurrency::array_view<float_2> &C, long cOffset, long ldc)

{
  Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);
  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
    float rCreal[1][1];
    float rAreal[1][STEPSIZE/TILESIZE];
    float rBreal[1][STEPSIZE/TILESIZE];
    float rCimg[1][1];
    float rAimg[1][STEPSIZE/TILESIZE];
    float rBimg[1][STEPSIZE/TILESIZE];
    tile_static float lAreal[TILESIZE * MICROTILESIZE * STEPSIZE];//8*8+8
    tile_static float lBreal[TILESIZE * MICROTILESIZE * STEPSIZE];
    tile_static float lAimg[TILESIZE * MICROTILESIZE * STEPSIZE];//8*8+8
    tile_static float lBimg[TILESIZE * MICROTILESIZE * STEPSIZE];
    rCreal[0][0] = 0;
    rCimg[0][0] = 0;
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
        if(gidy*TILESIZE+idxT  < N && (idyT + i * STEPSIZE + (TILESIZE * sec)) < K){ 
          lBreal[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = B[bOffset + (gidy*TILESIZE+ idxT) * ldb + idyT + i * STEPSIZE + (TILESIZE * sec)].x;
          lBimg[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = B[bOffset + (gidy*TILESIZE+ idxT) * ldb + idyT + i * STEPSIZE + (TILESIZE * sec)].y;
         }
        else{
          lBreal[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = 0;
          lBimg[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = 0;
        }

        // Load Section 'sec' from global memory A onto shared lA
        if(gidx * TILESIZE + idxT < M && (i * STEPSIZE + idyT + (TILESIZE * sec)) < K){
           lAreal[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = A[aOffset  + gidx*TILESIZE+ idxT + idyT*lda + i * (lda << shiftFactor) + (TILESIZE * sec) * lda].x;
           lAimg[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = A[aOffset  + gidx*TILESIZE+ idxT + idyT*lda + i * (lda << shiftFactor) + (TILESIZE * sec) * lda].y;
         }        
        else{
           lAreal[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = 0;
           lAimg[idxT*TILESIZE+idyT + (TILESIZE * TILESIZE * sec)] = 0;
        }
      }
      tidx.barrier.wait();

      int offA = idx * TILESIZE;
      int offB = idy * TILESIZE;
      int offset = 1;

      for (int iter=0; iter < TILESIZE; ++iter)
      {
         MS1x1(offset);
      }

      for (int iter=0; iter < TILESIZE; ++iter)
      {
         MS1x1img(offset);
      }
      i++;
    } while (--block_k > 0);


    tidx.barrier.wait();
    if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N){
        C[cOffset + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc].x = alpha.x * rCreal[0][0] + beta.x * C[cOffset + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc].x;
        C[cOffset + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc].y = alpha.y * rCimg[0][0] + beta.y * C[cOffset + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc].y;
    } 
 });

}



void cgemm_NoTransA_batch(int M, int N, int K, float_2 alpha,
                    Concurrency::array_view<float_2> &A, long aOffset, long lda,
                    Concurrency::array_view<float_2> &B, long bOffset, long ldb,
                    float_2 beta,
                    Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
  Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt); 
  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  { 
    int shiftFactor = Concurrency::fast_math::log2(TILESIZE);
    float rCreal[1][1];
    float rAreal[1][1];
    float rBreal[1][1];
    float rCimg[1][1];
    float rAimg[1][1];
    float rBimg[1][1];
    tile_static float lAreal[TILESIZE * MICROTILESIZE * TILESIZE];//8*8+8
    tile_static float lBreal[TILESIZE * MICROTILESIZE * TILESIZE];
    tile_static float lAimg[TILESIZE * MICROTILESIZE * TILESIZE];//8*8+8
    tile_static float lBimg[TILESIZE * MICROTILESIZE * TILESIZE];
    rCreal[0][0] = 0;
    rCimg [0][0] = 0;
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = TILESIZE*idy + idx;
    int idxT = idt % TILESIZE; // col
    int idyT = idt / TILESIZE; // row
    int block_k = ((K + (TILESIZE - 1)) & ~(TILESIZE - 1)) >> shiftFactor;
    int i = 0;
    do
    {
      //barrier(CLK_LOCAL_MEM_FENCE);
      tidx.barrier.wait();
      if(gidy*TILESIZE+idxT < N && i*TILESIZE+idyT < K)
      {
        lBreal[idyT*TILESIZE+idxT] = B[bOffset + gidy*TILESIZE+ idxT + idyT*ldb + i * (ldb << shiftFactor)].x;
        lBimg[idyT*TILESIZE+idxT] = B[bOffset + gidy*TILESIZE+ idxT + idyT*ldb + i * (ldb << shiftFactor)].y;
      }
      else
      {
        lBreal[idyT*TILESIZE+idxT] = 0;
        lBimg[idyT*TILESIZE+idxT] = 0;
      }
  
      if(gidx*TILESIZE+idxT < M && i*TILESIZE+idyT < K)
      {
        lAreal[idyT*TILESIZE+idxT] = A[aOffset  + gidx*TILESIZE+ idxT + idyT*lda + i * (lda << shiftFactor)].x;
        lAimg[idyT*TILESIZE+idxT] = A[aOffset  + gidx*TILESIZE+ idxT + idyT*lda + i * (lda << shiftFactor)].y;
      }
      else
      {
        lAreal[idyT*TILESIZE+idxT] = 0;
        lAimg[idyT*TILESIZE+idxT] = 0;
      }
      tidx.barrier.wait();

      //barrier(CLK_LOCAL_MEM_FENCE);
      int offA = idx;
      int offB = idy;
   
      for (int iter =0; iter < TILESIZE; ++iter)
      {
        M1x1(TILESIZE);
      }

      for (int iter =0; iter < TILESIZE; ++iter)
      {
        M1x1img(TILESIZE);
      }
    
      i++;
    } while (--block_k > 0);


    tidx.barrier.wait();
    if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N)
    {
        C[cOffset + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc].x = alpha.x * rCreal[0][0] + beta.x * C[cOffset + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc].x;
        C[cOffset + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc].y = alpha.y * rCimg[0][0] + beta.y * C[cOffset + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc].y;   
  }
  });
  C.synchronize();
}


void cgemm_NoTransB_batch(int M, int N, int K, float_2 alpha,
                          Concurrency::array_view<float_2> &A, long aOffset, long lda,
                          Concurrency::array_view<float_2> &B, long bOffset, long ldb,
                          float_2 beta,
                          Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
  Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);
  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    int shiftFactor = Concurrency::fast_math::log2(TILESIZE);
    float rCreal[1][1];
    float rAreal[1][1];
    float rBreal[1][1];
    float rCimg[1][1];
    float rAimg[1][1];
    float rBimg[1][1];
    tile_static float lAreal[TILESIZE * MICROTILESIZE * TILESIZE];//8*8+8
    tile_static float lBreal[TILESIZE * MICROTILESIZE * TILESIZE];
    tile_static float lAimg[TILESIZE * MICROTILESIZE * TILESIZE];//8*8+8
    tile_static float lBimg[TILESIZE * MICROTILESIZE * TILESIZE];
    rCreal[0][0] = 0;
    rCimg[0][0] = 0;
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
        lBreal[idyT+idxT*TILESIZE] = B[bOffset + (gidy*TILESIZE+ idxT)*ldb + idyT + i * TILESIZE].x;
        lBimg[idyT+idxT*TILESIZE] = B[bOffset + (gidy*TILESIZE+ idxT)*ldb + idyT + i * TILESIZE].y;

       }
      else
       {
        lBreal[idyT+idxT*TILESIZE] = 0;
        lBimg[idyT+idxT*TILESIZE] = 0;
       }
      if(gidx*TILESIZE+idxT < M && i*TILESIZE+idyT < K)
      {
        lAreal[idyT+idxT*TILESIZE] = A[aOffset  + (gidx*TILESIZE+ idxT)*lda + idyT + i * TILESIZE].x;
        lAimg[idyT+idxT*TILESIZE] = A[aOffset  + (gidx*TILESIZE+ idxT)*lda + idyT + i * TILESIZE].y;
      }
      else
      {
        lAreal[idyT+idxT*TILESIZE] = 0;
        lAimg[idyT+idxT*TILESIZE] = 0;
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
         
      for(int iter = 0; iter < TILESIZE; ++iter)
      {
            M1x1img(offset);
      }
      i++;
    } while (--block_k > 0);

    tidx.barrier.wait();
    if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N){
        C[cOffset + (gidx*TILESIZE +idx) + (gidy*TILESIZE + idy)*ldc].x = alpha.x * rCreal[0][0] + beta.x * C[cOffset + (gidx*TILESIZE+idx) + (gidy*TILESIZE + idy)*ldc].x;
        C[cOffset + (gidx*TILESIZE +idx) + (gidy*TILESIZE + idy)*ldc].y = alpha.y * rCimg[0][0] + beta.y * C[cOffset + (gidx*TILESIZE+idx) + (gidy*TILESIZE + idy)*ldc].y;
}
  });

}

void cgemm_TransAB_batch(int M, int N, int K, float_2 alpha,
                        Concurrency::array_view<float_2> &A, long aOffset, long lda,
                        Concurrency::array_view<float_2> &B, long bOffset, long ldb,
                        float_2 beta,
                        Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
  Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);
  Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    int shiftFactor = Concurrency::fast_math::log2(TILESIZE);
    float rCreal[1][1];
    float rAreal[1][1];
    float rBreal[1][1];
    float rCimg[1][1];
    float rAimg[1][1];
    float rBimg[1][1];
    tile_static float lAreal[TILESIZE * MICROTILESIZE * TILESIZE];
    tile_static float lBreal[TILESIZE * MICROTILESIZE * TILESIZE];
    tile_static float lAimg[TILESIZE * MICROTILESIZE * TILESIZE];
    tile_static float lBimg[TILESIZE * MICROTILESIZE * TILESIZE];
    rCreal[0][0] = 0;
    rCimg[0][0] = 0;
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
        lBreal[idyT*TILESIZE+idxT] = B[bOffset + gidy*TILESIZE+ idxT + idyT*ldb + i * (ldb << shiftFactor)].x;
        lBimg[idyT*TILESIZE+idxT] = B[bOffset + gidy*TILESIZE+ idxT + idyT*ldb + i * (ldb << shiftFactor)].y;
      }
      else
      {
        lBreal[idyT*TILESIZE+idxT] = 0;
        lBimg[idyT*TILESIZE+idxT] = 0;
      }
      if(gidx*TILESIZE+idxT < M && i*TILESIZE+idyT < K)
      {
        lAreal[idyT*TILESIZE+idxT] = A[aOffset  + (gidx*TILESIZE+ idxT)*lda + idyT + i * TILESIZE].x;
        lAimg[idyT*TILESIZE+idxT] = A[aOffset  + (gidx*TILESIZE+ idxT)*lda + idyT + i * TILESIZE].y;
      }
      else
      {
        lAreal[idyT*TILESIZE+idxT] = 0;
        lAimg[idyT*TILESIZE+idxT] = 0;
      }
      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;

      for(int iter = 0; iter < TILESIZE; iter++)
      {
         M1x1(TILESIZE);
      }
  
      for(int iter = 0; iter < TILESIZE; iter++)
      {
         M1x1img(TILESIZE);
      }
      i++;
    } while (--block_k > 0);


    tidx.barrier.wait();
    if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N){
        C[cOffset + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc].x = alpha.x * rCreal[0][0] + beta.x * C[cOffset + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc].x;
        C[cOffset + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc].y = alpha.y * rCimg[0][0] + beta.y * C[cOffset + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc].y;
    }
  });
}
#endif

ampblasStatus Ampblaslibrary:: ampblas_cgemm(const enum AMPBLAS_TRANS typeA,
                                             const enum AMPBLAS_TRANS typeB,
                                             const int M, const int N,
                                             const int K, const ampComplex *alpha,
                                             const ampComplex *A, long aOffset,
                                             long lda, const ampComplex *B,
                                             long bOffset, long ldb,
                                             const ampComplex *beta, 
                                             ampComplex *C, long cOffset,
                                             long ldc)
{  
    Concurrency::extent<1> ext(M * K);
    std::vector<float_2> src(M * K);
    array_view<float_2,1> Acmplx(ext, src);

    Concurrency::extent<1> ext2(K * N);
    std::vector<float_2> src2(K * N);
    array_view<float_2,1> Bcmplx(ext2, src2);
   
    Concurrency::extent<1> ext3(M * N);
    std::vector<float_2> src3(M * N);
    array_view<float_2,1> Ccmplx(ext3, src3);

    float_2 Calpha(alpha->real, alpha->img);
    float_2 Cbeta(beta->real, beta->img);
   
    for ( int i = 0 ;i <  M * K;i++) {
        Acmplx[i].x = A[i].real;
        Acmplx[i].y = A[i].img;
    }

    for ( int i = 0 ;i <  K * N;i++) {
        Bcmplx[i].x = B[i].real;
        Bcmplx[i].y = B[i].img;
    }
 
    for ( int i = 0 ;i <  M * N;i++) {
        Ccmplx[i].x = C[i].real;
        Ccmplx[i].y = C[i].img;
    }

   // Start the operations
#if LOOPUNROLL
{
    if (typeB == noTrans) {
        if (typeA == noTrans) {
            cgemm_NoTransAB_loopunroll(Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
	}
        else {
            cgemm_NoTransB_loopunroll(Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
        }
    }
    else if (typeA == noTrans) {
        cgemm_NoTransA_loopunroll(Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    }
    else {
        cgemm_TransAB_loopunroll(Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    }
}
#endif



#if REGISTER
{
   if (typeB == noTrans) {
        if (typeA == noTrans) {
            cgemm_NoTransAB(M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, ldb, Cbeta, Ccmplx, cOffset, ldc); 
        }
        else {
            cgemm_NoTransB(M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, lda, Cbeta, Ccmplx, cOffset, ldc);
        }
    }
    else if (typeA == noTrans) {
        cgemm_NoTransA( M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, ldb, Cbeta, Ccmplx, cOffset, ldc);
    }
    else {
        cgemm_TransAB(M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, ldb, Cbeta, Ccmplx, cOffset, ldc);
    }
}
#endif
#if STEP
{
    if (typeB == noTrans) {
        if (typeA == noTrans) {
            cgemm_NoTransAB_batch(M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, ldb, Cbeta, Ccmplx, cOffset, ldc); 
        }
        else {
            cgemm_NoTransB_batch(M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, lda, Cbeta, Ccmplx, cOffset, ldc);
        }
    }
    else if (typeA == noTrans) {
        cgemm_NoTransA_batch( M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, ldb, Cbeta, Ccmplx, cOffset, ldc);
    }
    else {
        cgemm_TransAB_batch(M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, ldb, Cbeta, Ccmplx, cOffset, ldc);
    }
}
#endif
    for ( int i = 0 ;i <  M * N;i++) {
        C[i].real = Ccmplx[i].x;
        C[i].img = Ccmplx[i].y;
    }
   
    return AMPBLAS_SUCCESS;

}

