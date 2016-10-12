#include "hc.hpp"
#include "hc_math.hpp"
#include "hcblaslib.h"
#include <cmath>
#include <iostream>
using namespace std;

/* data types */
#define uint unsigned int
#define DATA_TYPE_STR float

#define mad(a, b, c) a * b + c

#define  M4x4                            \
             rA[0][0] = lA[offA + 0];          \
             rA[0][1] = lA[offA + 16];         \
             rA[0][2] = lA[offA + 32];         \
             rA[0][3] = lA[offA + 48];         \
             rB[0][0] = lB[offB + 0];          \
             rB[0][1] = lB[offB + 16];         \
             rB[0][2] = lB[offB + 32];         \
             rB[0][3] = lB[offB + 48];         \
             offA += 65;                       \
             offB += 65;  \
            rC[0][0]=mad(rA[0][0],rB[0][0],rC[0][0]); \
            rC[1][0]=mad(rA[0][1],rB[0][0],rC[1][0]); \
            rC[2][0]=mad(rA[0][2],rB[0][0],rC[2][0]); \
            rC[3][0]=mad(rA[0][3],rB[0][0],rC[3][0]); \
            rC[0][1]=mad(rA[0][0],rB[0][1],rC[0][1]); \
            rC[1][1]=mad(rA[0][1],rB[0][1],rC[1][1]); \
            rC[2][1]=mad(rA[0][2],rB[0][1],rC[2][1]); \
            rC[3][1]=mad(rA[0][3],rB[0][1],rC[3][1]); \
            rC[0][2]=mad(rA[0][0],rB[0][2],rC[0][2]); \
            rC[1][2]=mad(rA[0][1],rB[0][2],rC[1][2]); \
            rC[2][2]=mad(rA[0][2],rB[0][2],rC[2][2]); \
            rC[3][2]=mad(rA[0][3],rB[0][2],rC[3][2]); \
            rC[0][3]=mad(rA[0][0],rB[0][3],rC[0][3]); \
            rC[1][3]=mad(rA[0][1],rB[0][3],rC[1][3]); \
            rC[2][3]=mad(rA[0][2],rB[0][3],rC[2][3]); \
            rC[3][3]=mad(rA[0][3],rB[0][3],rC[3][3]); \

extern "C" {
hcblasStatus sgemm_Col_NN_B1_M_N_K(
  hc::accelerator_view accl_view,
  DATA_TYPE_STR const * A,
  DATA_TYPE_STR const * B,
  DATA_TYPE_STR * C,
  DATA_TYPE_STR const alpha,
  DATA_TYPE_STR const beta,
  uint const M,
  uint const N,
  uint const K,
  uint const lda,
  uint const ldb,
  uint const ldc,
  uint const offsetA,
  uint const offsetB,
  uint const offsetC
) {

  int aOffset = offsetA;
  int bOffset = offsetB;
  int cOffset = offsetC;

  int M_ = (M-1)/4 + 1;
  int N_ = (N-1)/4 + 1;
  int N_R = (N_ + 15) & ~15;
  int M_R = (M_ + 15) & ~15;
  int K_R = (K + 15) & ~15;
  hc::extent<2> grdExt(N_R, M_R);
  hc::tiled_extent<2> t_ext = grdExt.tile(16, 16);
  hc::parallel_for_each(accl_view, t_ext, [ = ] (hc::tiled_index<2> tidx) __attribute__((hc, cpu)) {
    float rC[4][4] = {{(float)0}};
    float rA[1][4];
    float rB[1][4];
    tile_static float lA[1056];
    tile_static float lB[1056];
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int block_k = 0;
    int alIndex = (idy * 65) + idx;
    int blIndex = (idx * 65) + idy;
    int AIndex = aOffset + (gidx * 64) + idx + (idy * lda);
    int BIndex = bOffset + ((gidy * 64) + idy)*ldb + idx;
    long CIndex = cOffset + (gidx * 64) + idx + (((gidy * 64) + idy) * ldc);
    long AinitOffset = 0;
    long BinitOffset = 0;
    long CinitOffset = 0;
    int N_block = N_R >> 4;
    int M_block = M_R >> 4;
    int K_block = K_R >> 4;
    do {

      tidx.barrier.wait();

      if(gidx == M_block-1 || gidy == N_block-1 || block_k == K_block-1)
      {
          for(int sec = 0; sec < 4; ++sec) {
            int secVal = sec << 4;

            if( (gidy*64 + idy + secVal) < N && (block_k * 16 + idx) < K) {
              lB[ blIndex + secVal] = B[BIndex + BinitOffset + secVal * ldb];
            } else {
              lB[blIndex + secVal] = 0;
            }

            if( (gidx*64 + idx + secVal) < M && (block_k * 16 + idy) < K) {
              lA[ alIndex + secVal] = A[AIndex + secVal + AinitOffset];
            } else {
              lA[ alIndex + secVal] = 0;
            }
          }
      }
      else
      {
          lB[blIndex] = B[BIndex + BinitOffset];
          lB[blIndex + 16] = B[BIndex + BinitOffset +  16 * ldb];
          lB[blIndex + 32] = B[BIndex + BinitOffset +  32 * ldb];
          lB[blIndex + 48] = B[BIndex + BinitOffset +  48 * ldb];
          lA[alIndex] = A[AIndex + AinitOffset ];
          lA[alIndex + 16] = A[AIndex + 16 + AinitOffset];
          lA[alIndex + 32] = A[AIndex + 32 + AinitOffset];
          lA[alIndex + 48] = A[AIndex + 48 + AinitOffset];
      }

      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;

      for (int iter = 0; iter < 16; iter+=8) {
        M4x4;
        M4x4;
        M4x4;
        M4x4;
        M4x4;
        M4x4;
        M4x4;
        M4x4;
      }

      AinitOffset += lda << 4;
      BinitOffset += 16;

    } while (++block_k < (K_R >> 4));

      tidx.barrier.wait();
    if(gidx == M_block-1 || gidy == N_block-1)
    {
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 0 * 16) < N)
        C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[0][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 1 * 16) < N)
        C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[0][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 2 * 16) < N)
        C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[0][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 3 * 16) < N)
        C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[0][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
        CinitOffset+=16;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 0 * 16) < N)
        C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[1][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 1 * 16) < N)
        C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[1][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 2 * 16) < N)
        C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[1][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 3 * 16) < N)
        C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[1][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
        CinitOffset+=16;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 0 * 16) < N)
        C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[2][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 1 * 16) < N)
        C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[2][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 2 * 16) < N)
        C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[2][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 3 * 16) < N)
        C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[2][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
        CinitOffset+=16;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 0 * 16) < N)
        C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[3][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 1 * 16) < N)
        C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[3][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 2 * 16) < N)
        C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[3][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
        if((gidx * 64 + idx + CinitOffset)  < M && (gidy * 64 + idy + 3 * 16) < N)
        C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[3][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    }
    else
    {
        C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[0][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
        C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[0][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
        C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[0][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
        C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[0][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
        CinitOffset+=16;
        C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[1][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
        C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[1][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
        C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[1][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
        C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[1][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
        CinitOffset+=16;
        C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[2][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
        C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[2][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
        C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[2][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
        C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[2][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
        CinitOffset+=16;
        C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[3][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
        C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[3][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
        C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[3][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
        C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[3][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    }

  }).wait();
  return HCBLAS_SUCCEEDS;
}

}
