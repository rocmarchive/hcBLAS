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
    const float alpha = 1;
    int incX = 1;
    long xOffset = 0;
    hcblasStatus status;
    int batchSize = 128;
    long X_batchOffset = N; 
    long lenx = 1 + (N-1) * abs(incX);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    /* CBLAS implementation */
    bool ispassed = 1;
    float *Xcblas = (float*)calloc(lenx, sizeof(float));
    float *Xcblasbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type I - Inputs and Outputs are HCC float array containers */
    
    if (Imple_type == 1) {
        hc::array<float> xView(lenx, X);
        std::vector<float> HostX(lenx);
        for(int i = 0;i < lenx;i++){
            HostX[i] = rand() % 10;
            Xcblas[i] = HostX[i];
        }
        hc::copy(begin(HostX), end(HostX), xView);
        status = hc.hcblas_sscal(accl_view, N, alpha, xView, incX, xOffset);
        hc::copy(xView, begin(HostX));  
        cblas_sscal( N, alpha, Xcblas, incX );
        for(int i = 0; i < lenx ; i++){
            if (HostX[i] != Xcblas[i]){
                ispassed = 0;
                cout <<" HCSSCAL[" << i<< "] " << HostX[i] << " does not match with CBLASSSCAL[" << i <<"] "<< Xcblas[i] << endl;
                break;
            }
            else
                continue;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl; 
   }

/* Implementation type II - Inputs and Outputs are HCC float array containers with batch processing */

    else{
        hc::array<float> xbatchView(lenx * batchSize, Xbatch);
        std::vector<float> HostX_batch(lenx * batchSize); 
        for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
            Xcblasbatch[i] =  HostX_batch[i];
         }
        hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        status= hc.hcblas_sscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
        hc::copy(xbatchView, begin(HostX_batch));  
        for(int i = 0; i < batchSize; i++)
        	cblas_sscal( N, alpha, Xcblasbatch + i * N, incX);
        for(int i =0; i < lenx * batchSize; i ++){
            if (HostX_batch[i] != Xcblasbatch[i]){
                ispassed = 0;
                cout <<" HCSSCAL[" << i<< "] " << HostX_batch[i] << " does not match with CBLASSSCAL[" << i <<"] "<< Xcblasbatch[i] << endl;
                break;
            }
            else 
              continue;  
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl; 
    }
    return 0;
}
