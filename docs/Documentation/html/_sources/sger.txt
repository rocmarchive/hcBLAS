####
SGER 
####

| Vector-vector product with float elements and performs the rank 1 operation (Single precision).
|
| Vector-Vector products:
|
|    A := alpha*X*Y^T + A
|
| Where alpha is a scalar, A is the matrix and X, Y are vectors.
| () - the actual matrix and ()^T - transpose of the matrix
 

Functions
^^^^^^^^^

Implementation type I
---------------------

 .. note:: **Inputs and Outputs are host float pointers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_sger** (`hcblasOrder <HCBLAS_TYPES.html>`_ order, const int M, const int N, const float* alpha, float* X, const long XOffset, const int incX, float* Y, const long YOffset, const int incY, float* A, const long AOffset, const int lda) 

Implementation type II
----------------------

 .. note:: **Inputs and Outputs are HC++ float array containers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_sger** (Concurrency::accelerator_view &accl_view, `hcblasOrder <HCBLAS_TYPES.html>`_ order, const int M, const int N, const float &alpha, Concurrency::array<float> &X, const long XOffset, const int incX, Concurrency::array<float> &Y, const long YOffset, const int incY, Concurrency::array<float> &A, const long AOffset, const int lda)

Implementation type III
-----------------------

 .. note:: **Inputs and Outputs are HC++ float array containers with batch processing.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_sger** (Concurrency::accelerator_view &accl_view, `hcblasOrder <HCBLAS_TYPES.html>`_ order, const int M, const int N, const float &alpha, Concurrency::array<float> &X, const long XOffset, const long X_batchOffset, const int incX, Concurrency::array<float> &Y, const long YOffset, const long Y_batchOffset, const int incY, Concurrency::array<float> &A, const long AOffset, const long A_batchOffset, const int lda, int BatchSize)  

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

 ::

              hcblasStatus hcblas_sger (Concurrency::accelerator_view &accl_view, 
                                        hcblasOrder order,
                                        const int M, 
                                        const int N, 
                                        const float &alpha,
                                        Concurrency::array<float> &X, 
                                        const long XOffset, 
                                        const int incX,
                                        Concurrency::array<float> &Y, 
                                        const long YOffset, 
                                        const int incY,
                                        Concurrency::array<float> &A, 
                                        const long AOffset, 
                                        const int lda)


+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  accl_view      | `Using accelerator and accelerator_view Objects              |  
|            |                 | <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_   |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	order	       | Row/Column order (RowMajor/ColMajor).                        |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	M              | Number of rows in Matrix A.                                  |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	N	       | Number of columns in Matrix A.                               |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	alpha	       | specifies the scalar alpha.                                  |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	X              | Buffer object storing Vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    | 	XOffset        | Offset in number of elements for the first element           |
|            |                 | in Vector X.                                                 |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incX	       | Increment for the elements of X. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	Y	       | Buffer object storing Vector Y.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	YOffset	       | Offset in number of elements for the first element           |
|            |                 | in Vector Y.                                                 |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incY	       | Increment for the elements of Y. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   | 	A              | Buffer object storing Matrix A. On exit, A is overwritten    |
|            |                 | by the updated matrix.                                       |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	AOffset        | Offset in number of elements for the first element           |
|            |                 | in Matrix A.                                                 |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	lda	       | Leading dimension of Matrix A. It cannot be less than N when |
|            |                 | the order parameter is set to RowMajor, or less than M       |
|            |                 | when the parameter is set to ColMajor.                       |
+------------+-----------------+--------------------------------------------------------------+

| Implementation type III has 4 other parameters as follows,
+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  A_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the Matrix A in the buffer object. Counted in elements.      |
|            |                 | Offset should be a multiple of m by n.                       |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  X_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the Vector X in the buffer object. Counted in elements.      |
|            |                 | Offset should be a multiple of m.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  Y_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the Vector Y in the buffer object. Counted in elements.      |
|            |                 | Offset should be a multiple of n.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  BatchSize      | The size of batch of threads to be processed in parallel for |
|            |                 | Vectors X, Y and Output Matrix A.                            |
+------------+-----------------+--------------------------------------------------------------+

|
| Returns
|
|        HCBLAS_SUCCESS on success;
|        HCBLAS_INVALID when either XOffset, YOffset, AOffset or their respective batchOffsets exceeds the size of the respective buffer object; or
|         when M, N, incX or incY is zero or
|         when alpha and beta values are zero.
|        HCBLAS_ERROR on failure.
