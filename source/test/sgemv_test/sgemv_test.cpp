#include <iostream>
#include "../../ampblaslib.h"
#include <cstdlib> 
#include "cblas.h"
#include <unistd.h>
#include <amp_short_vectors.h>
using namespace std;
int main(int argc, char** argv)
{
    /*  AMPBLAS Implementation */
    Ampblaslibrary amp;
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
    ampblasStatus status;
    AMPBLAS_TRANS typeA;

    /* clBLAS Implementation */
    enum CBLAS_ORDER order;
    enum CBLAS_TRANSPOSE transa;
    order = CblasColMajor;
    if(isTransA == 0){
        row = N;
        col = M;
        lda = M;
        transa = CblasNoTrans;
        typeA = noTrans;
    }
    else{
        row = M;
        col = N;
        lda = N;
        transa = CblasTrans;
        typeA = trans;
    }
   
    lenx = 1 + (row - 1) * abs(incX);
    leny = 1 + (col - 1) * abs(incY);
    long X_batchOffset = row;
    long Y_batchOffset = col;
    long A_batchOffset = row * col;

    float *xSgemv = (float*)calloc( lenx , sizeof(float));
    float *ySgemv = (float*)calloc( leny , sizeof(float));
    float *ASgemv = (float *)calloc( row * col , sizeof(float));
    float *ycblas = (float *)calloc( col , sizeof(float));
    Concurrency::array_view<float> xView(lenx, xSgemv);
    Concurrency::array_view<float> yView(leny, ySgemv);       
    Concurrency::array_view<float> aMat(M * N, ASgemv);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    float *xSgemvbatch = (float*)calloc( lenx * batchSize, sizeof(float));
    float *ySgemvbatch = (float*)calloc( leny * batchSize, sizeof(float));
    float *ASgemvbatch = (float *)calloc( row * col * batchSize, sizeof(float));
    float *ycblasbatch = (float *)calloc( col * batchSize, sizeof(float));
    Concurrency::array_view<float> xbatchView(lenx * batchSize, xSgemvbatch);
    Concurrency::array_view<float> ybatchView(leny * batchSize, ySgemvbatch);
    Concurrency::array_view<float> abatchMat(M * N * batchSize, ASgemvbatch);
  
 {
    for(int i = 0;i < row;i++){
        xView[i] = rand() % 10;
        xSgemv[i] = xView[i];}
    for(int i = 0;i < col;i++){
        yView[i] = rand() % 15;
        ySgemv[i]= yView[i];
        ycblas[i] = ySgemv[i];}
    for(int i = 0;i< row * col;i++){
        aMat[i] = rand() % 25;
        ASgemv[i] = aMat[i];}

    if(Imple_type ==1){
        status =  amp.ampblas_sgemv(typeA, M, N, &alpha, ASgemv, aOffset, lda, xSgemv, xOffset, incX, &beta, ySgemv, yOffset, incY);
        lda = M;
        cblas_sgemv( order, transa, M, N, alpha, ASgemv, lda , xSgemv, incX, beta, ycblas, incY );
        for(int i =0; i < col; i ++){
            if (ySgemv[i] != ycblas[i]){
                ispassed = 0;
                cout <<" AMPSGEMV[" << i<< "] " << ySgemv[i] << " does not match with CBLASSGEMV[" << i <<"] "<< ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED": "TEST FAILED") << endl;
        free(xSgemv);
        free(ySgemv);
        free(ycblas);
        free(ASgemv);
    }
    else if(Imple_type ==2){
        status =  amp.ampblas_sgemv(accl_view, typeA, M, N, alpha, aMat, aOffset, lda, xView, xOffset, incX, beta, yView, yOffset, incY);
        lda = M;
        cblas_sgemv( order, transa, M, N, alpha, ASgemv, lda , xSgemv, incX, beta, ycblas, incY );
        for(int i =0; i < col; i ++){
            if (yView[i] != ycblas[i]){
                ispassed = 0;
                cout <<" AMPSGEMV[" << i<< "] " << yView[i] << " does not match with CBLASSGEMV[" << i <<"] "<< ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED": "TEST FAILED") << endl;
    }
    else{
        for(int i = 0;i < row * batchSize;i++){
            xbatchView[i] = rand() % 10;
            xSgemvbatch[i] = xbatchView[i];}
        for(int i = 0;i < col * batchSize;i++){
            ybatchView[i] = rand() % 15;
            ySgemvbatch[i]= ybatchView[i];
            ycblasbatch[i] = ySgemvbatch[i];}
        for(int i = 0;i< row * col * batchSize;i++){
            abatchMat[i] = rand() % 25;
            ASgemvbatch[i] = abatchMat[i];}

        status =  amp.ampblas_sgemv(accl_view, typeA, M, N, alpha, abatchMat, aOffset, A_batchOffset, lda, xbatchView, xOffset, X_batchOffset, incX, beta, ybatchView, yOffset, Y_batchOffset, incY, batchSize);
        lda = M;
        for(int i =0 ; i < batchSize; i++)
            cblas_sgemv( order, transa, M, N, alpha, ASgemvbatch + i * M * N, lda , xSgemvbatch + i * row, incX, beta, ycblasbatch + i * col, incY );
        for(int i =0; i < col * batchSize; i ++){
            if (ybatchView[i] != ycblasbatch[i]){
                ispassed = 0;
                cout <<" AMPSGEMV[" << i<< "] " << ybatchView[i] << " does not match with CBLASSGEMV[" << i <<"] "<< ycblasbatch[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED": "TEST FAILED") << endl;
    }
   }
    return 0;

}
