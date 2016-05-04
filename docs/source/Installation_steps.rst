***********************
1.4. Installation steps
***********************
-------------------------------------------------------------------------------------------------------------------------------------------

The following are the steps to use the library

      * ROCM 1.0 Kernel, Driver and Compiler Installation (if not done until now)
      * Library installation.

1.4.1. ROCM 1.0 Installation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-

1.4.2. Library Installation
^^^^^^^^^^^^^^^^^^^^^^^^^^^

a. Install using Prebuilt debian

    
        * ``wget https://bitbucket.org/multicoreware/hcblas/downloads/hcblas-master-db04c54-Linux.deb``
        
        
        * ``sudo dpkg -i hcblas-master-db04c54-Linux.deb``

     
b. Build debian from source

    
        * ``git clone https://bitbucket.org/multicoreware/hcblas.git && cd hcblas``
        
        
        * ``chmod +x build.sh && ./build.sh``
        
        
        build.sh execution builds the library and generates a debian under build dir
        

c. Install CPU based CBLAS library


       * ``sudo apt-get install libblas-dev``
       
       
       


