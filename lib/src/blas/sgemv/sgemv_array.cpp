#include "hcblas.h"
#include <hc.hpp>
#include <hc_math.hpp>
#define BLOCK_SIZE 256
using namespace concurrency::fast_math;
using namespace concurrency;

static void gemv_TransA(hc::accelerator_view &accl_view,
                        hc::array<float> &A_mat, long aOffset,
                        hc::array<float> &X_vec, long xOffset,
                        hc::array<float> &Y_vec, long yOffset,
                        float alpha, float beta, int lenX, int lenY) {
  if((lenX - lenY) > 5000) {
    int len_X = (lenX + (BLOCK_SIZE - 1)) & ~(BLOCK_SIZE - 1);
    int len_Y = (lenY + (BLOCK_SIZE - 1)) & ~(BLOCK_SIZE - 1);
    int num_blocks = len_X / BLOCK_SIZE;
    float* temp = (float*)malloc(num_blocks * len_Y * sizeof(float));
    hc::array<float> tempBuf( num_blocks * len_Y, temp);
    hc::extent<1> grdExt(len_X);
    hc::tiled_extent<BLOCK_SIZE> t_ext(grdExt);
    hc::parallel_for_each(accl_view, t_ext, [ =, &A_mat, &X_vec, &Y_vec, &tempBuf] (hc::tiled_index<BLOCK_SIZE> tidx) restrict(amp) {
      tile_static float t[BLOCK_SIZE];

      for (int Col = 0; Col < lenY; Col++) {
        int blockIdx = tidx.tile[0];
        int threadIdx = tidx.local[0];
        tempBuf[Col * num_blocks + blockIdx] = 0;
        t[threadIdx] = 0;

        if (Col < lenY && blockIdx * BLOCK_SIZE + threadIdx < lenX) {
          t[threadIdx] = X_vec[xOffset + blockIdx * BLOCK_SIZE + threadIdx] * A_mat[aOffset + Col * lenX + blockIdx * BLOCK_SIZE + threadIdx];
        }

        tidx.barrier.wait();

        for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
          if(threadIdx < stride) {
            t[threadIdx] += t[threadIdx + stride];
          }
        }

        tempBuf[Col * num_blocks + blockIdx] = t[0];
        tidx.barrier.wait();
      }

      if (tidx.tile[0] == 0) {
        for(int Col = 0; Col < lenY; Col++) {
          tile_static float sh[BLOCK_SIZE];
          int threadId = tidx.local[0];
          sh[tidx.local[0]] = 0;

          for (int i = threadId; i < num_blocks; i += tidx.tile_dim0) {
            sh[threadId] += tempBuf[Col * num_blocks + i];
          }

          tidx.barrier.wait();

          for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
            if(threadId < stride) {
              sh[threadId] += sh[threadId + stride];
            }
          }

          tidx.barrier.wait();
          long Y_index = yOffset + Col;
          Y_vec[Y_index] = (isnan(Y_vec[Y_index]) || isinf(Y_vec[Y_index])) ? 0 : Y_vec[Y_index];
          Y_vec[Y_index] *= beta;
          Y_vec[Y_index] += alpha * sh[0];
        }
      }
    });
  } else {
    hc::extent<1> grdExt(lenY * BLOCK_SIZE);
    hc::tiled_extent<BLOCK_SIZE> t_ext(grdExt);
    hc::parallel_for_each(accl_view, t_ext, [ =, &A_mat, &X_vec, &Y_vec] (hc::tiled_index<BLOCK_SIZE> tidx) restrict(amp) {
      int threadIdx = tidx.local[0];
      int blockIdx = tidx.tile[0];
      int Col = blockIdx;
      tile_static float sh[BLOCK_SIZE];
      sh[threadIdx] = 0;

      for (int tileId = 0; tileId < ((lenX + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1)) / BLOCK_SIZE; tileId++) {
        if (tileId * BLOCK_SIZE + threadIdx < lenX && Col < lenY) {
          sh[threadIdx] += X_vec[xOffset + tileId * BLOCK_SIZE + threadIdx] * A_mat[aOffset + Col * lenX + tileId * BLOCK_SIZE + threadIdx];
        }
      }

      tidx.barrier.wait();

      for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
        if (threadIdx < stride) {
          sh[threadIdx] += sh[threadIdx + stride];
        }

        tidx.barrier.wait();
      }

      if(threadIdx == 0 && Col < lenY) {
        long Y_index = yOffset + Col;
        Y_vec[Y_index] = (isnan(Y_vec[Y_index]) || isinf(Y_vec[Y_index])) ? 0 : Y_vec[Y_index];
        Y_vec[Y_index] *= beta;
        Y_vec[Y_index] += alpha * sh[0];
      }
    });
  }
}

