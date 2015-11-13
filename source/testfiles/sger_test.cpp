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
    if (argc < 4){
        cout<<"No sufficient commandline arguments specified"<<"argc :"<<argc<<endl;
        return -1;
    }
    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int Imple_type = atoi(argv[3]);
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
    hcblasOrder hcOrder = ColMajor;
    lda = (hcOrder)? M : N;  
    lenx =  1 + (M-1) * abs(incX);
    leny =  1 + (N-1) * abs(incY);
#ifdef LINUX
    /* CBLAS implementation */
    bool ispassed = 1;
    enum CBLAS_ORDER order;
    order = CblasColMajor;
    float *Acblas = (float *)calloc( lenx * leny , sizeof(float));
#endif
    float *xSger = (float*)calloc( lenx , sizeof(float));
    float *ySger = (float*)calloc( leny , sizeof(float));
    float *ASger = (float *)calloc( lenx * leny , sizeof(float));
    std::vector<float> HostX(lenx);
    std::vector<float> HostY(leny);
    std::vector<float> HostA(lenx * leny);
    Concurrency::array<float> xView(lenx, xSger);
    Concurrency::array<float> yView(leny, ySger);
    Concurrency::array<float> aMat( lenx * leny, ASger);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    for(int i = 0;i < lenx;i++) {
        HostX[i] = rand() % 10;
        xSger[i] = HostX[i];
    }
    for(int i = 0;i < leny;i++) {
        HostY[i] = rand() % 15;
        ySger[i] = HostY[i];
    }
    for(int iter=0; iter<10; iter++) {
        for(int i = 0;i< lenx * leny ;i++) {
            HostA[i] = rand() % 25;
#ifdef LINUX
            Acblas[i] = HostA[i];
#endif
            ASger[i] = HostA[i];
        }
        if(Imple_type == 1) {
            status = hc.hcblas_sger(hcOrder, M , N , &alpha, xSger, xOffset, incX, ySger, yOffset, incY, ASger, aOffset, lda );
#ifdef LINUX
            cblas_sger( order, M, N, alpha, xSger, incX, ySger, incY, Acblas, lda);
            for(int i =0; i < lenx * leny ; i++){
                if (ASger[i] != Acblas[i]){
                    ispassed = 0;
                    cout <<" HCSGER[" << i<< "] " << ASger[i] << " does not match with CBLASSGER[" << i <<"] "<< Acblas[i] << endl;
                    break;
                }
                else
                    continue;
            }
            cout << (ispassed? "TEST PASSED": "TEST FAILED") <<endl;
#else
            cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
        }
        else if(Imple_type == 2) {
            Concurrency::copy(begin(HostX), end(HostX), xView);
            Concurrency::copy(begin(HostY), end(HostY), yView);
            Concurrency::copy(begin(HostA), end(HostA), aMat);
            status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda );
            Concurrency::copy(aMat, begin(HostA));
#ifdef LINUX
            cblas_sger( order, M, N, alpha, xSger, incX, ySger, incY, Acblas, lda);
            for(int i =0; i < lenx * leny ; i++){
                if (HostA[i] != Acblas[i]){
                    ispassed = 0;
                    cout <<" HCSGER[" << i<< "] " << HostA[i] << " does not match with CBLASSGER[" << i <<"] "<< Acblas[i] << endl;
                    break;
                }
                else
                    continue;
            }
            cout << (ispassed? "TEST PASSED": "TEST FAILED") <<endl;
#else
            cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
        }

        else{
            float *xSgerbatch = (float*)calloc( lenx * batchSize, sizeof(float));
            float *ySgerbatch = (float*)calloc( leny * batchSize, sizeof(float));
            float *ASgerbatch = (float *)calloc( lenx * leny * batchSize, sizeof(float));
#ifdef LINUX
            float *Acblasbatch = (float *)calloc( lenx * leny * batchSize, sizeof(float));
#endif
            std::vector<float> HostX_batch(lenx * batchSize);
            std::vector<float> HostY_batch(leny * batchSize);
            std::vector<float> HostA_batch(lenx * leny * batchSize);
            Concurrency::array<float> xbatchView(lenx * batchSize, xSgerbatch);
            Concurrency::array<float> ybatchView(leny * batchSize, ySgerbatch);
            Concurrency::array<float> abatchMat( lenx * leny * batchSize, ASgerbatch);
            for(int i = 0;i < lenx * batchSize;i++){
                HostX_batch[i] = rand() % 10;
                xSgerbatch[i] = HostX_batch[i];
            }
            for(int i = 0;i < leny * batchSize;i++){
                HostY_batch[i] = rand() % 15;
                ySgerbatch[i] =  HostY_batch[i];
            }
            for(int i = 0;i< lenx * leny * batchSize;i++){
                HostA_batch[i] = rand() % 25;
#ifdef LINUX
                Acblasbatch[i] = HostA_batch[i];
#endif
                ASgerbatch[i] = HostA_batch[i];
            }
            Concurrency::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
            Concurrency::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
            Concurrency::copy(begin(HostA_batch), end(HostA_batch), abatchMat);
            status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
            Concurrency::copy(abatchMat, begin(HostA_batch));
#ifdef LINUX
            for(int i = 0; i < batchSize; i++)
               cblas_sger( order, M, N, alpha, xSgerbatch + i * M, incX, ySgerbatch + i * N, incY, Acblasbatch + i * M * N, lda); 
            for(int i =0; i < lenx * leny * batchSize; i++){
               if (HostA_batch[i] != Acblasbatch[i]){
                   ispassed = 0;
                   cout <<" HCSGER[" << i<< "] " << HostA_batch[i] << " does not match with CBLASSGER[" << i <<"] "<< Acblasbatch[i] << endl;
                   break;
               }
            else 
                  continue;  
            }
            cout << (ispassed? "TEST PASSED": "TEST FAILED") <<endl;
#else
            cout << (status?"TEST FAILED":"TEST PASSED")<< endl;
#endif
         }
      }
    return 0;
}
