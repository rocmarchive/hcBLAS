#include "ampblaslib.h"
#include <amp.h>
#include <amp_math.h>
using namespace Concurrency;

#define BANKTILESIZE (TILESIZE + 1)
#define STEPTILERATIO STEPSIZE/TILESIZE 
#define STEPTILEPROD STEPSIZE*TILESIZE
#define BANKNUMTILEELMTS TILESIZE*BANKTILESIZE
#define NUMTILEELMTS TILESIZE*TILESIZE

#define TOTMICROTILEPROD (TILESIZE*TILESIZE*MICROTILESIZE)
#define MICROTILEPROD (TILESIZE*MICROTILESIZE)
#define BANKMICROTILESIZE (TILESIZE*MICROTILESIZE+1)

#define  MS1x1(offsetA, offsetB)			\
            for(int iter = 0; iter < STEPSIZE/TILESIZE; ++iter) \
            {\
              rA[0][iter] = lA[offA + (TILESIZE * TILESIZE) * iter];	\
              rB[0][iter] = lB[offB + (TILESIZE * TILESIZE) * iter];	\
              rC[0][0] +=rA[0][iter] *rB[0][iter]; \
            }\
            offA += offsetA;			\
            offB += offsetB;			\

#define  MS1x1_NOBANK(offset)			\
            for(int iter = 0; iter < STEPTILERATIO; ++iter) \
            {\
              rA[0][iter] = lA[offA + (BANKNUMTILEELMTS) * iter];	\
              rB[0][iter] = lB[offB + (BANKNUMTILEELMTS) * iter];	\
              rC[0][0] +=rA[0][iter] *rB[0][iter]; \
            }\
            offA += offset;			\
            offB += offset;			\

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

#define  MTS_NOBANK                                                         \
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
           offA += BANKMICROTILESIZE;                              \
           offB += BANKMICROTILESIZE;                              \

static ampblasStatus gemm_NoTransAB(Concurrency::accelerator_view &accl_view,
                                    Concurrency::array_view<float, 1> &A, long aOffset,
                                    Concurrency::array_view<float, 1> &B, long bOffset,
                                    Concurrency::array_view<float, 1> &C, long cOffset,
                                    int M, int N, int K, int lda, int ldb, int ldc,
                                    float alpha, float beta)
{
  if ((M < 600 && N < 600 && K < 10) || (M < 1800 && N < 600 && K < 600)) 
  {
#define TILESIZE 8
#define STEPSIZE 8
      cout<<"\nSTEP NBK 8 8"<<endl;
      Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);
      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
        float rC[1][1] = {{0.0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];//8*8+8
        tile_static float lB[STEPTILEPROD + STEPSIZE];
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
            if(gidy*TILESIZE+idyT  < N && (idxT + i * STEPSIZE + (TILESIZE * sec)) < K) 
              lB[idyT * BANKTILESIZE + idxT + (BANKNUMTILEELMTS * sec)] = B[bOffset + (gidy*TILESIZE+ idyT) * ldb + idxT + i * STEPSIZE + (TILESIZE * sec)];
            else
              lB[idyT * BANKTILESIZE + idxT + (BANKNUMTILEELMTS * sec)] = 0;

            // Load Section 'sec' from global memory A onto shared lA
            if(gidx * TILESIZE + idxT < M && (i * STEPSIZE + idyT + (TILESIZE * sec)) < K)
               lA[idxT * BANKTILESIZE + idyT + (BANKNUMTILEELMTS * sec)] = A[aOffset  + gidx*TILESIZE+ idxT + idyT*lda + i * (lda << shiftFactor) + (TILESIZE * sec) * lda];
            else
               lA[idxT * BANKTILESIZE + idyT + (BANKNUMTILEELMTS * sec)] = 0;
          }
          tidx.barrier.wait();

          int offA = idx * BANKTILESIZE;
          int offB = idy * BANKTILESIZE;

          for (int iter=0; iter < TILESIZE; ++iter)
          {
             MS1x1_NOBANK(1);
          }

          i++;
        } while (--block_k > 0);


        tidx.barrier.wait();
        if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N)
            C[cOffset + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc] = alpha * rC[0][0] + beta * C[cOffset + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc];
      });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if ((M < 600 && N < 600 && K < 1800) || (M < 1800 && ((N < 600 && K < 1800) || (N < 1800 && K < 10))))
  {
#define TILESIZE 16
#define STEPSIZE 16
      cout<<"\nSTEP NBK 16 16"<<endl;
      Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);
      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
        float rC[1][1] = {{0.0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];//8*8+8
        tile_static float lB[STEPTILEPROD + STEPSIZE];
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
            if(gidy*TILESIZE+idyT  < N && (idxT + i * STEPSIZE + (TILESIZE * sec)) < K) 
              lB[idyT * BANKTILESIZE + idxT + (BANKNUMTILEELMTS * sec)] = B[bOffset + (gidy*TILESIZE+ idyT) * ldb + idxT + i * STEPSIZE + (TILESIZE * sec)];
            else
              lB[idyT * BANKTILESIZE + idxT + (BANKNUMTILEELMTS * sec)] = 0;

            // Load Section 'sec' from global memory A onto shared lA
            if(gidx * TILESIZE + idxT < M && (i * STEPSIZE + idyT + (TILESIZE * sec)) < K)
               lA[idxT * BANKTILESIZE + idyT + (BANKNUMTILEELMTS * sec)] = A[aOffset  + gidx*TILESIZE+ idxT + idyT*lda + i * (lda << shiftFactor) + (TILESIZE * sec) * lda];
            else
               lA[idxT * BANKTILESIZE + idyT + (BANKNUMTILEELMTS * sec)] = 0;
          }
          tidx.barrier.wait();

          int offA = idx * BANKTILESIZE;
          int offB = idy * BANKTILESIZE;

          for (int iter=0; iter < TILESIZE; ++iter)
          {
             MS1x1_NOBANK(1);
          }

          i++;
        } while (--block_k > 0);


        tidx.barrier.wait();
        if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N)
            C[cOffset + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc] = alpha * rC[0][0] + beta * C[cOffset + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc];
      });
