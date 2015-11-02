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
    Concurrency::array<float> xView(lenx, X);
    Concurrency::array<float> xbatchView(lenx * batchSize, Xbatch);
    std::vector<float> HostX(lenx);
    std::vector<float> HostX_batch(lenx * batchSize);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    for(int i = 0;i < N;i++){
        HostX[i] = rand() % 10;
        X[i] = HostX[i];
    }
    if (Imple_type == 1) {
	status = hc.hcblas_sasum(N, X, incX, xOffset, &asumhcblas);
#ifdef LINUX
        asumcblas = cblas_sasum( N, X, incX);
        for(int i = 0; i < N ; i++) {
            if (asumhcblas != asumcblas) {
                ispassed = 0;
                cout <<" HCSASUM[" << i<< "] " << asumhcblas << " does not match with CBLASSASUM[" << i <<"] "<< asumcblas << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
        free(X);
    }
      
    else if (Imple_type ==2) {
        Concurrency::copy(begin(HostX), end(HostX), xView);
        status = hc.hcblas_sasum(accl_view, N, xView, incX, xOffset, asumhcblas);
#ifdef LINUX
        asumcblas = cblas_sasum( N, X, incX);
        for(int i = 0; i < N ; i++) {
            if (asumhcblas != asumcblas) {
                ispassed = 0;
                cout <<" HCSASUM[" << i<< "] " << asumhcblas << " does not match with CBLASSASUM[" << i <<"] "<< asumcblas << endl;
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
        for(int i = 0;i < N * batchSize;i++) {
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
        for(int i =0; i < N * batchSize; i++) {
            if (asumhcblas != asumcblas) {
                ispassed = 0;
                cout <<" HCSASUM[" << i<< "] " << asumhcblas << " does not match with CBLASSASUM[" << i <<"] "<< asumcblas << endl;
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
