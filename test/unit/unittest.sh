#!/bin/bash

cd $HCBLAS_PATH/Build/linux/
sh clean.sh
sh build.sh
sudo make install
cd $HCBLAS_PATH/source/testfiles/build/linux/
sh clean.sh
sh build.sh
make
cd $HCBLAS_PATH/source/testfiles/Unit_tests/
path2sgemm="$HCBLAS_PATH/source/testfiles/build/linux/bin/sgemm"
path2cgemm="$HCBLAS_PATH/source/testfiles/build/linux/bin/cgemm"
path2sgemv="$HCBLAS_PATH/source/testfiles/build/linux/bin/sgemv"
path2sger="$HCBLAS_PATH/source/testfiles/build/linux/bin/sger"
path2saxpy="$HCBLAS_PATH/source/testfiles/build/linux/bin/saxpy"
path2sscal="$HCBLAS_PATH/source/testfiles/build/linux/bin/sscal"
path2dscal="$HCBLAS_PATH/source/testfiles/build/linux/bin/dscal"
path2scopy="$HCBLAS_PATH/source/testfiles/build/linux/bin/scopy"
path2dcopy="$HCBLAS_PATH/source/testfiles/build/linux/bin/dcopy"
path2sasum="$HCBLAS_PATH/source/testfiles/build/linux/bin/sasum"
path2dasum="$HCBLAS_PATH/source/testfiles/build/linux/bin/dasum"
path2sdot="$HCBLAS_PATH/source/testfiles/build/linux/bin/sdot"
path2ddot="$HCBLAS_PATH/source/testfiles/build/linux/bin/ddot"
workingdir="$HCBLAS_PATH/source/testfiles/Unit_tests/"

while read line; do
    Mvalue=$(echo $line | cut -f1 -d" " )
    Nvalue=$(echo $line | cut -f2 -d" " )
    Kvalue=$(echo $line | cut -f3 -d" " )
    transA=$(echo $line | cut -f4 -d" " )
    transB=$(echo $line | cut -f5 -d" " )
    Implem=$(echo $line | cut -f6 -d" " )
    if [ -x $path2sgemm ]; then
      runcmd="$path2sgemm $Mvalue $Nvalue $Kvalue $transA $transB $Implem"
      echo $runcmd
      eval $runcmd
    else
      echo $path2sgemm "doesnot exist"
    fi
done < $workingdir/sgemm_input.txt

while read line; do
    Mvalue=$(echo $line | cut -f1 -d" " )
    Nvalue=$(echo $line | cut -f2 -d" " )
    Kvalue=$(echo $line | cut -f3 -d" " )
    transA=$(echo $line | cut -f4 -d" " )
    transB=$(echo $line | cut -f5 -d" " )
    Implem=$(echo $line | cut -f6 -d" " )
    if [ -x $path2cgemm ]; then
      runcmd="$path2cgemm $Mvalue $Nvalue $Kvalue $transA $transB $Implem"
      echo $runcmd
      eval $runcmd
    else
      echo $path2cgemm "doesnot exist"
    fi
done < $workingdir/sgemm_input.txt                              

while read line; do
    Mvalue=$(echo $line | cut -f1 -d" " )
    Nvalue=$(echo $line | cut -f2 -d" " )
    transA=$(echo $line | cut -f3 -d" " )
    Implem=$(echo $line | cut -f4 -d" " )
    if [ -x $path2sgemv ]; then
      runcmd="$path2sgemv $Mvalue $Nvalue $transA $Implem"
      echo $runcmd
      eval $runcmd
    else
      echo $path2sgemv "doesnot exist"
    fi
done < $workingdir/sgemv_input.txt

while read line; do
    Mvalue=$(echo $line | cut -f1 -d" " )
    Nvalue=$(echo $line | cut -f2 -d" " )
    Implem=$(echo $line | cut -f3 -d" " )
    if [ -x $path2sger ]; then
      runcmd="$path2sger $Mvalue $Nvalue $Implem"
      echo $runcmd
      eval $runcmd
    else
      echo $path2sger "doesnot exist"
    fi
done < $workingdir/sger_input.txt

while read line; do
    Mvalue=$(echo $line | cut -f1 -d" " )
    Implem=$(echo $line | cut -f2 -d" " )
    if ([ -x $path2saxpy ] || [ -x $path2sscal ] || [ -x $path2dscal ] || [ -x $path2scopy ] || [ -x $path2dcopy ] || [ -x $path2sasum ] || [ -x $path2dasum ] || [ -x $path2sdot ] || [ -x $path2ddot ]); then
      runcmd1="$path2saxpy $Mvalue $Implem"
      echo $runcmd1
      eval $runcmd1
      runcmd2="$path2sscal $Mvalue $Implem"
      echo $runcmd2
      eval $runcmd2
      runcmd3="$path2dscal $Mvalue $Implem"
      echo $runcmd3
      eval $runcmd3
      runcmd4="$path2scopy $Mvalue $Implem"
      echo $runcmd4
      eval $runcmd4
      runcmd5="$path2dcopy $Mvalue $Implem"
      echo $runcmd5
      eval $runcmd5
      runcmd6="$path2sasum $Mvalue $Implem"
      echo $runcmd6
      eval $runcmd6
      runcmd7="$path2dasum $Mvalue $Implem"
      echo $runcmd7
      eval $runcmd7
      runcmd8="$path2sdot $Mvalue $Implem"
      echo $runcmd8
      eval $runcmd8
      runcmd9="$path2ddot $Mvalue $Implem"
      echo $runcmd9
      eval $runcmd9 
    else
      echo "Executables doesnot exist"
    fi
done < $workingdir/saxpy_input.txt
