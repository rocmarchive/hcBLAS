#include <iostream>
#include "hcblaslib.h"
#include <cstdlib> 
#include "cblas.h"
using namespace std;
int main(int argc, char** argv)
{   
    /* HCBLAS implementation */
    Hcblaslibrary hc;
    if (argc < 3){
        cout<<"No sufficient commandline arguments specified"<<"argc :"<<argc<<endl;
        return -1;
    }
    int N = atoi(argv[1]);
    int Imple_type = atoi(argv[2]);
    int incX = 1;
    float dothcblas;
    long xOffset = 0;
    int incY = 1;
    long yOffset = 0;
    long X_batchOffset = N;
    long Y_batchOffset = N;
    int batchSize = 128;
    hcblasStatus status;
    if (N > 5000)
	batchSize = 50;
    /* CBLAS implementation */
    bool ispassed = 1;
    float  dotcblas = 0.0;
    float *dotcblastemp =(float*)calloc(batchSize, sizeof(float));
    /* CBLAS implementation */
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Y = (float*)calloc(leny, sizeof(float));
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    float *Ybatch = (float*)calloc(leny * batchSize, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type I - Inputs and Outputs are HCC float array containers */
    
    if (Imple_type == 1){
         hc::array<float> xView(lenx, X);
         hc::array<float> yView(leny, Y);
         std::vector<float> HostX(lenx);
         std::vector<float> HostY(leny);
         for(int i = 0;i < lenx;i++){
             HostX[i] = rand() % 10;
             X[i] = HostX[i];
         }
        for(int i = 0;i < leny;i++){
            HostY[i] = rand() % 15;
            Y[i] = HostY[i];
        }
        hc::copy(begin(HostX), end(HostX), xView);
        hc::copy(begin(HostY), end(HostY), yView);
        status = hc.hcblas_sdot(accl_view, N, xView, incX, xOffset, yView, incY, yOffset, dothcblas);
        dotcblas = cblas_sdot( N, X, incX, Y, incY);
        if (dothcblas != dotcblas){
            ispassed = 0;
            cout <<" HCSDOT " << dothcblas << " does not match with CBLASSDOT "<< dotcblas << endl;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl; 
     }

/* Implementation type II - Inputs and Outputs are HCC float array containers with batch processing */

    else{
        hc::array<float> xbatchView(lenx * batchSize, Xbatch);
        hc::array<float> ybatchView(leny * batchSize, Ybatch);
        std::vector<float> HostX_batch(lenx * batchSize);
        std::vector<float> HostY_batch(leny * batchSize);
        for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
            Xbatch[i] = HostX_batch[i];
         }
       for(int i = 0;i < leny * batchSize;i++){
            HostY_batch[i] =  rand() % 15;
            Ybatch[i] = HostY_batch[i];
         }
        hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        hc::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
        status= hc.hcblas_sdot(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, dothcblas, X_batchOffset, Y_batchOffset, batchSize);
        for(int i = 0; i < batchSize; i++){
        	dotcblastemp[i] = cblas_sdot( N, Xbatch + i * N, incX, Ybatch + i * N, incY);
                dotcblas += dotcblastemp[i];
        }
        if (dothcblas != dotcblas){
            ispassed = 0;
            cout <<" HCSDOT " << dothcblas << " does not match with CBLASSDOT "<< dotcblas << endl;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl; 
    }
    return 0;
}
