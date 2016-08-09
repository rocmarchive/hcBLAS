/*******************************************************************************
 * This file was auto-generated using the AutoGemm.py python script.
 * DO NOT modify this file! Instead, make changes to scripts in
 *   hcblas/lib/src/blas/autoGemm/ then re-generate files
 *   (otherwise local changes will be lost after re-generation).
 ******************************************************************************/

#include "AutoGemmIncludes/AutoGemmKernelSelectionSpecific.h"
  
// sgemm kernel selection specific
template<>
bool gemmSelectKernelSpecific<float>(
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
) {
  if (order == ColMajor) {
    if (transA == NoTrans && transB == NoTrans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX016_NX016_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML016_NX016_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX016_NL016_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML016_NL016_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX016_NX016_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX016_NX016_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX016_NX016_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX016_NX016_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX032_NX032_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML032_NX032_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX032_NL032_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML032_NL032_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX032_NX032_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX032_NX032_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX032_NX032_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX032_NX032_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX048_NX048_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML048_NX048_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX048_NL048_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML048_NL048_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX048_NX048_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX048_NX048_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX048_NX048_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX048_NX048_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX064_NX064_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML064_NX064_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX064_NL064_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML064_NL064_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX064_NX064_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX064_NX064_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX064_NX064_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX064_NX064_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX080_NX080_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML080_NX080_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX080_NL080_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML080_NL080_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX080_NX080_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX080_NX080_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX080_NX080_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX080_NX080_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B0_MX096_NX096_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B0_ML096_NX096_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B0_MX096_NL096_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B0_ML096_NL096_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B0_MX096_NX096_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B0_MX096_NX096_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B0_MX096_NX096_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B0_MX096_NX096_KX16_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX016_NX016_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML016_NX016_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX016_NL016_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML016_NL016_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX016_NX016_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX016_NX016_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX016_NX016_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX016_NX016_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX032_NX032_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML032_NX032_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX032_NL032_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML032_NL032_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX032_NX032_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX032_NX032_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX032_NX032_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX032_NX032_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX048_NX048_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML048_NX048_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX048_NL048_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML048_NL048_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX048_NX048_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX048_NX048_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX048_NX048_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX048_NX048_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX064_NX064_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML064_NX064_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX064_NL064_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML064_NL064_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX064_NX064_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX064_NX064_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX064_NX064_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX064_NX064_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX080_NX080_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML080_NX080_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX080_NL080_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML080_NL080_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX080_NX080_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX080_NX080_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX080_NX080_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX080_NX080_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NN_B1_MX096_NX096_KX16_src;
          *rowKernelSource    =  sgemm_Col_NN_B1_ML096_NX096_KX16_src;
          *colKernelSource    =  sgemm_Col_NN_B1_MX096_NL096_KX16_src;
          *cornerKernelSource =  sgemm_Col_NN_B1_ML096_NL096_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NN_B1_MX096_NX096_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NN_B1_MX096_NX096_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NN_B1_MX096_NX096_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NN_B1_MX096_NX096_KX16_microTileNumCols;
          return true;
        }
      }
    } else if (transA == NoTrans && transB == Trans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX016_NX016_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML016_NX016_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX016_NL016_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML016_NL016_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX016_NX016_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX016_NX016_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX016_NX016_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX016_NX016_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX032_NX032_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML032_NX032_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX032_NL032_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML032_NL032_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX032_NX032_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX032_NX032_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX032_NX032_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX032_NX032_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX048_NX048_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML048_NX048_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX048_NL048_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML048_NL048_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX048_NX048_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX048_NX048_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX048_NX048_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX048_NX048_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX064_NX064_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML064_NX064_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX064_NL064_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML064_NL064_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX064_NX064_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX064_NX064_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX064_NX064_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX064_NX064_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX080_NX080_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML080_NX080_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX080_NL080_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML080_NL080_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX080_NX080_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX080_NX080_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX080_NX080_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX080_NX080_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B0_MX096_NX096_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B0_ML096_NX096_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B0_MX096_NL096_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B0_ML096_NL096_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B0_MX096_NX096_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B0_MX096_NX096_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B0_MX096_NX096_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B0_MX096_NX096_KX16_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX016_NX016_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML016_NX016_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX016_NL016_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML016_NL016_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX016_NX016_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX016_NX016_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX016_NX016_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX016_NX016_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX032_NX032_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML032_NX032_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX032_NL032_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML032_NL032_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX032_NX032_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX032_NX032_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX032_NX032_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX032_NX032_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX048_NX048_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML048_NX048_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX048_NL048_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML048_NL048_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX048_NX048_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX048_NX048_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX048_NX048_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX048_NX048_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX064_NX064_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML064_NX064_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX064_NL064_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML064_NL064_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX064_NX064_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX064_NX064_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX064_NX064_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX064_NX064_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX080_NX080_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML080_NX080_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX080_NL080_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML080_NL080_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX080_NX080_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX080_NX080_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX080_NX080_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX080_NX080_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_NT_B1_MX096_NX096_KX16_src;
          *rowKernelSource    =  sgemm_Col_NT_B1_ML096_NX096_KX16_src;
          *colKernelSource    =  sgemm_Col_NT_B1_MX096_NL096_KX16_src;
          *cornerKernelSource =  sgemm_Col_NT_B1_ML096_NL096_KX16_src;
          *workGroupNumRows   =  sgemm_Col_NT_B1_MX096_NX096_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_NT_B1_MX096_NX096_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_NT_B1_MX096_NX096_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_NT_B1_MX096_NX096_KX16_microTileNumCols;
          return true;
        }
      }
    } else if (transA == Trans && transB == NoTrans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX016_NX016_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML016_NX016_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX016_NL016_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML016_NL016_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX016_NX016_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX016_NX016_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX016_NX016_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX016_NX016_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX032_NX032_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML032_NX032_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX032_NL032_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML032_NL032_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX032_NX032_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX032_NX032_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX032_NX032_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX032_NX032_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX048_NX048_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML048_NX048_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX048_NL048_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML048_NL048_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX048_NX048_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX048_NX048_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX048_NX048_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX048_NX048_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX064_NX064_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML064_NX064_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX064_NL064_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML064_NL064_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX064_NX064_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX064_NX064_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX064_NX064_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX064_NX064_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX080_NX080_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML080_NX080_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX080_NL080_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML080_NL080_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX080_NX080_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX080_NX080_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX080_NX080_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX080_NX080_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B0_MX096_NX096_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B0_ML096_NX096_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B0_MX096_NL096_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B0_ML096_NL096_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B0_MX096_NX096_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B0_MX096_NX096_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B0_MX096_NX096_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B0_MX096_NX096_KX16_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX016_NX016_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML016_NX016_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX016_NL016_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML016_NL016_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX016_NX016_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX016_NX016_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX016_NX016_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX016_NX016_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX032_NX032_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML032_NX032_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX032_NL032_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML032_NL032_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX032_NX032_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX032_NX032_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX032_NX032_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX032_NX032_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX048_NX048_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML048_NX048_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX048_NL048_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML048_NL048_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX048_NX048_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX048_NX048_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX048_NX048_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX048_NX048_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX064_NX064_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML064_NX064_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX064_NL064_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML064_NL064_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX064_NX064_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX064_NX064_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX064_NX064_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX064_NX064_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX080_NX080_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML080_NX080_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX080_NL080_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML080_NL080_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX080_NX080_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX080_NX080_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX080_NX080_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX080_NX080_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TN_B1_MX096_NX096_KX16_src;
          *rowKernelSource    =  sgemm_Col_TN_B1_ML096_NX096_KX16_src;
          *colKernelSource    =  sgemm_Col_TN_B1_MX096_NL096_KX16_src;
          *cornerKernelSource =  sgemm_Col_TN_B1_ML096_NL096_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TN_B1_MX096_NX096_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TN_B1_MX096_NX096_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TN_B1_MX096_NX096_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TN_B1_MX096_NX096_KX16_microTileNumCols;
          return true;
        }
      }
    } else if (transA == Trans && transB == Trans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX016_NX016_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML016_NX016_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX016_NL016_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML016_NL016_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX016_NX016_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX016_NX016_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX016_NX016_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX016_NX016_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX032_NX032_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML032_NX032_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX032_NL032_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML032_NL032_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX032_NX032_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX032_NX032_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX032_NX032_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX032_NX032_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX048_NX048_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML048_NX048_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX048_NL048_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML048_NL048_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX048_NX048_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX048_NX048_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX048_NX048_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX048_NX048_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX064_NX064_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML064_NX064_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX064_NL064_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML064_NL064_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX064_NX064_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX064_NX064_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX064_NX064_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX064_NX064_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX080_NX080_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML080_NX080_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX080_NL080_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML080_NL080_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX080_NX080_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX080_NX080_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX080_NX080_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX080_NX080_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B0_MX096_NX096_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B0_ML096_NX096_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B0_MX096_NL096_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B0_ML096_NL096_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B0_MX096_NX096_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B0_MX096_NX096_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B0_MX096_NX096_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B0_MX096_NX096_KX16_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX016_NX016_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML016_NX016_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX016_NL016_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML016_NL016_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX016_NX016_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX016_NX016_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX016_NX016_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX016_NX016_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX032_NX032_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML032_NX032_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX032_NL032_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML032_NL032_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX032_NX032_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX032_NX032_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX032_NX032_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX032_NX032_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX048_NX048_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML048_NX048_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX048_NL048_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML048_NL048_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX048_NX048_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX048_NX048_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX048_NX048_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX048_NX048_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX064_NX064_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML064_NX064_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX064_NL064_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML064_NL064_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX064_NX064_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX064_NX064_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX064_NX064_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX064_NX064_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX080_NX080_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML080_NX080_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX080_NL080_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML080_NL080_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX080_NX080_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX080_NX080_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX080_NX080_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX080_NX080_KX16_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 16) {
          *tileKernelSource   =  sgemm_Col_TT_B1_MX096_NX096_KX16_src;
          *rowKernelSource    =  sgemm_Col_TT_B1_ML096_NX096_KX16_src;
          *colKernelSource    =  sgemm_Col_TT_B1_MX096_NL096_KX16_src;
          *cornerKernelSource =  sgemm_Col_TT_B1_ML096_NL096_KX16_src;
          *workGroupNumRows   =  sgemm_Col_TT_B1_MX096_NX096_KX16_workGroupNumRows;
          *workGroupNumCols   =  sgemm_Col_TT_B1_MX096_NX096_KX16_workGroupNumCols;
          *microTileNumRows   =  sgemm_Col_TT_B1_MX096_NX096_KX16_microTileNumRows;
          *microTileNumCols   =  sgemm_Col_TT_B1_MX096_NX096_KX16_microTileNumCols;
          return true;
        }
      }
    }
  }
  return false; // didn't find a match
}


