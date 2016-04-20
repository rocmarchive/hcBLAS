#!/bin/bash
#This script is invoked to test all generators of the hcblas library 
#Preliminary version

# CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

# Move to gtest bin
working_dir1="$current_work_dir/../../build/test/unit/gtest/bin/"
working_dir2="$current_work_dir/../../build/test/unit-api/bin/"
cd $working_dir1
rm $working_dir1/gtestlog.txt

#Gtest functions
unittest="$working_dir1/unittest"
unitapitest="$working_dir2/unit-api-test"
runcmd1="$unittest >> gtestlog.txt"
runcmd2="$unitapitest >> gtestlog.txt"

eval $runcmd1
eval $runcmd2

Log_file="$working_dir1/gtestlog.txt"

if [ ! -s "$Log_file" ]; then
   echo "${red}GTEST IS NOT WORKING....${reset}"
else
   if grep -q FAILED "$Log_file"; then
      echo "${red}GTEST               ----- [ FAILED ]${reset}"
   else
      echo "${green}GTEST               ----- [ PASSED ]${reset}"
      rm $working_dir1/gtestlog.txt
   fi
fi 
