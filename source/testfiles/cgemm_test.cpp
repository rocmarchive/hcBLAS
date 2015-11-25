#include<amp.h>
#include<iostream>
#include<amp_short_vectors.h>
#include "../hcblas.h"
#ifdef LINUX
#include<cblas.h>
#include<unistd.h>
#else
#include<io.h>
#endif
using namespace Concurrency::graphics;
using namespace Concurrency;
using namespace std;
int main(int argc, char* argv[])
{
	  /* HCBLAS implementation */
    Hcblaslibrary hc;  
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
    float alpha[2], beta[2];
    long lda, ldb, ldc;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    long bOffset = 0;
    long cOffset = 0;
    long A_batchOffset = 0;
    long B_batchOffset = 0;
    long C_batchOffset = M * N;
    int batchSize = 128;
    hcblasOrder hcOrder = ColMajor;
    hcblasStatus status; 
    hcblasTranspose typeA,typeB ;
    if((isTransA == 0 || isTransA == 1) && (isTransB == 0 || isTransB == 1)) {
        if( isTransA == 0){
	    typeA = NoTrans;
            lda = (hcOrder)? M : K;
        }
        else{
	    typeA = Trans;
            lda = (hcOrder)? K : M;
        }
        if( isTransB == 0){
            typeB = NoTrans;
            ldb = (hcOrder)? K : N;
        }
        else{
            typeB = Trans;
            ldb = (hcOrder)? N : K;
        }
    }
    else {
        cout<< "Invalid Transpose type specified"<<endl;
        return -1;
    }
    ldc = (hcOrder)? M : N;
    struct hcComplex calpha,cbeta;
    struct hcComplex *Ahc = (struct hcComplex *) calloc(M * K, sizeof(struct hcComplex ));
    struct hcComplex *Bhc = (struct hcComplex *) calloc(K * N, sizeof(struct hcComplex ));
    struct hcComplex *Chc = (struct hcComplex *) calloc(M * N, sizeof(struct hcComplex ));
    Concurrency::graphics::float_2 cAlpha, cBeta;
    cAlpha.x = calpha.real = 1;
    cAlpha.y = calpha.img  = 1;
    cBeta.x = cbeta.real = 1;
    cBeta.y = cbeta.img  = 1;
#ifdef LINUX 
    /* CBLAS implementation */
    bool isPassed = 1;
    CBLAS_ORDER order = CblasColMajor;
    enum CBLAS_TRANSPOSE Transa, Transb;
    Transa = (typeA == NoTrans)? CblasNoTrans: CblasTrans;
    Transb = (typeB == NoTrans)? CblasNoTrans: CblasTrans;
    alpha[0] = calpha.real;
    alpha[1] = calpha.img;
    beta[0] = cbeta.real;
    beta[1] = cbeta.img;
#endif
    Concurrency::array<float_2> A(M * K * 2);
    Concurrency::array<float_2> B(N * K * 2);
    Concurrency::array<float_2> C(M * N * 2);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    float* a = (float *)malloc(sizeof(float )* M * K * 2);
    float* b = (float *)malloc(sizeof(float )* K * N * 2);
    float* c = (float *)malloc(sizeof(float )* M * N * 2);
    if(M > 3000 && N > 3000) {
        batchSize = 25;
    }
    if(M > 9000 && N > 9000) {
        batchSize = 1;
    }
    std::vector<float_2> HostA(M * K * 2);
    std::vector<float_2> HostB(K * N * 2);
    std::vector<float_2> HostC(M * N * 2);
    int k = 0;
    for (int i = 0;i < M * K; i++) {
      HostA[i].x = rand() % 10;
      HostA[i].y = rand() % 20;
      a[k++] = Ahc[i].real = HostA[i].x;
      a[k++] = Ahc[i].img = HostA[i].y;
    }

    k = 0;
    for (int i = 0;i < K * N; i++) {
      HostB[i].x = rand() % 15;
      HostB[i].y = rand() % 25;
      b[k++] = Bhc[i].real = HostB[i].x;
      b[k++] = Bhc[i].img = HostB[i].y;
    }
    for(int iter=0; iter<10; iter++){
    k = 0;
    for (int i = 0;i < M * N; i++) {
      HostC[i].x = rand() % 18;
      HostC[i].y = rand() % 28;
      c[k++] = Chc[i].real = HostC[i].x ;
      c[k++] = Chc[i].img = HostC[i].y;
    }
    if(Imple_type == 1) {
    	status = hc.hcblas_cgemm(hcOrder, typeA, typeB, M, N, K, &calpha, Ahc, aOffset, lda, Bhc, bOffset, ldb, &cbeta, Chc, cOffset, ldc);
#ifdef LINUX
        cblas_cgemm( order, Transa, Transb, M, N, K, &alpha, a, lda, b, ldb, &beta, c, ldc );
        for(int i = 0,k = 0; ((i < M * N) && (k < M * N * 2)) ; i++, k = k + 2){
            if ((Chc[i].real != c[k]) || (Chc[i].img != c[k+1])){
                isPassed = 0;
                cout <<" HCCGEMM_REAL[" << i<< "] " << C[i].x << " does not match with CBLASCGEMM_REAL[" << k <<"] "<< c[k] << endl;
                cout <<" HCCGEMM_IMG[" << i<< "] " << C[i].y << " does not match with CBLASCGEMM_IMG[" << k <<"] "<< c[k + 1] << endl;
            }
            else
               continue;
         
         }
        cout << (isPassed ? "TEST PASSED" : "TEST FAILED") << endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
    }

    else if(Imple_type ==2){
        Concurrency::copy(begin(HostA), end(HostA), A);
        Concurrency::copy(begin(HostB), end(HostB), B);
        Concurrency::copy(begin(HostC), end(HostC), C);
    	status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, K, cAlpha, A, aOffset, lda, B, bOffset, ldb, cBeta, C, cOffset, ldc);
        Concurrency::copy(C, begin(HostC));
#ifdef LINUX
        cblas_cgemm( order, Transa, Transb, M, N, K, &alpha, a, lda, b, ldb, &beta, c, ldc );
        for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2){
            if ((HostC[i].x != c[k]) || (HostC[i].y != c[k+1])){
                isPassed = 0;
                cout <<" HCCGEMM_REAL[" << i<< "] " << HostC[i].x << " does not match with CBLASCGEMM_REAL[" << k <<"] "<< c[k] << endl;
                cout <<" HCCGEMM_IMG[" << i<< "] " << HostC[i].y << " does not match with CBLASCGEMM_IMG[" << k <<"] "<< c[k + 1] << endl;
            }
            else
               continue;

         }
        cout << (isPassed ? "TEST PASSED" : "TEST FAILED") << endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
    }

    else{
        Concurrency::array<float_2> Abatch(M * K * 2);
        Concurrency::array<float_2> Bbatch(N * K * 2);
        Concurrency::array<float_2> Cbatch(M * N * 2 * batchSize);
        float* abatch = (float *)malloc(sizeof(float )* M * K * 2);
        float* bbatch = (float *)malloc(sizeof(float )* K * N * 2);
        float* cbatch = (float *)malloc(sizeof(float )* M * N * 2 * batchSize);
        std::vector<float_2> HostC_batch(M * N * 2 * batchSize);
        int k = 0;
        for (int i = 0;i < M * K; i++) {
           HostA[i].x = rand() % 10;
           HostA[i].y = rand() % 20;
           abatch[k++] = HostA[i].x;
           abatch[k++] = HostA[i].y;
        }

        k = 0;
        for (int i = 0;i < K * N; i++) {
           HostB[i].x = rand() % 15;
           HostB[i].y = rand() % 25;
           bbatch[k++] = HostB[i].x;
           bbatch[k++] = HostB[i].y;
        }

        k = 0;
        for (int i = 0;i < M * N * batchSize; i++) {
           HostC_batch[i].x = rand() % 18;
           HostC_batch[i].y = rand() % 28;
           cbatch[k++] = HostC_batch[i].x ;
           cbatch[k++] = HostC_batch[i].y;
        }  
        Concurrency::copy(begin(HostA), end(HostA), Abatch);
        Concurrency::copy(begin(HostB), end(HostB), Bbatch);
        Concurrency::copy(begin(HostC_batch), end(HostC_batch), Cbatch);
    	status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, K, cAlpha, Abatch, aOffset, A_batchOffset, lda, Bbatch, bOffset, B_batchOffset, ldb, cBeta, Cbatch, cOffset, C_batchOffset, ldc, batchSize);
        Concurrency::copy(Cbatch, begin(HostC_batch));  
#ifdef LINUX
        for(int i = 0; i < batchSize;i++)
	     cblas_cgemm( order, Transa, Transb, M, N, K, &alpha, abatch, lda, bbatch, ldb, &beta, cbatch + i * M * N * 2, ldc );
        for(int i = 0,k = 0; ((i < M * N * batchSize)&&( k < M * N * 2 * batchSize)); i++, k = k + 2){
            if ((HostC_batch[i].x != cbatch[k]) || (HostC_batch[i].y != cbatch[k+1])){
                isPassed = 0;
                cout <<" HCCGEMM_REAL[" << i<< "] " << HostC_batch[i].x << " does not match with CBLASCGEMM_REAL[" << k <<"] "<< cbatch[k] << endl;
                cout <<" HCCGEMM_IMG[" << i<< "] " << HostC_batch[i].y << " does not match with CBLASCGEMM_IMG[" << k <<"] "<< cbatch[k + 1] << endl;
            }
            else
               continue;
        }
        cout << (isPassed ? "TEST PASSED" : "TEST FAILED") << endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
    }
}
    return 0;
}

