#####
SGEMV 
#####

| Single Precision real valued general matrix-vector multiplication.
|
| Matrix-vector products:
|
|    y := alpha*A*x + beta*y 
|    y := alpha*A^T*x + beta*y
|
| Where alpha and beta are scalars, A is the matrix and x, y are vectors.
| () - the actual matrix and ()^T - Transpose of the matrix 


Functions
^^^^^^^^^

Implementation type I
---------------------

	 **hcblasStatus** **hcblas_sgemv** (const enum **HCBLAS_ORDER** order, const enum **HCBLAS_TRANS** transA , const int M, const int N, const float* alpha, float* A, const long AOffset, const int lda, float* X, const long XOffset, const int incX, const float* beta, float* Y, const long YOffset, const int incY)

|

Implementation type II
----------------------

	 **hcblasStatus** **hcblas_sgemv** (Concurrency::accelerator_view &accl_view, const enum **HCBLAS_ORDER** order, const enum **HCBLAS_TRANS** transA, const int M, const int N, const float &alpha, Concurrency::array<float> &A, const long AOffset, const int lda, Concurrency::array<float> &X, const long XOffset, const int incX, const float &beta, Concurrency::array<float> &Y, const long YOffset, const int incY)

|

Implementation type III
-----------------------
	
	 **hcblasStatus** **hcblas_sgemv** (Concurrency::accelerator_view &accl_view, const enum **HCBLAS_ORDER** order, const enum **HCBLAS_TRANS** transA, const int M, const int N, const float &alpha, Concurrency::array<float> &A, const long AOffset, const long A_batchOffset, const int lda, Concurrency::array<float> &X, const long XOffset, const long X_batchOffset, const int incX, const float &beta, Concurrency::array<float> &Y, const long YOffset, const long Y_batchOffset, const int incY, const int BatchSize)

|

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

 ::

              hcblasStatus hcblas_sgemv (Concurrency::accelerator_view &accl_view, 
                                         const enum HCBLAS_ORDER order, 
                                         const enum HCBLAS_TRANS transA, 
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
|    [in]    |	order          | Row/column order.                                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	transA         | How matrix A is to be transposed.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	M              | Number of rows in matrix A.                                  |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	N              | Number of columns in matrix A.                               |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	alpha          | The factor of matrix A.                                      |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	A              | Buffer object storing matrix A.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	AOffset        | Offset of the first element of the matrix A in the           |
|            |                 | buffer object. Counted in elements.                          |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	lda            | Leading dimension of matrix A. It cannot be less than N when |
|            |                 | the order parameter is set to rowMajor, or less than M when  |
|            |                 | the parameter is set to columnMajor.                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	X	       | Buffer object storing vector x.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	XOffset        | Offset of first element of vector x in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incX           | Increment for the elements of x. It cannot be zero           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	beta           | The factor of the vector y.                                  |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |	Y              | Buffer object storing the vector y.                          |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	YOffset        | Offset of first element of vector y in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incY           | Increment for the elements of y. It cannot be zero.          |
+------------+-----------------+--------------------------------------------------------------+

| Implementation type III has 4 other parameters as follows
+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  A_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the matrix A in the buffer object. Counted in elements.      |
|            |                 | Offset should be a multiple of m by n.                       |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  X_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the vector X in the buffer object. Counted in elements.      |
|            |                 | Offset should be a multiple of m.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  Y_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the vector Y in the buffer object. Counted in elements.      |
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
|         when alpha or beta values are not appropriate.
|        HCBLAS_ERROR on failure.
