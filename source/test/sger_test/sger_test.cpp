#include <iostream>
#include "../../ampblaslib.h"
#include <cstdlib> 
#include "cblas.h"

using namespace std;
int main(int argc, char** argv)
{
    /* AMPBLAS implementation */
    Ampblaslibrary amp;
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
    ampblasStatus status;
    AMPBLAS_ORDER ampOrder = colMajor; 
    /* CBLAS implementation */
    enum CBLAS_ORDER order;
    order = CblasColMajor;
    if(ampOrder)
       lda = M;
    else
       lda = N;
    lenx =  1 + (M-1) * abs(incX);
    leny =  1 + (N-1) * abs(incY);
    float *xSger = (float*)calloc( lenx , sizeof(float));
    float *ySger = (float*)calloc( leny , sizeof(float));
    float *ASger = (float *)calloc( M * N , sizeof(float));
    float *Acblas = (float *)calloc( M * N , sizeof(float));

    array_view<float> xView(lenx, xSger);
    array_view<float> yView(leny, ySger);
    array_view<float> aMat( M * N, ASger);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
   
    float *xSgerbatch = (float*)calloc( lenx * batchSize, sizeof(float));
    float *ySgerbatch = (float*)calloc( leny * batchSize, sizeof(float));
    float *ASgerbatch = (float *)calloc( M * N * batchSize, sizeof(float));
    float *Acblasbatch = (float *)calloc( M * N * batchSize, sizeof(float));

    array_view<float> xbatchView(lenx * batchSize, xSgerbatch);
    array_view<float> ybatchView(leny * batchSize, ySgerbatch);
    array_view<float> abatchMat( M * N * batchSize, ASgerbatch);

    
    {
        for(int i = 0;i < M;i++){
            xView[i] = rand() % 10;
            xSger[i] = xView[i];
        }
        for(int i = 0;i < N;i++){
            yView[i] = rand() % 15;
            ySger[i] = yView[i];
        }
        for(int i = 0;i< M * N ;i++){
            aMat[i] = rand() % 25;
            Acblas[i] = aMat[i];
            ASger[i] = Acblas[i];
        }
        if(Imple_type == 1){
            status = amp.ampblas_sger(ampOrder, M , N , &alpha, xSger, xOffset, incX, ySger, yOffset, incY, ASger, aOffset, lda );
            cblas_sger( order, M, N, alpha, xSger, incX, ySger, incY, Acblas, lda);
            for(int i =0; i < M * N ; i++){
                if (ASger[i] != Acblas[i]){
                    ispassed = 0;
                    cout <<" AMPSGER[" << i<< "] " << ASger[i] << " does not match with CBLASSGER[" << i <<"] "<< Acblas[i] << endl;
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
            status = amp.ampblas_sger(accl_view, ampOrder, M , N , alpha, xView, xOffset, incX, yView, yOffset, incY, aMat, aOffset, lda );
            cblas_sger( order, M, N, alpha, xSger, incX, ySger, incY, Acblas, lda);
            for(int i =0; i < M * N ; i++){
                if (aMat[i] != Acblas[i]){
                    ispassed = 0;
                    cout <<" AMPSGER[" << i<< "] " << aMat[i] << " does not match with CBLASSGER[" << i <<"] "<< Acblas[i] << endl;
                    break;
                }
                else
                    continue;
            }
            cout << (ispassed? "TEST PASSED": "TEST FAILED") <<endl;

        }

        else{
            for(int i = 0;i < M * batchSize;i++){
                xbatchView[i] = rand() % 10;
                xSgerbatch[i] = xbatchView[i];
            }
            for(int i = 0;i < N * batchSize;i++){
                ybatchView[i] = rand() % 15;
                ySgerbatch[i] = ybatchView[i];
            }
            for(int i = 0;i< M * N * batchSize;i++){
                abatchMat[i] = rand() % 25;
                Acblasbatch[i] = abatchMat[i];
                ASgerbatch[i] = Acblasbatch[i];
            }
            status = amp.ampblas_sger(accl_view, ampOrder, M , N , alpha, xbatchView, xOffset, X_batchOffset, incX, ybatchView, yOffset, Y_batchOffset, incY, abatchMat, aOffset, A_batchOffset, lda, batchSize );
            for(int i = 0; i < batchSize; i++)
               cblas_sger( order, M, N, alpha, xSgerbatch + i * M, incX, ySgerbatch + i * N, incY, Acblasbatch + i * M * N, lda); 
            for(int i =0; i < M * N * batchSize; i++){
               if (abatchMat[i] != Acblasbatch[i]){
                   ispassed = 0;
                   cout <<" AMPSGER[" << i<< "] " << abatchMat[i] << " does not match with CBLASSGER[" << i <<"] "<< Acblasbatch[i] << endl;
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
