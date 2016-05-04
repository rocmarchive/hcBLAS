***********************
1.4. Installation steps
***********************
-------------------------------------------------------------------------------------------------------------------------------------------

The following are the steps to use the library

      * ROCM 1.0 Kernel, Driver and Compiler Installation (if not done until now)
      * Library installation.

1.4.1. ROCM 1.0 Installation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To Know more about ROCM Please refer [here](https://github.com/RadeonOpenCompute/ROCm/blob/master/README.md)

   a. Installing Debian ROCM repositories
     
     Before proceeding, make sure to completely [uninstall any pre-release ROCm packages](https://github.com/RadeonOpenCompute/ROCm#removing-pre-release-packages) 
     installed earlier:
     
      * ``wget -qO - http://packages.amd.com/rocm/apt/debian/rocm.gpg.key | sudo apt-key add -``
      
      * ``sudo sh -c 'echo deb [arch=amd64] http://packages.amd.com/rocm/apt/debian/ trusty main > /etc/apt/sources.list.d/rocm.list'``
      
      * ``sudo apt-get update``
    
      * ``sudo apt-get install rocm``
     
      * Reboot the system
      
    b. Once Reboot, verify the installation
    
      To verify that the ROCm stack completed successfully you can execute to HSA vectory_copy sample application:

       * ``cd /opt/rocm/hsa/sample``
       
       * ``make``
       
       * ``./vector_copy``
   

1.4.3. Library Installation
^^^^^^^^^^^^^^^^^^^^^^^^^^^

    a. Install using Prebuilt debian
    
        * ``wget https://bitbucket.org/multicoreware/hcblas/downloads/hcblas-master-db04c54-Linux.deb``
        
        * ``sudo dpkg -i hcblas-master-db04c54-Linux.deb``

     
    b. Build debian from source
    
        * ``git clone https://bitbucket.org/multicoreware/hcblas.git && cd hcblas``
        
        * ``chmod +x build.sh && ./build.sh``
        
        build.sh execution builds the library and generates a debian under build dir

    c. Install CPU based CBLAS library

       ``sudo apt-get install libblas-dev``


