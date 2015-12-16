#include <iostream>
#include "hcblas.h"
#include <cstdlib>
#ifdef LINUX
#include "cblas.h"
#endif
using namespace std;

int main(int argc,char* argv[])
{  
    /* HCBLAS Implementation */
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
    float alpha = 1;
    float beta = 1;
    long lda;
    long ldb;
    long ldc;
    int incX = 1;
    int incY = 1;
    long aOffset = 0;
    long bOffset = 0;
    long cOffset = 0;
    long A_batchOffset = 0;
    long B_batchOffset = 0;
    long C_batchOffset = M * N;
    int batchSize = 128;
    hcblasOrder hcOrder = ColMajor;
    hcblasTranspose typeA, typeB;
    hcblasStatus status;
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

#ifdef LINUX 
    /* CBLAS implementation */
    bool ispassed = 1;
    enum CBLAS_ORDER order;
    enum CBLAS_TRANSPOSE Transa, Transb;
    order = CblasColMajor;
    Transa = (typeA == NoTrans)?CblasNoTrans:CblasTrans;
    Transb = (typeB == NoTrans)?CblasNoTrans:CblasTrans;
    float *C_cblas = (float*) calloc(M * N, sizeof(float));
#endif
    float *Asgemm = (float*) calloc(M * K, sizeof(float));
    float *Bsgemm = (float*) calloc(K * N, sizeof(float));
    float *Csgemm = (float*) calloc(M * N, sizeof(float));
    if(M > 3000 && N > 3000){
	batchSize = 25;
    }
    if(M > 9000 && N > 9000){
        batchSize = 1;
    }
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view()); 

/* Implementation type I - Inputs and Outputs are host float pointers */

       if(Imple_type == 1) {    
            for(int i = 0; i < M * K; i++) {
                Asgemm[i] = rand() % 100;
            }
            for(int i = 0; i < K * N;i++) {
                Bsgemm[i] = rand() % 15;
            }
#ifdef PROFILE
            for(int iter = 0; iter < 10; iter++) {
#endif
            for(int i = 0; i < M * N;i++) {
                Csgemm[i] = rand() % 25;
#ifdef LINUX
                C_cblas[i] = Csgemm[i];
#endif
            }
            status = hc.hcblas_sgemm(hcOrder, typeA, typeB, M, N, K, &alpha, Asgemm, lda, Bsgemm,ldb, &beta, Csgemm, ldc, aOffset, bOffset, cOffset);
#ifdef LINUX
            cblas_sgemm(order, Transa, Transb, M, N, K, alpha, Asgemm, lda, Bsgemm, ldb, beta, C_cblas, ldc);
            for(int i = 0 ; i < M * N ; i++) { 
                if( C_cblas[i] != (Csgemm[i])) {
                    ispassed = 0;
                    cout << " HCSGEMM["<<i<<"] = "<<Csgemm[i]<<" doesnot match with CBLASSGEMM["<<i<<"] =" << C_cblas[i] << endl;
                    break;
                }
                else
                   continue;
            }
            if(!ispassed) cout << "TEST FAILED" << endl; 
#else
            if(status) cout << "TEST FAILED" << endl; 
#endif
#ifdef PROFILE
           }
#endif
       }

