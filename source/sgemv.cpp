#include "ampblaslib.h"
#include <amp.h>
#define BLOCK_SIZE 256

using namespace concurrency;

static void gemv_TransA(Concurrency::accelerator_view &accl_view,
			Concurrency::array_view<float> &A_mat, long aOffset,
                        Concurrency::array_view<float> &X_vec, long xOffset,
                        Concurrency::array_view<float> &Y_vec, long yOffset,
                        float alpha, float beta, int lenX, int lenY,
                        Concurrency::array_view<float> &tempBuf)
{
  if((lenX - lenY) > 5000)
  {
    int len_X = (lenX + (BLOCK_SIZE - 1)) & ~(BLOCK_SIZE - 1);
    int num_blocks = len_X / BLOCK_SIZE;
    Concurrency::extent<1> grdExt(len_X);
    Concurrency::tiled_extent<BLOCK_SIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext,[=] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp)
    {
      tile_static float t[BLOCK_SIZE];
      for (int Col = 0; Col < lenY; Col++)
      {
        int blockIdx = tidx.tile[0];
        int threadIdx = tidx.local[0];
        tempBuf[Col * num_blocks + blockIdx] = 0;
        t[threadIdx] = 0;

        if (Col < lenY && blockIdx * BLOCK_SIZE + threadIdx < lenX)
          t[threadIdx] = X_vec[xOffset + blockIdx * BLOCK_SIZE + threadIdx] * A_mat[aOffset + Col * lenX + blockIdx * BLOCK_SIZE + threadIdx];

        tidx.barrier.wait();

        for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2)
        {
          if(threadIdx < stride)
            t[threadIdx] += t[threadIdx + stride];
        }
        tempBuf[Col * num_blocks + blockIdx] = t[0];
        tidx.barrier.wait();
      }

      if (tidx.tile[0] == 0)
      {
        for(int Col = 0; Col < lenY; Col++)
        {
          tile_static float sh[BLOCK_SIZE];
          int threadId = tidx.local[0];
          sh[tidx.local[0]] = 0;

          for (int i = threadId; i < num_blocks; i += tidx.tile_dim0)
            sh[threadId] += tempBuf[Col * num_blocks + i];

          tidx.barrier.wait();

          for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2)
          {
            if(threadId < stride)
              sh[threadId] += sh[threadId + stride];
          }
          tidx.barrier.wait();
          Y_vec[yOffset + Col] *= beta;
          Y_vec[yOffset + Col] += alpha * sh[0];
        }
      }
    });
  }
  else
  {
    Concurrency::extent<1> grdExt(lenY * BLOCK_SIZE);
    Concurrency::tiled_extent<BLOCK_SIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp)
    {
      int threadIdx = tidx.local[0];
      int blockIdx = tidx.tile[0];
      int Col = blockIdx;

      tile_static float sh[BLOCK_SIZE];
      sh[threadIdx] = 0;

      for (int tileId = 0; tileId < ((lenX + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1)) / BLOCK_SIZE; tileId++)
      {
        if (tileId * BLOCK_SIZE + threadIdx < lenX && Col < lenY)
          sh[threadIdx] += X_vec[xOffset + tileId * BLOCK_SIZE + threadIdx] * A_mat[aOffset + Col * lenX + tileId * BLOCK_SIZE + threadIdx];
      }
      tidx.barrier.wait();

      for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2)
      {
        if (threadIdx < stride)
          sh[threadIdx] += sh[threadIdx + stride];
        tidx.barrier.wait();
      }

      if(threadIdx == 0 && Col < lenY)
      {
        Y_vec[yOffset + Col] *= beta;
        Y_vec[yOffset + Col] += alpha * sh[0];
      }
    });
  }
}

