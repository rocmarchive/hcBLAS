#!/bin/bash
#This script is invoked to test all generators of the hcblas library 
#Preliminary version

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

# Move to src test bin
working_dir="$current_work_dir/../../build/test/src/bin/"
cd $working_dir

#Temporary log file to compare with original log file
echo "TEST PASSED" >> testlog_temp.txt

#Path to executables
path2sgemm="$working_dir/sgemm"
path2cgemm="$working_dir/cgemm"
path2sgemv="$working_dir/sgemv"
path2sger="$working_dir/sger"
path2saxpy="$working_dir/saxpy"
path2sscal="$working_dir/sscal"
path2dscal="$working_dir/dscal"
path2scopy="$working_dir/scopy"
path2dcopy="$working_dir/dcopy"
path2sasum="$working_dir/sasum"
path2dasum="$working_dir/dasum"
path2sdot="$working_dir/sdot"
path2ddot="$working_dir/ddot"

#SGEMM TEST
while read line; do
    Mvalue=$(echo $line | cut -f1 -d" " )
    Nvalue=$(echo $line | cut -f2 -d" " )
    Kvalue=$(echo $line | cut -f3 -d" " )
    transA=$(echo $line | cut -f4 -d" " )
    transB=$(echo $line | cut -f5 -d" " )
    Implem=$(echo $line | cut -f6 -d" " )
    if [ -x $path2sgemm ]; then
      runcmd="$path2sgemm $Mvalue $Nvalue $Kvalue $transA $transB $Implem >> testlog.txt"
      eval $runcmd
    else
      echo $path2sgemm "doesnot exist"
    fi

#Input file for SGEMM
done < $current_work_dir/sgemm_input.txt

#CGEMM TEST
while read line; do
    Mvalue=$(echo $line | cut -f1 -d" " )
    Nvalue=$(echo $line | cut -f2 -d" " )
    Kvalue=$(echo $line | cut -f3 -d" " )
    transA=$(echo $line | cut -f4 -d" " )
    transB=$(echo $line | cut -f5 -d" " )
    Implem=$(echo $line | cut -f6 -d" " )
    if [ -x $path2cgemm ]; then
      runcmd="$path2cgemm $Mvalue $Nvalue $Kvalue $transA $transB $Implem >> testlog.txt"
      eval $runcmd
    else
      echo $path2cgemm "doesnot exist"
    fi

#Input file for CGEMM
done < $current_work_dir/sgemm_input.txt                              

#SGEMV TEST
while read line; do
    Mvalue=$(echo $line | cut -f1 -d" " )
    Nvalue=$(echo $line | cut -f2 -d" " )
    transA=$(echo $line | cut -f3 -d" " )
    Implem=$(echo $line | cut -f4 -d" " )
    if [ -x $path2sgemv ]; then
      runcmd="$path2sgemv $Mvalue $Nvalue $transA $Implem >> testlog.txt"
      eval $runcmd
    else
      echo $path2sgemv "doesnot exist"
    fi

#Input file for SGEMV
done < $current_work_dir/sgemv_input.txt

#SGER TEST
while read line; do
    Mvalue=$(echo $line | cut -f1 -d" " )
    Nvalue=$(echo $line | cut -f2 -d" " )
    Implem=$(echo $line | cut -f3 -d" " )
    if [ -x $path2sger ]; then
      runcmd="$path2sger $Mvalue $Nvalue $Implem >> testlog.txt"
      eval $runcmd
    else
      echo $path2sger "doesnot exist"
    fi

#Input file for SGER
done < $current_work_dir/sger_input.txt

#SAXPY, (D/S)SCAL, (D/S)COPY, (D/S)ASUM, (D/S)DOT TEST
while read line; do
    Mvalue=$(echo $line | cut -f1 -d" " )
    Implem=$(echo $line | cut -f2 -d" " )
    if ([ -x $path2saxpy ] || [ -x $path2sscal ] || [ -x $path2dscal ] || [ -x $path2scopy ] || [ -x $path2dcopy ] || [ -x $path2sasum ] || [ -x $path2dasum ] || [ -x $path2sdot ] || [ -x $path2ddot ]); then
      runcmd1="$path2saxpy $Mvalue $Implem >> testlog.txt"
      eval $runcmd1
      runcmd2="$path2sscal $Mvalue $Implem >> testlog.txt"
      eval $runcmd2
      runcmd3="$path2dscal $Mvalue $Implem >> testlog.txt"
      eval $runcmd3
      runcmd4="$path2scopy $Mvalue $Implem >> testlog.txt"
      eval $runcmd4
      runcmd5="$path2dcopy $Mvalue $Implem >> testlog.txt"
      eval $runcmd5
      runcmd6="$path2sasum $Mvalue $Implem >> testlog.txt"
      eval $runcmd6
      runcmd7="$path2dasum $Mvalue $Implem >> testlog.txt"
      eval $runcmd7
      runcmd8="$path2sdot $Mvalue $Implem >> testlog.txt"
      eval $runcmd8
      runcmd9="$path2ddot $Mvalue $Implem >> testlog.txt"
      eval $runcmd9 
    else
      echo "Executables doesnot exist"
    fi

#Input file for SAXPY, (D/S)SCAL, (D/S)COPY, (D/S)ASUM, (D/S)DOT
done < $current_work_dir/saxpy_input.txt

#All logs are appended to testlog file 
#Adding TEST PASSED to the log file
echo "TEST PASSED" >> testlog.txt
red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

#Difference between temporary log file and test log file is null
#when all tests are passed 
DIFF=$(diff testlog.txt testlog_temp.txt)
if [ "$DIFF" != "" ] 
then
    echo "${red}Functionality check ----- [ FAILED ]${reset}"
    rm testlog_temp.txt
else
    echo "${green}Functionality check ----- [ PASSED ]${reset}"
    rm testlog*
fi

# Move to gtest bin
working_dir1="$current_work_dir/../../build/test/unit/gtest/bin/"
cd $working_dir1

#Gtest functions
unittest="$working_dir1/unittest"

runcmd1="$unittest >> gtestlog.txt"
eval $runcmd1

Log_file="$working_dir1/gtestlog.txt"
if grep -q FAILED "$Log_file";
then
    echo "${red}GTEST               ----- [ FAILED ]${reset}"
else
    echo "${green}GTEST               ----- [ PASSED ]${reset}"
    rm $working_dir1/gtestlog.txt
fi


