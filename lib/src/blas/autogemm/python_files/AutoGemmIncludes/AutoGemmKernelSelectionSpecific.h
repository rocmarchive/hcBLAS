/*******************************************************************************
 * This file was auto-generated using the AutoGemm.py python script.
 * DO NOT modify this file! Instead, make changes to scripts in
 *   hcblas/lib/src/blas/autoGemm/ then re-generate files
 *   (otherwise local changes will be lost after re-generation).
 ******************************************************************************/

#include <hc.hpp>
#include "hcblaslib.h"
#include "AutoGemmIncludes/AutoGemmKernelSources.h"

// kernel selection specific template
template<typename Precision>
bool gemmSelectKernelSpecific(
  hcblasOrder order,
  hcblasTranspose transA,
  hcblasTranspose transB,
  bool betaNonZero,
  unsigned int macroTileNumRows,
  unsigned int macroTileNumCols,
  unsigned int unroll,
  const char **tileKernelSource,
  const char **rowKernelSource,
  const char **colKernelSource,
  const char **cornerKernelSource,
  unsigned int *workGroupNumRows,
  unsigned int *workGroupNumCols,
  unsigned int *microTileNumRows,
  unsigned int *microTileNumCols
);


