#ifndef KERNEL_SELECTION_SRC
#define KERNEL_SELECTION_SRC

#include "autogemm.h"

/* SelectMicrotileLogic(): Determing the Microtile logic to be called for
 *                         particular size
 *
 *          **** Works only for sizes M*N >= 3072*3072 ****
 *
 * Format: {Size, fallback tile, valid tiles}
 *
 * "s":[
 *  [ 3072, [ 16, 16, 6, 6], [ [ 16, 16, 6, 6], [ 16, 16, 5, 5], [ 16, 16, 4, 4] ] ],
 *  [ 2240, [ 16, 16, 6, 6], [ [ 16, 16, 6, 6], [ 16, 16, 4, 4], [ 16, 16, 5, 5], [ 16, 16, 3, 3] ] ],
 *  [ 1760, [ 16, 16, 4, 4], [ [ 16, 16, 6, 6], [ 16, 16, 4, 4], [ 16, 16, 5, 5], [ 16, 16, 3, 3] ] ],
 *  [ 1600, [ 16, 16, 4, 4], [ [ 16, 16, 4, 4], [ 16, 16, 6, 6], [ 16, 16, 5, 5], [ 16, 16, 3, 3] ] ],
 *  [ 1056, [ 16, 16, 4, 4], [ [ 16, 16, 4, 4], [ 16, 16, 6, 6], [ 16, 16, 5, 5], [ 16, 16, 3, 3], [ 16, 16, 2, 2] ] ],
 *  [  960, [ 16, 16, 4, 4], [ [ 16, 16, 4, 4], [ 16, 16, 5, 5], [ 16, 16, 3, 3], [ 16, 16, 2, 2] ] ],
 *  [  736, [ 16, 16, 3, 3], [ [ 16, 16, 4, 4], [ 16, 16, 3, 3], [ 16, 16, 5, 5], [ 16, 16, 2, 2] ] ],
 *  [  528, [ 16, 16, 3, 3], [ [ 16, 16, 4, 4], [ 16, 16, 3, 3], [ 16, 16, 2, 2], [ 16, 16, 1, 1] ] ],
 *  [  432, [ 16, 16, 2, 2], [ [ 16, 16, 3, 3], [ 16, 16, 2, 2], [ 16, 16, 1, 1] ] ],
 *  [  320, [ 16, 16, 2, 2], [ [ 16, 16, 2, 2], [ 16, 16, 1, 1] ] ],
 *  [    1, [ 16, 16, 1, 1], [ [ 16, 16, 1, 1] ] ],
 *
 */
