#####
DSCAL 
#####

| Scales a double vector by a double constant (Double precision).
|
|    x := alpha*x
|
| Where alpha is a scalar, and x is a n-dimensional vector. 

Functions
^^^^^^^^^

Implementation type I
---------------------

 .. note:: **Inputs and Outputs are host double pointers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_dscal** (const int N, const double* alpha, double* x, const int incx, const long xOffset)

Implementation type II
----------------------

 .. note:: **Inputs and Outputs are HC++ double array containers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_dscal** (Concurrency::accelerator_view &accl_view, const int N, const double &alpha, Concurrency::array<double> &x, const int incx, const long xOffset) 

Implementation type III
-----------------------

 .. note:: **Inputs and Outputs are HC++ double array containers with batch processing.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_dscal** (Concurrency::accelerator_view &accl_view, const int N, const double &alpha, Concurrency::array<double> &x, const int incx, const long xOffset, const long x_batchOffset, const int BatchSize) 

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

::

              hcblasStatus hcblas_dscal (Concurrency::accelerator_view &accl_view,
                                         const int N, 
                                         const double &alpha,
                                         Concurrency::array<double> &x, 
                                         const int incx,
                                         const long xOffset) 


+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  accl_view      | `Using accelerator and accelerator_view Objects              |  
|            |                 | <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_   |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	N              | Number of elements in vector x.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	alpha          | The constant factor for vector x.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |	x              | Buffer object storing vector x.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	xOffset        | Offset of first element of vector x in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incx           | Increment for the elements of x. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+

| Implementation type III has 2 other parameters as follows,
+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  x_batchOffset  | Batch Offset of vector x in buffer object. Offset should be  |
|            |                 | a multiple of n.                                             |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  BatchSize      | The size of batch for vector x.                              |
+------------+-----------------+--------------------------------------------------------------+

|
| Returns, 

==============   ======================
STATUS           DESCRIPTION
==============   ======================
HCBLAS_SUCCESS    Success
HCBLAS_INVALID    N or incx is zero
==============   ====================== 
