#####
SASUM
#####

Absolute sum of values of a vector containing float elements (Single Precision).

Functions
^^^^^^^^^

Implementation type I
---------------------

	**hcblasStatus** **hcblas_sasum** (const int N, float* X, const int incX, const long XOffset, float* Y)
 
|

Implementation type II
----------------------

	**hcblasStatus** **hcblas_sasum** (Concurrency::accelerator_view &accl_view, const int N, Concurrency::array<float> &X, const int incX, const long XOffset, float &Y)

|

Implementation type III
-----------------------

	**hcblasStatus** **hcblas_sasum** (Concurrency::accelerator_view &accl_view, const int N, Concurrency::array<float> &X, const int incX, const long XOffset, float &Y, const long X_batchOffset, const int BatchSize)

|

Detailed Description
^^^^^^^^^^^^^^^^^^^^

Function Documentation
^^^^^^^^^^^^^^^^^^^^^^

::

             hcblasStatus hcblas_sasum (Concurrency::accelerator_view &accl_view, 
                                        const int N,
                                        Concurrency::array<float> &X, 
                                        const int incX,
                                        const long XOffset, 
                                        float &Y) 


+------------+-----------------+--------------------------------------------------------------+
|  In/out    |  Parameters     | Description                                                  |
+============+=================+==============================================================+
|    [in]    |  accl_view      | `Using accelerator and accelerator_view Objects              |  
|            |                 | <https://msdn.microsoft.com/en-us/library/hh873132.aspx>`_   |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	N              | Number of elements in vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    | 	X              | Buffer object storing vector X.                              |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |  incX           | Increment for the elements of X. Must not be zero.           |
+------------+-----------------+--------------------------------------------------------------+
|    [in]    |	XOffset	       | Offset of first element of vector X in buffer object.        |
|            |                 | Counted in elements.                                         |
+------------+-----------------+--------------------------------------------------------------+
|    [out]   |  Y              | Buffer object that will contain the absolute sum value       |
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