int AutogemmKernel::selectMicrotileLogic(AutogemmKernel* gemmKernel,
                                         hcblasOrder order,
                                         hcblasTranspose typeA,
                                         hcblasTranspose typeB,
                                         uint M, uint N, uint K,
                                         float beta) {

   gemmKernel->isColMajor = (order == ColMajor) ? 1 : 0  ;
   gemmKernel->isTransA = (typeA == NoTrans) ? 0 : 1;
   gemmKernel->isTransB = (typeB == NoTrans) ? 0 : 1;
   gemmKernel->isBeta_nonZero = (beta == 0) ? 0 : 1;
   gemmKernel->isMinibatch = 0;

  if (M*N >= 3072*3072) {

    // Valid Tiles
    // Call minibatch logic for specific size
    // TODO : Tune the parameters based on the results.
     if (M%128 == 0 && N%128 == 0 && K%128 == 0) {
      if ( M > 6700) {
        gemmKernel->tileNumRows = 16;
        gemmKernel->tileNumCols = 16;
        gemmKernel->microtileNumRows = 6;
        gemmKernel->microtileNumCols = 6;
        gemmKernel->macrotileNumRows = 96;
        gemmKernel->macrotileNumCols = 96;
        gemmKernel->unroll = 16;
        gemmKernel->isMinibatch = 1;
        return SUCCESS;
     } else if (M <= 6700){
        gemmKernel->tileNumRows = 16;
        gemmKernel->tileNumCols = 16;
        gemmKernel->microtileNumRows = 4;
        gemmKernel->microtileNumCols = 4;
        gemmKernel->macrotileNumRows = 64;
        gemmKernel->macrotileNumCols = 64;
        gemmKernel->unroll = 16;
        gemmKernel->isMinibatch = 1;
        return SUCCESS;
     } else if (M <= 2560) {
        gemmKernel->tileNumRows = 16;
        gemmKernel->tileNumCols = 16;
        gemmKernel->microtileNumRows = 2;
        gemmKernel->microtileNumCols = 2;
        gemmKernel->macrotileNumRows = 32;
        gemmKernel->macrotileNumCols = 32;
        gemmKernel->unroll = 16;
        gemmKernel->isMinibatch = 1;
        return SUCCESS;
     }
    }
    if (M%96 == 0 && N%96 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 16;
      return SUCCESS;
    }
    if (M%96 == 0 && N%96 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 8;
      return SUCCESS;
    }
    if (M%96 == 0 && N%96 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 1;
      return SUCCESS;
    }
    if (M%80 == 0 && N%80 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 16;
      return SUCCESS;
   }
    if (M%80 == 0 && N%80 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }

    // Fallback Tile
    if (K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
  }
  if (M*N >= 2240*2240) {

    // Valid Tiles
      if (M%128 == 0 && N%128 == 0 && K%128 == 0) {
      if ( M > 6700) {
        gemmKernel->tileNumRows = 16;
        gemmKernel->tileNumCols = 16;
        gemmKernel->microtileNumRows = 6;
        gemmKernel->microtileNumCols = 6;
        gemmKernel->macrotileNumRows = 96;
        gemmKernel->macrotileNumCols = 96;
        gemmKernel->unroll = 16;
        gemmKernel->isMinibatch = 1;
        return SUCCESS;
     } else if (M <= 6700){
        gemmKernel->tileNumRows = 16;
        gemmKernel->tileNumCols = 16;
        gemmKernel->microtileNumRows = 4;
        gemmKernel->microtileNumCols = 4;
        gemmKernel->macrotileNumRows = 64;
        gemmKernel->macrotileNumCols = 64;
        gemmKernel->unroll = 16;
        gemmKernel->isMinibatch = 1;
        return SUCCESS;
     } else if (M <= 2560) {
        gemmKernel->tileNumRows = 16;
        gemmKernel->tileNumCols = 16;
        gemmKernel->microtileNumRows = 2;
        gemmKernel->microtileNumCols = 2;
        gemmKernel->macrotileNumRows = 32;
        gemmKernel->macrotileNumCols = 32;
        gemmKernel->unroll = 16;
        gemmKernel->isMinibatch = 1;
        return SUCCESS;
     }
    }
    if (M%96 == 0 && N%96 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%96 == 0 && N%96 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%96 == 0 && N%96 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }

    // Fallback Tile
    if (K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
  }
  if (M*N >= 1760*1760) {

     // Valid Tiles
      if (M%128 == 0 && N%128 == 0 && K%128 == 0) {
        gemmKernel->tileNumRows = 16;
        gemmKernel->tileNumCols = 16;
        gemmKernel->microtileNumRows = 2;
        gemmKernel->microtileNumCols = 2;
        gemmKernel->macrotileNumRows = 32;
        gemmKernel->macrotileNumCols = 32;
        gemmKernel->unroll = 16;
        gemmKernel->isMinibatch = 1;
        return SUCCESS;
    }
    if (M%96 == 0 && N%96 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%96 == 0 && N%96 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%96 == 0 && N%96 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }

    // Fallback Tile
    if (K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
  }
  if (M*N >= 1600*1600) {

      // Valid Tiles
    if (M%64 == 0 && N%64 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%96 == 0 && N%96 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%96 == 0 && N%96 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%96 == 0 && N%96 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }

    // Fallback Tile
    if (K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
  }
  if (M*N >= 1056*1056) {

    // Valid Tiles
    if (M%64 == 0 && N%64 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%96 == 0 && N%96 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%96 == 0 && N%96 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%96 == 0 && N%96 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 6;
      gemmKernel->microtileNumCols = 6;
      gemmKernel->macrotileNumRows = 96;
      gemmKernel->macrotileNumCols = 96;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }

    // Fallback Tile
    if (K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
  }
  if (M*N >= 960*960) {

    // Valid Tiles
    if (M%64 == 0 && N%64 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }

    // Fallback Tile
    if (K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
  }
  if (M*N >= 736*736) {

    // Valid Tiles
    if (M%64 == 0 && N%64 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%80 == 0 && N%80 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 5;
      gemmKernel->microtileNumCols = 5;
      gemmKernel->macrotileNumRows = 80;
      gemmKernel->macrotileNumCols = 80;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }

    // Fallback Tile
    if (K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
  }
  if (M*N >= 528*528) {

    // Valid Tiles
    if (M%64 == 0 && N%64 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%64 == 0 && N%64 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 4;
      gemmKernel->microtileNumCols = 4;
      gemmKernel->macrotileNumRows = 64;
      gemmKernel->macrotileNumCols = 64;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%16 == 0 && N%16 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%16 == 0 && N%16 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%16 == 0 && N%16 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }

    // Fallback Tile
    if (K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
  }
  if (M*N >= 432*432) {
    // Valid Tiles
    if (M%48 == 0 && N%48 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%48 == 0 && N%48 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 3;
      gemmKernel->microtileNumCols = 3;
      gemmKernel->macrotileNumRows = 48;
      gemmKernel->macrotileNumCols = 48;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%16 == 0 && N%16 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%16 == 0 && N%16 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%16 == 0 && N%16 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }

    // Fallback Tile
    if (K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
  }
  if (M*N >= 320*320) {
    // Valid Tiles
    if (M%32 == 0 && N%32 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%32 == 0 && N%32 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
    if (M%16 == 0 && N%16 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%16 == 0 && N%16 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%16 == 0 && N%16 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }

    // Fallback Tile
    if (K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 2;
      gemmKernel->microtileNumCols = 2;
      gemmKernel->macrotileNumRows = 32;
      gemmKernel->macrotileNumCols = 32;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
  }
  if (M*N >= 1*1) {
    // Valid Tiles
    if (M%16 == 0 && N%16 == 0 && K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (M%16 == 0 && N%16 == 0 && K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (M%16 == 0 && N%16 == 0 && K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }

    // Fallback Tile
    if (K%16 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 16;
      return SUCCESS;

    }
    if (K%8 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 8;
      return SUCCESS;

    }
    if (K%1 == 0) {
      gemmKernel->tileNumRows = 16;
      gemmKernel->tileNumCols = 16;
      gemmKernel->microtileNumRows = 1;
      gemmKernel->microtileNumCols = 1;
      gemmKernel->macrotileNumRows = 16;
      gemmKernel->macrotileNumCols = 16;
      gemmKernel->unroll = 1;
      return SUCCESS;

    }
  }
  return SUCCESS;
}

#endif //KERNEL_SELECTION_SRC
