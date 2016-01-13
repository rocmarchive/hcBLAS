#####
SGEMV 
#####

| Single Precision real valued general matrix-vector multiplication.
|
| Matrix-vector products:
|
|    y := alpha*A*x + beta*y 
|    y := alpha*A^T*y + beta*y
|
| Where alpha and beta are scalars, A is the matrix and x, y are vectors.
| () - the actual matrix and ()^T - Transpose of the matrix 


Functions
^^^^^^^^^

Implementation type I
---------------------

 .. note:: **Inputs and Outputs are host float pointers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_sgemv** (`hcblasOrder <HCBLAS_TYPES.html>`_ order, `hcblasTranspose <HCBLAS_TYPES.html>`_ transA , const int M, const int N, const float* alpha, float* A, const long AOffset, const int lda, float* x, const long xOffset, const int incx, const float* beta, float* y, const long yOffset, const int incy)

Implementation type II
----------------------

 .. note:: **Inputs and Outputs are HC++ float array containers.**

`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_sgemv** (hc::accelerator_view &accl_view, `hcblasOrder <HCBLAS_TYPES.html>`_ order, `hcblasTranspose <HCBLAS_TYPES.html>`_ transA, const int M, const int N, const float &alpha, hc::array<float> &A, const long AOffset, const int lda, hc::array<float> &x, const long xOffset, const int incx, const float &beta, hc::array<float> &y, const long yOffset, const int incy)

Implementation type III
-----------------------

 .. note:: **Inputs and Outputs are HC++ float array containers with batch processing.**
	
`hcblasStatus <HCBLAS_TYPES.html>`_ **hcblas_sgemv** (hc::accelerator_view &accl_view, `hcblasOrder <HCBLAS_TYPES.html>`_ order, `hcblasTranspose <HCBLAS_TYPES.html>`_ transA, const int M, const int N, const float &alpha, hc::array<float> &A, const long AOffset, const long A_batchOffset, const int lda, hc::array<float> &x, const long xOffset, const long x_batchOffset, const int incx, const float &beta, hc::array<float> &y, const long yOffset, const long y_batchOffset, const int incy, const int BatchSize)

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

 ::

              hcblasStatus hcblas_sgemv (hc::accelerator_view &accl_view, 
                                         hcblasOrder order, 
                                         hcblasTranspose transA, 
                                         const int M, 
                                         const int N, 
                                         const float &alpha, 
                                         hc::array<float> &A, 
                                         const long AOffset, 
                                         const int lda, 
                                         hc::array<float> &x, 
                                         const long xOffset, 
                                         const int incx, 
                                         const float &beta, 
                                         hc::array<float> &y, 
                                         const long yOffset, 
                                         const int incy)


+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  accl_view      | `Using accelerator and accelerator_view Objects              |  
|            |                 | <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_   |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	order          | Row/Column order (RowMajor/ColMajor).                        |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	transA         | How matrix A is to be transposed (0 and 1 for NoTrans and    | 
|            |                 | Trans case respectively).                                    |
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
|            |                 | the order parameter is set to RowMajor, or less than M when  |
|            |                 | the parameter is set to ColMajor.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	x	       | Buffer object storing vector x.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	xOffset        | Offset of first element of vector x in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incx           | Increment for the elements of x. It cannot be zero           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	beta           | The factor of the vector y.                                  |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |	y              | Buffer object storing the vector y.                          |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	yOffset        | Offset of first element of vector y in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incy           | Increment for the elements of y. It cannot be zero.          |
+------------+-----------------+--------------------------------------------------------------+

| Implementation type III has 4 other parameters as follows,
+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  A_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the matrix A in the buffer object. Counted in elements.      |
|            |                 | Offset should be a multiple of m by n.                       |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  x_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the vector x in the buffer object. Counted in elements.      |
|            |                 | Offset should be a multiple of m.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  y_batchOffset  | Batch Offset adding to the Offset of the first element of    |
|            |                 | the vector y in the buffer object. Counted in elements.      |
|            |                 | Offset should be a multiple of n.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  BatchSize      | The size of batch of threads to be processed in parallel for |
|            |                 | vectors x, y and matrix A.                                   |
+------------+-----------------+--------------------------------------------------------------+

|
| Returns, 

==============   ===========================
STATUS           DESCRIPTION
==============   ===========================
HCBLAS_SUCCESS    Success
HCBLAS_INVALID    M, N, incx or incy is zero
==============   ===========================  
