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
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type I - Inputs and Outputs are host float pointers */

        if(Imple_type == 1) {
            for(int i = 0;i < lenx;i++) {
                xSger[i] = rand() % 10;
            }
            for(int i = 0;i < leny;i++) {
                ySger[i] = rand() % 15;
            }
#ifdef PROFILE
            for(int iter=0; iter<10; iter++) {
#endif
            for(int i = 0;i< lenx * leny ;i++) {
                ASger[i] = rand() % 25;
#ifdef LINUX
                Acblas[i] = ASger[i];
#endif
            }
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
            if(!ispassed) cout << "TEST FAILED" << endl; 
#else
            if(status) cout << "TEST FAILED" << endl; 
#endif
#ifdef PROFILE
            }
#endif
        }

/* Implementation type II - Inputs and Outputs are HC++ float array_view containers */

        else if(Imple_type == 2){
            array_view<float> xView(lenx, xSger);
            array_view<float> yView(leny, ySger);
            array_view<float> aMat( M * N, ASger);
            for(int i = 0;i < lenx;i++) {
                xView[i] = rand() % 10;
                xSger[i] = xView[i];
            }
            for(int i = 0;i < leny;i++) {
                yView[i] = rand() % 15;
                ySger[i] = yView[i];
            }
#ifdef PROFILE
            for(int iter = 0; iter < 10; iter++) {
#endif
            for(int i = 0;i< lenx * leny ;i++) {
                aMat[i] = rand() % 25;
#ifdef LINUX
                Acblas[i] = aMat[i];
#endif
            }
            status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda);
#ifdef LINUX
            cblas_sger( order, M, N, alpha, xSger, incX, ySger, incY, Acblas, lda);
            for(int i =0; i < lenx * leny ; i++){
                if (aMat[i] != Acblas[i]){
                    ispassed = 0;
                    cout <<" HCSGER[" << i<< "] " << aMat[i] << " does not match with CBLASSGER[" << i <<"] "<< Acblas[i] << endl;
                    break;
                }
                else
                    continue;
            }
            if(!ispassed) cout << "TEST FAILED" << endl; 
#else
            if(status) cout << "TEST FAILED" << endl; 
#endif
#ifdef PROFILE
            }
#endif
        }

/* Implementation type III - Inputs and Outputs are HC++ float array_view containers with batch processing */

        else if(Imple_type == 3){
            float *xSgerbatch = (float*)calloc( lenx * batchSize, sizeof(float));
            float *ySgerbatch = (float*)calloc( leny * batchSize, sizeof(float));
            float *ASgerbatch = (float *)calloc( lenx * leny * batchSize, sizeof(float));
#ifdef LINUX
            float *Acblasbatch = (float *)calloc( lenx * leny * batchSize, sizeof(float));
#endif
            array_view<float> xbatchView(lenx * batchSize, xSgerbatch);
            array_view<float> ybatchView(leny * batchSize, ySgerbatch);
            array_view<float> abatchMat( lenx * leny * batchSize, ASgerbatch);
            for(int i = 0;i < lenx * batchSize;i++) {
                xbatchView[i] = rand() % 10;
                xSgerbatch[i] = xbatchView[i];
            }
            for(int i = 0;i < leny * batchSize;i++) {
                ybatchView[i] = rand() % 15;
                ySgerbatch[i] = ybatchView[i];
            }
#ifdef PROFILE
            for(int iter = 0; iter < 10; iter++) {
#endif
            for(int i = 0;i< lenx * leny * batchSize;i++) {
                abatchMat[i] = rand() % 25;
#ifdef LINUX
                Acblasbatch[i] = abatchMat[i];
#endif
                ASgerbatch[i] = abatchMat[i];
            }
            status = hc.hcblas_sger(accl_view, hcOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
#ifdef LINUX
            for(int i = 0; i < batchSize; i++)
               cblas_sger( order, M, N, alpha, xSgerbatch + i * M, incX, ySgerbatch + i * N, incY, Acblasbatch + i * M * N, lda);
            for(int i =0; i < lenx * leny * batchSize; i++){
               if (abatchMat[i] != Acblasbatch[i]){
                   ispassed = 0;
                   cout <<" HCSGER[" << i<< "] " << abatchMat[i] << " does not match with CBLASSGER[" << i <<"] "<< Acblasbatch[i] << endl;
                   break;
               }
            else
                  continue;
            }
            if(!ispassed) cout << "TEST FAILED" << endl; 
#else
            if(status) cout << "TEST FAILED" << endl; 
#endif
#ifdef PROFILE
         }
#endif
      }

/* Implementation type IV - Inputs and Outputs are HC++ float array containers */

        else if(Imple_type == 4) {
            std::vector<float> HostX(lenx);
            std::vector<float> HostY(leny);
            std::vector<float> HostA(lenx * leny);
            Concurrency::array<float> xView(lenx, xSger);
            Concurrency::array<float> yView(leny, ySger);
            Concurrency::array<float> aMat( lenx * leny, ASger);
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
#ifdef LINUX
                Acblas[i] = HostA[i];
#endif
            }
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
            if(!ispassed) cout << "TEST FAILED" << endl; 
#else
            if(status) cout << "TEST FAILED" << endl; 
#endif
#ifdef PROFILE
            }
#endif
        }

/* Implementation type V - Inputs and Outputs are HC++ float array containers with batch processing */

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
#ifdef PROFILE
            for(int iter = 0; iter < 10; iter++) {
#endif
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
            if(!ispassed) cout << "TEST FAILED" << endl; 
#else
            if(status) cout << "TEST FAILED" << endl; 
#endif
#ifdef PROFILE
            }
#endif
      }
    return 0;
}
