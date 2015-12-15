#!/bin/bash -e
CURRENTDIR=$PWD
input1=$1
var1=${input1,,}
input2=$2
var2=${input2,,}
if [ "x$var1" = "x" ]; then
   var1="test=off"
   var2="profile=off"
fi
if ([ "$var1" = "test=on" ] || [ "$var1" = "test=off" ]); then 
   if [ "x$var2" = "x" ]; then 
    var2="profile=off" 
   fi
fi  
if ([ "$var1" = "profile=on" ] || [ "$var1" = "profile=off" ]); then
   if [ "x$var2" = "x" ]; then 
    var2="test=off"
   fi
fi
if (([ "$var1" = "test=off" ] && [ "$var2" = "profile=off" ]) || ([ "$var1" = "profile=off" ] && [ "$var2" = "test=off" ])); then
   cd $CURRENTDIR/lib/build/linux/
   sh clean.sh
   sh build.sh
   sudo make install
   cd $CURRENTDIR/test/build/linux/
   sh clean.sh
   sh build.sh 
   make
elif (([ "$var1" = "test=on" ] && [ "$var2" = "profile=off" ]) || ([ "$var1" = "profile=off" ] && [ "$var2" = "test=on" ])); then
   cd $CURRENTDIR/test/unit/
   ./test.sh
elif (([ "$var1" = "profile=on" ] && [ "$var2" = "test=on" ]) || ([ "$var1" = "test=on" ] && [ "$var2" = "profile=on" ])); then
   cd $CURRENTDIR/test/unit/
   ./test.sh
   cd $CURRENTDIR/test/benchmark/
   ./runme.sh
elif (([ "$var1" = "profile=on" ] && [ "$var2" = "test=off" ]) || ([ "$var1" = "test=off" ] && [ "$var2" = "profile=on" ])); then
   cd $CURRENTDIR/test/benchmark/
   ./runme.sh
fi 
echo "HCBLAS Installation Completed!"
