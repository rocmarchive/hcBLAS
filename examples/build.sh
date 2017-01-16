#!/bin/bash
#This script is invoked to test some example of hcblas library
#Preliminary version

if [ ! -z $HCC_HOME ];
then
  if [ -x "$HCC_HOME/compiler/bin/clang++" ];
  then
    platform="hcc"
    cmake_c_compiler="$HCC_HOME/bin/clang"
    cmake_cxx_compiler="$HCC_HOME/bin/clang++"
  fi

elif [ -x "/opt/rocm/hcc/bin/clang++" ];
then
  platform="hcc"
  cmake_c_compiler="/opt/rocm/hcc/bin/clang"
  cmake_cxx_compiler="/opt/rocm/hcc/bin/clang++"
else
  echo "Compiler Error"
  exit 1
fi


# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

set +e
# MAKE BUILD EXAMPLES DIR
mkdir $current_work_dir/../build/examples
set -e

# SET BUILD DIR
build_dir=$current_work_dir/../build

#EXAMPLES
cd $build_dir/examples/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir/
make -j$working_threads

test_example_file=($current_work_dir/*.cpp)
numtests=${#test_example_file[@]}

for (( i=0; i<numtests; i++ ));  
do
  test_examples=$(basename "${test_example_file[$i]}" .cpp)
  working_dir1="$current_work_dir/../build/examples/bin"
  cd $working_dir1
  if [ ! -d "errlog" ]; then
    mkdir "errlog"
  fi
  errlogdir="${working_dir1}/errlog"
  
  rm -f $working_dir1/examplelog.txt

  #Example functions
  example="${working_dir1}/${test_examples}"
  runcmd1="$example >> examplelog.txt"
  eval $runcmd1

  Log_file="$working_dir1/examplelog.txt"
  if [ ! -e "$Log_file" ]; then
    echo "${red}EXAMPLES ARE NOT WORKING....${reset}"
  else
    if grep -q Error "$Log_file";
    then
      echo "${red}${test_examples}            ----- [ FAILED ]${reset}"
      mv "${working_dir1}/examplelog.txt" "${errlogdir}/${test_examples}.txt" 
    else
      echo "${green}${test_examples}         ----- [ PASSED ]${reset}"
      rm -f $working_dir1/examplelog.txt
    fi
  fi
done