/* Implementation type II - Inputs and Outputs are HC++ float array_view containers */

       else if(Imple_type ==2) {
            hc::array_view<float> A_mat(K * M, Asgemm);
            hc::array_view<float> B_mat(N * K, Bsgemm);
            hc::array_view<float> C_mat(M * N, Csgemm);
            for(int i = 0; i < M * K; i++) {
                A_mat[i] = rand() % 100;
                Asgemm[i] = A_mat[i];
            }
            for(int i = 0; i < K * N;i++) {
                B_mat[i] = rand() % 15;
                Bsgemm[i] = B_mat[i];
            }
#ifdef PROFILE
            for(int iter = 0; iter < 10; iter++) {
#endif
            for(int i = 0; i < M * N;i++) {
                C_mat[i] = rand() % 25;
                Csgemm[i] = C_mat[i];
#ifdef LINUX
                C_cblas[i] = C_mat[i];
#endif
            }
            status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat, ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
#ifdef LINUX
            cblas_sgemm(order, Transa, Transb, M, N, K, alpha, Asgemm, lda, Bsgemm, ldb, beta, C_cblas, ldc);
            for(int i = 0 ; i < M * N ; i++) {
                if( C_cblas[i] != (C_mat[i])) {
                    ispassed = 0;
                    cout << " HCSGEMM["<<i<<"] = "<<C_mat[i]<<" doesnot match with CBLASSGEMM["<<i<<"] =" << C_cblas[i] << endl;
                    break;
                }
                else
                   continue;
            }
            if(!ispassed) cout << "TEST FAILED" << endl; 
#else
            if(status) cout << "TEST FAILED" << endl; 
#endif
#ifdef PROFILE
           }
#endif
        }

/* Implementation type III - Inputs and Outputs are HC++ float array_view containers with batch processing */

       else if(Imple_type ==3) {
            float *Asgemm_batch = (float*) calloc(M * K, sizeof(float));
            float *Bsgemm_batch = (float*) calloc(K * N, sizeof(float));
            float *Csgemm_batch = (float*) calloc(M * N * batchSize, sizeof(float));
#ifdef LINUX
            float *CCblasbatch = (float*) calloc(M * N * batchSize, sizeof(float));
#endif
            hc::array_view<float> A_batch(K * M, Asgemm_batch);
            hc::array_view<float> B_batch(N * K, Bsgemm_batch);
            hc::array_view<float> C_batch(M * N * batchSize, Csgemm_batch);
            for(int i = 0; i < M * K; i++) {
                A_batch[i] = rand()%100;
                Asgemm_batch[i] = A_batch[i];
            }
            for(int i = 0; i < K * N;i++) {
                B_batch[i] = rand() % 15;
                Bsgemm_batch[i] = B_batch[i];
            }
#ifdef PROFILE
            for(int iter = 0; iter < 10; iter++) {
#endif
            for(int i = 0; i < M * N * batchSize;i++) {
                C_batch[i] = rand() % 25;
                Csgemm_batch[i] = C_batch[i];
#ifdef LINUX
                CCblasbatch[i] = Csgemm_batch[i];
#endif
            }
            status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch, ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
#ifdef LINUX
            for(int i = 0; i < batchSize; i++)
                cblas_sgemm( order, Transa, Transb, M, N, K, alpha, Asgemm_batch, lda, Bsgemm_batch, ldb, beta, CCblasbatch  + i * M * N ,ldc );
            for(int i = 0 ; i < M * N * batchSize; i++) {
                if( C_batch[i] != (CCblasbatch[i])) {
                    ispassed = 0;
                    cout << " HCSGEMM["<<i<<"] = "<<C_batch[i]<<" doesnot match with CBLASSGEMM["<<i<<"] =" << CCblasbatch[i] << endl;
                    break;
                }
                else
                   continue;
            }
            if(!ispassed) cout << "TEST FAILED" << endl; 
#else
            if(status) cout << "TEST FAILED" << endl;
#endif
#ifdef PROFILE
            }
#endif
        }

