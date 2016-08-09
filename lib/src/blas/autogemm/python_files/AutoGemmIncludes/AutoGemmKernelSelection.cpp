/*******************************************************************************
 * This file was auto-generated using the AutoGemm.py python script.
 * DO NOT modify this file! Instead, make changes to scripts in
 *   hcblas/lib/src/blas/autoGemm/ then re-generate files
 *   (otherwise local changes will be lost after re-generation).
 ******************************************************************************/

#include "AutoGemmIncludes/AutoGemmKernelSelection.h"

// sgemm kernel selection logic
template<>
void gemmSelectKernel<float>(
  hcblasOrder order,
  hcblasTranspose transA,
  hcblasTranspose transB,
  size_t M,
  size_t N,
  size_t K,
  bool betaNonZero,
  float optimalNumElementsPerWorkItem,
  const char **tileKernelSource,
  const char **rowKernelSource,
  const char **colKernelSource,
  const char **cornerKernelSource,
  unsigned int *workGroupNumRows,
  unsigned int *workGroupNumCols,
  unsigned int *microTileNumRows,
  unsigned int *microTileNumCols,
  unsigned int *unroll
) {
  if (order == ColMajor) {
    if (transA == NoTrans) {
      if (transB == NoTrans) {
        if ( !betaNonZero ) {
          if ( M*N >= 3072*3072) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1760*1760) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1600*1600) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 960*960) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 736*736) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 432*432) {
            // valid tiles
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 320*320) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 3072*3072) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1760*1760) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1600*1600) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 960*960) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 736*736) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 432*432) {
            // valid tiles
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 320*320) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == Trans) {
        if ( !betaNonZero ) {
          if ( M*N >= 3072*3072) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1760*1760) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1600*1600) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 960*960) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 736*736) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 432*432) {
            // valid tiles
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 320*320) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 3072*3072) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1760*1760) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1600*1600) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 960*960) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 736*736) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 432*432) {
            // valid tiles
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 320*320) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
    } // end transA
    if (transA == Trans) {
      if (transB == NoTrans) {
        if ( !betaNonZero ) {
          if ( M*N >= 3072*3072) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1760*1760) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1600*1600) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 960*960) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 736*736) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 432*432) {
            // valid tiles
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 320*320) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 3072*3072) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1760*1760) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1600*1600) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 960*960) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 736*736) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 432*432) {
            // valid tiles
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 320*320) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == Trans) {
        if ( !betaNonZero ) {
          if ( M*N >= 3072*3072) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1760*1760) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1600*1600) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 960*960) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 736*736) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 432*432) {
            // valid tiles
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 320*320) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 3072*3072) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1760*1760) {
            // valid tiles
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1600*1600) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%16 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 960*960) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 736*736) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%16 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%16 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 432*432) {
            // valid tiles
            if ( M%48 == 0 && N%48 == 0 && K%16 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 320*320) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%16 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%16 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%16 == 0 ) {
              return;
            }
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
    } // end transA
  } // end order
} // end precision function

// dgemm kernel selection logic
template<>
void gemmSelectKernel<double>(
  hcblasOrder order,
  hcblasTranspose transA,
  hcblasTranspose transB,
  size_t M,
  size_t N,
  size_t K,
  bool betaNonZero,
  float optimalNumElementsPerWorkItem,
  const char **tileKernelSource,
  const char **rowKernelSource,
  const char **colKernelSource,
  const char **cornerKernelSource,
  unsigned int *workGroupNumRows,
  unsigned int *workGroupNumCols,
  unsigned int *microTileNumRows,
  unsigned int *microTileNumCols,
  unsigned int *unroll
) {
  if (order == ColMajor) {
    if (transA == NoTrans) {
      if (transB == NoTrans) {
        if ( !betaNonZero ) {
          if ( M*N >= 3200*3200) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1632*1632) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1280*1280) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 672*672) {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 3200*3200) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1632*1632) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1280*1280) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 672*672) {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == Trans) {
        if ( !betaNonZero ) {
          if ( M*N >= 3200*3200) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1632*1632) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1280*1280) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 672*672) {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 3200*3200) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1632*1632) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1280*1280) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 672*672) {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
    } // end transA
    if (transA == Trans) {
      if (transB == NoTrans) {
        if ( !betaNonZero ) {
          if ( M*N >= 3200*3200) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1632*1632) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1280*1280) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 672*672) {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 3200*3200) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1632*1632) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1280*1280) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 672*672) {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == Trans) {
        if ( !betaNonZero ) {
          if ( M*N >= 3200*3200) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1632*1632) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1280*1280) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 672*672) {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 3200*3200) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1632*1632) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1280*1280) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1056*1056) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 672*672) {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
    } // end transA
  } // end order
} // end precision function

