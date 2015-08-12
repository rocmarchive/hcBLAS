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
    int incX = 1;
    long xOffset = 0;
    int incY = 1;
    long yOffset = 0;
    ampblasStatus status;
    int batchSize = 128;
    long X_batchOffset = N;
    long Y_batchOffset = N;

    /* CBLAS implementation */
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Y = (float*)calloc(leny, sizeof(float));
    float *Ycblas = (float*)calloc(leny, sizeof(float));
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    float *Ybatch = (float*)calloc(leny * batchSize, sizeof(float));
    float *Ycblasbatch = (float*)calloc(N * batchSize, sizeof(float));

    Concurrency::array_view<float> xView(lenx, X);
    Concurrency::array_view<float> yView(leny, Y);
    Concurrency::array_view<float> xbatchView(lenx * batchSize, Xbatch);
    Concurrency::array_view<float> ybatchView(leny * batchSize, Ybatch);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    
    for(int i = 0;i < N;i++){
        xView[i] = rand() % 10;
        X[i] = xView[i];
    }
    for(int i = 0;i < N;i++){
        yView[i] =  rand() % 15;
        Y[i] = yView[i];
        Ycblas[i] = Y[i];
    }
    
    if (Imple_type == 1){
	status = amp.ampblas_scopy(N, X, incX, xOffset, Y, incY, yOffset);
        cblas_scopy( N, X, incX, Ycblas, incY);
        for(int i = 0; i < N ; i++){
            if (Y[i] != Ycblas[i]){
                ispassed = 0;
                cout <<" AMPSCOPY[" << i<< "] " << Y[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
        free(X);
        free(Y);
        free(Ycblas);
    }
    
    else if (Imple_type ==2){
        status = amp.ampblas_scopy(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
        cblas_scopy( N, X, incX, Ycblas, incY );
        for(int i = 0; i < N ; i++){
            if (yView[i] != Ycblas[i]){
                ispassed = 0;
                cout <<" AMPSCOPY[" << i<< "] " << yView[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblas[i] << endl;
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
         }
       for(int i = 0;i < N * batchSize;i++){
            ybatchView[i] =  rand() % 15;
            Ybatch[i] = ybatchView[i];
            Ycblasbatch[i] = Ybatch[i];
         }

        status= amp.ampblas_scopy(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
        for(int i = 0; i < batchSize; i++)
        	cblas_scopy( N, Xbatch + i * N, incX, Ycblasbatch + i * N, incY );
        for(int i =0; i < N * batchSize; i ++){
            if (ybatchView[i] != Ycblasbatch[i]){
                ispassed = 0;
                cout <<" AMPSCOPY[" << i<< "] " << ybatchView[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblasbatch[i] << endl;
                break;
            }
            else 
              continue;  
        }
        cout << (ispassed? "TEST PASSED":"TEST FAILED") << endl;
    }
    return 0;
}
