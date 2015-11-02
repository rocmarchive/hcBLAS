#####
SAXPY 
#####

| Scale vector X of float elements and add to Y.
|
|    Y := alpha*X + Y 
|
| Where alpha is a scalar, and x, y are vectors of size n.

Implementation type I
---------------------

	**hcblasStatus** **hcblas_saxpy** (const int N, const float* alpha, float* X, const int incX, float* Y, const int incY, const long XOffset, const long YOffset)                                     
 
|

Implementation type II
----------------------

	**hcblasStatus** **hcblas_saxpy** (Concurrency::accelerator_view &accl_view, const int N, const float &alpha, Concurrency::array<float> &X, const int incX, Concurrency::array<float> &Y, const int incY, const long XOffset, const long YOffset)
 
|

Implementation type III
-----------------------

	**hcblasStatus** **hcblas_saxpy** (Concurrency::accelerator_view &accl_view, const int N, const float &alpha, Concurrency::array<float> &X, const int incX, const long X_batchOffset, Concurrency::array<float> &Y, const int incY, const long Y_batchOffset, const long XOffset, const long YOffset, const int BatchSize) 

|

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

 ::

              hcblasStatus hcblas_saxpy (Concurrency::accelerator_view &accl_view,
                                         const int N, 
                                         const float &alpha,
                                         Concurrency::array<float> &X, 
                                         const int incX,
                                         Concurrency::array<float> &Y, 
                                         const int incY,
                                         const long XOffset, 
                                         const long YOffset) 



+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  accl_view      | `Using accelerator and accelerator_view Objects              |  
|            |                 | <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_   |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	N	       | Number of elements in vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	alpha          | The constant factor for vector X.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	X              | Buffer object storing vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  incX           | Increment for the elements of X. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |	Y	       | Buffer object storing the vector Y.                          |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  incY           | Increment for the elements of Y. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  XOffset        | Offset of first element of vector X in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  YOffset	       | Offset of first element of vector Y in buffer object.        |
|            |                 | Counted in elements.                                         |
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
|    [in]    |  BatchSize      | The size of batch for vector X and Output vector Y.          |
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