static void gemv_TransA(Concurrency::accelerator_view &accl_view,
			Concurrency::array_view<float> &A_mat, long aOffset, long A_batchOffset,
                        Concurrency::array_view<float> &X_vec, long xOffset, long X_batchOffset,
                        Concurrency::array_view<float> &Y_vec, long yOffset, long Y_batchOffset,
                        float alpha, float beta, int lenX, int lenY,
                        Concurrency::array_view<float> &tempBuf, int batchSize)
{
  if((lenX - lenY) > 5000)
  {
    int len_X = (lenX + (BLOCK_SIZE - 1)) & ~(BLOCK_SIZE - 1);
    int num_blocks = len_X / BLOCK_SIZE;
    Concurrency::extent<2> grdExt(batchSize, len_X);
    Concurrency::tiled_extent<1,BLOCK_SIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext,[=] (Concurrency::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp)
    {
      tile_static float t[BLOCK_SIZE];
      int elt = tidx.tile[0];
      for (int Col = 0; Col < lenY; Col++)
      {
        int blockIdx = tidx.tile[1];
        int threadIdx = tidx.local[1];
        tempBuf[Col * num_blocks + blockIdx] = 0;
        t[threadIdx] = 0;

        if (Col < lenY && blockIdx * BLOCK_SIZE + threadIdx < lenX)
          t[threadIdx] = X_vec[xOffset + X_batchOffset * elt + blockIdx * BLOCK_SIZE + threadIdx] * A_mat[aOffset + A_batchOffset * elt + Col * lenX + blockIdx * BLOCK_SIZE + threadIdx];

        tidx.barrier.wait();

        for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2)
        {
          if(threadIdx < stride)
            t[threadIdx] += t[threadIdx + stride];
        }
        tempBuf[Col * num_blocks + blockIdx] = t[0];
        tidx.barrier.wait();
      }

      if (tidx.tile[1] == 0)
      {
        for(int Col = 0; Col < lenY; Col++)
        {
          tile_static float sh[BLOCK_SIZE];
          int threadId = tidx.local[1];
          sh[tidx.local[1]] = 0;

          for (int i = threadId; i < num_blocks; i += tidx.tile_dim0)
            sh[threadId] += tempBuf[Col * num_blocks + i];

          tidx.barrier.wait();

          for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2)
          {
            if(threadId < stride)
              sh[threadId] += sh[threadId + stride];
          }
          tidx.barrier.wait();
          Y_vec[yOffset + Y_batchOffset * elt + Col] *= beta;
          Y_vec[yOffset + Y_batchOffset * elt + Col] += alpha * sh[0];
        }
      }
    });
  }
  else
  {
    Concurrency::extent<2> grdExt(batchSize, lenY * BLOCK_SIZE);
    Concurrency::tiled_extent<1, BLOCK_SIZE> t_ext(grdExt);

    Concurrency::parallel_for_each(accl_view, t_ext, [=] (Concurrency::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp)
    {
      int elt = tidx.tile[0];
      int threadIdx = tidx.local[1];
      int blockIdx = tidx.tile[1];
      int Col = blockIdx;

      tile_static float sh[BLOCK_SIZE];
      sh[threadIdx] = 0;

      for (int tileId = 0; tileId < ((lenX + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1)) / BLOCK_SIZE; tileId++)
      {
        if (tileId * BLOCK_SIZE + threadIdx < lenX && Col < lenY)
          sh[threadIdx] += X_vec[xOffset + X_batchOffset * elt + tileId * BLOCK_SIZE + threadIdx] * A_mat[aOffset + A_batchOffset * elt + Col * lenX + tileId * BLOCK_SIZE + threadIdx];
      }
      tidx.barrier.wait();

      for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2)
      {
        if (threadIdx < stride)
          sh[threadIdx] += sh[threadIdx + stride];
        tidx.barrier.wait();
      }

      if(threadIdx == 0 && Col < lenY)
      {
        Y_vec[yOffset + Y_batchOffset * elt + Col] *= beta;
        Y_vec[yOffset + Y_batchOffset * elt + Col] += alpha * sh[0];
      }
    });
  }
}

