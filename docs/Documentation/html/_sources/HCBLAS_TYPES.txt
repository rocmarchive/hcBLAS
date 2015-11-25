############
HCBLAS TYPES
############

Enumerations
^^^^^^^^^^^^

| enum hcblasStatus { HCBLAS_SUCCESS = 0, HCBLAS_INVALID = -1}
| enum hcblasOrder { RowMajor, ColMajor}
| enum hcblasTranspose { NoTrans = 'n', Trans = 't'}

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
| HCBLAS_INVALID | Inputs are either not properly allocated or zero.                              |
+----------------+--------------------------------------------------------------------------------+

|
HCBLAS ORDER (hcblasOrder)
--------------------------

| Shows how matrices are placed in memory.
+------------+--------------------------------------------------------------------------------+
| Enumerator                                                                                  |
+============+================================================================================+
| RowMajor   | Every row is placed sequentially.                                              |
+------------+--------------------------------------------------------------------------------+    
| ColMajor   | Every column is placed sequentially.                                           |
+------------+--------------------------------------------------------------------------------+

|

HCBLAS TRANSPOSE (hcblasTranspose)
----------------------------------

| Used to specify whether the matrix is to be transposed or not. 
+------------+--------------------------------------------------------------------------------+
| Enumerator                                                                                  |
+============+================================================================================+
| NoTrans    | Operate with the matrix.                                                       |
+------------+--------------------------------------------------------------------------------+    
| Trans      | Operate with the transpose of the matrix.                                      |
+------------+--------------------------------------------------------------------------------+
