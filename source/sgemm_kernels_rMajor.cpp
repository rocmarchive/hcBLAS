#include "sgemm_kernels.h"

ampblasStatus gemm_NoTransAB_rMajor_STEP_NBK_TS8XSS8(Concurrency::accelerator_view &accl_view,
                                                     Concurrency::array_view<float, 1> &A, long aOffset,
                                                     Concurrency::array_view<float, 1> &B, long bOffset,
                                                     Concurrency::array_view<float, 1> &C, long cOffset,
                                                     int M, int N, int K, int lda, int ldb, int ldc,
                                                     float alpha, float beta)
{
#define TILESIZE 8
#define STEPSIZE 8
  cout<<"step nbk no trans AB row"<< endl;
  Concurrency::extent<2> grdExt((M + (TILESIZE - 1)) & ~(TILESIZE - 1), (N + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

  Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
    float rC[1][1] = {{(float)0}};
    float rA[1][STEPTILERATIO];
    float rB[1][STEPTILERATIO];
    tile_static float lA[STEPTILEPROD + STEPSIZE];
    tile_static float lB[STEPTILEPROD + STEPSIZE];
    int gidx = tidx.tile[0];
    int gidy = tidx.tile[1];
    int idx = tidx.local[0];
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
        C[cOffset + (gidx * TILESIZE + idx)*ldc + (gidy * TILESIZE + idy)] = alpha * rC[0][0] + beta * C[cOffset + (gidx * TILESIZE + idx)*ldc + (gidy * TILESIZE + idy)];
  });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;

}

ampblasStatus gemm_NoTransAB_rMajor_STEP_NBK_TS16XSS16(Concurrency::accelerator_view &accl_view,
                                                     Concurrency::array_view<float, 1> &A, long aOffset,
                                                     Concurrency::array_view<float, 1> &B, long bOffset,
                                                     Concurrency::array_view<float, 1> &C, long cOffset,
                                                     int M, int N, int K, int lda, int ldb, int ldc,
                                                     float alpha, float beta)
{
#define TILESIZE 16
#define STEPSIZE 16
  cout<<"step nbk no trans AB row 16"<< endl;
  Concurrency::extent<2> grdExt((M + (TILESIZE - 1)) & ~(TILESIZE - 1), (N + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

  Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    int shiftFactor = Concurrency::fast_math::log2(STEPSIZE);
    float rC[1][1] = {{(float)0}};
    float rA[1][STEPTILERATIO];
    float rB[1][STEPTILERATIO];
    tile_static float lA[STEPTILEPROD + STEPSIZE];
    tile_static float lB[STEPTILEPROD + STEPSIZE];
    int gidx = tidx.tile[0];
    int gidy = tidx.tile[1];
    int idx = tidx.local[0];
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
        C[cOffset + (gidx * TILESIZE + idx)*ldc + (gidy * TILESIZE + idy)] = alpha * rC[0][0] + beta * C[cOffset + (gidx * TILESIZE + idx)*ldc + (gidy * TILESIZE + idy)];
  });
#undef TILESIZE
#undef STEPSIZE
    return AMPBLAS_SUCCESS;

}

ampblasStatus gemm_NoTransAB_rMajor_MICRO_TS16XMTS2(Concurrency::accelerator_view &accl_view,
                                  Concurrency::array_view<float, 1> &A, long aOffset,
                                  Concurrency::array_view<float, 1> &B, long bOffset,
                                  Concurrency::array_view<float, 1> &C, long cOffset,
                                  int M, int N, int K, int lda, int ldb, int ldc,
                                  float alpha, float beta)
{
#define TILESIZE 16
#define MICROTILESIZE 2
  Concurrency::extent<2> grdExt(((M / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1), ((N / 2) + (TILESIZE - 1)) & ~(TILESIZE - 1));
  Concurrency::tiled_extent<TILESIZE, TILESIZE> t_ext(grdExt);

  Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<TILESIZE, TILESIZE> tidx) restrict(amp)
  {
    float rC[MICROTILESIZE][MICROTILESIZE] = {(float)0};
    float rA[1][MICROTILESIZE];
    float rB[1][MICROTILESIZE];
    tile_static float lA[TILESIZE * TILESIZE * MICROTILESIZE];
    tile_static float lB[TILESIZE * TILESIZE * MICROTILESIZE];
    int gidx = tidx.tile[0];
    int gidy = tidx.tile[1];
    int idx = tidx.local[0];
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

    int xIndex = (gidx * TILESIZE * MICROTILESIZE + idx) * ldc;
    int yIndex = (gidy * TILESIZE * MICROTILESIZE + idy);
    for( int row = 0; row < MICROTILESIZE; row++)
    {
      for( int col = 0; col < MICROTILESIZE ; col++)
      {
      if((xIndex/ldc) + (TILESIZE * col) < M && (yIndex) + (TILESIZE * row) < N)
        C[cOffset + (xIndex + (TILESIZE * col) * N) + yIndex + (TILESIZE * row)] = alpha * rC[col][row] + beta * C[cOffset + (xIndex + (TILESIZE * col) * N) + yIndex + (TILESIZE * row)];
      }
    }
 });
#undef TILESIZE
#undef MICROTILESIZE 
    return AMPBLAS_SUCCESS;

}


ampblasStatus gemm_NoTransAB_rMajor(Concurrency::accelerator_view &accl_view,
                                    Concurrency::array_view<float, 1> &A, long aOffset,
                                    Concurrency::array_view<float, 1> &B, long bOffset,
                                    Concurrency::array_view<float, 1> &C, long cOffset,
                                    int M, int N, int K, int lda, int ldb, int ldc,
                                    float alpha, float beta)
{
  if ((M < 600 && N < 600 && K < 10) || (M < 1800 && N < 600 && K < 600))
  {
    return gemm_NoTransAB_rMajor_STEP_NBK_TS8XSS8(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  if ((M < 600 && N < 600 && K < 1800) || (M < 1800 && ((N < 600 && K < 1800) || (N < 1800 && K < 10))))
  {
    return gemm_NoTransAB_rMajor_STEP_NBK_TS16XSS16(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
  else
  {
    return gemm_NoTransAB_rMajor_MICRO_TS16XMTS2(accl_view, A, aOffset, B, bOffset, C, cOffset, M, N, K, lda, ldb, ldc, alpha, beta);
  }
}

ampblasStatus gemm_NoTransA_rMajor(Concurrency::accelerator_view &accl_view,
		    		   Concurrency::array_view<float, 1> &A, long aOffset,
                                   Concurrency::array_view<float, 1> &B, long bOffset,
                                   Concurrency::array_view<float, 1> &C, long cOffset,
                                   int M, int N, int K, int lda, int ldb, int ldc,
                                   float alpha, float beta)
{
} 


ampblasStatus gemm_NoTransB_rMajor(Concurrency::accelerator_view &accl_view,
                     		   Concurrency::array_view<float, 1> &A, long aOffset,
                                   Concurrency::array_view<float, 1> &B, long bOffset,
                                   Concurrency::array_view<float, 1> &C, long cOffset,
                                   int M, int N, int K, int lda, int ldb, int ldc,
                                   float alpha, float beta)
{
   
}

ampblasStatus gemm_TransAB_rMajor(Concurrency::accelerator_view &accl_view,
                                  Concurrency::array_view<float, 1> &A, long aOffset,
                                  Concurrency::array_view<float, 1> &B, long bOffset,
                                  Concurrency::array_view<float, 1> &C, long cOffset,
                                  int M, int N, int K, int lda, int ldb, int ldc,
                                  float alpha, float beta)
{
  
}