#define TILE_SZ_A 16
#define TILE_SZ_A 16

static void gemv_TransA_register(Concurrency::accelerator_view &accl_view,
				 Concurrency::array_view<float> &A_mat, long aOffset,
                                 Concurrency::array_view<float> &X_vec, long xOffset,
                                 Concurrency::array_view<float> &Y_vec, long yOffset,
                                 float alpha, float beta, int lenX, int lenY,
                                 Concurrency::array_view<float> &tempBuf)
{

    Concurrency::extent<2> grdExt(((lenX - 1) / TILE_SZ_A + 1) * TILE_SZ_A, ((lenY - 1) / TILE_SZ_A + 1)*TILE_SZ_A);
    Concurrency::tiled_extent <TILE_SZ_A, TILE_SZ_A> t_ext(grdExt);
    Concurrency::parallel_for_each(accl_view, t_ext,
                                   [=] (Concurrency::tiled_index<TILE_SZ_A,TILE_SZ_A>tidx)
                                   restrict(amp) {

    // Shared memory for tiling input B array
    tile_static float A_s [TILE_SZ_A][TILE_SZ_A];

    // Macros for accessing flattened matrices
    #define A1(row,col) A_mat[(row) + (col) * lenX]
    const unsigned int row = tidx.local[0];
    const unsigned int col = tidx.global[1];

    float y_reg[TILE_SZ_A] = {(float)0};

    for(unsigned int tileIdx = 0; tileIdx < (lenX - 1)/TILE_SZ_A + 1; ++tileIdx) {
        if (tileIdx*TILE_SZ_A + row < lenX && col < lenY) {
            A_s[tidx.local[0]][tidx.local[1]] = A1(tileIdx*TILE_SZ_A + row, col);
        }
        else {
            A_s[tidx.local[0]][tidx.local[1]] = 0;
        }
        tidx.barrier.wait();

        for (unsigned int idx = 0; idx < TILE_SZ_A; ++idx) {
            float x_reg;
            if(tileIdx*TILE_SZ_A + idx < lenX) {
                x_reg = X_vec[tileIdx*TILE_SZ_A + idx];
            }
            else {
                x_reg = 0;
            }

            for(unsigned int outIdx = 0; outIdx < TILE_SZ_A; ++outIdx) {
                y_reg[outIdx] += x_reg*A_s[idx][outIdx];
            }
        }
          tidx.barrier.wait();
    }
    for (unsigned int outIdx = 0; outIdx < TILE_SZ_A; ++outIdx) {
        if (col < lenY) {
           Y_vec[tidx.tile[1] * TILE_SZ_A + outIdx] *= beta;
           Y_vec[tidx.tile[1] * TILE_SZ_A + outIdx] += y_reg[outIdx] * alpha;
        }
    }
});

}

