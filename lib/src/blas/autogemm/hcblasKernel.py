import os
import sys
import copy
import Common
import KernelParameters
import AutoGemmParameters
import argparse


##############################################################################
# Make OpenCL Kernel String
##############################################################################
def makeOpenCLKernelString(kernel):
  endLine = "\n"

  ####################################
  # parameters valid?
  if kernel.isValid() == False:
    return kernel.getName() + " invalid"

  ####################################
  # initializations
  kStr = "/* %s */" % kernel.getName()
  kStr += endLine

  ####################################
  # kernel parameters
  kStr += endLine
  kStr += "/* kernel parameters */" + endLine
  kStr += "#define WG_NUM_ROWS          %d%s" % (kernel.workGroupNumRows, endLine )
  kStr += "#define WG_NUM_COLS          %d%s" % (kernel.workGroupNumCols, endLine )
  kStr += "#define MICRO_TILE_NUM_ROWS  %d%s" % (kernel.microTileNumRows, endLine )
  kStr += "#define MICRO_TILE_NUM_COLS  %d%s" % (kernel.microTileNumCols, endLine )
  kStr += "#define MACRO_TILE_NUM_ROWS  %s%s" % ((kernel.workGroupNumRows * kernel.microTileNumRows), endLine )
  kStr += "#define MACRO_TILE_NUM_COLS  %s%s" % ((kernel.workGroupNumCols * kernel.microTileNumCols), endLine )
  kStr += "#define NUM_UNROLL_ITER      %s%s" % (kernel.unroll, endLine )
  kStr += "" + endLine
  kStr += "#define LOCAL_ROW_PAD        %s%s" % (kernel.localRowPad, endLine)
  kStr += "#define LOCAL_COL_PAD        %s%s" % (kernel.localColPad, endLine)

  ####################################
  # global memory indices
  # A
  kStr += endLine
  kStr += "/* global memory indices */" + endLine
  if (kernel.order=="clblasColumnMajor")==(kernel.transA=="N"):
    kStr += "#define GET_GLOBAL_INDEX_A(ROW,COL) (offsetA + (COL)*lda+(ROW))" + endLine
  else:
    kStr += "#define GET_GLOBAL_INDEX_A(ROW,COL) (offsetA + (ROW)*lda+(COL))" + endLine
  # B
  if (kernel.order=="clblasColumnMajor")==(kernel.transB=="N"):
    kStr += "#define GET_GLOBAL_INDEX_B(ROW,COL) (offsetB + (COL)*ldb+(ROW))" + endLine
  else:
    kStr += "#define GET_GLOBAL_INDEX_B(ROW,COL) (offsetB + (ROW)*ldb+(COL))" + endLine
  # C
  if (kernel.order=="clblasColumnMajor"):
    kStr += "#define GET_GLOBAL_INDEX_C(ROW,COL) (offsetC + (COL)*ldc+(ROW))" + endLine
  else:
    kStr += "#define GET_GLOBAL_INDEX_C(ROW,COL) (offsetC + (ROW)*ldc+(COL))" + endLine

  ####################################
  # local memory indices
  # A
  kStr += endLine
  kStr += "/* local memory indices */" + endLine
  kStr += "#define GET_LOCAL_INDEX_A(ROW,COL) ((ROW) + (COL)*((MACRO_TILE_NUM_ROWS)+(LOCAL_COL_PAD)) )" + endLine
  # B
  kStr += "#define GET_LOCAL_INDEX_B(ROW,COL) ((COL) + (ROW)*((MACRO_TILE_NUM_COLS)+(LOCAL_ROW_PAD)) )" + endLine

  ####################################
  # data types
  kStr += endLine
  kStr += "/* data types */" + endLine
  kStr += "#define uint unsigned int"
  kStr += "#define DATA_TYPE_STR %s%s" \
      % (Common.openclDataType[kernel.precision], endLine)
  if kernel.precision=="s" or kernel.precision=="d":
    # real arithmetic
    kStr += "#define TYPE_MAD(MULA,MULB,DST) DST = mad(MULA,MULB,DST);" + endLine
    if kernel.beta==1:
      kStr += "#define TYPE_MAD_WRITE(DST,ALPHA,REG,BETA) DST = (ALPHA)*(REG) + (BETA)*(DST);" + endLine
    else:
      kStr += "#define TYPE_MAD_WRITE(DST,ALPHA,REG,BETA) DST = (ALPHA)*(REG);" + endLine

  else:
    # complex arithmetic
    if kernel.transA!="C" and kernel.transB!="C":
      # neither conjugate
      kStr += (
        "#define TYPE_MAD(MULA,MULB,DST) \\\\" + endLine +
        "  DST.s0 = mad(  MULA.s0, MULB.s0, DST.s0 ); \\\\" + endLine +
        "  DST.s0 = mad( -MULA.s1, MULB.s1, DST.s0 ); \\\\" + endLine +
        "  DST.s1 = mad(  MULA.s0, MULB.s1, DST.s1 ); \\\\" + endLine +
        "  DST.s1 = mad(  MULA.s1, MULB.s0, DST.s1 );" + endLine )
    elif kernel.transA=="C" and kernel.transB!="C":
      # A conjugate (negate imaginary A.s1)
      kStr += (
        "#define TYPE_MAD(MULA,MULB,DST) \\\\" + endLine +
        "  DST.s0 = mad(  MULA.s0, MULB.s0, DST.s0 ); \\\\" + endLine +
        "  DST.s0 = mad(  MULA.s1, MULB.s1, DST.s0 ); \\\\" + endLine +
        "  DST.s1 = mad(  MULA.s0, MULB.s1, DST.s1 ); \\\\" + endLine +
        "  DST.s1 = mad( -MULA.s1, MULB.s0, DST.s1 );" + endLine )
    elif kernel.transA!="C" and kernel.transB=="C":
      # B conjugate (negate imaginary B.s1)
      kStr += (
        "#define TYPE_MAD(MULA,MULB,DST) \\\\" + endLine +
        "  DST.s0 = mad(  MULA.s0,  MULB.s0, DST.s0 ); \\\\" + endLine +
        "  DST.s0 = mad( -MULA.s1, -MULB.s1, DST.s0 ); \\\\" + endLine +
        "  DST.s1 = mad(  MULA.s0, -MULB.s1, DST.s1 ); \\\\" + endLine +
        "  DST.s1 = mad(  MULA.s1,  MULB.s0, DST.s1 );" + endLine )
    else:
      # A & B conjugate (negate imaginary .s1)
      kStr += (
        "#define TYPE_MAD(MULA,MULB,DST) \\\\" + endLine +
        "  DST.s0 = mad(  MULA.s0,  MULB.s0, DST.s0 ); \\\\" + endLine +
        "  DST.s0 = mad(  MULA.s1, -MULB.s1, DST.s0 ); \\\\" + endLine +
        "  DST.s1 = mad(  MULA.s0, -MULB.s1, DST.s1 ); \\\\" + endLine +
        "  DST.s1 = mad( -MULA.s1,  MULB.s0, DST.s1 );" + endLine )
    if kernel.beta==1:
      kStr += (
        "#define TYPE_MAD_WRITE( DST, ALPHA, REG, BETA ) \\\\" + endLine +
        "  /* (1) */ \\\\" + endLine +
        "  type_mad_tmp = REG.s0; \\\\" + endLine +
        "  REG.s0 *= ALPHA.s0; \\\\" + endLine +
        "  REG.s0 = mad( -ALPHA.s1, REG.s1, REG.s0 ); \\\\" + endLine +
        "  REG.s1 *= ALPHA.s0; \\\\" + endLine +
        "  REG.s1 = mad(  ALPHA.s1, type_mad_tmp, REG.s1 ); \\\\" + endLine +
        "  /* (2) */ \\\\" + endLine +
        "  REG.s0 = mad(  BETA.s0, DST.s0, REG.s0 ); \\\\" + endLine +
        "  REG.s0 = mad( -BETA.s1, DST.s1, REG.s0 ); \\\\" + endLine +
        "  REG.s1 = mad(  BETA.s1, DST.s0, REG.s1 ); \\\\" + endLine +
        "  REG.s1 = mad(  BETA.s0, DST.s1, REG.s1 ); \\\\" + endLine +
        "  /* (3) */ \\\\" + endLine +
        "  DST = REG;" + endLine )
    else:
      kStr += (
        "#define TYPE_MAD_WRITE( DST, ALPHA, REG, BETA ) \\\\" + endLine +
        "  /* (1) */ \\\\" + endLine +
        "  type_mad_tmp = REG.s0; \\\\" + endLine +
        "  REG.s0 *= ALPHA.s0; \\\\" + endLine +
        "  REG.s0 = mad( -ALPHA.s1, REG.s1, REG.s0 ); \\\\" + endLine +
        "  REG.s1 *= ALPHA.s0; \\\\" + endLine +
        "  REG.s1 = mad(  ALPHA.s1, type_mad_tmp, REG.s1 ); \\\\" + endLine +
        "  DST = REG;" + endLine )

  ####################################
  # micro-tile
  kStr += endLine
  kStr += "/* %dx%d micro-tile */%s" % (kernel.microTileNumRows, kernel.microTileNumCols, endLine)
  kStr += "#define MICRO_TILE \ " + endLine
  for a in range(0, int(kernel.microTileNumRows)):
    kStr += "  rA[%d] = lA[offA + %d*WG_NUM_ROWS]; \ %s" % (a, a, endLine)
  for b in range(0, int(kernel.microTileNumCols)):
    kStr += "  rB[%d] = lB[offB + %d*WG_NUM_COLS]; \ %s" % (b, b, endLine)
  kStr += "  offA += (MACRO_TILE_NUM_ROWS+LOCAL_COL_PAD); \ " + endLine
  kStr += "  offB += (MACRO_TILE_NUM_COLS+LOCAL_ROW_PAD); \ " + endLine
  for a in range(0, int(kernel.microTileNumRows)):
    for b in range(0, int(kernel.microTileNumCols)):
      kStr += "  TYPE_MAD(rA[%d],rB[%d],rC[%d][%d]); \ %s" % (a, b, a, b, endLine)
  kStr += endLine

  ####################################
  # function signature
  ####################################
  kStr += "__hcblasStatus %s" % ( kernel.getName() )
  kStr += "(" + endLine
  # arguments
  kStr += (
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
    ") {" + endLine )

  ####################################
  # launch the thread grids
  kStr += endLine 
  kStr += ( 
    "  /* Launch the Grid */" + endLine + 
    "  int M_ = (M - 1)/ MICRO_TILE_NUM_ROWS + 1;" + endLine + 
    "  int N_ = (N - 1)/ MICRO_TILE_NUM_COLS + 1;" + endLine +
    "  hc::extent<2> grdExt((N_ + (WG_NUM_COLS - 1)) & ~(WG_NUM_COLS - 1), (M_ + (WG_NUM_ROWS - 1)) & ~(WG_NUM_ROWS - 1));" + endLine +
    "  hc::tiled_extent<2> t_ext = grdExt.tile(WG_NUM_COLS, WG_NUM_ROWS);" + endLine )

  ####################################
  # Call parallel_for_each 
  kStr += endLine 
  kStr += "  hc::parallel_for_each(accl_view, t_ext, [ = ] (hc::tiled_index<2> tidx) __attribute__((hc, cpu)) { " + endLine

  ####################################
  # allocate registers
  kStr += endLine
  kStr += (
    "  /* allocate registers */" + endLine +
    "  DATA_TYPE_STR rC[MICRO_TILE_NUM_ROWS][MICRO_TILE_NUM_COLS] = { {0} };" + endLine +
    "  DATA_TYPE_STR rA[MICRO_TILE_NUM_ROWS];" + endLine +
    "  DATA_TYPE_STR rB[MICRO_TILE_NUM_COLS];" + endLine )

  ####################################
  # allocate local memory
  kStr += endLine
  kStr += (
    "  /* allocate local memory */" + endLine +
    "  tile_static DATA_TYPE_STR lA[NUM_UNROLL_ITER*(MACRO_TILE_NUM_ROWS+LOCAL_COL_PAD)];" + endLine +
    "  tile_static DATA_TYPE_STR lB[NUM_UNROLL_ITER*(MACRO_TILE_NUM_COLS+LOCAL_ROW_PAD)];" + endLine )

  ####################################
  # work item indices : TODO: CHANGE THE ORDER OF ACCESS
  kStr += endLine
  kStr += "  /* work item indices */" + endLine
  if kernel.isRowKernel():
    kStr += "  uint gidx = M / " + str(kernel.workGroupNumRows*kernel.microTileNumRows) + "; // last row" + endLine
  else:
    kStr += "  uint gidx = tidx.tile[1];" + endLine
  if kernel.isColKernel():
    kStr += "  uint gidy = N / " + str(kernel.workGroupNumCols*kernel.microTileNumCols) + "; // last column" + endLine
  else:
    kStr += "  uint gidy = tidx.tile[0];" + endLine
  
  kStr += (
    "  uint idx = tidx.local[1];" + endLine  +
    "  uint idy = tidx.local[0];" + endLine +
    "  uint lIndex = idy * WG_NUM_ROWS + idx;" + endLine +
    "  long AinitOffset = 0;" + endLine + 
    "  long BinitOffset = 0;" + endLine +
    "  long CinitOffset = 0;" + endLine )

  ####################################
  # global indices being loaded
  kStr += endLine
  kStr += "  /* global indices being loaded */" + endLine
  if (kernel.order=="clblasColumnMajor")==(kernel.transA=="N"):
    kStr += (
      "#define globalARow(LID) (gidx*MACRO_TILE_NUM_ROWS + (lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)%MACRO_TILE_NUM_ROWS)" + endLine +
      "#define globalACol(LID) ((lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)/MACRO_TILE_NUM_ROWS)" + endLine )
  else:
    kStr += (
      "#define globalARow(LID) (gidx*MACRO_TILE_NUM_ROWS + (lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)/NUM_UNROLL_ITER)" + endLine +
      "#define globalACol(LID) ((lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)%NUM_UNROLL_ITER)" + endLine )

  if (kernel.order=="clblasColumnMajor")==(kernel.transB=="N"):
    kStr += (
      "#define globalBRow(LID) ((lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)%NUM_UNROLL_ITER)" + endLine +
      "#define globalBCol(LID) (gidy*MACRO_TILE_NUM_COLS + (lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)/NUM_UNROLL_ITER)" + endLine )
  else:
    kStr += (
      "#define globalBRow(LID) ((lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)/MACRO_TILE_NUM_COLS)" + endLine +
      "#define globalBCol(LID) (gidy*MACRO_TILE_NUM_COLS + (lIndex+(LID)*WG_NUM_ROWS*WG_NUM_COLS)%MACRO_TILE_NUM_COLS)" + endLine )

  #kStr += (
  #  "  A += GET_GLOBAL_INDEX_A( globalARow, globalACol );" + endLine +
  #  "  B += GET_GLOBAL_INDEX_B( globalBRow, globalBCol );" + endLine )

  ####################################
  # loop over k
  kStr += endLine
  kStr += (
    "  /* loop over k */" + endLine +
    "  uint block_k = K / NUM_UNROLL_ITER;" + endLine +
    "  do {" + endLine )

  ####################################
  # local indices being written
  kStr += endLine
  kStr += "    /* local indices being written */" + endLine
  if (kernel.order=="clblasColumnMajor")==(kernel.transA=="N"):
    kStr += (
      "#define localARow (lIndex % MACRO_TILE_NUM_ROWS)" + endLine +
      "#define localACol (lIndex / MACRO_TILE_NUM_ROWS)" + endLine +
      "#define localAStride (WG_NUM_ROWS*WG_NUM_COLS)" + endLine )
  else:
    kStr += (
      "#define localARow (lIndex / NUM_UNROLL_ITER)" + endLine +
      "#define localACol (lIndex % NUM_UNROLL_ITER)" + endLine +
      "#define localAStride (WG_NUM_ROWS*WG_NUM_COLS/NUM_UNROLL_ITER)" + endLine )

  if (kernel.order=="clblasColumnMajor")==(kernel.transB=="N"):
    kStr += (
      "#define localBRow ( lIndex % NUM_UNROLL_ITER )" + endLine +
      "#define localBCol ( lIndex / NUM_UNROLL_ITER )" + endLine +
      "#define localBStride (WG_NUM_ROWS*WG_NUM_COLS/NUM_UNROLL_ITER)" + endLine )
  else:
    kStr += (
      "#define localBRow ( lIndex / MACRO_TILE_NUM_COLS )" + endLine +
      "#define localBCol ( lIndex % MACRO_TILE_NUM_COLS )" + endLine +
      "#define localBStride  (WG_NUM_ROWS*WG_NUM_COLS)" + endLine )

  kStr += endLine 
  kStr += "    tidx.barrier.wait();" + endLine 

  ####################################
  # load global -> local
  # threads to do loading = (workGroupNumRows*workGroupNumCols)
  # A elements to be loaded = workGroupNumRows*microTileNumRows*unroll
  # B elements to be loaded = workGroupNumCols*microTileNumCols*unroll
  kStr += endLine
  kStr += "    /* load global -> local */" + endLine
  numALoads  = (kernel.workGroupNumRows*kernel.microTileNumRows*kernel.unroll) \
      // (kernel.workGroupNumRows*kernel.workGroupNumCols) # // -- integer divide
  numALoadsR = (kernel.workGroupNumRows*kernel.microTileNumRows*kernel.unroll) \
      % (kernel.workGroupNumRows*kernel.workGroupNumCols)
  numBLoads  = (kernel.workGroupNumCols*kernel.microTileNumCols*kernel.unroll) \
      // (kernel.workGroupNumRows*kernel.workGroupNumCols) # // - integer divide
  numBLoadsR = (kernel.workGroupNumCols*kernel.microTileNumCols*kernel.unroll) \
      % (kernel.workGroupNumRows*kernel.workGroupNumCols)

  # TODO - zeroString for real and complex
  if kernel.precision == "c":
    zeroString = "(float2)(0.f, 0.f)"
  elif kernel.precision == "z":
    zeroString = "(double2)(0.0, 0.0)"
  else:
    zeroString = "0.0"
  for a in range(0, int(numALoads)):
    kStr += "    lA[ GET_LOCAL_INDEX_A(localARow, localACol) + %d*localAStride ] = " % a
    if kernel.isRowKernel():
      kStr += "( globalARow(%d) >= M) ? %s : " % ( a, zeroString )
    kStr += "A[ AinitOffset + GET_GLOBAL_INDEX_A( globalARow(%d), globalACol(%d) ) ];%s" % (a, a, endLine)
  if numALoadsR:
    kStr += "    if ( lIndex + " + str(numALoads) + "*WG_NUM_ROWS*WG_NUM_COLS < (WG_NUM_ROWS*MICRO_TILE_NUM_ROWS*NUM_UNROLL_ITER) ) {" + endLine
    kStr += "      lA[GET_LOCAL_INDEX_A(localARow, localACol)+ %d*localAStride ] = " % numALoads
    if kernel.isRowKernel():
      kStr += "( globalARow(%d) >= M) ? %s : " % ( numALoads, zeroString )
    kStr += "A[ AinitOffset + GET_GLOBAL_INDEX_A( globalARow(%d), globalACol(%d) ) ];%s" % (numALoads, numALoads, endLine)
    kStr += "    }" + endLine

  for b in range(0, int(numBLoads)):
    kStr += "    lB[ GET_LOCAL_INDEX_B(localBRow, localBCol) + %d*localBStride ] = " % b
    if kernel.isColKernel():
      kStr += "( globalBCol(%d) >= N) ? %s : " % ( b, zeroString )
    kStr += "B[ BinitOffset + GET_GLOBAL_INDEX_B( globalBRow(%d), globalBCol(%d) ) ];%s" % (b, b, endLine)
  if numBLoadsR:
    kStr += "    if ( lIndex + " + str(numBLoads) + "*WG_NUM_ROWS*WG_NUM_COLS < (WG_NUM_COLS*MICRO_TILE_NUM_COLS*NUM_UNROLL_ITER) ) {" + endLine
    kStr += "      lB[ GET_LOCAL_INDEX_B(localBRow, localBCol) + %d*localBStride ] = " % numBLoads
    if kernel.isColKernel():
      kStr += "(globalBCol(%d) >= N) ? %s : " % ( numBLoads, zeroString )
    kStr += "B[ BinitOffset + GET_GLOBAL_INDEX_B( globalBRow(%d), globalBCol(%d) ) ];%s" % (numBLoads, numBLoads, endLine)
    kStr += "    }" + endLine
  kStr += (
    "    tidx.barrier.wait();" + endLine +
    "    uint offA = idx;" + endLine +
    "    uint offB = idy;" + endLine )

  ####################################
  # do mads
  kStr += endLine
  kStr += "    /* do mads */" + endLine
  for u in range(0, int(kernel.unroll)):
    kStr += "    MICRO_TILE" + endLine

  ####################################
  # shift to next k block
  kStr += endLine
  kStr += "    /* shift to next k block */" + endLine
  if (kernel.order=="clblasColumnMajor")==(kernel.transA=="N"):
    kStr += "    AinitOffset += lda*NUM_UNROLL_ITER;" + endLine
  else:
    kStr += "    AinitOffset += NUM_UNROLL_ITER;" + endLine
  if (kernel.order=="clblasColumnMajor")==(kernel.transB=="N"):
    kStr += "    BinitOffset += NUM_UNROLL_ITER;" + endLine
  else:
    kStr += "    BinitOffset += ldb*NUM_UNROLL_ITER;" + endLine

  ####################################
  # end loop
  kStr += endLine
  kStr += "  } while (--block_k > 0);" + endLine
  kStr += endLine

  ####################################
  # which global Cij index
  kStr += endLine
  kStr += "  /* which global Cij index */" + endLine
  kStr += "  uint globalCRow = gidx * MACRO_TILE_NUM_ROWS + idx;" + endLine
  kStr += "  uint globalCCol = gidy * MACRO_TILE_NUM_COLS + idy;" + endLine

  ####################################
  # write global Cij
  kStr += endLine
  kStr += "  /* write global Cij */" + endLine
  if kernel.precision=="c":
    kStr += "  float type_mad_tmp;" + endLine
  if kernel.precision=="z":
    kStr += "  double type_mad_tmp;" + endLine

  for a in range(0, int(kernel.microTileNumRows)):
    for b in range(0, int(kernel.microTileNumCols)):
      if kernel.isRowKernel():
        kStr += "  if (globalCRow+%d*WG_NUM_ROWS < M)" % a
      if kernel.isColKernel():
        kStr += "  if (globalCCol+%d*WG_NUM_COLS < N)" % b
      if kernel.isRowKernel() or kernel.isColKernel():
        kStr += "{"
      kStr += "  TYPE_MAD_WRITE( C[ GET_GLOBAL_INDEX_C( globalCRow+%d*WG_NUM_ROWS, globalCCol+%d*WG_NUM_COLS) ], alpha, rC[%d][%d], beta )" % (a, b, a, b)
      if kernel.isRowKernel() or kernel.isColKernel():
        kStr += "}"
      kStr += endLine

  ####################################
  # end parallel for each
  kStr += endLine 
  kStr += "  }).wait();" + endLine
  kStr += "  return HCBLAS_SUCCEEDS;" + endLine 
 
  ####################################
  # end kernel
  kStr += endLine
  kStr += "}" + endLine

  return kStr


