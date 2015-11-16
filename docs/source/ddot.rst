####
DDOT
####

| Dot product of two vectors (vectors x and y) containing double elements (Double precision Dot product).
|
| Where x, y are n-dimensional vectors.

Functions
^^^^^^^^^

Implementation type I
---------------------

 .. note:: **Inputs and Outputs are host double pointers.**

`hcblasStatus <HCBLAS_TyPES.html>`_ **hcblas_ddot** (const int N, double* x, const int incx, const long xOffset, double* y, const int incy, const long yOffset, double* dot)

Implementation type II
----------------------

 .. note:: **Inputs and Outputs are HC++ double array containers.**

`hcblasStatus <HCBLAS_TyPES.html>`_ **hcblas_ddot** (Concurrency::accelerator_view &accl_view, const int N, Concurrency::array<double> &x, const int incx, const long xOffset, Concurrency::array<double> &y, const int incy, const long yOffset, double &dot) 

Implementation type III
-----------------------

 .. note:: **Inputs and Outputs are HC++ double array containers with batch processing.**

`hcblasStatus <HCBLAS_TyPES.html>`_ **hcblas_ddot** (Concurrency::accelerator_view &accl_view, const int N, Concurrency::array<double> &x, const int incx, const long xOffset, Concurrency::array<double> &y, const int incy, const long yOffset, double &dot, const long x_batchOffset, const long y_batchOffset, const int BatchSize) 

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

::

             hcblasStatus hcblas_ddot (Concurrency::accelerator_view &accl_view, 
                                       const int N,
                                       Concurrency::array<double> &x, 
                                       const int incx, 
                                       const long xOffset,
                                       Concurrency::array<double> &y, 
                                       const int incy, 
                                       const long yOffset, 
                                       double &dot) 

+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  accl_view      | `Using accelerator and accelerator_view Objects              |  
|            |                 | <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_   |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  N              | Number of elements in vector x.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  x              | Buffer object storing vector x.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  incx           | Increment for the elements of x. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  xOffset        | Offset of first element of vector x in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  y              | Buffer object storing the vector y.                          |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  incy           | Increment for the elements of y. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  yOffset        | Offset of first element of vector y in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |  dot            | Buffer object that will contain the dot-product value.       |
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
|    [in]    |  BatchSize      | The size of batch for vector x and vector y.                 |
+------------+-----------------+--------------------------------------------------------------+

|
| Returns,

==============   ===========================
STATUS           DESCRIPTION
==============   ===========================
HCBLAS_SUCCESS    Success
HCBLAS_INVALID    N, incx or incy is zero
==============   =========================== 
