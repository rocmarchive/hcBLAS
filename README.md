# ** HCBLAS ** #

##Introduction: ##

This repository hosts the C++ AMP implementation of BLAS subroutines. The following are the sub-routines that are implemented

1. Sgemm  : Single Precision real valued general matrix-matrix multiplication
2. Cgemm  : Complex valued general matrix matrix multiplication
3. Sgemv  : Single Precision real valued general matrix-vector multiplication
4. Sger   : Single Precision General matrix rank 1 operation
5. Saxpy  : Scale vector X and add to vector Y
6. Sscal  : Single Precision scaling of Vector X 
7. Dscal  : Double Precision scaling of Vector X
8. Scopy  : Single Precision Copy 
9. Dcopy  : Double Precision Copy
10. Sasum : Single Precision sum of Absolute values
11. Dasum : Double Precision sum of Absolute values
12. Sdot  : Single Precision Dot product
13. Ddot  : Double Precision Dot product

##Repository Structure: ##

##Prerequisites: ##
* **dGPU**:  AMD firepro S9150
* **OS** : Ubuntu 14.04 LTS
* **Ubuntu Pack**: libc6-dev-i386
* **AMD APP SDK** : Ver 2.9.1 launched on 18/8/2014 from [here](http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/)
* **AMD Driver installer**: amd-driver-installer-14.301.1001-x86.x86_64


## Installation Steps:    

### A. C++ AMP Compiler Installation: 
 
## (1) Array-View Version: --- Branch: Development

Make sure the parent directory chosen is say ~/ or any other folder of your choice. Lets take ~/ as an example

  (a) Prepare a directory for work space

       * mkdir ~/mcw_cppamp

       * cd ~/mcw_cppamp 
   
       * git clone https://bitbucket.org/multicoreware/cppamp-driver-ng.git src

       * cd ~/mcw_cppamp/src/

       * git checkout origin/torch-specific
         (Stable commit  -  ba9555e)
         
  (b) Create a build directory and configure using CMake.

       * mkdir ~/mcw_cppamp/build

       * cd ~/mcw_cppamp/build

       * export CLAMP_NOTILECHECK=ON

       * cmake ../src -DCMAKE_BUILD_TYPE=Release -DCXXAMP_ENABLE_BOLT=ON -DOPENCL_HEADER_DIR=<path to SDK's OpenCL headers> -DOPENCL_LIBRARY_DIR=<path to SDK's OpenCL library> 
  
       * For example, cmake ../src -DCMAKE_BUILD_TYPE=Release -DCXXAMP_ENABLE_BOLT=ON  -DOPENCL_HEADER_DIR=/opt/AMDAPPSDK-3.0.0-Beta/include/CL -DOPENCL_LIBRARY_DIR=/opt/AMDAPPSDK-3.0-0-Beta/lib/x86_64


  (c) Build AMP

       * cd ~/mcw_cppamp/build

       * make [-j #] world && make          (# is the number of parallel builds. Generally it is # of CPU cores)

## (2) Array Version ( KALMAR): --- Branch : Array_KALMAR_wrapper

Make sure the parent directory chosen is say ~/ or any other folder of your choice. Lets take ~/ as an example

  (a) Prepare a directory for work space

       * mkdir ~/mcw_cppamp_kalmar

       * cd ~/mcw_cppamp_kalmar 
   
       * git clone https://bitbucket.org:/multicoreware/cppamp-driver-ng-35.git src

       * cd ~/mcw_cppamp_kalmar/src/

       * git checkout origin/master
         (Stable commit  -   4fb5922)
         
  (b) Create a build directory and configure using CMake.

       * mkdir ~/mcw_cppamp_kalmar/build

       * cd ~/mcw_cppamp_kalmar/build

       * export CLAMP_NOTILECHECK=ON

       * cmake ../src -DCMAKE_BUILD_TYPE=Release -DCXXAMP_ENABLE_BOLT=ON -DOPENCL_HEADER_DIR=<path to SDK's OpenCL headers> -DOPENCL_LIBRARY_DIR=<path to SDK's OpenCL library> 
  
       * For example, cmake ../src -DCMAKE_BUILD_TYPE=Release -DCXXAMP_ENABLE_BOLT=ON  -DOPENCL_HEADER_DIR=/opt/AMDAPPSDK-3.0.0-Beta/include/CL -DOPENCL_LIBRARY_DIR=/opt/AMDAPPSDK-3.0-0-Beta/lib/x86_64


  (c) Build AMP

       * cd ~/mcw_cppamp_kalmar/build

       * make [-j #] world && make          (# is the number of parallel builds. Generally it is # of CPU cores)

With this the C++ AMP Compiler installation is complete.

### B. HCBLAS Installation 

(i) Clone MCW HCBLAS source codes

       * cd ~/
   
       * git clone https://bitbucket.org/multicoreware/ampblas.git 

       * cd ~/ampblas

       * git checkout Development
   

(ii) Platform-specific build

(a) For Linux:  

       * cd ~/ampblas/Build/linux
       
       * export MCWCPPAMPBUILD=<path_to>/<mcw_cppamp or mcw_cppamp_kalmar>/build (Here path_to points to parent folder of mcw_cppamp or mcw_cppamp_kalmar. ~/ in our case)

       * sh build.sh

       * make

(b)  For Windows: (Prerequisite: Visual Studio 12 version )

1. For 32 Bit:

       * cd ~/ampblas/Build/vc11-x86

       * make-solutions.bat (This creates a Visual studio solution for ampblas Library) 

 2. For 64-bit:

       * cd ~/ampblas/Build/vc11-x86_64

       * make-solutions.bat (This creates a Visual Studio solution for ampblas Library)
       

### C. Unit testing

1. Install CBLAS CPU library: (Prerequisite: libblas-dev ubuntu package)
     
      * cd ~/
      
      * wget http://www.netlib.org/blas/blast-forum/cblas.tgz.
     
      * tar -xvzf cblas.tgz
      
      * cd ~/CBLAS
      
      * rm Makefile.in
      
      * ln -s Makefile.LINUX Makefile.in
      
      * make
      
2. Sgemm Testing: 

     * export HCBLAS_LIBRARY_PATH = ~/ampblas/Build/linux
     
     * export CBLASPATH=~/CBLAS
     
     * export LD_LIBRARY_PATH=$HCBLAS_LIBRARY_PATH:$LD_LIBRARY_PATH
     
     * cd ~/ampblas/source/test/sgemm_test/
     
     * make
     
     * To test ./sgemm M N K TA TB Itype (Here TA, TB take binary values 0 or 1 while Implementation type takes 1, 2 or 3)