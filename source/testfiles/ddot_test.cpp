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
    int incX = 1;
    double dothcblas;
    long xOffset = 0;
    int incY = 1;
    long yOffset = 0;
    long X_batchOffset = N;
    long Y_batchOffset = N;
    int batchSize = 128;
    hcblasStatus status;
    if(N > 5000)
	batchSize = 50;
#ifdef LINUX 
    /* CBLAS implementation */
    bool ispassed = 1;
    double dotcblas = 0.0;
    double *dotcblastemp =(double*)calloc(batchSize, sizeof(double));
#endif 
    /* CBLAS implementation */
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    double *X = (double*)calloc(lenx, sizeof(double));
    double *Y = (double*)calloc(leny, sizeof(double));
    double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
    double *Ybatch = (double*)calloc(leny * batchSize, sizeof(double));
    Concurrency::array<double> xView(lenx, X);
    Concurrency::array<double> yView(leny, Y);
    Concurrency::array<double> xbatchView(lenx * batchSize, Xbatch);
    Concurrency::array<double> ybatchView(leny * batchSize, Ybatch);
    std::vector<double> HostX(lenx);
    std::vector<double> HostY(leny);
    std::vector<double> HostX_batch(lenx * batchSize);
    std::vector<double> HostY_batch(leny * batchSize);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    for(int i = 0;i < N;i++) {
        HostX[i] = rand() % 10;
        X[i] = HostX[i];
    }
    for(int i = 0;i < N;i++) {
        HostY[i] = rand() % 15;
        Y[i] = HostY[i];
    }
    if (Imple_type == 1) {
	status = hc.hcblas_ddot(N, X, incX, xOffset, Y, incY, yOffset, &dothcblas);
#ifdef LINUX
        dotcblas = cblas_ddot( N, X, incX, Y, incY);
        for(int i = 0; i < N ; i++) {
            if (dothcblas != dotcblas) {
                ispassed = 0;
                cout <<" HCDDOT[" << i<< "] " << dothcblas << " does not match with CBLASDDOT[" << i <<"] "<< dotcblas << endl;
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
        free(Y);
    }
    
    else if (Imple_type ==2){
        Concurrency::copy(begin(HostX), end(HostX), xView);
        Concurrency::copy(begin(HostY), end(HostY), yView);
        status = hc.hcblas_ddot(accl_view, N, xView, incX, xOffset, yView, incY, yOffset, dothcblas);
#ifdef LINUX
        dotcblas = cblas_ddot( N, X, incX, Y, incY);
        for(int i = 0; i < N ; i++) {
             if (dothcblas != dotcblas) {
                ispassed = 0;
                cout <<" HCDDOT[" << i<< "] " << dothcblas << " does not match with CBLASDDOT[" << i <<"] "<< dotcblas << endl;
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
       for(int i = 0;i < N * batchSize;i++) {
            HostY_batch[i] =  rand() % 15;
            Ybatch[i] = HostY_batch[i];
         }
        Concurrency::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        Concurrency::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
        status= hc.hcblas_ddot(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, dothcblas, X_batchOffset, Y_batchOffset, batchSize);
#ifdef LINUX
        for(int i = 0; i < batchSize; i++) {
        	dotcblastemp[i] = cblas_ddot( N, Xbatch + i * N, incX, Ybatch + i * N, incY);
                dotcblas += dotcblastemp[i];
        }
        for(int i =0; i < N * batchSize; i++) {
             if (dothcblas != dotcblas){
                ispassed = 0;
                cout <<" HCDDOT[" << i<< "] " << dothcblas << " does not match with CBLASDDOT[" << i <<"] "<< dotcblas << endl;
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
