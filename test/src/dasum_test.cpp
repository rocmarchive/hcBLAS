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
    double asumhcblas;
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
    double asumcblas = 0.0;
    double *asumcblastemp = (double*)calloc(batchSize, sizeof(double));
#endif
    /* CBLAS implementation */
    long lenx = 1 + (N-1) * abs(incX);
    double *X = (double*)calloc(lenx, sizeof(double));
    double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type I - Inputs and Outputs are host double pointers */

    if (Imple_type == 1) {
        for(int i = 0;i < lenx;i++){
            X[i] = rand() % 10;
        }
	status = hc.hcblas_dasum(N, X, incX, xOffset, &asumhcblas);
#ifdef LINUX
        asumcblas = cblas_dasum( N, X, incX);
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

/* Implementation type II - Inputs and Outputs are HC++ double array_view containers */

    else if (Imple_type == 2) {
        hc::array_view<double> xView(lenx, X);
        for(int i = 0;i < lenx; i++) {
            xView[i] = rand() % 10;
            X[i] = xView[i];
        }
        status = hc.hcblas_dasum(accl_view, N, xView, incX, xOffset, asumhcblas);
#ifdef LINUX
        asumcblas = cblas_dasum( N, X, incX);
        if (asumhcblas != asumcblas) {
            ispassed = 0;
            cout <<" HCSASUM " << asumhcblas << " does not match with CBLASSASUM "<< asumcblas << endl;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
     }

/* Implementation type III - Inputs and Outputs are HC++ double array_view containers with batch processing */

     else if (Imple_type == 3) {
        hc::array_view<double> xbatchView(lenx * batchSize, Xbatch);
        for(int i = 0;i < lenx * batchSize;i++) {
            xbatchView[i] = rand() % 10;
            Xbatch[i] = xbatchView[i];
         }

        status= hc.hcblas_dasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
#ifdef LINUX
        for(int i = 0; i < batchSize; i++){
            asumcblastemp[i] = cblas_dasum( N, Xbatch + i * N, incX);
            asumcblas += asumcblastemp[i];
        }
        if (asumhcblas != asumcblas){
            ispassed = 0;
            cout <<" HCSASUM " << asumhcblas << " does not match with CBLASSASUM "<< asumcblas << endl;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; ;
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
    }

/* Implementation type IV - Inputs and Outputs are HC++ double array containers */
      
    else if (Imple_type == 4) {
        hc::array<double> xView(lenx, X);
        std::vector<double> HostX(lenx);
        for(int i = 0;i < lenx;i++){
            HostX[i] = rand() % 10;
            X[i] = HostX[i];
        }
        hc::copy(begin(HostX), end(HostX), xView);
        status = hc.hcblas_dasum(accl_view, N, xView, incX, xOffset, asumhcblas);
#ifdef LINUX
        asumcblas = cblas_dasum( N, X, incX);
        if (asumhcblas != asumcblas) {
            ispassed = 0;
            cout <<" HCSASUM " << asumhcblas << " does not match with CBLASSASUM "<< asumcblas << endl;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
     }

/* Implementation type V - Inputs and Outputs are HC++ double array containers with batch processing */

    else{
        hc::array<double> xbatchView(lenx * batchSize, Xbatch);
        std::vector<double> HostX_batch(lenx * batchSize);
        for(int i = 0;i < lenx * batchSize;i++) {
            HostX_batch[i] = rand() % 10;
            Xbatch[i] = HostX_batch[i];
         }
        hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        status= hc.hcblas_dasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
#ifdef LINUX
        for(int i = 0; i < batchSize; i++) {
        	asumcblastemp[i] = cblas_dasum( N, Xbatch + i * N, incX);
                asumcblas += asumcblastemp[i];
        }
        if (asumhcblas != asumcblas) {
            ispassed = 0;
            cout <<" HCSASUM " << asumhcblas << " does not match with CBLASSASUM "<< asumcblas << endl;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; ;
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
    }
    return 0;
}
