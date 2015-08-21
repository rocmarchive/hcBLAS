#include <iostream>
#include "../../ampblaslib.h"
#include <cstdlib>
#include "cblas.h"

using namespace std;

int main(int argc,char* argv[])
{  
    /* AMPBLAS Implementation */
    Ampblaslibrary amp; 
    if (argc < 7){
        cout<<"No sufficient commandline arguments specified"<<"argc :"<<argc<<endl;
        return -1;
    }
    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int K = atoi(argv[3]);
    int isTransA = (atoi(argv[4]));
    int isTransB = (atoi(argv[5])); 
    int Imple_type = (atoi(argv[6])); 
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
    long A_batchOffset = 0;
    long B_batchOffset = 0;
    long C_batchOffset = M * N;
    int batchSize = 128;
    AMPBLAS_ORDER ampOrder = colMajor;
    AMPBLAS_TRANS typeA,typeB ;
    ampblasStatus status;
    
    /* CBLAS implementation */
    enum CBLAS_ORDER order;
    enum CBLAS_TRANSPOSE transa, transb;
    order = CblasColMajor;
    if((isTransA == 0 || isTransA == 1) && (isTransB == 0 || isTransB == 1)) {
        if( isTransA == 0){
            typeA = noTrans;
            transa = CblasNoTrans;
            (ampOrder)?(lda = M):(lda = K);
        }
        else{
            typeA = trans;
            transa = CblasTrans;
            (ampOrder)?(lda = K):(lda = M);
        }
        if( isTransB == 0){
            typeB = noTrans;
            transb = CblasNoTrans;
            (ampOrder)?(ldb = K):(ldb = N);
        }
        else{
            typeB = trans;
            transb = CblasTrans;
            (ampOrder)?(ldb = N):(ldb = K);
        }
    }
    else {
        cout<< "Invalid transpose type specified"<<endl;
        return -1;
    }

    if(ampOrder)
       ldc = M;
    else
       ldc = N;

    float *Asgemm = (float*) calloc(M * K, sizeof(float));
    float *Bsgemm = (float*) calloc(K * N, sizeof(float));
    float *Csgemm = (float*) calloc(M * N, sizeof(float));
    float *C_cblas = (float*) calloc(M * N, sizeof(float));
    Concurrency::array<float, 1> A_mat(K * M, Asgemm);
    Concurrency::array<float, 1> B_mat(N * K, Bsgemm);
    Concurrency::array<float, 1> C_mat(M * N, Csgemm);
    if(M > 3000 && N > 3000){
	batchSize = 25;
    }
    if(M > 9000 && N > 9000){
        batchSize = 1;
    }
   
    std::vector<float> HostA(M * K);
    std::vector<float> HostB(K * N);
    std::vector<float> HostC(M * N);
    std::vector<float> HostC_batch(M * N * batchSize);

    float *Asgemm_batch = (float*) calloc(M * K, sizeof(float));
    float *Bsgemm_batch = (float*) calloc(K * N, sizeof(float));
    float *Csgemm_batch = (float*) calloc(M * N * batchSize, sizeof(float));
    float *CCblasbatch = (float*) calloc(M * N * batchSize, sizeof(float));                     
    Concurrency::array<float> A_batch(K * M, Asgemm_batch);
    Concurrency::array<float> B_batch(N * K, Bsgemm_batch);
    Concurrency::array<float> C_batch(M * N * batchSize, Csgemm_batch);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view()); 
 
    
        for(int i = 0; i < M * K; i++){
            HostA[i] = rand()%100;
            Asgemm[i] = HostA[i];
        }
        for(int i = 0; i < K * N;i++){
            HostB[i] = rand() % 15;
            Bsgemm[i] = HostB[i];
        }
        for(int iter = 0; iter < 100; iter++) { 
        for(int i = 0; i < M * N;i++)  {
            HostC[i] = rand() % 25;
            C_cblas[i] = HostC[i];
            Csgemm[i] = HostC[i];
        }
       if(Imple_type ==1){    /* SINGLE GPU CALL   */
            status = amp.ampblas_sgemm(ampOrder, typeA, typeB, M, N, K, &alpha, Asgemm, lda, Bsgemm,ldb, &beta, Csgemm, ldc, aOffset, bOffset, cOffset);
            cblas_sgemm( order, transa, transb, M, N, K, alpha, Asgemm, lda, Bsgemm, ldb, beta, C_cblas, ldc );
            for(int i = 0 ; i < M * N ; i++){ 
                if( C_cblas[i] != (Csgemm[i])){
                    ispassed = 0;
                    cout << " AMPSGEMM["<<i<<"] = "<<Csgemm[i]<<" doesnot match with CBLASSGEMM["<<i<<"] =" << C_cblas[i] << endl;
                    break;
                }
                else
                   continue;
            }
            cout << (ispassed?"TEST PASSED":"TEST FAILED")<< endl;
            //free(Asgemm);
    	    //free(Bsgemm);
    	    //free(Csgemm);
    	    //free(C_cblas);
        }
        else if(Imple_type ==2){/* MULTIPLE GPU CALL */
            Concurrency::copy(begin(HostA), end(HostA), A_mat);
            Concurrency::copy(begin(HostB), end(HostB), B_mat);
            Concurrency::copy(begin(HostC), end(HostC), C_mat);
            status = amp.ampblas_sgemm(accl_view, ampOrder, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat,ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
            Concurrency::copy(C_mat, begin(HostC));
            cblas_sgemm( order, transa, transb, M, N, K, alpha, Asgemm, lda, Bsgemm, ldb, beta, C_cblas, ldc );
            for(int i = 0 ; i < M * N ; i++){ 
                if( C_cblas[i] != (HostC[i])){
                    ispassed = 0;
                    cout << " AMPSGEMM["<<i<<"] = "<<HostC[i]<<" doesnot match with CBLASSGEMM["<<i<<"] =" << C_cblas[i] << endl;
                    break;
                }
                else
                   continue;
            } 
       
            cout << (ispassed?"TEST PASSED":"TEST FAILED")<< endl;
 
        }
     
        else{         
           /* BATCH PROCESSING */
            
            for(int i = 0; i < M * K; i++){
                HostA[i] = rand()%100;
                Asgemm_batch[i] = HostA[i];
            }
            for(int i = 0; i < K * N;i++){
                HostB[i] = rand() % 15;
                Bsgemm_batch[i] = HostB[i];
            }
            for(int i = 0; i < M * N * batchSize;i++)  {
                HostC_batch[i] = rand() % 25;
                Csgemm_batch[i] = HostC_batch[i];
                CCblasbatch[i] = Csgemm_batch[i];
            } 
            Concurrency::copy(begin(HostA), end(HostA), A_batch);
            Concurrency::copy(begin(HostB), end(HostB), B_batch);
            Concurrency::copy(begin(HostC_batch), end(HostC_batch), C_batch);
            status = amp.ampblas_sgemm(accl_view, ampOrder, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch,ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
            Concurrency::copy(C_batch, begin(HostC_batch));          
            for(int i = 0; i < batchSize; i++)
                cblas_sgemm( order, transa, transb, M, N, K, alpha, Asgemm_batch, lda, Bsgemm_batch, ldb, beta, CCblasbatch  + i * M * N ,ldc );

            for(int i = 0 ; i < M * N * batchSize; i++){ 
                if( HostC_batch[i] != (CCblasbatch[i])){
                    ispassed = 0;
                    cout << " AMPSGEMM["<<i<<"] = "<<HostC_batch[i]<<" doesnot match with CBLASSGEMM["<<i<<"] =" << CCblasbatch[i] << endl;
                    break;
                }
                else
                   continue;
            }
            cout << (ispassed?"TEST PASSED":"TEST FAILED")<< endl;
    	}
       }
      
    return 0;   
}
   
   
  
