#ifndef HCBLAS_H
#define HCBLAS_H

// Type hcblasStatus

// The type  hcblasStatus  is used for function status returns. HCBLAS 
// helper functions return status directly, while the status of HCBLAS 
// core functions can be retrieved via  hcblasGetError() . Currently, the 
// following values are defined: 

typedef enum hcblasStatus_t {
  HCBLAS_STATUS_NOT_INITIALIZED  // HCBLAS library not initialized
  HCBLAS_STATUS_ALLOC_FAILED     // resource allocation failed
  HCBLAS_STATUS_INVALID_VALUE    // unsupported numerical value was passed to function
  HCBLAS_STATUS_MAPPING_ERROR    // access to GPU memory space failed
  HCBLAS_STATUS_EXECUTION_FAILED // GPU program failed to execute
  HCBLAS_STATUS_INTERNAL_ERROR   // an internal HCBLAS operation failed
} hcblasStatus;



#endif