// dgemm kernel selection specific
template<>
bool gemmSelectKernelSpecific<double>(
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
) {
  if (order == ColMajor) {
    if (transA == NoTrans && transB == NoTrans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NN_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  dgemm_Col_NN_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  dgemm_Col_NN_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  dgemm_Col_NN_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NN_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NN_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  dgemm_Col_NN_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  dgemm_Col_NN_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  dgemm_Col_NN_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NN_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NN_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  dgemm_Col_NN_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  dgemm_Col_NN_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  dgemm_Col_NN_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NN_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NN_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  dgemm_Col_NN_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  dgemm_Col_NN_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  dgemm_Col_NN_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NN_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NN_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  dgemm_Col_NN_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  dgemm_Col_NN_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  dgemm_Col_NN_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NN_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NN_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  dgemm_Col_NN_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  dgemm_Col_NN_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  dgemm_Col_NN_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NN_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NN_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  dgemm_Col_NN_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  dgemm_Col_NN_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  dgemm_Col_NN_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NN_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NN_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  dgemm_Col_NN_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  dgemm_Col_NN_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  dgemm_Col_NN_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NN_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NN_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  dgemm_Col_NN_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  dgemm_Col_NN_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  dgemm_Col_NN_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NN_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NN_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  dgemm_Col_NN_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  dgemm_Col_NN_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  dgemm_Col_NN_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NN_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NN_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  dgemm_Col_NN_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  dgemm_Col_NN_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  dgemm_Col_NN_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NN_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NN_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  dgemm_Col_NN_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  dgemm_Col_NN_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  dgemm_Col_NN_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NN_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NN_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  dgemm_Col_NN_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  dgemm_Col_NN_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  dgemm_Col_NN_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NN_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NN_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  dgemm_Col_NN_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  dgemm_Col_NN_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  dgemm_Col_NN_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NN_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NN_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  dgemm_Col_NN_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  dgemm_Col_NN_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  dgemm_Col_NN_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NN_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NN_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  dgemm_Col_NN_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  dgemm_Col_NN_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  dgemm_Col_NN_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NN_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NN_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  dgemm_Col_NN_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  dgemm_Col_NN_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  dgemm_Col_NN_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NN_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NN_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  dgemm_Col_NN_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  dgemm_Col_NN_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  dgemm_Col_NN_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NN_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NN_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  dgemm_Col_NN_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  dgemm_Col_NN_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  dgemm_Col_NN_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NN_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NN_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  dgemm_Col_NN_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  dgemm_Col_NN_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  dgemm_Col_NN_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NN_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NN_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NN_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NN_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == NoTrans && transB == Trans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NT_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  dgemm_Col_NT_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  dgemm_Col_NT_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  dgemm_Col_NT_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NT_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NT_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  dgemm_Col_NT_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  dgemm_Col_NT_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  dgemm_Col_NT_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NT_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NT_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  dgemm_Col_NT_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  dgemm_Col_NT_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  dgemm_Col_NT_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NT_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NT_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  dgemm_Col_NT_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  dgemm_Col_NT_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  dgemm_Col_NT_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NT_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NT_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  dgemm_Col_NT_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  dgemm_Col_NT_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  dgemm_Col_NT_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NT_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NT_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  dgemm_Col_NT_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  dgemm_Col_NT_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  dgemm_Col_NT_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NT_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NT_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  dgemm_Col_NT_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  dgemm_Col_NT_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  dgemm_Col_NT_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NT_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NT_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  dgemm_Col_NT_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  dgemm_Col_NT_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  dgemm_Col_NT_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NT_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NT_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  dgemm_Col_NT_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  dgemm_Col_NT_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  dgemm_Col_NT_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NT_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NT_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  dgemm_Col_NT_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  dgemm_Col_NT_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  dgemm_Col_NT_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NT_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NT_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  dgemm_Col_NT_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  dgemm_Col_NT_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  dgemm_Col_NT_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NT_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NT_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  dgemm_Col_NT_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  dgemm_Col_NT_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  dgemm_Col_NT_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NT_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NT_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  dgemm_Col_NT_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  dgemm_Col_NT_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  dgemm_Col_NT_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NT_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NT_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  dgemm_Col_NT_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  dgemm_Col_NT_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  dgemm_Col_NT_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NT_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NT_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  dgemm_Col_NT_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  dgemm_Col_NT_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  dgemm_Col_NT_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NT_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NT_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  dgemm_Col_NT_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  dgemm_Col_NT_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  dgemm_Col_NT_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NT_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NT_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  dgemm_Col_NT_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  dgemm_Col_NT_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  dgemm_Col_NT_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NT_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NT_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  dgemm_Col_NT_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  dgemm_Col_NT_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  dgemm_Col_NT_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NT_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_NT_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  dgemm_Col_NT_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  dgemm_Col_NT_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  dgemm_Col_NT_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  dgemm_Col_NT_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_NT_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  dgemm_Col_NT_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  dgemm_Col_NT_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  dgemm_Col_NT_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  dgemm_Col_NT_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_NT_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_NT_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_NT_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == Trans && transB == NoTrans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TN_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  dgemm_Col_TN_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  dgemm_Col_TN_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  dgemm_Col_TN_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TN_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TN_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  dgemm_Col_TN_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  dgemm_Col_TN_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  dgemm_Col_TN_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TN_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TN_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  dgemm_Col_TN_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  dgemm_Col_TN_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  dgemm_Col_TN_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TN_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TN_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  dgemm_Col_TN_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  dgemm_Col_TN_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  dgemm_Col_TN_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TN_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TN_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  dgemm_Col_TN_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  dgemm_Col_TN_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  dgemm_Col_TN_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TN_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TN_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  dgemm_Col_TN_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  dgemm_Col_TN_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  dgemm_Col_TN_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TN_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TN_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  dgemm_Col_TN_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  dgemm_Col_TN_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  dgemm_Col_TN_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TN_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TN_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  dgemm_Col_TN_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  dgemm_Col_TN_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  dgemm_Col_TN_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TN_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TN_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  dgemm_Col_TN_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  dgemm_Col_TN_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  dgemm_Col_TN_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TN_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TN_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  dgemm_Col_TN_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  dgemm_Col_TN_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  dgemm_Col_TN_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TN_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TN_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  dgemm_Col_TN_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  dgemm_Col_TN_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  dgemm_Col_TN_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TN_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TN_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  dgemm_Col_TN_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  dgemm_Col_TN_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  dgemm_Col_TN_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TN_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TN_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  dgemm_Col_TN_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  dgemm_Col_TN_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  dgemm_Col_TN_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TN_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TN_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  dgemm_Col_TN_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  dgemm_Col_TN_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  dgemm_Col_TN_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TN_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TN_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  dgemm_Col_TN_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  dgemm_Col_TN_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  dgemm_Col_TN_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TN_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TN_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  dgemm_Col_TN_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  dgemm_Col_TN_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  dgemm_Col_TN_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TN_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TN_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  dgemm_Col_TN_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  dgemm_Col_TN_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  dgemm_Col_TN_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TN_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TN_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  dgemm_Col_TN_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  dgemm_Col_TN_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  dgemm_Col_TN_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TN_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TN_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  dgemm_Col_TN_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  dgemm_Col_TN_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  dgemm_Col_TN_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TN_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TN_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  dgemm_Col_TN_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  dgemm_Col_TN_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  dgemm_Col_TN_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TN_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TN_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TN_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TN_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == Trans && transB == Trans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TT_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  dgemm_Col_TT_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  dgemm_Col_TT_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  dgemm_Col_TT_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TT_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TT_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  dgemm_Col_TT_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  dgemm_Col_TT_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  dgemm_Col_TT_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TT_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TT_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  dgemm_Col_TT_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  dgemm_Col_TT_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  dgemm_Col_TT_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TT_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TT_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  dgemm_Col_TT_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  dgemm_Col_TT_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  dgemm_Col_TT_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TT_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TT_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  dgemm_Col_TT_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  dgemm_Col_TT_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  dgemm_Col_TT_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TT_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TT_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  dgemm_Col_TT_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  dgemm_Col_TT_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  dgemm_Col_TT_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TT_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TT_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  dgemm_Col_TT_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  dgemm_Col_TT_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  dgemm_Col_TT_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TT_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TT_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  dgemm_Col_TT_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  dgemm_Col_TT_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  dgemm_Col_TT_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TT_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TT_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  dgemm_Col_TT_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  dgemm_Col_TT_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  dgemm_Col_TT_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TT_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TT_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  dgemm_Col_TT_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  dgemm_Col_TT_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  dgemm_Col_TT_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TT_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TT_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  dgemm_Col_TT_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  dgemm_Col_TT_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  dgemm_Col_TT_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TT_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TT_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  dgemm_Col_TT_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  dgemm_Col_TT_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  dgemm_Col_TT_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TT_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TT_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  dgemm_Col_TT_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  dgemm_Col_TT_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  dgemm_Col_TT_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TT_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TT_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  dgemm_Col_TT_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  dgemm_Col_TT_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  dgemm_Col_TT_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TT_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TT_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  dgemm_Col_TT_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  dgemm_Col_TT_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  dgemm_Col_TT_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TT_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TT_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  dgemm_Col_TT_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  dgemm_Col_TT_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  dgemm_Col_TT_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TT_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TT_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  dgemm_Col_TT_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  dgemm_Col_TT_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  dgemm_Col_TT_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TT_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TT_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  dgemm_Col_TT_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  dgemm_Col_TT_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  dgemm_Col_TT_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TT_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  dgemm_Col_TT_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  dgemm_Col_TT_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  dgemm_Col_TT_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  dgemm_Col_TT_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  dgemm_Col_TT_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  dgemm_Col_TT_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  dgemm_Col_TT_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  dgemm_Col_TT_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  dgemm_Col_TT_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  dgemm_Col_TT_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  dgemm_Col_TT_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  dgemm_Col_TT_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  dgemm_Col_TT_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
      }
    }
  }
  return false; // didn't find a match
}


