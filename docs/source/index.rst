====================
hcBLAS Documentation
====================
***************
1. Introduction
***************
--------------------------------------------------------------------------------------------------------------------------------------------

The hcBLAS library is an implementation of BLAS (Basic Linear Algebra Subprograms) targetting the AMD heterogenous hardware via HCC compiler runtime. The computational resources of underlying AMD heterogenous compute gets exposed and exploited through the HCC C++ frontend. Refer `here <https://bitbucket.org/multicoreware/hcc/wiki/Home>`_ for more details on HCC compiler.

To use the hcBLAS API, the application must allocate the required matrices and vectors in the GPU memory space, fill them with data, call the sequence of desired hcBLAS functions, and then upload the results from the GPU memory space back to the host. The hcBLAS API also provides helper functions for writing and retrieving data from the GPU.

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

*****************
2. Prerequisites
*****************
-------------------------------------------------------------------------------------------------------------------------------------------

This section lists the known set of hardware and software requirements to build this library

2.1. Hardware
^^^^^^^^^^^^^

* CPU: mainstream brand, Better if with >=4 Cores Intel Haswell based CPU 
* System Memory >= 4GB (Better if >10GB for NN application over multiple GPUs)
* Hard Drive > 200GB (Better if SSD or NVMe driver  for NN application over multiple GPUs)
* Minimum GPU Memory (Global) > 2GB

2.2. GPU cards supported
^^^^^^^^^^^^^^^^^^^^^^^^

* dGPU: AMD R9 Fury X, R9 Fury, R9 Nano
* APU: AMD Kaveri or Carrizo

2.3 AMD Driver and Runtime
^^^^^^^^^^^^^^^^^^^^^^^^^^

* Radeon Open Compute Kernel (ROCK) driver : https://github.com/RadeonOpenCompute/ROCK-Kernel-Driver
* HSA runtime API and runtime for Boltzmann:  https://github.com/RadeonOpenCompute/ROCR-Runtime

2.4. System software
^^^^^^^^^^^^^^^^^^^^^

* Ubuntu 14.04 trusty
* GCC 4.6 and later
* CPP 4.6 and later (come with GCC package)
* python 2.7 and later
* HCC 0.9 from `here <https://bitbucket.org/multicoreware/hcc/downloads/hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb>`_


2.5. Tools and Misc
^^^^^^^^^^^^^^^^^^^

* git 1.9 and later
* cmake 2.6 and later (2.6 and 2.8 are tested)
* firewall off
* root privilege or user account in sudo group


2.6. Ubuntu Packages
^^^^^^^^^^^^^^^^^^^^

* libc6-dev-i386
* liblapack-dev
* graphicsmagick

**********************
3. Tested Environments
**********************
-------------------------------------------------------------------------------------------------------------------------------------------

This sections enumerates the list of tested combinations of Hardware and system softwares.

3.1. Driver versions 
^^^^^^^^^^^^^^^^^^^^

* Boltzmann Early Release Driver + dGPU

      * Radeon Open Compute Kernel (ROCK) driver : https://github.com/RadeonOpenCompute/ROCK-Kernel-Driver
      * HSA runtime API and runtime for Boltzmann:  https://github.com/RadeonOpenCompute/ROCR-Runtime

* Traditional HSA driver + APU (Kaveri)


3.2. GPU Cards
^^^^^^^^^^^^^^

* Radeon R9 Nano
* Radeon R9 FuryX 
* Radeon R9 Fury 
* Kaveri and Carizo APU


3.3. Desktop System 
^^^^^^^^^^^^^^^^^^^

* Supermicro SYS-7048GR-TR  Tower 4 W9100 GPU
* ASUS X99-E WS motherboard with 4 AMD FirePro W9100
* Gigabyte GA-X79S 2 AMD FirePro W9100 GPU’s


3.4. Server System 
^^^^^^^^^^^^^^^^^^

* Supermicro SYS 2028GR-THT  6 R9 NANO
* Supermicro SYS-1028GQ-TRT 4 R9 NANO
* Supermicro SYS-7048GR-TR Tower 4 R9 NANO


************************
4. Installation steps
************************
-------------------------------------------------------------------------------------------------------------------------------------------

The following are the steps to use the library
      
      * Boltzmann Driver and Runtime installation (if not done until now)
         
      * Compiler installation.

      * Library installation.
      
