/*
*
*  FILENAME : ampblas.h
*  This file is the top level header file which includes the Ampblaslilbrary class
*  for performing five blas operations ( saxpy, sger, sgemv, sgemm, cgemm )
*
*/

#ifndef AMPBLASLIB_H
#define AMPBLASLIB_H

#include <iostream>
#include <amp.h>
#include <amp_short_vectors.h>

using namespace Concurrency;
using namespace Concurrency::graphics;
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
    ampblasStatus ampblas_saxpy(const int N,
                                const float *alpha, float *X, const int incX,
                                float *Y, const int incY, const long xOffset,
                                const long yOffset);

/* SAXPY - Overloaded function with arguments of type Concurrency::array_view */

    ampblasStatus ampblas_saxpy(Concurrency::accelerator_view &accl_view,
				const int N, const float &alpha,
				Concurrency::array_view<float> &X, const int incX,
                                Concurrency::array_view<float> &Y, const int incY, 
				const long xOffset, const long yOffset);

/* SAXPY - Overloaded function with arguments related to batch processing */

    ampblasStatus ampblas_saxpy(Concurrency::accelerator_view &accl_view,
                                const int N, const float &alpha,
                                Concurrency::array_view<float> &X, const int incX, const long X_batchOffset,
                                Concurrency::array_view<float> &Y, const int incY, const long Y_batchOffset,
                                const long xOffset, const long yOffset, const int batchSize);


/*                  A = alpha * X * Y' + A                               */
    ampblasStatus ampblas_sger(const enum AMPBLAS_ORDER order, const int M, const int N, const float *alpha,
                               float *X, const long xOffset, const int incX,
                               float *Y, const long yOffset, const int incY,
                               float *A, const long aOffset, const int lda);

/* SGER - Overloaded function with arguments of type Concurrency::array_view */

    ampblasStatus ampblas_sger(Concurrency::accelerator_view &accl_view,
			       const enum AMPBLAS_ORDER order, const int M, const int N, const float &alpha,
                               Concurrency::array_view<float> &X, const long xOffset, const int incX,
                               Concurrency::array_view<float> &Y, const long yOffset, const int incY,
                               Concurrency::array_view<float> &A, const long aOffset, const int lda);

/* SGER - Overloaded function with arguments related to batch processing */

    ampblasStatus ampblas_sger(Concurrency::accelerator_view &accl_view,
                               const enum AMPBLAS_ORDER order, const int M, const int N, const float &alpha,
                               Concurrency::array_view<float> &X, 
                               const long xOffset, const long X_batchOffset, const int incX,
                               Concurrency::array_view<float> &Y, 
                               const long yOffset, const long Y_batchOffset, const int incY,
                               Concurrency::array_view<float> &A, 
                               const long aOffset, const long A_batchOffset, const int lda, const int batchSize);

/*                  Y = alpha * op(A) * X + beta * Y                     */
    ampblasStatus ampblas_sgemv(const enum AMPBLAS_ORDER order, const enum AMPBLAS_TRANS type, const int M,
                                const int N, const float *alpha, float *A,
                                const long aOffset,const int lda, float *X,
                                const long xOffset, const int incX,
                                const float *beta, float *Y,const long yOffset,
                                const int incY);

/* SGEMV- Overloaded function with arguments of type Concurrency::array_view */
    ampblasStatus ampblas_sgemv(Concurrency::accelerator_view &accl_view,
				const enum AMPBLAS_ORDER order, const enum AMPBLAS_TRANS type, const int M,
                                const int N, const float &alpha, 
                                Concurrency::array_view<float> &A, const long aOffset, const int lda, 
				Concurrency::array_view<float> &X, const long xOffset, const int incX,
                                const float &beta,  
				Concurrency::array_view<float> &Y, const long yOffset, const int incY);

/* SGEMV- Overloaded function with arguments related to batch processing */
    ampblasStatus ampblas_sgemv(Concurrency::accelerator_view &accl_view,
                                const enum AMPBLAS_ORDER order, const enum AMPBLAS_TRANS type, const int M,
                                const int N, const float &alpha, Concurrency::array_view<float> &A, 
                                const long aOffset, const long A_batchOffset, const int lda,
                                Concurrency::array_view<float> &X, 
                                const long xOffset, const long X_batchOffset, const int incX,
                                const float &beta, Concurrency::array_view<float> &Y, 
                                const long yOffset, const long Y_batchOffset, const int incY, const int batchSize);

