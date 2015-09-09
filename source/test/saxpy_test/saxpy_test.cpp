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
    const float alpha = 1;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long X_batchOffset = N;
    long Y_batchOffset = N;
    int batchSize = 128;
    hcblasStatus status;

    /* CBLAS implementation */
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Y = (float*)calloc(leny, sizeof(float));
    float *Ycblas = (float*)calloc(N, sizeof(float));
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    float *Ybatch = (float*)calloc(leny * batchSize, sizeof(float));
    float *Ycblasbatch = (float*)calloc(N * batchSize, sizeof(float));


    Concurrency::array<float> xView(lenx, X);
    Concurrency::array<float> yView(leny, Y);
    Concurrency::array<float> xbatchView(lenx * batchSize, Xbatch);
    Concurrency::array<float> ybatchView(leny * batchSize, Ybatch);
    std::vector<float> HostX(lenx);
    std::vector<float> HostY(leny);
    std::vector<float> HostX_batch(lenx * batchSize);
    std::vector<float> HostY_batch(leny * batchSize);
   
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    
    for(int i = 0;i < N;i++){
        HostX[i] = rand() % 10;
        X[i] = HostX[i];
    }
    for(int i = 0;i < N;i++){
        HostY[i] =  rand() % 15;
        Y[i] = HostY[i];
        Ycblas[i] = Y[i];
    }
        
    if (Imple_type == 1){
	status = hc.hcblas_saxpy(N, &alpha, X, incX, Y, incY , xOffset, yOffset);
        cblas_saxpy( N, alpha, X, incX, Ycblas, incY );
        for(int i = 0; i < N ; i++){
            if (Y[i] != Ycblas[i]){
                ispassed = 0;
                cout <<" HCSAXPY[" << i<< "] " << Y[i] << " does not match with CBLASSAXPY[" << i <<"] "<< Ycblas[i] << endl;
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
  
     
    else if(Imple_type ==2){
        Concurrency::copy(begin(HostX), end(HostX), xView);
        Concurrency::copy(begin(HostY), end(HostY), yView);
        status = hc.hcblas_saxpy(accl_view, N, alpha, xView, incX, yView, incY , xOffset, yOffset);
        Concurrency::copy(yView, begin(HostY));
        cblas_saxpy( N, alpha, X, incX, Ycblas, incY );
        for(int i = 0; i < N ; i++){
            if (HostY[i] != Ycblas[i]){
                ispassed = 0;
                cout <<" HCSAXPY[" << i<< "] " << HostY[i] << " does not match with CBLASSAXPY[" << i <<"] "<< Ycblas[i] << endl;
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
            Xbatch[i] = HostX_batch[i];
         }
       for(int i = 0;i < N * batchSize;i++){
            HostY_batch[i] =  rand() % 15;
            Ybatch[i] = HostY_batch[i];
            Ycblasbatch[i] = Ybatch[i];
         }
        Concurrency::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        Concurrency::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
        status= hc.hcblas_saxpy(accl_view, N, alpha, xbatchView, incX, X_batchOffset, ybatchView, incY, Y_batchOffset, xOffset, yOffset, batchSize);
        Concurrency::copy(ybatchView, begin(HostY_batch));

        for(int i = 0; i < batchSize; i++)
        	cblas_saxpy( N, alpha, Xbatch + i * N, incX, Ycblasbatch + i * N, incY );
        for(int i =0; i < N * batchSize; i ++){
            if (HostY_batch[i] != Ycblasbatch[i]){
                ispassed = 0;
                cout <<" HCSAXPY[" << i<< "] " << HostY_batch[i] << " does not match with CBLASSAXPY[" << i <<"] "<< Ycblasbatch[i] << endl;
                break;
            }
            else 
              continue;  
        }
        cout << (ispassed? "TEST PASSED":"TEST FAILED") << endl;
    }
    return 0;
}
