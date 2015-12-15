#include <iostream>
#include "hcblas.h"
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
    float asumhcblas;
    int incX = 1;
    long xOffset = 0;
    hcblasStatus status;
    int batchSize = 128;
    long X_batchOffset = N;
    if(N > 10000)
	batchSize = 50;
#ifdef LINUX
    /* CBLAS implementation */
    bool ispassed = 1;
    float asumcblas = 0.0;
    float *asumcblastemp = (float*)calloc(batchSize, sizeof(float));
#endif
    /* CBLAS implementation */
    long lenx = 1 + (N-1) * abs(incX);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type I - Inputs and Outputs are host float pointers */

    if (Imple_type == 1) {
        for(int i = 0;i < lenx;i++){
            X[i] = rand() % 10;
        }
	status = hc.hcblas_sasum(N, X, incX, xOffset, &asumhcblas);
#ifdef LINUX
        asumcblas = cblas_sasum( N, X, incX);
        if (asumhcblas != asumcblas) {
            ispassed = 0;
            cout <<" HCSASUM " << asumhcblas << " does not match with CBLASSASUM "<< asumcblas << endl;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
        free(X);
    }

/* Implementation type II - Inputs and Outputs are HC++ float array_view containers */

    else if (Imple_type == 2) {
        Concurrency::array_view<float> xView(lenx, X);
        for(int i = 0;i < lenx; i++) {
            xView[i] = rand() % 10;
            X[i] = xView[i];
        }
        status = hc.hcblas_sasum(accl_view, N, xView, incX, xOffset, asumhcblas);
#ifdef LINUX
        asumcblas = cblas_sasum( N, X, incX);
        if (asumhcblas != asumcblas) {
            ispassed = 0;
            cout <<" HCSASUM " << asumhcblas << " does not match with CBLASSASUM "<< asumcblas << endl;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
     }

/* Implementation type III - Inputs and Outputs are HC++ float array_view containers with batch processing */

     else if (Imple_type == 3) {
        Concurrency::array_view<float> xbatchView(lenx * batchSize, Xbatch);
        for(int i = 0;i < lenx * batchSize;i++) {
            xbatchView[i] = rand() % 10;
            Xbatch[i] = xbatchView[i];
         }

        status= hc.hcblas_sasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
#ifdef LINUX
        for(int i = 0; i < batchSize; i++){
            asumcblastemp[i] = cblas_sasum( N, Xbatch + i * N, incX);
            asumcblas += asumcblastemp[i];
        }
        if (asumhcblas != asumcblas){
            ispassed = 0;
            cout <<" HCSASUM " << asumhcblas << " does not match with CBLASSASUM "<< asumcblas << endl;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
    }

/* Implementation type IV - Inputs and Outputs are HC++ float array containers */
      
    else if (Imple_type == 4) {
        Concurrency::array<float> xView(lenx, X);
        std::vector<float> HostX(lenx);
        for(int i = 0;i < lenx;i++){
            HostX[i] = rand() % 10;
            X[i] = HostX[i];
        }
        Concurrency::copy(begin(HostX), end(HostX), xView);
        status = hc.hcblas_sasum(accl_view, N, xView, incX, xOffset, asumhcblas);
#ifdef LINUX
        asumcblas = cblas_sasum( N, X, incX);
        if (asumhcblas != asumcblas) {
            ispassed = 0;
            cout <<" HCSASUM " << asumhcblas << " does not match with CBLASSASUM "<< asumcblas << endl;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
     }

/* Implementation type V - Inputs and Outputs are HC++ float array containers with batch processing */

    else{
        Concurrency::array<float> xbatchView(lenx * batchSize, Xbatch);
        std::vector<float> HostX_batch(lenx * batchSize);
        for(int i = 0;i < lenx * batchSize;i++) {
            HostX_batch[i] = rand() % 10;
            Xbatch[i] = HostX_batch[i];
         }
        Concurrency::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        status= hc.hcblas_sasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
#ifdef LINUX
        for(int i = 0; i < batchSize; i++) {
        	asumcblastemp[i] = cblas_sasum( N, Xbatch + i * N, incX);
                asumcblas += asumcblastemp[i];
        }
        if (asumhcblas != asumcblas) {
            ispassed = 0;
            cout <<" HCSASUM " << asumhcblas << " does not match with CBLASSASUM "<< asumcblas << endl;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
    }
    return 0;
}