/*                  C = alpha * op(A) * op(B) + beta * C                 */
    ampblasStatus ampblas_sgemm(const enum AMPBLAS_ORDER order, const enum AMPBLAS_TRANS typeA,
                                const enum AMPBLAS_TRANS typeB, const int M,
                                const int N, const int K, const float *alpha,
                                float *A, const long lda, float *B,
                                const long ldb, const float *beta, float *C,
                                const long ldc, const long aOffset,
                                const long bOffset, const long cOffset);

/* SGEMM- Overloaded function with arguments of type Concurrency::array_view */
    ampblasStatus ampblas_sgemm(Concurrency::accelerator_view &accl_view,
 				const enum AMPBLAS_ORDER order,const enum AMPBLAS_TRANS typeA,
                                const enum AMPBLAS_TRANS typeB, const int M,
                                const int N, const int K, const float &alpha,
                                Concurrency::array_view<float> &A, const long lda, 
		                Concurrency::array_view<float> &B, const long ldb, 
				const float &beta,  
				Concurrency::array_view<float> &C, const long ldc, 
				const long aOffset, const long bOffset, const long cOffset);


/* SGEMM- Overloaded function with arguments related to batch processing */
    ampblasStatus ampblas_sgemm(Concurrency::accelerator_view &accl_view,
                                const enum AMPBLAS_ORDER order, const enum AMPBLAS_TRANS typeA,
                                const enum AMPBLAS_TRANS typeB, const int M,
                                const int N, const int K, const float &alpha,
                                Concurrency::array_view<float> &A, const long lda, const long A_batchOffset,
                                Concurrency::array_view<float> &B, const long ldb, const long B_batchOffset,
                                const float &beta,
                                Concurrency::array_view<float> &C, const long ldc, const long C_batchOffset,
                                const long aOffset, const long bOffset, const long cOffset, const int batchSize);


/*                  C = alpha * op(A) * op(B) + beta * C                   */
    ampblasStatus ampblas_cgemm(const enum AMPBLAS_ORDER order, const enum AMPBLAS_TRANS typeA,
                                const enum AMPBLAS_TRANS typeB, const int M, 
                                const int N, const int K,
                                const ampComplex *alpha,
                                const ampComplex *A, const long aOffset, const long lda,
                                const ampComplex *B, const long bOffset, const long ldb,
                                const ampComplex *beta, ampComplex *C,
                                const long cOffset, const long ldc);

/* CGEMM - Overloaded function with arguments of type Concurrency::array_view */     
   ampblasStatus ampblas_cgemm(Concurrency::accelerator_view &accl_view,
			       const enum AMPBLAS_ORDER order, const enum AMPBLAS_TRANS typeA,
                               const enum AMPBLAS_TRANS typeB, const int M,
                               const int N, const int K,
                               const Concurrency::graphics::float_2 &alpha,
                               Concurrency::array_view<float_2> &A, const long aOffset, const long lda,
                               Concurrency::array_view<float_2> &B, const long bOffset, const long ldb,
                               const Concurrency::graphics::float_2 &beta, 
                               Concurrency::array_view<float_2> &C, const long cOffset, const long ldc);

/* CGEMM - Overloaded function with arguments related to batch processing */
   ampblasStatus ampblas_cgemm(Concurrency::accelerator_view &accl_view,
                               const enum AMPBLAS_ORDER order, const enum AMPBLAS_TRANS typeA,
                               const enum AMPBLAS_TRANS typeB, const int M,
                               const int N, const int K,
                               const Concurrency::graphics::float_2 &alpha,
                               Concurrency::array_view<float_2> &A, 
                               const long aOffset, const long A_batchOffset, const long lda,
                               Concurrency::array_view<float_2> &B, 
			       const long bOffset, const long B_batchOffset, const long ldb,
                               const Concurrency::graphics::float_2 &beta,
                               Concurrency::array_view<float_2> &C, 
			       const long cOffset, const long C_batchOffset, const long ldc, const int batchSize);

/* SSCAL - X = alpha * X */
   ampblasStatus ampblas_sscal(const int N, const float *alpha,
                               float *X, const int incX, const long xOffset);

/* SSCAL - Overloaded function with arguments of type Concurrency::array_view */
   ampblasStatus ampblas_sscal(Concurrency::accelerator_view &accl_view,
                               const int N, const float &alpha,
                               Concurrency::array_view<float> &X, const int incX,
                               const long xOffset);

/* SSCAL - Overloaded function with arguments related to batch processing */
   ampblasStatus ampblas_sscal(Concurrency::accelerator_view &accl_view,
                               const int N,const float &alpha,
                               Concurrency::array_view<float> &X, const int incX,
                               const long xOffset, const long X_batchOffset, const int batchSize);

