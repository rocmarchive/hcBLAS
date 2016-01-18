#include <iostream>
#include "hcblaslib.h"
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
    /* CBLAS implementation */
    bool ispassed = 1;
    enum CBLAS_ORDER order;
    order = CblasColMajor;
    float *Acblas = (float *)calloc( lenx * leny , sizeof(float));
    float *xSger = (float*)calloc( lenx , sizeof(float));
    float *ySger = (float*)calloc( leny , sizeof(float));
    float *ASger = (float *)calloc( lenx * leny , sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type I - Inputs and Outputs are HCC float array containers */

        if(Imple_type == 1) {
            std::vector<float> HostX(lenx);
            std::vector<float> HostY(leny);
            std::vector<float> HostA(lenx * leny);
            hc::array<float> xView(lenx, xSger);
            hc::array<float> yView(leny, ySger);
            hc::array<float> aMat( lenx * leny, ASger);
            for(int i = 0;i < lenx;i++) {
                HostX[i] = rand() % 10;
                xSger[i] = HostX[i];
            }
            for(int i = 0;i < leny;i++) {
                HostY[i] = rand() % 15;
                ySger[i] = HostY[i];
            }
#ifdef PROFILE
            for(int iter=0; iter<10; iter++) {
#endif
            for(int i = 0;i< lenx * leny ;i++) {
                HostA[i] = rand() % 25;
                Acblas[i] = HostA[i];
            }
            hc::copy(begin(HostX), end(HostX), xView);
            hc::copy(begin(HostY), end(HostY), yView);
            hc::copy(begin(HostA), end(HostA), aMat);
            status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda );
            hc::copy(aMat, begin(HostA));
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
            if(!ispassed) cout << "TEST FAILED" << endl; 
            if(status) cout << "TEST FAILED" << endl; 
#ifdef PROFILE
            }
#endif
        }

/* Implementation type II - Inputs and Outputs are HCC float array containers with batch processing */

        else{
            float *xSgerbatch = (float*)calloc( lenx * batchSize, sizeof(float));
            float *ySgerbatch = (float*)calloc( leny * batchSize, sizeof(float));
            float *ASgerbatch = (float *)calloc( lenx * leny * batchSize, sizeof(float));
            float *Acblasbatch = (float *)calloc( lenx * leny * batchSize, sizeof(float));
            std::vector<float> HostX_batch(lenx * batchSize);
            std::vector<float> HostY_batch(leny * batchSize);
            std::vector<float> HostA_batch(lenx * leny * batchSize);
            hc::array<float> xbatchView(lenx * batchSize, xSgerbatch);
            hc::array<float> ybatchView(leny * batchSize, ySgerbatch);
            hc::array<float> abatchMat( lenx * leny * batchSize, ASgerbatch);
            for(int i = 0;i < lenx * batchSize;i++){
                HostX_batch[i] = rand() % 10;
                xSgerbatch[i] = HostX_batch[i];
            }
            for(int i = 0;i < leny * batchSize;i++){
                HostY_batch[i] = rand() % 15;
                ySgerbatch[i] =  HostY_batch[i];
            }
#ifdef PROFILE
            for(int iter = 0; iter < 10; iter++) {
#endif
            for(int i = 0;i< lenx * leny * batchSize;i++){
                HostA_batch[i] = rand() % 25;
                Acblasbatch[i] = HostA_batch[i];
                ASgerbatch[i] = HostA_batch[i];
            }
            hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
            hc::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
            hc::copy(begin(HostA_batch), end(HostA_batch), abatchMat);
            status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
            hc::copy(abatchMat, begin(HostA_batch));
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
            if(!ispassed) cout << "TEST FAILED" << endl; 
            if(status) cout << "TEST FAILED" << endl; 
#ifdef PROFILE
            }
#endif
      }
    return 0;
}
