#include <iostream>
#include "../hcblas.h"
#include <cstdlib> 
#include <amp_short_vectors.h>
#ifdef LINUX
#include<cblas.h>
#include<unistd.h>
#else
#include<io.h>
#endif
using namespace std;
int main(int argc, char** argv)
{
    /*  HCBLAS Implementation */
    Hcblaslibrary hc;
    if (argc < 5){
        cout<<"No sufficient commandline arguments specified"<<"argc :"<<argc<<endl;
        return -1;
    }
    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int isTransA = (atoi(argv[3]));
    int Imple_type = (atoi(argv[4]));
    int row, col;
    bool ispassed = 1;
    float alpha = 1;
    float beta = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    int batchSize = 128;
    long lenx,  leny;
    hcblasStatus status;
    HCBLAS_TRANS typeA;
    HCBLAS_ORDER hcOrder = colMajor;
    if(isTransA == 0){
        row = N;
        col = M;
        lda = M;
        typeA = noTrans;
    }
    else{
        row = M;
        col = N;
        lda = N;
        typeA = trans;
    }
#ifdef LINUX 
    /* CBLAS Implementation */
    enum CBLAS_ORDER order;
    enum CBLAS_TRANSPOSE transa;
    order = CblasColMajor;
    transa = (typeA == noTrans)? CblasNoTrans : CblasTrans;
    float *ycblas = (float *)calloc( col , sizeof(float));
#endif   
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    long X_batchOffset = row;
    long Y_batchOffset = col;
    long A_batchOffset = row * col;
    float *xSgemv = (float*)calloc( lenx , sizeof(float));
    float *ySgemv = (float*)calloc( leny , sizeof(float));
    float *ASgemv = (float *)calloc( row * col , sizeof(float));
    Concurrency::array<float> xView(lenx, xSgemv);
    Concurrency::array<float> yView(leny, ySgemv);       
    Concurrency::array<float> aMat(M * N, ASgemv);
    std::vector<float> HostX(lenx);
    std::vector<float> HostY(leny);
    std::vector<float> HostA(M * N);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    for(int i = 0;i < row;i++) {
        HostX[i] = rand() % 10;
        xSgemv[i] = HostX[i];
    }
    for(int i = 0;i< row * col;i++) {
        HostA[i] = rand() % 25;
        ASgemv[i] = HostA[i];
    }
    for(int iter=0; iter<10; iter++) {
        for(int i = 0;i < col;i++) {
            HostY[i] = rand() % 15;
            ySgemv[i]= HostY[i];
#ifdef LINUX
            ycblas[i] = ySgemv[i];
#endif
    }
    if(Imple_type ==1) {
        status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, incY);
#ifdef LINUX
        lda = (hcOrder)? M : N;
        cblas_sgemv( order, transa, M, N, alpha, ASgemv, lda , xSgemv, incX, beta, ycblas, incY );
        for(int i =0; i < col; i ++){
            if (ySgemv[i] != ycblas[i]){
                ispassed = 0;
                cout <<" HCSGEMV[" << i<< "] " << ySgemv[i] << " does not match with CBLASSGEMV[" << i <<"] "<< ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED": "TEST FAILED") << endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
    }
    else if(Imple_type ==2) {
        Concurrency::copy(begin(HostX), end(HostX), xView);
        Concurrency::copy(begin(HostY), end(HostY), yView);
        Concurrency::copy(begin(HostA), end(HostA), aMat);
        status =  hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
        Concurrency::copy(yView, begin(HostY));
#ifdef LINUX
        lda = (hcOrder)? M: N;
        cblas_sgemv( order, transa, M, N, alpha, ASgemv, lda , xSgemv, incX, beta, ycblas, incY );
        for(int i =0; i < col; i ++){
            if (HostY[i] != ycblas[i]){
                ispassed = 0;
                cout <<" HCSGEMV[" << i<< "] " << HostY[i] << " does not match with CBLASSGEMV[" << i <<"] "<< ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED": "TEST FAILED") << endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
    }
    else{
        float *xSgemvbatch = (float*)calloc( lenx * batchSize, sizeof(float));
        float *ySgemvbatch = (float*)calloc( leny * batchSize, sizeof(float));
        float *ASgemvbatch = (float *)calloc( row * col * batchSize, sizeof(float));
#ifdef LINUX
        float *ycblasbatch = (float *)calloc( col * batchSize, sizeof(float));
#endif
        Concurrency::array<float> xbatchView(lenx * batchSize, xSgemvbatch);
        Concurrency::array<float> ybatchView(leny * batchSize, ySgemvbatch);
        Concurrency::array<float> abatchMat(M * N * batchSize, ASgemvbatch);
        std::vector<float> HostX_batch(lenx * batchSize);
        std::vector<float> HostY_batch(leny * batchSize);
        std::vector<float> HostA_batch(M * N * batchSize);
        for(int i = 0;i < row * batchSize;i++) {
            HostX_batch[i] = rand() % 10;
            xSgemvbatch[i] = HostX_batch[i];
        }
        for(int i = 0;i < col * batchSize;i++) {
            HostY_batch[i] = rand() % 15;
            ySgemvbatch[i]= HostY_batch[i];
#ifdef LINUX
            ycblasbatch[i] = ySgemvbatch[i];
#endif
        }
        for(int i = 0;i< row * col * batchSize;i++) {
            HostA_batch[i] = rand() % 25;
            ASgemvbatch[i] = HostA_batch[i]; 
        }
        Concurrency::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        Concurrency::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
        Concurrency::copy(begin(HostA_batch), end(HostA_batch), abatchMat);
        status =  hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
        Concurrency::copy(ybatchView, begin(HostY_batch));
#ifdef LINUX
        lda = (hcOrder)? M : N;
        for(int i =0 ; i < batchSize; i++)
            cblas_sgemv( order, transa, M, N, alpha, ASgemvbatch + i * M * N, lda , xSgemvbatch + i * row, incX, beta, ycblasbatch + i * col, incY );
        for(int i =0; i < col * batchSize; i ++){
            if (HostY_batch[i] != ycblasbatch[i]){
                ispassed = 0;
                cout <<" HCSGEMV[" << i<< "] " << HostY_batch[i] << " does not match with CBLASSGEMV[" << i <<"] "<< ycblasbatch[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED": "TEST FAILED") << endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
    }
   }
    return 0;

}
