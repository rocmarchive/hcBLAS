##Introduction: ##

This repository hosts the HCC based BLAS Library (hcBLAS), that targets GPU acceleration of the traditional set of BLAS routines on AMD devices. . To know what HCC compiler features, refer [here](https://bitbucket.org/multicoreware/hcc/wiki/Home). 


The following list enumerates the current set of BLAS sub-routines that are supported so far. 

* Sgemm  : Single Precision real valued general matrix-matrix multiplication
* Cgemm  : Complex valued general matrix matrix multiplication
* Sgemv  : Single Precision real valued general matrix-vector multiplication
* Sger   : Single Precision General matrix rank 1 operation
* Saxpy  : Scale vector X and add to vector Y
* Sscal  : Single Precision scaling of Vector X 
* Dscal  : Double Precision scaling of Vector X
* Scopy  : Single Precision Copy 
* Dcopy  : Double Precision Copy
* Sasum : Single Precision Absolute sum of values of a vector
* Dasum : Double Precision Absolute sum of values of a vector
* Sdot  : Single Precision Dot product
* Ddot  : Double Precision Dot product

## Key Features: ##

* Support for 13 commonly used BLAS routines
* Batched GEMM API
* Ability to Choose desired target accelerator
* Single and Double precision


##Prerequisites: ##

**A. Hardware Requirements:**

* CPU: mainstream brand, Better if with >=4 Cores Intel Haswell based CPU 
* System Memory >= 4GB (Better if >10GB for NN application over multiple GPUs)
* Hard Drive > 200GB (Better if SSD or NVMe driver  for NN application over multiple GPUs)
* Minimum GPU Memory (Global) > 2GB

**B. GPU SDK and driver Requirements:**

* AMD R9 Fury X, R9 Fur, R9 Nano
* AMD APU Kaveri or Carrizo

**C. System software requirements:**

* Ubuntu 14.04 trusty
* GCC 4.6 and later
* CPP 4.6 and later (come with GCC package)
* python 2.7 and later


**D. Tools and Misc Requirements:**

* git 1.9 and later
* cmake 2.6 and later (2.6 and 2.8 are tested)
* firewall off
* root privilege or user account in sudo group


**E. Ubuntu Packages requirements:**

* libc6-dev-i386
* liblapack-dev
* graphicsmagick


## Tested Environment so far: 

**A. Driver versions tested**  

* Boltzmann Early Release Driver 
* HSA driver

**B. GPU Cards tested:**

* Radeon R9 Nano
* Radeon R9 FuryX 
* Radeon R9 Fury 
* Kaveri and Carizo APU

**C. Desktop System Tested**

* Supermicro SYS-7048GR-TR  Tower 4 W9100 GPU
* ASUS X99-E WS motherboard with 4 AMD FirePro W9100
* Gigabyte GA-X79S 2 AMD FirePro W9100 GPU’s

**D. Server System Tested**

* Supermicro SYS 2028GR-THT  6 R9 NANO
* Supermicro SYS-1028GQ-TRT 4 R9 NANO
* Supermicro SYS-7048GR-TR Tower 4 R9 NANO


## Installation Steps:   

### A. HCC Compiler Installation: 

a) Download the compiler debian.

* Click [here](https://bitbucket.org/multicoreware/hcc/downloads/hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb)

   (or)

* via terminal: 

               wget https://bitbucket.org/multicoreware/hcc/downloads/hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb 


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
    
a) Automated testing:

     * cd ~/hcblas/test/unit/
     
     * ./test.sh
     
b) Manual testing:

     * cd ~/hcblas/test/build/linux/bin/
     
     * choose the appropriate named binary 
     
c) Notes for Manual testing:
     
     * TransA (TA) and TransB(TB) takes 0 or 1
       where,            0 - NoTrans (Operate with the given matrix)
                         1 - Trans   (Operate with the transpose of the given matrix)
                         
     * Implementation type (Itype) takes 1 or 2
       where,            1 - Inputs and Outputs are device pointers.
                         2 - Inputs and Outputs are device pointers with batch processing.
     
  (1) SGEMM - Single Precision real valued general matrix-matrix multiplication 
     
     * ./sgemm M N K TA TB Itype (Here TA, TB take binary values 0 or 1 while Implementation type (Itype) takes 1 or 2)
  
  (2) CGEMM - Complex valued general matrix matrix multiplication

     * ./cgemm M N K TA TB Itype (Here TA, TB take binary values 0 or 1 while Itype takes 1 or 2)

  (3) SGEMV - Single Precision real valued general matrix-vector multiplication
       
     * ./sgemv M N Trans Itype(Here Trans take binary values 0 or 1 while Itype takes 1 or 2)

  (4) SGER - Single Precision General matrix rank 1 operation
 
     * ./sger M N Itype(Itype takes 1 or 2)

  (5) SAXPY - Scale vector X and add to vector Y
    
     * ./saxpy N Itype(Itype takes 1 or 2)

  (6) SSCAL - Single Precision scaling of Vector X 
  
     * ./sscal N Itype(Itype takes 1 or 2)

  (7) DSCAL - Double Precision scaling of Vector X
   
     * ./dscal N Itype(Itype takes 1 or 2)
 
  (8) SCOPY - Single Precision Copy 

     * ./scopy N Itype(Itype takes 1 or 2)

  (9) DCOPY - Double Precision Copy
   
     * ./dcopy N Itype(Itype takes 1 or 2)

  (10) SASUM - Single Precision sum of Absolute values

     * ./sasum N Itype(Itype takes 1 or 2)

  (11) DASUM - Double Precision sum of Absolute values

     * ./dasum N Itype(Itype takes 1 or 2)

  (12) SDOT - Single Precision Dot product

     * ./sdot N Itype(Itype takes 1 or 2)

  (13) DDOT - Double Precision Dot product

     * ./ddot N Itype(Itype takes 1 or 2)