static void gemv_TransA(hc::accelerator_view &accl_view,
                        hc::array<float> &A_mat, long aOffset, long A_batchOffset,
                        hc::array<float> &X_vec, long xOffset, long X_batchOffset,
                        hc::array<float> &Y_vec, long yOffset, long Y_batchOffset,
                        float alpha, float beta, int lenX, int lenY, int batchSize) {
  if((lenX - lenY) > 5000 ) {
    int len_X = (lenX + (BLOCK_SIZE - 1)) & ~(BLOCK_SIZE - 1);
    int len_Y = (lenY + (BLOCK_SIZE - 1)) & ~(BLOCK_SIZE - 1);
    int num_blocks = len_X / BLOCK_SIZE;
    float* temp = (float*)malloc(num_blocks * len_Y * sizeof(float));
    hc::array<float> tempBuf( num_blocks * len_Y, temp);
    hc::extent<2> grdExt(batchSize, len_X);
    hc::tiled_extent<1, BLOCK_SIZE> t_ext(grdExt);
    hc::parallel_for_each(accl_view, t_ext, [ =, &A_mat, &X_vec, &Y_vec, &tempBuf] (hc::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp) {
      tile_static float t[BLOCK_SIZE];
      int elt = tidx.tile[0];

      for (int Col = 0; Col < lenY; Col++) {
        int blockIdx = tidx.tile[1];
        int threadIdx = tidx.local[1];
        tempBuf[Col * num_blocks + blockIdx] = 0;
        t[threadIdx] = 0;

        if (Col < lenY && blockIdx * BLOCK_SIZE + threadIdx < lenX) {
          t[threadIdx] = X_vec[xOffset + X_batchOffset * elt + blockIdx * BLOCK_SIZE + threadIdx] * A_mat[aOffset + A_batchOffset * elt + Col * lenX + blockIdx * BLOCK_SIZE + threadIdx];
        }

        tidx.barrier.wait();

        for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
          if(threadIdx < stride) {
            t[threadIdx] += t[threadIdx + stride];
          }
        }

        tempBuf[Col * num_blocks + blockIdx] = t[0];
        tidx.barrier.wait();
      }

      if (tidx.tile[1] == 0) {
        for(int Col = 0; Col < lenY; Col++) {
          tile_static float sh[BLOCK_SIZE];
          int threadId = tidx.local[1];
          sh[tidx.local[1]] = 0;

          for (int i = threadId; i < num_blocks; i += tidx.tile_dim0) {
            sh[threadId] += tempBuf[Col * num_blocks + i];
          }

          tidx.barrier.wait();

          for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
            if(threadId < stride) {
              sh[threadId] += sh[threadId + stride];
            }
          }

          tidx.barrier.wait();
          long Y_index = yOffset + Y_batchOffset * elt + Col;
          Y_vec[Y_index] = (isnan(Y_vec[Y_index]) || isinf(Y_vec[Y_index])) ? 0 : Y_vec[Y_index];
          Y_vec[Y_index] *= beta;
          Y_vec[Y_index] += alpha * sh[0];
        }
      }
    });
  } else {
    hc::extent<2> grdExt(batchSize, lenY * BLOCK_SIZE);
    hc::tiled_extent<1, BLOCK_SIZE> t_ext(grdExt);
    hc::parallel_for_each(accl_view, t_ext, [ =, &A_mat, &X_vec, &Y_vec] (hc::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp) {
      int elt = tidx.tile[0];
      int threadIdx = tidx.local[1];
      int blockIdx = tidx.tile[1];
      int Col = blockIdx;
      tile_static float sh[BLOCK_SIZE];
      sh[threadIdx] = 0;

      for (int tileId = 0; tileId < ((lenX + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1)) / BLOCK_SIZE; tileId++) {
        if (tileId * BLOCK_SIZE + threadIdx < lenX && Col < lenY) {
          sh[threadIdx] += X_vec[xOffset + X_batchOffset * elt + tileId * BLOCK_SIZE + threadIdx] * A_mat[aOffset + A_batchOffset * elt + Col * lenX + tileId * BLOCK_SIZE + threadIdx];
        }
      }

      tidx.barrier.wait();

      for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
        if (threadIdx < stride) {
          sh[threadIdx] += sh[threadIdx + stride];
        }

        tidx.barrier.wait();
      }

      if(threadIdx == 0 && Col < lenY) {
        long Y_index = yOffset + Y_batchOffset * elt + Col;
        Y_vec[Y_index] = (isnan(Y_vec[Y_index]) || isinf(Y_vec[Y_index])) ? 0 : Y_vec[Y_index];
        Y_vec[Y_index] *= beta;
        Y_vec[Y_index] += alpha * sh[0];
      }
    });
  }
}