/* DSCAL - X = alpha * X */
   ampblasStatus ampblas_dscal(const int N, const double *alpha,
                               double *X, const int incX, const long xOffset);

/* DSCAL - Overloaded function with arguments of type Concurrency::array_view */
   ampblasStatus ampblas_dscal(Concurrency::accelerator_view &accl_view,
                               const int N, const double &alpha,
                               Concurrency::array_view<double> &X, const int incX,
                               const long xOffset);

/* DSCAL - Overloaded function with arguments related to batch processing */
   ampblasStatus ampblas_dscal(Concurrency::accelerator_view &accl_view,
                               const int N,const double &alpha,
                               Concurrency::array_view<double> &X, const int incX,
                               const long xOffset, const long X_batchOffset, const int batchSize);

/* SCOPY - Copies a vector X to a vector Y */
   ampblasStatus ampblas_scopy(const int N, float *X, const int incX, const long xOffset, 
                               float *Y, const int incY, const long yOffset);

/* SCOPY - Overloaded function with arguments of type Concurrency::array_view */
   ampblasStatus ampblas_scopy(Concurrency::accelerator_view &accl_view, const int N,
                               Concurrency::array_view<float> &X, const int incX, const long xOffset,
                               Concurrency::array_view<float> &Y, const int incY, const long yOffset);

/* SCOPY - Overloaded function with arguments related to batch processing */
   ampblasStatus ampblas_scopy(Concurrency::accelerator_view &accl_view, const int N,
                               Concurrency::array_view<float> &X, const int incX, const long xOffset,
                               Concurrency::array_view<float> &Y, const int incY, const long yOffset,
                               const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* DCOPY - Copies a vector X to a vector Y */
   ampblasStatus ampblas_dcopy(const int N, double *X, const int incX, const long xOffset, 
                               double *Y, const int incY, const long yOffset);

/* DCOPY - Overloaded function with arguments of type Concurrency::array_view */
   ampblasStatus ampblas_dcopy(Concurrency::accelerator_view &accl_view, const int N,
                               Concurrency::array_view<double> &X, const int incX, const long xOffset,
                               Concurrency::array_view<double> &Y, const int incY, const long yOffset);

/* DCOPY - Overloaded function with arguments related to batch processing */
   ampblasStatus ampblas_dcopy(Concurrency::accelerator_view &accl_view, const int N,
                               Concurrency::array_view<double> &X, const int incX, const long xOffset,
                               Concurrency::array_view<double> &Y, const int incY, const long yOffset,
                               const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* SDOT - Single Precision Dot product */
   ampblasStatus ampblas_sdot(const int N, float *X, const int incX, const long xOffset, 
                              float *Y, const int incY, const long yOffset, float *dot);

/* SDOT - Overloaded function with arguments of type Concurrency::array_view */
   ampblasStatus ampblas_sdot(Concurrency::accelerator_view &accl_view, const int N,
                              Concurrency::array_view<float> &X, const int incX, const long xOffset,
                              Concurrency::array_view<float> &Y, const int incY, const long yOffset, float &dot);

/* SDOT - Overloaded function with arguments related to batch processing */
   ampblasStatus ampblas_sdot(Concurrency::accelerator_view &accl_view, const int N,
                              Concurrency::array_view<float> &X, const int incX, const long xOffset,
                              Concurrency::array_view<float> &Y, const int incY, const long yOffset, float &dot,
                              const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* DDOT - Double Precision Dot product */
   ampblasStatus ampblas_ddot(const int N, double *X, const int incX, const long xOffset,
                              double *Y, const int incY, const long yOffset, double *dot);

/* DDOT - Overloaded function with arguments of type Concurrency::array_view */
   ampblasStatus ampblas_ddot(Concurrency::accelerator_view &accl_view, const int N,
                              Concurrency::array_view<double> &X, const int incX, const long xOffset,
                              Concurrency::array_view<double> &Y, const int incY, const long yOffset, double &dot);

/* DDOT - Overloaded function with arguments related to batch processing */
   ampblasStatus ampblas_ddot(Concurrency::accelerator_view &accl_view, const int N,
                              Concurrency::array_view<double> &X, const int incX, const long xOffset,
                              Concurrency::array_view<double> &Y, const int incY, const long yOffset, double &dot,
                              const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* Absolute value of a Vector - Single Precision */
   ampblasStatus ampblas_sasum(const int N, float *X, const int incX, const long xOffset, float *Y);    
   
/* Absolute value of a Vector - Double Precision */
   ampblasStatus ampblas_dasum(const int N, double *X, const int incX, const long xOffset, double *Y);

};


#endif
