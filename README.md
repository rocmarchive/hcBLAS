# This was an experimental BLAS implementation for ROCm and has been DEPRECATED. Please use [rocBLAS](https://github.com/ROCmSoftwarePlatform/rocBLAS).

## A. Introduction: ##

This repository hosts the HCC based BLAS Library (hcBLAS), that targets GPU acceleration of the traditional set of BLAS routines on AMD devices. . To know what HCC compiler features, refer [here](https://github.com/RadeonOpenCompute/hcc). 


The following list enumerates the current set of BLAS sub-routines that are supported so far. 

* Sgemm  : Single Precision real valued general matrix-matrix multiplication
* Dgemm  : Double Precision real valued general matrix-matrix multiplication
* Cgemm  : Single Precision Complex valued general matrix-matrix multiplication
* Zgemm  : Double Precision Complex valued general matrix-matrix multiplication
* Hgemm  : Half Precision general matrix-matrix multiplication.
* Sgemv  : Single Precision real valued general matrix-vector multiplication
* Dgemv  : Double Precision real valued general matrix-vector multiplication
* Sger   : Single Precision General matrix rank 1 operation
* Dger   : Double Precision General matrix rank 1 operation
* Saxpy  : Single Precision Scale vector X and add to vector Y
* Daxpy  : Double Precision Scale vector X and add to vector Y
* Sscal  : Single Precision scaling of Vector X 
* Dscal  : Double Precision scaling of Vector X
* Cscal  : Single Precision Complex valued scaling of Vector X 
* Zscal  : Double Precision Complex valued scaling of Vector X
* Csscal : Single Precision scaling ofcomplex vector by a real constant
* Zdscal : Double Precision scaling ofcomplex vector by a real constant
* Scopy  : Single Precision Copy 
* Dcopy  : Double Precision Copy
* Sasum  : Single Precision Absolute sum of values of a vector
* Dasum  : Double Precision Absolute sum of values of a vector
* Sdot   : Single Precision Dot product
* Ddot   : Double Precision Dot product


## B. Key Features ##

* Support for 23 commonly used BLAS routines
* Batched GEMM API
* Ability to Choose desired target accelerator
* Single and Double precision


## C. Prerequisites ##

* Refer Prerequisites section [here](https://github.com/ROCmSoftwarePlatform/hcBLAS/wiki/Prerequisites)

## D. Tested Environment so far 

* Refer Tested environments enumerated [here](https://github.com/ROCmSoftwarePlatform/hcBLAS/wiki/Tested-Environments)


## E. Installation  

* Follow installation steps as described [here](https://github.com/ROCmSoftwarePlatform/hcBLAS/wiki/Installation)


## F. Unit testing

* Follow testing procedures as explained [here](https://github.com/ROCmSoftwarePlatform/hcBLAS/wiki/Unit-testing)

## G. API reference

* The Specification of API's supported along with description  can be found [here](http://hcblas-documentation.readthedocs.org/en/latest/API_reference.html)


## H. Examples
* Refer Examples section [here](https://github.com/ROCmSoftwarePlatform/hcBLAS/wiki/Examples)