4.1 Boltzmann Driver and Runtime Installation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

     a. Downloading the kernel binaries from the repo
     
        ``git clone https://github.com/RadeonOpenCompute/ROCK-Kernel-Driver.git``
        
     b. Go to the top of the repo
        
        ``cd ROCK-Kernel-Driver``
         
     c. Configure udev to allow any user to access /dev/kfd. 
        As root, use a text editor to create /etc/udev/rules.d/kfd.rules 
        containing one line: KERNEL=="kfd", MODE="0666", Or you could use the following command
        
        ``echo "KERNEL==\"kfd\", MODE=\"0666\"" | sudo tee /etc/udev/rules.d/kfd.rules``
        
     d. For Ubuntu, install the kernel and libhsakmt packages using:
       
        ``sudo dpkg -i packages/ubuntu/*.deb``
         
     e. Reboot the system to install the new kernel and enable the HSA kernel driver
     
        ``sudo reboot``
         
     f. Once done with reboot, one could proceed with runtime installation
     
        ``git clone https://github.com/RadeonOpenCompute/ROCR-Runtime``
         
        ``cd ROCR-Runtime/packages/ubuntu``
         
        ``sudo dpkg -i hsa-runtime-dev-1.0.0-amd64.deb``
         
        The contents get installed by default under /opt/hsa path
        

     e. Create a file called hsa.conf in /etc/ld.so.conf.d/ and write "/opt/hsa/lib" in it, 
        then run "sudo ldconfig -v" or you could use the following command

        ``echo "/opt/hsa/lib" | sudo tee /etc/ld.so.conf.d/hsa.conf``

        ``sudo ldconfig -v``

4.2 Compiler Installation
^^^^^^^^^^^^^^^^^^^^^^^^^

     a. Install pre-dependency packages

::

        sudo apt-get install cmake git subversion g++ libstdc++-4.8-dev libdwarf-dev libelf-dev 
        libtinfo-dev libc6-dev-i386 gcc-multilib llvm llvm-dev llvm-runtime libc++1 libc++-dev 
        libc++abi1 libc++abi-dev re2c libncurses5-dev

|
     b. Download Compiler 

        Click `here <https://bitbucket.org/multicoreware/hcc/downloads/hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb>`_
                                        
                                              (or)

        wget https://bitbucket.org/multicoreware/hcc/downloads/hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb

     c. Install the compiler

        ``sudo dpkg -i hcc-0.9.16041-0be508d-ff03947-5a1009a-Linux.deb``

Once done with the above steps the compiler headers, binaries and libraries gets installed under /opt system path as ``/opt/hcc`` .

4.3 Library Installation
^^^^^^^^^^^^^^^^^^^^^^^^

    a. Clone the repo
             
       ``git clone https://bitbucket.org/multicoreware/hcblas.git && cd hcblas``

    b. Modify scripts mode to binary

       ``chmod +x install.sh``

    c. Install CPU based CBLAS library 
  
       ``sudo apt-get install libblas-dev``

    d. Install library

       ``./install.sh``


    e. Additionally to run the unit test along with installation invoke the following command

       ``./install.sh test=ON``

Once done with the above steps the libhcblas.so and associated headers gets installed under system path.

To uninstall the library, invoke the following series of commands

       ``chmod +x uninstall.sh``

       ``./uninstall.sh``


************************
5. Unit testing
************************
--------------------------------------------------------------------------------------------------------------------------------------------

5.1. Testing hcBLAS against CBLAS:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    
a) Automated testing:

       ``cd ~/hcblas/test/unit/``
     
       ``./test.sh``
     
b) Manual testing:

       ``cd ~/hcblas/test/build/linux/bin/``
     
       choose the appropriate named binary 
     

Here are some notes for performing manual testing:
     
|      TransA (TA) and TransB(TB) takes 0 or 1
|       where,            0 - NoTrans (Operate with the given matrix)
|                         1 - Trans   (Operate with the transpose of the given matrix)
|                         
|      Implementation type (Itype) takes 1 or 2
|       where,            1 - Inputs and Outputs are device pointers.
|                         2 - Inputs and Outputs are device pointers with batch processing.
|
     
      * SGEMM
     
      ``./sgemm M N K TA TB Itype``
  
      * CGEMM 

      ``./cgemm M N K TA TB Itype``

      * SGEMV
       
      ``./sgemv M N Trans Itype``

      * SGER
 
      ``./sger M N Itype``

      * SAXPY

      ``./saxpy N Itype``

      * SSCAL
  
      ``./sscal N Itype``

      * DSCAL
   
      ``./dscal N Itype``
 
      * SCOPY 

      ``./scopy N Itype``

      * DCOPY
   
      ``./dcopy N Itype``

      * SASUM

      ``./sasum N Itype``

      * DASUM

      ``./dasum N Itype``

      * SDOT

      ``./sdot N Itype``

      * DDOT

      ``./ddot N Itype``


************************
6. hcBLAS API Reference
************************
-------------------------------------------------------------------------------------------------------------------------------------------
    
This section provides a brief description of APIs and helper routines hosted by the library

.. toctree::
   :maxdepth: 2

   HCBLAS_TYPES
   Helper_functions
   Modules

.. Index::
   SGEMM
   CGEMM
   SGEMV
   SGER
   SAXPY
   SSCAL
   DSCAL
   SCOPY
   DCOPY
   SASUM
   DASUM
   SDOT
   DDOT
   HCBLAS_TYPES






  








  






 
