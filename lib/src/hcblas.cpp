#include "hcblas.h"

// hcblas Helper functions 

// 1. hcblasCreate()

// This function initializes the HCBLAS library and creates a handle to an opaque structure
// holding the HCBLAS library context.

// Return Values
// --------------------------------------------------------------------
// HCBLAS_STATUS_SUCCESS            initialization succeeded
// HCBLAS_STATUS_NOT_INITIALIZED    the CUDA™ Runtime initialization failed
// HCBLAS_STATUS_ALLOC_FAILED       the resources could not be allocated  

hcblasStatus_t hcblasCreate(hcblasHandle_t *handle) {

  return HCBLAS_STATUS_SUCCESS;
}

// 2. hcblasDestory()

// This function releases hardware resources used by the HCBLAS library.
// This function is usually the last call with a particular handle to the HCBLAS library.

// Return Values
// ---------------------------------------------------------------------
// HCBLAS_STATUS_SUCCESS            the shut down succeeded
// HCBLAS_STATUS_NOT_INITIALIZED    the library was not initialized

hcblasStatus_t hcblasDestroy(hcblasHandle_t handle){
  
  return HCBLAS_STATUS_SUCCESS;
}


// 3. hcblasSetVector()

// This function copies n elements from a vector x in host memory space to a vector y in GPU memory space.
// Elements in both vectors are assumed to have a size of elemSize bytes. The storage spacing between
// consecutive elements is given by incx for the source vector x and by incy for the destination vector y.

// Return Values
// --------------------------------------------------------------------
// HCBLAS_STATUS_SUCCESS            the operation completed successfully
// HCBLAS_STATUS_NOT_INITIALIZED    the library was not initialized
// HCBLAS_STATUS_INVALID_VALUE      the parameters incx, incy, elemSize<=0
// HCBLAS_STATUS_MAPPING_ERROR      there was an error accessing GPU memory

hcblasStatus_t hcblasSetVector(int n, int elemSize, const void *x, int incx, void *y, int incy) {
  
  return HCBLAS_STATUS_SUCCESS;
}

// 4. hcblasGetVector()

// This function copies n elements from a vector x in GPU memory space to a vector y in host memory space.
// Elements in both vectors are assumed to have a size of elemSize bytes. The storage spacing between
// consecutive elements is given by incx for the source vector and incy for the destination vector y.

// Return Values
// --------------------------------------------------------------------
// HCBLAS_STATUS_SUCCESS            the operation completed successfully
// HCBLAS_STATUS_NOT_INITIALIZED    the library was not initialized
// HCBLAS_STATUS_INVALID_VALUE      the parameters incx, incy, elemSize<=0
// HCBLAS_STATUS_MAPPING_ERROR      there was an error accessing GPU memory

hcblasStatus_t hcblasGetVector(int n, int elemSize, const void *x, int incx, void *y, int incy) {

 return HCBLAS_STATUS_SUCCESS;
}

