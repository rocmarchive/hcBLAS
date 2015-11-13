####
SGER 
####

| Vector-vector product with float elements and performs the rank 1 operation (Single precision).
|
| Vector-vector products:
|
|    A := alpha*x*y^T + A
|
| Where alpha is a scalar, A is the matrix and x, y are vectors.
| () - the actual matrix and ()^T - transpose of the matrix
 

Functions
^^^^^^^^^

Implementation type I
---------------------

 .. note:: **Inputs and Outputs are host float pointers.**

`hcblasStatus <HCBLAS_TyPES.html>`_ **hcblas_sger** (`hcblasOrder <HCBLAS_TyPES.html>`_ order, const int M, const int N, const float* alpha, float* x, const long xOffset, const int incx, float* y, const long yOffset, const int incy, float* A, const long AOffset, const int lda) 

Implementation type II
----------------------

 .. note:: **Inputs and Outputs are HC++ float array containers.**

`hcblasStatus <HCBLAS_TyPES.html>`_ **hcblas_sger** (Concurrency::accelerator_view &accl_view, `hcblasOrder <HCBLAS_TyPES.html>`_ order, const int M, const int N, const float &alpha, Concurrency::array<float> &x, const long xOffset, const int incx, Concurrency::array<float> &y, const long yOffset, const int incy, Concurrency::array<float> &A, const long AOffset, const int lda)

Implementation type III
-----------------------

 .. note:: **Inputs and Outputs are HC++ float array containers with batch processing.**

`hcblasStatus <HCBLAS_TyPES.html>`_ **hcblas_sger** (Concurrency::accelerator_view &accl_view, `hcblasOrder <HCBLAS_TyPES.html>`_ order, const int M, const int N, const float &alpha, Concurrency::array<float> &x, const long xOffset, const long x_batchOffset, const int incx, Concurrency::array<float> &y, const long yOffset, const long y_batchOffset, const int incy, Concurrency::array<float> &A, const long AOffset, const long A_batchOffset, const int lda, int BatchSize)  

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
                                        Concurrency::array<float> &x, 
                                        const long xOffset, 
                                        const int incx,
                                        Concurrency::array<float> &y, 
                                        const long yOffset, 
                                        const int incy,
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
|    [in]    |	M              | Number of rows in matrix A.                                  |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	N	       | Number of columns in matrix A.                               |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	alpha	       | specifies the scalar alpha.                                  |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	x              | Buffer object storing vector x.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    | 	xOffset        | Offset in number of elements for the first element           |
|            |                 | in vector x.                                                 |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incx	       | Increment for the elements of x. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	y	       | Buffer object storing vector y.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	yOffset	       | Offset in number of elements for the first element           |
|            |                 | in vector y.                                                 |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incy	       | Increment for the elements of y. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   | 	A              | Buffer object storing matrix A. On exit, A is overwritten    |
|            |                 | by the updated matrix.                                       |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	AOffset        | Offset in number of elements for the first element           |
|            |                 | in matrix A.                                                 |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	lda	       | Leading dimension of matrix A. It cannot be less than N when |
|            |                 | the order parameter is set to RowMajor, or less than M       |
|            |                 | when the parameter is set to ColMajor.                       |
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
|            |                 | vectors x, y and Output matrix A.                            |
+------------+-----------------+--------------------------------------------------------------+

|
| Returns, 

==============   ===========================
STATUS           DESCRIPTION
==============   ===========================
HCBLAS_SUCCESS    Success
HCBLAS_INVALID    M, N, incx or incy is zero
HCBLAS_ERROR      Failure
==============   ===========================  
