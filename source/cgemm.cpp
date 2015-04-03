#include "ampblaslib.h"
#include<assert.h>
#include <vector>
#include <amp.h>
#include <iostream>
#include <amp_short_vectors.h>
using namespace Concurrency;
using namespace Concurrency::graphics;

void cgemm_NoTransAB(int M, int N, int K, float_2 alpha,
             Concurrency::array_view<float_2> &A, long aOffset, long lda,
             Concurrency::array_view<float_2> &B, long bOffset, long ldb,
             float_2 beta,
             Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(M,N);

    Concurrency::parallel_for_each(grdExt , [=](index<2> idx) restrict(amp)
    {
        int row = idx[0];
        int col = idx[1];
        float tempReal = 0.0;
        float tempImg = 0.0;

        /* row by column multiplication */
        float realSum = 0.0;
        float imgSum = 0.0;

        if (row < M && col < N ) {
            for ( int i = 0 ; i < K; i++) {
                 float xAVal = A[aOffset + row + i * M].x;
                 float yAVal = A[aOffset + row + i * M].y;
                 float xBVal = B[bOffset + i + col * K].x;
                 float yBVal = B[aOffset + i + col * K].y;
                 realSum += (xAVal * xBVal) - (yAVal * yBVal);
                 imgSum += (xAVal * yBVal) + (yAVal * xBVal);
            }

            /* Multiply results with scalar complex alpha */
            float CrealValue = (realSum * alpha.x) - (imgSum * alpha.y);
            float CimgValue  = (realSum * alpha.y) + (imgSum * alpha.x);

            float xCVal = C[cOffset + row * N + col].x;
            float yCVal = C[cOffset + row * N + col].y;
            /* Multiply C matrix with scalar beta complex number */
            tempReal = xCVal * beta.x - yCVal * beta.y;
            tempImg = xCVal * beta.y + yCVal * beta.x;

            /* Add both the results and store in C Matrix */
            C[cOffset + row * N + col].x = tempReal + CrealValue;
            C[cOffset + row * N + col].y = tempImg + CimgValue;
       }

  });

  C.synchronize();

}

void cgemm_NoTransA(int M, int N, int K, float_2 alpha,
                    Concurrency::array_view<float_2> &A, long aOffset, long lda,
                    Concurrency::array_view<float_2> &B, long bOffset, long ldb,
                    float_2 beta,
                    Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(M,N);

    Concurrency::parallel_for_each(grdExt, [=](index<2> idx) restrict(amp)
    {
        int row = idx[0];
        int col = idx[1];
        float tempReal = 0.0;
        float tempImg = 0.0;

        /* column by column multiplication */
        float realSum = 0.0;
        float imgSum = 0.0;

        if (row < M && col < N ) {
            for (int i = 0 ; i < K; i++) {
                float xAVal = A[aOffset + row + i * M].x;
                float xBVal = B[bOffset + col + i * N].x;
                float yAVal = A[aOffset + row + i * M].y;
                float yBVal = B[bOffset + col + i * N].y;

                realSum += (xAVal * xBVal - yAVal * yBVal);
                imgSum += (xAVal * yBVal + yAVal * xBVal);
            }

            /* Multiply results with scalar complex alpha */
            float CrealValue = (realSum * alpha.x) - (imgSum * alpha.y);
            float CimgValue  = (realSum * alpha.y) + (imgSum * alpha.x);

            float xCVal = C[cOffset + row * N + col].x;
            float yCVal = C[cOffset + row * N + col].y;

            /* Multiply C matrix with scalar beta complex number */
            tempReal = xCVal * beta.x - yCVal * beta.y;
            tempImg = xCVal * beta.y + yCVal * beta.x;

            /* Add both the results and store in C Matrix */
            C[cOffset + row * N + col].x = tempReal + CrealValue;
            C[cOffset + row * N + col].y = tempImg + CimgValue;
        }
    });
    C.synchronize();
}

void cgemm_NoTransB(int M, int N, int K, float_2 alpha,
                    Concurrency::array_view<float_2> &A, long aOffset, long lda,
                    Concurrency::array_view<float_2> &B, long bOffset, long ldb,
                    float_2 beta,
                    Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(M,N);

    Concurrency::parallel_for_each(grdExt , [=](index<2> idx) restrict(amp)
    {
        int row = idx[0];
        int col = idx[1];
        float tempReal = 0.0;
        float tempImg = 0.0;
      
        /* row by row multiplication */
        float realSum = 0.0;
        float imgSum = 0.0;
      
        if (row < M && col < N ) {
            for ( int i = 0 ; i < K; i++) {
                float xAVal = A[(row * K) + i].x;
                float xBVal = B[(col * K) + i].x;
                float yAVal = A[(row * K) + i].y;
                float yBVal = B[(col * K) + i].y;

                realSum += (xAVal * xBVal - yAVal * yBVal);
                imgSum += (xAVal * yBVal + yAVal * xBVal);
            }
      
            /* Multiply results with scalar complex alpha */      
            float CrealValue = (realSum * alpha.x) - (imgSum * alpha.y); 
            float CimgValue  = (realSum * alpha.y) + (imgSum * alpha.x);

            float xCVal = C[cOffset + row * N + col].x;
            float yCVal = C[cOffset + row * N + col].y ;

            /* Multiply C matrix with scalar beta complex number */
            tempReal = xCVal * beta.x - yCVal * beta.y;
            tempImg = xCVal * beta.y + yCVal * beta.x;
     
            /* Add both the results and store in C Matrix */
            C[cOffset + row * N + col].x = tempReal + CrealValue;
            C[cOffset + row * N + col].y = tempImg + CimgValue;
        }
    });
    C.synchronize();
}

