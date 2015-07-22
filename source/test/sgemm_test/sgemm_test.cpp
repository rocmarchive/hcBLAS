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
    long A_batchOffset = 1;
    long B_batchOffset = 1;
    long C_batchOffset = 1;
    int batchSize =1;
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
    float *C = (float*) calloc(M * N, sizeof(float));
    Concurrency::array_view<float> A_mat(K * M, Asgemm);
    Concurrency::array_view<float> B_mat(N * K, Bsgemm);
    Concurrency::array_view<float> C_mat(M * N, Csgemm);
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
 
   for(int k = 0; k < 1; k++) {
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
            C[i] = C_mat[i];
        }
        if(Imple_type ==1)     /* SINGLE GPU CALL   */
            status = amp.ampblas_sgemm(typeA, typeB, M, N, K, &alpha, Asgemm, lda, Bsgemm,ldb, &beta, Csgemm, ldc, aOffset, bOffset, cOffset);
        else if(Imple_type ==2)/* MULTIPLE GPU CALL */
            status = amp.ampblas_sgemm(accl_view, typeA, typeB, M, N, K, alpha, A_mat, lda, B_mat,ldb, beta, C_mat, ldc, aOffset, bOffset, cOffset);
        else                   /* BATCH PROCESSING  */
            status = amp.ampblas_sgemm(accl_view, typeA, typeB, M, N, K, alpha, A_mat, lda, A_batchOffset, B_mat,ldb, B_batchOffset, beta, C_mat, ldc, C_batchOffset, aOffset, bOffset, cOffset, batchSize);
		
		
	cblas_sgemm( order, transa, transb, M, N, K, alpha, Asgemm, lda, Bsgemm, ldb, beta, C, ldc );
        for(int i = 0 ; i< M* N ; i++) 
            if( C_mat[i] != (Csgemm[i])){
                cout << "At k = "<< k <<" Csgemm["<<i<<"] = "<<C_mat[i]<<" doesnot match with C["<<i<<"] =" << C[i] << endl;
                break;
            }
            else
               continue;
    }

    if (status == 0 )
    cout << " Test Passed "<< endl;
    if(Imple_type ==1){
		free(Asgemm);
    	free(Bsgemm);
    	free(Csgemm);
    	free(C);
    }
    return 0;   
}
   
   
  
