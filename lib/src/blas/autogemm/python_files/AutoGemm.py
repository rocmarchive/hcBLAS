################################################################################
# AutoGemm
# - Automatically generate gemm kernels based on tile parameters
# - This script generates the following to ease integration into clBLAS:
#   - generate all the kernel files
#   - kernel selection logic
#   - include files for kernel strings
#
# TODO Now
# - offline compilation
# TODO Future
# - fuse together unroll=8 and unroll=1 in same kernel ?
#     functionally works fine, but lowers performance by ~10%
################################################################################

import os
import sys
import argparse
import getopt

import Common
import Includes
import KernelSelection
import AutoGemmParameters
import hcblasKernel 


################################################################################
# Main
################################################################################
if __name__ == "__main__":
  # parse arguments
  ap = argparse.ArgumentParser(description="AutoGemm")
  ap.add_argument("--output-path", dest="output" )
  args = ap.parse_args()
  if args.output:
    Common.setOutputPath(args.output)
  else:
    print("AutoGemm.py: Warning: No output path specified; default is working directory.")

  hcblasKernel.writeOpenCLKernels()
  KernelSelection.writeKernelSelection()
  Includes.writeIncludes()
