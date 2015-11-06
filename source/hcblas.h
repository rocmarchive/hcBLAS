/*
*
*  FILENAME : hcblas.h
*  This file is the top level header file which includes the Hcblaslilbrary class
*  for performing five blas operations ( saxpy, sger, sgemv, sgemm, cgemm )
*
*/

#ifndef HCBLAS_H
#define HCBLAS_H

#include <iostream>
#include <amp.h>
#include <amp_short_vectors.h>

using namespace Concurrency;
using namespace Concurrency::graphics;
using namespace std;
/* enumerator to indicate the status of  blas operation */
enum hcblasStatus {
    HCBLAS_SUCCESS = 0,
    HCBLAS_INVALID = -1,
    HCBLAS_ERROR = -2
};

/* enumerator to define the layout of  input matrix for blas operation */
enum HCBLAS_ORDER {
    rowMajor ,
    colMajor
};

/* enumerator to define the type of operation to be performed on the input matrix
 ( NO_TRANSPOSE, TRANSPOSE, CONJUGATE) */
enum HCBLAS_TRANS {
    noTrans = 'n',
    trans = 't',
    conjugate = 'c'
};

struct hcComplex
{
     float real;
     float img;
};

/* Class which implements the blas ( SGEMM, CGEMM, SGEMV, SGER, SAXPY )  */
class Hcblaslibrary
{
    public:
/*                  Y = alpha * X + Y                                    */
    hcblasStatus hcblas_saxpy(const int N,
                              const float *alpha, float *X, const int incX,
                              float *Y, const int incY, const long xOffset,
                              const long yOffset);

/* SAXPY - Overloaded function with arguments of type Concurrency::array_view */

    hcblasStatus hcblas_saxpy(Concurrency::accelerator_view &accl_view,
			      const int N, const float &alpha,
			      Concurrency::array<float> &X, const int incX,
                              Concurrency::array<float> &Y, const int incY, 
			      const long xOffset, const long yOffset);

/* SAXPY - Overloaded function with arguments related to batch processing */

    hcblasStatus hcblas_saxpy(Concurrency::accelerator_view &accl_view,
                              const int N, const float &alpha,
                              Concurrency::array<float> &X, const int incX, const long X_batchOffset,
                              Concurrency::array<float> &Y, const int incY, const long Y_batchOffset,
                              const long xOffset, const long yOffset, const int batchSize);

/*                  A = alpha * X * Y' + A                               */
    hcblasStatus hcblas_sger(HCBLAS_ORDER order, const int M, const int N, const float *alpha,
                             float *X, const long xOffset, const int incX,
                             float *Y, const long yOffset, const int incY,
                             float *A, const long aOffset, const int lda);

/* SGER - Overloaded function with arguments of type Concurrency::array_view */

    hcblasStatus hcblas_sger(Concurrency::accelerator_view &accl_view,
			     HCBLAS_ORDER order, const int M, const int N, const float &alpha,
                             Concurrency::array<float> &X, const long xOffset, const int incX,
                             Concurrency::array<float> &Y, const long yOffset, const int incY,
                             Concurrency::array<float> &A, const long aOffset, const int lda);

/* SGER - Overloaded function with arguments related to batch processing */

