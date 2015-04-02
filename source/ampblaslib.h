/*
*
*  FILENAME : ampblas.h
*  This file is the top level header file which includes the Ampblaslilbrary class
*  for performing five blas operations ( saxpy, sger, sgemv, sgemm, cgemm )
*
*/

#include <iostream>

using namespace std;
/* enumerator to indicate the status of  blas operation */
enum ampblasStatus {
    AMPBLAS_SUCCESS = 0,
    AMPBLAS_INVALID = -1,
    AMPBLAS_ERROR = -2
};

/* enumerator to define the layout of  input matrix for blas operation */
enum AMPBLAS_ORDER {
    rowMajor ,
    colMajor
};

/* enumerator to define the type of operation to be performed on the input matrix
 ( NO_TRANSPOSE, TRANSPOSE, CONJUGATE) */
enum AMPBLAS_TRANS {
    noTrans = 'n',
    trans = 't',
    conjugate = 'c'
};

struct ampComplex
{
     float real;
     float img;
};

/* Class which implements the blas ( SGEMM, CGEMM, SGEMV, SGER, SAXPY )  */
class Ampblaslibrary
{
    public:
/*                  Y = alpha * X + Y                                    */
    ampblasStatus  ampblas_saxpy(const int N,
                                 const float *alpha, float *X, const int incX,
                                 float *Y, const int incY, long xOffset,
                                 long yOffset);

/*                  A = alpha * X * Y' + A                               */
    ampblasStatus ampblas_sger(const int M, const int N, const float *alpha,
                               float *X, const long xOffset, const int incX,
                               float *Y, const long yOffset, const int incY,
                               float *A, const long aOffset, const int lda);

/*                  Y = alpha * op(A) * X + beta * Y                     */
   ampblasStatus  ampblas_sgemv(const enum AMPBLAS_TRANS type, const int M,
                                const int N, const float *alpha, float *A,
                                const long aOffset,const int lda, float *X,
                                const long xOffset, const int incX,
                                const float *beta, float *Y,const long yOffset,
                                const int incY);

/*                  C = alpha * op(A) * op(B) + beta * C                 */
    ampblasStatus  ampblas_sgemm(const enum AMPBLAS_TRANS typeA,
                                 const enum AMPBLAS_TRANS typeB, const int M,
                                 const int N, const int K, const float *alpha,
                                 float *A, const long lda, float *B,
                                 const long ldb, const float *beta, float *C,
                                 const long ldc, const long aOffset,
                                 const long bOffset, const long cOffset);

/*                  C = alpha * op(A) * op(B) + beta * C                  */
    ampblasStatus  ampblas_cgemm(char transA, char transB, const int M, 
                                 const int N, const int K,
                                 const ampComplex *alpha,
                                 const ampComplex *A, long aOffset, long lda,
                                 const ampComplex *B, long bOffset, long ldb,
                                 const ampComplex *beta, ampComplex *C,
                                 long cOffset, long ldc);
};
