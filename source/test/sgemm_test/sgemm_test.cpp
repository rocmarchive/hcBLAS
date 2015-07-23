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
    long A_batchOffset = K * M;
    long B_batchOffset = N * K;
    long C_batchOffset = M * N;
    int batchSize =128;
    AMPBLAS_TRANS typeA,typeB ;
    ampblasStatus status;
    if((isTransA == 0 || isTransA == 1) && (isTransB == 0 || isTransB == 1)){ 
        if( isTransA == 0) typeA = noTrans;
        else typeA = trans;
        if( isTransB == 0) typeB = noTrans;
        else typeB = trans;
    }
    else{
        cout<< "Invalid transpose type specified"<<endl;
        return -1;
    } 

    float *Asgemm = (float*) calloc(M * K, sizeof(float));
    float *Bsgemm = (float*) calloc(K * N, sizeof(float));
    float *Csgemm = (float*) calloc(M * N, sizeof(float));
    float *C_cblas = (float*) calloc(M * N, sizeof(float));
    Concurrency::array_view<float> A_mat(K * M, Asgemm);
    Concurrency::array_view<float> B_mat(N * K, Bsgemm);
    Concurrency::array_view<float> C_mat(M * N, Csgemm);
    
    float *Asgemm_batch = (float*) calloc(M * K * batchSize, sizeof(float));
    float *Bsgemm_batch = (float*) calloc(K * N * batchSize, sizeof(float));
    float *Csgemm_batch = (float*) calloc(M * N * batchSize, sizeof(float));
    float *CCblasbatch = (float*) calloc(M * N * batchSize, sizeof(float));                     
    Concurrency::array_view<float> A_batch(K * M * batchSize, Asgemm_batch);
    Concurrency::array_view<float> B_batch(N * K * batchSize, Bsgemm_batch);
    Concurrency::array_view<float> C_batch(M * N * batchSize, Csgemm_batch);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view()); 
 
   /* CBLAS implementation */
    enum CBLAS_ORDER order;
    enum CBLAS_TRANSPOSE transa, transb;  
    order = CblasColMajor;
    if(typeA == noTrans){
        transa = CblasNoTrans;
        lda = M;
    }
    else{
        transa = CblasTrans; 
        lda = K;
    }
    if(typeB == noTrans){
        transb = CblasNoTrans;
        ldb = K;
    }
    else{
        transb = CblasTrans;
        ldb = N;
    }
 
    {
        for(int i = 0; i < M * K; i++){
            A_mat[i] = rand()%100;
            Asgemm[i] = A_mat[i];
        }
        for(int i = 0; i < K * N;i++){
            B_mat[i] = rand() % 15;
            Bsgemm[i] = B_mat[i];
        }
        for(int i = 0; i < M * N;i++)  {
            C_mat[i] = rand() % 25;
            C_cblas[i] = C_mat[i];
            Csgemm[i] = C_mat[i];
        }
        if(Imple_type ==1){    /* SINGLE GPU CALL   */
            status = amp.ampblas_sgemm(typeA, typeB, M, N, K, &alpha, Asgemm, lda, Bsgemm,ldb, &beta, Csgemm, ldc, aOffset, bOffset, cOffset);
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
            free(Asgemm);
    	    free(Bsgemm);
    	    free(Csgemm);
    	    free(C_cblas);
        }
        else if(Imple_type ==2){/* MULTIPLE GPU CALL */
            status = amp.ampblas_sgemm(accl_view, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat,ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
            cblas_sgemm( order, transa, transb, M, N, K, alpha, Asgemm, lda, Bsgemm, ldb, beta, C_cblas, ldc );
            for(int i = 0 ; i < M * N ; i++){ 
                if( C_cblas[i] != (C_mat[i])){
                    ispassed = 0;
                    cout << " AMPSGEMM["<<i<<"] = "<<C_mat[i]<<" doesnot match with CBLASSGEMM["<<i<<"] =" << C_cblas[i] << endl;
                    break;
                }
                else
                   continue;
            }
            cout << (ispassed?"TEST PASSED":"TEST FAILED")<< endl;
 
        }
        else{         
           /* BATCH PROCESSING  */

            for(int i = 0; i < M * K * batchSize; i++){
                A_batch[i] = rand()%100;
                Asgemm_batch[i] = A_batch[i];
            }
            for(int i = 0; i < K * N * batchSize;i++){
                B_batch[i] = rand() % 15;
                Bsgemm_batch[i] = B_batch[i];
            }
            for(int i = 0; i < M * N * batchSize;i++)  {
                C_batch[i] = rand() % 25;
                Csgemm_batch[i] = C_batch[i];
                CCblasbatch[i] = Csgemm_batch[i];
            } 
            status = amp.ampblas_sgemm(accl_view, typeA, typeB, M, N, K, alpha, A_batch, lda, A_batchOffset, B_batch,ldb, B_batchOffset, beta, C_batch, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
            for(int i = 0; i < batchSize; i++)
                cblas_sgemm( order, transa, transb, M, N, K, alpha, Asgemm_batch + i * M * K , lda, Bsgemm_batch + i * K * N, ldb, beta, CCblasbatch  + i * M * N ,ldc );

            for(int i = 0 ; i < M * N * batchSize; i++){ 
                if( C_batch[i] != (CCblasbatch[i])){
                    ispassed = 0;
                    cout << " AMPSGEMM["<<i<<"] = "<<C_batch[i]<<" doesnot match with CBLASSGEMM["<<i<<"] =" << CCblasbatch[i] << endl;
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
   
   
  
