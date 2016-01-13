#include <iostream>
#include "hcblaslib.h"
#include <cstdlib> 
#include <amp_short_vectors.h>
#include <cblas.h>
#include <unistd.h>
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
    hcblasTranspose typeA;
    hcblasOrder hcOrder = ColMajor;
    if(isTransA == 0){
        row = N;
        col = M;
        lda = M;
        typeA = NoTrans;
    }
    else{
        row = M;
        col = N;
        lda = N;
        typeA = Trans;
    }
    /* CBLAS Implementation */
    enum CBLAS_ORDER order;
    enum CBLAS_TRANSPOSE transa;
    order = CblasColMajor;
    transa = (typeA == NoTrans)? CblasNoTrans : CblasTrans;
    float *ycblas = (float *)calloc( col , sizeof(float));
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    long X_batchOffset = row;
    long Y_batchOffset = col;
    long A_batchOffset = row * col;
    float *xSgemv = (float*)calloc( lenx , sizeof(float));
    float *ySgemv = (float*)calloc( leny , sizeof(float));
    float *ASgemv = (float *)calloc( lenx * leny , sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type I - Inputs and Outputs are host float pointers */

    if(Imple_type ==1) {
        for(int i = 0;i < lenx;i++) {
            xSgemv[i] = rand() % 10;
        }
        for(int i = 0;i< lenx * leny;i++) {
            ASgemv[i] = rand() % 25;
        }
#ifdef PROFILE
        for(int iter=0; iter<10; iter++) {
#endif
        for(int i = 0;i < leny;i++) {
            ySgemv[i] = rand() % 15;
            ycblas[i] = ySgemv[i];
        }
        status =  hc.hcblas_sgemv(hcOrder, typeA, M, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, incY);
        lda = (hcOrder)? M : N;
        cblas_sgemv( order, transa, M, N, alpha, ASgemv, lda , xSgemv, incX, beta, ycblas, incY );
        for(int i =0; i < leny; i ++){
            if (ySgemv[i] != ycblas[i]){
                ispassed = 0;
                cout <<" HCSGEMV[" << i<< "] " << ySgemv[i] << " does not match with CBLASSGEMV[" << i <<"] "<< ycblas[i] << endl;
                break;
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

/* Implementation type II - Inputs and Outputs are HC++ float array_view containers */

    else if(Imple_type ==2) {
        hc::array_view<float> xView(lenx, xSgemv);
        hc::array_view<float> yView(leny, ySgemv);
        hc::array_view<float> aMat(M * N, ASgemv);
        for(int i = 0;i < lenx;i++) {
           xView[i] = rand() % 10;
           xSgemv[i] = xView[i];
        }
        for(int i = 0;i< lenx * leny;i++) {
           aMat[i] = rand() % 25;
           ASgemv[i] = aMat[i];
        }
#ifdef PROFILE
        for(int iter = 0; iter < 10; iter++) {
#endif
        for(int i = 0;i < leny;i++) {
           yView[i] = rand() % 15;
           ySgemv[i]= yView[i];
           ycblas[i] = ySgemv[i];
        }
        status =  hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
        if(hcOrder)
                lda = M;
        else
                lda = N;
        cblas_sgemv( order, transa, M, N, alpha, ASgemv, lda , xSgemv, incX, beta, ycblas, incY );
        for(int i =0; i < leny; i ++){
            if (yView[i] != ycblas[i]){
                ispassed = 0;
                cout <<" HCSGEMV[" << i<< "] " << yView[i] << " does not match with CBLASSGEMV[" << i <<"] "<< ycblas[i] << endl;
                break;
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

/* Implementation type III - Inputs and Outputs are HC++ float array_view containers with batch processing */

    else if(Imple_type ==3) {
        float *xSgemvbatch = (float*)calloc( lenx * batchSize, sizeof(float));
        float *ySgemvbatch = (float*)calloc( leny * batchSize, sizeof(float));
        float *ASgemvbatch = (float *)calloc( row * col * batchSize, sizeof(float));
        float *ycblasbatch = (float *)calloc( col * batchSize, sizeof(float));
        hc::array_view<float> xbatchView(lenx * batchSize, xSgemvbatch);
        hc::array_view<float> ybatchView(leny * batchSize, ySgemvbatch);
        hc::array_view<float> abatchMat(M * N * batchSize, ASgemvbatch);
        for(int i = 0;i < lenx * batchSize;i++) {
            xbatchView[i] = rand() % 10;
            xSgemvbatch[i] = xbatchView[i];
        }
        for(int i = 0;i< lenx * leny * batchSize;i++){
            abatchMat[i] = rand() % 25;
            ASgemvbatch[i] = abatchMat[i];
        }
#ifdef PROFILE
        for(int iter=0; iter<10; iter++) {
#endif
        for(int i = 0;i < leny * batchSize;i++) {
            ybatchView[i] = rand() % 15;
            ySgemvbatch[i]= ybatchView[i];
            ycblasbatch[i] = ySgemvbatch[i];
        }

        status =  hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
        lda = (hcOrder)? M : N;
        for(int i =0 ; i < batchSize; i++)
            cblas_sgemv( order, transa, M, N, alpha, ASgemvbatch + i * M * N, lda , xSgemvbatch + i * row, incX, beta, ycblasbatch + i * col, incY );
        for(int i =0; i < leny * batchSize; i ++){
            if (ybatchView[i] != ycblasbatch[i]){
                ispassed = 0;
                cout <<" HCSGEMV[" << i<< "] " << ybatchView[i] << " does not match with CBLASSGEMV[" << i <<"] "<< ycblasbatch[i] << endl;
                break;
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

/* Implementation type IV - Inputs and Outputs are HC++ float array containers */

    else if(Imple_type ==4) {
        hc::array<float> xView(lenx, xSgemv);
        hc::array<float> yView(leny, ySgemv);
        hc::array<float> aMat(lenx * leny, ASgemv);
        std::vector<float> HostX(lenx);
        std::vector<float> HostY(leny);
        std::vector<float> HostA(lenx * leny);
        for(int i = 0;i < lenx;i++) {
           HostX[i] = rand() % 10;
           xSgemv[i] = HostX[i];
        }
        for(int i = 0;i< lenx * leny;i++) {
           HostA[i] = rand() % 25;
           ASgemv[i] = HostA[i];
        }
#ifdef PROFILE
        for(int iter=0; iter<10; iter++) {
#endif
        for(int i = 0;i < leny;i++) {
            HostY[i] = rand() % 15;
            ySgemv[i]= HostY[i];
            ycblas[i] = ySgemv[i];
        }
        hc::copy(begin(HostX), end(HostX), xView);
        hc::copy(begin(HostY), end(HostY), yView);
        hc::copy(begin(HostA), end(HostA), aMat);
        status =  hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
        hc::copy(yView, begin(HostY));
        lda = (hcOrder)? M: N;
        cblas_sgemv( order, transa, M, N, alpha, ASgemv, lda , xSgemv, incX, beta, ycblas, incY );
        for(int i =0; i < leny; i ++){
            if (HostY[i] != ycblas[i]){
                ispassed = 0;
                cout <<" HCSGEMV[" << i<< "] " << HostY[i] << " does not match with CBLASSGEMV[" << i <<"] "<< ycblas[i] << endl;
                break;
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

/* Implementation type V - Inputs and Outputs are HC++ float array containers with batch processing */

    else{
        float *xSgemvbatch = (float*)calloc( lenx * batchSize, sizeof(float));
        float *ySgemvbatch = (float*)calloc( leny * batchSize, sizeof(float));
        float *ASgemvbatch = (float *)calloc( lenx * leny * batchSize, sizeof(float));
        float *ycblasbatch = (float *)calloc( leny * batchSize, sizeof(float));
        hc::array<float> xbatchView(lenx * batchSize, xSgemvbatch);
        hc::array<float> ybatchView(leny * batchSize, ySgemvbatch);
        hc::array<float> abatchMat(lenx * leny * batchSize, ASgemvbatch);
        std::vector<float> HostX_batch(lenx * batchSize);
        std::vector<float> HostY_batch(leny * batchSize);
        std::vector<float> HostA_batch(lenx * leny * batchSize);
        for(int i = 0;i < lenx * batchSize;i++) {
            HostX_batch[i] = rand() % 10;
            xSgemvbatch[i] = HostX_batch[i];
        }
        for(int i = 0;i< lenx * leny * batchSize;i++) {
            HostA_batch[i] = rand() % 25;
            ASgemvbatch[i] = HostA_batch[i]; 
        }
#ifdef PROFILE
        for(int iter=0; iter<10; iter++) {
#endif
        for(int i = 0;i < leny * batchSize;i++) {
            HostY_batch[i] = rand() % 15;
            ySgemvbatch[i]= HostY_batch[i];
            ycblasbatch[i] = ySgemvbatch[i];
        }
        hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        hc::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
        hc::copy(begin(HostA_batch), end(HostA_batch), abatchMat);
        status =  hc.hcblas_sgemv(accl_view, hcOrder, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
        hc::copy(ybatchView, begin(HostY_batch));
        lda = (hcOrder)? M : N;
        for(int i =0 ; i < batchSize; i++)
            cblas_sgemv( order, transa, M, N, alpha, ASgemvbatch + i * M * N, lda , xSgemvbatch + i * row, incX, beta, ycblasbatch + i * col, incY );
        for(int i =0; i < leny * batchSize; i ++){
            if (HostY_batch[i] != ycblasbatch[i]){
                ispassed = 0;
                cout <<" HCSGEMV[" << i<< "] " << HostY_batch[i] << " does not match with CBLASSGEMV[" << i <<"] "<< ycblasbatch[i] << endl;
                break;
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
