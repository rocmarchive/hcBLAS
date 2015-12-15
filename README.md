# ** HCBLAS ** #

##Introduction: ##

This repository hosts the HCC implementation of BLAS subroutines. The following are the sub-routines that are implemented

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

### A. HCC Compiler Installation: 


**Install HCC compiler debian package:**

  Download the debian package from the link given below,
  
  [Compiler-Debians](https://multicorewareinc.egnyte.com/dl/TD5IwsNEx3)
  
  Install the package hcc-0.8.1544-a9f4d2f-ddba18d-Linux.deb 
  
  using the command,
  
    sudo dpkg -i <package_name>
      e.g. sudo dpkg -i  hcc-0.8.1544-a9f4d2f-ddba18d-Linux.deb 
      
  Note: 
      Ignore clamp-bolt, Bolt is not required for hcblas.
    


### B. HCBLAS Installation 

(i) Clone MCW HCBLAS source codes

       * cd ~/
   
       * git clone https://bitbucket.org/multicoreware/hcblas.git 

       * cd ~/hcblas
   
(ii) Platform-specific build

(a) For Linux:  

       * ./install.sh test=ON/OFF profile=ON/OFF
         Where
           test=OFF    - Build library and tests
           test=ON     - Build library, tests and run test.sh
           profile=OFF - HCBLAS profiling not enabled
           profile=ON  - Enable HCBLAS profiling 


(b)  For Windows: (Prerequisite: Visual Studio 12 version )
       
1. For 32 Bit:

       * cd ~/hcblas/lib/build/vc11-x86

       * make-solutions.bat (This creates a Visual studio solution for hcblas Library) 

 2. For 64-bit:

       * cd ~/hcblas/lib/build/vc11-x86_64

       * make-solutions.bat (This creates a Visual Studio solution for hcblas Library)
       
### C. Unit testing

### 1. Install libblas-dev ubuntu package (Installs CBLAS library)

     * sudo apt-get install libblas-dev

### 2. Testing:
    
(a) For Linux:

     * cd ~/hcblas/test/unit/
     
     * ./test.sh
     
     To test manually, 

     * cd ~/hcblas/test/build/linux/bin/
     
Note:
     
     * TransA (TA) and TransB(TB) takes 0 or 1
       where,            0 - noTrans (Operate with the actual matrix)
                         1 - Trans   (Operate with the transpose of the matrix)
                         
     * Implementation type (Itype) takes 1, 2, 3, 4 or 5
       where,            1 - Inputs and Outputs are host float/double pointers.
                         2 - Inputs and Outputs are HCC float/double array_View containers.
                         3 - Inputs and Outputs are HCC float/double array_View containers with batch processing.
                         4 - Inputs and Outputs are HCC float/double array containers.
                         5 - Inputs and Outputs are HCC float/double array containers with batch processing.
     
  (1) SGEMM - Single Precision real valued general matrix-matrix multiplication 
     
     * ./sgemm M N K TA TB Itype (Here TA, TB take binary values 0 or 1 while Implementation type (Itype) takes 1, 2, 3, 4 or 5)
  
  (2) CGEMM - Complex valued general matrix matrix multiplication

     * ./cgemm M N K TA TB Itype (Here TA, TB take binary values 0 or 1 while Itype takes 1, 2, 3, 4 or 5)

  (3) SGEMV - Single Precision real valued general matrix-vector multiplication
       
     * ./sgemv M N Trans Itype(Here Trans take binary values 0 or 1 while Itype takes 1, 2, 3, 4 or 5)

  (4) SGER - Single Precision General matrix rank 1 operation
 
     * ./sger M N Itype(Itype takes 1, 2, 3, 4 or 5)

  (5) SAXPY - Scale vector X and add to vector Y
    
     * ./saxpy N Itype(Itype takes 1, 2, 3, 4 or 5)

  (6) SSCAL - Single Precision scaling of Vector X 
  
     * ./sscal N Itype(Itype takes 1, 2, 3, 4 or 5)

  (7) DSCAL - Double Precision scaling of Vector X
   
     * ./dscal N Itype(Itype takes 1, 2, 3, 4 or 5)
 
  (8) SCOPY - Single Precision Copy 

     * ./scopy N Itype(Itype takes 1, 2, 3, 4 or 5)

  (9) DCOPY - Double Precision Copy
   
     * ./dcopy N Itype(Itype takes 1, 2, 3, 4 or 5)

  (10) SASUM - Single Precision sum of Absolute values

     * ./sasum N Itype(Itype takes 1, 2, 3, 4 or 5)

  (11) DASUM - Double Precision sum of Absolute values

     * ./dasum N Itype(Itype takes 1, 2, 3, 4 or 5)

  (12) SDOT - Single Precision Dot product

     * ./sdot N Itype(Itype takes 1, 2, 3, 4 or 5)

  (13) DDOT - Double Precision Dot product

     * ./ddot N Itype(Itype takes 1, 2, 3, 4 or 5)
     
(b)  For Windows: (Prerequisite: Visual Studio 12 version )
       
1. For 32 Bit:

       * cd ~/hcblas/test/build/vc11-x86

       * make-solutions.bat (This creates a Visual studio solution for hcblastest Library) 

 2. For 64-bit:

       * cd ~/hcblas/test/build/vc11-x86_64

       * make-solutions.bat (This creates a Visual Studio solution for hcblastest Library)
