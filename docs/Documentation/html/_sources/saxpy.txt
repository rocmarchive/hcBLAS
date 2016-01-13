#####
SAXPY 
#####

| Scale vector x of float elements and add to y.
|
|    y := alpha*x + y 
|
| Where alpha is a scalar, and x, y are n-dimensional vectors.

Implementation type I
---------------------

 .. note:: **Inputs and Outputs are host float pointers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_saxpy** (const int N, const float* alpha, float* x, const int incx, float* y, const int incy, const long xOffset, const long yOffset)                                     

Implementation type II
----------------------

 .. note:: **Inputs and Outputs are HC++ float array containers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_saxpy** (hc::accelerator_view &accl_view, const int N, const float &alpha, hc::array<float> &x, const int incx, hc::array<float> &y, const int incy, const long xOffset, const long yOffset)

Implementation type III
-----------------------

 .. note:: **Inputs and Outputs are HC++ float array containers with batch processing.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_saxpy** (hc::accelerator_view &accl_view, const int N, const float &alpha, hc::array<float> &x, const int incx, const long x_batchOffset, hc::array<float> &y, const int incy, const long y_batchOffset, const long xOffset, const long yOffset, const int BatchSize) 

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

 ::

              hcblasStatus hcblas_saxpy (hc::accelerator_view &accl_view,
                                         const int N, 
                                         const float &alpha,
                                         hc::array<float> &x, 
                                         const int incx,
                                         hc::array<float> &y, 
                                         const int incy,
                                         const long xOffset, 
                                         const long yOffset) 



+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  accl_view      | `Using accelerator and accelerator_view Objects              |  
|            |                 | <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_   |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	N	       | Number of elements in vector x.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	alpha          | The constant factor for vector x.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	x              | Buffer object storing vector x.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  incx           | Increment for the elements of x. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |	y	       | Buffer object storing the vector y.                          |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  incy           | Increment for the elements of y. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  xOffset        | Offset of first element of vector x in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  yOffset	       | Offset of first element of vector y in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+

| Implementation type III has 3 other parameters as follows,
+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  x_batchOffset  | Batch Offset of vector x in buffer object. Offset should be  |
|            |                 | a multiple of n.                                             |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  y_batchOffset  | Batch Offset of vector y in buffer object. Offset should be  |
|            |                 | a multiple of n.                                             |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  BatchSize      | The size of batch for vector x and Output vector y.          |
+------------+-----------------+--------------------------------------------------------------+

|
| Returns,

==============   ===========================
STATUS           DESCRIPTION
==============   ===========================
HCBLAS_SUCCESS    Success
HCBLAS_INVALID    N, incx or incy is zero
==============   =========================== 
