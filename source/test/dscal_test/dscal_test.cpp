#include <iostream>
#include "../../ampblaslib.h"
#include <cstdlib> 
#include "cblas.h"

using namespace std;
int main(int argc, char** argv)
{   

    /* AMPBLAS implementation */
    Ampblaslibrary amp;
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
    ampblasStatus status;
    int batchSize = 128;
    long X_batchOffset = N; 
  
    /* CBLAS implementation */
    long lenx = 1 + (N-1) * abs(incX);
    double *X = (double*)calloc(lenx, sizeof(double));
    double *Xcblas = (double*)calloc(lenx, sizeof(double));
    double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
    double *Xcblasbatch = (double*)calloc(N * batchSize, sizeof(double));

    Concurrency::array_view<double> xView(lenx, X);
    Concurrency::array_view<double> xbatchView(lenx * batchSize, Xbatch);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    
    for(int i = 0;i < N;i++){
        xView[i] = rand() % 10;
        X[i] = xView[i];
        Xcblas[i] = X[i];
    }
        
    if (Imple_type == 1){
	status = amp.ampblas_dscal(N, &alpha, X, incX, xOffset);
        cblas_dscal( N, alpha, Xcblas, incX);
        for(int i = 0; i < N ; i++){
            if (X[i] != Xcblas[i]){
                ispassed = 0;
                cout <<" AMPSSCAL[" << i<< "] " << X[i] << " does not match with CBLASSSCAL[" << i <<"] "<< Xcblas[i] << endl;
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
        status = amp.ampblas_dscal(accl_view, N, alpha, xView, incX, xOffset);
        cblas_dscal( N, alpha, Xcblas, incX );
        for(int i = 0; i < N ; i++){
            if (xView[i] != Xcblas[i]){
                ispassed = 0;
                cout <<" AMPSSCAL[" << i<< "] " << xView[i] << " does not match with CBLASSSCAL[" << i <<"] "<< Xcblas[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
     }

    else{ 
        for(int i = 0;i < N * batchSize;i++){
            xbatchView[i] = rand() % 10;
            Xbatch[i] = xbatchView[i];
            Xcblasbatch[i] = Xbatch[i];
         }

        status= amp.ampblas_dscal(accl_view, N, alpha, xbatchView, incX, xOffset, X_batchOffset, batchSize);
        for(int i = 0; i < batchSize; i++)
        	cblas_dscal( N, alpha, Xcblasbatch + i * N, incX);
        for(int i =0; i < N * batchSize; i ++){
            if (xbatchView[i] != Xcblasbatch[i]){
                ispassed = 0;
                cout <<" AMPSSCAL[" << i<< "] " << xbatchView[i] << " does not match with CBLASSSCAL[" << i <<"] "<< Xcblasbatch[i] << endl;
                break;
            }
            else 
              continue;  
        }
        cout << (ispassed? "TEST PASSED":"TEST FAILED") << endl;
    }
    return 0;
}
