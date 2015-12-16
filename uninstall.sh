#!/bin/bash
CURRENTDIR=$PWD
cd $CURRENTDIR/lib/build/linux/
rm -rf CMake*
rm Makefile
rm cmake*
rm libhcblas.so
rm -rf blas
rm install_manifest.txt
rm -rf include
cd $CURRENTDIR/test/build/linux/
rm -rf CMake*
rm Makefile
rm cmake*
rm -rf bin/
cd $CURRENTDIR/test/benchmark/
rm profileSummary*
rm -rf *ProfileData/
sudo rm /usr/lib/libhcblas.so
sudo rm /usr/local/include/hcblas.h
DIRECTORY=$CURRENTDIR/lib/build/linux/CMakeFiles
if [ -d "$DIRECTORY" ]; then
  cd $CURRENTDIR/lib/build/linux/
  sudo rm -rf CMakeFiles
fi