static void gemv_TransA_rMajor(hc::accelerator_view &accl_view,
                               hc::array<float> &A_mat, long aOffset,
                               hc::array<float> &X_vec, long xOffset,
                               hc::array<float> &Y_vec, long yOffset,
                               float alpha, float beta, int lenX, int lenY) {
  if((lenX - lenY) > 5000) {
    int len_X = (lenX + (BLOCK_SIZE - 1)) & ~(BLOCK_SIZE - 1);
    int len_Y = (lenY + (BLOCK_SIZE - 1)) & ~(BLOCK_SIZE - 1);
    int num_blocks = len_X / BLOCK_SIZE;
    float* temp = (float*)malloc(num_blocks * len_Y * sizeof(float));
    hc::array<float> tempBuf( num_blocks * len_Y, temp);
    hc::extent<1> grdExt(len_X);
    hc::tiled_extent<BLOCK_SIZE> t_ext(grdExt);
    hc::parallel_for_each(accl_view, t_ext, [ =, &A_mat, &X_vec, &Y_vec, &tempBuf] (hc::tiled_index<BLOCK_SIZE> tidx) restrict(amp) {
      tile_static float t[BLOCK_SIZE];

      for (int Col = 0; Col < lenY; Col++) {
        int blockIdx = tidx.tile[0];
        int threadIdx = tidx.local[0];
        tempBuf[Col * num_blocks + blockIdx] = 0;
        t[threadIdx] = 0;

        if (Col < lenY && blockIdx * BLOCK_SIZE + threadIdx < lenX) {
          t[threadIdx] = X_vec[xOffset + blockIdx * BLOCK_SIZE + threadIdx] * A_mat[aOffset + Col  + (blockIdx * BLOCK_SIZE + threadIdx) * lenY];
        }

        tidx.barrier.wait();

        for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
          if(threadIdx < stride) {
            t[threadIdx] += t[threadIdx + stride];
          }
        }

        tempBuf[Col * num_blocks + blockIdx] = t[0];
        tidx.barrier.wait();
      }

      if (tidx.tile[0] == 0) {
        for(int Col = 0; Col < lenY; Col++) {
          tile_static float sh[BLOCK_SIZE];
          int threadId = tidx.local[0];
          sh[tidx.local[0]] = 0;

          for (int i = threadId; i < num_blocks; i += tidx.tile_dim0) {
            sh[threadId] += tempBuf[Col * num_blocks + i];
          }

          tidx.barrier.wait();

          for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
            if(threadId < stride) {
              sh[threadId] += sh[threadId + stride];
            }
          }

          tidx.barrier.wait();
          long Y_index = yOffset + Col;
          Y_vec[Y_index] = (isnan(Y_vec[Y_index]) || isinf(Y_vec[Y_index])) ? 0 : Y_vec[Y_index];
          Y_vec[Y_index] *= beta;
          Y_vec[Y_index] += alpha * sh[0];
        }
      }
    });
  } else {
    hc::extent<1> grdExt(lenY * BLOCK_SIZE);
    hc::tiled_extent<BLOCK_SIZE> t_ext(grdExt);
    hc::parallel_for_each(accl_view, t_ext, [ =, &A_mat, &X_vec, &Y_vec] (hc::tiled_index<BLOCK_SIZE> tidx) restrict(amp) {
      int threadIdx = tidx.local[0];
      int blockIdx = tidx.tile[0];
      int Col = blockIdx;
      tile_static float sh[BLOCK_SIZE];
      sh[threadIdx] = 0;

      for (int tileId = 0; tileId < ((lenX + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1)) / BLOCK_SIZE; tileId++) {
        if (tileId * BLOCK_SIZE + threadIdx < lenX && Col < lenY) {
          sh[threadIdx] += X_vec[xOffset + tileId * BLOCK_SIZE + threadIdx] * A_mat[aOffset + Col + (tileId * BLOCK_SIZE + threadIdx) * lenY];
        }
      }

      tidx.barrier.wait();

      for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
        if (threadIdx < stride) {
          sh[threadIdx] += sh[threadIdx + stride];
        }

        tidx.barrier.wait();
      }

      if(threadIdx == 0 && Col < lenY) {
        long Y_index = yOffset + Col;
        Y_vec[Y_index] = (isnan(Y_vec[Y_index]) || isinf(Y_vec[Y_index])) ? 0 : Y_vec[Y_index];
        Y_vec[Y_index] *= beta;
        Y_vec[Y_index] += alpha * sh[0];
      }
    });
  }
}

