#include <iostream>
#include "../hcblas.h"
#include <cstdlib> 
#ifdef LINUX 
#include "cblas.h"
#endif
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
#ifdef LINUX 
    /* CBLAS implementation */
    bool ispassed = 1;
    float *Xcblas = (float*)calloc(lenx, sizeof(float));
    float *Xcblasbatch = (float*)calloc(lenx * batchSize, sizeof(float));
#endif
    Concurrency::array<float> xView(lenx, X);
    Concurrency::array<float> xbatchView(lenx * batchSize, Xbatch);
    std::vector<float> HostX(lenx);
    std::vector<float> HostX_batch(lenx * batchSize);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    for(int i = 0;i < lenx;i++){
        HostX[i] = rand() % 10;
        X[i] = HostX[i];
#ifdef LINUX
        Xcblas[i] = X[i];
#endif
    }
    if (Imple_type == 1) {
	status = hc.hcblas_sscal(N, &alpha, X, incX, xOffset);
#ifdef LINUX
        cblas_sscal( N, alpha, Xcblas, incX);
        for(int i = 0; i < lenx ; i++){
            if (X[i] != Xcblas[i]){
                ispassed = 0;
                cout <<" HCSSCAL[" << i<< "] " << X[i] << " does not match with CBLASSSCAL[" << i <<"] "<< Xcblas[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
        free(Xcblas);
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
        free(X);  
    }
    
    else if (Imple_type ==2) {
        Concurrency::copy(begin(HostX), end(HostX), xView);
        status = hc.hcblas_sscal(accl_view, N, alpha, xView, incX, xOffset);
#ifdef LINUX
        Concurrency::copy(xView, begin(HostX));  
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
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
     }

    else{ 
        for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
#ifdef LINUX
            Xcblasbatch[i] =  HostX_batch[i];
#endif
         }
        Concurrency::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        status= hc.hcblas_sscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
        Concurrency::copy(xbatchView, begin(HostX_batch));  
#ifdef LINUX
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
        cout << (ispassed? "TEST PASSED":"TEST FAILED") << endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
    }
    return 0;
}
