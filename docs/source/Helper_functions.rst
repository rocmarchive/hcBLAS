#######################
HCBLAS Helper functions 
#######################

1. hcblasCreate()
-----------------

hcblasHandle_t* **hcblasCreate** ()

| This function initializes the HCBLAS library and creates a handle to an opaque structure
| holding the HCBLAS library context.
|

2. hcblasDestory()
------------------

`hcblasStatus_t <HCBLAS_TYPES.html>`_ **hcblasDestroy** (hcblasHandle_t* &handle)

| This function releases hardware resources used by the HCBLAS library. 
| This function is usually the last call with a particular handle to the HCBLAS library.
|
| Return Values,

==============================    =============================================
STATUS                            DESCRIPTION
==============================    =============================================
 HCBLAS_STATUS_SUCCESS            the shut down succeeded
 HCBLAS_STATUS_NOT_INITIALIZED    the library was not initialized
==============================    ============================================= 

3. hcblasSetVector()
--------------------

`hcblasStatus_t <HCBLAS_TYPES.html>`_ **hcblasSetVector** (int n, int elemSize, const void* x, int incx, void* y, int incy)

| This function copies n elements from a vector x in host memory space to a vector y in GPU memory space. 
| Elements in both vectors are assumed to have a size of elemSize bytes. The storage spacing between 
| consecutive elements is given by incx for the source vector x and by incy for the destination vector y.
|
| Return Values,

==============================    =============================================
STATUS                            DESCRIPTION
==============================    =============================================
 HCBLAS_STATUS_SUCCESS            the operation completed successfully
 HCBLAS_STATUS_NOT_INITIALIZED    the library was not initialized
 HCBLAS_STATUS_INVALID_VALUE      the parameters incx, incy, elemSize<=0
 HCBLAS_STATUS_MAPPING_ERROR      there was an error accessing GPU memory
==============================    ============================================= 

4. hcblasGetVector()
--------------------

`hcblasStatus_t <HCBLAS_TYPES.html>`_ **hcblasGetVector** (int n, int elemSize, const void* x, int incx, void* y, int incy)

| This function copies n elements from a vector x in GPU memory space to a vector y in host memory space. 
| Elements in both vectors are assumed to have a size of elemSize bytes. The storage spacing between 
| consecutive elements is given by incx for the source vector and incy for the destination vector y.
|
| Return Values,

==============================    =============================================
STATUS                            DESCRIPTION
==============================    =============================================
 HCBLAS_STATUS_SUCCESS            the operation completed successfully
 HCBLAS_STATUS_NOT_INITIALIZED    the library was not initialized
 HCBLAS_STATUS_INVALID_VALUE      the parameters incx, incy, elemSize<=0
 HCBLAS_STATUS_MAPPING_ERROR      there was an error accessing GPU memory
==============================    ============================================= 

5. hcblasSetMatrix()
--------------------

`hcblasStatus_t <HCBLAS_TYPES.html>`_ **hcblasSetMatrix** (int rows, int cols, int elemSize, const void* A, int lda, void* B, int ldb)

| This function copies a tile of rows x cols elements from a matrix A in host memory space to a 
| matrix B in GPU memory space. It is assumed that each element requires storage of elemSize bytes 
| and that both matrices are stored in column-major format, with the leading dimension of the source 
| matrix A and destination matrix B given in lda and ldb, respectively.
|
| Return Values,

==============================    =====================================================
STATUS                            DESCRIPTION
==============================    =====================================================
 HCBLAS_STATUS_SUCCESS            the operation completed successfully
 HCBLAS_STATUS_NOT_INITIALIZED    the library was not initialized
 HCBLAS_STATUS_INVALID_VALUE      the parameters rows, cols<0 or elemSize, lda, ldb<=0
 HCBLAS_STATUS_MAPPING_ERROR      there was an error accessing GPU memory
==============================    ===================================================== 

6. hcblasGetMatrix()
--------------------

`hcblasStatus_t <HCBLAS_TYPES.html>`_ **hcblasGetMatrix** (int rows, int cols, int elemSize, const void* A, int lda, void* B, int ldb)

| This function copies a tile of rows x cols elements from a matrix A in GPU memory space to 
| a matrix B in host memory space. It is assumed that each element requires storage of elemSize 
| bytes and that both matrices are stored in column-major format, with the leading dimension of 
| the source matrix A and destination matrix B given in lda and ldb, respectively.
|
| Return Values,

==============================    =====================================================
STATUS                            DESCRIPTION
==============================    =====================================================
 HCBLAS_STATUS_SUCCESS            the operation completed successfully
 HCBLAS_STATUS_NOT_INITIALIZED    the library was not initialized
 HCBLAS_STATUS_INVALID_VALUE      the parameters rows, cols<0 or elemSize, lda, ldb<=0
 HCBLAS_STATUS_MAPPING_ERROR      there was an error accessing GPU memory
==============================    ===================================================== 

7. hcblasDeviceOrderSelect()
-----------------------

`hcblasStatus_t <HCBLAS_TYPES.html>`_ **hcblasDeviceOrderSelect** (hcblasHandle_t* handle, int deviceId, hcblasOrder order)

| This function allows the user to provide the number of GPU devices and their respective Ids that will participate to the subsequent hcblas API Math function calls. User can select their order of operation in this function (RowMajor/ColMajor).
|
| Return Values,

==============================    =======================================================
STATUS                            DESCRIPTION
==============================    =======================================================
 HCBLAS_STATUS_SUCCESS            user call was sucessful
 HCBLAS_STATUS_INVALID_VALUE      Access to at least one of the device could not be done
 HCBLAS_STATUS_MAPPING_ERROR      there was an error accessing GPU memory
==============================    =======================================================
