#ifndef HCBLAS_H
#define HCBLAS_H

// Type hcblasStatus

// The type  hcblasStatus  is used for function status returns. CUBLAS 
// helper functions return status directly, while the status of CUBLAS 
// core functions can be retrieved via  hcblasGetError() . Currently, the 
// following values are defined: 

typedef enum hcblasStatus_t {
  CUBLAS_STATUS_NOT_INITIALIZED  // CUBLAS library not initialized
  CUBLAS_STATUS_ALLOC_FAILED     // resource allocation failed
  CUBLAS_STATUS_INVALID_VALUE    // unsupported numerical value was passed to function
  CUBLAS_STATUS_MAPPING_ERROR    // access to GPU memory space failed
  CUBLAS_STATUS_EXECUTION_FAILED // GPU program failed to execute
  CUBLAS_STATUS_INTERNAL_ERROR   // an internal CUBLAS operation failed
} hcblasStatus;



#endif
