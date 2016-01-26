##Introduction: ##

This repository hosts the HCC based BLAS Library (hcBLAS), that targets GPU acceleration of the traditional set of BLAS routines on AMD devices. . To know what HCC compiler features, refer [here](https://bitbucket.org/multicoreware/hcc/wiki/Home). 


The following list enumerates the current set of BLAS sub-routines that are supported so far. 

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

## Key Features: ##

1. Support for 13 commonly used BLAS routines
2. Batched GEMM API
3. Ability to Choose desired target accelerator
4. Single and Double precision


##Prerequisites: ##

**Hardware Requirements:**

* CPU: mainstream brand, Better if with >=4 Cores Intel Haswell based CPU 
* System Memory >= 4GB (Better if >10GB for NN application over multiple GPUs)
* Hard Drive > 200GB (Better if SSD or NVMe driver  for NN application over multiple GPUs)
* Minimum GPU Memory (Global) > 2GB

**GPU SDK and driver Requirements:**

* AMD R9 Fury X, R9 Fur, R9 Nano
* AMD APU Kaveri or Carrizo

**System software requirements:**

* Ubuntu 14.04 trusty
* GCC 4.6 and later
* CPP 4.6 and later (come with GCC package)
* python 2.7 and later


**Tools and Misc Requirements:**

* git 1.9 and later
* cmake 2.6 and later (2.6 and 2.8 are tested)
* firewall off
* root privilege or user account in sudo group


**Ubuntu Packages requirements:**

* libc6-dev-i386
* liblapack-dev
* graphicsmagick


## Tested Environment so far: 

**Driver versions tested**  

* Boltzmann Early Release Driver 
* HSA driver

**GPU Cards tested:**

* Radeon R9 Nano
* Radeon R9 FuryX 
* Radeon R9 Fury 
* Kaveri and Carizo APU

**Desktop System Tested**

* Supermicro SYS-7048GR-TR  Tower 4 W9100 GPU
* ASUS X99-E WS motherboard with 4 AMD FirePro W9100
* Gigabyte GA-X79S 2 AMD FirePro W9100 GPU’s

**Server System Tested**

* Supermicro SYS 2028GR-THT  6 R9 NANO
* Supermicro SYS-1028GQ-TRT 4 R9 NANO
* Supermicro SYS-7048GR-TR Tower 4 R9 NANO


## Installation Steps: ##    

### A. HCC Compiler Installation: 

a) Download the compiler debian.

           Click [here](https://bitbucket.org/multicoreware/hcc/downloads/hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb)

                            (or)

           wget https://bitbucket.org/multicoreware/hcc/downloads/hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb via terminal


b) Install the compiler
 
      sudo dpkg -i hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb
      
    
### B. HCBLAS Installation 
   
       * git clone https://bitbucket.org/multicoreware/hcblas.git 

       * cd ~/hcblas

       * ./install.sh test=ON/OFF 
         Where
           test=OFF    - Build library and tests
           test=ON     - Build library, tests and run test.sh

       
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