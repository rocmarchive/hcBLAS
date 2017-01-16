/* Example Sgemm program (NoTranspose Case) */
/* Compilation: /opt/rocm/hcc/bin/clang++ `/opt/rocm/hcc/bin/hcc-config --cxxflags --ldflags` -lhc_am -lhcblas -I../lib/include -L../build/lib/src sgemm_example.cpp */

#include <iostream>
#include <cstdlib>
#include "hcblas.h"
#include "hc_am.hpp"
#include "hcblaslib.h"

int main() {
  
  // Sgemm input variables
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  float alpha = 1;
  float beta = 1;
  long lda;
  long ldb;
  long ldc;

  // variable to hold return status of hcblas routines
  hcblasStatus_t status;

  // Create hcBlas handle object. 
  // Sets default target accelerator (id =1) and data layout as column major 
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &default_acc);

  // Variables to hold Transpose combinations
  hcblasOperation_t typeA, typeB;

  // Allocate host pointers
  float* h_A = (float*) calloc( M * K, sizeof(float));
  float* h_B = (float*) calloc( K * N, sizeof(float));
  float* h_C = (float*) calloc( M * N, sizeof(float));

  // Initialize host pointers
  for(int i = 0; i < M * K; i++) {
    h_A[i] = rand()%100;
  }
  for(int i = 0; i < K * N;i++) {
    h_B[i] = rand() % 15;
  }
  for(int i = 0; i < M * N;i++) {
    h_C[i] = rand() % 25;
  }
 
  // Allocate device pointers
  float* d_A = hc::am_alloc(sizeof(float) * M * K, handle->currentAccl, 0);
  float* d_B = hc::am_alloc(sizeof(float) * K * N, handle->currentAccl, 0);
  float* d_C = hc::am_alloc(sizeof(float) * M * N, handle->currentAccl, 0);

  
  // Initialze device pointers using hcblasSetMatrix utility
  status = hcblasSetMatrix(handle, M, K, sizeof(float), h_A, M, d_A, K);
  if(status != HCBLAS_STATUS_SUCCESS) {
     printf("Error : Data download failure\n");
     exit(1);
  }
  status = hcblasSetMatrix(handle, K, N, sizeof(float), h_B, K, d_A, N);
  if(status != HCBLAS_STATUS_SUCCESS) {
     printf("Error : Data download failure\n");
     exit(1);
  }
  status = hcblasSetMatrix(handle, M, N, sizeof(float), h_C, M, d_C, N);
  if(status != HCBLAS_STATUS_SUCCESS) {
     printf("Error : Data download failure\n");
     exit(1);
  }

  // NoTransA and NoTransB */           
  typeA = HCBLAS_OP_N;
  typeB = HCBLAS_OP_N;

  // Column major Settings */
  lda = M; ldb = K ; ldc = M;

  // Invoke Sgemm Blas routine
  status = hcblasSgemm(handle, typeA, typeB, M, N, K, &alpha, d_A, lda, d_B, ldb, &beta, d_C, ldc);
  if(status != HCBLAS_STATUS_SUCCESS) {
     printf("Error : Function invocation failure\n");
     exit(1);
  }

  // Get the device output d_C back to host
  status = hcblasGetMatrix(handle, M, N, sizeof(float), d_C, M, h_C, N);;
  if(status != HCBLAS_STATUS_SUCCESS) {
     printf("Error : Upload failure\n");
     exit(1);
  }

  // h_C now contains the results. The user can now print or use h_c for further computation

  // Deallocate the resources
  
  // Destroy the handle
  status = hcblasDestroy(&handle);
  if(status != HCBLAS_STATUS_SUCCESS) {
     printf("Error : Handle deallocation failure\n");
     exit(1);
  }

  //Free host resources
  free(h_A);
  free(h_B);
  free(h_C);
 
  // Release device resources 
  hc::am_free(d_A);
  hc::am_free(d_B);
  hc::am_free(d_C);
}

