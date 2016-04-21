***********************
1.4. Installation steps
***********************
-------------------------------------------------------------------------------------------------------------------------------------------

The following are the steps to use the library

      * Boltzmann Driver and Runtime installation (if not done until now)

      * Compiler installation.

      * Library installation.

1.4.1. Boltzmann Driver and Runtime Installation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

1.4.2. Compiler Installation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

1.4.3. Library Installation
^^^^^^^^^^^^^^^^^^^^^^^^^^^

    a. Clone the repo

       ``git clone https://bitbucket.org/multicoreware/hcblas.git && cd hcblas``

    b. Modify scripts mode to binary

       ``chmod +x install.sh``

    c. Install CPU based CBLAS library

       ``sudo apt-get install libblas-dev``

    d. Install library 

       (1) Installation in /opt/rocm/ (sudo) 

            ``./install.sh``

            Additionally to run the unit test along with installation invoke the following command,

            ``./install.sh --test=on`` 
      
            To get performance scores,
 
            ``export CODEXL_PATH=/path/to/profiler/``

            ``./install.sh --profile=on``

       .. note:: ``--test`` and ``--profile`` options could be used together as well.

       (2) Installation in User Specific path

            ``./install.sh --path=/path/to/user/installation/`` 
 
            Additionally to run the unit test along with installation invoke the following command,

            ``./install.sh --path=/path/to/user/installation/ --test=on``

            To get performance scores,

            ``export CODEXL_PATH=/path/to/profiler/``

            ``./install.sh --path=/path/to/user/installation/ --profile=on``
       
       .. note:: ``--test`` and ``--profile`` options could be used together as well.

    .. note:: **To switch between the installation paths, please uninstall the library and start the installation again.**

Once done with the above steps the libhcblas.so and associated headers gets installed under either /opt/rocm path (needs sudo access) or User specific path.

To uninstall the library, invoke the following series of commands

       ``chmod +x uninstall.sh``

       ``./uninstall.sh``