// cgemm kernel selection specific
template<>
bool gemmSelectKernelSpecific<float2>(
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
) {
  if (order == ColMajor) {
    if (transA == NoTrans && transB == NoTrans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NN_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_NN_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_NN_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_NN_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NN_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NN_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NN_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NN_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == NoTrans && transB == Trans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NT_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_NT_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_NT_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_NT_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NT_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NT_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NT_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NT_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == NoTrans && transB == 0) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_NC_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_NC_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_NC_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_NC_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_NC_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_NC_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_NC_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_NC_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == Trans && transB == NoTrans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TN_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_TN_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_TN_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_TN_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TN_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TN_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TN_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TN_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == Trans && transB == Trans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TT_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_TT_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_TT_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_TT_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TT_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TT_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TT_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TT_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == Trans && transB == 0) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_TC_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_TC_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_TC_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_TC_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_TC_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_TC_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_TC_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_TC_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == 0 && transB == NoTrans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CN_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_CN_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_CN_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_CN_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CN_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CN_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CN_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CN_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == 0 && transB == Trans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CT_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_CT_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_CT_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_CT_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CT_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CT_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CT_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CT_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == 0 && transB == 0) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B0_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B0_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B0_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B0_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B0_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B0_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B0_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B0_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX080_NX080_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML080_NX080_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX080_NL080_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML080_NL080_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX080_NX080_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX080_NX080_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX080_NX080_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX080_NX080_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 80 && macroTileNumCols == 80 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX080_NX080_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML080_NX080_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX080_NL080_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML080_NL080_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX080_NX080_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX080_NX080_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX080_NX080_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX080_NX080_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 1) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX096_NX096_KX01_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML096_NX096_KX01_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX096_NL096_KX01_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML096_NL096_KX01_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX096_NX096_KX01_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX096_NX096_KX01_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX096_NX096_KX01_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX096_NX096_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 96 && macroTileNumCols == 96 && unroll == 8) {
          *tileKernelSource   =  cgemm_Col_CC_B1_MX096_NX096_KX08_src;
          *rowKernelSource    =  cgemm_Col_CC_B1_ML096_NX096_KX08_src;
          *colKernelSource    =  cgemm_Col_CC_B1_MX096_NL096_KX08_src;
          *cornerKernelSource =  cgemm_Col_CC_B1_ML096_NL096_KX08_src;
          *workGroupNumRows   =  cgemm_Col_CC_B1_MX096_NX096_KX08_workGroupNumRows;
          *workGroupNumCols   =  cgemm_Col_CC_B1_MX096_NX096_KX08_workGroupNumCols;
          *microTileNumRows   =  cgemm_Col_CC_B1_MX096_NX096_KX08_microTileNumRows;
          *microTileNumCols   =  cgemm_Col_CC_B1_MX096_NX096_KX08_microTileNumCols;
          return true;
        }
      }
    }
  }
  return false; // didn't find a match
}