static void gemv_TransA_rMajor(hc::accelerator_view &accl_view,
                               hc::array<float> &A_mat, long aOffset, long A_batchOffset,
                               hc::array<float> &X_vec, long xOffset, long X_batchOffset,
                               hc::array<float> &Y_vec, long yOffset, long Y_batchOffset,
                               float alpha, float beta, int lenX, int lenY, int batchSize) {
  if((lenX - lenY) > 5000) {
    int len_X = (lenX + (BLOCK_SIZE - 1)) & ~(BLOCK_SIZE - 1);
    int len_Y = (lenY + (BLOCK_SIZE - 1)) & ~(BLOCK_SIZE - 1);
    int num_blocks = len_X / BLOCK_SIZE;
    float* temp = (float*)malloc(num_blocks * len_Y * sizeof(float));
    hc::array<float> tempBuf( num_blocks * len_Y, temp);
    hc::extent<2> grdExt(batchSize, len_X);
    hc::tiled_extent<1, BLOCK_SIZE> t_ext(grdExt);
    hc::parallel_for_each(accl_view, t_ext, [ =, &A_mat, &X_vec, &Y_vec, &tempBuf] (hc::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp) {
      tile_static float t[BLOCK_SIZE];
      int elt = tidx.tile[0];

      for (int Col = 0; Col < lenY; Col++) {
        int blockIdx = tidx.tile[1];
        int threadIdx = tidx.local[1];
        tempBuf[Col * num_blocks + blockIdx] = 0;
        t[threadIdx] = 0;

        if (Col < lenY && blockIdx * BLOCK_SIZE + threadIdx < lenX) {
          t[threadIdx] = X_vec[xOffset + X_batchOffset * elt + blockIdx * BLOCK_SIZE + threadIdx] * A_mat[aOffset + A_batchOffset * elt + Col + (blockIdx * BLOCK_SIZE + threadIdx) * lenY];
        }

        tidx.barrier.wait();

        for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
          if(threadIdx < stride) {
            t[threadIdx] += t[threadIdx + stride];
          }
        }

        tempBuf[Col * num_blocks + blockIdx] = t[0];
        tidx.barrier.wait();
      }

      if (tidx.tile[1] == 0) {
        for(int Col = 0; Col < lenY; Col++) {
          tile_static float sh[BLOCK_SIZE];
          int threadId = tidx.local[1];
          sh[tidx.local[1]] = 0;

          for (int i = threadId; i < num_blocks; i += tidx.tile_dim0) {
            sh[threadId] += tempBuf[Col * num_blocks + i];
          }

          tidx.barrier.wait();

          for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
            if(threadId < stride) {
              sh[threadId] += sh[threadId + stride];
            }
          }

          tidx.barrier.wait();
          long Y_index = yOffset + Y_batchOffset * elt + Col;
          Y_vec[Y_index] = (isnan(Y_vec[Y_index]) || isinf(Y_vec[Y_index])) ? 0 : Y_vec[Y_index];
          Y_vec[Y_index] *= beta;
          Y_vec[Y_index] += alpha * sh[0];
        }
      }
    });
  } else {
    hc::extent<2> grdExt(batchSize, lenY * BLOCK_SIZE);
    hc::tiled_extent<1, BLOCK_SIZE> t_ext(grdExt);
    hc::parallel_for_each(accl_view, t_ext, [ =, &A_mat, &X_vec, &Y_vec] (hc::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp) {
      int elt = tidx.tile[0];
      int threadIdx = tidx.local[1];
      int blockIdx = tidx.tile[1];
      int Col = blockIdx;
      tile_static float sh[BLOCK_SIZE];
      sh[threadIdx] = 0;

      for (int tileId = 0; tileId < ((lenX + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1)) / BLOCK_SIZE; tileId++) {
        if (tileId * BLOCK_SIZE + threadIdx < lenX && Col < lenY) {
          sh[threadIdx] += X_vec[xOffset + X_batchOffset * elt + tileId * BLOCK_SIZE + threadIdx] * A_mat[aOffset + A_batchOffset * elt + Col + (tileId * BLOCK_SIZE + threadIdx) * lenY];
        }
      }

      tidx.barrier.wait();

      for (int stride = BLOCK_SIZE / 2; stride >= 1; stride /= 2) {
        if (threadIdx < stride) {
          sh[threadIdx] += sh[threadIdx + stride];
        }

        tidx.barrier.wait();
      }

      if(threadIdx == 0 && Col < lenY) {
        long Y_index = yOffset + Y_batchOffset * elt + Col;
        Y_vec[Y_index] = (isnan(Y_vec[Y_index]) || isinf(Y_vec[Y_index])) ? 0 : Y_vec[Y_index];
        Y_vec[Y_index] *= beta;
        Y_vec[Y_index] += alpha * sh[0];
      }
    });
  }
}