void cgemm_TransAB(int M, int N, int K, float_2 alpha,
                   Concurrency::array_view<float_2> &A, long aOffset, long lda,
                   Concurrency::array_view<float_2> &B, long bOffset, long ldb,
                   float_2 beta,
                   Concurrency::array_view<float_2> &C, long cOffset, long ldc)
{
    Concurrency::extent<2> grdExt(M,N);

    Concurrency::parallel_for_each(grdExt , [=](index<2> idx) restrict(amp)
    {
        int row = idx[0];
        int col = idx[1];
        float tempReal = 0.0;
        float tempImg = 0.0;

        /* row by column multiplication */
        float realSum = 0.0;
        float imgSum = 0.0;

        if (row < M && col < N ) {
            for ( int i = 0 ; i < K; i++) {
                float xAVal = A[aOffset + row * K + i].x;
                float yAVal = A[aOffset + row * K + i].y;
                float xBVal = B[bOffset + i * N + col].x;
                float yBVal = B[bOffset + i * N + col].y;
                realSum += xAVal * xBVal - yAVal * yBVal;
                imgSum += xAVal * yBVal + yAVal * xBVal;
            }

            /* Multiply results with scalar complex alpha */
            float CrealValue = (realSum * alpha.x) - (imgSum * alpha.y);
            float CimgValue  = (realSum * alpha.y) + (imgSum * alpha.x);

            float xCVal = C[cOffset + row * N + col].x;
            float yCVal = C[cOffset + row * N + col].y;

            /* Multiply C matrix with scalar beta complex number */
            tempReal = xCVal * beta.x - yCVal * beta.y;
            tempImg = xCVal * beta.y + yCVal * beta.x;

            /* Add both the results and store in C Matrix */
            C[cOffset + row * N + col].x = tempReal + CrealValue;
            C[cOffset + row * N + col].y = tempImg + CimgValue;
        }
    });
    C.synchronize();
}

ampblasStatus Ampblaslibrary:: ampblas_cgemm(const enum AMPBLAS_TRANS typeA,
                                             const enum AMPBLAS_TRANS typeB,
                                             const int M, const int N,
                                             const int K, const ampComplex *alpha,
                                             const ampComplex *A, long aOffset,
                                             long lda, const ampComplex *B,
                                             long bOffset, long ldb,
                                             const ampComplex *beta, 
                                             ampComplex *C, long cOffset,
                                             long ldc)
{  
    Concurrency::extent<1> ext(M * K);
    std::vector<float_2> src(M * K);
    array_view<float_2,1> Acmplx(ext, src);

    Concurrency::extent<1> ext2(K * N);
    std::vector<float_2> src2(K * N);
    array_view<float_2,1> Bcmplx(ext2, src2);
   
    Concurrency::extent<1> ext3(M * N);
    std::vector<float_2> src3(M * N);
    array_view<float_2,1> Ccmplx(ext3, src3);

    float_2 Calpha(alpha->real, alpha->img);
    float_2 Cbeta(beta->real, beta->img);
   
    for ( int i = 0 ;i <  M * K;i++) {
        Acmplx[i].x = A[i].real;
        Acmplx[i].y = A[i].img;
    }

    for ( int i = 0 ;i <  K * N;i++) {
        Bcmplx[i].x = B[i].real;
        Bcmplx[i].y = B[i].img;
    }
 
    for ( int i = 0 ;i <  M * N;i++) {
        Ccmplx[i].x = C[i].real;
        Ccmplx[i].y = C[i].img;
    }

   // Start the operations
    if (typeB == noTrans) {
        if (typeA == noTrans) {
            cgemm_NoTransAB(M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, ldb, Cbeta, Ccmplx, cOffset, ldc); 
        }
        else {
            cgemm_NoTransB(M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, lda, Cbeta, Ccmplx, cOffset, ldc);
        }
    }
    else if (typeA == noTrans) {
        cgemm_NoTransA( M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, ldb, Cbeta, Ccmplx, cOffset, ldc);
    }
    else {
        cgemm_TransAB(M, N, K, Calpha, Acmplx, aOffset, lda, Bcmplx, bOffset, ldb, Cbeta, Ccmplx, cOffset, ldc);
    }

    for ( int i = 0 ;i <  M * N;i++) {
        C[i].real = Ccmplx[i].x;
        C[i].img = Ccmplx[i].y;
    }
   
    return AMPBLAS_SUCCESS;

}

