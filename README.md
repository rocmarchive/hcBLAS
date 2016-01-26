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

To know more, go through the [Documentation](http://hcblas-documentation.readthedocs.org/en/latest/)

## Key Features: ##

* Support for 13 commonly used BLAS routines
* Batched GEMM API
* Ability to Choose desired target accelerator
* Single and Double precision


##Prerequisites: ##

* Refer Prerequisites section [here](http://hcblas-documentation.readthedocs.org/en/latest/#prerequisites)

## Tested Environment so far: 

* Refer Tested environments enumerated [here](http://hcblas-documentation.readthedocs.org/en/latest/#tested-environments)


## Installation Steps:  

* Follow installation steps as described [here](http://hcblas-documentation.readthedocs.org/en/latest/#installation-steps)


## Unit testing

* Follow testing procedures as explained [here](http://hcblas-documentation.readthedocs.org/en/latest/#unit-testing)