#undef TILESIZE
#undef STEPSIZE
     return AMPBLAS_SUCCESS;
  }
  else if (M < 1800 && N < 1800 && K < 1800)
  {
#define TILESIZE 16
#define MICROTILESIZE 2
      cout<<"\n MICRO 16 2"<<endl;
      Concurrency::extent<2> grdExt(((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        float rC[MICROTILESIZE][MICROTILESIZE];
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
#undef TILESIZE
#undef MICROTILESIZE
    return AMPBLAS_SUCCESS;
  }
  else {
    cout<<"Input matrix Size  "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<"doesnot comes under wrapper sizes"<<endl;
    return AMPBLAS_ERROR;
  }
}
static ampblasStatus gemm_NoTransA(Concurrency::accelerator_view &accl_view,
	                           Concurrency::array_view<float, 1> &A, long aOffset,
                                   Concurrency::array_view<float, 1> &B, long bOffset,
                                   Concurrency::array_view<float, 1> &C, long cOffset,
                                   int M, int N, int K, int lda, int ldb, int ldc,
                                   float alpha, float beta)
{
  if ((M < 10 && N < 1800 && K < 600) || (M < 600 && ((N < 600 && K < 1800) || (N < 1800 && K < 10))))
  {
#define TILESIZE 16
#define STEPSIZE 16
      cout<<"\nSTEP NBK 16 16"<<endl;
      Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);
      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
      { 
        int tilemulshift = (int)Concurrency::fast_math::log2(TILESIZE);
        int shiftfactor = Concurrency::fast_math::log2(STEPSIZE);
        int block_k = ((K + (STEPSIZE - 1)) & ~(STEPSIZE - 1)) >> shiftfactor;
        float rC[1][1] = {{0.0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];
        tile_static float lB[STEPTILEPROD + STEPSIZE];
        int gidx = tidx.tile[1];
        int gidy = tidx.tile[0];
        int idx = tidx.local[1];
        int idy = tidx.local[0];
        int idt = (idy << tilemulshift) + idx;
        int idyT = idt >> tilemulshift;
        int idxT = idt & (TILESIZE - 1);
        int gidyOffset = gidy << tilemulshift;
        int gidxOffset = gidx << tilemulshift;
        int idyTOffset = idyT * BANKTILESIZE;

        int i = 0;
        do
        {
          tidx.barrier.wait();
          int iOffset = i << shiftfactor;
          for(int sec = 0; sec < STEPTILERATIO; ++sec)
          {
            int secOffset  = sec << tilemulshift;
            int secStartPt = (sec << tilemulshift) * BANKTILESIZE;
            int localIdx = secStartPt + idxT + idyTOffset;
            int kIndex = iOffset + idyT + secOffset;

            // Initialize the local memory with zero
            lB[localIdx] = 0;
            lA[localIdx] = 0;

            if(gidyOffset + idxT < N && kIndex < K)
            {
              lB[localIdx] = B[bOffset + gidyOffset + idxT + kIndex * ldb];
            }

            if(gidxOffset + idxT < M && kIndex < K)
            {
              lA[localIdx] = A[aOffset + gidxOffset + idxT + kIndex * lda];
            }
          }
          tidx.barrier.wait();

          int offA = idx;
          int offB = idy;

          for (int iter=0; iter < TILESIZE; ++iter)
          {
            MS1x1_NOBANK(BANKTILESIZE);
          }

          i++;
        } while (--block_k > 0);
        tidx.barrier.wait();
        int crow = gidxOffset + idx;
        int ccolprod = (gidyOffset + idy) * ldc;
        if(crow < M && ccolprod/ldc < N)
          C[cOffset + crow + ccolprod] =  alpha * rC[0][0] + beta * C[cOffset + crow + ccolprod];
      });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if ((M < 10 && N < 1800 && K < 1800) || (M < 600 && N < 10 && K < 1800) || (M < 1800 && N < 10 && K < 1800))
  {
#define TILESIZE 16
#define STEPSIZE 16
        cout<<"\nSTEP 16 16"<<endl;
        Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
        Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

        Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
        { 
        int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
        float rC[1][1] = {{(float)0}};
        float rA[1][STEPSIZE / TILESIZE];
        float rB[1][STEPSIZE / TILESIZE];
        tile_static float lA[TILESIZE * STEPSIZE];
        tile_static float lB[TILESIZE * STEPSIZE];
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
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if ((M < 600 && N < 1800 && K < 1800) || (M < 1800 && N < 1800 && K < 600))
  {
#define TILESIZE 16
#define MICROTILESIZE 2
    cout<<"\n MICRO NBK 16 2"<<endl;
    Concurrency::extent<2> grdExt(((N >> 1) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M >> 1) + (TILESIZE - 1)) & ~(TILESIZE - 1));
    Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
    {
        int shiftTS = Concurrency::fast_math::log2(TILESIZE);
        float rC[MICROTILESIZE][MICROTILESIZE] = {{(float)0}};
        float rA[1][MICROTILESIZE];
        float rB[1][MICROTILESIZE];
        tile_static float lA[TOTMICROTILEPROD + TILESIZE];
        tile_static float lB[TOTMICROTILEPROD + TILESIZE];
        int gidx = tidx.tile[1];
        int gidy = tidx.tile[0];
        int idx = tidx.local[1];
        int idy = tidx.local[0];
        int idt = ( idy << shiftTS) + idx;
        int idxT = idt & ( TILESIZE - 1);
        int idyT = idt >> shiftTS;
        int block_k = 0;
        do
        {
          int colIndex = ( block_k << shiftTS ) + idyT;
          int lIndex = (idyT * BANKMICROTILESIZE) + idxT;

          tidx.barrier.wait();
          for(int sec = 0; sec < MICROTILESIZE; ++sec)
          {
            int secVal = sec << shiftTS;
            int BrowIndex = (gidy * MICROTILEPROD) + idxT + secVal;
            int ArowIndex = (gidx * MICROTILEPROD) + idxT + secVal;

            if( BrowIndex < N && colIndex < K)
            {
              lB[ lIndex + secVal] = B[bOffset + BrowIndex + colIndex * ldb];
            }
            else
            {
              lB[lIndex + secVal] = 0;
            }

            if(ArowIndex < M && colIndex < K)
            {
              lA[lIndex + secVal] = A[aOffset + ArowIndex + colIndex * lda];
            }
            else
            {
              lA[lIndex + secVal] = 0;
            }
          }
          tidx.barrier.wait();

          int offA = idx;
          int offB = idy;
          for (int iter=0; iter < TILESIZE; ++iter)
          {
            MTS_NOBANK;
          }
          tidx.barrier.wait();
        } while (++block_k < (((K + TILESIZE - 1) & ~(TILESIZE - 1)) >> shiftTS));

        int xIndex = (gidx * MICROTILEPROD) + idx;
        int yIndex = ((gidy * MICROTILEPROD) + idy) * ldc;
        for( int row = 0; row < MICROTILESIZE; row++)
        {
          for( int col = 0; col < MICROTILESIZE ; col++)
          {
          if(xIndex + (col << shiftTS) < M && (yIndex / ldc) + (row << shiftTS) < N)
            C[cOffset + (xIndex + (col << shiftTS)) + yIndex + (row << shiftTS) * ldc] = alpha * rC[col][row] + beta * C[cOffset + (xIndex + (col << shiftTS)) + yIndex + (row << shiftTS) * ldc];
          }
        }
    });
#undef TILESIZE
#undef MICROTILESIZE
    return AMPBLAS_SUCCESS;
  }
  else if (M < 1800 && N < 1800 && K < 1800)
  {
#define TILESIZE 16
#define MICROTILESIZE 2
      cout<<"\n MICRO 16 2"<<endl;
      Concurrency::extent<2> grdExt(((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        float rC[MICROTILESIZE][MICROTILESIZE];
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
#undef TILESIZE 
#undef MICROTILESIZE
    return AMPBLAS_SUCCESS;
  }
  else {
      cout<<"Input matrix Size  "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<"doesnot comes under wrapper sizes"<<endl;
      return AMPBLAS_ERROR;
  }
}

static ampblasStatus gemm_NoTransB(Concurrency::accelerator_view &accl_view,
          		  Concurrency::array_view<float, 1> &A, long aOffset,
                          Concurrency::array_view<float, 1> &B, long bOffset,
                          Concurrency::array_view<float, 1> &C, long cOffset,
                          int M, int N, int K, int lda, int ldb, int ldc,
                          float alpha, float beta)
{
  if ((M > 10 && M < 600) && N < 10 && K < 10)
  {
#define TILESIZE 8
#define STEPSIZE 8
      cout<<"\n STEP NBK 8 8"<<endl;
      Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int tilemulshift = (int)Concurrency::fast_math::log2(TILESIZE);
        int shiftfactor = (int)Concurrency::fast_math::log2(STEPSIZE);
        int block_k =((K + (STEPSIZE - 1)) & ~(STEPSIZE - 1)) >> shiftfactor;
        float rC[1][1] = {{0.0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];
        tile_static float lB[STEPTILEPROD + STEPSIZE];
        int gidx = tidx.tile[1];
        int gidy = tidx.tile[0];
        int idx = tidx.local[1];
        int idy = tidx.local[0];
        int idt = (idy << tilemulshift) + idx; //(idy * TILESIZE + idx)
        int ids = (idy << shiftfactor) + idx; //(idy * STEPSIZE + idx)
        int idxS = ids & (STEPSIZE - 1);
      
       
        int idyT = (idt)>> tilemulshift;
        int gidyOffset = gidy << tilemulshift;
        int gidxOffset = gidx << tilemulshift;
        int idyTOffset = idyT * BANKTILESIZE;


        int i = 0;
        do
        {
          tidx.barrier.wait();
          int iOffset = i << shiftfactor;
          for(int sec = 0; sec < STEPTILERATIO; ++sec)
          {
            int secOffset  = sec << tilemulshift;
            int secStartPt = (sec << tilemulshift) * BANKTILESIZE;
            int localIdx = secStartPt + idxS + idyTOffset;
            int kIndex = iOffset + idxS + secOffset;

            // Initialize the local memory with zero
            lB[localIdx] = 0;
            lA[localIdx] = 0;

            if(gidyOffset + idyT < N && kIndex < K)
            {
              lB[localIdx] = B[bOffset + (gidyOffset + idyT) * ldb + kIndex];
            }
            if(gidxOffset + idyT < M && kIndex < K)
            {
              lA[localIdx] = A[aOffset + (gidxOffset + idyT) * lda + kIndex];
            }
          }

          tidx.barrier.wait();

          int offA = idx * BANKTILESIZE;
          int offB = idy * BANKTILESIZE;
          
          for (int piter=0; piter < TILESIZE; ++piter)
          {
            MS1x1_NOBANK(1);
          }

          i++;

        }while (--block_k > 0);

        tidx.barrier.wait();

        int crow = gidxOffset + idx;
        int ccolprod = (gidyOffset + idy) * ldc;
        if(crow < M && ccolprod/ldc < N)
            C[cOffset + crow + ccolprod] = alpha * rC[0][0] + beta * C[cOffset + crow + ccolprod];
      });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if ((M > 10 && M < 600) && N < 600 && K < 10)
  {
#define TILESIZE 8
#define STEPSIZE 8
    cout<<"\n STEP 8 8"<<endl;
    Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
    Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
    {
    int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
    float rC[1][1] = {{(float)0}};
    float rA[1][STEPSIZE / TILESIZE];
    float rB[1][STEPSIZE / TILESIZE];
    tile_static float lA[TILESIZE * STEPSIZE];
    tile_static float lB[TILESIZE * STEPSIZE];
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
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if (((M < 10 && N < 1800) || (M < 1800 && N < 10) || (M < 600 && N < 1800) || (M < 1800 && N < 10)) && K < 1800)
  {
#define TILESIZE 16
#define STEPSIZE 16
    cout<<"\n STEP NBK 16 16"<<endl;
    Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
    Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
    {
    int tilemulshift = (int)Concurrency::fast_math::log2(TILESIZE);
    int shiftfactor = (int)Concurrency::fast_math::log2(STEPSIZE);
    int block_k =((K + (STEPSIZE - 1)) & ~(STEPSIZE - 1)) >> shiftfactor;
    float rC[1][1] = {{0.0}};
    float rA[1][STEPTILERATIO];
    float rB[1][STEPTILERATIO];
    tile_static float lA[STEPTILEPROD + STEPSIZE];
    tile_static float lB[STEPTILEPROD + STEPSIZE];
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = (idy << tilemulshift) + idx; //(idy * TILESIZE + idx)
    int ids = (idy << shiftfactor) + idx; //(idy * STEPSIZE + idx)
    int idxS = ids & (STEPSIZE - 1);


    int idyT = (idt)>> tilemulshift;
    int gidyOffset = gidy << tilemulshift;
    int gidxOffset = gidx << tilemulshift;
    int idyTOffset = idyT * BANKTILESIZE;


    int i = 0;
    do
    {
      tidx.barrier.wait();
      int iOffset = i << shiftfactor;
      for(int sec = 0; sec < STEPTILERATIO; ++sec)
      {
        int secOffset  = sec << tilemulshift;
        int secStartPt = (sec << tilemulshift) * BANKTILESIZE;
        int localIdx = secStartPt + idxS + idyTOffset;
        int kIndex = iOffset + idxS + secOffset;

        // Initialize the local memory with zero
        lB[localIdx] = 0;
        lA[localIdx] = 0;

        if(gidyOffset + idyT < N && kIndex < K)
        {
          lB[localIdx] = B[bOffset + (gidyOffset + idyT) * ldb + kIndex];
        }
        if(gidxOffset + idyT < M && kIndex < K)
        {
          lA[localIdx] = A[aOffset + (gidxOffset + idyT) * lda + kIndex];
        }
      }

      tidx.barrier.wait();

      int offA = idx * BANKTILESIZE;
      int offB = idy * BANKTILESIZE;
      
      for (int piter=0; piter < TILESIZE; ++piter)
      {
        MS1x1_NOBANK(1);
      }

      i++;

    }while (--block_k > 0);

    tidx.barrier.wait();

    int crow = gidxOffset + idx;
    int ccolprod = (gidyOffset + idy) * ldc;
    if(crow < M && ccolprod/ldc < N)
        C[cOffset + crow + ccolprod] = alpha * rC[0][0] + beta * C[cOffset + crow + ccolprod];
    });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if (M < 1800 && N < 1800 && K < 600)  
  {
#define TILESIZE 16
#define MICROTILESIZE 2
    cout<<"\n MICRO NBK 16 2"<<endl;
    Concurrency::extent<2> grdExt(((N >> 1) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M >> 1) + (TILESIZE - 1)) & ~(TILESIZE - 1));
    Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
    {
    int shiftTS = Concurrency::fast_math::log2(TILESIZE);
    float rC[MICROTILESIZE][MICROTILESIZE] = {{(float)0}};
    float rA[1][MICROTILESIZE];
    float rB[1][MICROTILESIZE];
    tile_static float lA[TOTMICROTILEPROD + TILESIZE];
    tile_static float lB[TOTMICROTILEPROD + TILESIZE];
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int idt = ( idy << shiftTS ) + idx;
    int idxT = idt % TILESIZE ;
    int idyT = idt / TILESIZE;
    int block_k = 0;
    do
    {
      int colIndex =( block_k << shiftTS )+ idyT;
      int lIndex = (idyT * BANKMICROTILESIZE) + idxT;

      tidx.barrier.wait();
      for(int sec = 0; sec < MICROTILESIZE; ++sec)
      {
        int secVal = sec << shiftTS;
        int BrowIndex = ( gidy * MICROTILEPROD) + idxT + secVal;
        int ArowIndex = ( gidx * MICROTILEPROD) + idxT + secVal;

        tidx.barrier.wait();
        if( BrowIndex < N && colIndex < K)
        {
          lB[ lIndex + secVal] = B[ bOffset + BrowIndex * ldb + colIndex ];
        }
        else
        {
          lB[ lIndex + secVal] = 0;
        }

        if( ArowIndex < M && colIndex < K)
        {
          lA[ lIndex + secVal] = A[aOffset + ArowIndex * lda +  colIndex];
        }
        else
        {
          lA[ lIndex + secVal] = 0;
        }
      }
      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;
      for (int iter=0; iter < TILESIZE; ++iter)
      {
        MTS_NOBANK;
      }
      tidx.barrier.wait();
    } while (++block_k < (((K + TILESIZE - 1) & ~(TILESIZE - 1)) >> shiftTS));

    int xIndex = (gidx * MICROTILEPROD) + idx;
    int yIndex = ((gidy * MICROTILEPROD) + idy) * ldc;
    for( int row = 0; row < MICROTILESIZE; row++)
    {
      for( int col = 0; col < MICROTILESIZE ; col++)
      {
      if(xIndex + (col << shiftTS) < M && (yIndex / ldc) + (row << shiftTS) < N)
        C[cOffset + (xIndex + (col << shiftTS)) + yIndex + (row << shiftTS ) * ldc] = alpha * rC[col][row] + beta * C[cOffset + (xIndex + (col << shiftTS)) + yIndex + (row << shiftTS) * ldc];
      }
    }
    });
#undef TILESIZE
#undef MICROTILESIZE
    return AMPBLAS_SUCCESS;
  }
  else if (M < 1800 && N < 1800 && K < 1800)
  {
#define TILESIZE 16
#define MICROTILESIZE 2
    cout<<"\n MICRO 16 2"<<endl;
    Concurrency::extent<2> grdExt(((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
    Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
    {
        float rC[MICROTILESIZE][MICROTILESIZE] = {{(float)0}};
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
#undef TILESIZE
#undef MICROTILESIZE
    return AMPBLAS_SUCCESS;
  }
  else {
     cout<<"Input matrix Size "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<"is not covered under wrapper sizes"<<endl;
     return AMPBLAS_ERROR;
  }
}
static ampblasStatus gemm_TransAB(Concurrency::accelerator_view &accl_view,
                                  Concurrency::array_view<float, 1> &A, long aOffset,
                                  Concurrency::array_view<float, 1> &B, long bOffset,
                                  Concurrency::array_view<float, 1> &C, long cOffset,
                                  int M, int N, int K, int lda, int ldb, int ldc,
                                  float alpha, float beta)
{
  if ((M < 600 && N < 600 && K < 10) || (M < 1800 && N < 600 && K < 600))
  {
#define TILESIZE 8
#define STEPSIZE 8
      cout<<"\n STEP NBK 8 8"<<endl;
      Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
        float rC[1][1] = {{(float)0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];
        tile_static float lB[STEPTILEPROD + STEPSIZE];
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
              lB[((idxT + sec * TILESIZE) * BANKTILESIZE) + idyT] = B[bOffset + gidy * TILESIZE + idxT + ((idyT + (sec * TILESIZE)) *ldb) + i * (ldb << shiftFactor)];
            }
            else
            {
              lB[((idxT + sec * TILESIZE) * BANKTILESIZE) + idyT] = 0;
            }

            if(gidx * TILESIZE + idxT < M && i * STEPSIZE + idyT + (sec * TILESIZE) < K)
            {
              lA[(sec * BANKNUMTILEELMTS) + idyT + idxT * BANKTILESIZE] = A[aOffset  + (gidx * TILESIZE + idxT) * lda + idyT + i * STEPSIZE + (sec * TILESIZE)];
            }
            else
            {
              lA[(sec * BANKNUMTILEELMTS ) + idyT + idxT * BANKTILESIZE] = 0;
            }
          }
          tidx.barrier.wait();

          int offA = idx * BANKTILESIZE;
          int offB = idy * BANKTILESIZE;
          for (int iter = 0; iter < TILESIZE; ++iter)
          {
            MS1x1_NOBANK(1);
          }
          i++;
        } while (--block_k > 0);


        tidx.barrier.wait();
        if(gidx * TILESIZE + idx < M && gidy * TILESIZE + idy < N)
            C[cOffset + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc] = alpha * rC[0][0] + beta * C[cOffset + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc];
      });
#undef TILESIZE
#undef STEPSIZE
     return AMPBLAS_SUCCESS;
  }
  if ((M < 600 && N < 600 && K < 1800) || (M < 1800 && ((N < 600 && K < 1800) || (N < 1800 && K < 10))))
  {
#define TILESIZE 16
#define STEPSIZE 16
      cout<<"\n STEP NBK 16 16"<<endl;
      Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
        float rC[1][1] = {{(float)0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];
        tile_static float lB[STEPTILEPROD + STEPSIZE];
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
              lB[((idxT + sec * TILESIZE) * BANKTILESIZE) + idyT] = B[bOffset + gidy * TILESIZE + idxT + ((idyT + (sec * TILESIZE)) *ldb) + i * (ldb << shiftFactor)];
            }
            else
            {
              lB[((idxT + sec * TILESIZE) * BANKTILESIZE) + idyT] = 0;
            }

            if(gidx * TILESIZE + idxT < M && i * STEPSIZE + idyT + (sec * TILESIZE) < K)
            {
              lA[(sec * BANKNUMTILEELMTS) + idyT + idxT * BANKTILESIZE] = A[aOffset  + (gidx * TILESIZE + idxT) * lda + idyT + i * STEPSIZE + (sec * TILESIZE)];
            }
            else
            {
              lA[(sec * BANKNUMTILEELMTS ) + idyT + idxT * BANKTILESIZE] = 0;
            }
          }
          tidx.barrier.wait();

          int offA = idx * BANKTILESIZE;
          int offB = idy * BANKTILESIZE;
          for (int iter = 0; iter < TILESIZE; ++iter)
          {
            MS1x1_NOBANK(1);
          }
          i++;
        } while (--block_k > 0);


        tidx.barrier.wait();
        if(gidx * TILESIZE + idx < M && gidy * TILESIZE + idy < N)
            C[cOffset + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc] = alpha * rC[0][0] + beta * C[cOffset + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc];
      });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  
  else if (M < 1800 && N < 1800 && K < 1800)
  {
#define TILESIZE 16
#define MICROTILESIZE 2
      cout<<"\n MICRO 16 2"<<endl;
      Concurrency::extent<2> grdExt(((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        float rC[MICROTILESIZE][MICROTILESIZE] = {{(float)0}};
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
#undef TILESIZE
#undef MICROTILESIZE 
    return AMPBLAS_SUCCESS;
  }
  else  {
     cout<<"Input matrix Size "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<" not covered under wrapper sizes"<<endl; 
     return AMPBLAS_ERROR;
  }
}


/*................................batch..........................................*/



static ampblasStatus gemm_NoTransAB(Concurrency::accelerator_view &accl_view,
                                    Concurrency::array_view<float, 1> &A, long aOffset, long A_batchOffset,
                                    Concurrency::array_view<float, 1> &B, long bOffset, long B_batchOffset,
                                    Concurrency::array_view<float, 1> &C, long cOffset, long C_batchOffset,
                                    int M, int N, int K, int lda, int ldb, int ldc,
                                    float alpha, float beta, int batchSize)
{
  if ((M < 600 && N < 600 && K < 10) || (M < 1800 && N < 600 && K < 600)) 
  {
#define TILESIZE 8
#define STEPSIZE 8
      cout<<"\nSTEP NBK 8 8"<<endl;
      Concurrency::extent<3> grdExt(batchSize,(N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);
      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int elt = tidx.tile[0];
        int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
        float rC[1][1] = {{0.0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];//8*8+8
        tile_static float lB[STEPTILEPROD + STEPSIZE];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
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
            if(gidy*TILESIZE+idyT  < N && (idxT + i * STEPSIZE + (TILESIZE * sec)) < K) 
              lB[idyT * BANKTILESIZE + idxT + (BANKNUMTILEELMTS * sec)] = B[bOffset + B_batchOffset * elt +(gidy*TILESIZE+ idyT) * ldb + idxT + i * STEPSIZE + (TILESIZE * sec)];
            else
              lB[idyT * BANKTILESIZE + idxT + (BANKNUMTILEELMTS * sec)] = 0;

            // Load Section 'sec' from global memory A onto shared lA
            if(gidx * TILESIZE + idxT < M && (i * STEPSIZE + idyT + (TILESIZE * sec)) < K)
               lA[idxT * BANKTILESIZE + idyT + (BANKNUMTILEELMTS * sec)] = A[aOffset + A_batchOffset * elt + gidx*TILESIZE+ idxT + idyT*lda + i * (lda << shiftFactor) + (TILESIZE * sec) * lda];
            else
               lA[idxT * BANKTILESIZE + idyT + (BANKNUMTILEELMTS * sec)] = 0;
          }
          tidx.barrier.wait();

          int offA = idx * BANKTILESIZE;
          int offB = idy * BANKTILESIZE;

          for (int iter=0; iter < TILESIZE; ++iter)
          {
             MS1x1_NOBANK(1);
          }

          i++;
        } while (--block_k > 0);


        tidx.barrier.wait();
        if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N)
            C[cOffset + C_batchOffset * elt + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc] = alpha * rC[0][0] + beta * C[cOffset + C_batchOffset * elt + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc];
      });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if ((M < 600 && N < 600 && K < 1800) || (M < 1800 && ((N < 600 && K < 1800) || (N < 1800 && K < 10))))
  {
#define TILESIZE 16
#define STEPSIZE 16
      cout<<"\nSTEP NBK 16 16"<<endl;
      Concurrency::extent<3> grdExt(batchSize, (N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);
      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int elt = tidx.tile[0];
        int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
        float rC[1][1] = {{0.0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];//8*8+8
        tile_static float lB[STEPTILEPROD + STEPSIZE];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
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
            if(gidy*TILESIZE+idyT  < N && (idxT + i * STEPSIZE + (TILESIZE * sec)) < K) 
              lB[idyT * BANKTILESIZE + idxT + (BANKNUMTILEELMTS * sec)] = B[bOffset + B_batchOffset * elt +(gidy*TILESIZE+ idyT) * ldb + idxT + i * STEPSIZE + (TILESIZE * sec)];
            else
              lB[idyT * BANKTILESIZE + idxT + (BANKNUMTILEELMTS * sec)] = 0;

            // Load Section 'sec' from global memory A onto shared lA
            if(gidx * TILESIZE + idxT < M && (i * STEPSIZE + idyT + (TILESIZE * sec)) < K)
               lA[idxT * BANKTILESIZE + idyT + (BANKNUMTILEELMTS * sec)] = A[aOffset  + A_batchOffset * elt + gidx*TILESIZE+ idxT + idyT*lda + i * (lda << shiftFactor) + (TILESIZE * sec) * lda];
            else
               lA[idxT * BANKTILESIZE + idyT + (BANKNUMTILEELMTS * sec)] = 0;
          }
          tidx.barrier.wait();

          int offA = idx * BANKTILESIZE;
          int offB = idy * BANKTILESIZE;

          for (int iter=0; iter < TILESIZE; ++iter)
          {
             MS1x1_NOBANK(1);
          }

          i++;
        } while (--block_k > 0);


        tidx.barrier.wait();
        if(gidx*TILESIZE+idx < M && gidy*TILESIZE+idy < N)
            C[cOffset + C_batchOffset * elt + gidx*TILESIZE +idx + (gidy*TILESIZE + idy)*ldc] = alpha * rC[0][0] + beta * C[cOffset + C_batchOffset * elt + gidx*TILESIZE+idx + (gidy*TILESIZE + idy)*ldc];
      });
#undef TILESIZE
#undef STEPSIZE
     return AMPBLAS_SUCCESS;
  }
  else if (M < 1800 && N < 1800 && K < 1800)
  {
#define TILESIZE 16
#define MICROTILESIZE 2
      cout<<"\n MICRO 16 2"<<endl;
      Concurrency::extent<3> grdExt(batchSize,((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int elt = tidx.tile[0];
        float rC[MICROTILESIZE][MICROTILESIZE];
        float rA[1][MICROTILESIZE];
        float rB[1][MICROTILESIZE];
        tile_static float lA[TILESIZE * TILESIZE * MICROTILESIZE];
        tile_static float lB[TILESIZE * TILESIZE * MICROTILESIZE];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
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
              lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = B[bOffset + B_batchOffset * elt + (gidy * TILESIZE * MICROTILESIZE + idxT + sec * TILESIZE) * ldb + idyT + block_k * TILESIZE];
            }
            else
            {
              lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
	        }

            if(gidx * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < M && block_k * TILESIZE + idyT < K)
            {
              lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = A[aOffset + A_batchOffset * elt + (gidx * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE) +  idyT * lda + block_k * (lda * TILESIZE)];
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
            C[cOffset + C_batchOffset * elt + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc] = alpha * rC[col][row] + beta * C[cOffset + C_batchOffset * elt + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc];
          }
        }
     });
#undef TILESIZE
#undef MICROTILESIZE
    return AMPBLAS_SUCCESS;
  }
  else {
    cout<<"Input matrix Size  "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<"doesnot comes under wrapper sizes"<<endl;
    return AMPBLAS_ERROR;
  }
}
static ampblasStatus gemm_NoTransA(Concurrency::accelerator_view &accl_view,
	                               Concurrency::array_view<float, 1> &A, long aOffset, long A_batchOffset,
                                   Concurrency::array_view<float, 1> &B, long bOffset, long B_batchOffset,
                                   Concurrency::array_view<float, 1> &C, long cOffset, long C_batchOffset,
                                   int M, int N, int K, int lda, int ldb, int ldc,
                                   float alpha, float beta, int batchSize)
{
  if ((M < 10 && N < 1800 && K < 600) || (M < 600 && ((N < 600 && K < 1800) || (N < 1800 && K < 10))))
  {
#define TILESIZE 16
#define STEPSIZE 16
      cout<<"\nSTEP NBK 16 16"<<endl;
      Concurrency::extent<3> grdExt(batchSize,(N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);
      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
      { 
        int tilemulshift = (int)Concurrency::fast_math::log2(TILESIZE);
        int shiftfactor = Concurrency::fast_math::log2(STEPSIZE);
        int block_k = ((K + (STEPSIZE - 1)) & ~(STEPSIZE - 1)) >> shiftfactor;
        float rC[1][1] = {{0.0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];
        tile_static float lB[STEPTILEPROD + STEPSIZE];
        int elt = tidx.tile[0];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
        int idt = (idy << tilemulshift) + idx;
        int idyT = idt >> tilemulshift;
        int idxT = idt & (TILESIZE - 1);
        int gidyOffset = gidy << tilemulshift;
        int gidxOffset = gidx << tilemulshift;
        int idyTOffset = idyT * BANKTILESIZE;

        int i = 0;
        do
        {
          tidx.barrier.wait();
          int iOffset = i << shiftfactor;
          for(int sec = 0; sec < STEPTILERATIO; ++sec)
          {
            int secOffset  = sec << tilemulshift;
            int secStartPt = (sec << tilemulshift) * BANKTILESIZE;
            int localIdx = secStartPt + idxT + idyTOffset;
            int kIndex = iOffset + idyT + secOffset;

            // Initialize the local memory with zero
            lB[localIdx] = 0;
            lA[localIdx] = 0;

            if(gidyOffset + idxT < N && kIndex < K)
            {
              lB[localIdx] = B[bOffset + B_batchOffset * elt + gidyOffset + idxT + kIndex * ldb];
            }

            if(gidxOffset + idxT < M && kIndex < K)
            {
              lA[localIdx] = A[aOffset + A_batchOffset * elt + gidxOffset + idxT + kIndex * lda];
            }
          }
          tidx.barrier.wait();

          int offA = idx;
          int offB = idy;

          for (int iter=0; iter < TILESIZE; ++iter)
          {
            MS1x1_NOBANK(BANKTILESIZE);
          }

          i++;
        } while (--block_k > 0);
        tidx.barrier.wait();
        int crow = gidxOffset + idx;
        int ccolprod = (gidyOffset + idy) * ldc;
        if(crow < M && ccolprod/ldc < N)
          C[cOffset + C_batchOffset * elt + crow + ccolprod] =  alpha * rC[0][0] + beta * C[cOffset + C_batchOffset * elt + crow + ccolprod];
      });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if ((M < 10 && N < 1800 && K < 1800) || (M < 600 && N < 10 && K < 1800) || (M < 1800 && N < 10 && K < 1800))
  {
#define TILESIZE 16
#define STEPSIZE 16
        cout<<"\nSTEP 16 16"<<endl;
        Concurrency::extent<3> grdExt(batchSize,(N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
        Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

        Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
        { 
        int elt = tidx.tile[0];
        int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
        float rC[1][1] = {{(float)0}};
        float rA[1][STEPSIZE / TILESIZE];
        float rB[1][STEPSIZE / TILESIZE];
        tile_static float lA[TILESIZE * STEPSIZE];
        tile_static float lB[TILESIZE * STEPSIZE];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
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
              lB[(idyT + (sec * TILESIZE)) * TILESIZE + idxT] = B[bOffset + B_batchOffset * elt + gidy * TILESIZE + idxT + (idyT + (sec * TILESIZE)) * ldb + i * (ldb << shiftFactor)];
            }
            else
            {
              lB[(idyT + (sec * TILESIZE )) * TILESIZE + idxT] = 0;
            }

            if(gidx * TILESIZE + idxT < M && i * STEPSIZE + idyT + (sec * TILESIZE) < K)
            {
              lA[(idyT + (sec * TILESIZE)) * TILESIZE + idxT] = A[aOffset + A_batchOffset * elt + gidx * TILESIZE + idxT + (idyT + (sec * TILESIZE)) * lda + i * (lda << shiftFactor)];
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
          C[cOffset + C_batchOffset * elt + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc] =  alpha * rC[0][0] + beta * C[cOffset + C_batchOffset * elt + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc];
        });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if ((M < 600 && N < 1800 && K < 1800) || (M < 1800 && N < 1800 && K < 600))
  {
#define TILESIZE 16
#define MICROTILESIZE 2
    cout<<"\n MICRO NBK 16 2"<<endl;
    Concurrency::extent<3> grdExt(batchSize,((N >> 1) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M >> 1) + (TILESIZE - 1)) & ~(TILESIZE - 1));
    Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
    {
        int elt = tidx.tile[0];
        int shiftTS = Concurrency::fast_math::log2(TILESIZE);
        float rC[MICROTILESIZE][MICROTILESIZE] = {{(float)0}};
        float rA[1][MICROTILESIZE];
        float rB[1][MICROTILESIZE];
        tile_static float lA[TOTMICROTILEPROD + TILESIZE];
        tile_static float lB[TOTMICROTILEPROD + TILESIZE];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
        int idt = ( idy << shiftTS) + idx;
        int idxT = idt & ( TILESIZE - 1);
        int idyT = idt >> shiftTS;
        int block_k = 0;
        do
        {
          int colIndex = ( block_k << shiftTS ) + idyT;
          int lIndex = (idyT * BANKMICROTILESIZE) + idxT;

          tidx.barrier.wait();
          for(int sec = 0; sec < MICROTILESIZE; ++sec)
          {
            int secVal = sec << shiftTS;
            int BrowIndex = (gidy * MICROTILEPROD) + idxT + secVal;
            int ArowIndex = (gidx * MICROTILEPROD) + idxT + secVal;

            if( BrowIndex < N && colIndex < K)
            {
              lB[ lIndex + secVal] = B[bOffset + B_batchOffset * elt + BrowIndex + colIndex * ldb];
            }
            else
            {
              lB[lIndex + secVal] = 0;
            }

            if(ArowIndex < M && colIndex < K)
            {
              lA[lIndex + secVal] = A[aOffset + A_batchOffset * elt + ArowIndex + colIndex * lda];
            }
            else
            {
              lA[lIndex + secVal] = 0;
            }
          }
          tidx.barrier.wait();

          int offA = idx;
          int offB = idy;
          for (int iter=0; iter < TILESIZE; ++iter)
          {
            MTS_NOBANK;
          }
          tidx.barrier.wait();
        } while (++block_k < (((K + TILESIZE - 1) & ~(TILESIZE - 1)) >> shiftTS));

        int xIndex = (gidx * MICROTILEPROD) + idx;
        int yIndex = ((gidy * MICROTILEPROD) + idy) * ldc;
        for( int row = 0; row < MICROTILESIZE; row++)
        {
          for( int col = 0; col < MICROTILESIZE ; col++)
          {
          if(xIndex + (col << shiftTS) < M && (yIndex / ldc) + (row << shiftTS) < N)
            C[cOffset + C_batchOffset * elt + (xIndex + (col << shiftTS)) + yIndex + (row << shiftTS) * ldc] = alpha * rC[col][row] + beta * C[cOffset + C_batchOffset * elt + (xIndex + (col << shiftTS)) + yIndex + (row << shiftTS) * ldc];
          }
        }
    });
#undef TILESIZE
#undef MICROTILESIZE
    return AMPBLAS_SUCCESS;
  }
  else if (M < 1800 && N < 1800 && K < 1800)
  {
#define TILESIZE 16
#define MICROTILESIZE 2
      cout<<"\n MICRO 16 2"<<endl;
      Concurrency::extent<3> grdExt(batchSize,((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int elt = tidx.tile[0]; 
        float rC[MICROTILESIZE][MICROTILESIZE];
        float rA[1][MICROTILESIZE];
        float rB[1][MICROTILESIZE];
        tile_static float lA[TILESIZE * TILESIZE * MICROTILESIZE];
        tile_static float lB[TILESIZE * TILESIZE * MICROTILESIZE];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
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
              lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = B[bOffset + B_batchOffset * elt + (gidy * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE) + idyT * ldb + block_k * (ldb * TILESIZE)];
            }
            else
            {
              lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
	        }

            if(gidx * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < M && block_k * TILESIZE + idyT < K)
            {
              lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = A[aOffset + A_batchOffset * elt + (gidx * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE) +  idyT * lda + block_k * (lda * TILESIZE)];
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
            C[cOffset + C_batchOffset * elt + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc] = alpha * rC[col][row] + beta * C[cOffset + C_batchOffset * elt + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc];
          }
        }
     });
#undef TILESIZE 
#undef MICROTILESIZE
    return AMPBLAS_SUCCESS;
  }
  else {
      cout<<"Input matrix Size  "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<"doesnot comes under wrapper sizes"<<endl;
      return AMPBLAS_ERROR;
  }
}

static ampblasStatus gemm_NoTransB(Concurrency::accelerator_view &accl_view,
          		                   Concurrency::array_view<float, 1> &A, long aOffset, long A_batchOffset,
                                   Concurrency::array_view<float, 1> &B, long bOffset, long B_batchOffset,
                                   Concurrency::array_view<float, 1> &C, long cOffset, long C_batchOffset,
                                   int M, int N, int K, int lda, int ldb, int ldc,
                                   float alpha, float beta, int batchSize)
{
  if ((M > 10 && M < 600) && N < 10 && K < 10)
  {
#define TILESIZE 8
#define STEPSIZE 8
      cout<<"\n STEP NBK 8 8"<<endl;
      Concurrency::extent<3> grdExt(batchSize, (N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int elt = tidx.tile[0];
        int tilemulshift = (int)Concurrency::fast_math::log2(TILESIZE);
        int shiftfactor = (int)Concurrency::fast_math::log2(STEPSIZE);
        int block_k =((K + (STEPSIZE - 1)) & ~(STEPSIZE - 1)) >> shiftfactor;
        float rC[1][1] = {{0.0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];
        tile_static float lB[STEPTILEPROD + STEPSIZE];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
        int idt = (idy << tilemulshift) + idx; //(idy * TILESIZE + idx)
        int ids = (idy << shiftfactor) + idx; //(idy * STEPSIZE + idx)
        int idxS = ids & (STEPSIZE - 1);
      
       
        int idyT = (idt)>> tilemulshift;
        int gidyOffset = gidy << tilemulshift;
        int gidxOffset = gidx << tilemulshift;
        int idyTOffset = idyT * BANKTILESIZE;


        int i = 0;
        do
        {
          tidx.barrier.wait();
          int iOffset = i << shiftfactor;
          for(int sec = 0; sec < STEPTILERATIO; ++sec)
          {
            int secOffset  = sec << tilemulshift;
            int secStartPt = (sec << tilemulshift) * BANKTILESIZE;
            int localIdx = secStartPt + idxS + idyTOffset;
            int kIndex = iOffset + idxS + secOffset;

            // Initialize the local memory with zero
            lB[localIdx] = 0;
            lA[localIdx] = 0;

            if(gidyOffset + idyT < N && kIndex < K)
            {
              lB[localIdx] = B[bOffset + B_batchOffset * elt + (gidyOffset + idyT) * ldb + kIndex];
            }
            if(gidxOffset + idyT < M && kIndex < K)
            {
              lA[localIdx] = A[aOffset + A_batchOffset * elt + (gidxOffset + idyT) * lda + kIndex];
            }
          }

          tidx.barrier.wait();

          int offA = idx * BANKTILESIZE;
          int offB = idy * BANKTILESIZE;
          
          for (int piter=0; piter < TILESIZE; ++piter)
          {
            MS1x1_NOBANK(1);
          }

          i++;

        }while (--block_k > 0);

        tidx.barrier.wait();


        int crow = gidxOffset + idx;
        int ccolprod = (gidyOffset + idy) * ldc;
        if(crow < M && ccolprod/ldc < N)
            C[cOffset + C_batchOffset * elt + crow + ccolprod] = alpha * rC[0][0] + beta * C[cOffset + C_batchOffset * elt + crow + ccolprod];
      });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if ((M > 10 && M < 600) && N < 600 && K < 10)
  {
#define TILESIZE 8
#define STEPSIZE 8
    cout<<"\n STEP 8 8"<<endl;
    Concurrency::extent<3> grdExt(batchSize, (N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
    Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
    {
    int elt = tidx.tile[0];
    int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
    float rC[1][1] = {{(float)0}};
    float rA[1][STEPSIZE / TILESIZE];
    float rB[1][STEPSIZE / TILESIZE];
    tile_static float lA[TILESIZE * STEPSIZE];
    tile_static float lB[TILESIZE * STEPSIZE];
    int gidx = tidx.tile[2];
    int gidy = tidx.tile[1];
    int idx = tidx.local[2];
    int idy = tidx.local[1];
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
          lB[(sec * TILESIZE * TILESIZE) + idyT + idxT * TILESIZE] = B[bOffset + B_batchOffset * elt + (gidy * TILESIZE + idxT) * ldb + idyT + i * STEPSIZE + (sec * TILESIZE)];
        }
        else
        {
          lB[(sec * TILESIZE * TILESIZE ) + idyT + idxT * TILESIZE] = 0;
        }

        if(gidx * TILESIZE + idxT < M && i * STEPSIZE + idyT + (sec * TILESIZE ) < K)
        {
          lA[(sec * TILESIZE * TILESIZE) + idyT + idxT * TILESIZE] = A[aOffset + A_batchOffset * elt + (gidx * TILESIZE + idxT) * lda + idyT + i * STEPSIZE + (sec * TILESIZE)];
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
        C[cOffset + C_batchOffset * elt + (gidx * TILESIZE + idx) + (gidy * TILESIZE + idy) * ldc] = alpha * rC[0][0] + beta * C[cOffset + C_batchOffset * elt + (gidx * TILESIZE + idx) + (gidy * TILESIZE + idy) * ldc];
    });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if (((M < 10 && N < 1800) || (M < 1800 && N < 10) || (M < 600 && N < 1800) || (M < 1800 && N < 10)) && K < 1800)
  {
#define TILESIZE 16
#define STEPSIZE 16
    cout<<"\n STEP NBK 16 16"<<endl;
    Concurrency::extent<3> grdExt(batchSize,(N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
    Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
    {
    int elt = tidx.tile[0];
    int tilemulshift = (int)Concurrency::fast_math::log2(TILESIZE);
    int shiftfactor = (int)Concurrency::fast_math::log2(STEPSIZE);
    int block_k =((K + (STEPSIZE - 1)) & ~(STEPSIZE - 1)) >> shiftfactor;
    float rC[1][1] = {{0.0}};
    float rA[1][STEPTILERATIO];
    float rB[1][STEPTILERATIO];
    tile_static float lA[STEPTILEPROD + STEPSIZE];
    tile_static float lB[STEPTILEPROD + STEPSIZE];
    int gidx = tidx.tile[2];
    int gidy = tidx.tile[1];
    int idx = tidx.local[2];
    int idy = tidx.local[1];
    int idt = (idy << tilemulshift) + idx; //(idy * TILESIZE + idx)
    int ids = (idy << shiftfactor) + idx; //(idy * STEPSIZE + idx)
    int idxS = ids & (STEPSIZE - 1);


    int idyT = (idt)>> tilemulshift;
    int gidyOffset = gidy << tilemulshift;
    int gidxOffset = gidx << tilemulshift;
    int idyTOffset = idyT * BANKTILESIZE;


    int i = 0;
    do
    {
      tidx.barrier.wait();
      int iOffset = i << shiftfactor;
      for(int sec = 0; sec < STEPTILERATIO; ++sec)
      {
        int secOffset  = sec << tilemulshift;
        int secStartPt = (sec << tilemulshift) * BANKTILESIZE;
        int localIdx = secStartPt + idxS + idyTOffset;
        int kIndex = iOffset + idxS + secOffset;

        // Initialize the local memory with zero
        lB[localIdx] = 0;
        lA[localIdx] = 0;

        if(gidyOffset + idyT < N && kIndex < K)
        {
          lB[localIdx] = B[bOffset + B_batchOffset * elt + (gidyOffset + idyT) * ldb + kIndex];
        }
        if(gidxOffset + idyT < M && kIndex < K)
        {
          lA[localIdx] = A[aOffset + A_batchOffset * elt + (gidxOffset + idyT) * lda + kIndex];
        }
      }

      tidx.barrier.wait();

      int offA = idx * BANKTILESIZE;
      int offB = idy * BANKTILESIZE;
      
      for (int piter=0; piter < TILESIZE; ++piter)
      {
        MS1x1_NOBANK(1);
      }

      i++;

    }while (--block_k > 0);

    tidx.barrier.wait();

    int crow = gidxOffset + idx;
    int ccolprod = (gidyOffset + idy) * ldc;
    if(crow < M && ccolprod/ldc < N)
        C[cOffset + C_batchOffset * elt + crow + ccolprod] = alpha * rC[0][0] + beta * C[cOffset + C_batchOffset * elt + crow + ccolprod];
    });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  else if (M < 1800 && N < 1800 && K < 600)  
  {
#define TILESIZE 16
#define MICROTILESIZE 2
    cout<<"\n MICRO NBK 16 2"<<endl;
    Concurrency::extent<3> grdExt(batchSize,((N >> 1) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M >> 1) + (TILESIZE - 1)) & ~(TILESIZE - 1));
    Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
    {
    int elt = tidx.tile[0];
    int shiftTS = Concurrency::fast_math::log2(TILESIZE);
    float rC[MICROTILESIZE][MICROTILESIZE] = {{(float)0}};
    float rA[1][MICROTILESIZE];
    float rB[1][MICROTILESIZE];
    tile_static float lA[TOTMICROTILEPROD + TILESIZE];
    tile_static float lB[TOTMICROTILEPROD + TILESIZE];
    int gidx = tidx.tile[2];
    int gidy = tidx.tile[1];
    int idx = tidx.local[2];
    int idy = tidx.local[1];
    int idt = ( idy << shiftTS ) + idx;
    int idxT = idt % TILESIZE ;
    int idyT = idt / TILESIZE;
    int block_k = 0;
    do
    {
      int colIndex =( block_k << shiftTS )+ idyT;
      int lIndex = (idyT * BANKMICROTILESIZE) + idxT;

      tidx.barrier.wait();
      for(int sec = 0; sec < MICROTILESIZE; ++sec)
      {
        int secVal = sec << shiftTS;
        int BrowIndex = ( gidy * MICROTILEPROD) + idxT + secVal;
        int ArowIndex = ( gidx * MICROTILEPROD) + idxT + secVal;

        tidx.barrier.wait();
        if( BrowIndex < N && colIndex < K)
        {
          lB[ lIndex + secVal] = B[ bOffset + B_batchOffset * elt + BrowIndex * ldb + colIndex ];
        }
        else
        {
          lB[ lIndex + secVal] = 0;
        }

        if( ArowIndex < M && colIndex < K)
        {
          lA[ lIndex + secVal] = A[aOffset + A_batchOffset * elt + ArowIndex * lda +  colIndex];
        }
        else
        {
          lA[ lIndex + secVal] = 0;
        }
      }
      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;
      for (int iter=0; iter < TILESIZE; ++iter)
      {
        MTS_NOBANK;
      }
      tidx.barrier.wait();
    } while (++block_k < (((K + TILESIZE - 1) & ~(TILESIZE - 1)) >> shiftTS));

    int xIndex = (gidx * MICROTILEPROD) + idx;
    int yIndex = ((gidy * MICROTILEPROD) + idy) * ldc;
    for( int row = 0; row < MICROTILESIZE; row++)
    {
      for( int col = 0; col < MICROTILESIZE ; col++)
      {
      if(xIndex + (col << shiftTS) < M && (yIndex / ldc) + (row << shiftTS) < N)
        C[cOffset + C_batchOffset * elt + (xIndex + (col << shiftTS)) + yIndex + (row << shiftTS ) * ldc] = alpha * rC[col][row] + beta * C[cOffset + C_batchOffset * elt + (xIndex + (col << shiftTS)) + yIndex + (row << shiftTS) * ldc];
      }
    }
    });
#undef TILESIZE
#undef MICROTILESIZE
    return AMPBLAS_SUCCESS;
  }
  else if (M < 1800 && N < 1800 && K < 1800)
  {
#define TILESIZE 16
#define MICROTILESIZE 2
    cout<<"\n MICRO 16 2"<<endl;
    Concurrency::extent<3> grdExt(batchSize,((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
    Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
    {
        int elt = tidx.tile[0];
        float rC[MICROTILESIZE][MICROTILESIZE] = {{(float)0}};
        float rA[1][MICROTILESIZE];
        float rB[1][MICROTILESIZE];
        tile_static float lA[TILESIZE * TILESIZE * MICROTILESIZE];
        tile_static float lB[TILESIZE * TILESIZE * MICROTILESIZE];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
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
              lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = B[bOffset + B_batchOffset * elt + (gidy * TILESIZE * MICROTILESIZE + idxT + sec * TILESIZE) * ldb + idyT + block_k * TILESIZE];
            }
            else
            {
              lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
            }

            if(gidx * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < M && block_k * TILESIZE + idyT < K)
            {
              lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = A[aOffset + A_batchOffset * elt + (gidx * TILESIZE * MICROTILESIZE + idxT + sec * TILESIZE) * lda +  idyT + block_k * TILESIZE];
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
            C[cOffset + C_batchOffset * elt + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc] = alpha * rC[col][row] + beta * C[cOffset + C_batchOffset * elt + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc];
          }
        }
    });
#undef TILESIZE
#undef MICROTILESIZE
    return AMPBLAS_SUCCESS;
  }
  else {
     cout<<"Input matrix Size "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<"is not covered under wrapper sizes"<<endl;
     return AMPBLAS_ERROR;
  }
}
static ampblasStatus gemm_TransAB(Concurrency::accelerator_view &accl_view,
                                  Concurrency::array_view<float, 1> &A, long aOffset, long A_batchOffset,
                                  Concurrency::array_view<float, 1> &B, long bOffset, long B_batchOffset,
                                  Concurrency::array_view<float, 1> &C, long cOffset, long C_batchOffset,
                                  int M, int N, int K, int lda, int ldb, int ldc,
                                  float alpha, float beta, int batchSize)
{
  if ((M < 600 && N < 600 && K < 10) || (M < 1800 && N < 600 && K < 600))
  {
#define TILESIZE 8
#define STEPSIZE 8
      cout<<"\n STEP NBK 8 8"<<endl;
      Concurrency::extent<3> grdExt(batchSize,(N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int elt = tidx.tile[0];
        int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
        float rC[1][1] = {{(float)0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];
        tile_static float lB[STEPTILEPROD + STEPSIZE];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
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
              lB[((idxT + sec * TILESIZE) * BANKTILESIZE) + idyT] = B[bOffset + B_batchOffset * elt + gidy * TILESIZE + idxT + ((idyT + (sec * TILESIZE)) *ldb) + i * (ldb << shiftFactor)];
            }
            else
            {
              lB[((idxT + sec * TILESIZE) * BANKTILESIZE) + idyT] = 0;
            }

            if(gidx * TILESIZE + idxT < M && i * STEPSIZE + idyT + (sec * TILESIZE) < K)
            {
              lA[(sec * BANKNUMTILEELMTS) + idyT + idxT * BANKTILESIZE] = A[aOffset  + A_batchOffset * elt +(gidx * TILESIZE + idxT) * lda + idyT + i * STEPSIZE + (sec * TILESIZE)];
            }
            else
            {
              lA[(sec * BANKNUMTILEELMTS ) + idyT + idxT * BANKTILESIZE] = 0;
            }
          }
          tidx.barrier.wait();

          int offA = idx * BANKTILESIZE;
          int offB = idy * BANKTILESIZE;
          for (int iter = 0; iter < TILESIZE; ++iter)
          {
            MS1x1_NOBANK(1);
          }
          i++;
        } while (--block_k > 0);


        tidx.barrier.wait();
        if(gidx * TILESIZE + idx < M && gidy * TILESIZE + idy < N)
            C[cOffset + C_batchOffset * elt + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc] = alpha * rC[0][0] + beta * C[cOffset + C_batchOffset * elt + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc];
      });
#undef TILESIZE
#undef STEPSIZE
     return AMPBLAS_SUCCESS;
  }
  if ((M < 600 && N < 600 && K < 1800) || (M < 1800 && ((N < 600 && K < 1800) || (N < 1800 && K < 10))))
  {
#define TILESIZE 16
#define STEPSIZE 16
      cout<<"\n STEP NBK 16 16"<<endl;
      Concurrency::extent<3> grdExt(batchSize,(N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int elt = tidx.tile[0];
        int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
        float rC[1][1] = {{(float)0}};
        float rA[1][STEPTILERATIO];
        float rB[1][STEPTILERATIO];
        tile_static float lA[STEPTILEPROD + STEPSIZE];
        tile_static float lB[STEPTILEPROD + STEPSIZE];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
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
              lB[((idxT + sec * TILESIZE) * BANKTILESIZE) + idyT] = B[bOffset + B_batchOffset * elt + gidy * TILESIZE + idxT + ((idyT + (sec * TILESIZE)) *ldb) + i * (ldb << shiftFactor)];
            }
            else
            {
              lB[((idxT + sec * TILESIZE) * BANKTILESIZE) + idyT] = 0;
            }

            if(gidx * TILESIZE + idxT < M && i * STEPSIZE + idyT + (sec * TILESIZE) < K)
            {
              lA[(sec * BANKNUMTILEELMTS) + idyT + idxT * BANKTILESIZE] = A[aOffset  + A_batchOffset * elt + (gidx * TILESIZE + idxT) * lda + idyT + i * STEPSIZE + (sec * TILESIZE)];
            }
            else
            {
              lA[(sec * BANKNUMTILEELMTS ) + idyT + idxT * BANKTILESIZE] = 0;
            }
          }
          tidx.barrier.wait();

          int offA = idx * BANKTILESIZE;
          int offB = idy * BANKTILESIZE;
          for (int iter = 0; iter < TILESIZE; ++iter)
          {
            MS1x1_NOBANK(1);
          }
          i++;
        } while (--block_k > 0);


        tidx.barrier.wait();
        if(gidx * TILESIZE + idx < M && gidy * TILESIZE + idy < N)
            C[cOffset + C_batchOffset * elt + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc] = alpha * rC[0][0] + beta * C[cOffset + C_batchOffset * elt + gidx * TILESIZE + idx + (gidy * TILESIZE + idy) * ldc];
      });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;
  }
  
  else if (M < 1800 && N < 1800 && K < 1800)
  {
#define TILESIZE 16
#define MICROTILESIZE 2
      cout<<"\n MICRO 16 2"<<endl;
      Concurrency::extent<3> grdExt(batchSize,((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<1, TILESIZE, TILESIZE> t_ext(grdExt);

      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        int elt = tidx.tile[0];
        float rC[MICROTILESIZE][MICROTILESIZE] = {{(float)0}};
        float rA[1][MICROTILESIZE];
        float rB[1][MICROTILESIZE];
        tile_static float lA[TILESIZE * TILESIZE * MICROTILESIZE];
        tile_static float lB[TILESIZE * TILESIZE * MICROTILESIZE];
        int gidx = tidx.tile[2];
        int gidy = tidx.tile[1];
        int idx = tidx.local[2];
        int idy = tidx.local[1];
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
              lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = B[bOffset + B_batchOffset * elt + (gidy * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE) + idyT * ldb + block_k * (ldb * TILESIZE)];
            }
            else
            {
              lB[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = 0;
	        }

            if(gidx * TILESIZE * MICROTILESIZE + idxT + (sec * TILESIZE) < M && block_k * TILESIZE + idyT < K)
            {
              lA[(idyT * TILESIZE * MICROTILESIZE) + idxT + (sec * TILESIZE)] = A[aOffset + A_batchOffset * elt + (gidx * TILESIZE * MICROTILESIZE + idxT + sec * TILESIZE) * lda +  idyT + block_k * TILESIZE];
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
            C[cOffset + C_batchOffset * elt + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc] = alpha * rC[col][row] + beta * C[cOffset + C_batchOffset * elt + (xIndex + TILESIZE * col) + yIndex + (TILESIZE * row) * ldc];
          }
        }
     });
#undef TILESIZE
#undef MICROTILESIZE 
    return AMPBLAS_SUCCESS;
  }
  else  {
     cout<<"Input matrix Size "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<" not covered under wrapper sizes"<<endl; 
     return AMPBLAS_ERROR;
  }
}

