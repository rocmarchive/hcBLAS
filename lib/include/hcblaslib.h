/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/*
* This file is the top level header file which includes the Hcblaslilbrary
* classfor performing blas operations ( saxpy, sger, sgemv, sgemm, cgemm ,
* dgemm,
* zgemm )
*/

#ifndef LIB_INCLUDE_HCBLASLIB_H_
#define LIB_INCLUDE_HCBLASLIB_H_

#include <hc.hpp>
#include <hc_am.hpp>
#include <hc_defines.h>
#include <hc_short_vector.hpp>
#include <iostream>
#include <vector>

// Compulsory wait
#define _WAIT2 .wait()

// Ensuring Optional wait on kernels
#if (SERIALIZE_KERNEL == 1)
#define _WAIT1 .wait()
#else
#define _WAIT1
#endif
#if (SERIALIZE_KERNEL == 2)
#undef _WAIT2
#define _WAIT2
#endif

#define __HC_FP16_DECL_SUFFIX__ [[hc]]

/* enumerator to indicate the status of  blas operation */
enum hcblasStatus { HCBLAS_SUCCEEDS = 0, HCBLAS_INVALID = -1 };

/* enumerator to define the layout of  input matrix for blas operation */
enum hcblasOrder : unsigned short { RowMajor, ColMajor };

/* enumerator to define the type of operation to be performed on the input
 matrix ( NO_TRANSPOSE, TRANSPOSE, CONJUGATE) */
enum hcblasTranspose { NoTrans = 'n', Trans = 't' };

union SP_FP32 {
  unsigned int u;
  float f;
};

bool hisnan(hc::half raw) __HC_FP16_DECL_SUFFIX__;
int hisinf(hc::half raw) __HC_FP16_DECL_SUFFIX__;

struct hc_Complex {
  float real;
  float img;
};

/* Class which implements the blas ( SGEMM, CGEMM, SGEMV, SGER, SAXPY )  */
struct Hcblaslibrary {
 public:
  // Constructor to initialize the library with the given hc::accelerator
  explicit Hcblaslibrary(hc::accelerator_view *av)
      : currentAccl(av->get_accelerator()), currentAcclView(*av) {
    std::vector<hc::accelerator> accs = hc::accelerator::get_all();
    for (int i = 0; i < accs.size(); i++) {
      if (accs[i] == this->currentAccl) {
        this->initialized = true;
        break;
      }
    }
    assert(this->initialized);
    // TODO(Neelakandan): Add another constructor to accommodate row
    // major setting
    this->Order = ColMajor;
  }

  ~Hcblaslibrary() {
    // Deinitialize the library
    this->initialized = false;
  }

  // Add current Accerator field
  hc::accelerator currentAccl;

  // Filed to check if library is initialized
  bool initialized = false;

  hc::accelerator_view currentAcclView;

  // StreamInfo
  void *currentStream = NULL;

  hcblasOrder Order;

  /* SAXPY - Y = alpha * X + Y                                    */
  /* SAXPY - Overloaded function with arguments of type hc::array */

  hcblasStatus hcblas_saxpy(hc::accelerator_view accl_view, const int N,
                            const float &alpha, const float *X, const int incX,
                            float *Y, const int incY, const __int64_t xOffset,
                            const __int64_t yOffset);

  /* SAXPY - Overloaded function with arguments related to batch processing */

  hcblasStatus hcblas_saxpy(hc::accelerator_view accl_view, const int N,
                            const float &alpha, const float *X, const int incX,
                            const __int64_t X_batchOffset, float *Y,
                            const int incY, const __int64_t Y_batchOffset,
                            const __int64_t xOffset, const __int64_t yOffset,
                            const int batchSize);

  /* DAXPY - Y = alpha * X + Y                                    */
  /* DAXPY - Overloaded function with arguments of type hc::array */

  hcblasStatus hcblas_daxpy(hc::accelerator_view accl_view, const int N,
                            const double &alpha, const double *X,
                            const int incX, double *Y, const int incY,
                            const __int64_t xOffset, const __int64_t yOffset);

