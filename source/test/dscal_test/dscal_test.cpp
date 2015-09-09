#include <iostream>
#include "../../hcblaslib.h"
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
    bool ispassed = 1;
    const double alpha = 2;
    int incX = 1;
    long xOffset = 0;
    hcblasStatus status;
    int batchSize = 128;
    long X_batchOffset = N; 
  
    /* CBLAS implementation */
    long lenx = 1 + (N-1) * abs(incX);
    double *X = (double*)calloc(lenx, sizeof(double));
    double *Xcblas = (double*)calloc(lenx, sizeof(double));
    double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
    double *Xcblasbatch = (double*)calloc(N * batchSize, sizeof(double));

    Concurrency::array<double> xView(lenx, X);
    Concurrency::array<double> xbatchView(lenx * batchSize, Xbatch);
    std::vector<double> HostX(lenx);
    std::vector<double> HostX_batch(lenx * batchSize);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    
    for(int i = 0;i < N;i++){
        HostX[i] = rand() % 10;
        X[i] = HostX[i];
        Xcblas[i] = X[i];
    }
        
    if (Imple_type == 1){
	status = hc.hcblas_dscal(N, &alpha, X, incX, xOffset);
        cblas_dscal( N, alpha, Xcblas, incX);
        for(int i = 0; i < N ; i++){
            if (X[i] != Xcblas[i]){
                ispassed = 0;
                cout <<" HCDSCAL[" << i<< "] " << X[i] << " does not match with CBLASDSCAL[" << i <<"] "<< Xcblas[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
        free(X);
        free(Xcblas);
    }
    
    else if (Imple_type ==2){
        Concurrency::copy(begin(HostX), end(HostX), xView);
        status = hc.hcblas_dscal(accl_view, N, alpha, xView, incX, xOffset);
        Concurrency::copy(xView, begin(HostX));  
        cblas_dscal( N, alpha, Xcblas, incX );
        for(int i = 0; i < N ; i++){
            if (HostX[i] != Xcblas[i]){
                ispassed = 0;
                cout <<" HCDSCAL[" << i<< "] " << HostX[i] << " does not match with CBLASDSCAL[" << i <<"] "<< Xcblas[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
     }

    else{ 
        for(int i = 0;i < N * batchSize;i++){
            HostX_batch[i] = rand() % 10;
            Xcblasbatch[i] = HostX_batch[i];
         }
        Concurrency::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        status= hc.hcblas_dscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
        Concurrency::copy(xbatchView, begin(HostX_batch));  
        for(int i = 0; i < batchSize; i++)
        	cblas_dscal( N, alpha, Xcblasbatch + i * N, incX);
        for(int i =0; i < N * batchSize; i ++){
            if (HostX_batch[i] != Xcblasbatch[i]){
                ispassed = 0;
                cout <<" HCDSCAL[" << i<< "] " << HostX_batch[i] << " does not match with CBLASDSCAL[" << i <<"] "<< Xcblasbatch[i] << endl;
                break;
            }
            else 
              continue;  
        }
        cout << (ispassed? "TEST PASSED":"TEST FAILED") << endl;
    }
    return 0;
}