// zgemm kernel selection specific
template<>
bool gemmSelectKernelSpecific<double2>(
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
) {
  if (order == ColMajor) {
    if (transA == NoTrans && transB == NoTrans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NN_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_NN_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_NN_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_NN_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NN_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NN_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_NN_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_NN_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_NN_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NN_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NN_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_NN_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_NN_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_NN_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NN_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NN_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_NN_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_NN_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_NN_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NN_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NN_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_NN_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_NN_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_NN_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NN_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NN_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_NN_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_NN_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_NN_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NN_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NN_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_NN_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_NN_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_NN_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NN_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NN_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_NN_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_NN_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_NN_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NN_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NN_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_NN_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_NN_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_NN_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NN_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NN_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_NN_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_NN_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_NN_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NN_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NN_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_NN_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_NN_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_NN_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NN_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NN_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_NN_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_NN_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_NN_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NN_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NN_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_NN_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_NN_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_NN_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NN_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NN_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_NN_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_NN_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_NN_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NN_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NN_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_NN_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_NN_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_NN_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NN_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NN_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_NN_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_NN_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_NN_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NN_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NN_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NN_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NN_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == NoTrans && transB == Trans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NT_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_NT_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_NT_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_NT_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NT_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NT_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_NT_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_NT_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_NT_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NT_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NT_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_NT_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_NT_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_NT_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NT_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NT_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_NT_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_NT_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_NT_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NT_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NT_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_NT_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_NT_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_NT_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NT_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NT_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_NT_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_NT_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_NT_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NT_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NT_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_NT_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_NT_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_NT_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NT_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NT_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_NT_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_NT_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_NT_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NT_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NT_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_NT_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_NT_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_NT_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NT_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NT_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_NT_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_NT_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_NT_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NT_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NT_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_NT_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_NT_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_NT_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NT_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NT_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_NT_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_NT_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_NT_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NT_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NT_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_NT_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_NT_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_NT_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NT_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NT_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_NT_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_NT_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_NT_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NT_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NT_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_NT_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_NT_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_NT_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NT_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NT_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_NT_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_NT_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_NT_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NT_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NT_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NT_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NT_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == NoTrans && transB == 0) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NC_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_NC_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_NC_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_NC_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NC_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NC_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_NC_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_NC_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_NC_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NC_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NC_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_NC_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_NC_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_NC_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NC_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NC_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_NC_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_NC_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_NC_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NC_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NC_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_NC_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_NC_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_NC_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NC_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NC_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_NC_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_NC_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_NC_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NC_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NC_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_NC_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_NC_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_NC_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NC_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NC_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_NC_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_NC_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_NC_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NC_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NC_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_NC_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_NC_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_NC_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NC_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NC_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_NC_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_NC_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_NC_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NC_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NC_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_NC_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_NC_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_NC_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NC_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NC_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_NC_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_NC_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_NC_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NC_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NC_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_NC_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_NC_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_NC_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NC_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NC_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_NC_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_NC_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_NC_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NC_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_NC_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_NC_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_NC_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_NC_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_NC_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_NC_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_NC_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_NC_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_NC_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_NC_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_NC_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_NC_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_NC_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == Trans && transB == NoTrans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TN_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_TN_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_TN_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_TN_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TN_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TN_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_TN_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_TN_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_TN_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TN_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TN_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_TN_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_TN_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_TN_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TN_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TN_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_TN_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_TN_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_TN_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TN_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TN_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_TN_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_TN_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_TN_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TN_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TN_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_TN_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_TN_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_TN_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TN_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TN_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_TN_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_TN_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_TN_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TN_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TN_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_TN_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_TN_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_TN_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TN_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TN_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_TN_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_TN_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_TN_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TN_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TN_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_TN_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_TN_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_TN_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TN_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TN_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_TN_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_TN_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_TN_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TN_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TN_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_TN_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_TN_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_TN_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TN_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TN_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_TN_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_TN_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_TN_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TN_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TN_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_TN_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_TN_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_TN_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TN_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TN_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_TN_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_TN_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_TN_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TN_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TN_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_TN_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_TN_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_TN_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TN_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TN_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TN_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TN_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == Trans && transB == Trans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TT_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_TT_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_TT_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_TT_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TT_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TT_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_TT_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_TT_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_TT_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TT_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TT_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_TT_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_TT_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_TT_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TT_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TT_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_TT_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_TT_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_TT_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TT_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TT_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_TT_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_TT_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_TT_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TT_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TT_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_TT_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_TT_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_TT_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TT_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TT_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_TT_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_TT_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_TT_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TT_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TT_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_TT_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_TT_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_TT_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TT_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TT_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_TT_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_TT_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_TT_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TT_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TT_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_TT_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_TT_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_TT_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TT_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TT_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_TT_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_TT_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_TT_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TT_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TT_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_TT_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_TT_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_TT_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TT_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TT_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_TT_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_TT_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_TT_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TT_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TT_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_TT_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_TT_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_TT_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TT_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TT_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_TT_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_TT_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_TT_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TT_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TT_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_TT_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_TT_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_TT_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TT_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TT_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TT_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TT_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == Trans && transB == 0) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TC_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_TC_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_TC_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_TC_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TC_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TC_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_TC_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_TC_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_TC_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TC_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TC_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_TC_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_TC_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_TC_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TC_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TC_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_TC_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_TC_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_TC_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TC_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TC_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_TC_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_TC_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_TC_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TC_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TC_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_TC_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_TC_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_TC_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TC_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TC_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_TC_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_TC_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_TC_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TC_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TC_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_TC_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_TC_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_TC_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TC_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TC_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_TC_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_TC_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_TC_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TC_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TC_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_TC_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_TC_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_TC_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TC_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TC_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_TC_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_TC_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_TC_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TC_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TC_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_TC_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_TC_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_TC_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TC_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TC_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_TC_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_TC_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_TC_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TC_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TC_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_TC_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_TC_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_TC_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TC_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_TC_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_TC_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_TC_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_TC_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_TC_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_TC_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_TC_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_TC_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_TC_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_TC_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_TC_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_TC_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_TC_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == 0 && transB == NoTrans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CN_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_CN_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_CN_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_CN_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CN_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CN_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_CN_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_CN_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_CN_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CN_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CN_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_CN_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_CN_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_CN_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CN_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CN_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_CN_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_CN_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_CN_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CN_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CN_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_CN_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_CN_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_CN_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CN_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CN_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_CN_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_CN_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_CN_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CN_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CN_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_CN_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_CN_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_CN_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CN_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CN_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_CN_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_CN_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_CN_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CN_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CN_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_CN_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_CN_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_CN_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CN_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CN_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_CN_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_CN_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_CN_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CN_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CN_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_CN_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_CN_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_CN_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CN_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CN_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_CN_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_CN_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_CN_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CN_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CN_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_CN_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_CN_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_CN_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CN_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CN_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_CN_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_CN_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_CN_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CN_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CN_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_CN_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_CN_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_CN_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CN_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CN_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_CN_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_CN_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_CN_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CN_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CN_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CN_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CN_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == 0 && transB == Trans) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CT_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_CT_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_CT_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_CT_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CT_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CT_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_CT_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_CT_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_CT_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CT_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CT_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_CT_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_CT_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_CT_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CT_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CT_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_CT_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_CT_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_CT_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CT_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CT_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_CT_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_CT_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_CT_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CT_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CT_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_CT_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_CT_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_CT_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CT_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CT_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_CT_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_CT_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_CT_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CT_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CT_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_CT_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_CT_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_CT_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CT_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CT_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_CT_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_CT_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_CT_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CT_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CT_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_CT_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_CT_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_CT_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CT_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CT_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_CT_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_CT_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_CT_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CT_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CT_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_CT_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_CT_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_CT_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CT_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CT_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_CT_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_CT_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_CT_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CT_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CT_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_CT_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_CT_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_CT_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CT_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CT_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_CT_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_CT_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_CT_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CT_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CT_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_CT_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_CT_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_CT_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CT_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CT_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CT_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CT_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      }
    } else if (transA == 0 && transB == 0) {
      if ( !betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CC_B0_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_CC_B0_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_CC_B0_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_CC_B0_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CC_B0_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B0_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B0_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B0_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CC_B0_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_CC_B0_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_CC_B0_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_CC_B0_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CC_B0_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B0_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B0_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B0_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CC_B0_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_CC_B0_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_CC_B0_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_CC_B0_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CC_B0_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B0_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B0_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B0_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CC_B0_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_CC_B0_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_CC_B0_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_CC_B0_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CC_B0_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B0_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B0_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B0_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CC_B0_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_CC_B0_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_CC_B0_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_CC_B0_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CC_B0_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B0_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B0_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B0_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CC_B0_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_CC_B0_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_CC_B0_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_CC_B0_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CC_B0_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B0_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B0_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B0_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CC_B0_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_CC_B0_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_CC_B0_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_CC_B0_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CC_B0_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B0_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B0_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B0_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CC_B0_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_CC_B0_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_CC_B0_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_CC_B0_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CC_B0_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B0_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B0_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B0_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      } else if ( betaNonZero ) {
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CC_B1_MX016_NX016_KX01_src;
          *rowKernelSource    =  zgemm_Col_CC_B1_ML016_NX016_KX01_src;
          *colKernelSource    =  zgemm_Col_CC_B1_MX016_NL016_KX01_src;
          *cornerKernelSource =  zgemm_Col_CC_B1_ML016_NL016_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CC_B1_MX016_NX016_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B1_MX016_NX016_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B1_MX016_NX016_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B1_MX016_NX016_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 16 && macroTileNumCols == 16 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CC_B1_MX016_NX016_KX08_src;
          *rowKernelSource    =  zgemm_Col_CC_B1_ML016_NX016_KX08_src;
          *colKernelSource    =  zgemm_Col_CC_B1_MX016_NL016_KX08_src;
          *cornerKernelSource =  zgemm_Col_CC_B1_ML016_NL016_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CC_B1_MX016_NX016_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B1_MX016_NX016_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B1_MX016_NX016_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B1_MX016_NX016_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CC_B1_MX032_NX032_KX01_src;
          *rowKernelSource    =  zgemm_Col_CC_B1_ML032_NX032_KX01_src;
          *colKernelSource    =  zgemm_Col_CC_B1_MX032_NL032_KX01_src;
          *cornerKernelSource =  zgemm_Col_CC_B1_ML032_NL032_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CC_B1_MX032_NX032_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B1_MX032_NX032_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B1_MX032_NX032_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B1_MX032_NX032_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 32 && macroTileNumCols == 32 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CC_B1_MX032_NX032_KX08_src;
          *rowKernelSource    =  zgemm_Col_CC_B1_ML032_NX032_KX08_src;
          *colKernelSource    =  zgemm_Col_CC_B1_MX032_NL032_KX08_src;
          *cornerKernelSource =  zgemm_Col_CC_B1_ML032_NL032_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CC_B1_MX032_NX032_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B1_MX032_NX032_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B1_MX032_NX032_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B1_MX032_NX032_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CC_B1_MX048_NX048_KX01_src;
          *rowKernelSource    =  zgemm_Col_CC_B1_ML048_NX048_KX01_src;
          *colKernelSource    =  zgemm_Col_CC_B1_MX048_NL048_KX01_src;
          *cornerKernelSource =  zgemm_Col_CC_B1_ML048_NL048_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CC_B1_MX048_NX048_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B1_MX048_NX048_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B1_MX048_NX048_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B1_MX048_NX048_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 48 && macroTileNumCols == 48 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CC_B1_MX048_NX048_KX08_src;
          *rowKernelSource    =  zgemm_Col_CC_B1_ML048_NX048_KX08_src;
          *colKernelSource    =  zgemm_Col_CC_B1_MX048_NL048_KX08_src;
          *cornerKernelSource =  zgemm_Col_CC_B1_ML048_NL048_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CC_B1_MX048_NX048_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B1_MX048_NX048_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B1_MX048_NX048_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B1_MX048_NX048_KX08_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 1) {
          *tileKernelSource   =  zgemm_Col_CC_B1_MX064_NX064_KX01_src;
          *rowKernelSource    =  zgemm_Col_CC_B1_ML064_NX064_KX01_src;
          *colKernelSource    =  zgemm_Col_CC_B1_MX064_NL064_KX01_src;
          *cornerKernelSource =  zgemm_Col_CC_B1_ML064_NL064_KX01_src;
          *workGroupNumRows   =  zgemm_Col_CC_B1_MX064_NX064_KX01_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B1_MX064_NX064_KX01_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B1_MX064_NX064_KX01_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B1_MX064_NX064_KX01_microTileNumCols;
          return true;
        }
        if ( macroTileNumRows == 64 && macroTileNumCols == 64 && unroll == 8) {
          *tileKernelSource   =  zgemm_Col_CC_B1_MX064_NX064_KX08_src;
          *rowKernelSource    =  zgemm_Col_CC_B1_ML064_NX064_KX08_src;
          *colKernelSource    =  zgemm_Col_CC_B1_MX064_NL064_KX08_src;
          *cornerKernelSource =  zgemm_Col_CC_B1_ML064_NL064_KX08_src;
          *workGroupNumRows   =  zgemm_Col_CC_B1_MX064_NX064_KX08_workGroupNumRows;
          *workGroupNumCols   =  zgemm_Col_CC_B1_MX064_NX064_KX08_workGroupNumCols;
          *microTileNumRows   =  zgemm_Col_CC_B1_MX064_NX064_KX08_microTileNumRows;
          *microTileNumCols   =  zgemm_Col_CC_B1_MX064_NX064_KX08_microTileNumCols;
          return true;
        }
      }
    }
  }
  return false; // didn't find a match
}

