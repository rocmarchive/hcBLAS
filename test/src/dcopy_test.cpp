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
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type I - Inputs and Outputs are host double pointers */

    if (Imple_type == 1) {
        for(int i = 0;i < lenx;i++) {
            X[i] = rand() % 10;
        }
        for(int i = 0;i < leny;i++) {
            Y[i] =  rand() % 15;
#ifdef LINUX
            Ycblas[i] = Y[i];
#endif
        }

	status = hc.hcblas_dcopy(N, X, incX, xOffset, Y, incY, yOffset);
#ifdef LINUX
        cblas_dcopy( N, X, incX, Ycblas, incY);
        for(int i = 0; i < leny ; i++){
            if (Y[i] != Ycblas[i]){
                ispassed = 0;
                cout <<" HCSCOPY[" << i<< "] " << Y[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        free(Ycblas);
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
        free(X);
        free(Y);
    }

/* Implementation type II - Inputs and Outputs are HC++ double array_view containers */

    else if (Imple_type ==2) {
        Concurrency::array_view<double> xView(lenx, X);
        Concurrency::array_view<double> yView(leny, Y);
        for(int i = 0;i < lenx;i++) {
            xView[i] = rand() % 10;
            X[i] = xView[i];
        }
        for(int i = 0;i < leny;i++) {
            yView[i] =  rand() % 15;
#ifdef LINUX
            Ycblas[i] = yView[i];
#endif
        }
        status = hc.hcblas_dcopy(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
#ifdef LINUX
        cblas_dcopy( N, X, incX, Ycblas, incY );
        for(int i = 0; i < N ; i++) {
            if (yView[i] != Ycblas[i]) {
                ispassed = 0;
                cout <<" HCSCOPY[" << i<< "] " << yView[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif

     }

/* Implementation type III - Inputs and Outputs are HC++ double array_view containers with batch processing */

     else if(Imple_type == 3) {
        Concurrency::array_view<double> xbatchView(lenx * batchSize, Xbatch);
        Concurrency::array_view<double> ybatchView(leny * batchSize, Ybatch);
        for(int i = 0;i < lenx * batchSize;i++) {
            xbatchView[i] = rand() % 10;
            Xbatch[i] = xbatchView[i];
        }
        for(int i = 0;i < leny * batchSize;i++) {
            ybatchView[i] =  rand() % 15;
#ifdef LINUX
            Ycblasbatch[i] = ybatchView[i];
#endif
        }
        status= hc.hcblas_dcopy(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
#ifdef LINUX
        for(int i = 0; i < batchSize; i++)
                cblas_dcopy( N, Xbatch + i * N, incX, Ycblasbatch + i * N, incY );
        for(int i =0; i < N * batchSize; i++) {
            if (ybatchView[i] != Ycblasbatch[i]) {
                ispassed = 0;
                cout <<" HCSCOPY[" << i<< "] " << ybatchView[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblasbatch[i] << endl;
                break;
            }
            else
              continue;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
    }

/* Implementation type IV - Inputs and Outputs are HC++ double array containers */
    
    else if (Imple_type == 4) {
        Concurrency::array<double> xView(lenx, X);
        Concurrency::array<double> yView(leny, Y);
        std::vector<double> HostX(lenx);
        std::vector<double> HostY(leny);
        for(int i = 0;i < lenx;i++){
            HostX[i] = rand() % 10;
            X[i] = HostX[i];
        }
        for(int i = 0;i < leny;i++){
            HostY[i] =  rand() % 15;
#ifdef LINUX
            Ycblas[i] = Y[i];
#endif
        }
        Concurrency::copy(begin(HostX), end(HostX), xView);
        Concurrency::copy(begin(HostY), end(HostY), yView);
        status = hc.hcblas_dcopy(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
        Concurrency::copy(yView, begin(HostY));
#ifdef LINUX
        cblas_dcopy( N, X, incX, Ycblas, incY );
        for(int i = 0; i < leny; i++){
            if (HostY[i] != Ycblas[i]){
                ispassed = 0;
                cout <<" HCSCOPY[" << i<< "] " << HostY[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
     }

/* Implementation type V - Inputs and Outputs are HC++ double array containers with batch processing */

    else{ 
        Concurrency::array<double> xbatchView(lenx * batchSize, Xbatch);
        Concurrency::array<double> ybatchView(leny * batchSize, Ybatch);
        std::vector<double> HostX_batch(lenx * batchSize);
        std::vector<double> HostY_batch(leny * batchSize);
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
                cout <<" HCSCOPY[" << i<< "] " <<HostY_batch[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblasbatch[i] << endl;
                break;
            }
            else 
              continue;  
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
#else
        if(status) cout << "TEST FAILED" << endl; 
#endif
    }
    return 0;
}
