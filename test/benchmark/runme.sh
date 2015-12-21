#!/bin/bash -e
#This script is invoked to profile major five kernels of the library

#CURRENT_WORK_DIRECTORY
CURRENTDIR=$PWD

#Export HCBLAS_PATH to be used by python scripts
export HCBLAS_PATH=$CURRENTDIR/../../

#Move to library build
cd $CURRENTDIR/../../lib/build/linux/
sh clean.sh

#Invoke build script
sh build.sh

#Install the library
sudo make install

#Move to test build
cd $CURRENTDIR/../build/linux/
sh clean.sh

#Invoke build script and enable PROFILE
sh build.sh PROFILE=ON

#Build the tests
make

#Move to test benchmark 
cd $CURRENTDIR

#Profile SGEMM
./runmesgemm.sh

#Profile SAXPY
./runmesaxpy.sh

#Profile SGER
./runmesger.sh

#Profile CGEMM
./runmecgemm.sh

#Profile SGEMV
./runmesgemv.sh

echo "HCBLAS Profiling Completed!"
