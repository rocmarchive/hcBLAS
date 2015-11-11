#####
DASUM
#####

| Absolute sum of values of a vector (Vector X) containing double elements (Double precision).
|
| Where X is a n-dimensional vector.

Functions
^^^^^^^^^

Implementation type I
---------------------

 .. note:: **Inputs and Outputs are host double pointers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_dasum** (const int N, double* X, const int incX, const long XOffset, double* Y)

Implementation type II
----------------------

 .. note:: **Inputs and Outputs are HC++ double array containers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_dasum** (Concurrency::accelerator_view &accl_view, const int N, Concurrency::array<double> &X, const int incX, const long XOffset, double &Y)

Implementation type III
-----------------------

 .. note:: **Inputs and Outputs are HC++ double array containers with batch processing.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_dasum** (Concurrency::accelerator_view &accl_view, const int N, Concurrency::array<double> &X, const int incX, const long XOffset, double &Y, const long X_batchOffset, const int BatchSize) 

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

::

             hcblasStatus hcblas_dasum (Concurrency::accelerator_view &accl_view, 
                                        const int N,
                                        Concurrency::array<double> &X, 
                                        const int incX,
                                        const long XOffset, 
                                        double &Y) 


+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  accl_view      | `Using accelerator and accelerator_view Objects              |  
|            |                 | <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_   |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  N              | Number of elements in Vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  X              | Buffer object storing Vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  incX           | Increment for the elements of X. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  XOffset        | Offset of first element of Vector X in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |  Y              | Buffer object that will contain the absolute sum value.      |
+------------+-----------------+--------------------------------------------------------------+

| Implementation type III has 2 other parameters as follows,
+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  X_batchOffset  | Batch Offset of Vector X in buffer object. Offset should be  |
|            |                 | a multiple of n.                                             |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  BatchSize      | The size of batch for Vector X.                              |
+------------+-----------------+--------------------------------------------------------------+

|
| Returns
|
|        HCBLAS_SUCCESS on success;
|        HCBLAS_INVALID when
|         N is zero, or
|         incX is zero, or
|         the vector sizes along with the increments lead to accessing outside of any of the buffers;
|        HCBLAS_ERROR on failure.
