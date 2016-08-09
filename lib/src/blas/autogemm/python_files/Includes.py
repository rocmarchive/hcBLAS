import os
import sys
import getopt
import Common
import AutoGemmParameters
import KernelParameters

################################################################################
# SINC - Kernel Source Includes
################################################################################
class KernelSourceIncludes:

  ##############################################################################
  # SINC - default constructor
  ##############################################################################
  def __init__(self):
    self.incFileName = Common.getIncludePath() + "AutoGemmKernelSources.h"
    self.incFile = open(self.incFileName, "w")
    self.incFile.write( Common.getAutoGemmHeader() )
    self.incStr = "#ifndef AUTOGEMM_KERNEL_SOURCE_INCLUDES_H\n"
    self.incStr += "#define AUTOGEMM_KERNEL_SOURCE_INCLUDES_H\n"
    self.incStr += "\n"

    self.cppFileName = Common.getIncludePath() + "AutoGemmKernelSources.cpp"
    self.cppFile = open(self.cppFileName, "w")
    self.cppFile.write( Common.getAutoGemmHeader() )
    self.cppStr  = "\n"
    self.cppStr += "#include \"hc.hpp\"\n"
    self.cppStr += "#include \"hc_math.hpp\"\n"
    self.cppStr += "#include \"hc_am.hpp\"\n"
    self.cppStr += "#include \"hcblaslib.h\"\n"
    self.cppStr += "#include \"%sAutoGemmKernelSources.h\"\n" % Common.getRelativeIncludePath()
# TODO : Add user gemm Kernels in hcBLAS
#    self.cppStr += "#include \"UserGemmKernelSources/UserGemmKernelSourceIncludes.cpp\"\n"
	#self.cppStr += "#include \"UserGemmKernelSources/UserGemmKernelSources.cpp\"\n"

  def addKernel(self, kernel):
    kernelName = kernel.getName()
    self.incStr += "extern const unsigned int %s_workGroupNumRows;\n" % kernelName
    self.incStr += "extern const unsigned int %s_workGroupNumCols;\n" % kernelName
    self.incStr += "extern const unsigned int %s_microTileNumRows;\n" % kernelName
    self.incStr += "extern const unsigned int %s_microTileNumCols;\n" % kernelName
    self.incStr += "extern const unsigned int %s_unroll;\n" % kernelName
    self.incStr += "extern const char * const %s_src;\n" % kernelName
    self.cppStr += "#include \"%s%s_src.cpp\"\n" % (Common.getRelativeKernelSourcePath(), kernelName)
    kernelName = kernel.getRowName()
    self.incStr += "extern const unsigned int %s_workGroupNumRows;\n" % kernelName
    self.incStr += "extern const unsigned int %s_workGroupNumCols;\n" % kernelName
    self.incStr += "extern const unsigned int %s_microTileNumRows;\n" % kernelName
    self.incStr += "extern const unsigned int %s_microTileNumCols;\n" % kernelName
    self.incStr += "extern const unsigned int %s_unroll;\n" % kernelName
    self.incStr += "extern const char * const %s_src;\n" % kernelName
    self.cppStr += "#include \"%s%s_src.cpp\"\n" % (Common.getRelativeKernelSourcePath(), kernelName )
    kernelName = kernel.getColName()
    self.incStr += "extern const unsigned int %s_workGroupNumRows;\n" % kernelName
    self.incStr += "extern const unsigned int %s_workGroupNumCols;\n" % kernelName
    self.incStr += "extern const unsigned int %s_microTileNumRows;\n" % kernelName
    self.incStr += "extern const unsigned int %s_microTileNumCols;\n" % kernelName
    self.incStr += "extern const unsigned int %s_unroll;\n" % kernelName
    self.incStr += "extern const char * const %s_src;\n" % kernelName
    self.cppStr += "#include \"%s%s_src.cpp\"\n" % (Common.getRelativeKernelSourcePath(), kernelName)
    kernelName = kernel.getCornerName()
    self.incStr += "extern const unsigned int %s_workGroupNumRows;\n" % kernelName
    self.incStr += "extern const unsigned int %s_workGroupNumCols;\n" % kernelName
    self.incStr += "extern const unsigned int %s_microTileNumRows;\n" % kernelName
    self.incStr += "extern const unsigned int %s_microTileNumCols;\n" % kernelName
    self.incStr += "extern const unsigned int %s_unroll;\n" % kernelName
    self.incStr += "extern const char * const %s_src;\n" % kernelName
    self.cppStr += "#include \"%s%s_src.cpp\"\n" % (Common.getRelativeKernelSourcePath(), kernelName)

    self.incFile.write( self.incStr )
    self.incStr = ""
    self.cppFile.write( self.cppStr )
    self.cppStr = ""

  def writeToFile(self):
    self.incFile.write( self.incStr )
    self.incFile.write( "\n#endif\n" )
    self.incFile.close()
    self.cppFile.write( self.cppStr )
    self.cppFile.close()



