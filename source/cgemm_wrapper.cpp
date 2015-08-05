#include "cgemm_kernels.h"

// CGEMM Wrapper routine that invokes the appropriate kernel routines depending on the input dimension M N and K
// CGEMM Call Type 1: Inputs and Outputs are host float pointers
ampblasStatus Ampblaslibrary:: ampblas_cgemm(const int order, const enum AMPBLAS_TRANS typeA,
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

    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

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
    
   ampblasStatus status;
   // Start the operations
   if(order){
   	if (typeB == noTrans) {
        	if (typeA == noTrans) {
        	    	status = cgemm_NoTransAB(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
		}
        	else {
            		status = cgemm_NoTransB(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
        	}
    	}
    	else if (typeA == noTrans) {
        	status = cgemm_NoTransA(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    	}
    	else {
        	status = cgemm_TransAB(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    	}
    }
    else{
        if (typeB == noTrans) {
                if (typeA == noTrans) {
                        status = cgemm_NoTransAB_rMajor(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
                }
                else {
                        status = cgemm_NoTransB_rMajor(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
                }
        }
        else if (typeA == noTrans) {
                status = cgemm_NoTransA_rMajor(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
        }
        else {
                status = cgemm_TransAB_rMajor(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
        }
    }

    for ( int i = 0 ;i <  M * N;i++) {
        C[i].real = Ccmplx[i].x;
        C[i].img = Ccmplx[i].y;
    }
   
    return status;

}

// CGEMM Call Type II: Inputs and outputs are C++ AMP float array_View containers
ampblasStatus Ampblaslibrary :: ampblas_cgemm(Concurrency::accelerator_view &accl_view,
					      const int order, const enum AMPBLAS_TRANS typeA,
                                              const enum AMPBLAS_TRANS typeB, const int M,
                                              const int N, const int K,
                                              const Concurrency::graphics::float_2 &Calpha,
                                              Concurrency::array_view<float_2> &Acmplx, long aOffset, long lda,
                                              Concurrency::array_view<float_2> &Bcmplx, long bOffset, long ldb,
                                              const Concurrency::graphics::float_2 &Cbeta,
                                              Concurrency::array_view<float_2> &Ccmplx, long cOffset, long ldc)
{
  int i, j;
  ampblasStatus status = AMPBLAS_SUCCESS;
    // Quick return if possible
  if (!M || !N || (((Calpha.x  == 0 && Calpha.y == 0) || !K) && (Cbeta.x == 1 && Cbeta.y == 1)))
    return AMPBLAS_INVALID;
  // For alpha = 0
  if (!Calpha.x  && !Calpha.y)
  {
    if (!Cbeta.x && !Cbeta.y)
    {
      for (j = 0; j < N; ++j)
        for (i = 0; i < M; ++i)
          Ccmplx[i + j * ldc].x = 0;
          Ccmplx[i + j * ldc].y = 0;
    }
    else
    {
      for (j = 0; j < N; ++j)
        for (i = 0; i < M; ++i)
          Ccmplx[i + j * ldc].x *= Cbeta.x;
          Ccmplx[i + j * ldc].y *= Cbeta.y;
    }
    return status;
  }
  
  if(order){
    	if (typeB == noTrans) {
        	if (typeA == noTrans) {
           		status = cgemm_NoTransAB(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
		}
        	else {
            		status = cgemm_NoTransB(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
        	}
    	}
   	else if (typeA == noTrans) {
        	status = cgemm_NoTransA(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
   	}
    	else {
        	status = cgemm_TransAB(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
    	}
   }
   else{
        if (typeB == noTrans) {
                if (typeA == noTrans) {
                        status = cgemm_NoTransAB_rMajor(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
                }
                else {
                        status = cgemm_NoTransB_rMajor(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
                }
        }
        else if (typeA == noTrans) {
                status = cgemm_NoTransA_rMajor(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
        }
        else {
                status = cgemm_TransAB_rMajor(accl_view, Acmplx, aOffset,Bcmplx, bOffset, Ccmplx, cOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta);
        }
    }

    return status;
}

/* CGEMM Call Type III - Overloaded function with arguments related to batch processing */
ampblasStatus Ampblaslibrary :: ampblas_cgemm(Concurrency::accelerator_view &accl_view,
                                              const int order, const enum AMPBLAS_TRANS typeA,
                                              const enum AMPBLAS_TRANS typeB, const int M,
                                              const int N, const int K,
                                              const Concurrency::graphics::float_2 &Calpha,
                                              Concurrency::array_view<float_2> &Acmplx, 
                                              const long aOffset, const long A_batchOffset, const long lda,
                                              Concurrency::array_view<float_2> &Bcmplx, 
			                      const long bOffset, const long B_batchOffset, const long ldb,
                                              const Concurrency::graphics::float_2 &Cbeta,
                                              Concurrency::array_view<float_2> &Ccmplx, 
			                      const long cOffset, const long C_batchOffset, const long ldc, const int batchSize)
{
  int i, j;
  ampblasStatus status = AMPBLAS_SUCCESS;
    // Quick return if possible
  if (!M || !N || (((Calpha.x  == 0 && Calpha.y == 0) || !K) && (Cbeta.x == 1 && Cbeta.y == 1)))
    return AMPBLAS_INVALID;
  // For alpha = 0
  if (!Calpha.x  && !Calpha.y)
  {
    if (!Cbeta.x && !Cbeta.y)
    {
      for (j = 0; j < N; ++j)
        for (i = 0; i < M; ++i)
          Ccmplx[i + j * ldc].x = 0;
          Ccmplx[i + j * ldc].y = 0;
    }
    else
    {
      for (j = 0; j < N; ++j)
        for (i = 0; i < M; ++i)
          Ccmplx[i + j * ldc].x *= Cbeta.x;
          Ccmplx[i + j * ldc].y *= Cbeta.y;
    }
    return status;
  }

  if(order){
  	if (typeB == noTrans){
        	if (typeA == noTrans){
            		status = cgemm_NoTransAB(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
		}
        	else{
            		status = cgemm_NoTransB(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
        	}
    	}
   	else if (typeA == noTrans) {
        	status = cgemm_NoTransA(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
    	}
    	else {
        	status = cgemm_TransAB(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
    	}
   }
   else{
        if (typeB == noTrans) {
                if (typeA == noTrans) {
                        status = cgemm_NoTransAB_rMajor(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
                }
                else {
                        status = cgemm_NoTransB_rMajor(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
                }
        }
        else if (typeA == noTrans) {
                status = cgemm_NoTransA_rMajor(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
        }
        else {
                status = cgemm_TransAB_rMajor(accl_view, Acmplx, aOffset, A_batchOffset, Bcmplx, bOffset, B_batchOffset, Ccmplx, cOffset, C_batchOffset, M, N, K, lda, ldb, ldc, Calpha, Cbeta, batchSize);
        }
    }

    return status;
}