static void gemv_NoTransA_register(Concurrency::accelerator_view &accl_view,Concurrency::array_view<float> &A, long aOffset,
                                   Concurrency::array_view<float> &X, long xOffset,
                                   Concurrency::array_view<float> &Y, long yOffset,
                                   float alpha, float beta, int lenX, int lenY)
{
    Concurrency::extent<2> grdExt( ((lenX - 1) / TILE_SZ_A + 1)*TILE_SZ_A, ((lenY - 1) / TILE_SZ_A + 1)*TILE_SZ_A);
    Concurrency::tiled_extent <TILE_SZ_A, TILE_SZ_A> t_ext(grdExt);
    Concurrency::parallel_for_each(accl_view, t_ext,
                                   [=] (Concurrency::tiled_index<TILE_SZ_A,TILE_SZ_A> tidx)
                                   restrict(amp) {

    tile_static float A_s [TILE_SZ_A][TILE_SZ_A];

    #define A(row,col) A[(row)*lenY + (col)]

    const unsigned int row = tidx.local[0];
    const unsigned int col = tidx.global[1];

    float y_reg[TILE_SZ_A] = {(float)0};

    for(unsigned int tileIdx = 0; tileIdx < (lenX - 1)/TILE_SZ_A + 1; ++tileIdx) {
        if (tileIdx*TILE_SZ_A + row < lenX && col < lenY) {
            A_s[tidx.local[0]][tidx.local[1]] = A(tileIdx*TILE_SZ_A + row, col);
        }
        else {
            A_s[tidx.local[0]][tidx.local[1]] = 0;
        }
        tidx.barrier.wait();

        for (unsigned int idx = 0; idx < TILE_SZ_A; ++idx) {
            float x_reg;
            if(tileIdx*TILE_SZ_A + idx < lenX) {
                x_reg = X[tileIdx*TILE_SZ_A + idx];
            }
            else {
                x_reg = 0;
            }

            for(unsigned int outIdx = 0; outIdx < TILE_SZ_A; ++outIdx) {
                y_reg[outIdx] += x_reg*A_s[idx][outIdx];
            }
        }
          tidx.barrier.wait();
    }
    for (unsigned int outIdx = 0; outIdx < TILE_SZ_A; ++outIdx) {
        if (col < lenY) {
           Y[tidx.tile[1] * TILE_SZ_A + outIdx] *= beta;
           Y[tidx.tile[1] * TILE_SZ_A + outIdx] += y_reg[outIdx] * alpha;
        }
    }
});


}
                                                                                                                                                                          




static void gemv_NoTransA(Concurrency::accelerator_view &accl_view,
			  Concurrency::array_view<float> &A, long aOffset, 
                          Concurrency::array_view<float> &X, long xOffset,
                          Concurrency::array_view<float> &Y, long yOffset,
                          float alpha, float beta, int lenX, int lenY)
{
  long size = (lenY + 255) & ~255;
  Concurrency::extent<1> compute_domain(size);

  Concurrency::parallel_for_each(accl_view, compute_domain.tile<BLOCK_SIZE>(),[=] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp)
  {
    int bx = tidx.tile[0];
    int tx = tidx.local[0];
    tile_static float Xds[BLOCK_SIZE];
    int Col = bx * BLOCK_SIZE + tx;
    float Pvalue = 0;

    for (int m = 0; m < (lenX - 1) / BLOCK_SIZE + 1; ++m)
    {
      if (m * BLOCK_SIZE + tx < lenX)
        Xds[tx] = X[xOffset + m * BLOCK_SIZE + tx];
      else
        Xds[tx]=0;

      tidx.barrier.wait();

      for (int k = 0; k < BLOCK_SIZE; k++)
        if (Col < lenY && m * BLOCK_SIZE + k < lenX)
          Pvalue += Xds[k] * A[aOffset + Col + (m * BLOCK_SIZE + k) * lenY];

      tidx.barrier.wait();
    }
    if (Col < lenY)
    {
      Y[yOffset + Col] *= beta;
      Y[yOffset + Col] += alpha * Pvalue;
    }
    tidx.barrier.wait();
  });
}

