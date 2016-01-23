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

// 5. hcblasSetMatrix()

// This function copies a tile of rows x cols elements from a matrix A in host memory space to a 
// matrix B in GPU memory space. It is assumed that each element requires storage of elemSize bytes 
// and that both matrices are stored in column-major format, with the leading dimension of the source 
// matrix A and destination matrix B given in lda and ldb, respectively.

// Return Values
// --------------------------------------------------------------------
// HCBLAS_STATUS_SUCCESS            the operation completed successfully
// HCBLAS_STATUS_NOT_INITIALIZED    the library was not initialized
// HCBLAS_STATUS_INVALID_VALUE      the parameters rows, cols<0 or elemSize, lda, ldb<=0
// HCBLAS_STATUS_MAPPING_ERROR      there was an error accessing GPU memory

hcblasStatus_t hcblasSetMatrix(int rows, int cols, int elemSize, const void *A, int lda, void *B, int ldb) {
 
 return HCBLAS_STATUS_SUCCESS;
}

// 6. hcblasGetMatrix()

// This function copies a tile of rows x cols elements from a matrix A in GPU memory space to 
// a matrix B in host memory space. It is assumed that each element requires storage of elemSize 
// bytes and that both matrices are stored in column-major format, with the leading dimension of 
// the source matrix A and destination matrix B given in lda and ldb, respectively.

// Return Values
// --------------------------------------------------------------------
// HCBLAS_STATUS_SUCCESS            the operation completed successfully
// HCBLAS_STATUS_NOT_INITIALIZED    the library was not initialized
// HCBLAS_STATUS_INVALID_VALUE      the parameters rows, cols<0 or elemSize, lda, ldb<=0
// HCBLAS_STATUS_MAPPING_ERROR      there was an error accessing GPU memory

hcblasStatus_t hcblasGetMatrix(int rows, int cols, int elemSize, const void *A, int lda, void *B, int ldb) {

 return HCBLAS_STATUS_SUCCESS;
}

// HCBLAS Level-1 function reference

// Level-1 Basic Linear Algebra Subprograms (BLAS1) functions perform scalar and vector based operations. 
// We will use abbreviations <type> for type and <t> for the corresponding short type to make a more concise 
// and clear presentation of the implemented functions. 
// Unless otherwise specified <type> and <t> have the following meanings:

// <type>       <t>          Meaning
// ---------------------------------------------------
// float     ‘s’ or ‘S’      real single-precision
// double    ‘d’ or ‘D’      real double-precision
// hcComplex ‘c’ or ‘C’      complex single-precision

// The abbreviation Re(.) and Im(.) will stand for the real and imaginary part of a number, respectively.

// 1. hcblas<t>asum()

// This function computes the sum of the absolute values of the elements of vector x.

// Param.       Memory           In/out         Meaning
// -------------------------------------------------------------------------------------
// handle       host             input          handle to the HCBLAS library context.
// n            host             input          number of elements in the vector x.
// x            device           input          <type> vector with elements.
// incx         host             input          stride between consecutive elements of x.
// result       host or device   output         the resulting index, which is 0.0 if n,incx<=0.

// Return Values
// --------------------------------------------------------------------
// HCBLAS_STATUS_SUCCESS           the operation completed successfully
// HCBLAS_STATUS_NOT_INITIALIZED   the library was not initialized
// HCBLAS_STATUS_ALLOC_FAILED      the reduction buffer could not be allocated
// HCBLAS_STATUS_ARCH_MISMATCH     the device does not support double-precision
// HCBLAS_STATUS_EXECUTION_FAILED  the function failed to launch on the GPU

hcblasStatus_t  hcblasSasum(hcblasHandle_t handle, int n,
                            const float           *x, int incx, float  *result) {

return HCBLAS_STATUS_SUCCESS;
}

hcblasStatus_t  hcblasDasum(hcblasHandle_t handle, int n,
                            const double          *x, int incx, double *result) {

return HCBLAS_STATUS_SUCCESS;
}


// 2. hcblas<t>axpy()

// This function multiplies the vector x by the scalar α and adds it to the vector y overwriting 
// the latest vector with the result.

// Param.       Memory           In/out         Meaning
// -------------------------------------------------------------------------------------
// handle       host             input          handle to the HCBLAS library context.
// alpha        host or device   input          <type> scalar used for multiplication.
// n            host             input          number of elements in the vector x and y.
// x            device           input          <type> vector with n elements.
// incx         host             input          stride between consecutive elements of x.
// y            device           in/out         <type> vector with n elements.
// incy         host             input          stride between consecutive elements of y.

// Return Values
// --------------------------------------------------------------------
// HCBLAS_STATUS_SUCCESS           the operation completed successfully
// HCBLAS_STATUS_NOT_INITIALIZED   the library was not initialized
// HCBLAS_STATUS_ARCH_MISMATCH     the device does not support double-precision
// HCBLAS_STATUS_EXECUTION_FAILED  the function failed to launch on the GPU

hcblasStatus_t hcblasSaxpy(hcblasHandle_t handle, int n,
                           const float           *alpha,
                           const float           *x, int incx,
                           float                 *y, int incy) {
 return HCBLAS_STATUS_SUCCESS;
}

