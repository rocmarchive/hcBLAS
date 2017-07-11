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

/* Example Ddot program */
/* Compilation: /opt/rocm/hcc/bin/clang++ `/opt/rocm/hcc/bin/hcc-config
 * --cxxflags --ldflags` -lhc_am -lhcblas -I../lib/ -L../build/lib/src
 * ddot_example.cpp */

#include "include/hcblas.h"
#include "include/hcblaslib.h"
#include <cstdlib>
#include <hc_am.hpp>
#include <iostream>

int main() {
  // variable to hold return status of hcblas routines
  hcblasStatus_t status;

  // Create hcBlas handle object.
  // Sets default target accelerator (id =1) and data layout as column major
  hcblasHandle_t handle = NULL;
  hc::accelerator default_acc;
  hc::accelerator_view av = default_acc.get_default_view();
  // Passing a Null handle and default accelerator to the API
  status = hcblasCreate(&handle, &av);

  // Ddot input variables
  int n = 123;
  int incx = 1;
  int incy = 1;
  __int64_t lenx = 1 + (n - 1) * abs(incx);
  __int64_t leny = 1 + (n - 1) * abs(incy);
  double result;

  // Allocate host pointers
  // host input
  double* h_X = reinterpret_cast<double*>(calloc(lenx, sizeof(double)));
  double* h_Y = reinterpret_cast<double*>(calloc(leny, sizeof(double)));

  // Initialize host pointers
  for (int i = 0; i < lenx; i++) {
    h_X[i] = rand_r(&global_seed) % 10;
  }
  for (int i = 0; i < leny; i++) {
    h_Y[i] = rand_r(&global_seed) % 15;
  }

  // Allocate device pointers
  double* d_X = hc::am_alloc(sizeof(double) * lenx, handle->currentAccl, 0);
  double* d_Y = hc::am_alloc(sizeof(double) * leny, handle->currentAccl, 0);

  // Initialze device pointers using hcblasSetVector utility
  status = hcblasSetVector(handle, lenx, sizeof(double), h_X, incx, d_X, incx);
  if (status != HCBLAS_STATUS_SUCCESS) {
    printf("Error : Data download failure\n");
    exit(1);
  }
  status = hcblasSetVector(handle, leny, sizeof(double), h_Y, incy, d_Y, incy);
  if (status != HCBLAS_STATUS_SUCCESS) {
    printf("Error : Data download failure\n");
    exit(1);
  }

  // Invoke Ddot Blas routine
  status = hcblasDdot(handle, n, d_X, incx, d_Y, incy, &result);
  if (status != HCBLAS_STATUS_SUCCESS) {
    printf("Error : Function invocation failure\n");
    exit(1);
  }

  // Get the device output d_Y back to host
  status = hcblasGetVector(handle, leny, sizeof(double), d_Y, incy, h_Y, incy);
  if (status != HCBLAS_STATUS_SUCCESS) {
    printf("Error : Upload failure\n");
    exit(1);
  }

  // h_Y now contains the results. The user can now print or use h_Y for further
  // computation

  // Deallocate the resources

  // Destroy the handle
  status = hcblasDestroy(&handle);
  if (status != HCBLAS_STATUS_SUCCESS) {
    printf("Error : Handle deallocation failure\n");
    exit(1);
  }

  // Free host resources
  free(h_X);
  free(h_Y);

  // Release device resources
  hc::am_free(d_X);
  hc::am_free(d_Y);
}