static void gemv_NoTransA(Concurrency::accelerator_view &accl_view,
			  Concurrency::array_view<float> &A, long aOffset, long A_batchOffset,
                          Concurrency::array_view<float> &X, long xOffset, long X_batchOffset,
                          Concurrency::array_view<float> &Y, long yOffset, long Y_batchOffset,
                          float alpha, float beta, int lenX, int lenY, int batchSize)
{
  long size = (lenY + 255) & ~255;
  Concurrency::extent<2> compute_domain(batchSize, size);

  Concurrency::parallel_for_each(accl_view, compute_domain.tile<1, BLOCK_SIZE>(),[=] (Concurrency::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp)
  {
    int elt = tidx.tile[0];
    int bx = tidx.tile[1];
    int tx = tidx.local[1];
    tile_static float Xds[BLOCK_SIZE];
    int Col = bx * BLOCK_SIZE + tx;
    float Pvalue = 0;

    for (int m = 0; m < (lenX - 1) / BLOCK_SIZE + 1; ++m)
    {
      if (m * BLOCK_SIZE + tx < lenX)
        Xds[tx] = X[xOffset + X_batchOffset * elt + m * BLOCK_SIZE + tx];
      else
        Xds[tx]=0;

      tidx.barrier.wait();

      for (int k = 0; k < BLOCK_SIZE; k++)
        if (Col < lenY && m * BLOCK_SIZE + k < lenX)
          Pvalue += Xds[k] * A[aOffset + A_batchOffset * elt + Col + (m * BLOCK_SIZE + k) * lenY];

      tidx.barrier.wait();
    }
    if (Col < lenY)
    {
      Y[yOffset + Y_batchOffset * elt + Col] *= beta;
      Y[yOffset + Y_batchOffset * elt + Col] += alpha * Pvalue;
    }
    tidx.barrier.wait();
  });
}

void gemv_AMP(Concurrency::accelerator_view &accl_view,
              char TransA, int M, int N, float alpha,
              Concurrency::array_view<float> &A, long aOffset,
              Concurrency::array_view<float> &X, long xOffset, long incX, float beta,
              Concurrency::array_view<float> &Y, long yOffset, long incY,
              Concurrency::array_view<float> &temp_buf)
{

  int lenX, lenY;
  if (M == 0 || N == 0)
    return;

  if (alpha == 0.0 && beta == 1.0)
    return;

  if (TransA == 'n')
  {
    lenX = N;
    lenY = M;
  }
  else
  {
    lenX = M;
    lenY = N;
  }

  if (TransA == 't')
    gemv_TransA(accl_view, A, aOffset, X, xOffset, Y, yOffset, alpha, beta, lenX, lenY, temp_buf);
  else if (TransA == 'n')
    gemv_NoTransA(accl_view, A, aOffset, X, xOffset, Y, yOffset, alpha, beta, lenX, lenY);
}

void gemv_AMP(Concurrency::accelerator_view &accl_view,
              char TransA, int M, int N, float alpha,
              Concurrency::array_view<float> &A, long aOffset, long A_batchOffset,
              Concurrency::array_view<float> &X, long xOffset, long X_batchOffset, 
              long incX, float beta,
              Concurrency::array_view<float> &Y, long yOffset, long Y_batchOffset,
              long incY, Concurrency::array_view<float> &temp_buf, int batchSize)
{

  int lenX, lenY;
  if (M == 0 || N == 0)
    return;

  if (alpha == 0.0 && beta == 1.0)
    return;

  if (TransA == 'n')
  {
    lenX = N;
    lenY = M;
  }
  else
  {
    lenX = M;
    lenY = N;
  }

  if (TransA == 't')
    gemv_TransA(accl_view, A, aOffset, A_batchOffset, X, xOffset, X_batchOffset, Y, yOffset, Y_batchOffset, alpha, beta, lenX, lenY, temp_buf, batchSize);
  else if (TransA == 'n')
    gemv_NoTransA(accl_view, A, aOffset, A_batchOffset, X, xOffset, X_batchOffset, Y, yOffset, Y_batchOffset, alpha, beta, lenX, lenY, batchSize);
}



