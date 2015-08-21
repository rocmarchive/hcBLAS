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
    float dotampblas, dotcblas = 0.0;
    long xOffset = 0;
    int incY = 1;
    long yOffset = 0;
    long X_batchOffset = N;
    long Y_batchOffset = N;
    int batchSize = 128;
    ampblasStatus status;
    if (N > 5000)
	batchSize = 50;

    /* CBLAS implementation */
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Y = (float*)calloc(leny, sizeof(float));
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    float *Ybatch = (float*)calloc(leny * batchSize, sizeof(float));
    float *dotcblastemp =(float*)calloc(batchSize, sizeof(float)); 
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
        HostY[i] = rand() % 15;
        Y[i] = HostY[i];
    }
    
    if (Imple_type == 1){
	status = amp.ampblas_sdot(N, X, incX, xOffset, Y, incY, yOffset, &dotampblas);
        dotcblas = cblas_sdot( N, X, incX, Y, incY);
        for(int i = 0; i < N ; i++){
            if (dotampblas != dotcblas){
                ispassed = 0;
                cout <<" AMPSDOT[" << i<< "] " << dotampblas << " does not match with CBLASSDOT[" << i <<"] "<< dotcblas << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
        free(X);
        free(Y);
    }
    
    else if (Imple_type ==2){
        Concurrency::copy(begin(HostX), end(HostX), xView);
        Concurrency::copy(begin(HostY), end(HostY), yView);
        status = amp.ampblas_sdot(accl_view, N, xView, incX, xOffset, yView, incY, yOffset, dotampblas);
        dotcblas = cblas_sdot( N, X, incX, Y, incY);
        for(int i = 0; i < N ; i++){
             if (dotampblas != dotcblas){
                ispassed = 0;
                cout <<" AMPSDOT[" << i<< "] " << dotampblas << " does not match with CBLASSDOT[" << i <<"] "<< dotcblas << endl;
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
         }
        Concurrency::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        Concurrency::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
        status= amp.ampblas_sdot(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, dotampblas, X_batchOffset, Y_batchOffset, batchSize);
        for(int i = 0; i < batchSize; i++){
        	dotcblastemp[i] = cblas_sdot( N, Xbatch + i * N, incX, Ybatch + i * N, incY);
                dotcblas += dotcblastemp[i];
        }
        for(int i =0; i < N * batchSize; i ++){
             if (dotampblas != dotcblas){
                ispassed = 0;
                cout <<" AMPSDOT[" << i<< "] " << dotampblas << " does not match with CBLASSDOT[" << i <<"] "<< dotcblas << endl;
                break;
            }
            else 
              continue;  
        }
        cout << (ispassed? "TEST PASSED":"TEST FAILED") << endl;
    }
    return 0;
}