static void gemv_NoTransA(hc::accelerator_view &accl_view,
                          hc::array<float> &A, long aOffset,
                          hc::array<float> &X, long xOffset,
                          hc::array<float> &Y, long yOffset,
                          float alpha, float beta, int lenX, int lenY) {
  long size = (lenY + 255) & ~255;
  hc::extent<1> compute_domain(size);
  hc::parallel_for_each(accl_view, compute_domain.tile<BLOCK_SIZE>(), [ =, &A, &X, &Y] (hc::tiled_index<BLOCK_SIZE> tidx) restrict(amp) {
    int bx = tidx.tile[0];
    int tx = tidx.local[0];
    tile_static float Xds[BLOCK_SIZE];
    int Col = bx * BLOCK_SIZE + tx;
    float Pvalue = 0;

    for (int m = 0; m < (lenX - 1) / BLOCK_SIZE + 1; ++m) {
      if (m * BLOCK_SIZE + tx < lenX) {
        Xds[tx] = X[xOffset + m * BLOCK_SIZE + tx];
      } else {
        Xds[tx] = 0;
      }

      tidx.barrier.wait();

      for (int k = 0; k < BLOCK_SIZE; k++)
        if (Col < lenY && m * BLOCK_SIZE + k < lenX) {
          Pvalue += Xds[k] * A[aOffset + Col + (m * BLOCK_SIZE + k) * lenY];
        }

      tidx.barrier.wait();
    }

    if (Col < lenY) {
      long Y_index = yOffset + Col;
      Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
      Y[Y_index] *= beta;
      Y[Y_index] += alpha * Pvalue;
    }

    tidx.barrier.wait();
  });
}

static void gemv_NoTransA(hc::accelerator_view &accl_view,
                          hc::array<float> &A, long aOffset, long A_batchOffset,
                          hc::array<float> &X, long xOffset, long X_batchOffset,
                          hc::array<float> &Y, long yOffset, long Y_batchOffset,
                          float alpha, float beta, int lenX, int lenY, int batchSize) {
  long size = (lenY + 255) & ~255;
  hc::extent<2> compute_domain(batchSize, size);
  hc::parallel_for_each(accl_view, compute_domain.tile<1, BLOCK_SIZE>(), [ =, &A, &X, &Y] (hc::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp) {
    int elt = tidx.tile[0];
    int bx = tidx.tile[1];
    int tx = tidx.local[1];
    tile_static float Xds[BLOCK_SIZE];
    int Col = bx * BLOCK_SIZE + tx;
    float Pvalue = 0;

    for (int m = 0; m < (lenX - 1) / BLOCK_SIZE + 1; ++m) {
      if (m * BLOCK_SIZE + tx < lenX) {
        Xds[tx] = X[xOffset + X_batchOffset * elt + m * BLOCK_SIZE + tx];
      } else {
        Xds[tx] = 0;
      }

      tidx.barrier.wait();

      for (int k = 0; k < BLOCK_SIZE; k++)
        if (Col < lenY && m * BLOCK_SIZE + k < lenX) {
          Pvalue += Xds[k] * A[aOffset + A_batchOffset * elt + Col + (m * BLOCK_SIZE + k) * lenY];
        }

      tidx.barrier.wait();
    }

    if (Col < lenY) {
      long Y_index = yOffset + Y_batchOffset * elt + Col;
      Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
      Y[Y_index] *= beta;
      Y[Y_index] += alpha * Pvalue;
    }

    tidx.barrier.wait();
  });
}

static void gemv_NoTransA_rMajor(hc::accelerator_view &accl_view,
                                 hc::array<float> &A, long aOffset,
                                 hc::array<float> &X, long xOffset,
                                 hc::array<float> &Y, long yOffset,
                                 float alpha, float beta, int lenX, int lenY) {
  long size = (lenY + 255) & ~255;
  hc::extent<1> compute_domain(size);
  hc::parallel_for_each(accl_view, compute_domain.tile<BLOCK_SIZE>(), [ =, &A, &X, &Y] (hc::tiled_index<BLOCK_SIZE> tidx) restrict(amp) {
    int bx = tidx.tile[0];
    int tx = tidx.local[0];
    tile_static float Xds[BLOCK_SIZE];
    int Col = bx * BLOCK_SIZE + tx;
    float Pvalue = 0;

    for (int m = 0; m < (lenX - 1) / BLOCK_SIZE + 1; ++m) {
      if (m * BLOCK_SIZE + tx < lenX) {
        Xds[tx] = X[xOffset + m * BLOCK_SIZE + tx];
      } else {
        Xds[tx] = 0;
      }

      tidx.barrier.wait();

      for (int k = 0; k < BLOCK_SIZE; k++)
        if (Col < lenY && m * BLOCK_SIZE + k < lenX) {
          Pvalue += Xds[k] * A[aOffset + Col * lenX + m * BLOCK_SIZE + k];
        }

      tidx.barrier.wait();
    }

    if (Col < lenY) {
      long Y_index = yOffset + Col;
      Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
      Y[Y_index] *= beta;
      Y[Y_index] += alpha * Pvalue;
    }

    tidx.barrier.wait();
  });
}




