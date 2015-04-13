#include "ampblaslib.h"
#include<assert.h>
#include <vector>
#include <amp.h>
#include <iostream>
#include <amp_short_vectors.h>
#include <amp_math.h>
using namespace Concurrency;
using namespace Concurrency::graphics;
#define THREADS    16
#define GEMM_BLOCK 256
#define TILE_DIM  16

#define TILE_SZ_A 64
#define TILE_SZ_B 16
#define TILE_SZ_RATIO (TILE_SZ_A/TILE_SZ_B)
#define TILESIZE 8
#define MICROTILESIZE 1
#define STEPSIZE 128 

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

void cgemm_NoTransAB(int M, int N, int K, float_2 alpha,
             Concurrency::array_view<float_2> &A, long aOffset, long lda,
             Concurrency::array_view<float_2> &B, long bOffset, long ldb,
             float_2 beta,
             Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(M,N);

    Concurrency::parallel_for_each(grdExt , [=](index<2> idx) restrict(amp)
    {
        int row = idx[0];
        int col = idx[1];
        float tempReal = 0.0;
        float tempImg = 0.0;

        /* row by column multiplication */
        float realSum = 0.0;
        float imgSum = 0.0;

        if (row < M && col < N ) {
            for ( int i = 0 ; i < K; i++) {
                 float xAVal = A[aOffset + row + i * M].x;
                 float yAVal = A[aOffset + row + i * M].y;
                 float xBVal = B[bOffset + i + col * K].x;
                 float yBVal = B[aOffset + i + col * K].y;
                 realSum += (xAVal * xBVal) - (yAVal * yBVal);
                 imgSum += (xAVal * yBVal) + (yAVal * xBVal);
            }

            /* Multiply results with scalar complex alpha */
            float CrealValue = (realSum * alpha.x) - (imgSum * alpha.y);
            float CimgValue  = (realSum * alpha.y) + (imgSum * alpha.x);

            float xCVal = C[cOffset + row * N + col].x;
            float yCVal = C[cOffset + row * N + col].y;
            /* Multiply C matrix with scalar beta complex number */
            tempReal = xCVal * beta.x - yCVal * beta.y;
            tempImg = xCVal * beta.y + yCVal * beta.x;

            /* Add both the results and store in C Matrix */
            C[cOffset + col * M + row].x = tempReal + CrealValue;
            C[cOffset + col * M + row].y = tempImg + CimgValue;
       }

  });

  C.synchronize();
}

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




