#include<hc.hpp>
#include<iostream>
#include<amp_short_vectors.h>
#include "hcblaslib.h"
#include<cblas.h>
#include<unistd.h>
using namespace Concurrency::graphics;
using namespace hc;
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
    long lda, ldb, ldc;
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
        if(isTransA == 0) {
	    typeA = NoTrans;
            lda = (hcOrder)? M : K;
        }
        else {
	    typeA = Trans;
            lda = (hcOrder)? K : M;
        }
        if(isTransB == 0) {
            typeB = NoTrans;
            ldb = (hcOrder)? K : N;
        }
        else {
            typeB = Trans;
            ldb = (hcOrder)? N : K;
        }
    }
    else {
        cout<< "Invalid Transpose type specified"<<endl;
        return -1;
    }
    ldc = (hcOrder)? M : N;
    struct hc_Complex calpha,cbeta;
    Concurrency::graphics::float_2 cAlpha, cBeta;
    cAlpha.x = calpha.real = 1;
    cAlpha.y = calpha.img  = 1;
    cBeta.x = cbeta.real = 1;
    cBeta.y = cbeta.img  = 1;
    /* CBLAS implementation */
    bool ispassed = 1;
    float alpha[2], beta[2];
    CBLAS_ORDER order = CblasColMajor;
    enum CBLAS_TRANSPOSE Transa, Transb;
    Transa = (typeA == NoTrans)? CblasNoTrans: CblasTrans;
    Transb = (typeB == NoTrans)? CblasNoTrans: CblasTrans;
    alpha[0] = calpha.real;
    alpha[1] = calpha.img;
    beta[0] = cbeta.real;
    beta[1] = cbeta.img;
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    hc::accelerator_view accl_view = (acc[1].create_view());
    float* a = (float *)malloc(sizeof(float )* M * K * 2);
    float* b = (float *)malloc(sizeof(float )* K * N * 2);
    float* c = (float *)malloc(sizeof(float )* M * N * 2);
    if(M > 3000 && N > 3000) {
        batchSize = 25;
    }
    if(M > 9000 && N > 9000) {
        batchSize = 1;
    }

/* Implementation type I - Inputs and Outputs are HCC float array containers */

    if(Imple_type == 1) {
        hc::array<float_2> A(M * K * 2);
        hc::array<float_2> B(N * K * 2);
        hc::array<float_2> C(M * N * 2);
        std::vector<float_2> HostA(M * K * 2);
        std::vector<float_2> HostB(K * N * 2);
        std::vector<float_2> HostC(M * N * 2);
        int k = 0;
        for (int i = 0;i < M * K; i++) {
            HostA[i].x = rand() % 10;
            HostA[i].y = rand() % 20;
            a[k++] = HostA[i].x;
            a[k++] = HostA[i].y;
        }
        k = 0;
        for (int i = 0;i < K * N; i++) {
            HostB[i].x = rand() % 15;
            HostB[i].y = rand() % 25;
            b[k++] = HostB[i].x;
            b[k++] = HostB[i].y;
        }
#ifdef PROFILE
        for (int iter=0; iter<10; iter++) {
#endif
        k = 0;
        for (int i = 0;i < M * N; i++) {
            HostC[i].x = rand() % 18;
            HostC[i].y = rand() % 28;
            c[k++] = HostC[i].x ;
            c[k++] = HostC[i].y;
        }
        hc::copy(begin(HostA), end(HostA), A);
        hc::copy(begin(HostB), end(HostB), B);
        hc::copy(begin(HostC), end(HostC), C);
    	status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, K, cAlpha, A, aOffset, lda, B, bOffset, ldb, cBeta, C, cOffset, ldc);
        hc::copy(C, begin(HostC));
        cblas_cgemm( order, Transa, Transb, M, N, K, &alpha, a, lda, b, ldb, &beta, c, ldc );
        for(int i = 0,k = 0; ((i < M * N) && ( k < M * N * 2)) ; i++, k = k + 2){
            if ((HostC[i].x != c[k]) || (HostC[i].y != c[k+1])){
                ispassed = 0;
                cout <<" HCCGEMM_REAL[" << i<< "] " << HostC[i].x << " does not match with CBLASCGEMM_REAL[" << k <<"] "<< c[k] << endl;
                cout <<" HCCGEMM_IMG[" << i<< "] " << HostC[i].y << " does not match with CBLASCGEMM_IMG[" << k <<"] "<< c[k + 1] << endl;
            }
            else
               continue;

         }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl; 
#ifdef PROFILE
        }
#endif
    }

/* Implementation type II - Inputs and Outputs are HCC float array containers with batch processing */

    else{
        hc::array<float_2> Abatch(M * K * 2);
        hc::array<float_2> Bbatch(N * K * 2);
        hc::array<float_2> Cbatch(M * N * 2 * batchSize);
        float* abatch = (float *)malloc(sizeof(float )* M * K * 2);
        float* bbatch = (float *)malloc(sizeof(float )* K * N * 2);
        float* cbatch = (float *)malloc(sizeof(float )* M * N * 2 * batchSize);
        std::vector<float_2> HostA(M * K * 2);
        std::vector<float_2> HostB(K * N * 2);
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
#ifdef PROFILE
        for(int iter=0; iter<10; iter++) {   
#endif
        k = 0;
        for (int i = 0;i < M * N * batchSize; i++) {
           HostC_batch[i].x = rand() % 18;
           HostC_batch[i].y = rand() % 28;
           cbatch[k++] = HostC_batch[i].x ;
           cbatch[k++] = HostC_batch[i].y;
        }  
        hc::copy(begin(HostA), end(HostA), Abatch);
        hc::copy(begin(HostB), end(HostB), Bbatch);
        hc::copy(begin(HostC_batch), end(HostC_batch), Cbatch);
    	status = hc.hcblas_cgemm(accl_view, hcOrder, typeA, typeB, M, N, K, cAlpha, Abatch, aOffset, A_batchOffset, lda, Bbatch, bOffset, B_batchOffset, ldb, cBeta, Cbatch, cOffset, C_batchOffset, ldc, batchSize);
        hc::copy(Cbatch, begin(HostC_batch));  
        for(int i = 0; i < batchSize;i++)
	     cblas_cgemm( order, Transa, Transb, M, N, K, &alpha, abatch, lda, bbatch, ldb, &beta, cbatch + i * M * N * 2, ldc );
        for(int i = 0,k = 0; ((i < M * N * batchSize)&&( k < M * N * 2 * batchSize)); i++, k = k + 2){
            if ((HostC_batch[i].x != cbatch[k]) || (HostC_batch[i].y != cbatch[k+1])){
                ispassed = 0;
                cout <<" HCCGEMM_REAL[" << i<< "] " << HostC_batch[i].x << " does not match with CBLASCGEMM_REAL[" << k <<"] "<< cbatch[k] << endl;
                cout <<" HCCGEMM_IMG[" << i<< "] " << HostC_batch[i].y << " does not match with CBLASCGEMM_IMG[" << k <<"] "<< cbatch[k + 1] << endl;
            }
            else
               continue;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl; 
#ifdef PROFILE
        }
#endif
    }
    return 0;
}

