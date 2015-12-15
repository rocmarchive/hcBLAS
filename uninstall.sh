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