void cgemm_NoTransA(int M, int N, int K, float_2 alpha,
                    Concurrency::array_view<float_2> &A, long aOffset, long lda,
                    Concurrency::array_view<float_2> &B, long bOffset, long ldb,
                    float_2 beta,
                    Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(M,N);

    Concurrency::parallel_for_each(grdExt, [=](index<2> idx) restrict(amp)
    {
        int row = idx[0];
        int col = idx[1];
        float tempReal = 0.0;
        float tempImg = 0.0;

        /* column by column multiplication */
        float realSum = 0.0;
        float imgSum = 0.0;

        if (row < M && col < N ) {
            for (int i = 0 ; i < K; i++) {
                float xAVal = A[aOffset + row + i * M].x;
                float xBVal = B[bOffset + col + i * N].x;
                float yAVal = A[aOffset + row + i * M].y;
                float yBVal = B[bOffset + col + i * N].y;

                realSum += (xAVal * xBVal - yAVal * yBVal);
                imgSum += (xAVal * yBVal + yAVal * xBVal);
            }

            /* Multiply results with scalar complex alpha */
            float CrealValue = (realSum * alpha.x) - (imgSum * alpha.y);
            float CimgValue  = (realSum * alpha.y) + (imgSum * alpha.x);

            float xCVal = C[cOffset + row * N + col].x;
            float yCVal = C[cOffset + row * N + col].y;

            /* Multiply C matrix with scalar beta complex number */
            tempReal = xCVal * beta.x - yCVal * beta.y;
            tempImg = xCVal * beta.y + yCVal * beta.x;

            /* Add both the results and store in C Matrix */
            C[cOffset + col * M + row].x = tempReal + CrealValue;
            C[cOffset + col * M + row].y = tempImg + CimgValue;
        }
    });
    C.synchronize();
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
void cgemm_NoTransB(int M, int N, int K, float_2 alpha,
                    Concurrency::array_view<float_2> &A, long aOffset, long lda,
                    Concurrency::array_view<float_2> &B, long bOffset, long ldb,
                    float_2 beta,
                    Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(M,N);

    Concurrency::parallel_for_each(grdExt , [=](index<2> idx) restrict(amp)
    {
        int row = idx[0];
        int col = idx[1];
        float tempReal = 0.0;
        float tempImg = 0.0;
      
        /* row by row multiplication */
        float realSum = 0.0;
        float imgSum = 0.0;
      
        if (row < M && col < N ) {
            for ( int i = 0 ; i < K; i++) {
                float xAVal = A[(row * K) + i].x;
                float xBVal = B[(col * K) + i].x;
                float yAVal = A[(row * K) + i].y;
                float yBVal = B[(col * K) + i].y;

                realSum += (xAVal * xBVal - yAVal * yBVal);
                imgSum += (xAVal * yBVal + yAVal * xBVal);
            }
      
            /* Multiply results with scalar complex alpha */      
            float CrealValue = (realSum * alpha.x) - (imgSum * alpha.y); 
            float CimgValue  = (realSum * alpha.y) + (imgSum * alpha.x);

            float xCVal = C[cOffset + row * N + col].x;
            float yCVal = C[cOffset + row * N + col].y ;

            /* Multiply C matrix with scalar beta complex number */
            tempReal = xCVal * beta.x - yCVal * beta.y;
            tempImg = xCVal * beta.y + yCVal * beta.x;
     
            /* Add both the results and store in C Matrix */
            C[cOffset + col * M + row].x = tempReal + CrealValue;
            C[cOffset + col * M + row].y = tempImg + CimgValue;
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

void cgemm_TransAB(int M, int N, int K, float_2 alpha,
                   Concurrency::array_view<float_2> &A, long aOffset, long lda,
                   Concurrency::array_view<float_2> &B, long bOffset, long ldb,
                   float_2 beta,
                   Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(M,N);

    Concurrency::parallel_for_each(grdExt , [=](index<2> idx) restrict(amp)
    {
        int row = idx[0];
        int col = idx[1];
        float tempReal = 0.0;
        float tempImg = 0.0;

        /* row by column multiplication */
        float realSum = 0.0;
        float imgSum = 0.0;

        if (row < M && col < N ) {
            for ( int i = 0 ; i < K; i++) {
                float xAVal = A[aOffset + row * K + i].x;
                float yAVal = A[aOffset + row * K + i].y;
                float xBVal = B[bOffset + i * N + col].x;
                float yBVal = B[bOffset + i * N + col].y;
                realSum += xAVal * xBVal - yAVal * yBVal;
                imgSum += xAVal * yBVal + yAVal * xBVal;
            }

            /* Multiply results with scalar complex alpha */
            float CrealValue = (realSum * alpha.x) - (imgSum * alpha.y);
            float CimgValue  = (realSum * alpha.y) + (imgSum * alpha.x);

            float xCVal = C[cOffset + row * N + col].x;
            float yCVal = C[cOffset + row * N + col].y;

            /* Multiply C matrix with scalar beta complex number */
            tempReal = xCVal * beta.x - yCVal * beta.y;
            tempImg = xCVal * beta.y + yCVal * beta.x;

            /* Add both the results and store in C Matrix */
            C[cOffset + col * M + row].x = tempReal + CrealValue;
            C[cOffset + col * M + row].y = tempImg + CimgValue;
        }
    });
    C.synchronize();
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

    for ( int i = 0 ;i <  M * N;i++) {
        C[i].real = Ccmplx[i].x;
        C[i].img = Ccmplx[i].y;
    }
   
    return AMPBLAS_SUCCESS;

}