static void gemv_NoTransA_rMajor(hc::accelerator_view &accl_view,
                                 hc::array<float> &A, long aOffset, long A_batchOffset,
                                 hc::array<float> &X, long xOffset, long X_batchOffset,
                                 hc::array<float> &Y, long yOffset, long Y_batchOffset,
                                 float alpha, float beta, int lenX, int lenY, int batchSize) {
  long size = (lenY + 255) & ~255;
  hc::extent<2> compute_domain(batchSize, size);
  hc::parallel_for_each(accl_view, compute_domain.tile<1, BLOCK_SIZE>(), [ =, &A, &X, &Y] (hc::tiled_index<1, BLOCK_SIZE> tidx) restrict(amp) {
    int elt = tidx.tile[0];
    int bx = tidx.tile[1];
    int tx = tidx.local[1];
    tile_static float Xds[BLOCK_SIZE];
    int Col = bx * BLOCK_SIZE + tx;
    float Pvalue = 0;

    for (int m = 0; m < (lenX - 1) / BLOCK_SIZE + 1; ++m) {
      if (m * BLOCK_SIZE + tx < lenX) {
        Xds[tx] = X[xOffset + X_batchOffset * elt + m * BLOCK_SIZE + tx];
      } else {
        Xds[tx] = 0;
      }

      tidx.barrier.wait();

      for (int k = 0; k < BLOCK_SIZE; k++)
        if (Col < lenY && m * BLOCK_SIZE + k < lenX) {
          Pvalue += Xds[k] * A[aOffset + A_batchOffset * elt + Col * lenX + m * BLOCK_SIZE + k];
        }

      tidx.barrier.wait();
    }

    if (Col < lenY) {
      long Y_index = yOffset + Y_batchOffset * elt + Col;
      Y[Y_index] = (isnan(Y[Y_index]) || isinf(Y[Y_index])) ? 0 : Y[Y_index];
      Y[Y_index] *= beta;
      Y[Y_index] += alpha * Pvalue;
    }

    tidx.barrier.wait();
  });
}

void gemv_HC(hc::accelerator_view &accl_view,
             char TransA, int M, int N, float alpha,
             hc::array<float> &A, long aOffset,
             hc::array<float> &X, long xOffset, long incX, float beta,
             hc::array<float> &Y, long yOffset, long incY) {
  int lenX, lenY;

  if (TransA == 'n') {
    lenX = N;
    lenY = M;
  } else {
    lenX = M;
    lenY = N;
  }

  if (alpha == 0) {
     std::vector<float> HostY(lenY);
     hc::copy(Y, begin(HostY));
     if (beta == 0) {
       for (int j = 0; j < lenY; ++j) {
           HostY[yOffset + j] = 0;
       }
     } else {
       for (int j = 0; j < lenY; ++j) {
           HostY[yOffset + j] *= beta;
       }
     }
    hc::copy(begin(HostY), end(HostY), Y);
    return;
  }

  if (TransA == 't') {
    gemv_TransA(accl_view, A, aOffset, X, xOffset, Y, yOffset, alpha, beta, lenX, lenY);
  } else if (TransA == 'n') {
    gemv_NoTransA(accl_view, A, aOffset, X, xOffset, Y, yOffset, alpha, beta, lenX, lenY);
  }
}

void gemv_HC(hc::accelerator_view &accl_view,
             char TransA, int M, int N, float alpha,
             hc::array<float> &A, long aOffset, long A_batchOffset,
             hc::array<float> &X, long xOffset, long X_batchOffset,
             long incX, float beta,
             hc::array<float> &Y, long yOffset, long Y_batchOffset,
             long incY, int batchSize) {
  int lenX, lenY;

  if (TransA == 'n') {
    lenX = N;
    lenY = M;
  } else {
    lenX = M;
    lenY = N;
  }
 
  if (alpha == 0) {
     std::vector<float> HostY(lenY * batchSize);
     hc::copy(Y, begin(HostY));
     if (beta == 0) {
      for(int k = 0; k < batchSize; ++k) {
       for (int j = 0; j < lenY; ++j) {
           HostY[yOffset + Y_batchOffset * k + j] = 0;
       }
      }
     } else {
      for(int k = 0; k < batchSize; ++k) {
       for (int j = 0; j < lenY; ++j) {
           HostY[yOffset + Y_batchOffset * k + j] *= beta;
       }
      }
     }
    hc::copy(begin(HostY), end(HostY), Y);
    return;
  }

  if (TransA == 't') {
    gemv_TransA(accl_view, A, aOffset, A_batchOffset, X, xOffset, X_batchOffset, Y, yOffset, Y_batchOffset, alpha, beta, lenX, lenY, batchSize);
  } else if (TransA == 'n') {
    gemv_NoTransA(accl_view, A, aOffset, A_batchOffset, X, xOffset, X_batchOffset, Y, yOffset, Y_batchOffset, alpha, beta, lenX, lenY, batchSize);
  }
}

