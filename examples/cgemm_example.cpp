/* Example Cgemm program (NoTranspose Case) */
/* Compilation: /opt/rocm/hcc/bin/clang++ `/opt/rocm/hcc/bin/hcc-config --cxxflags --ldflags` -lhc_am -lhcblas -I../lib/include -L../build/lib/src cgemm_example.cpp */

#include <iostream>
#include <cstdlib>
#include "hcblas.h"
#include "hc_am.hpp"
#include "hcblaslib.h"

int main() {
  
  // Cgemm input variables
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  long lda;
  long ldb;
  long ldc;
  hcComplex alpha, beta;
  alpha.x = 1;
  alpha.y = 1;
  beta.x = 1;
  beta.y = 1;

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
  hcComplex* h_A = (hcComplex*) calloc( M * K , sizeof(hcComplex));
  hcComplex* h_B = (hcComplex*) calloc( K * N , sizeof(hcComplex));
  hcComplex* h_C = (hcComplex*) calloc( M * N , sizeof(hcComplex));

  int k=0;
  // Initialize host pointers
  for(int i = 0; i < M * K; i++) {
    h_A[i].x = rand() % 10;
    h_A[i].y = rand() % 20;
  }
  k = 0;
  for(int i = 0; i < K * N;i++) {
    h_B[i].x = rand() % 15;
    h_B[i].y = rand() % 25;
  }
  k = 0;
  for(int i = 0; i < M * N;i++) {
    h_C[i].x = rand() % 18;
    h_C[i].y = rand() % 28;
  }
 
  // Allocate device pointers
  hcComplex* d_A = hc::am_alloc(sizeof(hcComplex) * M * K, handle->currentAccl, 0);
  hcComplex* d_B = hc::am_alloc(sizeof(hcComplex) * K * N, handle->currentAccl, 0);
  hcComplex* d_C = hc::am_alloc(sizeof(hcComplex) * M * N, handle->currentAccl, 0);

  
  // Initialze device pointers using hcblasSetMatrix utility
  status = hcblasSetMatrix(handle, M, K, sizeof(hcComplex), h_A, M, d_A, K);
  if(status != HCBLAS_STATUS_SUCCESS) {
     printf("Error : Data download failure\n");
     exit(1);
  }
  status = hcblasSetMatrix(handle, K, N, sizeof(hcComplex), h_B, K, d_B, N);
  if(status != HCBLAS_STATUS_SUCCESS) {
     printf("Error : Data download failure\n");
     exit(1);
  }
  status = hcblasSetMatrix(handle, M, N, sizeof(hcComplex), h_C, M, d_C, N);
  if(status != HCBLAS_STATUS_SUCCESS) {
     printf("Error : Data download failure\n");
     exit(1);
  }

  // NoTransA and NoTransB */           
  typeA = HCBLAS_OP_N;
  typeB = HCBLAS_OP_N;

  // Column major Settings */
  lda = M; ldb = K ; ldc = M;

  // Invoke Cgemm Blas routine
  status = hcblasCgemm(handle, typeA, typeB, M, N, K, &alpha, d_A, lda, d_B, ldb, &beta, d_C, ldc);
  if(status != HCBLAS_STATUS_SUCCESS) {
     printf("Error : Function invocation failure\n");
     exit(1);
  }

  // Get the device output d_C back to host
  status = hcblasGetMatrix(handle, M, N, sizeof(hcComplex), d_C, M, h_C, N);;
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

