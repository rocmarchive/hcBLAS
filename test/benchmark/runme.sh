#!/bin/bash -e
CURRENTDIR=$PWD
export HCBLAS_PATH=$CURRENTDIR/../../
cd $CURRENTDIR/../../lib/build/linux/
sh clean.sh
sh build.sh
sudo make install
cd $CURRENTDIR/../build/linux/
sh clean.sh
sh build.sh PROFILE=ON
make
cd $CURRENTDIR
./runmesgemm.sh
./runmesaxpy.sh
./runmesger.sh
./runmecgemm.sh
./runmesgemv.sh
echo "HCBLAS Profiling Completed!"
