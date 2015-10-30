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
10. Sasum : Single Precision Absolute sum of values of a vector
11. Dasum : Double Precision Absolute sum of values of a vector
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

** Build from source **

  To build the compiler from source follow the steps given below,
 
  Make sure the parent directory chosen is say ~/ or any other folder of your choice. Lets take ~/ as an example

  (a) Prepare a directory for work space

       * mkdir ~/mcw_cppamp

       * cd ~/mcw_cppamp 
   
       * git clone https://bitbucket.org/multicoreware/cppamp-driver-ng-35.git src

       * cd ~/mcw_cppamp/src/

       * git checkout origin/master

  (b) Create a build directory and configure using CMake.

       * mkdir ~/mcw_cppamp/build

       * cd ~/mcw_cppamp/build

       * export CLAMP_NOTILECHECK=ON
       
       * cmake ../src -DCMAKE_BUILD_TYPE=Release -DCXXAMP_ENABLE_BOLT=ON -DOPENCL_HEADER_DIR=<path to SDK's OpenCL headers> -DOPENCL_LIBRARY_DIR=<path to SDK's OpenCL library>, 

       * for example cmake ../src -DCMAKE_BUILD_TYPE=Release -DCXXAMP_ENABLE_BOLT=ON  -DOPENCL_HEADER_DIR=/opt/AMDAPPSDK-2.9-1/include/CL/ -DOPENCL_LIBRARY_DIR=/opt/AMDAPPSDK-2.9-1/lib/x86_64/

  (c) Build AMP

       * cd ~/mcw_cppamp/build

       * make [-j#] world && make          (# is the number of parallel builds. Generally it is # of CPU cores)

       * For example: make -j8 world && make

With this the C++ AMP Compiler installation is complete.

### B. HCBLAS Installation 

(i) Clone MCW HCBLAS source codes

       * cd ~/
   
       * git clone https://bitbucket.org/multicoreware/hcblas.git 

       * cd ~/hcblas

       * git checkout Array_KALMAR_wrapper
   
(ii) Platform-specific build

(a) For Linux:  

       * cd ~/hcblas/Build/linux
       
       * export MCWCPPAMPBUILD=<path/to/mcw_cppamp/build>

       * sh build.sh

       * make

(b)  For Windows: (Prerequisite: Visual Studio 12 version )
       
1. For 32 Bit:

       * cd ~/hcblas/Build/vc11-x86

       * make-solutions.bat (This creates a Visual studio solution for hcblas Library) 

 2. For 64-bit:

       * cd ~/hcblas/Build/vc11-x86_64

       * make-solutions.bat (This creates a Visual Studio solution for hcblas Library)
       
       
### C. Unit testing

### 1. Install libblas-dev ubuntu package (Installs CBLAS library)

     * sudo apt-get install libblas-dev

### 2. Testing:
    
     * export HCBLAS_LIBRARY_PATH = ~/hcblas/Build/linux
     
     * export LD_LIBRARY_PATH=$HCBLAS_LIBRARY_PATH:$LD_LIBRARY_PATH
     
Note:
     
     * TransA (TA) and TransB(TB) takes 0 or 1
       where,            0 - noTrans (Operate with the actual matrix)
                         1 - Trans   (Operate with the transpose of the matrix)
                         
     * Implementation type (Itype) takes 1, 2 or 3
       where,            1 - Inputs and Outputs are host float pointers.
                         2 - Inputs and Outputs are C++ AMP float array containers.
                         3 - Inputs and Outputs are C++ AMP float array containers with batch processing.
                         

  (1) SGEMM - Single Precision real valued general matrix-matrix multiplication

     * cd ~/hcblas/source/test/sgemm_test/
     
     * make
     
     * To test ./sgemm M N K TA TB Itype (Here TA, TB take binary values 0 or 1 while Implementation type (Itype) takes 1, 2 or 3)
  
  (2) CGEMM - Complex valued general matrix matrix multiplication

     * cd ~/hcblas/source/test/cgemm_test/
     
     * make
     
     * To test ./cgemm M N K TA TB Itype (Here TA, TB take binary values 0 or 1 while Itype takes 1, 2 or 3)

  (3) SGEMV - Single Precision real valued general matrix-vector multiplication
    
     * cd ~/hcblas/source/test/sgemv_test/
     
     * make
   
     * To Test ./sgemv M N Trans Itype(Here Trans take binary values 0 or 1 while Itype takes 1, 2 or 3)

  (4) SGER - Single Precision General matrix rank 1 operation

     * cd ~/hcblas/source/test/sger_test/
     
     * make
   
     * To Test ./sger M N Itype(Itype takes 1, 2 or 3)

  (5) SAXPY - Scale vector X and add to vector Y
 
     * cd ~/hcblas/source/test/saxpy_test/
     
     * make
   
     * To Test ./saxpy N Itype(Itype takes 1, 2 or 3)

  (6) SSCAL - Single Precision scaling of Vector X 

     * cd ~/hcblas/source/test/sscal_test/
     
     * make
   
     * To Test ./sscal N Itype(Itype takes 1, 2 or 3)

  (7) DSCAL - Double Precision scaling of Vector X

     * cd ~/hcblas/source/test/dscal_test/
     
     * make
   
     * To Test ./dscal N Itype(Itype takes 1, 2 or 3)
 
  (8) SCOPY - Single Precision Copy 

     * cd ~/hcblas/source/test/scopy_test/
     
     * make
   
     * To Test ./scopy N Itype(Itype takes 1, 2 or 3)

  (9) DCOPY - Double Precision Copy

     * cd ~/hcblas/source/test/dcopy_test/
     
     * make
   
     * To Test ./dcopy N Itype(Itype takes 1, 2 or 3)

  (10) SASUM - Single Precision sum of Absolute values

     * cd ~/hcblas/source/test/sasum_test/
     
     * make
   
     * To Test ./sasum N Itype(Itype takes 1, 2 or 3)

  (11) DASUM - Double Precision sum of Absolute values

     * cd ~/hcblas/source/test/dasum_test/
     
     * make
   
     * To Test ./dasum N Itype(Itype takes 1, 2 or 3)

  (12) SDOT - Single Precision Dot product

     * cd ~/hcblas/source/test/sdot_test/
     
     * make
   
     * To Test ./sdot N Itype(Itype takes 1, 2 or 3)

  (13) DDOT - Double Precision Dot product

     * cd ~/hcblas/source/test/ddot_test/
     
     * make
   
     * To Test ./ddot N Itype(Itype takes 1, 2 or 3)