    hcblasStatus hcblas_sger(Concurrency::accelerator_view &accl_view,
                             HCBLAS_ORDER order, const int M, const int N, const float &alpha,
                             Concurrency::array<float> &X, 
                             const long xOffset, const long X_batchOffset, const int incX,
                             Concurrency::array<float> &Y, 
                             const long yOffset, const long Y_batchOffset, const int incY,
                             Concurrency::array<float> &A, 
                             const long aOffset, const long A_batchOffset, const int lda, const int batchSize);

/*                  Y = alpha * op(A) * X + beta * Y                     */
    hcblasStatus hcblas_sgemv(HCBLAS_ORDER order, HCBLAS_TRANS type, const int M,
                              const int N, const float *alpha, float *A,
                              const long aOffset,const int lda, float *X,
                              const long xOffset, const int incX,
                              const float *beta, float *Y,const long yOffset,
                              const int incY);

/* SGEMV- Overloaded function with arguments of type Concurrency::array_view */
    hcblasStatus hcblas_sgemv(Concurrency::accelerator_view &accl_view,
			      HCBLAS_ORDER order, HCBLAS_TRANS type, const int M,
                              const int N, const float &alpha, 
                              Concurrency::array<float> &A, const long aOffset, const int lda, 
			      Concurrency::array<float> &X, const long xOffset, const int incX,
                              const float &beta,  
			      Concurrency::array<float> &Y, const long yOffset, const int incY);

/* SGEMV- Overloaded function with arguments related to batch processing */
    hcblasStatus hcblas_sgemv(Concurrency::accelerator_view &accl_view,
                              HCBLAS_ORDER order, HCBLAS_TRANS type, const int M,
                              const int N, const float &alpha, Concurrency::array<float> &A, 
                              const long aOffset, const long A_batchOffset, const int lda,
                              Concurrency::array<float> &X, 
                              const long xOffset, const long X_batchOffset, const int incX,
                              const float &beta, Concurrency::array<float> &Y, 
                              const long yOffset, const long Y_batchOffset, const int incY, const int batchSize);

/*                  C = alpha * op(A) * op(B) + beta * C                 */
    hcblasStatus hcblas_sgemm(HCBLAS_ORDER order, HCBLAS_TRANS typeA,
                              HCBLAS_TRANS typeB, const int M,
                              const int N, const int K, const float *alpha,
                              float *A, const long lda, float *B,
                              const long ldb, const float *beta, float *C,
                              const long ldc, const long aOffset,
                              const long bOffset, const long cOffset);

/* SGEMM- Overloaded function with arguments of type Concurrency::array_view */
    hcblasStatus hcblas_sgemm(Concurrency::accelerator_view &accl_view,
 			      HCBLAS_ORDER order, HCBLAS_TRANS typeA,
                              HCBLAS_TRANS typeB, const int M,
                              const int N, const int K, const float &alpha,
                              Concurrency::array<float> &A, const long lda, 
		              Concurrency::array<float> &B, const long ldb, 
			      const float &beta,  
		       	      Concurrency::array<float> &C, const long ldc, 
			      const long aOffset, const long bOffset, const long cOffset);


/* SGEMM- Overloaded function with arguments related to batch processing */
    hcblasStatus hcblas_sgemm(Concurrency::accelerator_view &accl_view,
                              HCBLAS_ORDER order, HCBLAS_TRANS typeA,
                              HCBLAS_TRANS typeB, const int M,
                              const int N, const int K, const float &alpha,
                              Concurrency::array<float> &A, const long lda, const long A_batchOffset,
                              Concurrency::array<float> &B, const long ldb, const long B_batchOffset,
                              const float &beta,
                              Concurrency::array<float> &C, const long ldc, const long C_batchOffset,
                              const long aOffset, const long bOffset, const long cOffset, const int batchSize);


/*                  C = alpha * op(A) * op(B) + beta * C                   */
    hcblasStatus hcblas_cgemm(HCBLAS_ORDER order, HCBLAS_TRANS typeA,
                              HCBLAS_TRANS typeB, const int M, 
                              const int N, const int K,
                              const hcComplex *alpha,
                              const hcComplex *A, const long aOffset, const long lda,
                              const hcComplex *B, const long bOffset, const long ldb,
                              const hcComplex *beta, hcComplex *C,
                              const long cOffset, const long ldc);

/* CGEMM - Overloaded function with arguments of type Concurrency::array_view */     
   hcblasStatus hcblas_cgemm(Concurrency::accelerator_view &accl_view,
			     HCBLAS_ORDER order, HCBLAS_TRANS typeA,
                             HCBLAS_TRANS typeB, const int M,
                             const int N, const int K,
                             const Concurrency::graphics::float_2 &alpha,
                             Concurrency::array<float_2> &A, const long aOffset, const long lda,
                             Concurrency::array<float_2> &B, const long bOffset, const long ldb,
                             const Concurrency::graphics::float_2 &beta, 
                             Concurrency::array<float_2> &C, const long cOffset, const long ldc);

/* CGEMM - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_cgemm(Concurrency::accelerator_view &accl_view,
                             HCBLAS_ORDER order, HCBLAS_TRANS typeA,
                             HCBLAS_TRANS typeB, const int M,
                             const int N, const int K,
                             const Concurrency::graphics::float_2 &alpha,
                             Concurrency::array<float_2> &A, 
                             const long aOffset, const long A_batchOffset, const long lda,
                             Concurrency::array<float_2> &B, 
			     const long bOffset, const long B_batchOffset, const long ldb,
                             const Concurrency::graphics::float_2 &beta,
                             Concurrency::array<float_2> &C, 
			     const long cOffset, const long C_batchOffset, const long ldc, const int batchSize);

/* SSCAL - X = alpha * X */
   hcblasStatus hcblas_sscal(const int N, const float *alpha,
                             float *X, const int incX, const long xOffset);

/* SSCAL - Overloaded function with arguments of type Concurrency::array_view */
   hcblasStatus hcblas_sscal(Concurrency::accelerator_view &accl_view,
                             const int N, const float &alpha,
                             Concurrency::array<float> &X, const int incX,
                             const long xOffset);

/* SSCAL - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_sscal(Concurrency::accelerator_view &accl_view,
                             const int N,const float &alpha,
                             Concurrency::array<float> &X, const int incX,
                             const long xOffset, const long X_batchOffset, const int batchSize);

/* DSCAL - X = alpha * X */
   hcblasStatus hcblas_dscal(const int N, const double *alpha,
                             double *X, const int incX, const long xOffset);

/* DSCAL - Overloaded function with arguments of type Concurrency::array_view */
   hcblasStatus hcblas_dscal(Concurrency::accelerator_view &accl_view,
                             const int N, const double &alpha,
                             Concurrency::array<double> &X, const int incX,
                             const long xOffset);

/* DSCAL - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_dscal(Concurrency::accelerator_view &accl_view,
                             const int N,const double &alpha,
                             Concurrency::array<double> &X, const int incX,
                             const long xOffset, const long X_batchOffset, const int batchSize);

/* SCOPY - Copies a vector X to a vector Y */
   hcblasStatus hcblas_scopy(const int N, float *X, const int incX, const long xOffset, 
                             float *Y, const int incY, const long yOffset);

/* SCOPY - Overloaded function with arguments of type Concurrency::array_view */
   hcblasStatus hcblas_scopy(Concurrency::accelerator_view &accl_view, const int N,
                             Concurrency::array<float> &X, const int incX, const long xOffset,
                             Concurrency::array<float> &Y, const int incY, const long yOffset);

/* SCOPY - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_scopy(Concurrency::accelerator_view &accl_view, const int N,
                             Concurrency::array<float> &X, const int incX, const long xOffset,
                             Concurrency::array<float> &Y, const int incY, const long yOffset,
                             const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* DCOPY - Copies a vector X to a vector Y */
   hcblasStatus hcblas_dcopy(const int N, double *X, const int incX, const long xOffset, 
                             double *Y, const int incY, const long yOffset);

/* DCOPY - Overloaded function with arguments of type Concurrency::array_view */
   hcblasStatus hcblas_dcopy(Concurrency::accelerator_view &accl_view, const int N,
                             Concurrency::array<double> &X, const int incX, const long xOffset,
                             Concurrency::array<double> &Y, const int incY, const long yOffset);

/* DCOPY - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_dcopy(Concurrency::accelerator_view &accl_view, const int N,
                             Concurrency::array<double> &X, const int incX, const long xOffset,
                             Concurrency::array<double> &Y, const int incY, const long yOffset,
                             const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* SDOT - Single Precision Dot product */
   hcblasStatus hcblas_sdot(const int N, float *X, const int incX, const long xOffset, 
                            float *Y, const int incY, const long yOffset, float *dot);

/* SDOT - Overloaded function with arguments of type Concurrency::array_view */
   hcblasStatus hcblas_sdot(Concurrency::accelerator_view &accl_view, const int N,
                            Concurrency::array<float> &X, const int incX, const long xOffset,
                            Concurrency::array<float> &Y, const int incY, const long yOffset, float &dot);

/* SDOT - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_sdot(Concurrency::accelerator_view &accl_view, const int N,
                            Concurrency::array<float> &X, const int incX, const long xOffset,
                            Concurrency::array<float> &Y, const int incY, const long yOffset, float &dot,
                            const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* DDOT - Double Precision Dot product */
   hcblasStatus hcblas_ddot(const int N, double *X, const int incX, const long xOffset,
                            double *Y, const int incY, const long yOffset, double *dot);

/* DDOT - Overloaded function with arguments of type Concurrency::array_view */
   hcblasStatus hcblas_ddot(Concurrency::accelerator_view &accl_view, const int N,
                            Concurrency::array<double> &X, const int incX, const long xOffset,
                            Concurrency::array<double> &Y, const int incY, const long yOffset, double &dot);

/* DDOT - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_ddot(Concurrency::accelerator_view &accl_view, const int N,
                            Concurrency::array<double> &X, const int incX, const long xOffset,
                            Concurrency::array<double> &Y, const int incY, const long yOffset, double &dot,
                            const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* SASUM - Absolute value of a Vector - Single Precision */
   hcblasStatus hcblas_sasum(const int N, float *X, const int incX, const long xOffset, float *Y);    
   
/* SASUM - Overloaded function with arguments of type Concurrency::array_view */
   hcblasStatus hcblas_sasum(Concurrency::accelerator_view &accl_view, const int N,
                             Concurrency::array<float> &X, const int incX,
                             const long xOffset, float &Y);

/* SASUM - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_sasum(Concurrency::accelerator_view &accl_view, const int N,
                             Concurrency::array<float> &X, const int incX,
                             const long xOffset, float &Y, const long X_batchOffset, const int batchSize);

/* DASUM - Absolute value of a Vector - Double Precision */
   hcblasStatus hcblas_dasum(const int N, double *X, const int incX, const long xOffset, double *Y);

/* DASUM - Overloaded function with arguments of type Concurrency::array_view */
   hcblasStatus hcblas_dasum(Concurrency::accelerator_view &accl_view, const int N,
                             Concurrency::array<double> &X, const int incX,
                             const long xOffset, double &Y);

/* DASUM - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_dasum(Concurrency::accelerator_view &accl_view, const int N,
                             Concurrency::array<double> &X, const int incX,
                             const long xOffset, double &Y, const long X_batchOffset, const int batchSize);

};


#endif
