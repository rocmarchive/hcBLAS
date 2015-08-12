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
    float asumampblas, asumcblas = 0.0;
    int incX = 1;
    long xOffset = 0;
    ampblasStatus status;
    int batchSize = 128;
    long X_batchOffset = N;
    if(N > 10000)
	batchSize = 50;
    /* CBLAS implementation */
    long lenx = 1 + (N-1) * abs(incX);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    float *asumcblastemp = (float*)calloc(batchSize, sizeof(float));

    Concurrency::array_view<float> xView(lenx, X);
    Concurrency::array_view<float> xbatchView(lenx * batchSize, Xbatch);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    
    for(int i = 0;i < N;i++){
        xView[i] = rand() % 10;
        X[i] = xView[i];
    }
        
    if (Imple_type == 1){
	status = amp.ampblas_sasum(N, X, incX, xOffset, &asumampblas);
        asumcblas = cblas_sasum( N, X, incX);
        for(int i = 0; i < N ; i++){
            if (asumampblas != asumcblas){
                ispassed = 0;
                cout <<" AMPSASUM[" << i<< "] " << asumampblas << " does not match with CBLASSASUM[" << i <<"] "<< asumcblas << endl;
                break;
            }
            else
                continue;
        }
        cout << (ispassed? "TEST PASSED" : "TEST FAILED") <<endl;
        free(X);
    }
      
    else if (Imple_type ==2){
        status = amp.ampblas_sasum(accl_view, N, xView, incX, xOffset, asumampblas);
        asumcblas = cblas_sasum( N, X, incX);
        for(int i = 0; i < N ; i++){
            if (asumampblas != asumcblas){
                ispassed = 0;
                cout <<" AMPSASUM[" << i<< "] " << asumampblas << " does not match with CBLASSASUM[" << i <<"] "<< asumcblas << endl;
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

        status= amp.ampblas_sasum(accl_view, N, xbatchView, incX, xOffset, asumampblas, X_batchOffset, batchSize);
        for(int i = 0; i < batchSize; i++){
        	asumcblastemp[i] = cblas_sasum( N, Xbatch + i * N, incX);
                asumcblas += asumcblastemp[i];
        }
        for(int i =0; i < N * batchSize; i ++){
            if (asumampblas != asumcblas){
                ispassed = 0;
                cout <<" AMPSASUM[" << i<< "] " << asumampblas << " does not match with CBLASSASUM[" << i <<"] "<< asumcblas << endl;
                break;
            }
            else 
              continue;  
        }
        cout << (ispassed? "TEST PASSED":"TEST FAILED") << endl;
    }
    return 0;
}