  /* DAXPY - Overloaded function with arguments related to batch processing */

  hcblasStatus hcblas_daxpy(hc::accelerator_view accl_view, const int N,
                            const double &alpha, const double *X,
                            const int incX, const __int64_t X_batchOffset,
                            double *Y, const int incY,
                            const __int64_t Y_batchOffset,
                            const __int64_t xOffset, const __int64_t yOffset,
                            const int batchSize);

  /* SGER - A = alpha * X * Y' + A                               */
  /* SGER - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_sger(hc::accelerator_view accl_view, hcblasOrder order,
                           const int M, const int N, const float &alpha,
                           const float *X, const __int64_t xOffset,
                           const int incX, const float *Y,
                           const __int64_t yOffset, const int incY, float *A,
                           const __int64_t aOffset, const int lda);

  /* SGER - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_sger(hc::accelerator_view accl_view, hcblasOrder order,
                           const int M, const int N, const float &alpha,
                           const float *X, const __int64_t xOffset,
                           const __int64_t X_batchOffset, const int incX,
                           const float *Y, const __int64_t yOffset,
                           const __int64_t Y_batchOffset, const int incY,
                           float *A, const __int64_t aOffset,
                           const __int64_t A_batchOffset, const int lda,
                           const int batchSize);

  /* DGER - A = alpha * X * Y' + A                               */
  hcblasStatus hcblas_dger(hc::accelerator_view accl_view, hcblasOrder order,
                           const int M, const int N, const double &alpha,
                           const double *X, const __int64_t xOffset,
                           const int incX, const double *Y,
                           const __int64_t yOffset, const int incY, double *A,
                           const __int64_t aOffset, const int lda);

  /* DGER - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_dger(hc::accelerator_view accl_view, hcblasOrder order,
                           const int M, const int N, const double &alpha,
                           const double *X, const __int64_t xOffset,
                           const __int64_t X_batchOffset, const int incX,
                           const double *Y, const __int64_t yOffset,
                           const __int64_t Y_batchOffset, const int incY,
                           double *A, const __int64_t aOffset,
                           const __int64_t A_batchOffset, const int lda,
                           const int batchSize);

  /* SGEMV - Y = alpha * op(A) * X + beta * Y                     */
  hcblasStatus hcblas_sgemv(hc::accelerator_view accl_view, hcblasOrder order,
                            hcblasTranspose type, const int M, const int N,
                            const float &alpha, float *A,
                            const __int64_t aOffset, const int lda, float *X,
                            const __int64_t xOffset, const int incX,
                            const float &beta, float *Y,
                            const __int64_t yOffset, const int incY);

  /* SGEMV - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_sgemv(
      hc::accelerator_view accl_view, hcblasOrder order, hcblasTranspose type,
      const int M, const int N, const float &alpha, float *A,
      const __int64_t aOffset, const __int64_t A_batchOffset, const int lda,
      float *X, const __int64_t xOffset, const __int64_t X_batchOffset,
      const int incX, const float &beta, float *Y, const __int64_t yOffset,
      const __int64_t Y_batchOffset, const int incY, const int batchSize);

  /* DGEMV - Y = alpha * op(A) * X + beta * Y                     */
  hcblasStatus hcblas_dgemv(hc::accelerator_view accl_view, hcblasOrder order,
                            hcblasTranspose type, const int M, const int N,
                            const double &alpha, double *A,
                            const __int64_t aOffset, const int lda, double *X,
                            const __int64_t xOffset, const int incX,
                            const double &beta, double *Y,
                            const __int64_t yOffset, const int incY);

