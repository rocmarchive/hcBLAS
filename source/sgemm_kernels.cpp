#include "sgemm_kernels.h"

ampblasStatus gemm_NoTransAB_STEP_NBK_TS8XSS8(Concurrency::accelerator_view &accl_view,
                                    Concurrency::array_view<float, 1> &A, long aOffset,
                                    Concurrency::array_view<float, 1> &B, long bOffset,
                                    Concurrency::array_view<float, 1> &C, long cOffset,
                                    int M, int N, int K, int lda, int ldb, int ldc,
                                    float alpha, float beta)
{
#define TILESIZE 8
#define STEPSIZE 8
      cout<<"\nSTEP NBK" <<TILESIZE<<" "<<STEPSIZE<<endl;
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

ampblasStatus gemm_NoTransAB_STEP_NBK_TS16XSS16(Concurrency::accelerator_view &accl_view,
                                    Concurrency::array_view<float, 1> &A, long aOffset,
                                    Concurrency::array_view<float, 1> &B, long bOffset,
                                    Concurrency::array_view<float, 1> &C, long cOffset,
                                    int M, int N, int K, int lda, int ldb, int ldc,
                                    float alpha, float beta)
{
#define TILESIZE 16 
#define STEPSIZE 16
      cout<<"\nSTEP NBK" <<TILESIZE<<" "<<STEPSIZE<<endl;
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

ampblasStatus gemm_NoTransAB_MICRO_TS16XMTS2(Concurrency::accelerator_view &accl_view,
                                    Concurrency::array_view<float, 1> &A, long aOffset,
                                    Concurrency::array_view<float, 1> &B, long bOffset,
                                    Concurrency::array_view<float, 1> &C, long cOffset,
                                    int M, int N, int K, int lda, int ldb, int ldc,
                                    float alpha, float beta)
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



ampblasStatus gemm_NoTransA_STEP_NBK_TS16XSS16(Concurrency::accelerator_view &accl_view,
	                           Concurrency::array_view<float, 1> &A, long aOffset,
                                   Concurrency::array_view<float, 1> &B, long bOffset,
                                   Concurrency::array_view<float, 1> &C, long cOffset,
                                   int M, int N, int K, int lda, int ldb, int ldc,
                                   float alpha, float beta)
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

ampblasStatus gemm_NoTransA_STEP_TS16XSS16(Concurrency::accelerator_view &accl_view,
	                           Concurrency::array_view<float, 1> &A, long aOffset,
                                   Concurrency::array_view<float, 1> &B, long bOffset,
                                   Concurrency::array_view<float, 1> &C, long cOffset,
                                   int M, int N, int K, int lda, int ldb, int ldc,
                                   float alpha, float beta)
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

ampblasStatus gemm_NoTransA_MICRO_NBK_TS16XMTS2(Concurrency::accelerator_view &accl_view,
	                           Concurrency::array_view<float, 1> &A, long aOffset,
                                   Concurrency::array_view<float, 1> &B, long bOffset,
                                   Concurrency::array_view<float, 1> &C, long cOffset,
                                   int M, int N, int K, int lda, int ldb, int ldc,
                                   float alpha, float beta)
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


ampblasStatus gemm_NoTransA_MICRO_TS16XMTS2(Concurrency::accelerator_view &accl_view,
	                           Concurrency::array_view<float, 1> &A, long aOffset,
                                   Concurrency::array_view<float, 1> &B, long bOffset,
                                   Concurrency::array_view<float, 1> &C, long cOffset,
                                   int M, int N, int K, int lda, int ldb, int ldc,
                                   float alpha, float beta)
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


ampblasStatus gemm_NoTransB_STEP_NBK_TS8XSS8(Concurrency::accelerator_view &accl_view,
          		  Concurrency::array_view<float, 1> &A, long aOffset,
                          Concurrency::array_view<float, 1> &B, long bOffset,
                          Concurrency::array_view<float, 1> &C, long cOffset,
                          int M, int N, int K, int lda, int ldb, int ldc,
                          float alpha, float beta)
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

ampblasStatus gemm_NoTransB_STEP_TS8XSS8(Concurrency::accelerator_view &accl_view,
          		  Concurrency::array_view<float, 1> &A, long aOffset,
                          Concurrency::array_view<float, 1> &B, long bOffset,
                          Concurrency::array_view<float, 1> &C, long cOffset,
                          int M, int N, int K, int lda, int ldb, int ldc,
                          float alpha, float beta)
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

ampblasStatus gemm_NoTransB_STEP_NBK_TS16XSS16(Concurrency::accelerator_view &accl_view,
          		  Concurrency::array_view<float, 1> &A, long aOffset,
                          Concurrency::array_view<float, 1> &B, long bOffset,
                          Concurrency::array_view<float, 1> &C, long cOffset,
                          int M, int N, int K, int lda, int ldb, int ldc,
                          float alpha, float beta)
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


ampblasStatus gemm_NoTransB_MICRO_NBK_TS16XMTS2(Concurrency::accelerator_view &accl_view,
          		  Concurrency::array_view<float, 1> &A, long aOffset,
                          Concurrency::array_view<float, 1> &B, long bOffset,
                          Concurrency::array_view<float, 1> &C, long cOffset,
                          int M, int N, int K, int lda, int ldb, int ldc,
                          float alpha, float beta)
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


ampblasStatus gemm_NoTransB_MICRO_TS16XMTS2(Concurrency::accelerator_view &accl_view,
          		  Concurrency::array_view<float, 1> &A, long aOffset,
                          Concurrency::array_view<float, 1> &B, long bOffset,
                          Concurrency::array_view<float, 1> &C, long cOffset,
                          int M, int N, int K, int lda, int ldb, int ldc,
                          float alpha, float beta)
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




ampblasStatus gemm_TransAB_STEP_NBK_TS8XSS8(Concurrency::accelerator_view &accl_view,
                                  Concurrency::array_view<float, 1> &A, long aOffset,
                                  Concurrency::array_view<float, 1> &B, long bOffset,
                                  Concurrency::array_view<float, 1> &C, long cOffset,
                                  int M, int N, int K, int lda, int ldb, int ldc,
                                  float alpha, float beta)
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

ampblasStatus gemm_TransAB_STEP_NBK_TS16XSS16(Concurrency::accelerator_view &accl_view,
                                  Concurrency::array_view<float, 1> &A, long aOffset,
                                  Concurrency::array_view<float, 1> &B, long bOffset,
                                  Concurrency::array_view<float, 1> &C, long cOffset,
                                  int M, int N, int K, int lda, int ldb, int ldc,
                                  float alpha, float beta)
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

ampblasStatus gemm_TransAB_MICRO_TS16XMTS2(Concurrency::accelerator_view &accl_view,
                                  Concurrency::array_view<float, 1> &A, long aOffset,
                                  Concurrency::array_view<float, 1> &B, long bOffset,
                                  Concurrency::array_view<float, 1> &C, long cOffset,
                                  int M, int N, int K, int lda, int ldb, int ldc,
                                  float alpha, float beta)
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


/*------------------------------------------- New Kernels from Greg Stoner-------------------------------------------*/

// Kernel 1

/*
 * Inputs and Outputs are processed in Column major form 
 * Matrix-Matrix-Multiplication using local memory as a buffer
 * that has [TILESIZE x TILESIZE] elements
 * 
 * Dimensions:
 *   Matrix A is [MxK] and A is transposed
 *   Matrix B is [KxN] and B is transposed
 *   Matrix C is [MxN]
 * 
 * Global Index Space
 *   global_size[0] := global_size[0] % TILESIZE == 0 && global_size[0] >= N
 *   global_size[1] := global_size[1] % TILESIZE == 0 && global_size[1] >= M
 *   
 * Local Index Space
 *   local_size[0] := TILESIZE
 *   local_size[1] := TILESIZE
 *  
 * Number of Threads in each local workgroup
 *   localThreadCount := TILESIZE*TILESIZE
 */

ampblasStatus gemm_TransAB_K1(Concurrency::accelerator_view &accl_view,
                                    Concurrency::array_view<float, 1> &A, long aOffset,
                                    Concurrency::array_view<float, 1> &B, long bOffset,
                                    Concurrency::array_view<float, 1> &C, long cOffset,
                                    int M, int N, int K, int lda, int ldb, int ldc,
                                    float alpha, float beta)
{
#define TILESIZE 8
      Concurrency::extent<2> grdExt((N + (TILESIZE - 1)) & ~(TILESIZE - 1), (M + (TILESIZE - 1)) & ~(TILESIZE - 1));
      Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);
      Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
      {
        // coordinates for each tile of [TILESIZE x TILESIZE]
        int tile_x = tidx.tile[0];
        int tile_y = tidx.tile[1];
 
        // local index of each thread
        int thread_x = tidx.local[0];
        int thread_y = tidx.local[1];

        // first index of first thread reading A in local workgroup
        int a_bgn = K * TILESIZE * tile_y;

        // last index to first thread reading A in local workgroup
        int a_end   = a_bgn + K - 1;

        // step taken by each thread reading A
        int a_stp  = TILESIZE;

        // first index of first thread reading B in local workgroup
        int b_bgn = TILESIZE * tile_x;
  
        // last index of first thread reading B in local workgroup -- unused in code
        //int b_end = b_bgn + N*(K-1);

        // step taken by each thread reading B in local workgroup
        int b_stp  = TILESIZE * N;
      
        // accumulates the result
        float sum = 0.0;

        int global_x = 0;
        int global_y = 0;

        // local memory for matrix A
        tile_static float localMemA[TILESIZE][TILESIZE];

        // local memory for matrix B
        tile_static float localMemB[TILESIZE][TILESIZE];
  
        
        for (int a = a_bgn, b = b_bgn; a <= a_end; a += a_stp, b += b_stp, global_x += TILESIZE, global_y += TILESIZE)  
        {
          // each thread in workgroup reads one element of matrix A from global to local memory
          if ( thread_x + global_x < K ) 
          {
            localMemA[thread_y][thread_x] = alpha*A[a + aOffset +  K * thread_y + thread_x];
          } 
          else 
          { // needed on AMD
            localMemA[thread_y][thread_x] = 0.0;
          }
          // each thread in workgroup reads one element of matrix B from global to local memory
          if ( thread_y + global_y < K ) 
          {
            localMemB[thread_y][thread_x] = B[b + bOffset + N * thread_y + thread_x];
          } 
          else 
          { // needed on AMD
            localMemB[thread_y][thread_x] = 0.0;
          }
   
          // Synchronize the reads of A and B
          tidx.barrier.wait();
          // multiply matrix A and B using local memory
          for (int k = 0; k < TILESIZE; k++) 
          {
            sum += localMemA[thread_y][k] * localMemB[k][thread_x];
          }

          // Synchronize all sub-results
          tidx.barrier.wait();
       }

       // write all results back to global memory
       if ( tidx.global[0] < N && tidx.global[1] < M ) 
       {
         int c = N * TILESIZE * tile_y + TILESIZE * tile_x;
         if (c + N * thread_y + thread_x < M*N ) 
         {
           C[c + cOffset +  N * thread_y + thread_x] = sum + beta * C[c + cOffset + N * thread_y + thread_x];
         }
       }
   });
#undef TILESIZE 
  
   return AMPBLAS_SUCCESS;
}