##############################################################################
# Write OpenCL kernel to file
##############################################################################
def writeOpenCLKernelToFile(kernel):
  kernelName = kernel.getName()
  kernelString = makeOpenCLKernelString(kernel)
  kernelFileName = Common.getKernelSourcePath() + kernelName +"_src.cpp"
  kernelFile = open(kernelFileName, "w")
  kernelFile.write( Common.getAutoGemmHeader() )
  kernelFile.write("#ifndef KERNEL_" + kernelName.upper() + "_SRC_H\n")
  kernelFile.write("#define KERNEL_" + kernelName.upper() + "_SRC_H\n")
  kernelFile.write(kernelString)
  kernelFile.write("#endif\n")
  kernelFile.close()


##############################################################################
# Write OpenCL kernel to file
##############################################################################
def writeOpenCLKernels():

  if not os.path.exists( Common.getKernelSourcePath() ):
    os.makedirs( Common.getKernelSourcePath() )
  if not os.path.exists( Common.getKernelBinaryPath() ):
    os.makedirs( Common.getKernelBinaryPath() )

  numKernels = 0
  # for each precision
  kernel = KernelParameters.KernelParameters()
  for precision in AutoGemmParameters.precisions:
    kernel.precision = precision

    # valid tiles for this precision
    tiles = AutoGemmParameters.getTilesForPrecision(precision)

    # for non tile parameters
    for order in AutoGemmParameters.orders:
      kernel.order = order
      for transA in AutoGemmParameters.transposes[precision]:
        kernel.transA = transA
        for transB in AutoGemmParameters.transposes[precision]:
          kernel.transB = transB
          for beta in AutoGemmParameters.betas:
            kernel.beta = beta

            # for tile parameters
            for tile in tiles:
              # tile kernel
              kernel.useTile(tile)
              writeOpenCLKernelToFile(kernel)
              # row kernel
              rowKernel = copy.copy(kernel)
              rowKernel.macroTileNumRows = 1
              writeOpenCLKernelToFile(rowKernel)
              # col kernel
              colKernel = copy.copy(kernel)
              colKernel.macroTileNumCols = 1
              writeOpenCLKernelToFile(colKernel)
              # corner kernel
              cornerKernel = copy.copy(kernel)
              cornerKernel.macroTileNumRows = 1
              cornerKernel.macroTileNumCols = 1
              writeOpenCLKernelToFile(cornerKernel)
              numKernels += 4
  print("AutoGemm.py: generated %d kernels" % numKernels)



