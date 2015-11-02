HCBLAS TYPES
############

Enumerations
^^^^^^^^^^^^

| enum hcblasStatus { HCBLAS_SUCCESS = 0, HCBLAS_INVALID = -1, HCBLAS_ERROR = -2}
| enum HCBLAS_ORDER { rowMajor, colMajor}
| enum HCBLAS_TRANS { noTrans = 'n', trans = 't'}

Structures
^^^^^^^^^^

| struct hcComplex { float real; float img;}

Detailed Description
^^^^^^^^^^^^^^^^^^^^

HCBLAS STATUS (hcblasStatus)
----------------------------

| This enumeration is the set of HCBLAS error codes.
+----------------+--------------------------------------------------------------------------------+
| Enumerator                                                                                      |
+================+================================================================================+
| HCBLAS_SUCCESS | Kernel completed its work successfully.                                        |
+----------------+--------------------------------------------------------------------------------+    
| HCBLAS_INVALID | Inputs are either not approprite or exceeds the buffer object.                 |
+----------------+--------------------------------------------------------------------------------+
| HCBLAS_ERROR   | Kernel call failure.                                                           |
+----------------+--------------------------------------------------------------------------------+

|
HCBLAS_ORDER
------------

| Shows how matrices are placed in memory.
+------------+--------------------------------------------------------------------------------+
| Enumerator                                                                                  |
+============+================================================================================+
| rowMajor   | Every row is placed sequentially.                                              |
+------------+--------------------------------------------------------------------------------+    
| colMajor   | Every column is placed sequentially.                                           |
+------------+--------------------------------------------------------------------------------+

|

HCBLAS_TRANS
------------

| Used to specify whether the matrix is to be transposed or not. 
+------------+--------------------------------------------------------------------------------+
| Enumerator                                                                                  |
+============+================================================================================+
| noTrans    | Operate with the matrix.                                                       |
+------------+--------------------------------------------------------------------------------+    
| trans      | Operate with the transpose of the matrix.                                      |
+------------+--------------------------------------------------------------------------------+
