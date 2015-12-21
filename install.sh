#!/bin/bash -e
#This script is invoked to install the hcblas library and test sources
#CURRENT_WORK_DIRECTORY
CURRENTDIR=$PWD

#Inputs converted to smallcase format
input1=$1
var1=${input1,,}
input2=$2
var2=${input2,,}

#If number of arguments = 0
if [ "x$var1" = "x" ]; then
   var1="test=off"
   var2="profile=off"
fi

#If number of arguments = 1 
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

#Various possibilities of test and profile arguments
#Test=OFF and Profile=OFF (Build library and tests)
if (([ "$var1" = "test=off" ] && [ "$var2" = "profile=off" ]) || ([ "$var1" = "profile=off" ] && [ "$var2" = "test=off" ])); then
#Move to library build
   cd $CURRENTDIR/lib/build/linux/
   sh clean.sh

#Invoke build script
   sh build.sh

#Install the library
   sudo make install

#Move to test build
   cd $CURRENTDIR/test/build/linux/
   sh clean.sh

#Invoke build script
   sh build.sh 

#Build the test files
   make

#Test=ON and Profile=OFF (Build and test the library)
elif (([ "$var1" = "test=on" ] && [ "$var2" = "profile=off" ]) || ([ "$var1" = "profile=off" ] && [ "$var2" = "test=on" ])); then
   cd $CURRENTDIR/test/unit/
# Invoke test script 
   ./test.sh

# Test=ON and Profile=ON (Build, test and profile the library)
elif (([ "$var1" = "profile=on" ] && [ "$var2" = "test=on" ]) || ([ "$var1" = "test=on" ] && [ "$var2" = "profile=on" ])); then
   cd $CURRENTDIR/test/unit/
#Invoke test script
   ./test.sh
   cd $CURRENTDIR/test/benchmark/
#Invoke profiling script
   ./runme.sh

# Test=OFF and Profile=ON (Build and profile the library)
elif (([ "$var1" = "profile=on" ] && [ "$var2" = "test=off" ]) || ([ "$var1" = "test=off" ] && [ "$var2" = "profile=on" ])); then
   cd $CURRENTDIR/test/benchmark/
#Invoke profiling script
   ./runme.sh
fi 
echo "HCBLAS Installation Completed!"
