#####
CGEMM 
#####

| Complex valued general matrix-matrix multiplication.
|
| Matrix-matrix products:
|
|    C := alpha*A*B     + beta*C 
|    C := alpha*A^T*B   + beta*C 
|    C := alpha*A*B^T   + beta*C 
|    C := alpha*A^T*B^T + beta*C 
|
| Where alpha and beta are scalars, and A, B and C are matrices.
| matrix A - m by k matrix
| matrix B - k by n matrix
| matrix C - m by n matrix
| () - the actual matrix and ()^T - Transpose of the matrix 

Functions
^^^^^^^^^

Implementation type I
---------------------

	 **hcblasStatus** **hcblas_cgemm** (const enum **HCBLAS_ORDER** order, const enum **HCBLAS_TRANS** transA, const enum **HCBLAS_TRANS** transB, const int M, const int N, const int K, const **hcComplex*** alpha, const **hcComplex*** A, long AOffset, long lda, const **hcComplex*** B, long BOffset, long ldb, const **hcComplex*** beta, **hcComplex*** C, long COffset, long ldc) 

|

Implementation type II
----------------------

	 **hcblasStatus** **hcblas_cgemm** (Concurrency::accelerator_view &accl_view, const enum **HCBLAS_ORDER** order, const enum **HCBLAS_TRANS** transA, const enum **HCBLAS_TRANS** transB, const int M, const int N, const int K, const Concurrency::graphics::float_2 &alpha, Concurrency::array<float_2> &A, long AOffset, long lda, Concurrency::array<float_2> &B, long BOffset, long ldb, const Concurrency::graphics::float_2 &beta, Concurrency::array<float_2> &C, long COffset, long ldc) 

|

Implementation type III
-----------------------

	 **hcblasStatus** **hcblas_cgemm** (Concurrency::accelerator_view &accl_view, const enum **HCBLAS_ORDER** order, const enum **HCBLAS_TRANS** transA, const enum **HCBLAS_TRANS** transB, const int M, const int N, const int K, const Concurrency::graphics::float_2 &alpha, Concurrency::array<float_2> &A, const long AOffset, const long A_batchOffset, const long lda, Concurrency::array<float_2> &B, const long BOffset, const long B_batchOffset, const long ldb, const Concurrency::graphics::float_2 &beta, Concurrency::array<float_2> &C, const long COffset, const long C_batchOffset, const long ldc, const int BatchSize)

|

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

 ::

             hcblasStatus hcblas_cgemm (Concurrency::accelerator_view &accl_view, 
                                        const enum HCBLAS_ORDER order, 
                                        const enum HCBLAS_TRANS transA, 
                                        const enum HCBLAS_TRANS transB, 
                                        const int M, 
                                        const int N, 
                                        const int K, 
                                        const Concurrency::graphics::float_2 &alpha, 
                                        Concurrency::array<float_2> &A, 
                                        long AOffset, 
                                        long lda, 
                                        Concurrency::array<float_2> &B, 
                                        long BOffset, 
                                        long ldb, 
                                        const Concurrency::graphics::float_2 &beta, 
                                        Concurrency::array<float_2> &C, 
                                        long COffset, 
                                        long ldc)

+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |    accl_view    |  `Using accelerator and accelerator_view Objects             |  
|            |                 |  <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_  |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    order        | Row/column order (rowMajor/colMajor)                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    transA       | How matrix A is to be transposed (0 and 1 for noTrans        |
|            |                 | and Trans case respectively).                                |                            
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    transB       | How matrix B is to be transposed (0 and 1 for noTrans        |
|            |                 | and Trans case respectively).                                |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    M            | Number of rows in matrix A.                                  |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    N            | Number of columns in matrix B.                               |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    K            | Number of columns in matrix A and rows in matrix B.          |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    alpha        | The factor of matrix A.                                      |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    A            | Buffer object storing matrix A.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    AOffset      | Offset of the first element of the matrix A in the buffer    |
|            |                 | object. Counted in elements.                                 |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    lda          | Leading dimension of matrix A. It cannot be less than K when |
|            |                 | the order parameter is set to rowMajor, or less than M when  |
|            |                 | the parameter is set to colMajor.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    B            | Buffer object storing matrix B.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    BOffset      | Offset of the first element of the matrix B in the buffer    |
|            |                 | object. Counted in elements.                                 |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    ldb          | Leading dimension of matrix B. It cannot be less than N when |
|            |                 | the order parameter is set to rowMajor, or less than K when  |
|            |                 | it is set to colMajor.                                       |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    beta         | The factor of matrix C.                                      |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |    C            | Buffer object storing matrix C.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    COffset      | Offset of the first element of the matrix C in the buffer    |
|            |                 | object. Counted in elements.                                 |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |    ldc          | Leading dimension of matrix C. It cannot be less than N when |
|            |                 | the order parameter is set to rowMajor, or less than M when  |
|            |                 | it is set to colMajor.                                       |
+------------+-----------------+--------------------------------------------------------------+  

| Implementation type III has 4 other parameters as follows
+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  A_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the matrix A in the buffer object. Counted in elements.      |
|            |                 | Offset should be a multiple of m by k.                       |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  B_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the matrix B in the buffer object. Counted in elements.      |
|            |                 | Offset should be a multiple of n by k.                       |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  C_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the matrix C in the buffer object. Counted in elements.      |
|            |                 | Offset should be a multiple of m by n.                       |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  BatchSize      | The size of batch of threads to be processed in parallel for |
|            |                 | Matrices A, B and Output Matrix C.                           |
+------------+-----------------+--------------------------------------------------------------+

|
| Returns
|
|        HCBLAS_SUCCESS on success;
|        HCBLAS_INVALID when either AOffset, BOffset, COffset or their respective batchOffsets exceeds the size of the respective buffer object; or
|         when M or N is zero or
|         when alpha or beta values are not appropriate.
|        HCBLAS_ERROR on failure.
