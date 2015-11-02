#####
SSCAL 
#####

| Scales a float vector by a float constant (Single Precision).
|
|    X := alpha*X 
|
| Where alpha is a scalar, and X is a vector of size n.


Functions
^^^^^^^^^

Implementation type I
---------------------

	**hcblasStatus** **hcblas_sscal** (const int N, const float* alpha, float* X, const int incX, const long XOffset)
 
|

Implementation type II
----------------------

	**hcblasStatus** **hcblas_sscal** (Concurrency::accelerator_view &accl_view, const int N, const float &alpha, Concurrency::array<float> &X, const int incX, const long XOffset)

|

Implementation type III
-----------------------

	**hcblasStatus** **hcblas_sscal** (Concurrency::accelerator_view &accl_view, const int N, const float &alpha, Concurrency::array<float> &X, const int incX, const long XOffset, const long X_batchOffset, const int BatchSize) 

|

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

::

             hcblasStatus hcblas_sscal (Concurrency::accelerator_view &accl_view, 
                                        const int N, 
                                        const float &alpha, 
                                        Concurrency::array<float> &X, 
                                        const int incX, 
                                        const long XOffset)


+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  accl_view      | `Using accelerator and accelerator_view Objects              |  
|            |                 | <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_   |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	N              | Number of elements in vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  alpha          | The constant factor for vector X.                            |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |	X              | Buffer object storing vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  XOffset        | Offset of first element of vector X in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	incX	       | Increment for the elements of X. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+

| Implementation type III has 2 other parameters as follows
+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  X_batchOffset  | Batch Offset of vector X in buffer object. Offset should be  |
|            |                 | a multiple of n.                                             |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  BatchSize      | The size of batch for vector X.                              |
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