################################################################################
# CPPKE - Cpp Kernel enumeration
################################################################################
class CppKernelEnumeration:

  ##############################################################################
  # CPPKE - default constructor
  ##############################################################################
  def __init__(self):
    self.fileName = Common.getIncludePath() + "AutoGemmKernelEnumeration.h"
    self.kernelStr = ""
    self.tileStr = ""
    self.nonTileStr = ""
    self.kernelCount = 0
    self.tileCount = 0
    self.nonTileCount = 0
    self.precision = ""
    self.precisionInitialized = False

  def newPrecision(self, precision):
    if self.precisionInitialized:
      self.kernelStr += "};\n"
      self.kernelStr += "const unsigned int %sgemmNumKernels = %d;\n\n" \
          % (self.precision, self.kernelCount)
      self.tileStr += "};\n"
      self.tileStr += "const unsigned int %sgemmNumTiles = %d;\n\n" \
          % (self.precision, self.tileCount)
      self.nonTileStr += "};\n"
      self.nonTileStr += "const unsigned int %sgemmNumNonTiles = %d;\n\n" \
          % (self.precision, self.nonTileCount)
    self.precisionInitialized = True
    self.precision = precision

    self.kernelStr += "// order, transA, transB, beta, macroTileNumRows, macroTileNumCols, unroll, mSpill, nSpill\n"
    self.kernelStr += "unsigned int " + precision + "gemmKernelEnumeration[][9] = {\n"

    self.tileStr += "// macroTileNumRows, macroTileNumCols, unroll\n"
    self.tileStr += "unsigned int " + precision + "gemmTileEnumeration[][3] = {\n"

    self.nonTileStr += "// order, transA, transB, beta\n"
    self.nonTileStr += "unsigned int " + precision + "gemmNonTileEnumeration[][4] = {\n"
    self.tileCount = 0
    self.nonTileCount = 0
    self.kernelCount = 0

  def addTile(self, tile):
    self.tileStr += "  { %3u, %3u, %1u },\n" % ( \
        tile.macroTileNumRows, \
        tile.macroTileNumCols, \
        tile.unroll )
    self.tileCount += 1

  def addNonTile(self, nonTile):
    self.nonTileStr += "  { %1u, %1u, %1u, %1u },\n" % ( \
        1 if nonTile.order=="ColMajor" else 0, \
        0 if nonTile.transA=="N" else 1 if nonTile.transA=="T" else 2 , \
        0 if nonTile.transB=="N" else 1 if nonTile.transB=="T" else 2, \
        1 if nonTile.beta>0 else 0 )
    self.nonTileCount += 1

  def addKernel(self, kernel):
    # 6) list to add to ktest for automated kernel testing
    for mSpill in range(0, 2):
      for nSpill in range(0, 2):
        self.kernelStr += "  { %1u, %1u, %1u, %1u, %3u, %3u, %2u, %1u, %1u },\n" % ( \
          1 if kernel.order=="ColMajor" else 0, \
          0 if kernel.transA=="N" else 1 if kernel.transA=="T" else 2 , \
          0 if kernel.transB=="N" else 1 if kernel.transB=="T" else 2, \
          1 if kernel.beta>0 else 0, \
          kernel.macroTileNumRows, \
          kernel.macroTileNumCols, \
          kernel.unroll, \
          mSpill, \
          nSpill )
    self.kernelCount += 4

  def writeToFile(self):
    self.kernelStr += "};\n"
    self.kernelStr += "const unsigned int %sgemmNumKernels = %d;\n" % (self.precision, self.kernelCount)
    self.tileStr += "};\n"
    self.tileStr += "const unsigned int %sgemmNumTiles = %d;\n" % (self.precision, self.tileCount)
    self.nonTileStr += "};\n"
    self.nonTileStr += "const unsigned int %sgemmNumNonTiles = %d;\n" % (self.precision, self.nonTileCount)
    incFile = open(self.fileName, "w")
    incFile.write( Common.getAutoGemmHeader() )
    incFile.write( self.tileStr )
    incFile.write( "\n\n" )
    incFile.write( self.nonTileStr )
    incFile.write( "\n\n" )
    incFile.write( self.kernelStr )
    incFile.close()

################################################################################
# Write Includes
################################################################################
def writeIncludes():
  print("AutoGemm.py: Generating include files.")
  if not os.path.exists( Common.getIncludePath() ):
    os.makedirs( Common.getIncludePath() )

  kernelSourceIncludes     = KernelSourceIncludes()
  cppKernelEnumeration     = CppKernelEnumeration()


  # for each precision
  kernel = KernelParameters.KernelParameters()
  for precision in AutoGemmParameters.precisions:
    kernel.precision = precision
    cppKernelEnumeration.newPrecision(precision)

    # valid tiles for this precision
    tiles = AutoGemmParameters.getTilesForPrecision(precision)

    # add tiles for this precision to Cpp
    for tile in tiles:
      cppKernelEnumeration.addTile(tile)

    # for non tile parameters
    for order in AutoGemmParameters.orders:
      kernel.order = order
      for transA in AutoGemmParameters.transposes[precision]:
        kernel.transA = transA
        for transB in AutoGemmParameters.transposes[precision]:
          kernel.transB = transB
          for beta in AutoGemmParameters.betas:
            kernel.beta = beta

            # add this nonTile combo for this precision to Cpp
            cppKernelEnumeration.addNonTile(kernel)

            # for tile parameters
            for tile in tiles:
              kernel.useTile(tile)
              kernelSourceIncludes.addKernel(kernel)
              cppKernelEnumeration.addKernel(kernel)

  # save written files
  kernelSourceIncludes.writeToFile()
  cppKernelEnumeration.writeToFile()



################################################################################
# Main
################################################################################
if __name__ == "__main__":
  if len(sys.argv) == 2:
    Common.setOutputPath(sys.argv[1])
  else:
    print("Warning: No output path specified; default is working directory.")
  writeIncludes()
