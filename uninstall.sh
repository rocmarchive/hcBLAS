#!/bin/bash
# This script is invoked to uninstall the hcblas library and test sources

# CURRENT_WORK_DIRECTORY
CURRENTDIR=$PWD

#Move to library build
cd $CURRENTDIR/lib/build/linux/

#Invoke clean script
sh clean.sh

#Move to test build
cd $CURRENTDIR/test/build/linux/

#Invoke clean script
sh clean.sh

#Move to test benchmark
cd $CURRENTDIR/test/benchmark/

#Remove profile data
rm profileSummary*
rm -rf *ProfileData/

#Remove installed libraries and headers
sudo rm /usr/lib/libhcblas.so
sudo rm /usr/local/include/hcblas.h

#Remove CMake progress folders
DIRECTORY=$CURRENTDIR/lib/build/linux/CMakeFiles
if [ -d "$DIRECTORY" ]; then
  cd $CURRENTDIR/lib/build/linux/
  sudo rm -rf CMakeFiles
fi