ampblasStatus Ampblaslibrary :: ampblas_sgemv(const enum AMPBLAS_TRANS type,
                                              const int M, const int N,
                                              const float *alpha, float *A, const long aOffset,
                                              const int lda, float *X, const long xOffset,
                                              const int incX, const float *beta,
                                              float *Y,const long yOffset, const int incY)
{

    if(alpha == NULL || X == NULL || Y == NULL || A == NULL || M <= 0 || N <= 0 || beta == NULL )
        return AMPBLAS_INVALID;

    long lenXn = 1 + (N - 1) * abs(incX);
    long lenXt = 1 + (M - 1) * abs(incX);
    long lenYn = 1 + (M - 1) * abs(incY);
    long lenYt = 1 + (N - 1) * abs(incY);
    
    array_view<float> aMat(M * N, A);
    int num_blocks = lenXt / BLOCK_SIZE;
    float* temp = (float*)malloc(num_blocks * lenYt * sizeof(float));
    Concurrency::array_view<float> tempBuf(num_blocks * lenYt, temp);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());


    if( type == 'n')
    {
    Concurrency::array_view<float> xView(lenXn, X);
    Concurrency::array_view<float> yView(lenYn, Y);  
    gemv_AMP(accl_view, type, M, N, *alpha, aMat, aOffset, xView, xOffset, incX, *beta, yView, yOffset, incY, tempBuf);
    }
    
    
    if( type == 't')
    {
    Concurrency::array_view<float> xView(lenXt, X);
    Concurrency::array_view<float> yView(lenYt, Y);
    gemv_AMP(accl_view, type, M, N, *alpha, aMat, aOffset, xView, xOffset, incX, *beta, yView, yOffset, incY, tempBuf);
    }

    return AMPBLAS_SUCCESS;
}


ampblasStatus Ampblaslibrary :: ampblas_sgemv(Concurrency::accelerator_view &accl_view,
                                              const enum AMPBLAS_TRANS type, const int M,
                                              const int N, const float &alpha,
                                              Concurrency::array_view<float> &A, const long aOffset, const int lda,
                                              Concurrency::array_view<float> &X, const long xOffset, const int incX,
                                              const float &beta,
                                              Concurrency::array_view<float> &Y, const long yOffset, const int incY)
{ 
  /*Check the conditions*/
  if(M <= 0 || N <= 0)
        return AMPBLAS_INVALID;

  if( alpha == 0 && beta == 1)
        return AMPBLAS_SUCCESS;

    long lenXt = 1 + (M - 1) * abs(incX);
    long lenYt = 1 + (N - 1) * abs(incY);

    int num_blocks = lenXt / BLOCK_SIZE;
    float* temp = (float*)malloc(num_blocks * lenYt * sizeof(float));
    Concurrency::array_view<float> tempBuf(num_blocks * lenYt, temp);

    gemv_AMP(accl_view, type, M, N, alpha, A, aOffset, X, xOffset, incX, beta, Y, yOffset, incY, tempBuf);

    return AMPBLAS_SUCCESS;
}

ampblasStatus Ampblaslibrary :: ampblas_sgemv(Concurrency::accelerator_view &accl_view,
                                              const enum AMPBLAS_TRANS type, const int M,
                                              const int N, const float &alpha, Concurrency::array_view<float> &A,
                                              const long aOffset, const long A_batchOffset, const int lda,
                                              Concurrency::array_view<float> &X, 
                                              const long xOffset, const long X_batchOffset, const int incX,
                                              const float &beta, Concurrency::array_view<float> &Y, 
                                              const long yOffset, const long Y_batchOffset, const int incY, const int batchSize)
{
  /*Check the conditions*/
  if(M <= 0 || N <= 0)
        return AMPBLAS_INVALID;

  if( alpha == 0 && beta == 1)
        return AMPBLAS_SUCCESS;

    long lenXt = 1 + (M - 1) * abs(incX);
    long lenYt = 1 + (N - 1) * abs(incY);

    int num_blocks = lenXt / BLOCK_SIZE;
    float* temp = (float*)malloc(num_blocks * lenYt * sizeof(float));
    Concurrency::array_view<float> tempBuf(num_blocks * lenYt, temp);

    gemv_AMP(accl_view, type, M, N, alpha, A, aOffset, A_batchOffset, X, xOffset, X_batchOffset, incX, beta, Y, yOffset, Y_batchOffset, incY, tempBuf, batchSize);

    return AMPBLAS_SUCCESS;
}


