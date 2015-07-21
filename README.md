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


##Building and set up:    
######Need to be a super user

(i)  ** C++ AMP Compiler installation**: Indepth details can be found [here](https://bitbucket.org/multicoreware/cppamp-driver-ng/wiki/Home)

Prepare a directory for work space.

   * mkdir mcw_cppamp

   * cd mcw_cppamp 
   
   * git clone https://bitbucket.org/multicoreware/cppamp-driver-ng.git src

   * git checkout torch-specific (torch-specific branch is tailor made for torch7 use case)
(note that you can also use git checkout origin/torch-specific)

Create a build directory and configure using CMake.

  *  mkdir mcw_cppamp/build

  * cd mcw_cppamp/build

  * cmake ../src -DCMAKE_BUILD_TYPE=Release (The torch-specific branch expects the AMDAPP SDK in the path /opt/AMDAPP)

Build the whole system. This will build clang and other libraries that require one time build.

  * make [-j #] world           (# is the number of parallel builds. Generally it is # of CPU cores)

  * make                        (this builds llvm utilities)

Note that you might need to manually check updates from C++ AMP Compiler.
Please do the following and rebuild the Compiler if any update is available

```
#!python
 # check updates from C++AMP Compiler
 cd mcw_cppamp/src
 git fetch --all
 git checkout origin/torch-specific

 # check updates from C++AMP Compiler's dependency
 cd mcw_cppamp/src/compiler/tools/clang
 git fetch --all
 git checkout origin/master
```


Prior to building the library the following environment variables need to be set using export command

* AMDAPPSDKROOT=<path to AMD APP SDK>
* MCWCPPAMPROOT=<path to mcw_cppamp dir>

(ii)  ** AMPBLAS Library Installation **: 
   
   * git clone https://bitbucket.org/multicoreware/ampblas.git 

   * cd ampblas

   * git checkout Development
   
  For Linux:  

     * cd Build/linux
     * sh build.sh
     * make

  For 32-bit Windows Systems: ( It requires Visual Studio 12 version )

     * cd Build/vc11-x86
     * make-solutions.bat (This creates a Visual studio solution for ampblas Library) 

  For 64-bit Windows Systems: ( It requires Visual Studio 12 version )

     * cd Build/vc11-x86_64
     * make-solutions.bat (This creates a Visual Studio solution for ampblas Library)

 