void gemv_HC_rMajor(hc::accelerator_view &accl_view,
                    char TransA, int M, int N, float alpha,
                    hc::array<float> &A, long aOffset,
                    hc::array<float> &X, long xOffset, long incX, float beta,
                    hc::array<float> &Y, long yOffset, long incY) {
  int lenX, lenY;

  if (TransA == 'n') {
    lenX = N;
    lenY = M;
  } else {
    lenX = M;
    lenY = N;
  }

  if (alpha == 0) {
     std::vector<float> HostY(lenY);
     hc::copy(Y, begin(HostY));
     if (beta == 0) {
       for (int j = 0; j < lenY; ++j) {
           HostY[yOffset + j] = 0;
       }
     } else {
       for (int j = 0; j < lenY; ++j) {
           HostY[yOffset + j] *= beta;
       }
     }
    hc::copy(begin(HostY), end(HostY), Y);
    return;
  }

  if (TransA == 't') {
    gemv_TransA_rMajor(accl_view, A, aOffset, X, xOffset, Y, yOffset, alpha, beta, lenX, lenY);
  } else if (TransA == 'n') {
    gemv_NoTransA_rMajor(accl_view, A, aOffset, X, xOffset, Y, yOffset, alpha, beta, lenX, lenY);
  }
}

void gemv_HC_rMajor(hc::accelerator_view &accl_view,
                    char TransA, int M, int N, float alpha,
                    hc::array<float> &A, long aOffset, long A_batchOffset,
                    hc::array<float> &X, long xOffset, long X_batchOffset,
                    long incX, float beta,
                    hc::array<float> &Y, long yOffset, long Y_batchOffset,
                    long incY, int batchSize) {
  int lenX, lenY;

  if (TransA == 'n') {
    lenX = N;
    lenY = M;
  } else {
    lenX = M;
    lenY = N;
  }

  if (alpha == 0) {
     std::vector<float> HostY(lenY * batchSize);
     hc::copy(Y, begin(HostY));
     if (beta == 0) {
      for(int k = 0; k < batchSize; ++k) {
       for (int j = 0; j < lenY; ++j) {
           HostY[yOffset + Y_batchOffset * k + j] = 0;
       }
      }
     } else {
      for(int k = 0; k < batchSize; ++k) {
       for (int j = 0; j < lenY; ++j) {
           HostY[yOffset + Y_batchOffset * k + j] *= beta;
       }
      }
     }
    hc::copy(begin(HostY), end(HostY), Y);
    return;
  }

  if (TransA == 't') {
    gemv_TransA_rMajor(accl_view, A, aOffset, A_batchOffset, X, xOffset, X_batchOffset, Y, yOffset, Y_batchOffset, alpha, beta, lenX, lenY, batchSize);
  } else if (TransA == 'n') {
    gemv_NoTransA_rMajor(accl_view, A, aOffset, A_batchOffset, X, xOffset, X_batchOffset, Y, yOffset, Y_batchOffset, alpha, beta, lenX, lenY, batchSize);
  }
}



