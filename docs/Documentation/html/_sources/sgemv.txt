#####
SGEMV 
#####

| Single Precision real valued general matrix-vector multiplication.
|
| Matrix-Vector products:
|
|    Y := alpha*A*X + beta*Y 
|    Y := alpha*A^T*Y + beta*Y
|
| Where alpha and beta are scalars, A is the matrix and X, Y are vectors.
| () - the actual matrix and ()^T - Transpose of the matrix 


Functions
^^^^^^^^^

Implementation type I
---------------------

 .. note:: **Inputs and Outputs are host float pointers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_sgemv** (`hcblasOrder <HCBLAS_TYPES.html>`_ order, `hcblasTranspose <HCBLAS_TYPES.html>`_ transA , const int M, const int N, const float* alpha, float* A, const long AOffset, const int lda, float* X, const long XOffset, const int incX, const float* beta, float* Y, const long YOffset, const int incY)

Implementation type II
----------------------

 .. note:: **Inputs and Outputs are HC++ float array containers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_sgemv** (Concurrency::accelerator_view &accl_view, `hcblasOrder <HCBLAS_TYPES.html>`_ order, `hcblasTranspose <HCBLAS_TYPES.html>`_ transA, const int M, const int N, const float &alpha, Concurrency::array<float> &A, const long AOffset, const int lda, Concurrency::array<float> &X, const long XOffset, const int incX, const float &beta, Concurrency::array<float> &Y, const long YOffset, const int incY)

Implementation type III
-----------------------

 .. note:: **Inputs and Outputs are HC++ float array containers with batch processing.**
	
`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_sgemv** (Concurrency::accelerator_view &accl_view, `hcblasOrder <HCBLAS_TYPES.html>`_ order, `hcblasTranspose <HCBLAS_TYPES.html>`_ transA, const int M, const int N, const float &alpha, Concurrency::array<float> &A, const long AOffset, const long A_batchOffset, const int lda, Concurrency::array<float> &X, const long XOffset, const long X_batchOffset, const int incX, const float &beta, Concurrency::array<float> &Y, const long YOffset, const long Y_batchOffset, const int incY, const int BatchSize)

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

 ::

              hcblasStatus hcblas_sgemv (Concurrency::accelerator_view &accl_view, 
                                         hcblasOrder order, 
                                         hcblasTranspose transA, 
                                         const int M, 
                                         const int N, 
                                         const float &alpha, 
                                         Concurrency::array<float> &A, 
                                         const long AOffset, 
                                         const int lda, 
                                         Concurrency::array<float> &X, 
                                         const long XOffset, 
                                         const int incX, 
                                         const float &beta, 
                                         Concurrency::array<float> &Y, 
                                         const long YOffset, 
                                         const int incY)


+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  accl_view      | `Using accelerator and accelerator_view Objects              |  
|            |                 | <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_   |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	order          | Row/Column order (RowMajor/ColMajor).                        |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	transA         | How Matrix A is to be transposed (0 and 1 for NoTrans and    | 
|            |                 | Trans case respectively).                                    |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	M              | Number of rows in Matrix A.                                  |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	N              | Number of columns in Matrix A.                               |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	alpha          | The factor of Matrix A.                                      |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	A              | Buffer object storing Matrix A.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	AOffset        | Offset of the first element of the Matrix A in the           |
|            |                 | buffer object. Counted in elements.                          |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	lda            | Leading dimension of Matrix A. It cannot be less than N when |
|            |                 | the order parameter is set to RowMajor, or less than M when  |
|            |                 | the parameter is set to ColMajor.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	X	       | Buffer object storing Vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	XOffset        | Offset of first element of Vector X in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incX           | Increment for the elements of X. It cannot be zero           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	beta           | The factor of the Vector Y.                                  |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |	Y              | Buffer object storing the Vector Y.                          |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	YOffset        | Offset of first element of Vector Y in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incY           | Increment for the elements of Y. It cannot be zero.          |
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
|            |                 | Vectors X, Y and Matrix A.                                   |
+------------+-----------------+--------------------------------------------------------------+

|
| Returns
|
|        HCBLAS_SUCCESS on success;
|        HCBLAS_INVALID when either XOffset, YOffset, AOffset or their respective batchOffsets exceeds the size of the respective buffer object; or
|         when M, N, incX or incY is zero or
|         when alpha and beta values are zero.
|        HCBLAS_ERROR on failure.
