#include "ampblaslib.h"
#include<assert.h>
#include <vector>
#include <amp.h>
#include <iostream>
#include <amp_short_vectors.h>
using namespace Concurrency;
using namespace Concurrency::graphics;
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
                realSum += (A[aOffset + row * K + i].x * B[bOffset + i * N + col].x) - (A[aOffset + row * K + i].y * B[aOffset + i * N + col].y);
                imgSum += (A[aOffset + row * K + i].x * B[bOffset + i * N + col].y) + (A[aOffset + row * K + i].y * B[aOffset + i * N + col].x);
            }
      
            /* Multiply results with scalar complex alpha */      
            float CrealValue = (realSum * alpha.x) - (imgSum * alpha.y); 
            float CimgValue  = (realSum * alpha.y) + (imgSum * alpha.x);
      
            /* Multiply C matrix with scalar beta complex number */
            tempReal = C[cOffset + row * N + col].x * beta.x - C[cOffset + row * N + col].y * beta.y;
            tempImg = C[cOffset + row * N + col].x * beta.y + C[cOffset + row * N + col].y * beta.x;
     
            /* Add both the results and store in C Matrix */
            C[cOffset + row * N + col].x = tempReal + CrealValue;
            C[cOffset + row * N + col].y = tempImg + CimgValue;
       }

  });

  C.synchronize();
  
}
ampblasStatus Ampblaslibrary:: ampblas_cgemm(char transA, char transB, const int M,
                                             const int N, const int K, 
                                             const ampComplex *alpha,
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

    if( transA == 'n' && transB == 'n') 
        cgemm_TransAB( M, N, K, Calpha, Acmplx, 0, lda, Bcmplx, 0, ldb, Cbeta, Ccmplx, 0, ldc);
    else
        return AMPBLAS_ERROR; 

    for ( int i = 0 ;i <  M * K;i++) {
        C[i].real = Ccmplx[i].x;
        C[i].img = Ccmplx[i].y;
    }
   
    return AMPBLAS_SUCCESS;

}