/* Implementation type IV - Inputs and Outputs are HC++ float array containers */

        else if(Imple_type == 4) {/* MULTIPLE GPU CALL */
            hc::array<float, 1> A_mat(K * M, Asgemm);
            hc::array<float, 1> B_mat(N * K, Bsgemm);
            hc::array<float, 1> C_mat(M * N, Csgemm);
            std::vector<float> HostA(M * K);
            std::vector<float> HostB(K * N);
            std::vector<float> HostC(M * N);
            for(int i = 0; i < M * K; i++) {
                HostA[i] = rand()%100;
                Asgemm[i] = HostA[i];
            }
            for(int i = 0; i < K * N;i++) {
                HostB[i] = rand() % 15;
                Bsgemm[i] = HostB[i];
            }
#ifdef PROFILE
            for(int iter = 0; iter < 10; iter++) {
#endif
            for(int i = 0; i < M * N;i++) {
            HostC[i] = rand() % 25;
#ifdef LINUX
            C_cblas[i] = HostC[i];
#endif
            Csgemm[i] = HostC[i];
            }
            hc::copy(begin(HostA), end(HostA), A_mat);
            hc::copy(begin(HostB), end(HostB), B_mat);
            hc::copy(begin(HostC), end(HostC), C_mat);
            status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat,ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
            hc::copy(C_mat, begin(HostC));
#ifdef LINUX
            cblas_sgemm( order, Transa, Transb, M, N, K, alpha, Asgemm, lda, Bsgemm, ldb, beta, C_cblas, ldc);
            for(int i = 0 ; i < M * N ; i++) { 
                if( C_cblas[i] != (HostC[i])) {
                    ispassed = 0;
                    cout << " HCSGEMM["<<i<<"] = "<<HostC[i]<<" doesnot match with CBLASSGEMM["<<i<<"] =" << C_cblas[i] << endl;
                    break;
                }
                else
                   continue;
            } 
            if(!ispassed) cout << "TEST FAILED" << endl; 
#else
           if(status) cout << "TEST FAILED" << endl; 
#endif
#ifdef PROFILE
            }
#endif
        }
    
/* Implementation type V - Inputs and Outputs are HC++ float array containers with batch processing */
 
        else {         
            float *Asgemm_batch = (float*) calloc(M * K, sizeof(float));
            float *Bsgemm_batch = (float*) calloc(K * N, sizeof(float));
            float *Csgemm_batch = (float*) calloc(M * N * batchSize, sizeof(float));
#ifdef LINUX
            float *CCblasbatch = (float*) calloc(M * N * batchSize, sizeof(float));                     
#endif
            hc::array<float> A_batch(K * M, Asgemm_batch);
            hc::array<float> B_batch(N * K, Bsgemm_batch);
            hc::array<float> C_batch(M * N * batchSize, Csgemm_batch);
            std::vector<float> HostA(M * K);
            std::vector<float> HostB(K * N);
            std::vector<float> HostC_batch(M * N * batchSize);
            for(int i = 0; i < M * K; i++) {
                HostA[i] = rand()%100;
                Asgemm_batch[i] = HostA[i];
            }
            for(int i = 0; i < K * N;i++) {
                HostB[i] = rand() % 15;
                Bsgemm_batch[i] = HostB[i];
            }
#ifdef PROFILE
            for(int iter = 0; iter < 10; iter++) {
#endif
            for(int i = 0; i < M * N * batchSize;i++) {
                HostC_batch[i] = rand() % 25;
                Csgemm_batch[i] = HostC_batch[i];
#ifdef LINUX
                CCblasbatch[i] = Csgemm_batch[i];
#endif
            } 
            hc::copy(begin(HostA), end(HostA), A_batch);
            hc::copy(begin(HostB), end(HostB), B_batch);
            hc::copy(begin(HostC_batch), end(HostC_batch), C_batch);
            status = hc.hcblas_sgemm(accl_view, hcOrder, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch,ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
            hc::copy(C_batch, begin(HostC_batch));         
#ifdef LINUX 
            for(int i = 0; i < batchSize; i++)
                cblas_sgemm( order, Transa, Transb, M, N, K, alpha, Asgemm_batch, lda, Bsgemm_batch, ldb, beta, CCblasbatch  + i * M * N ,ldc );

            for(int i = 0 ; i < M * N * batchSize; i++){ 
                if( HostC_batch[i] != (CCblasbatch[i])){
                    ispassed = 0;
                    cout << " HCSGEMM["<<i<<"] = "<<HostC_batch[i]<<" doesnot match with CBLASSGEMM["<<i<<"] =" << CCblasbatch[i] << endl;
                    break;
                }
                else
                   continue;
            }
            if(!ispassed) cout << "TEST FAILED" << endl; 
#else
           if(status) cout << "TEST FAILED" << endl; 
#endif
#ifdef PROFILE
    	    } 
#endif
       } 
    return 0;   
}
   
   
  