  /* DGEMV - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_dgemv(
      hc::accelerator_view accl_view, hcblasOrder order, hcblasTranspose type,
      const int M, const int N, const double &alpha, double *A,
      const __int64_t aOffset, const __int64_t A_batchOffset, const int lda,
      double *X, const __int64_t xOffset, const __int64_t X_batchOffset,
      const int incX, const double &beta, double *Y, const __int64_t yOffset,
      const __int64_t Y_batchOffset, const int incY, const int batchSize);

  /* SGEMM - C = alpha * op(A) * op(B) + beta * C                 */
  /* SGEMM - Overloaded function with arguments of type dev pointer */
  hcblasStatus hcblas_sgemm(hc::accelerator_view accl_view, hcblasOrder order,
                            hcblasTranspose typeA, hcblasTranspose typeB,
                            const int M, const int N, const int K,
                            const float &alpha, float *A, const __int64_t lda,
                            float *B, const __int64_t ldb, const float &beta,
                            float *C, const __int64_t ldc,
                            const __int64_t aOffset, const __int64_t bOffset,
                            const __int64_t cOffset);

  hcblasStatus hcblas_dgemm(hc::accelerator_view accl_view, hcblasOrder order,
                            hcblasTranspose typeA, hcblasTranspose typeB,
                            const int M, const int N, const int K,
                            const double &alpha, double *A, const __int64_t lda,
                            double *B, const __int64_t ldb, const double &beta,
                            double *C, const __int64_t ldc,
                            const __int64_t aOffset, const __int64_t bOffset,
                            const __int64_t cOffset);

  hcblasStatus hcblas_hgemm(hc::accelerator_view accl_view, hcblasOrder order,
                            hcblasTranspose typeA, hcblasTranspose typeB,
                            const int M, const int N, const int K,
                            const hc::half &alpha, hc::half *A,
                            const __int64_t lda, hc::half *B,
                            const __int64_t ldb, const hc::half &beta,
                            hc::half *C, const __int64_t ldc,
                            const __int64_t aOffset, const __int64_t bOffset,
                            const __int64_t cOffset);

  /* SGEMM - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_sgemm(hc::accelerator_view accl_view, hcblasOrder order,
                            hcblasTranspose typeA, hcblasTranspose typeB,
                            const int M, const int N, const int K,
                            const float &alpha, float *A[], const __int64_t lda,
                            const __int64_t A_batchOffset, float *B[],
                            const __int64_t ldb, const __int64_t B_batchOffset,
                            const float &beta, float *C[], const __int64_t ldc,
                            const __int64_t C_batchOffset,
                            const __int64_t aOffset, const __int64_t bOffset,
                            const __int64_t cOffset, const int batchSize);

  /*  DGEMM - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_dgemm(hc::accelerator_view accl_view, hcblasOrder order,
                            hcblasTranspose typeA, hcblasTranspose typeB,
                            const int M, const int N, const int K,
                            const double &alpha, double *A[],
                            const __int64_t lda, const __int64_t A_batchOffset,
                            double *B[], const __int64_t ldb,
                            const __int64_t B_batchOffset, const double &beta,
                            double *C[], const __int64_t ldc,
                            const __int64_t C_batchOffset,
                            const __int64_t aOffset, const __int64_t bOffset,
                            const __int64_t cOffset, const int batchSize);
  /* CGEMM - C = alpha * op(A) * op(B) + beta * C                   */
  /* CGEMM - Overloaded function with arguments of type hc::array   */
  hcblasStatus hcblas_cgemm(hc::accelerator_view accl_view, hcblasOrder order,
                            hcblasTranspose typeA, hcblasTranspose typeB,
                            const int M, const int N, const int K,
                            const hc::short_vector::float_2 &alpha,
                            hc::short_vector::float_2 *A,
                            const __int64_t aOffset, const __int64_t lda,
                            hc::short_vector::float_2 *B,
                            const __int64_t bOffset, const __int64_t ldb,
                            const hc::short_vector::float_2 &beta,
                            hc::short_vector::float_2 *C,
                            const __int64_t cOffset, const __int64_t ldc);

  hcblasStatus hcblas_zgemm(hc::accelerator_view accl_view, hcblasOrder order,
                            hcblasTranspose typeA, hcblasTranspose typeB,
                            const int M, const int N, const int K,
                            const hc::short_vector::double_2 &alpha,
                            hc::short_vector::double_2 *A,
                            const __int64_t aOffset, const __int64_t lda,
                            hc::short_vector::double_2 *B,
                            const __int64_t bOffset, const __int64_t ldb,
                            const hc::short_vector::double_2 &beta,
                            hc::short_vector::double_2 *C,
                            const __int64_t cOffset, const __int64_t ldc);

