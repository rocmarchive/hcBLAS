####
DDOT
####

Dot product of two vectors containing double elements (Double Precision Dot product).

Functions
^^^^^^^^^

Implementation type I
---------------------

	**hcblasStatus** **hcblas_ddot** (const int N, double* X, const int incX, const long XOffset, double* Y, const int incY, const long YOffset, double* dot)

|

Implementation type II
----------------------

	**hcblasStatus** **hcblas_ddot** (Concurrency::accelerator_view &accl_view, const int N, Concurrency::array<double> &X, const int incX, const long XOffset, Concurrency::array<double> &Y, const int incY, const long YOffset, double &dot) 

|

Implementation type III
-----------------------

	**hcblasStatus** **hcblas_ddot** (Concurrency::accelerator_view &accl_view, const int N, Concurrency::array<double> &X, const int incX, const long XOffset, Concurrency::array<double> &Y, const int incY, const long YOffset, double &dot, const long X_batchOffset, const long Y_batchOffset, const int BatchSize) 

|

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

::

             hcblasStatus hcblas_ddot (Concurrency::accelerator_view &accl_view, 
                                       const int N,
                                       Concurrency::array<double> &X, 
                                       const int incX, 
                                       const long XOffset,
                                       Concurrency::array<double> &Y, 
                                       const int incY, 
                                       const long YOffset, 
                                       double &dot) 

+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  accl_view      | `Using accelerator and accelerator_view Objects              |  
|            |                 | <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_   |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  N              | Number of elements in vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  X              | Buffer object storing vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  incX           | Increment for the elements of X. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  XOffset        | Offset of first element of vector X in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  Y              | Buffer object storing the vector Y.                          |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  incY           | Increment for the elements of Y. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  YOffset        | Offset of first element of vector Y in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |  dot            | Buffer object that will contain the dot-product value        |
+------------+-----------------+--------------------------------------------------------------+

| Implementation type III has 3 other parameters as follows
+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  X_batchOffset  | Batch Offset of vector X in buffer object. Offset should be  |
|            |                 | a multiple of n.                                             |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  Y_batchOffset  | Batch Offset of vector Y in buffer object. Offset should be  |
|            |                 | a multiple of n.                                             |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  BatchSize      | The size of batch for vector X and vector Y.                 |
+------------+-----------------+--------------------------------------------------------------+

|
| Returns
|
|        HCBLAS_SUCCESS on success
|        HCBLAS_INVALID when
|         N is zero, or
|         either incX or incY is zero, or
|         the vector sizes along with the increments lead to accessing outside of any of the buffers;
|        HCBLAS_ERROR on failure.
