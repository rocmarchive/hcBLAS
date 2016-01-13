#!/bin/bash -e
#This script is invoked to install the hcblas library and test sources

# CHECK FOR COMPILER PATH

if [ ! -z $MCWHCCBUILD ];
then
  if [ -x "$MCWHCCBUILD/compiler/bin/clang++" ];
  then
    cmake_c_compiler="$MCWHCCBUILD/compiler/bin/clang"
    cmake_cxx_compiler="$MCWHCCBUILD/compiler/bin/clang++"
  fi

elif [ -x "/opt/hcc/bin/clang++" ];
then
  cmake_c_compiler="/opt/hcc/bin/clang"
  cmake_cxx_compiler="/opt/hcc/bin/clang++"
else
  echo "Clang compiler not found"
  exit 1
fi

#CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

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

set +e
# MAKE BUILD DIR
mkdir $current_work_dir/build
mkdir $current_work_dir/build/test
set -e

# SET BUILD DIR
build_dir=$current_work_dir/build

# change to library build
cd $build_dir

# Cmake and make libhcRNG: Install hcRNG
cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir
sudo make install


# Build Tests
cd $build_dir/test/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir/test/

set +e
mkdir $current_work_dir/build/test/src/bin/
#mkdir $current_work_dir/build/test/example/bin/
mkdir $current_work_dir/build/test/unit/gtest/bin/
set -e

make
red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

#Various possibilities of test and profile arguments
#Test=OFF and Profile=OFF (Build library and tests)
if (([ "$var1" = "test=off" ] && [ "$var2" = "profile=off" ]) || ([ "$var1" = "profile=off" ] && [ "$var2" = "test=off" ])); then
   echo "${green}HCBLAS Installation Completed!${reset}"
#Test=ON and Profile=OFF (Build and test the library)
elif (([ "$var1" = "test=on" ] && [ "$var2" = "profile=off" ]) || ([ "$var1" = "profile=off" ] && [ "$var2" = "test=on" ])); then
   cd $current_work_dir/test/unit/
# Invoke test script 
   ./test.sh
# Test=ON and Profile=ON (Build, test and profile the library)
elif (([ "$var1" = "profile=on" ] && [ "$var2" = "test=on" ]) || ([ "$var1" = "test=on" ] && [ "$var2" = "profile=on" ])); then
   cd $current_work_dir/test/unit/
#Invoke test script
   ./test.sh
   cd $current_work_dir/test/benchmark/
#Invoke profiling script
   ./runme.sh
# Test=OFF and Profile=ON (Build and profile the library)
elif (([ "$var1" = "profile=on" ] && [ "$var2" = "test=off" ]) || ([ "$var1" = "test=off" ] && [ "$var2" = "profile=on" ])); then
   cd $current_work_dir/test/benchmark/
#Invoke profiling script
   ./runme.sh
fi 

