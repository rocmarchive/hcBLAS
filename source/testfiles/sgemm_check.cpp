#include <iostream>
#include "../hcblas.h"
#include <cstdlib>
#include "cblas.h"

using namespace std;

int main(int argc,char* argv[])
{  
    /* HCBLAS Implementation */
    Hcblaslibrary hc; 
    if (argc < 6){
        cout<<"No sufficient commandline arguments specified"<<"argc :"<<argc<<endl;
        return -1;
    }
    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int K = atoi(argv[3]);
    int isTransA = (atoi(argv[4]));
    int isTransB = (atoi(argv[5])); 
    bool ispassed = 1;
    float alpha = 1;
    float beta = 1;
    long lda;
    long ldb;
    long ldc = M;
    int incX = 1;
    int incY = 1;
    long aOffset = 0;
    long bOffset = 0;
    long cOffset = 0;
    HCBLAS_ORDER hcOrder = colMajor;
    HCBLAS_TRANS typeA,typeB ;
    hcblasStatus status;
    
    /* CBLAS implementation */
    enum CBLAS_ORDER order;
    enum CBLAS_TRANSPOSE transa, transb;
    order = CblasColMajor;
    if((isTransA == 0 || isTransA == 1) && (isTransB == 0 || isTransB == 1)) {
        if( isTransA == 0){
            typeA = noTrans;
            transa = CblasNoTrans;
            (hcOrder)?(lda = M):(lda = K);
        }
        else{
            typeA = trans;
            transa = CblasTrans;
            (hcOrder)?(lda = K):(lda = M);
        }
        if( isTransB == 0){
            typeB = noTrans;
            transb = CblasNoTrans;
            (hcOrder)?(ldb = K):(ldb = N);
        }
        else{
            typeB = trans;
            transb = CblasTrans;
            (hcOrder)?(ldb = N):(ldb = K);
        }
    }
    else {
        cout<< "Invalid transpose type specified"<<endl;
        return -1;
    }

    if(hcOrder)
       ldc = M;
    else
       ldc = N;

    float *Asgemm = (float*) calloc(M * K, sizeof(float));
    float *Bsgemm = (float*) calloc(K * N, sizeof(float));
    float *Csgemm = (float*) calloc(M * N, sizeof(float));
    float *C_cblas = (float*) calloc(M * N, sizeof(float));
        for(int i = 0; i < M * K; i++){
            Asgemm[i] = rand()%100;
        }
        for(int i = 0; i < K * N;i++){
            Bsgemm[i] = rand() % 15;
        }
        for(int iter = 0; iter < 10; iter++) { 
        for(int i = 0; i < M * N;i++)  {
            Csgemm[i] = rand() % 25;
            C_cblas[i] = Csgemm[i];
        }
            status = hc.hcblas_sgemm(hcOrder, typeA, typeB, M, N, K, &alpha, Asgemm, lda, Bsgemm,ldb, &beta, Csgemm, ldc, aOffset, bOffset, cOffset);
            cblas_sgemm( order, transa, transb, M, N, K, alpha, Asgemm, lda, Bsgemm, ldb, beta, C_cblas, ldc );
            for(int i = 0 ; i < M * N ; i++){ 
                if( C_cblas[i] != (Csgemm[i])){
                    ispassed = 0;
                    cout << " HCSGEMM["<<i<<"] = "<<Csgemm[i]<<" doesnot match with CBLASSGEMM["<<i<<"] =" << C_cblas[i] << endl;
                    break;
                }
                else
                   continue;
            }
            cout << (ispassed?"TEST PASSED":"TEST FAILED")<< endl;
        }
     
    return 0;   
}
   
   
  
