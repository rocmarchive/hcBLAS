# ** C++ AMP BLAS ** #

##Introduction: ##

This repository hosts the C++ AMP implementation of BLAS subroutines. The following are the sub-routines that are implemented

1. Sgemm : Single Precision real valued general matrix-matrix multiplication
2. Cgemm : Complex valued general matrix matrix multiplication
2. Sgemv : Single Precision real valued general matrix-vector multiplication
3. Sger : Single Precision General matrix rank 1 operation
4. Saxpy : Scale vector X and add to vector Y


##Repository Structure: ##

##Prerequisites: ##
* **dGPU**:  AMD firepro S9150
* **OS** : Ubuntu 14.04 LTS
* **Ubuntu Pack**: libc6-dev-i386
* **AMD APP SDK** : Ver 2.9.1 launched on 18/8/2014 from [here](http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/)
* **AMD Driver installer**: amd-driver-installer-14.301.1001-x86.x86_64


## Installation Steps:    

### A. C++ AMP Compiler Installation: 

Make sure the parent directory chosen is say ~/ or any other folder of your choice. Lets take ~/ as an example

  (a) Prepare a directory for work space

       * mkdir ~/mcw_cppamp

       * cd ~/mcw_cppamp 
   
       * git clone https://bitbucket.org/multicoreware/cppamp-driver-ng.git src

       * cd ~/mcw_cppamp/src/

       * git checkout origin/torch-specific

  (b) Create a build directory and configure using CMake.

       * mkdir ~/mcw_cppamp/build

       * cd ~/mcw_cppamp/build

       * export CLAMP_NOTILECHECK=ON

       * cmake ../src -DCMAKE_BUILD_TYPE=Release -DCXXAMP_ENABLE_BOLT=ON -DOPENCL_HEADER_DIR=<path to SDK's OpenCL headers> -DOPENCL_LIBRARY_DIR=<path to SDK's OpenCL library> 
  
       * For example, cmake ../src -DCMAKE_BUILD_TYPE=Release -DCXXAMP_ENABLE_BOLT=ON  -DOPENCL_HEADER_DIR=/opt/AMDAPPSDK-3.0.0-Beta/include/CL -DOPENCL_LIBRARY_DIR=/opt/AMDAPPSDK-3.0-0-Beta/lib/x86_64


  (c) Build AMP

       * cd ~/mcw_cppamp/build

       * make [-j #] world && make          (# is the number of parallel builds. Generally it is # of CPU cores)

With this the C++ AMP Compiler installation is complete.

### B. AMPBLAS Installation 

(i) Clone MCW AMPBLAS source codes

   * cd ~/
   
   * git clone https://bitbucket.org/multicoreware/ampblas.git 

   * cd ~/ampblas

   * git checkout Development
   

(ii) Platform-specific build

(a) For Linux:  

     * cd ~/ampblas/Build/linux

     * sh build.sh

     * make

(b)  For Windows: (Prerequisite: Visual Studio 12 version )

1. For 32 Bit:

     * cd Build/vc11-x86

     * make-solutions.bat (This creates a Visual studio solution for ampblas Library) 

 2. For 64-bit:

     * cd Build/vc11-x86_64

     * make-solutions.bat (This creates a Visual Studio solution for ampblas Library)