#ifndef HCBLAS_H
#define HCBLAS_H

#include "hcblaslib.h"

//2.2.1. hcblasHandle_t

// The hcblasHandle_t type is a pointer type to an opaque structure holding the cuBLAS library context. 
// The cuBLAS library context must be initialized using hcblasCreate() and the returned handle must be 
// passed to all subsequent library function calls. The context should be destroyed at the end using 
// hcblasDestroy().

typedef hcblasHandle_t Hcblaslibrary;


// Type hcblasStatus

// The type  hcblasStatus  is used for function status returns. HCBLAS 
// helper functions return status directly, while the status of HCBLAS 
// core functions can be retrieved via  hcblasGetError() . Currently, the 
// following values are defined: 

enum hcblasStatus_t {
  HCBLAS_SUCCESS,                 // Function succeeds
  HCBLAS_STATUS_NOT_INITIALIZED,  // HCBLAS library not initialized
  HCBLAS_STATUS_ALLOC_FAILED,     // resource allocation failed
  HCBLAS_STATUS_INVALID_VALUE,    // unsupported numerical value was passed to function
  HCBLAS_STATUS_MAPPING_ERROR,    // access to GPU memory space failed
  HCBLAS_STATUS_EXECUTION_FAILED, // GPU program failed to execute
  HCBLAS_STATUS_INTERNAL_ERROR    // an internal HCBLAS operation failed
} hcblasStatus;

// 2.2.3. hcblasOperation_t

// The hcblasOperation_t type indicates which operation needs to be performed with 
// the dense matrix. Its values correspond to Fortran characters ‘N’ or ‘n’ (non-transpose),
// ‘T’ or ‘t’ (transpose) and ‘C’ or ‘c’ (conjugate transpose) that are often used as parameters 
// to legacy BLAS implementations.

enum hcblasOperation_t {
  HCBLAS_OP_N,  // The Non transpose operation is selected
  HCBLAS_OP_T,  // Transpose operation is selected
  HCBLAS_OP_C   // Conjugate transpose operation is selected
}
  
// hcblas Helper functions 

// 1. hcblasCreate()

// This function initializes the HCBLAS library and creates a handle to an opaque structure
// holding the HCBLAS library context.

// Return Values
// --------------------------------------------------------------------
// HCBLAS_STATUS_SUCCESS            initialization succeeded
// HCBLAS_STATUS_NOT_INITIALIZED    the CUDA™ Runtime initialization failed
// HCBLAS_STATUS_ALLOC_FAILED       the resources could not be allocated  

hcblasStatus_t hcblasCreate(hcblasHandle_t *handle);

// 2.











#endif
