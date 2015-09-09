#include <iostream>
#include "../../hcblaslib.h"
#include <cstdlib> 
#include "cblas.h"

using namespace std;
int main(int argc, char** argv)
{
    /* HCBLAS implementation */
    Hcblaslibrary hc;
    if (argc < 4){
        cout<<"No sufficient commandline arguments specified"<<"argc :"<<argc<<endl;
        return -1;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int Imple_type = atoi(argv[3]);
    bool ispassed = 1;
    float alpha = 1;
    long lda;
    int incX = 1;
    int incY = 1;
    long xOffset = 0;
    long yOffset = 0;
    long aOffset = 0;
    long X_batchOffset = M;
    long Y_batchOffset = N;
    long A_batchOffset = M * N;
    int batchSize = 128;
    long lenx,  leny; 
    hcblasStatus status;
    HCBLAS_ORDER hcOrder = colMajor; 
    /* CBLAS implementation */
    enum CBLAS_ORDER order;
    order = CblasColMajor;
    if(hcOrder)
       lda = M;
    else
       lda = N;
    lenx =  1 + (M-1) * abs(incX);
    leny =  1 + (N-1) * abs(incY);
    float *xSger = (float*)calloc( lenx , sizeof(float));
    float *ySger = (float*)calloc( leny , sizeof(float));
    float *ASger = (float *)calloc( M * N , sizeof(float));
    float *Acblas = (float *)calloc( M * N , sizeof(float));
    std::vector<float> HostX(lenx);
    std::vector<float> HostY(leny);
    std::vector<float> HostA(M * N);
    Concurrency::array<float> xView(lenx, xSger);
    Concurrency::array<float> yView(leny, ySger);
    Concurrency::array<float> aMat( M * N, ASger);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
   
    float *xSgerbatch = (float*)calloc( lenx * batchSize, sizeof(float));
    float *ySgerbatch = (float*)calloc( leny * batchSize, sizeof(float));
    float *ASgerbatch = (float *)calloc( M * N * batchSize, sizeof(float));
    float *Acblasbatch = (float *)calloc( M * N * batchSize, sizeof(float));
    std::vector<float> HostX_batch(lenx * batchSize);
    std::vector<float> HostY_batch(leny * batchSize);
    std::vector<float> HostA_batch(M * N * batchSize);
    Concurrency::array<float> xbatchView(lenx * batchSize, xSgerbatch);
    Concurrency::array<float> ybatchView(leny * batchSize, ySgerbatch);
    Concurrency::array<float> abatchMat( M * N * batchSize, ASgerbatch);

    
    {
        for(int i = 0;i < M;i++){
            HostX[i] = rand() % 10;
            xSger[i] = HostX[i];
        }
        for(int i = 0;i < N;i++){
            HostY[i] = rand() % 15;
            ySger[i] = HostY[i];
        }
        for(int i = 0;i< M * N ;i++){
            HostA[i] = rand() % 25;
            Acblas[i] = HostA[i];
            ASger[i] = Acblas[i];
        }
        if(Imple_type == 1){
            status = hc.hcblas_sger(hcOrder, M , N , &alpha, xSger, xOffset, incX, ySger, yOffset, incY, ASger, aOffset, lda );
            cblas_sger( order, M, N, alpha, xSger, incX, ySger, incY, Acblas, lda);
            for(int i =0; i < M * N ; i++){
                if (ASger[i] != Acblas[i]){
                    ispassed = 0;
                    cout <<" HCSGER[" << i<< "] " << ASger[i] << " does not match with CBLASSGER[" << i <<"] "<< Acblas[i] << endl;
                    break;
                }
                else
                    continue;
            }
            cout << (ispassed? "TEST PASSED": "TEST FAILED") <<endl;
            free(xSger);
            free(ySger);
            free(ASger);
            free(Acblas);

        }

        else if(Imple_type == 2){
            Concurrency::copy(begin(HostX), end(HostX), xView);
            Concurrency::copy(begin(HostY), end(HostY), yView);
            Concurrency::copy(begin(HostA), end(HostA), aMat);
            status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda );
            Concurrency::copy(aMat, begin(HostA));
            cblas_sger( order, M, N, alpha, xSger, incX, ySger, incY, Acblas, lda);
            for(int i =0; i < M * N ; i++){
                if (HostA[i] != Acblas[i]){
                    ispassed = 0;
                    cout <<" HCSGER[" << i<< "] " << HostA[i] << " does not match with CBLASSGER[" << i <<"] "<< Acblas[i] << endl;
                    break;
                }
                else
                    continue;
            }
            cout << (ispassed? "TEST PASSED": "TEST FAILED") <<endl;

        }

        else{
            for(int i = 0;i < M * batchSize;i++){
                HostX_batch[i] = rand() % 10;
                xSgerbatch[i] = HostX_batch[i];
            }
            for(int i = 0;i < N * batchSize;i++){
                HostY_batch[i] = rand() % 15;
                ySgerbatch[i] =  HostY_batch[i];
            }
            for(int i = 0;i< M * N * batchSize;i++){
                HostA_batch[i] = rand() % 25;
                Acblasbatch[i] = HostA_batch[i];
                ASgerbatch[i] = Acblasbatch[i];
            }
            Concurrency::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
            Concurrency::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
            Concurrency::copy(begin(HostA_batch), end(HostA_batch), abatchMat);
            status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
            Concurrency::copy(abatchMat, begin(HostA_batch));
            for(int i = 0; i < batchSize; i++)
               cblas_sger( order, M, N, alpha, xSgerbatch + i * M, incX, ySgerbatch + i * N, incY, Acblasbatch + i * M * N, lda); 
            for(int i =0; i < M * N * batchSize; i++){
               if (HostA_batch[i] != Acblasbatch[i]){
                   ispassed = 0;
                   cout <<" HCSGER[" << i<< "] " << HostA_batch[i] << " does not match with CBLASSGER[" << i <<"] "<< Acblasbatch[i] << endl;
                   break;
               }
            else 
                  continue;  
            }
            cout << (ispassed? "TEST PASSED": "TEST FAILED") <<endl;
         }
    }
    return 0;
}
