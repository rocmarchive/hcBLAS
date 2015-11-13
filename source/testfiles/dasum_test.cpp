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
    Concurrency::array<double> xView(lenx, X);
    Concurrency::array<double> xbatchView(lenx * batchSize, Xbatch);
    std::vector<double> HostX(lenx);
    std::vector<double> HostX_batch(lenx * batchSize);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    for(int i = 0;i < lenx;i++) {
        HostX[i] = rand() % 10;
        X[i] = HostX[i];
    }
    if (Imple_type == 1) {
	status = hc.hcblas_dasum(N, X, incX, xOffset, &asumhcblas);
#ifdef LINUX
        asumcblas = cblas_dasum( N, X, incX);
        if (asumhcblas != asumcblas){
            ispassed = 0;
            cout <<" HCDASUM " << asumhcblas << " does not match with CBLASDASUM "<< asumcblas << endl;
        }
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
        free(X);
    }
      
    else if (Imple_type ==2) {
        Concurrency::copy(begin(HostX), end(HostX), xView);
        status = hc.hcblas_dasum(accl_view, N, xView, incX, xOffset, asumhcblas);
#ifdef LINUX
        asumcblas = cblas_dasum( N, X, incX);
        if (asumhcblas != asumcblas){
            ispassed = 0;
            cout <<" HCDASUM " << asumhcblas << " does not match with CBLASDASUM "<< asumcblas << endl;
        }
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
     }

    else{ 
        for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
            Xbatch[i] = HostX_batch[i];
         }
        Concurrency::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        status= hc.hcblas_dasum(accl_view, N, xbatchView, incX, xOffset, asumhcblas, X_batchOffset, batchSize);
#ifdef LINUX
        for(int i = 0; i < batchSize; i++){
        	asumcblastemp[i] = cblas_dasum( N, Xbatch + i * N, incX);
                asumcblas += asumcblastemp[i];
        }
        if (asumhcblas != asumcblas){
            ispassed = 0;
            cout <<" HCDASUM " << asumhcblas << " does not match with CBLASDASUM "<< asumcblas << endl;
        }
        cout << (ispassed? "TEST PASSED":"TEST FAILED") << endl;
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
    }
    return 0;
}