  /* CGEMM - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_cgemm(
      hc::accelerator_view accl_view, hcblasOrder order, hcblasTranspose typeA,
      hcblasTranspose typeB, const int M, const int N, const int K,
      const hc::short_vector::float_2 &alpha, hc::short_vector::float_2 *A[],
      const __int64_t aOffset, const __int64_t A_batchOffset,
      const __int64_t lda, hc::short_vector::float_2 *B[],
      const __int64_t bOffset, const __int64_t B_batchOffset,
      const __int64_t ldb, const hc::short_vector::float_2 &beta,
      hc::short_vector::float_2 *C[], const __int64_t cOffset,
      const __int64_t C_batchOffset, const __int64_t ldc, const int batchSize);

  /* ZGEMM - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_zgemm(
      hc::accelerator_view accl_view, hcblasOrder order, hcblasTranspose typeA,
      hcblasTranspose typeB, const int M, const int N, const int K,
      const hc::short_vector::double_2 &alpha, hc::short_vector::double_2 *A[],
      const __int64_t aOffset, const __int64_t A_batchOffset,
      const __int64_t lda, hc::short_vector::double_2 *B[],
      const __int64_t bOffset, const __int64_t B_batchOffset,
      const __int64_t ldb, const hc::short_vector::double_2 &beta,
      hc::short_vector::double_2 *C[], const __int64_t cOffset,
      const __int64_t C_batchOffset, const __int64_t ldc, const int batchSize);

  /* SSCAL - X = alpha * X */
  /* SSCAL - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_sscal(hc::accelerator_view accl_view, const int N,
                            const float &alpha, float *X, const int incX,
                            const __int64_t xOffset);

  /* SSCAL - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_sscal(hc::accelerator_view accl_view, const int N,
                            const float &alpha, float *X, const int incX,
                            const __int64_t xOffset,
                            const __int64_t X_batchOffset, const int batchSize);

  /* DSCAL - X = alpha * X */
  /* DSCAL - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_dscal(hc::accelerator_view accl_view, const int N,
                            const double &alpha, double *X, const int incX,
                            const __int64_t xOffset);

  /* DSCAL - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_dscal(hc::accelerator_view accl_view, const int N,
                            const double &alpha, double *X, const int incX,
                            const __int64_t xOffset,
                            const __int64_t X_batchOffset, const int batchSize);

  /* CSCAL - X = alpha * X */
  /* CSCAL - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_cscal(hc::accelerator_view accl_view, const int N,
                            const hc::short_vector::float_2 &alpha,
                            hc::short_vector::float_2 *X, const int incX,
                            const __int64_t xOffset);

  /* CSCAL - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_cscal(hc::accelerator_view accl_view, const int N,
                            const hc::short_vector::float_2 &alpha,
                            hc::short_vector::float_2 *X, const int incX,
                            const __int64_t xOffset,
                            const __int64_t X_batchOffset, const int batchSize);

  /* ZSCAL - X = alpha * X */
  /* ZSCAL - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_zscal(hc::accelerator_view accl_view, const int N,
                            const hc::short_vector::double_2 &alpha,
                            hc::short_vector::double_2 *X, const int incX,
                            const __int64_t xOffset);

  /* ZSCAL - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_zscal(hc::accelerator_view accl_view, const int N,
                            const hc::short_vector::double_2 &alpha,
                            hc::short_vector::double_2 *X, const int incX,
                            const __int64_t xOffset,
                            const __int64_t X_batchOffset, const int batchSize);

  /* CSSCAL - X = alpha * X */
  /* CSSCAL - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_csscal(hc::accelerator_view accl_view, const int N,
                             const float &alpha, hc::short_vector::float_2 *X,
                             const int incX, const __int64_t xOffset);

  /* CSSCAL - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_csscal(hc::accelerator_view accl_view, const int N,
                             const float &alpha, hc::short_vector::float_2 *X,
                             const int incX, const __int64_t xOffset,
                             const __int64_t X_batchOffset,
                             const int batchSize);

  /* ZDSCAL - X = alpha * X */
  /* ZDSCAL - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_zdscal(hc::accelerator_view accl_view, const int N,
                             const double &alpha, hc::short_vector::double_2 *X,
                             const int incX, const __int64_t xOffset);

  /* ZDSCAL - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_zdscal(hc::accelerator_view accl_view, const int N,
                             const double &alpha, hc::short_vector::double_2 *X,
                             const int incX, const __int64_t xOffset,
                             const __int64_t X_batchOffset,
                             const int batchSize);

  /* SCOPY - Copies a vector X to a vector Y */
  /* SCOPY - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_scopy(hc::accelerator_view accl_view, const int N,
                            const float *X, const int incX,
                            const __int64_t xOffset, float *Y, const int incY,
                            const __int64_t yOffset);

  /* SCOPY - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_scopy(hc::accelerator_view accl_view, const int N,
                            const float *X, const int incX,
                            const __int64_t xOffset, float *Y, const int incY,
                            const __int64_t yOffset,
                            const __int64_t X_batchOffset,
                            const __int64_t Y_batchOffset, const int batchSize);

  /* DCOPY - Copies a vector X to a vector Y */
  /* DCOPY - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_dcopy(hc::accelerator_view accl_view, const int N,
                            const double *X, const int incX,
                            const __int64_t xOffset, double *Y, const int incY,
                            const __int64_t yOffset);

  /* DCOPY - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_dcopy(hc::accelerator_view accl_view, const int N,
                            const double *X, const int incX,
                            const __int64_t xOffset, double *Y, const int incY,
                            const __int64_t yOffset,
                            const __int64_t X_batchOffset,
                            const __int64_t Y_batchOffset, const int batchSize);

  /* SDOT - Single Precision Dot product */
  /* SDOT - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_sdot(hc::accelerator_view accl_view, const int N,
                           const float *X, const int incX,
                           const __int64_t xOffset, const float *Y,
                           const int incY, const __int64_t yOffset, float &dot);

  /* SDOT - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_sdot(hc::accelerator_view accl_view, const int N,
                           const float *X, const int incX,
                           const __int64_t xOffset, const float *Y,
                           const int incY, const __int64_t yOffset, float &dot,
                           const __int64_t X_batchOffset,
                           const __int64_t Y_batchOffset, const int batchSize);

  /* DDOT - Double Precision Dot product */
  /* DDOT - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_ddot(hc::accelerator_view accl_view, const int N,
                           const double *X, const int incX,
                           const __int64_t xOffset, const double *Y,
                           const int incY, const __int64_t yOffset,
                           double &dot);

  /* DDOT - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_ddot(hc::accelerator_view accl_view, const int N,
                           const double *X, const int incX,
                           const __int64_t xOffset, const double *Y,
                           const int incY, const __int64_t yOffset, double &dot,
                           const __int64_t X_batchOffset,
                           const __int64_t Y_batchOffset, const int batchSize);

  /* SASUM - Absolute value of a Vector - Single Precision */
  /* SASUM - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_sasum(hc::accelerator_view accl_view, const int N,
                            float *X, const int incX, const __int64_t xOffset,
                            float *Y);

  /* SASUM - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_sasum(hc::accelerator_view accl_view, const int N,
                            float *X, const int incX, const __int64_t xOffset,
                            float *Y, const __int64_t X_batchOffset,
                            const int batchSize);

  /* DASUM - Absolute value of a Vector - Double Precision */
  /* DASUM - Overloaded function with arguments of type hc::array */
  hcblasStatus hcblas_dasum(hc::accelerator_view accl_view, const int N,
                            double *X, const int incX, const __int64_t xOffset,
                            double *Y);

  /* DASUM - Overloaded function with arguments related to batch processing */
  hcblasStatus hcblas_dasum(hc::accelerator_view accl_view, const int N,
                            double *X, const int incX, const __int64_t xOffset,
                            double *Y, const __int64_t X_batchOffset,
                            const int batchSize);
};

#endif  // LIB_INCLUDE_HCBLASLIB_H_
