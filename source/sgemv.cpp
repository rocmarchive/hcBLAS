#include "ampblaslib.h"
#include <amp.h>
#define BLOCK_SIZE 256

using namespace concurrency;

static void gemv_TransA(Concurrency::array_view<float> &A_mat, int aOffset,
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

    Concurrency::parallel_for_each(t_ext,[=] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp)
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

    Concurrency::parallel_for_each(t_ext, [=] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp)
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

static void gemv_NoTransA(Concurrency::array_view<float> &A, long aOffset,
                          Concurrency::array_view<float> &X, long xOffset,
                          Concurrency::array_view<float> &Y, long yOffset,
                          float alpha, float beta, int lenX, int lenY)
{
  long size = (lenY + 255) & ~255;
  Concurrency::extent<1> compute_domain(size);

  Concurrency::parallel_for_each(compute_domain.tile<BLOCK_SIZE>(),[=] (Concurrency::tiled_index<BLOCK_SIZE> tidx) restrict(amp)
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

void gemv_AMP(char TransA, int M, int N, float alpha,
              Concurrency::array_view<float> &A, long aOffset,
              Concurrency::array_view<float> &X, long xOffset, long incX, float beta,
              Concurrency::array_view<float> &Y, long yOffset, long incY,
              Concurrency::array_view<float> &temp_buf)
{
  if (alpha == 0.0)
    return;

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
    gemv_TransA(A, aOffset, X, xOffset, Y, yOffset, alpha, beta, lenX, lenY, temp_buf);
  else if (TransA == 'n')
    gemv_NoTransA(A, aOffset, X, xOffset, Y, yOffset, alpha, beta, lenX, lenY);
}

ampblasStatus Ampblaslibrary :: ampblas_sgemv(const enum AMPBLAS_ORDER order,
                                              const enum AMPBLAS_TRANS type,
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

    if( type == 'n')
    {
    Concurrency::array_view<float> xView(lenXn, X);
    Concurrency::array_view<float> yView(lenYn, Y);  
    gemv_AMP(type, M, N, *alpha, aMat, aOffset, xView, xOffset, incX, *beta, yView, yOffset, incY, tempBuf);
    aMat.synchronize();
    }
    
    
    if( type == 't')
    {
    Concurrency::array_view<float> xView(lenXt, X);
    Concurrency::array_view<float> yView(lenYt, Y);
    gemv_AMP(type, M, N, *alpha, aMat, aOffset, xView, xOffset, incX, *beta, yView, yOffset, incY, tempBuf);
    aMat.synchronize();
    }
    return AMPBLAS_SUCCESS;
}
