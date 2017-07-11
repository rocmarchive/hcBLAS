/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/* Example Sgemm program (NoTranspose Case) */
/* Compilation: /opt/rocm/hcc/bin/clang++ `/opt/rocm/hcc/bin/hcc-config
 * --cxxflags --ldflags` -lhc_am -lhcblas -I../lib/ -L../build/lib/src
 * sgemm_example.cpp */

#include "include/hcblas.h"
#include "include/hcblaslib.h"
#include <cstdlib>
#include <hc_am.hpp>
#include <iostream>

int main() {
  // Sgemm input variables
  int M = 123;
  int N = 78;
  int K = 23;
  int incx = 1, incy = 1;
  float alpha = 1;
  float beta = 1;
  __int64_t lda;
  __int64_t ldb;
  __int64_t ldc;

  // variable to hold return status of hcblas routines
  hcblasStatus_t status;

  // Create hcBlas handle object.
  // Sets default target accelerator (id =1) and data layout as column major
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);

  // Variables to hold Transpose combinations
  hcblasOperation_t typeA, typeB;

  // Allocate host pointers
  float* h_A = reinterpret_cast<float*>(calloc(M * K, sizeof(float)));
  float* h_B = reinterpret_cast<float*>(calloc(K * N, sizeof(float)));
  float* h_C = reinterpret_cast<float*>(calloc(M * N, sizeof(float)));

  // Initialize host pointers
  for (int i = 0; i < M * K; i++) {
    h_A[i] = rand_r(&global_seed) % 100;
  }
  for (int i = 0; i < K * N; i++) {
    h_B[i] = rand_r(&global_seed) % 15;
  }
  for (int i = 0; i < M * N; i++) {
    h_C[i] = rand_r(&global_seed) % 25;
  }

  // Allocate device pointers
  float* d_A = hc::am_alloc(sizeof(float) * M * K, handle->currentAccl, 0);
  float* d_B = hc::am_alloc(sizeof(float) * K * N, handle->currentAccl, 0);
  float* d_C = hc::am_alloc(sizeof(float) * M * N, handle->currentAccl, 0);

  // Initialze device pointers using hcblasSetMatrix utility
  status = hcblasSetMatrix(handle, M, K, sizeof(float), h_A, M, d_A, K);
  if (status != HCBLAS_STATUS_SUCCESS) {
    printf("Error : Data download failure\n");
    exit(1);
  }
  status = hcblasSetMatrix(handle, K, N, sizeof(float), h_B, K, d_B, N);
  if (status != HCBLAS_STATUS_SUCCESS) {
    printf("Error : Data download failure\n");
    exit(1);
  }
  status = hcblasSetMatrix(handle, M, N, sizeof(float), h_C, M, d_C, N);
  if (status != HCBLAS_STATUS_SUCCESS) {
    printf("Error : Data download failure\n");
    exit(1);
  }

  // NoTransA and NoTransB */
  typeA = HCBLAS_OP_N;
  typeB = HCBLAS_OP_N;

  // Column major Settings */
  lda = M;
  ldb = K;
  ldc = M;

  // Invoke Sgemm Blas routine
  status = hcblasSgemm(handle, typeA, typeB, M, N, K, &alpha, d_A, lda, d_B,
                       ldb, &beta, d_C, ldc);
  if (status != HCBLAS_STATUS_SUCCESS) {
    printf("Error : Function invocation failure\n");
    exit(1);
  }

  // Get the device output d_C back to host
  status = hcblasGetMatrix(handle, M, N, sizeof(float), d_C, M, h_C, N);
  if (status != HCBLAS_STATUS_SUCCESS) {
    printf("Error : Upload failure\n");
    exit(1);
  }

  // h_C now contains the results. The user can now print or use h_c for further
  // computation

  // Deallocate the resources

  // Destroy the handle
  status = hcblasDestroy(&handle);
  if (status != HCBLAS_STATUS_SUCCESS) {
    printf("Error : Handle deallocation failure\n");
    exit(1);
  }

  // Free host resources
  free(h_A);
  free(h_B);
  free(h_C);

  // Release device resources
  hc::am_free(d_A);
  hc::am_free(d_B);
  hc::am_free(d_C);
}