ampblasStatus gemm_NoTransAB(Concurrency::accelerator_view &accl_view,
                                    Concurrency::array_view<float, 1> &A, long aOffset,
                                    Concurrency::array_view<float, 1> &B, long bOffset,
                                    Concurrency::array_view<float, 1> &C, long cOffset,
                                    int M, int N, int K, int lda, int ldb, int ldc,
                                    float alpha, float beta)
{
  if ((M < 600 && N < 600 && K < 10) || (M < 1800 && N < 600 && K < 600)) 
  {
    return gemm_NoTransAB_STEP_NBK_TS8XSS8(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else if ((M < 600 && N < 600 && K < 1800) || (M < 1800 && ((N < 600 && K < 1800) || (N < 1800 && K < 10))))
  {
    return gemm_NoTransAB_STEP_NBK_TS16XSS16(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else if (M < 1800 && N < 1800 && K < 1800)
  {
    return gemm_NoTransAB_MICRO_TS16XMTS2(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else {
    cout<<"Input matrix Size  "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<"doesnot comes under wrapper sizes"<<endl;
    return AMPBLAS_ERROR;
  }
}


ampblasStatus gemm_NoTransA(Concurrency::accelerator_view &accl_view,
	                           Concurrency::array_view<float, 1> &A, long aOffset,
                                   Concurrency::array_view<float, 1> &B, long bOffset,
                                   Concurrency::array_view<float, 1> &C, long cOffset,
                                   int M, int N, int K, int lda, int ldb, int ldc,
                                   float alpha, float beta)
{
  if ((M < 10 && N < 1800 && K < 600) || (M < 600 && ((N < 600 && K < 1800) || (N < 1800 && K < 10))))
  {
    return gemm_NoTransA_STEP_NBK_TS16XSS16(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else if ((M < 10 && N < 1800 && K < 1800) || (M < 600 && N < 10 && K < 1800) || (M < 1800 && N < 10 && K < 1800))
  {
    return gemm_NoTransA_STEP_TS16XSS16(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else if ((M < 600 && N < 1800 && K < 1800) || (M < 1800 && N < 1800 && K < 600))
  {
    return gemm_NoTransA_MICRO_NBK_TS16XMTS2(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else if (M < 1800 && N < 1800 && K < 1800)
  {
    return gemm_NoTransA_MICRO_TS16XMTS2(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else {
      cout<<"Input matrix Size  "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<"doesnot comes under wrapper sizes"<<endl;
      return AMPBLAS_ERROR;
  }
}


ampblasStatus gemm_NoTransB(Concurrency::accelerator_view &accl_view,
          		  Concurrency::array_view<float, 1> &A, long aOffset,
                          Concurrency::array_view<float, 1> &B, long bOffset,
                          Concurrency::array_view<float, 1> &C, long cOffset,
                          int M, int N, int K, int lda, int ldb, int ldc,
                          float alpha, float beta)
{
  if ((M > 10 && M < 600) && N < 10 && K < 10)
  {
    return gemm_NoTransB_STEP_NBK_TS8XSS8(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else if ((M > 10 && M < 600) && N < 600 && K < 10)
  {
    return gemm_NoTransB_STEP_TS8XSS8(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else if (((M < 10 && N < 1800) || (M < 1800 && N < 10) || (M < 600 && N < 1800) || (M < 1800 && N < 10)) && K < 1800)
  {
    return gemm_NoTransB_STEP_NBK_TS16XSS16(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else if (M < 1800 && N < 1800 && K < 600)  
  {
    return  gemm_NoTransB_MICRO_NBK_TS16XMTS2(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else if (M < 1800 && N < 1800 && K < 1800)
  {
    return  gemm_NoTransB_MICRO_TS16XMTS2(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else {
     cout<<"Input matrix Size "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<"is not covered under wrapper sizes"<<endl;
     return AMPBLAS_ERROR;
  }
}

ampblasStatus gemm_TransAB(Concurrency::accelerator_view &accl_view,
                                  Concurrency::array_view<float, 1> &A, long aOffset,
                                  Concurrency::array_view<float, 1> &B, long bOffset,
                                  Concurrency::array_view<float, 1> &C, long cOffset,
                                  int M, int N, int K, int lda, int ldb, int ldc,
                                  float alpha, float beta)
{
  if ((M < 600 && N < 600 && K < 10) || (M < 1800 && N < 600 && K < 600))
  {
    return gemm_TransAB_STEP_NBK_TS8XSS8(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  if ((M < 600 && N < 600 && K < 1800) || (M < 1800 && ((N < 600 && K < 1800) || (N < 1800 && K < 10))))
  {
    return gemm_TransAB_STEP_NBK_TS16XSS16(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  
  else if (M < 1800 && N < 1800 && K < 1800)
  {
    return gemm_TransAB_MICRO_TS16XMTS2(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else  
  {
     cout<<"Input matrix Size "<<"M = "<<M<<"N = "<<N<<"K = "<<K<<" not covered under wrapper sizes"<<endl; 
     return AMPBLAS_ERROR;
  }
}