ampblasStatus gemm_AMP(Concurrency::accelerator_view &accl_view,
	     char TransA, char TransB, const int M, const int N, const int K,
             const float alpha, Concurrency::array_view<float> &A_mat, long aOffset, long lda,
             Concurrency::array_view<float> &B_mat, long bOffset, long ldb, const float beta,
             Concurrency::array_view<float> &C_mat, long cOffset, long ldc,
             Concurrency::array_view<float> &temp_buf)
{
  int i, j;
  ampblasStatus status = AMPBLAS_SUCCESS;
  // Quick return if possible
  if (!M || !N || ((alpha == 0 || !K) && beta == 1))
    return status;
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
    return status;
  }
  // Start the operations
    if (TransB == 'n')
    {
      if (TransA == 'n')
        status = gemm_NoTransAB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
      else
        status = gemm_NoTransB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
    }
    else if (TransA == 'n')
      status = gemm_NoTransA(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
    else
      status = gemm_TransAB(accl_view, A_mat, aOffset, B_mat, bOffset, C_mat, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);

  return status;
}

ampblasStatus gemm_AMP(Concurrency::accelerator_view &accl_view,
	                   char TransA, char TransB, const int M, const int N, const int K,
                       const float alpha, Concurrency::array_view<float> &A_mat, 
                       long aOffset, long A_batchOffset, long lda,
                       Concurrency::array_view<float> &B_mat, 
                       long bOffset, long B_batchOffset, long ldb, const float beta,
                       Concurrency::array_view<float> &C_mat, 
                       long cOffset, long C_batchOffset, long ldc,
                       Concurrency::array_view<float> &temp_buf, int batchSize)
{
  int i, j;
  ampblasStatus status = AMPBLAS_SUCCESS;
  // Quick return if possible
  if (!M || !N || ((alpha == 0 || !K) && beta == 1))
    return status;
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
    return status;
  }
  // Start the operations
    if (TransB == 'n')
    {
      if (TransA == 'n')
        status = gemm_NoTransAB(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
      else
        status = gemm_NoTransB(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
    }
    else if (TransA == 'n')
      status = gemm_NoTransA(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);
    else
      status = gemm_TransAB(accl_view, A_mat, aOffset, A_batchOffset, B_mat, bOffset, B_batchOffset, C_mat, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, alpha, beta, batchSize);

  return status;
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
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

 
    ampblasStatus status = gemm_AMP(accl_view, typeA, typeB, M, N, K, *alpha,
                                    A_mat, aOffset, lda, B_mat, bOffset, ldb,
                                    *beta, C_mat, cOffset, ldc, *temp_buf);

    C_mat.synchronize();

    /* Print Output */
    /*for (int i = 0; i < M * N; i++)
        cout<<" C_mat["<<i<<"] "<<C_mat[i]<<endl;*/

    return status;
}

ampblasStatus  Ampblaslibrary :: ampblas_sgemm(Concurrency::accelerator_view &accl_view,
                                 	       const enum AMPBLAS_TRANS typeA,
                                               const enum AMPBLAS_TRANS typeB, const int M,
                                               const int N, const int K, const float &alpha,
                                               Concurrency::array_view<float> &A, const long lda,
                                               Concurrency::array_view<float> &B, const long ldb,
                                               const float &beta,
                                               Concurrency::array_view<float> &C, const long ldc,
                                               const long aOffset, const long bOffset, const long cOffset)

{
    Concurrency::array_view<float> *temp_buf = NULL;
    ampblasStatus status = gemm_AMP(accl_view, typeA, typeB, M, N, K, alpha, A,
                                    aOffset, lda, B, bOffset, ldb, beta, C,
                                    cOffset, ldc, *temp_buf);

    C.synchronize();

    /* Print Output */
    /*for (int i = 0; i < M * N; i++)
        cout<<" C_mat["<<i<<"] "<<C_mat[i]<<endl;*/

    return status;
}

/* SGEMM- Overloaded function with arguments related to batch processing */
ampblasStatus Ampblaslibrary :: ampblas_sgemm(Concurrency::accelerator_view &accl_view,
                                              const enum AMPBLAS_TRANS typeA,
                                              const enum AMPBLAS_TRANS typeB, const int M,
                                              const int N, const int K, const float &alpha,
                                              Concurrency::array_view<float> &A, const long lda, const long A_batchOffset,
                                              Concurrency::array_view<float> &B, const long ldb, const long B_batchOffset,
                                              const float &beta,
                                              Concurrency::array_view<float> &C, const long ldc, const long C_batchOffset,
                                              const long aOffset, const long bOffset, const long cOffset, const int batchSize)

{
    Concurrency::array_view<float> *temp_buf = NULL;
    gemm_AMP(accl_view, typeA, typeB, M, N, K, alpha, A, aOffset, A_batchOffset, lda, B,
             bOffset, B_batchOffset, ldb, beta, C, cOffset, C_batchOffset, ldc, *temp_buf, batchSize);

    C.synchronize();

    /* Print Output */
    /*for (int i = 0; i < M * N; i++)
        cout<<" C_mat["<<i<<"] "<<C_mat[i]<<endl;*/

    return AMPBLAS_SUCCESS;
}

