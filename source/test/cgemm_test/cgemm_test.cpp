#include<amp.h>
#include<cblas.h>
#include<iostream>
#include<unistd.h>
#include<amp_short_vectors.h>
#include "../../ampblaslib.h"
using namespace Concurrency::graphics;
using namespace Concurrency;
using namespace std;
 
int main(int argc, char* argv[])
{
	  /* AMPBLAS implementation */
    
    if (argc < 7) {
        cout<<"No sufficient commandline arguments specified"<<"argc :"<<argc<<endl;
        return -1;
    }
    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int K = atoi(argv[3]);
    int isTransA = (atoi(argv[4]));
    int isTransB = (atoi(argv[5]));
    int Imple_type = (atoi(argv[6])); 
    
    Ampblaslibrary amp;
    bool isPassed = 1;
    float alpha[2], beta[2];
    long lda, ldb,ldc;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    long bOffset = 0;
    long cOffset = 0;
    long A_batchOffset = 0;
    long B_batchOffset = 0;
    long C_batchOffset = M * N;
    int batchSize = 128;

    ampblasStatus status; 
    AMPBLAS_TRANS typeA,typeB ;
    if((isTransA == 0 || isTransA == 1) && (isTransB == 0 || isTransB == 1)) {
        if( isTransA == 0)    typeA = noTrans;
        else typeA = trans;
        if( isTransB == 0) typeB = noTrans;
        else typeB = trans;
    }
    else {
        cout<< "Invalid transpose type specified"<<endl;
        return -1;
    }

    /* CBLAS implementation */
    CBLAS_ORDER order = CblasColMajor;
    enum CBLAS_TRANSPOSE transa, transb;

    if(typeA == noTrans){
       lda = M;
       transa = CblasNoTrans;
       }
    else{
       lda = K;
       transa = CblasTrans;
       }

    if(typeB == noTrans){
       ldb = K;
       transb = CblasNoTrans;
       }
    else{
       ldb = N;
       transb = CblasTrans;
        }

    ldc = M;
    
    struct ampComplex calpha,cbeta;

    struct ampComplex *Aamp = (struct ampComplex *) calloc(M * K, sizeof(struct ampComplex ));
    struct ampComplex *Bamp = (struct ampComplex *) calloc(K * N, sizeof(struct ampComplex ));
    struct ampComplex *Camp = (struct ampComplex *) calloc(M * N, sizeof(struct ampComplex ));

    Concurrency::graphics::float_2 cAlpha, cBeta;
    cAlpha.x = alpha[0] = calpha.real = 1;
    cAlpha.y = alpha[1] = calpha.img  = 1;
    cBeta.x = beta[0] = cbeta.real = 1;
    cBeta.y = beta[1] = cbeta.img  = 1;
 
    Concurrency::array_view<float_2> A(M * K * 2);
    Concurrency::array_view<float_2> B(N * K * 2);
    Concurrency::array_view<float_2> C(M * N * 2);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

 
    float* a = (float *)malloc(sizeof(float )* M * K * 2);
    float* b = (float *)malloc(sizeof(float )* K * N * 2);
    float* c = (float *)malloc(sizeof(float )* M * N * 2);
   
    if(M > 3000 && N > 3000){
        batchSize = 25;
    }
    if(M > 9000 && N > 9000){
        batchSize = 1;
    }

    Concurrency::array_view<float_2> Abatch(M * K * 2);
    Concurrency::array_view<float_2> Bbatch(N * K * 2);
    Concurrency::array_view<float_2> Cbatch(M * N * 2 * batchSize);
    float* abatch = (float *)malloc(sizeof(float )* M * K * 2);
    float* bbatch = (float *)malloc(sizeof(float )* K * N * 2);
    float* cbatch = (float *)malloc(sizeof(float )* M * N * 2 * batchSize);

  
    int k = 0;
    for (int i = 0;i < M * K; i++) {

      A[i].x = rand() % 10;
      A[i].y = rand() % 20;
      a[k++] = Aamp[i].real = A[i].x;
      a[k++] = Aamp[i].img = A[i].y;
    }

    k = 0;
    for (int i = 0;i < K * N; i++) {
      B[i].x = rand() % 15;
      B[i].y = rand() % 25;
      b[k++] = Bamp[i].real = B[i].x;
      b[k++] = Bamp[i].img = B[i].y;
    }

    k = 0;
    for (int i = 0;i < M * N; i++) {
      C[i].x = rand() % 18;
      C[i].y = rand() % 28;
      c[k++] = Camp[i].real = C[i].x ;
      c[k++] = Camp[i].img = C[i].y;
    }

    if(Imple_type == 1){
    	status = amp.ampblas_cgemm(typeA, typeB, M, N, K, &calpha, Aamp, aOffset, lda, Bamp, bOffset, ldb, &cbeta, Camp, cOffset, ldc);
        cblas_cgemm( order, transa, transb, M, N, K, &alpha, a, lda, b, ldb, &beta, c, ldc );
        for(int i = 0,k = 0; ((i < M * N) && (k < M * N * 2)) ; i++, k = k + 2){
            if ((Camp[i].real != c[k]) || (Camp[i].img != c[k+1])){
                isPassed = 0;
                cout <<" AMPCGEMM_REAL[" << i<< "] " << C[i].x << " does not match with CBLASCGEMM_REAL[" << k <<"] "<< c[k] << endl;
                cout <<" AMPCGEMM_IMG[" << i<< "] " << C[i].y << " does not match with CBLASCGEMM_IMG[" << k <<"] "<< c[k + 1] << endl;
            }
            else
               continue;
         
         }
   
        cout << (isPassed ? "TEST PASSED" : "TEST FAILED") << endl;
        free(a);
        free(b);
        free(c);
        free(Aamp);
        free(Bamp);
        free(Camp);
    }

    else if(Imple_type ==2){
    	status = amp.ampblas_cgemm(accl_view, typeA, typeB, M, N, K, cAlpha, A, aOffset, lda, B, bOffset, ldb, cBeta, C, cOffset, ldc);
        cblas_cgemm( order, transa, transb, M, N, K, &alpha, a, lda, b, ldb, &beta, c, ldc );
        for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2){
            if ((C[i].x != c[k]) || (C[i].y != c[k+1])){
                isPassed = 0;
                cout <<" AMPCGEMM_REAL[" << i<< "] " << C[i].x << " does not match with CBLASCGEMM_REAL[" << k <<"] "<< c[k] << endl;
                cout <<" AMPCGEMM_IMG[" << i<< "] " << C[i].y << " does not match with CBLASCGEMM_IMG[" << k <<"] "<< c[k + 1] << endl;
            }
            else
               continue;

         }

        cout << (isPassed ? "TEST PASSED" : "TEST FAILED") << endl;
    }

    else{
        
        int k = 0;
        for (int i = 0;i < M * K; i++) {
           Abatch[i].x = rand() % 10;
           Abatch[i].y = rand() % 20;
           abatch[k++] = Abatch[i].x;
           abatch[k++] = Abatch[i].y;
        }

        k = 0;
        for (int i = 0;i < K * N; i++) {
           Bbatch[i].x = rand() % 15;
           Bbatch[i].y = rand() % 25;
           bbatch[k++] = Bbatch[i].x;
           bbatch[k++] = Bbatch[i].y;
        }

        k = 0;
        for (int i = 0;i < M * N * batchSize; i++) {
           Cbatch[i].x = rand() % 18;
           Cbatch[i].y = rand() % 28;
           cbatch[k++] = Cbatch[i].x ;
           cbatch[k++] = Cbatch[i].y;
        }  

    	status = amp.ampblas_cgemm(accl_view, typeA, typeB, M, N, K, cAlpha, Abatch, aOffset, A_batchOffset, lda, Bbatch, bOffset, B_batchOffset, ldb, cBeta, Cbatch, cOffset, C_batchOffset, ldc, batchSize);
        for(int i = 0; i < batchSize;i++)
	     cblas_cgemm( order, transa, transb, M, N, K, &alpha, abatch, lda, bbatch, ldb, &beta, cbatch + i * M * N * 2, ldc );
        for(int i = 0,k = 0; ((i < M * N * batchSize)&&( k < M * N * 2 * batchSize)); i++, k = k + 2){
            if ((Cbatch[i].x != cbatch[k]) || (Cbatch[i].y != cbatch[k+1])){
                isPassed = 0;
                cout <<" AMPCGEMM_REAL[" << i<< "] " << Cbatch[i].x << " does not match with CBLASCGEMM_REAL[" << k <<"] "<< cbatch[k] << endl;
                cout <<" AMPCGEMM_IMG[" << i<< "] " << Cbatch[i].y << " does not match with CBLASCGEMM_IMG[" << k <<"] "<< cbatch[k + 1] << endl;
            }
            else
               continue;

        }
        cout << (isPassed ? "TEST PASSED" : "TEST FAILED") << endl;
    }
    return 0;
 
}

