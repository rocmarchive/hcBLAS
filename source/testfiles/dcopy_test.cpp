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
    long xOffset = 0;
    int incY = 1;
    long yOffset = 0;
    hcblasStatus status;
    int batchSize = 128;
    long X_batchOffset = N;
    long Y_batchOffset = N;
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    double *X = (double*)calloc(lenx, sizeof(double));
    double *Y = (double*)calloc(leny, sizeof(double));
    double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
    double *Ybatch = (double*)calloc(leny * batchSize, sizeof(double));
#ifdef LINUX
    /* CBLAS implementation */
    bool ispassed = 1;
    double *Ycblas = (double*)calloc(leny, sizeof(double));
    double *Ycblasbatch = (double*)calloc(leny * batchSize, sizeof(double));
#endif
    Concurrency::array<double> xView(lenx, X);
    Concurrency::array<double> yView(leny, Y);
    Concurrency::array<double> xbatchView(lenx * batchSize, Xbatch);
    Concurrency::array<double> ybatchView(leny * batchSize, Ybatch);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    std::vector<double> HostX(lenx);
    std::vector<double> HostY(leny);
    std::vector<double> HostX_batch(lenx * batchSize);
    std::vector<double> HostY_batch(leny * batchSize);
    for(int i = 0;i < lenx;i++) {
        HostX[i] = rand() % 10;
        X[i] =  HostX[i];
    }
    for(int i = 0;i < leny;i++) {
        HostY[i] =  rand() % 15;
        Y[i] = HostY[i];
#ifdef LINUX
        Ycblas[i] = Y[i];
#endif
    }
    if (Imple_type == 1) {
	status = hc.hcblas_dcopy(N, X, incX, xOffset, Y, incY, yOffset);
#ifdef LINUX
        cblas_dcopy( N, X, incX, Ycblas, incY);
        for(int i = 0; i < leny ; i++){
            if (Y[i] != Ycblas[i]){
                ispassed = 0;
                cout <<" HCDCOPY[" << i<< "] " << Y[i] << " does not match with CBLASDCOPY[" << i <<"] "<< Ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
        free(Ycblas);
#else
        cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
        free(X);
        free(Y);
    }
    else if (Imple_type ==2) {
        Concurrency::copy(begin(HostX), end(HostX), xView);
        Concurrency::copy(begin(HostY), end(HostY), yView);
        status = hc.hcblas_dcopy(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
        Concurrency::copy(yView, begin(HostY));
#ifdef LINUX
        cblas_dcopy( N, X, incX, Ycblas, incY);
        for(int i = 0; i < leny; i++){
            if (HostY[i] != Ycblas[i]){
                ispassed = 0;
                cout <<" HCDCOPY[" << i<< "] " << HostY[i] << " does not match with CBLASDCOPY[" << i <<"] "<< Ycblas[i] << endl;
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
    else { 
        for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
            Xbatch[i] = HostX_batch[i];
         }
       for(int i = 0;i < leny * batchSize;i++){
            HostY_batch[i] =  rand() % 15;
#ifdef LINUX
            Ycblasbatch[i] = HostY_batch[i];
#endif
         }
        Concurrency::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        Concurrency::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
        status= hc.hcblas_dcopy(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
        Concurrency::copy(ybatchView, begin(HostY_batch));
#ifdef LINUX
        for(int i = 0; i < batchSize; i++)
        	cblas_dcopy( N, Xbatch + i * N, incX, Ycblasbatch + i * N, incY );
        for(int i =0; i < leny * batchSize; i ++){
            if (HostY_batch[i] != Ycblasbatch[i]){
                ispassed = 0;
                cout <<" HCDCOPY[" << i<< "] " <<  HostY_batch[i] << " does not match with CBLASDCOPY[" << i <<"] "<< Ycblasbatch[i] << endl;
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