hcblasStatus Hcblaslibrary :: hcblas_sgemv(hcblasOrder order, hcblasTranspose type,
				           const int M, const int N,
				           const float* alpha, float* A, const long aOffset,
				           const int lda, float* X, const long xOffset,
				           const int incX, const float* beta,
				           float* Y, const long yOffset, const int incY) {
  if(alpha == NULL || X == NULL || Y == NULL || A == NULL || M <= 0 || N <= 0 || beta == NULL || incX <= 0 || incY <= 0 ) {
    return HCBLAS_INVALID;
  }

  long lenXn = 1 + (N - 1) * abs(incX);
  long lenXt = 1 + (M - 1) * abs(incX);
  long lenYn = 1 + (M - 1) * abs(incY);
  long lenYt = 1 + (N - 1) * abs(incY);
  hc::array<float> aMat(M * N, A);
  std::vector<hc::accelerator>acc = hc::accelerator::get_all();
  accelerator_view accl_view = (acc[1].create_view());
  std::vector<float> HostA(M * N);

  for( int i = 0; i < M * N; i++) {
    HostA[i] = A[i];
  }

  hc::copy(begin(HostA), end(HostA), aMat);

  if( type == 'n') {
    hc::array<float> xView(lenXn, X);
    hc::array<float> yView(lenYn, Y);
    std::vector<float> HostX(lenXn);
    std::vector<float> HostY(lenYn);

    for( int i = 0; i < lenXn; i++) {
      HostX[i] = X[i];
    }

    for( int i = 0; i < lenYn; i++) {
      HostY[i] = Y[i];
    }

    if (*alpha == 0) {
    if (*beta == 0) {
      for (int j = 0; j < lenYn; ++j) {
          HostY[yOffset + j] = 0;
      }
    } else {
      for (int j = 0; j < lenYn; ++j) {
          HostY[yOffset + j] *=  (*beta);
      }
    }
    for( int i = 0; i < lenYn; i++) {
      Y[i] = HostY[i];
    }
    return HCBLAS_SUCCESS;
 }


    hc::copy(begin(HostX), end(HostX), xView);
    hc::copy(begin(HostY), end(HostY), yView);

    if(order) {
      gemv_HC(accl_view, type, M, N, *alpha, aMat, aOffset, xView, xOffset, incX, *beta, yView, yOffset, incY);
    } else {
      gemv_HC_rMajor(accl_view, type, M, N, *alpha, aMat, aOffset, xView, xOffset, incX, *beta, yView, yOffset, incY);
    }

    hc::copy(yView, begin(HostY));

    for( int i = 0; i < lenYn; i++) {
      Y[i] = HostY[i];
    }
  }

  if( type == 't') {
    hc::array<float> xView(lenXt, X);
    hc::array<float> yView(lenYt, Y);
    std::vector<float> HostX(lenXt);
    std::vector<float> HostY(lenYt);

    for( int i = 0; i < lenXt; i++) {
      HostX[i] = X[i];
    }

    for( int i = 0; i < lenYt; i++) {
      HostY[i] = Y[i];
    }
    
    if (*alpha == 0) {
     if (*beta == 0) {
       for (int j = 0; j < lenYt; ++j) {
           HostY[yOffset + j] = 0;
       }
     } else {
       for (int j = 0; j < lenYt; ++j) {
           HostY[yOffset + j] *=  (*beta);
       }
     }
     for( int i = 0; i < lenYn; i++) {
       Y[i] = HostY[i];
     }
    return HCBLAS_SUCCESS;
    }
    hc::copy(begin(HostX), end(HostX), xView);
    hc::copy(begin(HostY), end(HostY), yView);

    if(order) {
      gemv_HC(accl_view, type, M, N, *alpha, aMat, aOffset, xView, xOffset, incX, *beta, yView, yOffset, incY);
    } else {
      gemv_HC_rMajor(accl_view, type, M, N, *alpha, aMat, aOffset, xView, xOffset, incX, *beta, yView, yOffset, incY);
    }

    hc::copy(yView, begin(HostY));

    for( int i = 0; i < lenYt; i++) {
      Y[i] = HostY[i];
    }
  }

  return HCBLAS_SUCCESS;
}


hcblasStatus Hcblaslibrary :: hcblas_sgemv(hc::accelerator_view &accl_view,
				           hcblasOrder order, hcblasTranspose type, const int M,
				           const int N, const float &alpha,
				           hc::array<float> &A, const long aOffset, const int lda,
				           hc::array<float> &X, const long xOffset, const int incX,
				           const float &beta,
				           hc::array<float> &Y, const long yOffset, const int incY) {
  /*Check the conditions*/
  if( M <= 0 || N <= 0 || incX <= 0 || incY <= 0 ) {
    return HCBLAS_INVALID;
  }

  if(order) {
    gemv_HC(accl_view, type, M, N, alpha, A, aOffset, X, xOffset, incX, beta, Y, yOffset, incY);
  } else {
    gemv_HC_rMajor(accl_view, type, M, N, alpha, A, aOffset, X, xOffset, incX, beta, Y, yOffset, incY);
  }

  return HCBLAS_SUCCESS;
}

hcblasStatus Hcblaslibrary :: hcblas_sgemv(hc::accelerator_view &accl_view,
				           hcblasOrder order, hcblasTranspose type, const int M,
				           const int N, const float &alpha, hc::array<float> &A,
				           const long aOffset, const long A_batchOffset, const int lda,
				           hc::array<float> &X,
				           const long xOffset, const long X_batchOffset, const int incX,
				           const float &beta, hc::array<float> &Y,
				           const long yOffset, const long Y_batchOffset, const int incY, const int batchSize) {
  /*Check the conditions*/
  if( M <= 0 || N <= 0 || incX <= 0 || incY <= 0 ) {
    return HCBLAS_INVALID;
  }

  if(order) {
    gemv_HC(accl_view, type, M, N, alpha, A, aOffset, A_batchOffset, X, xOffset, X_batchOffset, incX, beta, Y, yOffset, Y_batchOffset, incY, batchSize);
  } else {
    gemv_HC_rMajor(accl_view, type, M, N, alpha, A, aOffset, A_batchOffset, X, xOffset, X_batchOffset, incX, beta, Y, yOffset, Y_batchOffset, incY, batchSize);
  }

  return HCBLAS_SUCCESS;
}


