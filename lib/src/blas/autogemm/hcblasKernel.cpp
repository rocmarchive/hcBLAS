#ifndef MAKE_GEMM_KERNEL_SRC
#define MAKE_GEMM_KERNEL_SRC

#include "autogemm.h"

/* writeHEader():  Write the header files to the kernel file once
 *
 */
void AutogemmKernel::writeHeader(AutogemmKernel* gemmKernel, std::string& kStr) {

  // Add header files
  kStr = kStr + "#include \"hc.hpp\"" + endLine;
  kStr = kStr + "#include \"hc_math.hpp\"" + endLine;
  kStr = kStr + "#include \"hcblaslib.h\"" + endLine;
  kStr = kStr + "#include <cmath>" + endLine;
  kStr = kStr + "#include <iostream>" + endLine + "using namespace std;" + endLine;

 // data types
  kStr += endLine;
  kStr = kStr + "/* data types */" + endLine;
  kStr = kStr + "#define uint unsigned int" + endLine;
  kStr = kStr + "#define DATA_TYPE_STR " + dataTypes[gemmKernel->precision] + endLine;
}

/* makeGemmKernel():   Generates gemm Kernel String and write onto
 *                                         the file
 */
int AutogemmKernel::makeGemmKernel(AutogemmKernel* gemmKernel, kernTypes* kernelType, std::string& kStr) {

  // Check whether the kernel parameters are valid
  if (gemmKernel->validateKernParam(gemmKernel) != SUCCESS) {
        return CRIT_ERR;
  }

  kStr += endLine;

  // initializations
  kStr += "/* " + gemmKernel->getKernelName(kernelType)  +  " */" + endLine;

  // Add header guards
  kStr = kStr + "#ifndef " + gemmKernel->getKernelName(kernelType) + "_H" + endLine;
  kStr = kStr + "#define " + gemmKernel->getKernelName(kernelType) + "_H" + endLine;

  kStr += endLine;
  //function signature

  kStr = kStr + "extern \"C\" {" + endLine;
  kStr = kStr + "hcblasStatus " + gemmKernel->getKernelName(kernelType);
  kStr = kStr + "(" + endLine;
  // arguments
  kStr = kStr +
    "  hc::accelerator_view accl_view, " + endLine +
    "  DATA_TYPE_STR const * A," + endLine +
    "  DATA_TYPE_STR const * B," + endLine +
    "  DATA_TYPE_STR * C," + endLine +
    "  DATA_TYPE_STR const alpha," + endLine +
    "  DATA_TYPE_STR const beta," + endLine +
    "  uint const M," + endLine +
    "  uint const N," + endLine +
    "  uint const K," + endLine +
    "  uint const lda," + endLine +
    "  uint const ldb," + endLine +
    "  uint const ldc," + endLine +
    "  uint const offsetA," + endLine +
    "  uint const offsetB," + endLine +
    "  uint const offsetC" + endLine +
    ") {" + endLine;

  // kernel parameters
  kStr += endLine;
  kStr = kStr + "/* kernel parameters */" + endLine;
  kStr = kStr + "#define WG_NUM_ROWS          " + to_string(gemmKernel->tileNumRows) + endLine;
  kStr = kStr + "#define WG_NUM_COLS          " + to_string(gemmKernel->tileNumCols) + endLine;
  kStr = kStr + "#define MICRO_TILE_NUM_ROWS  " + to_string(gemmKernel->microtileNumRows) + endLine;
  kStr = kStr + "#define MICRO_TILE_NUM_COLS  " + to_string(gemmKernel->microtileNumCols) + endLine;
  kStr = kStr + "#define MACRO_TILE_NUM_ROWS  " + to_string(gemmKernel->tileNumRows*gemmKernel->microtileNumCols) + endLine;
  kStr = kStr + "#define MACRO_TILE_NUM_COLS  " + to_string(gemmKernel->tileNumCols*gemmKernel->microtileNumCols) + endLine;
  kStr = kStr + "#define NUM_UNROLL_ITER      " + to_string(gemmKernel->unroll) + endLine;
  if (gemmKernel->isMinibatch) {
    kStr = kStr + "#define MINIBATCH_SIZE       2" +  + endLine;
  }
  kStr += endLine;
  kStr = kStr + "#define LOCAL_ROW_PAD        0"  + endLine;
  kStr = kStr + "#define LOCAL_COL_PAD        0" + endLine;

  // global memory indices
  // A
  kStr += endLine;
  kStr = kStr +"/* global memory indices */" + endLine;
  if ((gemmKernel->isColMajor) && (!gemmKernel->isTransA))
    kStr = kStr +"#define GET_GLOBAL_INDEX_A(ROW,COL) (offsetA + (COL)*lda+(ROW))" + endLine;
  else
    kStr = kStr +"#define GET_GLOBAL_INDEX_A(ROW,COL) (offsetA + (ROW)*lda+(COL))" + endLine;
  // B
  if ((gemmKernel->isColMajor) && (!gemmKernel->isTransB))
    kStr = kStr +"#define GET_GLOBAL_INDEX_B(ROW,COL) (offsetB + (COL)*ldb+(ROW))" + endLine;
  else
    kStr = kStr +"#define GET_GLOBAL_INDEX_B(ROW,COL) (offsetB + (ROW)*ldb+(COL))" + endLine;
  // C
  if (gemmKernel->isColMajor)
    kStr = kStr +"#define GET_GLOBAL_INDEX_C(ROW,COL) (offsetC + (COL)*ldc+(ROW))" + endLine;
  else
    kStr = kStr +"#define GET_GLOBAL_INDEX_C(ROW,COL) (offsetC + (ROW)*ldc+(COL))" + endLine;

  //local memory indices
  // A
  kStr += endLine;
  kStr = kStr + "/* local memory indices */" + endLine;
  kStr = kStr + "#define GET_LOCAL_INDEX_A(ROW,COL) " + \
  "((ROW) + (COL)*((MACRO_TILE_NUM_ROWS)+(LOCAL_COL_PAD)) )" + endLine;
  // B
  kStr = kStr + "#define GET_LOCAL_INDEX_B(ROW,COL) " + \
  "((COL) + (ROW)*((MACRO_TILE_NUM_COLS)+(LOCAL_ROW_PAD)) )" + endLine;

  if (gemmKernel->precision=='s' || gemmKernel->precision=='d') {
     //real arithmetic
    kStr = kStr + "#define mad(a, b ,c) a*b+c" + endLine;
    kStr = kStr + "#define TYPE_MAD(MULA,MULB,DST) DST = mad(MULA,MULB,DST);" + endLine;
    if (gemmKernel->isBeta_nonZero)
      kStr = kStr + "#define TYPE_MAD_WRITE(DST,ALPHA,REG,BETA) DST = (ALPHA)*(REG) + (BETA)*(DST);" + endLine;
    else
      kStr = kStr + "#define TYPE_MAD_WRITE(DST,ALPHA,REG,BETA) DST = (ALPHA)*(REG);" + endLine;
  }
  else{
//    # complex arithmetic
//    if gemmkernel->transA!="C" and gemmkernel->transB!="C":
//      # neither conjugate
//      kStr = kStr + (
//        "#define TYPE_MAD(MULA,MULB,DST) \\" + endLine +
//        "  DST.x = mad(  MULA.x, MULB.x, DST.x ); \\" + endLine +
//        "  DST.x = mad( -MULA.y, MULB.y, DST.x ); \\" + endLine +
//        "  DST.y = mad(  MULA.x, MULB.y, DST.y ); \\" + endLine +
//        "  DST.y = mad(  MULA.y, MULB.x, DST.y );" + endLine )
//    elif gemmkernel->transA=="C" and gemmkernel->transB!="C":
//      # A conjugate (negate imaginary A.y)
//      kStr = kStr + (
//        "#define TYPE_MAD(MULA,MULB,DST) \\" + endLine +
//        "  DST.x = mad(  MULA.x, MULB.x, DST.x ); \\" + endLine +
//        "  DST.x = mad(  MULA.y, MULB.y, DST.x ); \\" + endLine +
//        "  DST.y = mad(  MULA.x, MULB.y, DST.y ); \\" + endLine +
//        "  DST.y = mad( -MULA.y, MULB.x, DST.y );" + endLine )
//    elif gemmkernel->transA!="C" and gemmkernel->transB=="C":
//      # B conjugate (negate imaginary B.y)
//      kStr = kStr + (
//        "#define TYPE_MAD(MULA,MULB,DST) \\" + endLine +
//        "  DST.x = mad(  MULA.x,  MULB.x, DST.x ); \\" + endLine +
//        "  DST.x = mad( -MULA.y, -MULB.y, DST.x ); \\" + endLine +
//        "  DST.y = mad(  MULA.x, -MULB.y, DST.y ); \\" + endLine +
//        "  DST.y = mad(  MULA.y,  MULB.x, DST.y );" + endLine )
//    else:
//      # A & B conjugate (negate imaginary .y)
//      kStr = kStr + (
//        "#define TYPE_MAD(MULA,MULB,DST) \\" + endLine +
//        "  DST.x = mad(  MULA.x,  MULB.x, DST.x ); \\" + endLine +
//        "  DST.x = mad(  MULA.y, -MULB.y, DST.x ); \\" + endLine +
//        "  DST.y = mad(  MULA.x, -MULB.y, DST.y ); \\" + endLine +
//        "  DST.y = mad( -MULA.y,  MULB.x, DST.y );" + endLine )
//    if gemmkernel->beta==1:
//      kStr = kStr + (
//        "#define TYPE_MAD_WRITE( DST, ALPHA, REG, BETA ) \\" + endLine +
//        "  /* (1) */ \\" + endLine +
//        "  type_mad_tmp = REG.x; \\" + endLine +
//        "  REG.x *= ALPHA.x; \\" + endLine +
//        "  REG.x = mad( -ALPHA.y, REG.y, REG.x ); \\" + endLine +
//        "  REG.y *= ALPHA.x; \\" + endLine +
//        "  REG.y = mad(  ALPHA.y, type_mad_tmp, REG.y ); \\" + endLine +
//        "  /* (2) */ \\" + endLine +
//        "  REG.x = mad(  BETA.x, DST.x, REG.x ); \\" + endLine +
//        "  REG.x = mad( -BETA.y, DST.y, REG.x ); \\" + endLine +
//        "  REG.y = mad(  BETA.y, DST.x, REG.y ); \\" + endLine +
//        "  REG.y = mad(  BETA.x, DST.y, REG.y ); \\" + endLine +
//        "  /* (3) */ \\" + endLine +
//        "  DST = REG;" + endLine )
//    else:
//      kStr = kStr + (
//        "#define TYPE_MAD_WRITE( DST, ALPHA, REG, BETA ) \\" + endLine +
//        "  /* (1) */ \\" + endLine +
//        "  type_mad_tmp = REG.x; \\" + endLine +
//        "  REG.x *= ALPHA.x; \\" + endLine +
//        "  REG.x = mad( -ALPHA.y, REG.y, REG.x ); \\" + endLine +
//        "  REG.y *= ALPHA.x; \\" + endLine +
//        "  REG.y = mad(  ALPHA.y, type_mad_tmp, REG.y ); \\" + endLine +
//        "  DST = REG;" + endLine) ;
  }

  //micro-tile
  kStr += endLine;
  kStr = kStr + "/* " + to_string(gemmKernel->microtileNumRows) + "x" + \
             to_string(gemmKernel->microtileNumCols) + " micro-tile */"  + endLine;
  if (gemmKernel->isMinibatch) {
    kStr = kStr + "#define MICRO_TILE \\" + endLine;
    for (int a = 0; a < gemmKernel->microtileNumRows; a++)
      for (int c = 0; c < 2; c++) 
        kStr = kStr + "  rA[" + to_string(a*2+c) +"] = lA[offA + " + to_string(a) +"*WG_NUM_ROWS*2 + " + to_string(c) + "]; \\" + endLine;
    for (int b = 0; b < gemmKernel->microtileNumCols; b++)
      for (int c = 0; c < 2; c++) 
        kStr = kStr + "  rB[" + to_string(b*2+c) + "] = lB[offB + " + to_string(b) + "*WG_NUM_COLS*2 + " + to_string(c) + "]; \\" + endLine;
    kStr = kStr + "  offA += (MACRO_TILE_NUM_ROWS*MINIBATCH_SIZE+LOCAL_COL_PAD+1); \\" + endLine;
    kStr = kStr + "  offB += (MACRO_TILE_NUM_COLS*MINIBATCH_SIZE+LOCAL_ROW_PAD+1); \\" + endLine;
    for (int a = 0; a < gemmKernel->microtileNumRows*2; a++)
      for (int b = 0; b < gemmKernel->microtileNumCols*2; b++)
        kStr = kStr + "  TYPE_MAD(rA[" + to_string(a) +"],rB[" + to_string(b) \
                    + "],rC[" + to_string(a) + "][" + to_string(b) + "]); \\" + endLine;
    kStr += endLine;
  } else {
    kStr = kStr + "#define MICRO_TILE \\" + endLine;
    for (int a = 0; a < gemmKernel->microtileNumRows; a++)
      kStr = kStr + "  rA[" + to_string(a) +"] = lA[offA + " + to_string(a) +"*WG_NUM_ROWS]; \\" + endLine;
    for (int b = 0; b < gemmKernel->microtileNumCols; b++)
      kStr = kStr + "  rB[" + to_string(b) + "] = lB[offB + " + to_string(b) + "*WG_NUM_COLS]; \\" + endLine;
    kStr = kStr + "  offA += (MACRO_TILE_NUM_ROWS+LOCAL_COL_PAD); \\" + endLine;
    kStr = kStr + "  offB += (MACRO_TILE_NUM_COLS+LOCAL_ROW_PAD); \\" + endLine;
    for (int a = 0; a < gemmKernel->microtileNumRows; a++)
      for (int b = 0; b < gemmKernel->microtileNumCols; b++)
        kStr = kStr + "  TYPE_MAD(rA[" + to_string(a) +"],rB[" + to_string(b) \
                    + "],rC[" + to_string(a) + "][" + to_string(b) + "]); \\" + endLine;
    kStr += endLine;
  }

  // launch the thread grids
  kStr += endLine;
  if (gemmKernel->isMinibatch) {
    kStr = kStr +
      "  /* Launch the Grid */" + endLine +
      "  int M_ = (M - 1)/ (MICRO_TILE_NUM_ROWS * MINIBATCH_SIZE) + 1;" + endLine +
      "  int N_ = (N - 1)/ (MICRO_TILE_NUM_COLS * MINIBATCH_SIZE) + 1;" + endLine +
      "  hc::extent<2> grdExt((N_ + (WG_NUM_COLS - 1)) & ~(WG_NUM_COLS - 1), (M_ + (WG_NUM_ROWS - 1)) & ~(WG_NUM_ROWS - 1));" + endLine +
      "  hc::tiled_extent<2> t_ext = grdExt.tile(WG_NUM_COLS, WG_NUM_ROWS);" + endLine;
  } else {
    kStr = kStr +
      "  /* Launch the Grid */" + endLine +
      "  int M_ = (M - 1)/ MICRO_TILE_NUM_ROWS + 1;" + endLine +
      "  int N_ = (N - 1)/ MICRO_TILE_NUM_COLS + 1;" + endLine +
      "  hc::extent<2> grdExt((N_ + (WG_NUM_COLS - 1)) & ~(WG_NUM_COLS - 1), (M_ + (WG_NUM_ROWS - 1)) & ~(WG_NUM_ROWS - 1));" + endLine +
      "  hc::tiled_extent<2> t_ext = grdExt.tile(WG_NUM_COLS, WG_NUM_ROWS);" + endLine;
  }

  // Call parallel_for_each
  kStr += endLine;
  kStr = kStr +"  hc::parallel_for_each(accl_view, t_ext, [ = ] (hc::tiled_index<2> tidx) __attribute__((hc, cpu)) { " + endLine;

  // allocate registers
  kStr += endLine;
  if (gemmKernel->isMinibatch) {
    kStr = kStr +
      "  /* allocate registers */" + endLine +
      "  DATA_TYPE_STR rC[(MICRO_TILE_NUM_ROWS*MINIBATCH_SIZE)][(MICRO_TILE_NUM_COLS*MINIBATCH_SIZE)] = { {0} };" + endLine +
      "  DATA_TYPE_STR rA[MICRO_TILE_NUM_ROWS*MINIBATCH_SIZE];" + endLine +
      "  DATA_TYPE_STR rB[MICRO_TILE_NUM_COLS*MINIBATCH_SIZE];" + endLine;
  } else {
    kStr = kStr +
      "  /* allocate registers */" + endLine +
      "  DATA_TYPE_STR rC[MICRO_TILE_NUM_ROWS][MICRO_TILE_NUM_COLS] = { {0} };" + endLine +
      "  DATA_TYPE_STR rA[MICRO_TILE_NUM_ROWS];" + endLine +
      "  DATA_TYPE_STR rB[MICRO_TILE_NUM_COLS];" + endLine;
  }

  // allocate local memory
  kStr += endLine;
  if (!gemmKernel->isMinibatch) {
    kStr = kStr +
      "  /* allocate local memory */" + endLine +
      "  tile_static DATA_TYPE_STR lA[NUM_UNROLL_ITER*(MACRO_TILE_NUM_ROWS+LOCAL_COL_PAD)+WG_NUM_ROWS];" + endLine +
      "  tile_static DATA_TYPE_STR lB[NUM_UNROLL_ITER*(MACRO_TILE_NUM_COLS+LOCAL_ROW_PAD)+WG_NUM_COLS];" + endLine;
  } else {
    kStr = kStr +
      "  /* allocate local memory */" + endLine +
      "  tile_static DATA_TYPE_STR lA[WG_NUM_ROWS*(MACRO_TILE_NUM_ROWS*MINIBATCH_SIZE+LOCAL_COL_PAD)+WG_NUM_ROWS];" + endLine +
      "  tile_static DATA_TYPE_STR lB[WG_NUM_ROWS*(MACRO_TILE_NUM_COLS*MINIBATCH_SIZE+LOCAL_ROW_PAD)+WG_NUM_ROWS];" + endLine;
  }

  // work item indices : TODO: CHANGE THE ORDER OF ACCESS
  kStr += endLine;
  if (gemmKernel->isMinibatch) {
    kStr = kStr + "  /* work item indices */" + endLine;
    kStr = kStr + "  uint gidx = tidx.tile[1];" + endLine;
    kStr = kStr + "  uint gidy = tidx.tile[0];" + endLine;
  } else {
    kStr = kStr + "  /* work item indices */" + endLine;
    if (gemmKernel->isRowKernel(kernelType))
      kStr = kStr + "  uint gidx = M / " + to_string(gemmKernel->tileNumRows*gemmKernel->microtileNumRows) + "; // last row" + endLine;
    else
      kStr = kStr + "  uint gidx = tidx.tile[1];" + endLine;
    if (gemmKernel->isColKernel(kernelType))
      kStr = kStr + "  uint gidy = N / " + to_string(gemmKernel->tileNumCols*gemmKernel->microtileNumCols) + "; // last column" + endLine;
    else
      kStr = kStr + "  uint gidy = tidx.tile[0];" + endLine;
  }

  if (!gemmKernel->isMinibatch) {
    kStr = kStr +
      "  uint idx = tidx.local[1];" + endLine  +
      "  uint idy = tidx.local[0];" + endLine +
      "  uint lIndex = idy * WG_NUM_ROWS + idx;" + endLine +
      "  long AinitOffset = 0;" + endLine +
      "  long BinitOffset = 0;" + endLine +
      "  long CinitOffset = 0;" + endLine;
  
    // global indices being loaded
    kStr += endLine;
    kStr = kStr + "  /* global indices being loaded */" + endLine;
    if ((gemmKernel->isColMajor) && (!gemmKernel->isTransA))
      kStr = kStr +
        "#define globalARow(LID) (gidx*MACRO_TILE_NUM_ROWS + (lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)%MACRO_TILE_NUM_ROWS)" + endLine +
        "#define globalACol(LID) ((lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)/MACRO_TILE_NUM_ROWS)" + endLine;
    else
      kStr = kStr +
        "#define globalARow(LID) (gidx*MACRO_TILE_NUM_ROWS + (lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)/NUM_UNROLL_ITER)" + endLine +
        "#define globalACol(LID) ((lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)%NUM_UNROLL_ITER)" + endLine;
  
    if ((gemmKernel->isColMajor) && (!gemmKernel->isTransB))
      kStr = kStr +
        "#define globalBRow(LID) ((lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)%NUM_UNROLL_ITER)" + endLine +
        "#define globalBCol(LID) (gidy*MACRO_TILE_NUM_COLS + (lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)/NUM_UNROLL_ITER)" + endLine;
    else
      kStr = kStr +
        "#define globalBRow(LID) ((lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)/MACRO_TILE_NUM_COLS)" + endLine +
        "#define globalBCol(LID) (gidy*MACRO_TILE_NUM_COLS + (lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)%MACRO_TILE_NUM_COLS)" + endLine;
  } else {
       kStr = kStr +
      "  uint idx = tidx.local[1];" + endLine  +
      "  uint idy = tidx.local[0];" + endLine +
      "  long AinitOffset = 0;" + endLine +
      "  long BinitOffset = 0;" + endLine +
      "  long CinitOffset = 0;" + endLine +
      "  uint alIndex = (idy * (WG_NUM_COLS * MICRO_TILE_NUM_COLS * 2 + 1) + idx * 2);" + endLine +
      "  uint blIndex = (idx * (WG_NUM_ROWS * MICRO_TILE_NUM_ROWS * 2 + 1) + idy * 2);" + endLine +
      "  uint AIndex = offsetA + (gidx * WG_NUM_ROWS * MICRO_TILE_NUM_ROWS * 2) + idx * 2 + (idy * lda);" + endLine +
      "  uint BIndex = offsetB + ((gidy * WG_NUM_COLS * MICRO_TILE_NUM_ROWS * 2) + idy * 2) * ldb + idx;" + endLine +
      "  uint CIndex = offsetC + ((gidx * WG_NUM_ROWS * MICRO_TILE_NUM_ROWS * 2) + idx * 2) + ( ((gidy * WG_NUM_COLS * MICRO_TILE_NUM_ROWS * 2) + idy * 2) * ldc);" + endLine;
  }

  // loop over k
  kStr += endLine;
  kStr = kStr +
    "  /* loop over k */" + endLine +
    "  uint block_k = K / NUM_UNROLL_ITER;" + endLine +
    "  do {" + endLine;

  // local indices being written
  kStr += endLine;
  if (!gemmKernel->isMinibatch) {
    kStr = kStr + "    /* local indices being written */" + endLine;
    if ((gemmKernel->isColMajor) && (!gemmKernel->isTransA))
      kStr = kStr +
        "#define localARow (lIndex % MACRO_TILE_NUM_ROWS)" + endLine +
        "#define localACol (lIndex / MACRO_TILE_NUM_ROWS)" + endLine +
        "#define localAStride (WG_NUM_ROWS*WG_NUM_COLS)" + endLine;
    else
      kStr = kStr +
        "#define localARow (lIndex / NUM_UNROLL_ITER)" + endLine +
        "#define localACol (lIndex % NUM_UNROLL_ITER)" + endLine +
        "#define localAStride (WG_NUM_ROWS*WG_NUM_COLS/NUM_UNROLL_ITER)" + endLine;
   
    if ((gemmKernel->isColMajor) && (!gemmKernel->isTransB))
      kStr = kStr +
        "#define localBRow ( lIndex % NUM_UNROLL_ITER )" + endLine +
        "#define localBCol ( lIndex / NUM_UNROLL_ITER )" + endLine +
        "#define localBStride (WG_NUM_ROWS*WG_NUM_COLS/NUM_UNROLL_ITER)" + endLine;
    else
      kStr = kStr +
        "#define localBRow ( lIndex / MACRO_TILE_NUM_COLS )" + endLine +
        "#define localBCol ( lIndex % MACRO_TILE_NUM_COLS )" + endLine +
        "#define localBStride  (WG_NUM_ROWS*WG_NUM_COLS)" + endLine;
  }

  kStr += endLine;
  kStr = kStr + "    tidx.barrier.wait();" + endLine;

  // load global -> local
  // threads to do loading = (tileNumRows*tileNumCols)
  // A elements to be loaded = tileNumRows*microTileNumRows*unroll
  // B elements to be loaded = tileNumCols*microTileNumCols*unroll
  kStr += endLine;
  kStr = kStr + "    /* load global -> local */" + endLine;
  int numALoads  = (gemmKernel->tileNumRows*gemmKernel->microtileNumRows*gemmKernel->unroll) \
                                    / (gemmKernel->tileNumRows*gemmKernel->tileNumCols);
  int numALoadsR = (gemmKernel->tileNumRows*gemmKernel->microtileNumRows*gemmKernel->unroll) \
                                    % (gemmKernel->tileNumRows*gemmKernel->tileNumCols);
  int numBLoads  = (gemmKernel->tileNumCols*gemmKernel->microtileNumCols*gemmKernel->unroll) \
                                    / (gemmKernel->tileNumRows*gemmKernel->tileNumCols);
  int numBLoadsR = (gemmKernel->tileNumCols*gemmKernel->microtileNumCols*gemmKernel->unroll) \
                                % (gemmKernel->tileNumRows*gemmKernel->tileNumCols);

 // TODO - zeroString for real and complex
  std::string zeroString = "0.0";

  if (gemmKernel->isMinibatch) {

    for ( int a = 0; a < numALoads; a++) 
      for (int b = 0; b < 2; b++) { 
        kStr = kStr + "    lA[alIndex + " + to_string(a) + "*WG_NUM_COLS*2 + " + to_string(b) + " ] = ";
        kStr = kStr + "A[ AinitOffset + AIndex + " + to_string(a) + \
                  " * WG_NUM_COLS * 2 + " + to_string(b) + " ];" + endLine;
      }
    
    for ( int a = 0 ; a < numBLoads; a++) 
      for (int b = 0; b < 2; b++) { 
        kStr = kStr + "    lB[blIndex + " + to_string(a) + "*WG_NUM_ROWS*2 + " + to_string(b) + " ] = "; 
        kStr = kStr + "B[ BinitOffset + BIndex + (" + to_string(a) + \
                  " * WG_NUM_COLS * 2 + " + to_string(b) + ") * ldb ];" + endLine;
      }

  } else {

    for ( int a = 0; a < numALoads; a++) {
      kStr = kStr + "    lA[ GET_LOCAL_INDEX_A(localARow, localACol) + " + to_string(a) + "*localAStride ] = ";
      if (gemmKernel->isRowKernel(kernelType))
        kStr = kStr + "( globalARow(" + to_string(a) + ") >= M) ? " + zeroString + " : ";
      kStr = kStr + "A[ AinitOffset + GET_GLOBAL_INDEX_A( globalARow(" + to_string(a) + \
                  "), globalACol(" + to_string(a) + ") ) ];" + endLine;
    }
    if (numALoadsR) {
      kStr = kStr + "    if ( lIndex + " + to_string(numALoads) + "*WG_NUM_ROWS*WG_NUM_COLS < (WG_NUM_ROWS*MICRO_TILE_NUM_ROWS*NUM_UNROLL_ITER) ) {" + endLine;
      kStr = kStr + "      lA[GET_LOCAL_INDEX_A(localARow, localACol)+ " + to_string(numALoads) + "*localAStride ] = ";
      if (gemmKernel->isRowKernel(kernelType))
        kStr = kStr + "( globalARow(" + to_string(numALoads) + ") >= M) ? " + zeroString + " : ";
      kStr = kStr + "A[ AinitOffset + GET_GLOBAL_INDEX_A( globalARow(" + to_string(numALoads) + \
                  "), globalACol(" + to_string(numALoads) + ") ) ];" + endLine;
      kStr = kStr + "    }" + endLine;
    }
  
     for ( int b = 0 ; b < numBLoads; b++) {
      kStr = kStr + "    lB[ GET_LOCAL_INDEX_B(localBRow, localBCol) + "  + to_string(b) + "*localBStride ] = ";
      if (gemmKernel->isColKernel(kernelType))
        kStr = kStr + "( globalBCol(" + to_string(b) + ") >= N) ? " + zeroString + " : ";
      kStr = kStr + "B[ BinitOffset + GET_GLOBAL_INDEX_B( globalBRow(" + to_string(b) + \
                  "), globalBCol(" + to_string(b) + ") ) ];" + endLine;
     }
    if (numBLoadsR) {
      kStr = kStr + "    if ( lIndex + " + to_string(numBLoads) + "*WG_NUM_ROWS*WG_NUM_COLS " \
                  + "< (WG_NUM_COLS*MICRO_TILE_NUM_COLS*NUM_UNROLL_ITER) ) {" + endLine;
      kStr = kStr + "      lB[ GET_LOCAL_INDEX_B(localBRow, localBCol) + " + to_string(numBLoads) + "*localBStride ] = ";
      if (gemmKernel->isColKernel(kernelType))
        kStr = kStr + "(globalBCol(" + to_string(numBLoads) + ") >= N) ?" + zeroString + " : ";
      kStr = kStr + "B[ BinitOffset + GET_GLOBAL_INDEX_B( globalBRow(" + to_string(numBLoads) + \
                  "), globalBCol(" + to_string(numBLoads) + ") ) ];" + endLine;
      kStr = kStr + "    }" + endLine;
    }
  }

  if (!gemmKernel->isMinibatch) {
    kStr = kStr +
      "    tidx.barrier.wait();" + endLine +
      "    uint offA = idx;" + endLine +
      "    uint offB = idy;" + endLine;
  } else {
    kStr = kStr +
      "    tidx.barrier.wait();" + endLine +
      "    uint offA = idx*2;" + endLine +
      "    uint offB = idy*2;" + endLine;
  }

  // do mads
  kStr += endLine;
  kStr = kStr + "    /* do mads */" + endLine;
  kStr = kStr + "    for ( int u = 0; u < NUM_UNROLL_ITER; u++) {" + endLine;
    kStr = kStr + "      MICRO_TILE; " + endLine  "    }"+ endLine;

  // shift to next k block
  kStr += endLine;
  kStr = kStr + "    /* shift to next k block */" + endLine;
  if ((gemmKernel->isColMajor) && (!gemmKernel->isTransA))
    kStr = kStr + "    AinitOffset += lda*NUM_UNROLL_ITER;" + endLine;
  else
    kStr = kStr + "    AinitOffset += NUM_UNROLL_ITER;" + endLine;
  if ((gemmKernel->isColMajor) && (!gemmKernel->isTransB))
    kStr = kStr + "    BinitOffset += NUM_UNROLL_ITER;" + endLine;
  else
    kStr = kStr + "    BinitOffset += ldb*NUM_UNROLL_ITER;" + endLine;

  // end loop
  kStr += endLine;
  kStr = kStr + "  } while (--block_k > 0);" + endLine;
  kStr += endLine;

  // which global Cij index
  if (!gemmKernel->isMinibatch) {

    kStr = kStr + endLine
              + "  /* which global Cij index */" + endLine
              + "  uint globalCRow = gidx * MACRO_TILE_NUM_ROWS + idx;" + endLine
              + "  uint globalCCol = gidy * MACRO_TILE_NUM_COLS + idy;" + endLine;

    // write global Cij
    kStr += endLine;
    kStr = kStr + "  /* write global Cij */" + endLine;
    if (gemmKernel->precision=='c')
      kStr = kStr + "  float type_mad_tmp;" + endLine;
    if (gemmKernel->precision=='z')
      kStr = kStr + "  double type_mad_tmp;" + endLine;
 
     for (int a = 0; a < gemmKernel->microtileNumRows; a++) {
      for (int b = 0; b < gemmKernel->microtileNumCols; b++) {
        if (gemmKernel->isRowKernel(kernelType))
          kStr = kStr + "  if (globalCRow+" + to_string(a) + "*WG_NUM_ROWS < M)";
        if (gemmKernel->isColKernel(kernelType))
          kStr = kStr + "  if (globalCCol+" + to_string(b) + "*WG_NUM_COLS < N)";
        if (gemmKernel->isRowKernel(kernelType) || gemmKernel->isColKernel(kernelType))
          kStr = kStr + "{";
        kStr = kStr + "  TYPE_MAD_WRITE( C[ GET_GLOBAL_INDEX_C( globalCRow+" + \
                      to_string(a) + "*WG_NUM_ROWS, globalCCol+" + to_string(b) + \
                      "*WG_NUM_COLS) ], alpha, rC[" + to_string(a) + "][" + to_string(b) + "], beta )";
        if (gemmKernel->isRowKernel(kernelType) || gemmKernel->isColKernel(kernelType))
          kStr = kStr + "}";
        kStr = kStr + endLine;
      }
     }

  } else {
    for (int a = 0; a < gemmKernel->microtileNumRows*2; a++) {
      int it = 0;
      for (int b = 0; b < gemmKernel->microtileNumCols*2; b++) {
          kStr = kStr + "  TYPE_MAD_WRITE(C[CIndex + CinitOffset + (WG_NUM_ROWS * " + to_string(it) +" *2 + "+ to_string(b%2) + ")* ldc + " + to_string(a%2) + "], alpha, rC[" + to_string(a) + "][" + to_string(b) + "], beta )" + endLine;
          if (b%2 != 0)
            it++;
      }
      if (a % 2 != 0)
      kStr = kStr + "  CinitOffset += WG_NUM_ROWS * 2;" + endLine;
    }
  }

  // end parallel for each
  kStr += endLine;
  kStr = kStr + "  }).wait();" + endLine;

  // Undefs all the macros for future use
  // kernel parameters
  kStr += endLine;
  kStr = kStr + "#undef WG_NUM_ROWS          " + endLine;
  kStr = kStr + "#undef WG_NUM_COLS          " + endLine;
  kStr = kStr + "#undef MICRO_TILE_NUM_ROWS  " + endLine;
  kStr = kStr + "#undef MICRO_TILE_NUM_COLS  " + endLine;
  kStr = kStr + "#undef MACRO_TILE_NUM_ROWS  " + endLine;
  kStr = kStr + "#undef MACRO_TILE_NUM_COLS  " + endLine;
  kStr = kStr + "#undef NUM_UNROLL_ITER      " + endLine;
  kStr = kStr + "#undef LOCAL_ROW_PAD        " + endLine;
  kStr = kStr + "#undef LOCAL_COL_PAD        " + endLine;

  // global memory indices
  // A
  kStr += endLine;
  kStr = kStr +"#undef GET_GLOBAL_INDEX_A " + endLine;
  // B
  kStr = kStr +"#undef GET_GLOBAL_INDEX_B " + endLine;
  // C
  kStr = kStr +"#undef GET_GLOBAL_INDEX_C " + endLine;

  //local memory indices
  // A
  kStr += endLine;
  kStr = kStr + "#undef GET_LOCAL_INDEX_A " + endLine;
  // B
  kStr = kStr + "#undef GET_LOCAL_INDEX_B " + endLine;

  //real arithmetic
  kStr = kStr + "#undef mad " + endLine;
  kStr = kStr + "#undef TYPE_MAD " + endLine;
  kStr = kStr + "#undef TYPE_MAD_WRITE " + endLine;

  kStr += endLine;
  kStr = kStr + "#undef MICRO_TILE " + endLine;
  
  kStr = kStr + "  return HCBLAS_SUCCEEDS;" + endLine ;

  // end kernel
  kStr = kStr + endLine + "}" + endLine;
  kStr = kStr + "} /// extern" + endLine;
  kStr = kStr + endLine + "#endif" + endLine;

  return SUCCESS;
}

#endif // MAKE_GEMM_KERNEL_SRC