// cgemm kernel selection logic
template<>
void gemmSelectKernel<float2>(
  hcblasOrder order,
  hcblasTranspose transA,
  hcblasTranspose transB,
  size_t M,
  size_t N,
  size_t K,
  bool betaNonZero,
  float optimalNumElementsPerWorkItem,
  const char **tileKernelSource,
  const char **rowKernelSource,
  const char **colKernelSource,
  const char **cornerKernelSource,
  unsigned int *workGroupNumRows,
  unsigned int *workGroupNumCols,
  unsigned int *microTileNumRows,
  unsigned int *microTileNumCols,
  unsigned int *unroll
) {
  if (order == ColMajor) {
    if (transA == NoTrans) {
      if (transB == NoTrans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == Trans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == 0) {
        if ( !betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
    } // end transA
    if (transA == Trans) {
      if (transB == NoTrans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == Trans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == 0) {
        if ( !betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
    } // end transA
    if (transA == 0) {
      if (transB == NoTrans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == Trans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == 0) {
        if ( !betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2240*2240) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1440*1440) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%8 == 0) {
              return;
            }
            if ( M%96 == 0 && N%96 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1088*1088) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 704*704) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%8 == 0) {
              return;
            }
            if ( M%80 == 0 && N%80 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 528*528) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 336*336) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
    } // end transA
  } // end order
} // end precision function

// zgemm kernel selection logic
template<>
void gemmSelectKernel<double2>(
  hcblasOrder order,
  hcblasTranspose transA,
  hcblasTranspose transB,
  size_t M,
  size_t N,
  size_t K,
  bool betaNonZero,
  float optimalNumElementsPerWorkItem,
  const char **tileKernelSource,
  const char **rowKernelSource,
  const char **colKernelSource,
  const char **cornerKernelSource,
  unsigned int *workGroupNumRows,
  unsigned int *workGroupNumCols,
  unsigned int *microTileNumRows,
  unsigned int *microTileNumCols,
  unsigned int *unroll
) {
  if (order == ColMajor) {
    if (transA == NoTrans) {
      if (transB == NoTrans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == Trans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == 0) {
        if ( !betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
    } // end transA
    if (transA == Trans) {
      if (transB == NoTrans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == Trans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == 0) {
        if ( !betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
    } // end transA
    if (transA == 0) {
      if (transB == NoTrans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == Trans) {
        if ( !betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
      if (transB == 0) {
        if ( !betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
        if ( betaNonZero ) {
          if ( M*N >= 2528*2528) {
            // valid tiles
            if ( M%64 == 0 && N%64 == 0 && K%8 == 0) {
              return;
            }
            if ( M%64 == 0 && N%64 == 0 && K%1 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1872*1872) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%8 == 0) {
              return;
            }
            if ( M%48 == 0 && N%48 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 1040*1040) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          if ( M*N >= 768*768) {
            // valid tiles
            if ( M%32 == 0 && N%32 == 0 && K%8 == 0) {
              return;
            }
            if ( M%32 == 0 && N%32 == 0 && K%1 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
          {
            // valid tiles
            if ( M%16 == 0 && N%16 == 0 && K%8 == 0) {
              return;
            }
            if ( M%16 == 0 && N%16 == 0 && K%1 == 0) {
              return;
            }
            // fallback tile
            if ( K%8 == 0 ) {
              return;
            }
            if ( K%1 == 0 ) {
              return;
            }
          } // end size
        } // end beta
      } // end transB
    } // end transA
  } // end order
} // end precision function

