/*******************************************************************************
 * This file was auto-generated using the AutoGemm.py python script.
 * DO NOT modify this file! Instead, make changes to scripts in
 *   hcblas/lib/src/blas/autoGemm/ then re-generate files
 *   (otherwise local changes will be lost after re-generation).
 ******************************************************************************/

#include <hc.hpp>
#include "hcblaslib.h"
#include "AutoGemmIncludes/AutoGemmKernelSources.h"

#define EXACT_MULTIPLES(MULTIPLE_STR) MULTIPLE_STR

// kernel selection logic template
template<typename Precision>
void gemmSelectKernel(
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
);