################################################################################
# Main
################################################################################
if __name__ == "__main__":
  ap = argparse.ArgumentParser(description="Kernelgemm")
  ap.add_argument("precision", choices=["s","d","c","z"], help="precision" )
  ap.add_argument("order", choices=["row","col"], help="order: row major or column major" )
  ap.add_argument("transA", choices=["N","T", "C"], help="transA" )
  ap.add_argument("transB", choices=["N","T", "C"], help="transB" )
  ap.add_argument("beta", choices=[0, 1], type=int, help="0 for beta is zero, 1 for beta is non-zero" )
  ap.add_argument("workGroupNumRows", type=int )
  ap.add_argument("workGroupNumCols", type=int )
  ap.add_argument("microTileNumRows", type=int )
  ap.add_argument("microTileNumCols", type=int )
  ap.add_argument("unroll", type=int, help="number of iterations to unroll the loop over k" )
  ap.add_argument("outputPath", default=".", help="output path; %s will be appended to path" % Common.getRelativeKernelSourcePath() )

  args = ap.parse_args()

  kernel = KernelParameters.KernelParameters()
  kernel.precision = args.precision
  if args.order == "col":
    kernel.order = "clblasColumnMajor"
  else:
    kernel.order = "clblasRowMajor"
  kernel.transA = args.transA
  kernel.transB = args.transB
  kernel.beta = args.beta
  kernel.workGroupNumRows = args.workGroupNumRows
  kernel.workGroupNumCols = args.workGroupNumCols
  kernel.microTileNumRows = args.microTileNumRows
  kernel.microTileNumCols = args.microTileNumCols
  kernel.unroll = args.unroll
  Common.setOutputPath(args.outputPath)

  kernel.macroTileNumRows = kernel.workGroupNumRows * kernel.microTileNumRows
  kernel.macroTileNumCols = kernel.workGroupNumCols * kernel.microTileNumCols

  if not os.path.exists( Common.getKernelSourcePath() ):
    os.makedirs( Common.getKernelSourcePath() )

  writeOpenCLKernelToFile(kernel)

  kernelName = kernel.getName()
  kernelFileName = Common.getKernelSourcePath() + kernelName +"_src.cpp"
  print("kernel \"%s\" written to %s" % (kernelName, kernelFileName))
