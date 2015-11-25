#!/bin/bash

cd $HCBLAS_PATH/Build/linux/
sh clean.sh
sh build.sh
make install
cd $HCBLAS_PATH/source/testfiles/build/linux/
sh clean.sh
sh build.sh
make
cd $HCBLAS_PATH/profile/
./runmesgemm.sh
./runmesaxpy.sh
./runmesger.sh
./runmecgemm.sh
./runmesgemv.sh
